/*
 *  To get more understanding what is FAX, T30, T37, T38 and how FoIP works,
 *  read the:
 *  "Fax, Modem, and Text for IP Telephony"(David Hanes, Gonzalo Salgueiro,2008)
 *  and also ITU official documentation (T30, T38, T37).
 *
 */
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <spandsp/t4_rx.h>
#include <sys/poll.h>
#include <pthread.h>

#include "fax.h"

#define THIS_FILE "fax.c"

#define DEF_T38_MAX_BITRATE       9600
#define DEF_T38_FAX_VERSION       0
#define DEF_T38_FILL_BIT_REMOVAL  0
#define DEF_T38_TRANSCODING_MMR   0
#define DEF_T38_TRANSCODING_JBIG  0
#define DEF_T38_RATE_MANAGEMENT   "transferredTCF"
#define DEF_T38_MAX_BUFFER        72
#define DEF_T38_MAX_DATAGRAM      316
#define DEF_T38_VENDOR_INFO       "Tyryshkin M V"
#define DEF_T38_UDP_EC            "t38UDPRedundancy"

#define DEF_FAX_IDENT             "FAX_TRANSMITTER"
#define DEF_FAX_HEADER            "FAX_DEFAULT_HEADER"
#define DEF_FAX_VERBOSE           0
#define DEF_FAX_USE_ECM           1
#define DEF_FAX_DISABLE_V17       0

#define MAX_FEC_ENTRIES           4
#define MAX_FEC_SPAN              4
#define DEFAULT_FEC_ENTRIES       3
#define DEFAULT_FEC_SPAN          3

#define FRAMES_PER_CHUNK          160

#define MAX_MSG_SIZE 1500

static uint8_t msg_buffer[MAX_MSG_SIZE];


static int socket_init(struct sockaddr_in *addr, fax_session_t *f_session)
{
    int sock_fd = 0;
    int optval = 1;
    int res;

    addr->sin_family = AF_INET;
    addr->sin_port   = ntohs(f_session->local_port);
    addr->sin_addr.s_addr = INADDR_ANY;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        printf("fax: %s: socket() error\n", __func__);
        return -1;
    }

    res = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (res < 0) {
        printf("fax: %s: setsockopt() error\n", __func__);
        return -1;
    }

    res = bind(sock_fd, (struct sockaddr *) addr, sizeof(*addr));
    if (res < 0) {
        printf("fax: %s: bind() error\n", __func__);
        return -1;
    }

    f_session->socket_fd = sock_fd;

    if(f_session->pvt.caller)
    {
        f_session->rem_addr = malloc(sizeof(struct sockaddr_in));
        f_session->rem_addr->sin_family = AF_INET;
        f_session->rem_addr->sin_port = htons(f_session->remote_port);
        f_session->rem_addr->sin_addr.s_addr = htonl(f_session->remote_ip);
    }

    return sock_fd;
}


static int send_frame(fax_session_t *f_session, uint8_t *frame_buf, int len)
{
    int bytes_sent = 0;
    bytes_sent = sendto(f_session->socket_fd, frame_buf, len, 0,
                        (struct sockaddr *)f_session->rem_addr,
                        sizeof(*f_session->rem_addr));

    return bytes_sent;
}


static int receive_frame(fax_session_t *f_session, uint8_t *frame_buf, int len)
{
    int bytes_received = 0;
    struct sockaddr_storage remote_addr;
    socklen_t rem_addr_len = sizeof(remote_addr);
    bytes_received = recvfrom(f_session->socket_fd, frame_buf, len, 0,
                              (struct sockaddr *)&remote_addr, &rem_addr_len);
    if (!f_session->rem_addr) {
        f_session->rem_addr = malloc(sizeof(struct sockaddr_in));
        memcpy(f_session->rem_addr, &remote_addr, rem_addr_len);
        printf("fax: Remote fax addres: %s:%d\n",
                   inet_ntoa(f_session->rem_addr->sin_addr),
                   ntohs(f_session->rem_addr->sin_port));
    }

    return bytes_received;
}


/*
 *  Spandsp log callback implenetation
 */
