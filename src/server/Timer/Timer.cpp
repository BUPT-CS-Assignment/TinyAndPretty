#include <connect/Timer/Timer.h>
#include <connect/Timer/TimeLine.h>
#include <connect/Timer/TimerManager.h>
#include <connect/HttpProtocal/HttpRequest.h>

#include <libs/BasicUtils.hpp>
using namespace UTILSTD;

/*------------------ TimeManager ------------------*/

HttpResponseBase* Timer::createComet(
	HttpRequest& req , 
	HttpResponseBase* fail_res)
{
	if (TimerManager::getInstance()
			->listenConnect(req.getConnection())
	)	{delete fail_res;  return nullptr;}
	
	else {
		return fail_res;
	}
} 
void Timer::launchBroadCast(HttpResponseBase* reps)
{
	std::shared_ptr<HttpResponseBase> _reps {reps};
	TimerManager::getInstance()->broadCast(_reps);
}


void Timer::setHeartResponse(HttpResponseBase* reps) 
{

	std::shared_ptr<HttpResponseBase> _reps {reps};

	TimerManager::getInstance()->modifyHeartResp(_reps);
}

/*------------------ TimeLine ------------------*/

static TimeLine tline;

void Timer::changeTimeLineRate(uint32_t new_R)
{
	tline.changeRate(new_R);
}

uint32_t Timer::getTimeLineRate(void)
{
	auto ret =  tline.getRate();
	return ret;
}

/* Format : Year / Month / Day / Weekday / Hour / Minutes / Seconds */ 
const std::vector<int> Timer::getVirtualTime(void)
{
	auto ret =  tline.getVirtualTime();
	return ret;
}