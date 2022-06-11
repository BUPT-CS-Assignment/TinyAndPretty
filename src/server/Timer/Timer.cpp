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
    CONSOLE_LOG(0,1,1,"Clock::createComet\n");
	if (TimerManager::getInstance()
			->listenConnect(req.getConnection())
	)	{delete fail_res;  
		CONSOLE_LOG(0,1,1,"Clock::createSuccess\n");
		return nullptr;}
	
	else {
		CONSOLE_LOG(0,1,1,"Clock::RepeatConnetion\n");
		return fail_res;
	}
} 
void Timer::launchBroadCast(HttpResponseBase* reps)
{
	CONSOLE_LOG(0,1,1,"Clock::launchBroadCast\n");
	std::shared_ptr<HttpResponseBase> _reps {reps};
	TimerManager::getInstance()->broadCast(_reps);
	CONSOLE_LOG(0,1,1,"Clock::launchSuccessful\n");

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
	CONSOLE_LOG(0,1,1,"Clock::changeTimeLineRate\n");

	tline.changeRate(new_R);

	CONSOLE_LOG(0,1,1,"Clock::changeSuccessful\n");

}

uint32_t Timer::getTimeLineRate(void)
{
	CONSOLE_LOG(0,1,1,"Clock::getTimeLineRate\n");
	auto ret =  tline.getRate();
	CONSOLE_LOG(0,1,1,"Clock::getSuccess\n");
	return ret;
}

const std::vector<int> Timer::getVirtualTime(void)
{
	CONSOLE_LOG(0,1,1,"Clock::getVirtualTime\n");

	auto ret =  tline.getVirtualTime();
	CONSOLE_LOG(0,1,1,"Clock::getSuccess\n");
	return ret;
}