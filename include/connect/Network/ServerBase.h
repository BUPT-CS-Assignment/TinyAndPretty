#ifndef __SERVER_BASE__
#define __SERVER_BASE__

#include <common.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <functional>

#include "ServerConfig.h"

class Connection {
    bool close_flag;
    int connfd;
    socklen_t len;
    struct sockaddr_in client_addr;
public:
    Connection(int _connfd , socklen_t _len ,struct sockaddr_in _addr) 
            : close_flag(false) ,  connfd(_connfd) , len(_len) , client_addr(_addr){}

    int  getFD()        const {return connfd;}
    bool getCloseFlag() const {return close_flag;}
    void setCloseFlag()       {close_flag = true;}
    void closeFD()            {close(connfd);}
    const char*getAddr()const {return inet_ntoa(client_addr.sin_addr);}
};

/* unix socket */
class Socket {
    int sockfd ;

    struct sockaddr_in address;
public : 
    Socket();
    ~Socket();

    // get socket file descriptor in unix
    int getFD() const {return sockfd;}

    // recv raw data in the format of byte stream
    size_t recvData(int _connfd , uint8_t **data ,int flags = 0,size_t _buff_size = BUFF_INIT_SIZE);
    // block recv but don't flush buffer 
    size_t recvPeekData(int _connfd , uint8_t **data);
    /* non-block recv */
    size_t recvNonBlockData(int _connfd , uint8_t **data);

    size_t recvCertainData(int _connfd , uint8_t **data , size_t len);
    // send raw data in the format of byte stream
    size_t sendData(int _connfd , uint8_t *data , size_t len);
    // send file (FULL path needed)
    size_t sendFile(int _connfd , const char* _fpath);
    // send file (FULL path needed) with header
    size_t sendFileWithHeader(int _connfd , const char* _fpath , uint8_t *header ,  size_t header_len);
    
    // get one connection in TCP socket
    Connection* onConnect();
};

using EpollFunc = std::function<void(epoll_data_t , int)>;
/* unix event pool based on epoll */
class EventPool{
    int epfd;

    struct epoll_event events[MAX_EVENTS];
public:
    EventPool();
    bool mountFD(int fd , uint32_t type);
    bool mountPtr(void *ptr , int fd, uint32_t type);
    bool modifyPtr(void *ptr , int fd, uint32_t type);
    void Poll( EpollFunc &func );
    void Loop( EpollFunc  func );
};

#define NETERROR(cond , tar) {if((cond)) {perror( tar );exit(EXIT_FAILURE);}}


#endif