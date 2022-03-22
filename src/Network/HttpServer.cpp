#include <Network/HttpServer.h>


HttpServer::HttpServer(uint16_t _port , uint32_t _listen_q_max) {
    port = _port , listen_q_max = _listen_q_max;
    sockfd = socket(PF_INET , SOCK_STREAM , 0);
    ERROR(sockfd >= 0 , "create socket error : %d!" , sockfd);

    address = new sockaddr_in {
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

void HttpServer::start() {
    //while(1) {
        int connfd = accept(sockfd ,(struct sockaddr*)NULL, NULL);
        ERROR(connfd >= 0 , "accept socket error!");

        char buff[2048] = {0};
        recv(connfd , buff , 1024 , 0);
        //printf("Data Size : %d\n%s\n" ,n , buff);
        try
        {
            HttpRequest request(buff);
            auto &entry = URLParser::getInstance().url_table[ request.Path() ];

            auto ret = entry(request);
            
            memset(buff , 0 , sizeof(buff));
            ret->stringize(buff);
            send(connfd , buff , sizeof(buff), 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        close(connfd);
    //}
}

void HttpServer::stop() { __close_socket();}
HttpServer::~HttpServer()  { __close_socket();}