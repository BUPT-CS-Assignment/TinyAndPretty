#include <common.h>
#include "../Network/ManagerBase.h"


/* TimerTask center manager */
class TimerManager : public ManagerBase
{
	std::vector<Connection *> connPool;

public:
	virtual bool protocalConfirm(const int magic_n) override;
	virtual bool createTask(Connection* conn) override;
};