static void spanfax_log_message(int level, const char *msg)
{
	switch (level) {
		case SPAN_LOG_NONE:
			return;
		case SPAN_LOG_ERROR:
		case SPAN_LOG_PROTOCOL_ERROR:
			if (msg) printf("SPANDSP_LOG %s\n", msg);
			break;
		case SPAN_LOG_WARNING:
		case SPAN_LOG_PROTOCOL_WARNING:
			if (msg) printf("SPANDSP_LOG %s\n", msg);
			break;
		case SPAN_LOG_FLOW:
		case SPAN_LOG_FLOW_2:
		case SPAN_LOG_FLOW_3:
		default:	/* SPAN_LOG_DEBUG, SPAN_LOG_DEBUG_2, SPAN_LOG_DEBUG_3 */
			if (msg) printf("SPANDSP_LOG %s\n", msg);
			break;
	}
}


/*
 *  Spandsp t38-terminal callback implementation to handle
 *  IFP packets generated by spandsp during fax-session
 */
static int t38_tx_packet_handler(t38_core_state_t *s, void *user_data,
								 const uint8_t *buf, int len, int count)
{
	fax_session_t *f_session;
	uint8_t pkt[LOCAL_FAX_MAX_DATAGRAM];
	int udptl_packtlen;
	int x;
	int r = 0;

	f_session = (fax_session_t *)user_data;

	if ((udptl_packtlen = udptl_build_packet(f_session->pvt.udptl_state, pkt, buf, len)) > 0) {
		for (x = 0; x < count; x++) {
			if (send_frame(f_session, pkt, udptl_packtlen) < 0) {
				printf("fax: send_frame() ERROR\n");
				perror("send error");
				r = -1;
				break;
			}
		}
	} else {
		printf("fax: INVALID UDPTL_PACKETLEN: %d PASSED: %d:%d\n", r,
				   len, count);
	}

	if (r < 0) {
		t30_state_t *t30;

		printf("fax: TERMINATING T30 STATE\n");

		if (f_session->pvt.t38_state && (t30 = t38_terminal_get_t30_state(f_session->pvt.t38_state))) {
			t30_terminate(t30);
		}
	}

	return r < 0 ? r : 0;
}


/*
 * T30 fax session consists of 5 phases:
 *
 *      1) Phase A "Call Establishment"
 *          Off-hook, Dialing, Ringing, Answering, CNG and CED Tones
 *
 *      2) Phase B "Pre-Message Procedure"
 *          Fax Terminal Identification, Capabilities exchanged and Set, Training
 *
 *      3) Phase C "In-Message Procedure, Message Transmission"
 *          Transmission of Pages, Line Supervision, Error Detection and Correction
 *
 *      4) Phase D "Post Message Procedure"
 *          End-of-Message Signaling, Page Confirmation
 *
 *      5) Phase E "Call Release"
 *          Call Disconnect and Return to On-hook State
 */
static int phase_b_handler(t30_state_t *s, void *user_data, int result)
{
	t30_stats_t t30_stats;

	const char *local_ident;
	const char *far_ident;
	const char *tmp;
	fax_session_t *f_session = (fax_session_t *)user_data;

	t30_get_transfer_statistics(s, &t30_stats);

	tmp = t30_get_tx_ident(s);
	local_ident = tmp ? tmp : "";
	tmp = t30_get_rx_ident(s);
	far_ident = tmp ? tmp : "";

	printf("fax: Phase B handler (Call: '%s' %s)\n", f_session->call_id,
		   f_session->pvt.caller ? "sender" : "receiver");
	printf("fax: === Negotiation Result =======================================================\n");
	printf("fax: Remote station id: %s\n", far_ident);
	printf("fax: Local station id:  %s\n", local_ident);
	printf("fax: Transfer Rate:     %i\n", t30_stats.bit_rate);

	printf("fax: ECM status        %s\n", (t30_stats.error_correcting_mode) ? "on" : "off");
	printf("fax: remote country:   %s\n", (t30_get_rx_country(s) ? (t30_get_rx_country(s)) : ""));
	printf("fax: remote vendor:    %s\n", (t30_get_rx_vendor(s) ? (t30_get_rx_vendor(s)) : ""));
	printf("fax: remote model:     %s\n", (t30_get_rx_model(s) ? (t30_get_rx_model(s)) : ""));

	printf("fax: ==============================================================================\n");

	return T30_ERR_OK;
}

