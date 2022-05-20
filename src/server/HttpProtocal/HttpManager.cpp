#include <connect/HttpProtocal/HttpManager.h>
#include <connect/Network/URLParser.h>
/* Judge by clip */ 
bool HttpManager::protocalConfirm() 
{
	return true;//currently only itself , so return true default
}


void HttpManager::createTask(Connection* conn)
{
	//get full data from socket
	auto [raw, rlen] = wrapper->recvHttpData(conn);
	//execute
	std::shared_ptr<HttpResponseBase> ret(taskExecute(conn, raw, rlen));

	wrapper->sendHttpData( conn , ret );
}


HttpResponseBase *HttpManager::taskExecute(
	Connection* conn, 
	std::shared_ptr<uint8_t> raw, 
	size_t len)
{
	try {
		if (len == -1ULL)
			throw HttpException::OUT_OF_LIMIT;
		else if (len == 0)
			throw HttpException::NON_CONN;

		HttpRequest request {conn, raw.get(), len};
		auto& entry = URLParser::getInstance().URLparse(request.Path());
		return entry(request);

	} catch (const HttpException &e) {
		return dispatchException(e);
	}
}