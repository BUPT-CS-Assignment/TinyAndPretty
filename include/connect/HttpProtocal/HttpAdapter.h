#ifndef __HTTP_ADAPTER__
#define __HTTP_ADAPTER__

#include <memory>

class Socket;
class Connection;
class HttpResponseBase;

/* Adapter : make a bridge between socket and http io */
class HttpAdapter
{
private:
	std::shared_ptr<Socket> sock;

	// dynamically convert base ptr to FileResponse
	bool branchFilePathResp(Connection *conn , std::shared_ptr<HttpResponseBase> ret);
public:
	HttpAdapter(std::shared_ptr<Socket> _sock) : sock(_sock) {};
	// recv data from socket in a modern c++ way
    auto recvHttpData(Connection *conn)
        -> std::tuple< std::shared_ptr<uint8_t> , size_t > ;

	// send data to socket also checking file 
	void sendHttpData(Connection *conn , std::shared_ptr<HttpResponseBase> ret) ;
};

#endif