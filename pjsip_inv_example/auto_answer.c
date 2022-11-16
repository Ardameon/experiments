#include <pjsip.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>
#include <pjsip_ua.h>
#include <pjsip_simple.h>
#include <pjlib-util.h>
#include <pjlib.h>

#define THIS_FILE             "Auto-answer"

#define WAV_FILE              "track.wav"
#define TONE_CNT              2
#define CLOCK_RATE            16000
#define CHANNEL_COUNT         1
#define PTIME                 20
#define SAMPLES_PER_FRAME     (CLOCK_RATE*PTIME/1000)
#define BITS_PER_SAMPLE       16
#define TONE_FREQ             425
#define TONE_ON               1000
#define TONE_OFF              4000
#define NUMBER_OF_EXTENSIONS  3
#define DEFAULT_CALL_DURATION 15000 /* 15 seconds before auto hangup */
#define UDP_PORT              5062
#define RTP_PORT              4000
#define BUFFER_SIZE           256	
#define AUTO_ANSWER_DELAY     1000 /* 1 second delay after SIP 180 Ringing */  
#define AUTO_ANSWER_INTERVAL  10   /* 10 seconds interval */

#define AF                          pj_AF_INET() /* Change to pj_AF_INET6() for IPv6 */
#define AUTO_ANSWER_MAX_CALLS       20
#define AUTO_ANSWER_MEDIA_CNT       1
#define AUTO_ANSWER_WAV_PLAYERS	    1
#define AUTO_ANSWER_TONE_PORTS	    2
#define AUTO_ANSWER_NULL_PORTS      1
#define AUTO_ANSWER_MAX_CONF_PORTS  (AUTO_ANSWER_MAX_CALLS + \
                                     AUTO_ANSWER_WAV_PLAYERS + \
                                     AUTO_ANSWER_TONE_PORTS + \
				     AUTO_ANSWER_NULL_PORTS)

#define AUTO_ANSWER_POOL_LEN        1000 /* Memory block for application */
#define AUTO_ANSWER_POOL_INC        1000 /* Memory increment for application */

enum {									 
   AUTO_ANSWER_INVALID_ID = -1
};
			
typedef int status_t;
				 
typedef enum {
    eAUTO_ANSWER_DST_TYPE_NONE,
    eAUTO_ANSWER_DST_TYPE_LONG_TONE,   /* Continous tone with 425Hz frequency will be provided */
    eAUTO_ANSWER_DST_TYPE_SHORT_TONE,  /* Repeated tone with 425Hz frequency will be provided */
    eAUTO_ANSWER_DST_TYPE_WAV_FILE     /* WAV file will be played when call will be established */	
} auto_answer_dst_type_e;

typedef enum {
    eAUTO_ANSWER_TONE_TYPE_LONG_TONE =  0,   /* Continous tone with 425Hz frequency */
    eAUTO_ANSWER_TONE_TYPE_SHORT_TONE = 1    /* Repeated tone with 425Hz frequency */
} auto_answer_tone_type_e;

/* Struct to store available extentions for providing different playbacks */
typedef struct {
    pj_str_t auto_answer_number;
    auto_answer_dst_type_e auto_answer_type;
} auto_answer_extension_t;

typedef struct {
   unsigned               call_id;
   unsigned               media_id;
   pjmedia_stream         *stream;      /* Call's audio stream */
   pjmedia_port           *media_port;  /* The media port */
   unsigned               conf_slot;
   pjmedia_transport      *transport;
   pjmedia_transport_info tpinfo;
   pjmedia_sock_info      sock_info;
} auto_answer_call_media_t;

typedef struct {
    unsigned                   call_id;
    pjsip_inv_session	       *inv;
    auto_answer_call_media_t   media[AUTO_ANSWER_MEDIA_CNT];
    auto_answer_dst_type_e     dst_type;	
    pj_timer_entry             auto_hangup_timer;
    pj_timer_entry             auto_answer_delay;
} auto_answer_call_t;

/* Struct for application configuration settings */
typedef struct { 
    pj_caching_pool         cp;	                        /* Global pool factory */
    pj_pool_t               *pool;
    pj_mutex_t              *mutex;                     /* Mutex to sync call array */
    pj_str_t                local_uri;
    pjsip_endpoint          *sip_endpt;	                /* Global endpoint */
    pjmedia_endpt           *med_endpt;                 /* Media endpoint */
    pjmedia_conf            *conf;                      /* Conference bridge */
    unsigned                wav_slot;                   /* Slot on conference bridge for WAV file play */
    pjmedia_port            *wav_port;
    pj_str_t                wav_file;
    pjmedia_tone_desc       tone[TONE_CNT];
    unsigned	            tone_slots[TONE_CNT];
    pjmedia_master_port     *master_port;
    pjmedia_port            *null_port;
    unsigned                call_duration;              /* call duratiom in ms */
    auto_answer_call_t      call[AUTO_ANSWER_MAX_CALLS];
    unsigned                max_calls;
    pj_thread_t             *worker_thread;
    pj_bool_t               quit;
} app_instance_t; 

/* Notification on incoming messages */
static pj_bool_t logger_on_rx_msg(pjsip_rx_data *rdata);

/* Notification on outgoing messages */
static pj_status_t logger_on_tx_msg(pjsip_tx_data *tdata);

/* Hangup all calls */
static void auto_answer_hangup_all_calls(void);

/* Auto hangup timer callback */
static void auto_answer_hangup_timer_cb(pj_timer_heap_t *th, pj_timer_entry *entry);

/* Call back to be called to handle auto answer delay */
static void auto_answer_timer_cb(pj_timer_heap_t *th, pj_timer_entry *entry);

/* Display error and exit application */
static void auto_answer_error(const char *title, pj_status_t status);

/* Callback to be called to handle incoming requests outside dialogs */
static pj_bool_t on_rx_request(pjsip_rx_data *rdata);

