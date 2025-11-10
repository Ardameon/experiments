> [!note] tcp_listener_struct
```c
struct tcp_listener
{
    pjsip_tpfactory          factory;
    pj_bool_t                is_registered;
    pjsip_endpoint          *endpt;
    pjsip_tpmgr             *tpmgr;
    pj_activesock_t         *asock;
    pj_sockaddr              bound_addr;
    pj_qos_type              qos_type;
    pj_qos_params            qos_params;
    pj_sockopt_params        sockopt_params;
    pj_bool_t                reuse_addr;        
    unsigned                 async_cnt;    
    unsigned                 initial_timeout;

    uint32_t                 transport_id;
    /* Group lock to be used by TCP listener and ioqueue key */
    pj_grp_lock_t           *grp_lock;
};
```

Сначала создаётся `listener` для TCP и регистрируется в `ioqueu` на `endpoint`:

>[!note] создание транспорта
```c
/* custom function (SIPBL) */
tcp_tranpsort_create() 
 /* creating new TCP transport */
 pjsip_tcp_transport_start3(sip_endpt, &tcp_cfg, &tcp_tp_factory) 
  /* Here listener with factory created */
  struct tcp_listener *listener;
  pjsip_tcp_transport_lis_start(factory, bind_addr, addr_name)
   /* Creating listening socket */
   pj_sock_socket()
   pj_sock_setsockopt()
   pj_sock_bind()
   pj_sock_listen()
   /* Create activesock to handle incoming connections by ioqueue */
   listener_cb.on_accept_complete = &on_accept_complete;
   pj_activesock_create(sock, endpt_ioqueue, &listener_cb, listener)
    ioq_cb.on_accept_complete = &ioqueue_on_accept_complete;
    pj_ioqueue_register_sock2()
   pj_activesock_start_accept()
  pjsip_tpmgr_register_tpfactory(tpmgr, factory);
```

Затем `ioqueue` видя новую коннекцию вызывает `accept`:

>[!note] приём нового TCP-соединения
```c
/* This function is called on poll() has new read events */
ioqueue_dispatch_read_event()
 pj_sock_accept()
 /* on_accept_complete() cb invoked */
 /* This is from activesock.c */
 ioqueue_on_accept_complete()
  /* This is from sip_transport_tcp.c */
  on_accept_complete()
   /* Create TCP connection */
   tcp_create(&tcp)
    /* Create activesock to handle new tcp connection by ioqueue */
    tcp_cb.on_data_read = &on_data_read;
    tcp_cb.on_data_sent = &on_data_sent;
    tcp_cb.on_connect_complete = &on_connect_complete;
	
    pj_activesock_create(sock, endpt_ioqueue, &tcp_cb, tcp)
	 ioq_cb.on_read_complete = &ioqueue_on_read_complete;
     ioq_cb.on_write_complete = &ioqueue_on_write_complete;
     ioq_cb.on_connect_complete = &ioqueue_on_connect_complete;
	 pj_ioqueue_register_sock2()
    pjsip_transport_register()
   /* Start reading from new TCP connection */
   tcp_start_read(tcp)
    pj_activesock_start_read2()
```

Чтение из нового TCP-сокета происходит так:

>[!note]- чтение данных из сокета созданной коннекции
```c
ioqueue_dispatch_read_event()
 pj_sock_recv()
 /* on_read_complete() cb invoked */
 /* This is from activesock.c */
 ioqueue_on_read_complete()
  /* This is from sip_transport_tcp.c */
  on_data_read()
   pjsip_tpmgr_receive_packet()
    /* Here we check receiving of full SIP message */
    pjsip_find_msg()
    /* Here full SIP message parsing happens */
    pjsip_parse_rdata()
    /* on_rx_msg() cb invoked */
	endpt_on_rx_msg()
	 pjsip_endpt_process_rx_data()
      /* Invoke modules rx callbacks */
	  mod->on_rx_request()
```