static int phase_d_handler(t30_state_t *s, void *user_data, int msg)
{
	t30_stats_t t30_stats;

	fax_session_t *f_session = (fax_session_t *)user_data;

	t30_get_transfer_statistics(s, &t30_stats);

	printf("fax: Phase D handler (Call: '%s' %s)\n", f_session->call_id,
		   f_session->pvt.caller ? "sender" : "receiver");
	printf("fax: ==== Page %s ===========================================================\n",
		   f_session->pvt.caller ? "Sent" : "Received");
	printf("fax: Page no = %d\n", f_session->pvt.caller ?
			   t30_stats.pages_tx : t30_stats.pages_rx);
	printf("fax: Image size = %d x %d pixels\n", t30_stats.width, t30_stats.length);
	printf("fax: Image resolution = %d/m x %d/m\n", t30_stats.x_resolution, t30_stats.y_resolution);
	printf("fax: Compressed image size = %d bytes\n", t30_stats.image_size);
	printf("fax: Bad rows = %d\n", t30_stats.bad_rows);
	printf("fax: Longest bad row run = %d\n", t30_stats.longest_bad_row_run);
	printf("fax: ==============================================================================\n");

	return T30_ERR_OK; /* I don't think this does anything */
}

extern int sendRelese(fax_session_t *f_session);

/*
 *  Called at the end of the document
 */
static void phase_e_handler(t30_state_t *s, void *user_data, int result)
{
	t30_stats_t t;
	const char *local_ident;
	const char *far_ident;
	const char *tmp;

	fax_session_t *f_session;

	t30_get_transfer_statistics(s, &t);
	f_session = (fax_session_t *)user_data;

	tmp = t30_get_tx_ident(s);
	local_ident = tmp ? tmp : "";
	tmp = t30_get_rx_ident(s);
	far_ident = tmp ? tmp : "";

	printf("fax: Phase E handler (Call: '%s' %s)\n", f_session->call_id,
		   f_session->pvt.caller ? "sender" : "receiver");
	printf("fax: ==============================================================================\n");

	if (result == T30_ERR_OK) {
		printf("fax: Fax successfully %s\n", f_session->pvt.caller ?
				   "sent" : "received");
	} else {
		printf("fax: Fax processing not successful - result (%d) %s\n", result,
						  t30_completion_code_to_str(result));
	}

	printf("fax: Remote station id: %s\n", far_ident);
	printf("fax: Local station id:  %s\n", local_ident);
	printf("fax: Pages transferred: %i\n", f_session->pvt.caller ?
			   t.pages_tx : t.pages_rx);
	printf("fax: Total fax pages:   %i\n", t.pages_in_file);
	printf("fax: Image resolution:  %ix%i\n", t.x_resolution, t.y_resolution);
	printf("fax: Transfer Rate:     %i\n", t.bit_rate);

	printf("fax: ECM status        %s\n", (t.error_correcting_mode) ? "on" : "off");
	printf("fax: remote country:   %s\n", (t30_get_rx_country(s) ? (t30_get_rx_country(s)) : ""));
	printf("fax: remote vendor:    %s\n", (t30_get_rx_vendor(s) ? (t30_get_rx_vendor(s)) : ""));
	printf("fax: remote model:     %s\n", (t30_get_rx_model(s) ? (t30_get_rx_model(s)) : ""));

	printf("fax: ==============================================================================\n");

	/*
	   Set our channel variables, variables are also used in event
	 */

	f_session->pvt.done = 1;

	if (result == T30_ERR_OK)
		f_session->fax_success = 1;
	else
		f_session->fax_success = 0;

	if(f_session->pvt.caller) sendRelese(f_session);
}


/*
 *  Init appropriate spandsp objects to receive fax
 */