/* Callback to be called when invite session's state has changed: */
static void call_on_state_changed(pjsip_inv_session *inv, pjsip_event *e);

/* Callback to be called when SDP negotiation is done in the call: */
static void call_on_media_update(pjsip_inv_session *inv, pj_status_t status);


/* Application instance */
app_instance_t app;

/* Array of different destination extensions and their type */
auto_answer_extension_t auto_answer_dst[NUMBER_OF_EXTENSIONS];

/* This is a PJSIP module to be registered by application to handle
 * incoming requests outside any dialogs/transactions. The main purpose
 * here is to handle incoming INVITE request message, where we will
 * create a dialog and INVITE session for it.
 */
static pjsip_module mod_auto_answer =
{
    NULL, NULL,	                                /* prev, next       */
    { "mod-auto_answer", 15 },                  /* Name             */
    -1,				                /* Id               */
    PJSIP_MOD_PRIORITY_APPLICATION,             /* Priority         */
    NULL,			                /* load()           */
    NULL,			                /* start()          */
    NULL,			                /* stop()           */
    NULL,			                /* unload()         */
    &on_rx_request,                             /* on_rx_request()  */
    NULL,			                /* on_rx_response() */
    NULL,			                /* on_tx_request()  */
    NULL,			                /* on_tx_response() */
    NULL,			                /* on_tsx_state()   */
};

/* The module instance. */
static pjsip_module msg_logger = 
{
    NULL, NULL,	                                         /* prev, next       */
    { "mod-msg-log", 11 },                               /* Name             */
    -1,					                 /* Id               */
    PJSIP_MOD_PRIORITY_TRANSPORT_LAYER-1,                /* Priority	     */
    NULL,				                 /* load()           */
    NULL,				                 /* start()          */
    NULL,				                 /* stop()           */
    NULL,				                 /* unload()         */
    &logger_on_rx_msg,                                   /* on_rx_request()  */
    &logger_on_rx_msg,                                   /* on_rx_response() */
    &logger_on_tx_msg,                                   /* on_tx_request()  */
    &logger_on_tx_msg,                                   /* on_tx_response() */
    NULL,				                 /* on_tsx_state()   */
};

/* Notification on incoming messages */
static pj_bool_t logger_on_rx_msg(pjsip_rx_data *rdata)
{
    PJ_LOG(4,(THIS_FILE, "RX %d bytes %s from %s:%d:\n"
			 "%s\n"
			 "--end msg--",
			 rdata->msg_info.len,
			 pjsip_rx_data_get_info(rdata),
			 rdata->pkt_info.src_name,
			 rdata->pkt_info.src_port,
			 rdata->msg_info.msg_buf));
    
    /* Always return false, otherwise messages will not get processed! */
    return PJ_FALSE;
}

/* Notification on outgoing messages */
static pj_status_t logger_on_tx_msg(pjsip_tx_data *tdata)
{
    
    /* Important note:
     *	tp_info field is only valid after outgoing messages has passed
     *	transport layer. So don't try to access tp_info when the module
     *	has lower priority than transport layer.
     */

    PJ_LOG(4,(THIS_FILE, "TX %d bytes %s to %s:%d:\n"
			 "%s\n"
			 "--end msg--",
			 (tdata->buf.cur - tdata->buf.start),
			 pjsip_tx_data_get_info(tdata),
			 tdata->tp_info.dst_name,
			 tdata->tp_info.dst_port,
			 tdata->buf.start));

    /* Always return success, otherwise message will not get sent! */
    return PJ_SUCCESS;
}

static status_t worker_proc(void *arg)
{
    PJ_UNUSED_ARG(arg);
    
    while (!app.quit)
    {
        pj_time_val interval = { 0, AUTO_ANSWER_INTERVAL };
        pjsip_endpt_handle_events(app.sip_endpt, & interval);
    }
    
    return PJ_SUCCESS;
}

/* Configure available extentions and associated playbacks */
static void auto_answer_dst_config(void)
{
    auto_answer_dst[0] = (auto_answer_extension_t){pj_str("1111"), eAUTO_ANSWER_DST_TYPE_WAV_FILE};
    auto_answer_dst[1] = (auto_answer_extension_t){pj_str("2222"), eAUTO_ANSWER_DST_TYPE_LONG_TONE}; 
    auto_answer_dst[2] = (auto_answer_extension_t){pj_str("3333"), eAUTO_ANSWER_DST_TYPE_SHORT_TONE};
}

/* Configure tone types */
static void auto_answer_tone_config(void)
{
    pj_bzero(&app.tone, sizeof(app.tone));

    app.tone[eAUTO_ANSWER_TONE_TYPE_LONG_TONE].freq1 = TONE_FREQ;
    app.tone[eAUTO_ANSWER_TONE_TYPE_LONG_TONE].freq2 = TONE_FREQ;
    app.tone[eAUTO_ANSWER_TONE_TYPE_LONG_TONE].on_msec = TONE_ON;
    app.tone[eAUTO_ANSWER_TONE_TYPE_LONG_TONE].off_msec = 0;
	
    app.tone[eAUTO_ANSWER_TONE_TYPE_SHORT_TONE].freq1 = TONE_FREQ;
    app.tone[eAUTO_ANSWER_TONE_TYPE_SHORT_TONE].freq2 = TONE_FREQ;
    app.tone[eAUTO_ANSWER_TONE_TYPE_SHORT_TONE].on_msec = TONE_ON;
    app.tone[eAUTO_ANSWER_TONE_TYPE_SHORT_TONE].off_msec = TONE_OFF;
		
}

