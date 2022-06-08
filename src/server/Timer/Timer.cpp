#include <connect/Timer/Timer.h>
#include <connect/Timer/TimerManager.h>
#include <connect/HttpProtocal/HttpRequest.h>


void Timer::createComet(HttpRequest& req)
{
	TimerManager::getInstance()->listenConnect(req.getConnection());
} 
void Timer::launchBroadCast(HttpResponseBase* reps)
{
	std::shared_ptr<HttpResponseBase> _reps {reps};
	TimerManager::getInstance()->broadCast(_reps);
}
