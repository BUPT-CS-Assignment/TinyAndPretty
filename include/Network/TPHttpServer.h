#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <common.h>

#include "StateCode.h"

class TPHttpServer {
// private :
//     static TPHttpServer server;
private :   
    int sockfd , port , listen_q_max;
    struct sockaddr_in *address;
    
    volatile StateCode cur_state;


    inline void __close_socket() { close(sockfd); };

public:
    TPHttpServer(int _port , int _listen_q_max);
    ~TPHttpServer();
    StateCode start();
    StateCode stop();
    inline StateCode get_cur_state(){ return cur_state; };
};

#endif