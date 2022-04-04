#include <Network/ServerBase.h>

Socket::Socket(uint16_t _port) : port(_port)
{
	/*----------------------------------------*/
	// Initialize socket port
	sockfd = socket(PROTO_FAMILT, SOCK_TYPE, 0);
	NETERROR(sockfd < 0, "init socket error ");

	/*----------------------------------------*/
	// Set up host address
#ifdef IPV_4
	address = sockaddr_in{
		sin_family : PROTO_FAMILT,
		sin_port : htons(port),
		sin_addr : {s_addr : htonl(HOST_ADDR)},
	};
#elif IPV_6 // near future
	address = sockaddr_in{
		sin_family : PROTO_FAMILT,
		sin_port : htons(port),
		sin_addr : {s_addr : htonl(HOST_ADDR)},
	};
#endif
	/*----------------------------------------*/
	// Permit to reuse address
	addr_reuse = ADDR_REUSE;
	NETERROR(
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &addr_reuse, sizeof(int)) < 0, "set address reuse");
	/*----------------------------------------*/
	// Set up timeout limit for receive and send
	 recv_timeout = (timeval)RECV_TIMEOUT;
	 NETERROR(
	     setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&recv_timeout,sizeof(timeval)) < 0
	 , "set timeVal error");

	send_timeout = (timeval)SEND_TIMEOUT;
	NETERROR(
	    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&send_timeout,sizeof(timeval)) < 0
	, "set timeVal error");

	/*----------------------------------------*/
	// Bind port to socket file
	NETERROR(
		bind(sockfd, (sockaddr *)(&address), sizeof(address)) < 0, "bind error");

	/*----------------------------------------*/
	// Set up max capacity of listening queue
	NETERROR(
		listen(sockfd, LISTEN_Q_MAX) < 0, "listen error");
}

Connection *Socket::onConnect()
{
	sockaddr_in _addr = {0};
	socklen_t _len = sizeof(_addr);
	int _connfd = accept(sockfd, (sockaddr *)&_addr, &_len);
	return new Connection{_connfd, _len, _addr};
}

size_t Socket::recvData(int _connfd, uint8_t **data)
{
	size_t buff_size = BUFF_INIT_SIZE;
	size_t len = 0;
	int buff_len = 0;
	*data = (uint8_t *)calloc(1 , buff_size);

	while ( (buff_len = recv(_connfd, *data + len, buff_size - len, 0)) )
	{
		//std::cerr << "Buff Info : " << buff_size << " " << len  << " " << buff_len << std::endl;
		//std::cerr << "errno : " << errno << " ## " << std::endl;
		if (buff_len == -1)
		{
			if 		(errno == EAGAIN)	{errno = 0 ;break;}
			else if (errno == EINTR)	{errno = 0 ;continue;}
			else						{errno = 0 ;break;}
		}
		
		len += buff_len;
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

void Socket::sendData(int _connfd , uint8_t* buff , size_t _len) // stupid version
{
	send(_connfd , buff , _len , 0);
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
		epoll_ctl(epfd, EPOLL_CTL_ADD, _fd, &ev) < 0,
		"add epoll error");
	return true;
}

bool EventPool::mountPtr(void *_ptr, int _fd, uint32_t _type)
{
	epoll_event ev = {
		events : _type,
		data : {ptr : _ptr}
	};

	NETERROR(
		epoll_ctl(epfd, EPOLL_CTL_ADD, _fd, &ev) < 0,
		"add epoll error");
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