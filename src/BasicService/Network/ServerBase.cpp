#include <BasicService/Network/ServerBase.h>
#include <sys/sendfile.h>
#include <netinet/tcp.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

Socket::Socket()
{
	// Initialize socket port
	sockfd = socket(PROTO_FAMILT, SOCK_TYPE, 0);
	NETERROR(sockfd < 0, "init socket error ");

// Set up host address
#ifdef IPV_4
	address = sockaddr_in{
		sin_family : PROTO_FAMILT,
		sin_port : htons(PORT),
		sin_addr : {s_addr : htonl(HOST_ADDR)},
	};
#elif IPV_6 // near future
#endif

// Permit to reuse address
#ifdef ADDR_REUSE
	int addr_reuse = ADDR_REUSE;
	NETERROR(
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &addr_reuse, sizeof(int)) < 0
	, "set address reuse");
#endif

	// Set up timeout limit for receive and send
	struct timeval recv_timeout = (timeval)RECV_TIMEOUT;
	 NETERROR(
	     setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&recv_timeout,sizeof(timeval)) < 0
	 , "set timeVal error");

	struct timeval send_timeout = (timeval)SEND_TIMEOUT;
	NETERROR(
	    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&send_timeout,sizeof(timeval)) < 0
	, "set timeVal error");

	// Bind port to socket file
	NETERROR(
		bind(sockfd, (sockaddr *)(&address), sizeof(address)) < 0
	, "bind error");

	// Set up max capacity of listening queue
	NETERROR(
		listen(sockfd, LISTEN_Q_MAX) < 0
	, "listen error");

	::printf("Successfully Listen on %d\n" , PORT);
}

////get one connection
Connection *Socket::onConnect()
{
	struct sockaddr_in _addr {0};
	socklen_t _len = sizeof(_addr);
	int _connfd = accept(sockfd, (sockaddr *)&_addr, &_len);

	if(_connfd == -1) 
		{errno = 0; return nullptr;}
	else 
		return new Connection{_connfd, _len, _addr};
}
////block recv with time out 
size_t Socket::recvData(int _connfd, uint8_t **data)
{
	//initial temporary length var
	size_t  len 	  = 0;
	size_t  buff_size = BUFF_INIT_SIZE;
	ssize_t buff_len  = 0;
	*data = (uint8_t *)calloc(1 , buff_size);
	//loop recv and dynamically adjust buff size
	while ( (buff_len = recv(_connfd, *data + len, buff_size - len, 0)) )
	{

	// IFDEBUG(
	// 	std::cerr << "Recv Buff Info : FROM - " << _connfd << " " << len  << " " << buff_len << "\n"
	// 			  << "\terrno : " << errno << " ## " << std::endl;
	// )
		//handle errno occurs
		if (buff_len == -1)
		{
			if 		(errno == EAGAIN)	{errno = 0; break;}
			else if (errno == EINTR)	{errno = 0; continue;}
			else						{errno = 0; break;}
		}
		
		len += buff_len;
		//adjust buff size to 2x
		if (len == buff_size)
		{
			buff_size <<= 1;
			if (buff_size > BUFF_MAX_SIZE || buff_size < len)
			{
				len = -1ULL;
				break;
			} // ulld max
			else
			{
				*data = (uint8_t *)realloc(*data, buff_size); // 2^N
				memset(*data + len, 0, buff_size >> 1);
			}
		}
	}
	return len;
}
////block send with time out 
size_t Socket::sendData(int _connfd , uint8_t* buff , size_t _len) // stupid version
{
	//initial temporary length var
	ssize_t buff_len = 0;
	size_t  cur      = 0;
	
	//loop send
	while( ( buff_len = send(_connfd , buff + cur, _len - cur, MSG_NOSIGNAL ) ) ) {
		if(buff_len == -1) {errno = 0; break;}

		cur += buff_len;
		// IFDEBUG(
		// 	std::cerr << "Send Buff Info : " << _len << " " 
		// 			  << cur  << " " << buff_len <<"\n"
		// 			  << "\terrno : " << errno << " ## " << std::endl
		// );
	}
	return ( cur == _len ) ? cur : -1ULL;
}

static void handlePipe(int id) {
	::printf("# Peer Socket Close\n");
}

////specially send file base on unix/sendfile() 
size_t Socket::sendFile(int _connfd , const char* _fpath) 
{
	struct stat target {0};
	//check whether file exists and get it size
	if( stat(_fpath , &target) < 0 ) {errno = 0; return 0;}

	//initial temporary length var and open valid file
	int     fd  	 = open(_fpath , O_RDONLY);
	ssize_t cur 	 = 0;
	ssize_t buff_len = 0;

	signal(SIGPIPE , handlePipe);

	//loop send
	while( ( buff_len = sendfile(_connfd , fd , (off_t *)&cur , target.st_size - cur) ) ) {
		if(buff_len == -1) {errno = 0; break;}

		IFDEBUG (
			std::cerr << "Send FILE Info : " << target.st_size << " " 
					  << cur  << " " << buff_len <<"\n"
					  << "\terrno : " << errno << " ## " << std::endl
		);
	}
	close(fd);
	return ( cur == target.st_size ) ? cur : -1ULL;
}

////same as sendFile() just with a header
size_t Socket::sendFileWithHeader(int _connfd , const char* _fpath , uint8_t *header ,  size_t header_len)
{
	//int opt = 1; // use TCP_CORK , but segmentation fault ***
	// NETERROR(
	// 	setsockopt(sockfd, SOL_TCP, TCP_CORK, &opt, sizeof (opt)) < 0
	// , "cork error");

	if( sendData(_connfd , header , header_len) < 0 ) 
											return -1ULL;
	if( sendFile(_connfd , _fpath) < 0 ) 	return -1ULL;

	//opt = 0;
	// NETERROR(
	// 	setsockopt(sockfd, SOL_TCP, TCP_CORK, &opt, sizeof (opt)) < 0
	// , "cork error");
	return 1ULL;
}
 

Socket::~Socket()
{
	close(sockfd);
}

EventPool::EventPool()
{
	epfd = epoll_create(0x1ADC);
	NETERROR(epfd < 0, "create epoll error");
}

bool EventPool::mountFD(int _fd, uint32_t _type)
{
	epoll_event ev = {
		events : _type,
		data : {fd : _fd}
	};

	NETERROR(
		epoll_ctl(epfd, EPOLL_CTL_ADD, _fd, &ev) < 0 
	, "add epoll error");
	return true;
}

bool EventPool::mountPtr(void *_ptr, int _fd, uint32_t _type)
{
	epoll_event ev = {
		events : _type,
		data : {ptr : _ptr}
	};

	NETERROR(
		epoll_ctl(epfd, EPOLL_CTL_ADD, _fd, &ev) < 0
	, "add epoll error");
	return true;
}

void EventPool::Poll(std::function<void(epoll_data_t, int)> &func)
{
	size_t epfd_n = epoll_wait(epfd, events, MAX_EVENTS, -1);
	NETERROR(epfd_n < 0, "poll error");

	for (size_t i = 0; i < epfd_n; i++)
	{
		func(events[i].data, events[i].events);
	}
}

void EventPool::Loop(std::function<void(epoll_data_t, int)> func)
{
	while (true)
	{
		Poll(func);
	}
}