/* Init calls array */
static void auto_answer_call_array_init(void)
{
    auto_answer_call_t *call = NULL;
    auto_answer_call_media_t *media = NULL;
	
    for (unsigned i = 0; i < app.max_calls; ++i)
    {
        call = &app.call[i];
    	pj_bzero(call, sizeof(*call));
        call->call_id = i;
        call->dst_type = eAUTO_ANSWER_DST_TYPE_NONE;
	
        pj_timer_entry_init(&call->auto_hangup_timer, PJ_FALSE, (void*)(pj_size_t)i, &auto_answer_hangup_timer_cb);
        pj_timer_entry_init(&call->auto_answer_delay, PJ_FALSE, (void*)(pj_size_t)i, &auto_answer_timer_cb);
	
        for (unsigned j = 0; j < AUTO_ANSWER_MEDIA_CNT; ++j) 
        {
            media = &call->media[j];
            media->call_id = i;
            media->media_id = j;
            media->stream = NULL;
            media->media_port = NULL;
            media->conf_slot = AUTO_ANSWER_INVALID_ID;
            media->transport = NULL;
            pjmedia_transport_info_init(&media->tpinfo);
            pj_bzero(&media->sock_info, sizeof(media->sock_info));
        }
    }
}

/* Set default values for application instance */
static void auto_answer_app_init(void)
{
    pj_bzero(&app, sizeof(app));

    app.pool = NULL;    
    app.mutex = NULL;
	
    app.local_uri = pj_str("");
	
    app.sip_endpt = NULL;
    app.med_endpt = NULL;
    app.conf = NULL;
   
    app.wav_slot = AUTO_ANSWER_INVALID_ID;
    app.wav_port = NULL;
    app.wav_file = pj_str(WAV_FILE);
	
    for (int i = 0; i < TONE_CNT; ++i) {
        app.tone_slots[i] = AUTO_ANSWER_INVALID_ID;
    }

    pjmedia_port *null_port = NULL;
    pjmedia_port *master_port = NULL;
    
    app.call_duration = DEFAULT_CALL_DURATION;
    app.max_calls = AUTO_ANSWER_MAX_CALLS;
	
    auto_answer_tone_config();
    auto_answer_dst_config(); 
    
    app.quit = PJ_FALSE;
    app.worker_thread = NULL;
}

/* SIP stack initialization */
static pj_status_t auto_answer_sip_stack_init()
{
    pj_status_t status = PJ_SUCCESS;
    int af = AF;
    pjsip_transport *tp = NULL;
    pj_sockaddr host_addr;
    pjsip_inv_callback inv_cb;
    
    pj_bzero(&host_addr, sizeof(host_addr));
    pj_bzero(&inv_cb, sizeof(inv_cb));

    /* Create the endpoint */
    status = pjsip_endpt_create(&app.cp.factory, pj_gethostname()->ptr, &app.sip_endpt);  
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error in pjsip_endpt_create()", status);
        goto _on_return;
    }

    /* Add UDP transport. */	
    pj_sockaddr_init(af, &host_addr, NULL, (pj_uint16_t)UDP_PORT);

    if (af == pj_AF_INET()) 
        status = pjsip_udp_transport_start(app.sip_endpt, &host_addr.ipv4, NULL, 1, &tp);
    else if (af == pj_AF_INET6()) 
        status = pjsip_udp_transport_start6(app.sip_endpt, &host_addr.ipv6, NULL, 1, &tp);
    else 
        status = PJ_EAFNOTSUP;

    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to start UDP transport", status);
        goto _on_return;
    }
    
    PJ_LOG(3,(THIS_FILE, "SIP UDP listening on %.*s:%d", (int)tp->local_name.host.slen, 
                                                         tp->local_name.host.ptr,
                                                         tp->local_name.port));

    /* Initialize transaction layer */
    status = pjsip_tsx_layer_init_module(app.sip_endpt);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to initialize transaction layer", status);
        goto _on_return;
    }
    
    /*  Initialize UA layer */
    status = pjsip_ua_init_module(app.sip_endpt, NULL);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to initialize UA layer", status);
        goto _on_return;
    }
    
    /* Initialize the callback for INVITE session */
    inv_cb.on_state_changed = &call_on_state_changed;
    inv_cb.on_media_update = &call_on_media_update;

    /* Initialize invite session module */
    status = pjsip_inv_usage_init(app.sip_endpt, &inv_cb);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to initialize invite session module", status);
        goto _on_return;
    }
    
    /* Register autoanswer module to receive incoming requests and to log */
    status = (pjsip_endpt_register_module(app.sip_endpt, &mod_auto_answer)
              || pjsip_endpt_register_module(app.sip_endpt, &msg_logger));
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to register auto_answer or msg_logger module", status);
        goto _on_return;
    }
    
    status = pj_thread_create(app.pool, "auto_answer", &worker_proc, NULL, 0, 0, &app.worker_thread);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to create thread", status);
        goto _on_return;
    }
  
_on_return:
    return status;
}

/* Init media transport for all calls */
static pj_status_t auto_answer_media_transport_init(void)
{
    pj_status_t status = PJ_SUCCESS;
    pj_uint16_t rtp_port = (pj_uint16_t)RTP_PORT;
    
    for (unsigned i = 0; i < app.max_calls; ++i) 
    {
        /* Create transport for each media in the call */
        for (unsigned j = 0; j < AUTO_ANSWER_MEDIA_CNT; ++j) 
	{
            /* Repeat binding media socket to next port when fails to bind
             * to current port number.
             */
            app.call[i].media[j].call_id = i;
            app.call[i].media[j].media_id = j;

            status = PJ_EUNKNOWN;
            for (unsigned retry = 0; retry < 100; ++retry, rtp_port+=2)  
	    {
                auto_answer_call_media_t *m = &app.call[i].media[j];

                status = pjmedia_transport_udp_create3(app.med_endpt, 
                                                       AF, 
                                                       NULL, 
                                                       NULL, 
                                                       rtp_port, 
                                                       0, 
                                                       &m->transport);
                if (status == PJ_SUCCESS) 
                {
                    /* Get socket info (address, port) of the media transport. Possibly
	             * it will be useful during SDP creation (i.e. the address and port info in
	             * the SDP).
                     */
	            pjmedia_transport_info_init(&m->tpinfo);
	            pjmedia_transport_get_info(m->transport, &m->tpinfo);

	            pj_memcpy(&m->sock_info, &m->tpinfo.sock_info, sizeof(pjmedia_sock_info));

	     	    rtp_port += 2;
                    break;
                }
            }
        }
    }
    
    return status;
}

