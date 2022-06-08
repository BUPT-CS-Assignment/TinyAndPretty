class HttpRequest;
class HttpResponseBase;

namespace Timer
{

extern void setHeartResponse(HttpResponseBase* reps);

extern void createComet(HttpRequest& req);

extern void launchBroadCast(HttpResponseBase* reps);


};