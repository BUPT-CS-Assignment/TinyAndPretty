#include <common.h>
#include <connect/HttpProtocal/HttpResponse.h>
#include "../Network/ManagerBase.h"

class HttpAdapter;
/* TimerTask center manager */
class TimerManager : public ManagerBase
{
	using Rep_T = std::shared_ptr<HttpResponseBase>;
public:
	TimerManager();

	static TimerManager* getInstance();

	void listenConnect(Connection *conn);
	void broadCast(Rep_T reps);

	virtual bool protocalConfirm(const int magic_n) override;
	virtual bool createTask(Connection* conn) override;

private :
	TimerManager(const TimerManager&) = delete;
	TimerManager(const TimerManager&&) = delete;
	TimerManager& operator = (const TimerManager&) = delete;
	TimerManager& operator = (const TimerManager&&)= delete;

	inline static TimerManager* myself;
	inline static Rep_T heart_q;

	std::unordered_map<Connection * , int> connPool;
	std::unique_ptr<HttpAdapter> wrapper;
};