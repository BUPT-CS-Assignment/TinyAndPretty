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

	static auto getInstance() {
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

	epool->mountFD(sock->getFD(), EPOLLIN);
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
	for(auto& ptr : plugins) {

		// sock also =? why must = ?=
		thpool->enqueue([& , _conn = conn] 
		{ 
			if( !ptr->protocalConfirm() ) return ;

			// create Http task and judge whether it's alive
			if ( ptr->createTask(_conn) ) 
				epool->modifyPtr(_conn , _conn->getFD() , 
						EPOLLIN | EPOLLET | EPOLLONESHOT ); 
			// peer socket close
			else 
				sock->offConnect(_conn);
		});
	}
}

void TAPCenter::start()
{
	epool->Loop([&](epoll_data_t data, int type)
				{
		// motivated by socket-fd, new connection arrive
		if(data.fd == sock->getFD()) {

			for(;;) {
				// handled every new connection
				Connection* conn = sock->onConnect();
				if(conn == nullptr) break;

				// start listening connection
				epool->mountPtr(conn , conn->getFD() , 
						EPOLLIN | EPOLLET | EPOLLONESHOT );
			}

		// connection timeout. close forcely
		} else if (type & EPOLLHUP || type & EPOLLERR) {
			Connection* conn = static_cast<Connection *>(data.ptr);
			sock->offConnect(conn); // another strage : by time out

		// data from a connection arrived
		} else if (type == EPOLLIN) {
			// also move ownership to work-thread
			Connection* conn = static_cast<Connection *>(data.ptr);
			this->distributeTask(conn);

		} 
	});
}