/* Media initialization & conference bridge creation with slots for WAV and tones */
static pj_status_t auto_answer_media_init(void)
{
    pj_status_t status = PJ_SUCCESS;
    pjmedia_port *conf_port = NULL;
    
    /* Initialize media endpoint */
    status = pjmedia_endpt_create(&app.cp.factory, NULL, 1, &app.med_endpt);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error creating media endpoint", status);
        goto _on_return;
    }
    
    /* Register all codecs */
    status = pjmedia_codec_register_audio_codecs(app.med_endpt, NULL);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error registering codecs", status);
        goto _on_return;
    }
    
    /* Init conference bridge. */
    status = pjmedia_conf_create(app.pool, 
                                 AUTO_ANSWER_MAX_CONF_PORTS,
                                 CLOCK_RATE, 
                                 CHANNEL_COUNT, 
                                 SAMPLES_PER_FRAME,
                                 BITS_PER_SAMPLE, 
                                 PJMEDIA_CONF_NO_DEVICE, 
                                 &app.conf);
                                 
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error creating conference bridge", status);
        goto _on_return;
    }
    
    status = pjmedia_null_port_create(app.pool, CLOCK_RATE, CHANNEL_COUNT, SAMPLES_PER_FRAME, BITS_PER_SAMPLE, &app.null_port);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error creating null port", status);
        goto _on_return;
    }
    
    conf_port = pjmedia_conf_get_master_port(app.conf);
    if (conf_port == NULL)
    {
        auto_answer_error("Error conf port is NULL", status);
        goto _on_return;
    }
    
    status = pjmedia_master_port_create(app.pool, app.null_port, conf_port, 0, &app.master_port);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error creating master port", status);
        goto _on_return;
    }
    
    status = pjmedia_master_port_start(app.master_port);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error start master port", status);
        goto _on_return;
    }
    
    						 
    status = pjmedia_wav_player_port_create(app.pool, app.wav_file.ptr, PTIME, 0, 0, &app.wav_port);
                                            
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Error creating wav player", status);
        goto _on_return;
    }
    
    status = pjmedia_conf_add_port(app.conf, app.pool, app.wav_port, NULL, &app.wav_slot);
                                   
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to add file to conference bridge", status);
        goto _on_return;
    }
    
    for (int i = 0; i < TONE_CNT; ++i) 
    {
        pjmedia_port *tport;
        status = pjmedia_tonegen_create(app.pool, 
                                        CLOCK_RATE, 
                                        CHANNEL_COUNT, 
                                        SAMPLES_PER_FRAME, 
                                        BITS_PER_SAMPLE, 
                                        PJMEDIA_TONEGEN_LOOP, 
                                        &tport);

        if (status != PJ_SUCCESS) 
        {
            auto_answer_error("Error creating tone generator", status);
            goto _on_return;
        }
        
        status = pjmedia_tonegen_play(tport, 1, &app.tone[i], PJMEDIA_TONEGEN_LOOP);
        if (status != PJ_SUCCESS) 
        {
            auto_answer_error("Error during tone play attempt", status);
            goto _on_return;
        }
        
        status = pjmedia_conf_add_port(app.conf, app.pool, tport, NULL, &app.tone_slots[i]);
        if (status != PJ_SUCCESS) 
        {
            auto_answer_error("Error during adding the port to PJSUA conf brifge", status);
            goto _on_return;
        }							  
    }			 
					
    /* Create event manager */
    if (!pjmedia_event_mgr_instance()) 
    {
        status = pjmedia_event_mgr_create(app.pool, 0, NULL);
        if (status != PJ_SUCCESS) 
        {
            auto_answer_error("Error creating PJMEDIA event manager", status);
            goto _on_return;
        }
    }

    status = auto_answer_media_transport_init();
    if (status != PJ_SUCCESS) 
        auto_answer_error("Unable to create media transport for RTP", status);
    
_on_return:
    return status;
}

/* Retrieve playback type based on extension */
static auto_answer_dst_type_e auto_answer_translate_ext_to_dst(pj_str_t* extension)
{
    for (int i = 0; i < NUMBER_OF_EXTENSIONS; ++i) { 
        if (!pj_strcmp(&auto_answer_dst[i].auto_answer_number, extension))
            return auto_answer_dst[i].auto_answer_type;
    }
    return eAUTO_ANSWER_DST_TYPE_NONE;
}

/* Destroy media stream & port that were created for call */
static void auto_answer_destroy_call_media(unsigned call_id)
{
    PJ_LOG(4,(THIS_FILE, "Destroy media for call with id %d", call_id));
		  
    auto_answer_call_media_t *media = NULL;
	
    for (unsigned i = 0; i < AUTO_ANSWER_MEDIA_CNT; ++i)
    {
        media = &app.call[call_id].media[i];

        if (media->stream)
        {
            if (media->conf_slot != AUTO_ANSWER_INVALID_ID) 
            {
                if (app.conf) 
                    pjmedia_conf_remove_port(app.conf, media->conf_slot);

                media->conf_slot = AUTO_ANSWER_INVALID_ID;
            }	

            if (media->media_port) 
            {
                pjmedia_port_destroy(media->media_port);
                media->media_port = NULL;
            }	 

            pjmedia_stream_destroy(media->stream);
            media->stream = NULL;
            pjmedia_transport_media_stop(media->transport);
        }
    }
}