static int spanfax_init(fax_session_t *f_session, fax_transport_mod_e trans_mode)
{
    t38_terminal_state_t *t38;
    t38_core_state_t *t38_core;
    t30_state_t *t30;
    logging_state_t *logging;
    int log_level;

    int fec_entries = DEFAULT_FEC_ENTRIES;
    int fec_span = DEFAULT_FEC_SPAN;

    printf("fax: %s: start!\n", __func__);

    switch(trans_mode) {
        case FAX_TRANSPORT_T38_MOD:

            memset(f_session->pvt.t38_state, 0, sizeof(t38_terminal_state_t));

			if (t38_terminal_init(f_session->pvt.t38_state, f_session->pvt.caller,
								  t38_tx_packet_handler, f_session) == NULL) {
				printf("fax: Cannot initialize T.38 structs\n");
				return 0;
			}

			t38 = f_session->pvt.t38_state;
			t30 = t38_terminal_get_t30_state(t38);

			t38_core = f_session->pvt.t38_core = t38_terminal_get_t38_core_state(t38);
			t38_terminal_set_config(t38, 0);
			t38_terminal_set_tep_mode(t38, 0);

			if (udptl_init(f_session->pvt.udptl_state, UDPTL_ERROR_CORRECTION_REDUNDANCY,
						   fec_span, fec_entries, (udptl_rx_packet_handler_t *) t38_core_rx_ifp_packet,
						   (void *) f_session->pvt.t38_core) == NULL) {
				printf("fax: Cannot initialize UDPTL structs\n");
				return 0;
			}

			if (f_session->pvt.verbose) {
				log_level = SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG |
						SPAN_LOG_SHOW_SEVERITY | SPAN_LOG_SHOW_PROTOCOL |
						SPAN_LOG_FLOW;

				logging = t38_terminal_get_logging_state(t38);
				span_log_set_message_handler(logging, spanfax_log_message);
				span_log_set_level(logging, log_level);
				span_log_set_tag(logging, f_session->log_tag);

				logging = t38_core_get_logging_state(t38_core);
				span_log_set_message_handler(logging, spanfax_log_message);
				span_log_set_level(logging, log_level);
				span_log_set_tag(logging, f_session->log_tag);

				logging = t30_get_logging_state(t30);
				span_log_set_message_handler(logging, spanfax_log_message);
				span_log_set_level(logging, log_level);
				span_log_set_tag(logging, f_session->log_tag);
			}

            break;
        case FAX_TRANSPORT_AUDIO_MOD:
        default:
            return 0;
            break;
    }

	/* All the things which are common to audio and T.38 FAX setup */

	t30_set_tx_ident(t30, f_session->pvt.ident);
	t30_set_tx_page_header_info(t30, f_session->pvt.header);

	t30_set_phase_e_handler(t30, phase_e_handler, f_session);
	t30_set_phase_d_handler(t30, phase_d_handler, f_session);
	t30_set_phase_b_handler(t30, phase_b_handler, f_session);

	t30_set_supported_image_sizes(t30, T30_SUPPORT_US_LETTER_LENGTH
								  | T30_SUPPORT_US_LEGAL_LENGTH
								  | T30_SUPPORT_UNLIMITED_LENGTH
								  | T30_SUPPORT_215MM_WIDTH
								  | T30_SUPPORT_255MM_WIDTH
								  | T30_SUPPORT_303MM_WIDTH
								  );
	t30_set_supported_resolutions(t30, T30_SUPPORT_STANDARD_RESOLUTION
								  | T30_SUPPORT_FINE_RESOLUTION
								  | T30_SUPPORT_SUPERFINE_RESOLUTION
								  | T30_SUPPORT_R16_RESOLUTION
								  |	T30_SUPPORT_R8_RESOLUTION
								  | T30_SUPPORT_300_300_RESOLUTION
								  | T30_SUPPORT_400_400_RESOLUTION
								  | T30_SUPPORT_600_600_RESOLUTION
								  |	T30_SUPPORT_1200_1200_RESOLUTION
								  |	T30_SUPPORT_300_600_RESOLUTION
								  | T30_SUPPORT_400_800_RESOLUTION
								  | T30_SUPPORT_600_1200_RESOLUTION
								  );
#if 0
	t30_set_supported_colour_resolutions(t30, 0);
#endif

	if (f_session->pvt.disable_v17) {
		t30_set_supported_modems(t30, T30_SUPPORT_V29 | T30_SUPPORT_V27TER);
	} else {
		t30_set_supported_modems(t30, T30_SUPPORT_V29 | T30_SUPPORT_V27TER
								 | T30_SUPPORT_V17);
	}

	if (f_session->pvt.use_ecm) {
		t30_set_ecm_capability(t30, TRUE);
		t30_set_supported_compressions(t30,
									   T30_SUPPORT_T4_1D_COMPRESSION
									   | T30_SUPPORT_T4_2D_COMPRESSION
									   | T30_SUPPORT_T6_COMPRESSION
									   | T30_SUPPORT_T85_COMPRESSION
									   | T30_SUPPORT_T85_L0_COMPRESSION);
	} else {
		t30_set_supported_compressions(t30, T30_SUPPORT_T4_1D_COMPRESSION
									   | T30_SUPPORT_T4_2D_COMPRESSION);
	}

	if (f_session->pvt.caller) {
		t30_set_tx_file(t30, f_session->pvt.filename, -1, -1);
	} else {
		t30_set_rx_file(t30, f_session->pvt.filename, -1);
	}

	return 0;
}


