#ifndef __MANAGER_BASE__
#define __MANAGER_BASE__

#include <memory>
#include "ServerBase.h"

class ManagerBase
{
protected :
	std::shared_ptr<Socket> sock;
public:
	void setSock(std::shared_ptr<Socket> _sock) {sock = _sock;}
	virtual bool protocalConfirm() = 0;
	virtual bool createTask(Connection* conn) = 0;
};

#endif