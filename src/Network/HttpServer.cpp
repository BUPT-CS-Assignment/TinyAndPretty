#include <Network/HttpServer.h>
#include <Network/URLParser.h>
size_t HttpRecv(int _connfd , uint8_t **raw);

HttpServer::HttpServer(uint16_t _port) {
	sock  = std::make_unique<Socket>(_port);
	epool = std::make_unique<EventPool>();
	epool->mountFD( sock->getFD() , EPOLLIN | EPOLLET );
}
//fd conn bug!

void HttpServer::createHttpTask(Connection *conn) {
#ifdef DEBUG
	std::cerr << "\n*FD IN : \t" << conn->getFD() << "\n";
#endif
	HttpResponseBase *ret = nullptr;
	size_t len = 0;
	try {
		uint8_t *raw = nullptr;
		len = sock->recvData(conn->getFD() , &raw);
		if      (len == -1ULL) throw HttpException::OUT_OF_LIMIT;
		else if (len == 0)     throw HttpException::NON_CONN;
#ifdef DEBUG
		std::cerr << "*Data Size :\t" << len <<"\n";
#endif
		HttpRequest request {conn , raw , len};
		auto &entry = URLParser::getInstance().URLparse( request.Path() );
		ret = entry(request);
		
		delete raw;
	 } catch(const HttpException& e) {
		switch (e) //near future
		{
			case HttpException::ERROR_LEN :
				std::cerr << "ERROR_LEN" << "\n";
				ret = new HttpResponse{"Error occur"};
				break;
			case HttpException::OUT_OF_LIMIT :
				std::cerr << "OUT_OF_LIMIT" << "\n";
				ret = new HttpResponse{"Error occur"};				
			break;
			case HttpException::NON_PATH  :
				std::cerr << "NON_PATH" << "\n";
				ret = new HttpResponse{"Error occur"};
				break; 
			case HttpException::NON_CONN  :
				std::cerr << "NON_CONN" << "\n";
				delete conn; break; 
			default: 
				std::cerr << "ELSE" << (int)e<< "\n";
				break;
		}
	}

	uint8_t *buff = nullptr;
	len = ret->stringize(&buff);
	len = sock->sendData(conn->getFD() , buff , len );
	if(ret != nullptr) 	delete ret;
	
	delete conn; // near future , now long connection is not supported
#ifdef DEBUG
	std::cerr << "##### Connection offline. #####\n";
#endif 
}

void HttpServer::start() {
	epool->Loop( [&] (epoll_data_t data , int type) {
		if(data.fd == sock->getFD()) {
			while(true) {
				Connection* con = sock->onConnect();
				if(con == nullptr) break;
			#ifdef DEBUG
				std::cerr << "Connection : " << con->getFD() << "handshake!\n"; 
			#endif
				epool->mountPtr(con , con->getFD() , EPOLLIN | EPOLLET);
			}
		}else if (type & EPOLLIN) {
			createHttpTask( static_cast<Connection *>(data.ptr) );
		}
	} );
}