/*
 *  Release spandsp objects
 */
static int spanfax_destroy(fax_session_t *f_session)
{
	t30_state_t *t30;
	printf("fax: %s: start!\n", __func__);

	if (f_session->pvt.t38_state) {

		t30 = t38_terminal_get_t30_state(f_session->pvt.t38_state);

		if (t30) {
			t30_terminate(t30);
		}

		t38_terminal_release(f_session->pvt.t38_state);
	}

	if (f_session->pvt.udptl_state) {
		udptl_release(f_session->pvt.udptl_state);
	}
	return 0;
}


/*
 *  Set T38 parameters of spandsp fax-terminal
 *  T38 session parameters determine during sdp negotiation
 */
static int configure_t38(fax_session_t *f_session)
{
	int method = 2;
	printf("fax: %s: start!\n", __func__);

	t38_set_t38_version(f_session->pvt.t38_core, f_session->t38_options.T38FaxVersion);
	t38_set_max_buffer_size(f_session->pvt.t38_core, f_session->t38_options.T38FaxMaxBuffer);
	t38_set_fastest_image_data_rate(f_session->pvt.t38_core, f_session->t38_options.T38MaxBitRate);
	t38_set_fill_bit_removal(f_session->pvt.t38_core, f_session->t38_options.T38FaxFillBitRemoval);
	t38_set_mmr_transcoding(f_session->pvt.t38_core, f_session->t38_options.T38FaxTranscodingMMR);
	t38_set_jbig_transcoding(f_session->pvt.t38_core, f_session->t38_options.T38FaxTranscodingJBIG);
	t38_set_max_datagram_size(f_session->pvt.t38_core, f_session->t38_options.T38FaxMaxDatagram);

	if (f_session->t38_options.T38FaxRateManagement) {
		if (!strcasecmp(f_session->t38_options.T38FaxRateManagement, "transferredTCF")) {
			method = 1;
		} else {
			method = 2;
		}
	}

	t38_set_data_rate_management_method(f_session->pvt.t38_core, method);

	return 0;
}


/*
 *  Thread for fax receiving
 *  We need to receive UDP (with UDPTL) packet and feed it to spandsp and also
 *  get UDPTL from spandsp and send them to remote IAF
 */
void *fax_worker_thread(void *data)
{
    fax_session_t *fax_session = (fax_session_t *)data;
    struct sockaddr_in local_addr;
    int ret_val = 0;
    int bytes_received = 0;
    const int poll_timeout = 20;
    struct pollfd fds;

    struct in_addr addr;
    addr.s_addr = htonl(fax_session->remote_ip);


    printf("fax: Starting T.38 FAX handling: local_port: %d remote_addr:%s:%d\n",
           fax_session->local_port, inet_ntoa(addr), fax_session->remote_port);

    fax_params_init(fax_session);

    ret_val = socket_init(&local_addr, fax_session);
    if (ret_val < 0) {
        printf("fax: Fax socket init failed\n");
        goto thread_finish;
    }

    fds.fd = ret_val;
    fds.events = POLLIN;

    fax_params_set_default(fax_session);
    if (spanfax_init(fax_session, FAX_TRANSPORT_T38_MOD) != 0) {
        printf("fax: spanfax_init() ERROR\n");
        goto thread_finish;
    }

    configure_t38(fax_session);

    if(!fax_session->pvt.caller)
    {
        printf("fax %s receiver: wait first frame\n", fax_session->call_id);
        bytes_received = receive_frame(fax_session, msg_buffer, MAX_MSG_SIZE);
        printf("fax %s receiver: first frame received\n", fax_session->call_id);
    }

    fcntl(fax_session->socket_fd, F_SETFL, O_NONBLOCK);

    while (!fax_session->pvt.done) {

        /*
         * All we need to do here is:
         *      1) Receive new UDPTL packet without blocking
         *      2) Feed it to spandsp (udptl_rx_packet)
         *      3) Invoke t38_terminal_send_timeout() function
         */
        ret_val = poll(&fds, 1, poll_timeout);
        if (ret_val == -1) {
            perror("poll");
            break;
        }

        if (fds.revents & POLLIN) {

            bytes_received = receive_frame(fax_session, msg_buffer, MAX_MSG_SIZE);
//            printf("fax: Packet RECEIVED (size: %d)\n", bytes_received);
            if (bytes_received == 0) continue;
            if (bytes_received == -1) {
                printf("fax: receive_frame() ERRROR: returned -1\n");
                break;
            }

            ret_val = udptl_rx_packet(fax_session->pvt.udptl_state,
                                      msg_buffer, bytes_received);
            if (ret_val) printf("fax %s: udptl_rx_packet() ERROR\n", fax_session->call_id);
        }

        t38_terminal_send_timeout(fax_session->pvt.t38_state, FRAMES_PER_CHUNK);
    }

thread_finish:
    spanfax_destroy(fax_session);
    fax_params_destroy(fax_session);

    close(fds.fd);

    pthread_exit(NULL);
}


