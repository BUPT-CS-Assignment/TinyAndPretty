#ifndef __HTTP_MANAGER__
#define __HTTP_MANAGER__

#include <common.h>
#include <Network/ManagerBase.h>
#include <HttpProtocal/HttpException.h>
#include <HttpProtocal/HttpProtocal.h>

class HttpManager : public ManagerBase
{
public:
	HttpResponse *dispatchException(const HttpException &e);
	HttpResponseBase *taskExecute(Connection* conn, std::shared_ptr<uint8_t> raw, size_t len);

	virtual bool protocalConfirm() override;
	virtual void createTask(Connection* conn) override;
};
#define def_HttpEntry(name) HttpResponseBase* (name)(HttpRequest &request) 


#endif