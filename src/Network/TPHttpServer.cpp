#include <Network/TPHttpServer.h>


TPHttpServer::TPHttpServer(int _port , int _listen_q_max) {
    port = _port , listen_q_max = _listen_q_max;
    sockfd = socket(PF_INET , SOCK_STREAM , 0);
    ERROR(sockfd >= 0 , "create socket error!");

    address  = new sockaddr_in {
        sin_family         : PF_INET           , 
        sin_port           : htons(port)       ,
        sin_addr  : {s_addr : htonl(INADDR_ANY)} ,
    };
    ERROR(
        bind(sockfd , (sockaddr *)(address) , sizeof(sockaddr_in)) >= 0
    , "create socket error!");

    ERROR(
        listen(sockfd , listen_q_max) >= 0
    , "liten socket error!");
}

static const char fault[] = 
"HTTP/1.1 200 OK\n"
"Date: Wed, 10 Aug 2020 13:13:13 GMT\n"
"Connection: Keep-Alive\n"
"Content-Type: text/html\n\n"
"<h1>Hello World!</h1>"
;
StateCode TPHttpServer::start() {
    cur_state = RUNNING;
    while(cur_state == RUNNING) {
        int connfd = accept(sockfd ,(struct sockaddr*)NULL, NULL);
        ERROR(connfd >= 0 , "accept socket error!");

        char buff[1024] = {0};
        int n = recv(connfd , buff , 1024 , 0);
        printf("Data Size : %d\n%s\n" ,n , buff);
        send(connfd , fault , sizeof(fault), 0);
        close(connfd);
    }
    return RUNNING;
}

StateCode TPHttpServer::stop() { __close_socket(); return STOP;}
TPHttpServer::~TPHttpServer()  { __close_socket();}