/* Application destroy */
static pj_status_t auto_answer_app_destroy(void)
{
    pj_status_t status = PJ_SUCCESS;

    auto_answer_hangup_all_calls();

    /* Close wav player */
    if (app.wav_slot != AUTO_ANSWER_INVALID_ID) 
    {
        pjmedia_conf_remove_port(app.conf, app.wav_slot);
        pjmedia_port_destroy(app.wav_port);
        app.wav_port = NULL;
        app.wav_slot = AUTO_ANSWER_INVALID_ID;
    }

    /* Close tone generators */
    for (int i = 0; i < TONE_CNT; ++i) 
    {
        if (app.tone_slots[i] != AUTO_ANSWER_INVALID_ID) {
            pjmedia_conf_remove_port(app.conf, app.tone_slots[i]);
            app.tone_slots[i] = AUTO_ANSWER_INVALID_ID;
        }
    }

    if (app.null_port)
    {
        pjmedia_port_destroy(app.null_port);
        app.null_port = NULL;
    }
    
    if (app.master_port)
    {
        pjmedia_master_port_destroy(app.master_port, PJ_FALSE);
        app.master_port = NULL;
    }
    
    /* Destroy conference bridge */
    if (app.conf) 
    {
        pjmedia_conf_destroy(app.conf);
        app.conf = NULL;
    }

    /* Destroy media */
    for (unsigned i = 0; i < app.max_calls; ++i) 
    {
        for (unsigned j = 0; j < AUTO_ANSWER_MEDIA_CNT; ++j) 
        {
            auto_answer_call_media_t *m = &app.call[i].media[j];
            if (m->transport) 
            {
                pjmedia_transport_close(m->transport);
                m->transport = NULL;
            }
        }
    }

    /* Destroy event manager */
    if (pjmedia_event_mgr_instance())
        pjmedia_event_mgr_destroy(NULL); 
	
    /* Destroy media endpoint */
    if (app.med_endpt) 
    {
        pjmedia_endpt_destroy(app.med_endpt);
        app.med_endpt = NULL;
    }

    app.quit = PJ_TRUE;
    if (app.worker_thread)
    {
       pj_thread_join(app.worker_thread);
       app.worker_thread = NULL;
    }
    
    /* Destroy pjsip endpoint */
    if (app.sip_endpt) 
    {
        pjsip_endpt_destroy(app.sip_endpt);
        app.sip_endpt = NULL;
    }
    
    /* Destroy mutex */
    if (app.mutex)
    {
        pj_mutex_destroy(app.mutex);
        app.mutex = NULL;
    }
	
    /* Release pool */
    if (app.pool) 
    {
        pj_pool_release(app.pool);
        app.pool = NULL;
    }
	
    /* Destroy pool factory */
    pj_caching_pool_destroy(&app.cp);
	
    /* Shutdown PJLIB */
    pj_shutdown();
	
    /* Reset config */
    pj_bzero(&app, sizeof(app));

    return status;
}

static void auto_answer_hangup_call(unsigned call_id)
{
    pj_status_t status = PJ_SUCCESS;
    unsigned code = PJSIP_SC_DECLINE;
    pjsip_tx_data *tdata = NULL;
    auto_answer_call_t *call = NULL;

    if (call_id >= app.max_calls) 
    {
        PJ_LOG(1,(THIS_FILE, "auto_answer_hangup_call(): invalid call id %d", call_id));
        goto _on_return;
    }

    call = &app.call[call_id];
	
    if (call->inv == NULL)
        goto _on_return;

    if (call->inv->state == PJSIP_INV_STATE_CONFIRMED)
        code = PJSIP_SC_OK;
 
    auto_answer_destroy_call_media(call_id);

    status = pjsip_inv_end_session(call->inv, code, NULL, &tdata);
	
    if (status == PJ_SUCCESS && tdata != NULL)
        pjsip_inv_send_msg(call->inv, tdata);
    
_on_return:
    return;
}

static void auto_answer_hangup_all_calls(void)
{
    PJ_LOG(4,(THIS_FILE, "Hangup all calls"));
	
    for (unsigned i = 0; i < app.max_calls; ++i) 
    {
        if (app.call[i].inv)
            auto_answer_hangup_call(i);
    }
}

/* Auto hangup timer callback */
static void auto_answer_hangup_timer_cb(pj_timer_heap_t *th, pj_timer_entry *entry)
{
    unsigned call_id = (unsigned)(pj_size_t)entry->user_data;
    
    PJ_LOG(4,(THIS_FILE, "Automatic hangup via %d milliseconds for call with id = %d", app.call_duration, call_id));
	
    app.call[call_id].auto_hangup_timer.id = PJ_FALSE;
    auto_answer_hangup_call(call_id);
}

/* Auto answer timer callback, this timer creates 1 second delay after SIP 180 Ringing message */
static void auto_answer_timer_cb(pj_timer_heap_t *th, pj_timer_entry *entry)
{
    pj_status_t status = PJ_SUCCESS;
    auto_answer_call_t *call = NULL;
    pjsip_tx_data *tdata = NULL;
    unsigned call_id = (unsigned)(pj_size_t)entry->user_data;
    
    app.call[call_id].auto_answer_delay.id = PJ_FALSE;
    
    if (call_id >= app.max_calls)
        goto _on_return;

    PJ_LOG(4,(THIS_FILE, "Automatic answered via %d milliseconds for call with id = %d", AUTO_ANSWER_DELAY, call_id));
    
    call = &app.call[call_id];
    
    if (call->inv == NULL)
        goto _on_return;
    
    /* Create 200 response */
    status = pjsip_inv_answer(call->inv, PJSIP_SC_OK, NULL, NULL, &tdata);
	
    if (status == PJ_SUCCESS)
        /* Send the 200 response */
        status = pjsip_inv_send_msg(call->inv, tdata);
    
    if (status != PJ_SUCCESS) 
        pjsip_inv_terminate(call->inv, PJSIP_SC_INTERNAL_SERVER_ERROR, PJ_FALSE);
    
_on_return:
    return;  
}

