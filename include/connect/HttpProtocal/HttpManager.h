#ifndef __HTTP_MANAGER__
#define __HTTP_MANAGER__

#include <common.h>
#include <connect/Network/ManagerBase.h>
#include <connect/HttpProtocal/HttpException.h>
#include <connect/HttpProtocal/HttpProtocal.h>
#include <connect/HttpProtocal/HttpAdapter.h>

/* HttpTask center manager */
class HttpManager : public ManagerBase
{
	//specialize http io through adapter
	std::unique_ptr<HttpAdapter> wrapper = std::make_unique<HttpAdapter>(sock);

	HttpResponse *dispatchException(const HttpException& e);
	HttpResponseBase *taskExecute(Connection* conn, std::shared_ptr<uint8_t> raw, size_t len);
public:

	virtual bool protocalConfirm(const int magic_n) override;
	virtual bool createTask(Connection* conn) override;
};

#endif