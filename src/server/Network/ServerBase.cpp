#include <connect/Network/ServerBase.h>
#include <sys/sendfile.h>
#include <netinet/tcp.h>
#include <sys/timerfd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libs/BasicUtils.hpp>

Socket::Socket()
{
	// Initialize socket port
	sockfd = ::socket(PROTO_FAMILT, SOCK_TYPE, 0);
	NETERROR(sockfd < 0, "init socket error ");

// Set up host address
#ifdef IPV_4
	address = sockaddr_in {
		sin_family : PROTO_FAMILT,
		sin_port   : htons(PORT),
		sin_addr   : {s_addr : htonl(HOST_ADDR)},
	};
#elif IPV_6 // near future
#endif

// Permit to reuse address
#ifdef ADDR_REUSE
	int addr_reuse = ADDR_REUSE;
	NETERROR(
		::setsockopt(
			sockfd, 
			SOL_SOCKET, 
			SO_REUSEADDR, 
			&addr_reuse, 
			sizeof(int)) < 0
	, "set address reuse");
#endif

	//Set up timeout limit for receive and send
	struct timeval recv_timeout = (timeval)RECV_TIMEOUT;
	NETERROR(
	    ::setsockopt(
			sockfd,
			SOL_SOCKET,
			SO_RCVTIMEO,
			&recv_timeout
			,sizeof(timeval)) < 0
	 , "set timeVal error");

	struct timeval send_timeout = (timeval)SEND_TIMEOUT;
	NETERROR(
	    ::setsockopt(
			sockfd,
			SOL_SOCKET,
			SO_SNDTIMEO,
			&send_timeout,
			sizeof(timeval)) < 0
	, "set timeVal error");

	// Bind port to socket file
	NETERROR(
		::bind(
			sockfd, 
			(sockaddr *)(&address), 
			sizeof(address)) < 0
	, "bind error");

	// Set up max capacity of listening queue
	NETERROR(
		::listen(sockfd, LISTEN_Q_MAX) < 0
	, "listen error");

    
    UTILSTD::CONSOLE_LOG(true,"Successfully Listen on %d\n",PORT);

}

/* get one valid connection */
Connection* Socket::onConnect()
{
	struct sockaddr_in _addr {0};
	socklen_t _len = sizeof(_addr);
	int _connfd = ::accept(sockfd, (sockaddr *)&_addr, &_len);

	if(_connfd == -1) 
		{errno = 0; return nullptr;}
	else 
		return new Connection{_connfd, _addr};
}

/* close one invalid connection */
void Socket::offConnect(Connection* _conn) {

	if (::fcntl(_conn->getFD(), F_GETFD) == -1 || errno == EBADF) 
		{errno = 0; return ;}

	_conn->closeFD();
	delete _conn;
}

/* block recv with time out*/ 
size_t Socket::recvData (
	int _connfd, 
	uint8_t **data,
	int flags,
	size_t buff_size )
{
	//initial temporary length var
	size_t  cur 	  = 0;
	ssize_t buff_len  = 0;

	*data = static_cast<uint8_t *>( ::calloc(1 , buff_size) );
	//loop recv and adjust buff size dynamically
	while ( (buff_len = ::recv(_connfd, *data + cur, buff_size - cur, flags)) )
	{
		// handle errno occurs
		if (buff_len == -1) {
			if (errno == EAGAIN && !flags)	
										{errno = 0; break;}
			else if (errno == EINTR)	{errno = 0; continue;}
			else						{errno = 0; break;}
		}
		
		cur += buff_len;
		// handle PEEK read
		if (flags & MSG_PEEK) break;
		// adjust buff size to 2x
		if (cur == buff_size && !flags) {
			buff_size <<= 1;
			if (buff_size > BUFF_MAX_SIZE || buff_size < cur) {
				cur = -1ULL; // ulld max
				break;

			} else { 
				*data = static_cast<uint8_t *>( ::realloc(*data, buff_size) ); // 2^N
				::memset(*data + cur, 0, buff_size >> 1);
			}
		}
	}
	return cur;
}

/* block recv but don't flush buffer */
size_t Socket::recvPeekData(int _connfd , uint8_t **data)
{
	return this->recvData(_connfd , data , MSG_PEEK | MSG_DONTWAIT);
}

/* non-block recv */
size_t Socket::recvNonBlockData(int _connfd , uint8_t **data)
{
	return this->recvData(_connfd , data , MSG_DONTWAIT);
}

/* non-block recv with absolutely length */
size_t Socket::recvCertainData(
	int _connfd , 
	uint8_t **data , 
	size_t _len)
{
	//initial temporary length var
	size_t  cur 	  = 0;
	ssize_t buff_len  = 0;

	*data = static_cast<uint8_t *>( ::calloc(1 , _len + 1) );

	while ((buff_len = ::recv(_connfd , *data + cur , _len - cur , MSG_DONTWAIT | MSG_WAITALL))) 
	{
		if (buff_len == -1) {
			if (cur == _len) 	 { errno = 0; break; } // successfully recv
			if (errno == EAGAIN) { errno = 0; continue; } 
			else  				 { errno = 0; break; }
		}
		cur += buff_len;
	}
	return (cur == _len) ? cur : -1ULL;
}