/* Start auto answer timer to initiate delay after SIP 180 Ringing message */
static void auto_answer_timer_start(unsigned call_id)
{
    pj_time_val delay = {0, AUTO_ANSWER_DELAY};
    auto_answer_call_t *call = &app.call[call_id];
    
    call->auto_answer_delay.id = PJ_TRUE;
    pjsip_endpt_schedule_timer(app.sip_endpt, &call->auto_answer_delay, &delay);
}

/* Connect conference ports depend on dialed numer - URI in SIP INVITE message */
static void auto_answer_conf_connect(unsigned conf_slot, auto_answer_dst_type_e dst_type, pjmedia_dir dir)
{
    if ((conf_slot != AUTO_ANSWER_INVALID_ID) && (dir & PJMEDIA_DIR_ENCODING_DECODING))
    {
        switch (dst_type) 
        {
            case eAUTO_ANSWER_DST_TYPE_WAV_FILE:
                if (app.wav_slot != AUTO_ANSWER_INVALID_ID)
                    pjmedia_conf_connect_port(app.conf, app.wav_slot, conf_slot, 0);
                    break;
            case eAUTO_ANSWER_DST_TYPE_LONG_TONE:
                if (app.tone_slots[eAUTO_ANSWER_TONE_TYPE_LONG_TONE] != AUTO_ANSWER_INVALID_ID) 
                    pjmedia_conf_connect_port(app.conf, app.tone_slots[eAUTO_ANSWER_TONE_TYPE_LONG_TONE], conf_slot, 0);
                    break;
            case eAUTO_ANSWER_DST_TYPE_SHORT_TONE:
                if (app.tone_slots[eAUTO_ANSWER_TONE_TYPE_SHORT_TONE] != AUTO_ANSWER_INVALID_ID) 
                    pjmedia_conf_connect_port(app.conf, app.tone_slots[eAUTO_ANSWER_TONE_TYPE_SHORT_TONE], conf_slot, 0);
                    break;

            default:
                pjmedia_conf_connect_port(app.conf, conf_slot, 0, 0);
                pjmedia_conf_connect_port(app.conf, 0, conf_slot, 0);
                break;				
          }
    }

}

/*
 * Callback when incoming requests outside any transactions and any
 * dialogs are received. We're only interested to hande incoming INVITE
 * request, and we'll reject any other requests with 500 response.
 */
static pj_bool_t on_rx_request(pjsip_rx_data *rdata)
{
    unsigned i = 0, options = 0;
    auto_answer_call_t *call = NULL;
    pjsip_dialog *dlg = NULL;
    pjmedia_sdp_session *local_sdp = NULL;
    pjsip_tx_data *tdata = NULL;
    pjsip_sip_uri *sip_uri = NULL;
    pj_status_t status = PJ_SUCCESS;
	
    /* Respond (statelessly) any non-INVITE requests with 500 */
    if (rdata->msg_info.msg->line.req.method.id != PJSIP_INVITE_METHOD) 
    {
        if (rdata->msg_info.msg->line.req.method.id != PJSIP_ACK_METHOD) 
        {
            pj_str_t reason = pj_str("Unsupported operation for auto answer application");					   
            pjsip_endpt_respond_stateless(app.sip_endpt, rdata, PJSIP_SC_INTERNAL_SERVER_ERROR, &reason, NULL, NULL);
            goto _on_return;
        }
    }

    pj_mutex_lock(app.mutex);
    
    /* Find free call slot */
    for (i = 0; i < app.max_calls; ++i) 
    {
        if (app.call[i].inv == NULL)
            break;
    }

    if (i == app.max_calls) 
    {
        const pj_str_t reason = pj_str("Too many calls");
        pjsip_endpt_respond_stateless(app.sip_endpt, rdata, PJSIP_SC_BUSY_HERE, &reason, NULL, NULL);
        pj_mutex_unlock(app.mutex);
        goto _on_return;
    }
    
    call = &app.call[i];
	
    /* Verify that we can handle the request */
    status = pjsip_inv_verify_request(rdata, &options, NULL, NULL, app.sip_endpt, &tdata); 
    if (status != PJ_SUCCESS) 
    {
        if (tdata) 
        {
            pjsip_response_addr res_addr;

            pjsip_get_response_addr(tdata->pool, rdata, &res_addr);
            status = pjsip_endpt_send_response(app.sip_endpt, &res_addr, tdata, NULL, NULL);
            if (status != PJ_SUCCESS) 
                pjsip_tx_data_dec_ref(tdata);
        } 
        else 
        {    
            /* Respond with 500 (Internal Server Error) */
            pjsip_endpt_respond_stateless(app.sip_endpt, rdata, PJSIP_SC_INTERNAL_SERVER_ERROR, NULL, NULL, NULL);
        }
        pj_mutex_unlock(app.mutex);
        goto _on_return;
    }

    /* Create UAS dialog */
    status = pjsip_dlg_create_uas_and_inc_lock(pjsip_ua_instance(), rdata, &app.local_uri, &dlg);
    if (status != PJ_SUCCESS) 
    {
        const pj_str_t reason = pj_str("Unable to create dialog");
        pjsip_endpt_respond_stateless(app.sip_endpt, rdata, PJSIP_SC_INTERNAL_SERVER_ERROR, &reason, NULL, NULL);
        pj_mutex_unlock(app.mutex);
        goto _on_return;
    }

    status = pjmedia_endpt_create_sdp(app.med_endpt, 
                                      rdata->tp_info.pool, 
                                      AUTO_ANSWER_MEDIA_CNT, 
                                      &call->media[0].sock_info, 
                                      &local_sdp);
    if (status != PJ_SUCCESS) 
    {
        pjsip_dlg_dec_lock(dlg);
        pj_mutex_unlock(app.mutex);
        goto _on_return;
    }

    /* Create invite session, and pass both the UAS dialog and the SDP
     * capability to the session
     */
    status = pjsip_inv_create_uas(dlg, rdata, local_sdp, 0, &call->inv);
    if (status != PJ_SUCCESS) 
    {
        pjsip_dlg_create_response(dlg, rdata, PJSIP_SC_INTERNAL_SERVER_ERROR, NULL, &tdata);
        pjsip_dlg_send_response(dlg, pjsip_rdata_get_tsx(rdata), tdata);
        pjsip_dlg_dec_lock(dlg);
        pj_mutex_unlock(app.mutex);
        goto _on_return;
    }

    /* Invite session has been created, decrement & release dialog lock & unlock mutex for call array */
    pjsip_dlg_dec_lock(dlg);
    pj_mutex_unlock(app.mutex);

    /* Attach call data to invite session */
    call->inv->mod_data[mod_auto_answer.id] = call;
	
    /* Determine destination type based on calling number in URI */
    sip_uri = (pjsip_sip_uri*)rdata->msg_info.msg->line.req.uri;
    call->dst_type = auto_answer_translate_ext_to_dst(&sip_uri->user);
 
    status = pjsip_inv_initial_answer(call->inv, rdata, PJSIP_SC_TRYING, NULL, NULL, &tdata);
   
    if (status == PJ_SUCCESS)
    	status = pjsip_inv_send_msg(call->inv, tdata);

    if (status == PJ_SUCCESS)
    	status = pjsip_inv_answer(call->inv, PJSIP_SC_RINGING, NULL, NULL, &tdata);
    
    if (status == PJ_SUCCESS)
    	status = pjsip_inv_send_msg(call->inv, tdata);

    if (status != PJ_SUCCESS) {
	pjsip_endpt_respond_stateless(app.sip_endpt, rdata, PJSIP_SC_INTERNAL_SERVER_ERROR, NULL, NULL, NULL);
	call->inv = NULL;
	goto _on_return;
    }
    else
    {
        auto_answer_timer_start(call->call_id);  
    }

_on_return:
    return PJ_TRUE;
}

