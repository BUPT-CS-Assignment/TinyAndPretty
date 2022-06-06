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

	void start();
	void distributeTask(EventChannel *eptr);
	static auto getInstance() {
		static std::shared_ptr<TAPCenter> ptr {new TAPCenter};
		return ptr;
	}
};

TAPCenter::TAPCenter()
{
	sock   = std::make_shared<Socket>();
	epool  = std::make_unique<EventPool>();
	thpool = std::make_unique<ThreadPool>(CONFIG_THREADS_MAXIMUM);

	epool->mountEvent( {
		SOCK_MAGICNUM , 
		sock->getFD() ,
		nullptr ,  
		EPOLLIN
	} );
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

void TAPCenter::distributeTask(EventChannel *eptr)
{
	for(auto& ptr : plugins) {

		// sock also =? why must = ?=
		thpool->enqueue([& , _eptr = eptr] 
		{ 
			if( !ptr->protocalConfirm(_eptr->magic_n) ) return ;
			auto conn = static_cast<Connection *>(_eptr->ptr);

			// create Http task and judge whether it's alive
			if ( ptr->createTask(conn) ) 
				epool->modifyEvent( {
					_eptr->magic_n,
					conn->getFD(),
					conn,
					EPOLLIN | EPOLLET | EPOLLONESHOT
				} ); 
			// peer socket close
			else {
				sock->offConnect(conn);
				// NEED TEST!!!
				epool->removeEvent(_eptr);
			}
				
		});
	}
}

void TAPCenter::start()
{
	epool->Loop([&](EventChannel* event)
				{
		auto type = event->type;
		std::cerr <<  "NIGHT : " << event->magic_n << "\n";
		// motivated by socket-fd, new connection arrive
		if(event->magic_n == SOCK_MAGICNUM) {

			for(;;) {
				// handled every new connection
				Connection* conn = sock->onConnect();
				if(conn == nullptr) break;

				// start listening connection
				epool->mountEvent( {
					HTTP_MAGICNUM, 
					conn->getFD(),
					conn,
					EPOLLIN | EPOLLET | EPOLLONESHOT
				} );
			}

		// connection timeout. close forcely
		} else if (type & EPOLLHUP || type & EPOLLERR || type & EPOLLRDHUP) {
			auto conn = static_cast<Connection *>(event->ptr);
			sock->offConnect(conn); // another strage : by time out
			epool->removeEvent(event);

		// data from a connection arrived
		} else if (type == EPOLLIN) {
			// also move ownership to work-thread
			this->distributeTask(event);

		} 
	});
}