/* block send with time out */ 
size_t Socket::sendData(int _connfd , uint8_t* buff , size_t _len) // stupid version
{
	//initial temporary length var
	ssize_t buff_len = 0;
	size_t  cur      = 0;
	
	//loop send
	while( ( buff_len = ::send(_connfd , buff + cur, _len - cur, MSG_NOSIGNAL ) ) ) {
		if(buff_len == -1) {errno = 0; break;}

		cur += buff_len;
	}
	return ( cur == _len ) ? cur : -1ULL;
}

static void handlePipe(int id) {
	::printf("# Broken pipe, Peer Socket Close\n");
}

/* specially send file base on unix/sendfile() */ 
size_t Socket::sendFile(int _connfd , const char* _fpath) 
{
	struct stat target {0};
	//check whether file exists and get it size
	if( ::stat(_fpath , &target) < 0 ) {errno = 0; return 0;}

	//initial temporary length var and open valid file
	int     fd  	 = ::open(_fpath , O_RDONLY);
	ssize_t cur 	 = 0;
	ssize_t buff_len = 0;

	::signal(SIGPIPE , handlePipe);

	//loop send
	while( ( buff_len = ::sendfile(_connfd , fd , (off_t *)&cur , target.st_size - cur) ) ) {
		if(buff_len == -1) {errno = 0; break;}

	}

	::close(fd);
	return ( cur == target.st_size ) ? cur : -1ULL;
}

/* same as sendFile() just with a header */
size_t Socket::sendFileWithHeader(int _connfd , const char* _fpath , uint8_t *header ,  size_t header_len)
{
	//int opt = 1; // use TCP_CORK , but segmentation fault ***
	// NETERROR(
	// 	setsockopt(sockfd, SOL_TCP, TCP_CORK, &opt, sizeof (opt)) < 0
	// , "cork error");

	if( this->sendData(_connfd , header , header_len) < 0 ||
		this->sendFile(_connfd , _fpath) < 0 ) 	
		return -1ULL;

	//opt = 0;
	// NETERROR(
	// 	setsockopt(sockfd, SOL_TCP, TCP_CORK, &opt, sizeof (opt)) < 0
	// , "cork error");
	return 1ULL;
}
 

Socket::~Socket()
{
	::close(sockfd);
}

EventPool::EventPool() : 
	epfd( ::epoll_create(0x1ADC) )
{
	NETERROR(epfd < 0, "create epoll error");
}

bool EventPool::mountEvent(const EventChannel&& echannel)
{
	struct epoll_event ev = {
		events : echannel.type ,
		data : {ptr : new EventChannel {echannel} }
	};

	NETERROR (
		::epoll_ctl(epfd, EPOLL_CTL_ADD, echannel.fd, &ev) < 0
	, "add epoll error");
	return true;
}

bool EventPool::modifyEvent(const EventChannel&& echannel)
{
	struct epoll_event ev = {
		events : echannel.type,
		data : {ptr : new EventChannel {echannel} }
	};

	NETERROR (
		::epoll_ctl(epfd, EPOLL_CTL_MOD, echannel.fd, &ev) < 0
	, "mod epoll error");
	return true;
}

bool EventPool::removeEvent(const EventChannel* eptr)
{

	if (::fcntl(eptr->fd, F_GETFD) != -1 || errno != EBADF) 
		{errno = 0; return false;}

	delete eptr;

	return true;	
}

int EventPool::createTimerFD(int64_t msec) {
	int timerfd = ::timerfd_create(CLOCK_REALTIME , TFD_NONBLOCK);
	NETERROR (timerfd < 0 , " create timer error");

	struct itimerspec new_value = {
		{ msec / 1000 , (msec % 1000) * 1000000 } , 
		{ msec / 1000 , (msec % 1000) * 1000000 }
	};

	NETERROR (
		::timerfd_settime(
			timerfd , 
			0 , 
			&new_value , 
			nullptr) < 0
	, "timer set error");

	return timerfd;
}

void EventPool::Poll(const EpollFunc& func)
{
	ssize_t epfd_n = ::epoll_wait(epfd, events, MAX_EVENTS, -1);

	if (epfd_n < 0) {
		::printf("\n"\
			"======================================	\n" \
			"#          Ctrl + C Received         #	\n"	\
			"#                                    # \n" \
			"# EventPool terminated, Good Bye~ :p # \n" \
			"====================================== \n"	
		);

		return;
	}

	for (ssize_t i = 0; i < epfd_n; i++) {
		auto echannel = static_cast<EventChannel*>(events[i].data.ptr);
		echannel->type = events[i].events;

		func( echannel );
	}
}

static bool stop = false;

static void terminalPoll(int id) {
	stop = true;
}

void EventPool::Loop(const EpollFunc func)
{
	::signal(SIGINT , terminalPoll);

	while(true) {
		this->Poll(func);

		if(stop) break;
	}

}