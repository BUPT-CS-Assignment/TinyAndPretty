#ifndef __TIMER__
#define __TIMER__

#include <common.h>

class HttpRequest;
class HttpResponseBase;

namespace Timer
{

extern void setHeartResponse(HttpResponseBase* reps);

extern HttpResponseBase* createComet(HttpRequest& req , HttpResponseBase* fail_res);

extern void launchBroadCast(HttpResponseBase* reps);

extern void changeTimeLineRate(uint32_t new_R);

extern uint32_t getTimeLineRate(void);

extern const std::vector<int> getVirtualTime(void);

};

#endif