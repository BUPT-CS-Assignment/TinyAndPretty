#include <connect/Timer/TimerManager.h>
#include <connect/HttpProtocal/HttpAdapter.h>

bool TimerManager::protocalConfirm(const int magic_n) {
	return (magic_n == TIME_MAGICNUM);
}


TimerManager* TimerManager::getInstance()
{
	if (myself == nullptr) {
		std::cerr << "TimerManager HasNot Registered Yet! [TimerManager.cpp]\n";
		std::abort();
	}

	return myself;
}

TimerManager::TimerManager() 
{
	if (myself != nullptr) {
		std::cerr << "TimerManager MutiConstructed! [TimerManager.cpp]\n";
		std::abort();
	}


	heart_q = std::shared_ptr<HttpResponse> 
				{ new HttpResponse{"60s HB"} };
	wrapper = std::make_unique<HttpAdapter>(sock);
	myself  = this;
}

void TimerManager::listenConnect(Connection *conn)
{

	int tfd = epool->createTimerEvent({
		TIME_MAGICNUM ,
		0 ,
		conn ,
		EPOLLIN | EPOLLET | EPOLLONESHOT
	} , std::chrono::seconds{20});

	connPool.emplace(conn , tfd);
}

void TimerManager::broadCast(Rep_T reps)
{

	for(auto [conn , tfd] : connPool) {
		wrapper->sendHttpData(conn , reps);

		::close(tfd);
	}

	connPool.clear();
}


bool TimerManager::createTask(Connection* conn)
{
	//std::cerr << "Timer Hit : " << connPool[conn] << std::endl;
	if(connPool[conn] == 0) return false;
	
	::close(connPool[conn]);

	wrapper->sendHttpData(conn , heart_q);

	connPool.erase(conn);

	return false;
}