/*
 * Callback when INVITE session state has changed.
 * This callback is registered when the invite session module is initialized.
 * When the invite session has been disconnected, it is needed to clear
 * the appropriate call.
 */
static void call_on_state_changed(pjsip_inv_session *inv, 
                                  pjsip_event *e)
{
    PJ_UNUSED_ARG(e);
    
    pj_time_val duration = { 0, 0 }; 
    auto_answer_call_t *call = (auto_answer_call_t*)inv->mod_data[mod_auto_answer.id];
	
    if (!call) 
    {
        PJ_LOG(3,(THIS_FILE, "There is no call to disconnect"));
        goto _on_return;
    }
	
    if (inv->state == PJSIP_INV_STATE_DISCONNECTED) 
    {
        PJ_LOG(3,(THIS_FILE, "Call DISCONNECTED [reason=%d (%s)]", inv->cause, 
                                                                   pjsip_get_status_text(inv->cause)->ptr));
        if (call->auto_hangup_timer.id != PJ_FALSE) 
        {
            pjsip_endpt_cancel_timer(app.sip_endpt, &call->auto_hangup_timer);
            call->auto_hangup_timer.id = PJ_FALSE;
        }
	
        auto_answer_destroy_call_media(call->call_id);

        call->inv = NULL;
        inv->mod_data[mod_auto_answer.id] = NULL;
        call->dst_type = eAUTO_ANSWER_DST_TYPE_NONE;	
    } 
    else if (inv->state == PJSIP_INV_STATE_CONFIRMED)
    {
        PJ_LOG(3,(THIS_FILE, "Call #%d is established", call->call_id));

        if (app.call_duration != 0) 
        {
            duration.sec = 0;
            duration.msec = app.call_duration;
            call->auto_hangup_timer.id = PJ_TRUE;
            pjsip_endpt_schedule_timer(app.sip_endpt, &call->auto_hangup_timer, &duration);
        }
    }
    else
    {
        PJ_LOG(3,(THIS_FILE, "Call state changed to %s", pjsip_inv_state_name(inv->state)));
    }
    
_on_return:
    return;
}

/*
 * Callback when SDP negotiation has completed.
 * Media will be started as soon as SDP negotiation is completed.
 */
static void call_on_media_update(pjsip_inv_session *inv,
                                 pj_status_t status)
{
    pjmedia_stream_info stream_info;
    const pjmedia_sdp_session *local_sdp = NULL;
    const pjmedia_sdp_session *remote_sdp = NULL;
    auto_answer_call_t *call = NULL;
    auto_answer_call_media_t *media = NULL;
    
    pj_bzero(&stream_info, sizeof(stream_info));

    /* Do nothing if media negotiation has failed */
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("SDP negotiation has failed", status);
        goto _on_return;
    }

    call = (auto_answer_call_t *)inv->mod_data[mod_auto_answer.id];
    
    if (!call) 
    {
        PJ_LOG(3,(THIS_FILE, "There is no call for media update"));
        goto _on_return;
    }
    
    media = &call->media[0];
 
    /* Get local and remote SDP to create a media session */
    pjmedia_sdp_neg_get_active_local(inv->neg, &local_sdp);
    pjmedia_sdp_neg_get_active_remote(inv->neg, &remote_sdp);

    /* Create stream info based on the media audio SDP */
    status = pjmedia_stream_info_from_sdp(&stream_info, inv->pool, app.med_endpt, local_sdp, remote_sdp, 0);

    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to create audio stream info", status);
        goto _on_return;
    }

    /* Create new audio media stream, passing the stream info, and also the
     * media socket that was created earlier.
     */
    status = pjmedia_stream_create(app.med_endpt, inv->pool, &stream_info, media->transport, NULL, &media->stream);
   
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to create audio stream", status);
        goto _on_return;
    }

    /* Start the audio stream */
    status = pjmedia_stream_start(media->stream);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to start audio stream", status);
        auto_answer_destroy_call_media(call->call_id);
        goto _on_return;
    }

    /* Start the UDP media transport */
    pjmedia_transport_media_start(media->transport, 0, 0, 0, 0);

    /* Get the media port interface of the audio stream. Media port interface is basicly a struct 
     * containing get_frame() and put_frame() function. With this media port interface, the port 
     * interface can be attached to conference bridge, or directly to a sound player/recorder device.
     */
    pjmedia_stream_get_port(media->stream, &media->media_port);

    status = pjmedia_conf_add_port(app.conf,
                                   inv->pool,
                                   media->media_port,
                                   NULL,
                                   &media->conf_slot);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to add conference port", status);
        auto_answer_destroy_call_media(call->call_id);		
        goto _on_return;
    }
    
    auto_answer_conf_connect(media->conf_slot, call->dst_type, stream_info.dir);   
    
