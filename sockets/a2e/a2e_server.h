#ifndef __A2E_SERVER_H__
#define __A2E_SERVER_H__

#include "a2e_def.h"
#include "a2e_iface.h"

#define SOCK_PATH_LEN_MAX 96

typedef struct
{
    a2e_t base;
    a2e_strategy_i iface;

    char sock_path[SOCK_PATH_LEN_MAX + 1];

    int loc_fd;
    int rem_fd;

    void *req;
    uint32_t req_size;
} a2e_server_t;

a2e_strategy_i a2e_server_iface_get(void);

#endif /* __A2E_SERVER_H__ */

#if 0
static void *sorm_med_thread_connection_routine(void *arg)
{
	sorm_med_t *sm_info = (sorm_med_t *)arg;
	struct sockaddr_un local_addr;
	int reuse = 1;
	struct stat st;
	stMspPresence *msp_presence = mspGetDevicePresence();
	stSormMediator *sorm_mediator_cfg = config_object_ptr(CFGO_SORM_MEDIATOR, 0);

	local_addr.sun_family = AF_UNIX;
	strcpy(local_addr.sun_path, MGAPP_TO_SORM_SOCK_PATH);

	/* If socket file already exists, need to remove it */
	if(!stat(MGAPP_TO_SORM_SOCK_PATH, &st))
	{
		unlink(MGAPP_TO_SORM_SOCK_PATH);
	}

	/* Wait if presence check is not already complete */
	while(!msp_presence->cnt)
	{
		app_pause(500);
	}

	/* Update msp usage map and falc used */
	config_update_submodule_usage(sorm_mediator_cfg, msp_presence);

	if((sm_info->loc_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		app_trace(TRACE_ERR, "cfg: can't create sorm mediator"
				  " interaction socket: %s", strerror(errno));
		goto _error_exit;
	}

	setsockopt(sm_info->loc_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if(bind(sm_info->loc_sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
	{
		app_trace(TRACE_ERR, "cfg: can't bind sorm mediator"
				  " interaction socket: %s", strerror(errno));
		goto _error_exit;
	}

	app_trace(TRACE_INFO, "cfg: sorm mediator interaction socket created");

	if(listen(sm_info->loc_sock, 1) < 0)
	{
		app_trace(TRACE_ERR, "cfg: can't listen sorm mediator"
				  " interaction socket: %s", strerror(errno));
		goto _error_exit;
	}

	sm_info->state = E_SORM_MED_STATE_WAIT_CONN;

	if((sm_info->rem_sock = accept(sm_info->loc_sock,
								   NULL, NULL)) < 0)
	{
		app_trace(TRACE_ERR, "cfg: can't accept sorm mediator"
				  " interaction connection: %s", strerror(errno));
		goto _error_exit;
	}

	sm_info->state = E_SORM_MED_STATE_CONNECTED;

	app_trace(TRACE_INFO, "cfg: sorm mediator connection accepted (rem_sock %d)",
			  sm_info->rem_sock);

	/* Send sorm mediator general config */
	sorm_med_send_general_config();

	/* Send submodule usage information  */
	config_sorm_med_send_submodule_usage();

	/* Restart rtpproxy */
	config_rtp_proxy_release();

	fcntl(sm_info->rem_sock, F_SETFL, O_NONBLOCK);

	while(sorm_med_alive())
	{
		sleep(1);
	}

	app_trace(TRACE_INFO, "cfg: sorm mediator doesn't response. Restart!");

	config_sorm_med_release();
	config_sorm_med_init();


	pthread_exit(NULL);

_error_exit:

	if(sm_info->loc_sock > -1)
	{
		close(sm_info->loc_sock);
		sm_info->loc_sock = -1;
		unlink(MGAPP_TO_SORM_SOCK_PATH);
	}

	pthread_exit(NULL);
}
#endif
