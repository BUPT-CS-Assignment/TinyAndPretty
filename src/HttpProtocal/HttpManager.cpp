#include <HttpProtocal/HttpManager.h>
#include <Network/URLParser.h>

bool HttpManager::protocalConfirm() 
{
	return true;
}


void HttpManager::createTask(Connection* conn)
{
	std::cerr << "connFD:"<<conn->getFD() << "\n";
	auto [raw, rlen] = sock->recvData(conn->getFD());
	std::unique_ptr<HttpResponseBase> ret(taskExecute(conn, raw, rlen));

	if (ret != nullptr)
	{
		auto [buff, slen] = ret->stringize();
		slen = sock->sendData(conn->getFD(), buff.get(), slen);

		if(slen == -1ULL) conn->closeFD();
	}
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