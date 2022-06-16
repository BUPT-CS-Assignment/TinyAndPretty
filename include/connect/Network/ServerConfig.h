#ifndef __SERVER_CONF__
#define __SERVER_CONF__

#include "../../generated/autoconf.h"

/* ------------------------Auto Configure----------------------------*/
#define PORT            CONFIG_PORT
#define LISTEN_Q_MAX    CONFIG_LINTEN_Q_MAX
#define RECV_TIMEOUT    {CONFIG_RECV_SECOND,CONFIG_RECV_MICROSECOND}
#define SEND_TIMEOUT    {CONFIG_SEND_SECOND,CONFIG_SEND_MICROSECOND}
#define MAX_EVENTS      CONFIG_MAX_EVENTS
#define BUFF_INIT_SIZE  CONFIG_BUFF_INIT_SIZE
#define BUFF_MAX_SIZE   CONFIG_BUFF_MAX_SIZE // (1024*1024*128) bytes


#ifdef CONFIG_IPV4
#define IPV_4           AF_INET
#define PROTO_FAMILT 	AF_INET
#endif

#ifdef CONFIG_ADDR_ANY
#define HOST_ADDR       INADDR_ANY
#endif

#ifdef CONFIG_ADDR_REUSE
#define ADDR_REUSE      1
#endif

/* ----------------------Manual Configure----------------------------*/
#define SOCK_TYPE       SOCK_STREAM | SOCK_NONBLOCK
#define SOCK_MAGICNUM 0x00
#define HTTP_MAGICNUM 0xAA
#define TIME_MAGICNUM 0xBB

#endif