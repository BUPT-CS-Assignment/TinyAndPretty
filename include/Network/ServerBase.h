#ifndef __SERVER_BASE__
#define __SERVER_BASE__

#include <common.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include <functional>

#define IPV_4           AF_INET
#define PROTO_FAMILT    IPV_4
#define HOST_ADDR       INADDR_ANY
#define SOCK_TYPE       SOCK_STREAM | SOCK_NONBLOCK
#define LISTEN_Q_MAX    16
#define ADDR_REUSE      1
#define RECV_TIMEOUT    {0,500000}
#define SEND_TIMEOUT    {0,500000}
#define MAX_EVENTS      128
#define BUFF_INIT_SIZE  4096
#define BUFF_MAX_SIZE   134217728 // (1024*1024*128) bytes
#define TIME_CLIP       30000

class Connection {
    bool close_flag;
    int connfd;
    socklen_t len;
    struct sockaddr_in client_addr;
public:
    Connection(int _connfd , socklen_t _len ,struct sockaddr_in _addr) : close_flag(false) ,  connfd(_connfd) , len(_len) , client_addr(_addr){}
    ~Connection() {close(connfd);}

    int  getFD()        const {return connfd;}
    char*getAdress()    const {return inet_ntoa(client_addr.sin_addr);}
    bool getCloseFlag() const {return close_flag;}
    void setCloseFlag()       {close_flag = true;}
};

class Socket {
    int sockfd ;
    int addr_reuse;
    uint16_t port;

    struct sockaddr_in address;
    struct timeval recv_timeout;
    struct timeval send_timeout;
public : 
    Socket(uint16_t _port);
    ~Socket();
    size_t recvData(int _connfd , uint8_t **data);
    void sendData(int _connfd , uint8_t *data , size_t len );
    Connection* onConnect();
    int getFD() const {return sockfd;}
};

class EventPool{
    int epfd;

    struct epoll_event events[MAX_EVENTS];
public:
    EventPool();
    bool mountFD(int fd , uint32_t type);
    bool mountPtr(void *ptr , int fd, uint32_t type);
    void Poll( std::function<void(epoll_data_t , int)> &func );
    void Loop( std::function<void(epoll_data_t , int)> func );
};

#define NETERROR(cond , tar) {if((cond)) {perror( tar );exit(EXIT_FAILURE);}}


#endif