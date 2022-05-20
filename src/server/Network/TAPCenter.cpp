#include <connect/Network/TAPManager.h>
#include <connect/Network/URLParser.h>
#include <connect/Network/ManagerBase.h>
#include <connect/Network/ThreadPool.h>
#include <connect/Network/ServerBase.h>

class TAPCenter
{
private:
	TAPCenter();
	TAPCenter(const TAPCenter &) = delete;
	TAPCenter(const TAPCenter &&) = delete;
	TAPCenter &operator=(const TAPCenter &) = delete;
	TAPCenter &operator=(const TAPCenter &&) = delete;

public:
	std::shared_ptr<Socket> sock;
	std::unique_ptr<EventPool> epool;
	std::unique_ptr<ThreadPool> thpool;
	std::vector<std::unique_ptr<ManagerBase>> plugins;

	static auto getInstance()
	{
		static std::shared_ptr<TAPCenter> ptr {new TAPCenter};
		return ptr;
	}
	void start();
	void distributeTask(Connection* conn);
};

TAPCenter::TAPCenter()
{
	sock   = std::make_shared<Socket>();
	epool  = std::make_unique<EventPool>();
	thpool = std::make_unique<ThreadPool>(CONFIG_THREADS_MAXIMUM);

	epool->mountFD(sock->getFD(), EPOLLIN );
}

TAPManager::TAPManager()
{
	ptr = TAPCenter::getInstance();
}
void TAPManager::start()
{
	ptr->start();
}

/* load additonal plugin */
void TAPManager::loadSubManager(std::unique_ptr<ManagerBase> sub)
{
	sub->setSock(ptr->sock);
	ptr->plugins.emplace_back(std::move(sub));
}

void TAPCenter::distributeTask(Connection* conn)
{
	IFDEBUG(std::cerr << "\n*FD IN : \t" << conn->getFD() << "\n");
	for(auto& ptr : plugins) 
	{
		thpool->enqueue([& , _conn = conn] { // sock also =? why must = ?=
			if( ptr->protocalConfirm() ){
				ptr->createTask(_conn);
				// move ownership back
				epool->modifyPtr(_conn , _conn->getFD() , EPOLLIN | EPOLLET | EPOLLONESHOT );
			}

		});
	}
}

void TAPCenter::start()
{
	epool->Loop([&](epoll_data_t data, int type)
				{
		IFDEBUG(std::cerr << "Type status : " << type << "\n");
		// motivated by socket-fd, new connection arrive
		if(data.fd == sock->getFD()) {
			for(;;) {
				Connection* con = sock->onConnect();
				// handled every new connection
				if(con == nullptr) break;

				IFDEBUG(std::cerr << "Connection : " << con->getFD() << " Handshake!\n");
				// start listening connection
				epool->mountPtr(con , con->getFD() , EPOLLIN | EPOLLET | EPOLLONESHOT );
			}

		// connection timeout. close forcely
		} else if (type & EPOLLHUP || type & EPOLLERR) {
			Connection* conn = static_cast<Connection *>(data.ptr) ;
			IFDEBUG( ::printf("I will try close %d" , conn->getFD()) );
			conn->closeFD();  // another strage : by time out

		// data from a connection arrived
		} else if (type == EPOLLIN) {
			// also move ownership to work-thread
			Connection* conn = static_cast<Connection *>(data.ptr) ;
			this->distributeTask( conn );

		} });
}
