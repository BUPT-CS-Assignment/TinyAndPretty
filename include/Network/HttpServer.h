#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <common.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <interfaces.h>

class HttpServer {
// private :
//     static TPHttpServer server;
private :   
    int sockfd , port , listen_q_max;
    struct sockaddr_in *address;
    
    inline void __close_socket() { delete address; close(sockfd); };

public:
    HttpServer(uint16_t _port , uint32_t _listen_q_max);
    ~HttpServer();
    void start();
    void stop();
};

class HttpTask {
public :
}


#endif