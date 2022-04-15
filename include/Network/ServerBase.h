#ifndef __SERVER_BASE__
#define __SERVER_BASE__

#include <common.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include <functional>

#include "ServerConfig.h"

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

    struct sockaddr_in address;
public : 
    Socket();
    ~Socket();
    size_t recvData(int _connfd , uint8_t **data);
    size_t sendData(int _connfd , uint8_t *data , size_t len );
    Connection* onConnect();
    int getFD() const {return sockfd;}

    auto recvData(int _confd)
        -> std::tuple<std::shared_ptr<uint8_t> , size_t > ;
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