void fax_params_init(fax_session_t *f_session)
{
    printf("fax: %s: start!\n", __func__);

    f_session->pvt.t38_state = malloc(sizeof(t38_terminal_state_t));
    f_session->pvt.udptl_state = malloc(sizeof(udptl_state_t));

    f_session->pvt.header = NULL;
    f_session->pvt.ident = NULL;

    f_session->t38_options.T38FaxUdpEC = NULL;
    f_session->t38_options.T38VendorInfo = NULL;
    f_session->t38_options.T38FaxRateManagement = NULL;

    f_session->rem_addr = NULL;
    f_session->socket_fd = 0;
}


void fax_params_destroy(fax_session_t *f_session)
{
    printf("fax: %s: Destroing fax parameters\n", __func__);

    if (f_session->pvt.t38_state) free(f_session->pvt.t38_state);
    if (f_session->pvt.udptl_state) free(f_session->pvt.udptl_state);

    if (f_session->pvt.header) free(f_session->pvt.header);
    if (f_session->pvt.ident) free(f_session->pvt.ident);

    if (f_session->t38_options.T38FaxUdpEC) free(f_session->t38_options.T38FaxUdpEC);
    if (f_session->t38_options.T38VendorInfo) free(f_session->t38_options.T38VendorInfo);
    if (f_session->t38_options.T38FaxRateManagement) free(f_session->t38_options.T38FaxRateManagement);

    if (f_session->rem_addr) free(f_session->rem_addr);
}


void fax_params_set_default(fax_session_t *f_session)
{
    printf("fax: %s: Setting default fax parameters\n", __func__);
    char ident_str[256];

    f_session->pvt.disable_v17 = DEF_FAX_DISABLE_V17;
    f_session->pvt.done = 0;

    sprintf(ident_str, "%s: %s %s", DEF_FAX_IDENT, f_session->call_id,
            f_session->pvt.caller ? "sender" : "receiver");
    f_session->pvt.ident = strdup(ident_str);

    f_session->pvt.header = strdup(DEF_FAX_HEADER);
    f_session->pvt.verbose = DEF_FAX_VERBOSE;
    f_session->pvt.use_ecm = DEF_FAX_USE_ECM;

    f_session->transport_mode = FAX_TRANSPORT_T38_MOD;

    f_session->t38_options.T38FaxVersion = DEF_T38_FAX_VERSION;
    f_session->t38_options.T38FaxMaxBuffer = DEF_T38_MAX_BUFFER;
    f_session->t38_options.T38FaxMaxDatagram = DEF_T38_MAX_DATAGRAM;
    f_session->t38_options.T38FaxRateManagement = strdup(DEF_T38_RATE_MANAGEMENT);
    f_session->t38_options.T38FaxTranscodingJBIG = DEF_T38_TRANSCODING_JBIG;
    f_session->t38_options.T38FaxTranscodingMMR = DEF_T38_TRANSCODING_MMR;
    f_session->t38_options.T38FaxUdpEC = strdup(DEF_T38_UDP_EC);
    f_session->t38_options.T38MaxBitRate = DEF_T38_MAX_BITRATE;
    f_session->t38_options.T38VendorInfo = strdup(DEF_T38_VENDOR_INFO);

    sprintf(f_session->log_tag, "%s-%s", f_session->call_id,
            f_session->pvt.caller ? "SENDER" : "RECVER");
}


























