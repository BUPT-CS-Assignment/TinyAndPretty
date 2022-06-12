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

	Json j; 
	j.push_back({"code" , -1});
	j.push_back({"msg" , "60s pass by....HeartBeat Packet"});

	heart_q = Rep_T { new JsonResponse{j, HTTP_STATUS_202} };
	wrapper = std::make_unique<HttpAdapter>(sock);
	myself  = this;
}

bool TimerManager::listenConnect(Connection *conn)
{
	if (connPool.count(conn) != 0) return false;

	int tfd = epool->createTimerEvent({
		TIME_MAGICNUM ,
		0 ,
		conn ,
		EPOLLIN | EPOLLET | EPOLLONESHOT
	} , std::chrono::seconds{HEART_TIMEOUT});

	connPool.emplace(conn , tfd);

	return true;
}

void TimerManager::modifyHeartResp(Rep_T new_heartq)
{
	heart_q.reset();

	heart_q = new_heartq;
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

	if(connPool.count(conn) == 0) return false;
	
	::close(connPool[conn]);

	wrapper->sendHttpData(conn , heart_q);

	connPool.erase(conn);

	return false;
}