_on_return:
    return;
}

static void auto_answer_console_output(void)
{
    char option[BUFFER_SIZE];
    
    puts("");
    puts("Auto answer application");
    puts("For 1111 called number wav file will be played");
    puts("For 2222 called number long tone will be played");
    puts("For 3333 called number short tone will be played");
    puts("For other numbers call will be established with sound device");
    puts("Press Ctrl-C to quit");
    puts("Available menu:");
    puts("  f   Set wav file for playback");
    puts("  t   Set autohangup timer for calls in milliseconds, 0 - disable timer");
    puts("  h   Hangup all calls");
    puts("  q   Quit");
    puts("");
    
    if (fgets(option, sizeof(option), stdin) == NULL) 
    {
        puts("EOF while reading stdin, will quit now..");
        app.quit = PJ_TRUE;
    }

    if (option[0] == 'f') 
    {
        char filename[PJ_MAXPATH];
        puts("Please make sure that file is available in directory with binary");
        puts("The file should be in wav format");
        if (sscanf(option + 2, "%s", filename) != 1)
            puts("error: incorrect value");
        else
            app.wav_file = pj_str(filename);
    }
 
    if (option[0] == 't') 
    {
        unsigned timer = 0;
        if (sscanf(option + 2, "%u", &timer) != 1)
            puts("error: incorrect value");
        else
            app.call_duration = timer;
    }

    if (option[0] == 'h')
        auto_answer_hangup_all_calls();

    if (option[0] == 'q') 
       app.quit = PJ_TRUE;
}
       
/* Display error message */
static void auto_answer_error(const char *title, pj_status_t status)
{
    char errmsg[PJ_ERR_MSG_SIZE];
	
    pj_strerror(status, errmsg, sizeof(errmsg));
    PJ_LOG(3,(THIS_FILE, "%s: %s [status=%d]", title, errmsg, status));
}

/*
 * main()
 */
int main(int argc, char *argv[])
{
    pj_status_t status = PJ_SUCCESS;
    int af = AF;
    pj_sockaddr host_addr;
    char temp[PJSIP_MAX_URL_SIZE], hostip[PJ_INET6_ADDRSTRLEN];
    
    pj_bzero(&temp, sizeof(temp));
    pj_bzero(&hostip, sizeof(hostip));
    pj_bzero(&host_addr, sizeof(host_addr));
    
    /* Inititalize application data */
    auto_answer_app_init();

    /* Init calls */
    auto_answer_call_array_init();

    /* Init PJLIB */
    status = pj_init();
    if (status != PJ_SUCCESS) 
        goto _on_return;

    pj_log_set_level(4);

    /* Init PJLIB-UTIL */
    status = pjlib_util_init();
    if (status != PJ_SUCCESS)
    { 
        auto_answer_error("Error in pjlib_util_init()", status);
        goto _on_return;
    }

    /* Create a pool factory before memory allocation */
    pj_caching_pool_init(&app.cp, &pj_pool_factory_default_policy, 0);

    /* Create application pool for application */
    app.pool = pj_pool_create(&app.cp.factory, "app", AUTO_ANSWER_POOL_LEN, AUTO_ANSWER_POOL_INC, NULL);
    if (app.pool == NULL) 
    {
        auto_answer_error("Unable to create pool for application", PJ_ENOMEM);
        goto _on_return;
    }

    /* Create mutex */
    status = pj_mutex_create_simple(app.pool, "auto_answer", &app.mutex);
    if (status != PJ_SUCCESS)
    {
        auto_answer_error("Unable to create mutex", status);
        goto _on_return;
    }
    
    status = auto_answer_sip_stack_init();
    if (status != PJ_SUCCESS)
        goto _on_return; 
    
    status = pj_gethostip(af, &host_addr);
    if (status != PJ_SUCCESS) 
    {
        auto_answer_error("Unable to retrieve local host IP", status);
        goto _on_return;        
    }

    pj_sockaddr_print(&host_addr, hostip, sizeof(hostip), 2);
    pj_ansi_sprintf(temp, "<sip:%s:%d>", hostip, UDP_PORT);
    app.local_uri = pj_str(temp);

    status = auto_answer_media_init();
    if (status != PJ_SUCCESS) 
        goto _on_return;

    PJ_LOG(3,(THIS_FILE, "Ready for incoming calls (max=%d)", app.max_calls));
   
    /* Wait until user press "q" to quit. */
    for (;;) 
    {     
       auto_answer_console_output();
       if (app.quit)
           break;
    }
      
_on_return:
    /* Destroy application */
    auto_answer_app_destroy();
    return PJ_SUCCESS;
}
