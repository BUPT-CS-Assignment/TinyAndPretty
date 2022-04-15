#include <Network/HttpServer.h>
#include <Network/URLParser.h>
#include <Network/HttpException.h>

HttpServer::HttpServer()
{
	sock   = std::make_unique<Socket>();
	epool  = std::make_unique<EventPool>();
	thpool = std::make_unique<ThreadPool>(CONFIG_THREADS_MAXIMUM);

	epool->mountFD(sock->getFD(), EPOLLIN | EPOLLET);
}
// fd conn bug!
HttpResponse *dispatchException(const HttpException &e);

HttpResponseBase *taskExecute(Connection *conn, std::shared_ptr<uint8_t> raw, size_t len)
{
	try
	{
		if (len == -1ULL)
			throw HttpException::OUT_OF_LIMIT;
		else if (len == 0)
			throw HttpException::NON_CONN;

#ifdef DEBUG
		std::cerr << "*Data Size :\t" << len << "\n";
#endif

		HttpRequest request{conn, raw.get(), len};
		auto &entry = URLParser::getInstance().URLparse(request.Path());
		return entry(request);
	}
	catch (const HttpException &e)
	{
		return dispatchException(e);
	}
}

void HttpServer::createHttpTask(Connection *conn)
{
	thpool->enqueue([=] { // sock also =?

#ifdef DEBUG
	std::cerr << "\n*FD IN : \t" << conn->getFD() << "\n";
#endif

		auto [raw  , rlen] = sock->recvData(conn->getFD()) ;
		std::unique_ptr<HttpResponseBase> ret ( taskExecute(conn, raw, rlen) );

		if(ret != nullptr)  
		{
			auto [buff , slen] = ret->stringize();
			sock->sendData(conn->getFD(), buff.get(), slen);
		}

		delete conn; // near future , CURRENTLY long connection is not supported

#ifdef DEBUG
	std::cerr << "##### Connection offline. #####\n";
#endif

	} );
}

void HttpServer::start()
{
	epool->Loop([&](epoll_data_t data, int type)
				{
		if(data.fd == sock->getFD()) {
			while(true) {
				Connection* con = sock->onConnect();
				if(con == nullptr) break;

#ifdef DEBUG
				std::cerr << "Connection : " << con->getFD() << " Handshake!\n";
#endif

				epool->mountPtr(con , con->getFD() , EPOLLIN | EPOLLET);
			}
		}else if (type & EPOLLIN) {
			createHttpTask( static_cast<Connection *>(data.ptr) );
		} });
}
