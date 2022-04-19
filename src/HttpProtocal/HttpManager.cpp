#include <HttpProtocal/HttpManager.h>
#include <Network/URLParser.h>

////Judge by clip 
bool HttpManager::protocalConfirm() 
{
	return true;//currently only itself , so return true default
}


void HttpManager::createTask(Connection* conn)
{
	//get full data from socket
	std::cerr << conn->getFD() << "\n";
	auto [raw, rlen] = sock->recvData(conn->getFD());

	//execute
	std::unique_ptr<HttpResponseBase> ret(taskExecute(conn, raw, rlen));

	if (ret != nullptr)// successfully get response(even exception occurs)
	{
		//send back to socket 
		auto [buff, slen] = ret->stringize();
		slen = sock->sendData(conn->getFD(), buff.get(), slen);
	}
	conn->closeFD(); // near future
}


HttpResponseBase *HttpManager::taskExecute(Connection* conn, std::shared_ptr<uint8_t> raw, size_t len)
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

		HttpRequest request {conn, raw.get(), len};
		auto &entry = URLParser::getInstance().URLparse(request.Path());
		return entry(request);
	}
	catch (const HttpException &e)
	{
		return dispatchException(e);
	}
}