#include <BasicService/HttpProtocal/HttpAdapter.h>
#include <BasicService/HttpProtocal/HttpResponse.h>
#include <BasicService/Network/ServerBase.h>
#include <exception>

////receive raw byte stream
auto HttpAdapter::recvHttpData(Connection *conn)
	-> std::tuple<std::shared_ptr<uint8_t> , size_t > 
{
	uint8_t* data = nullptr;
	size_t len = sock->recvData(conn->getFD() , &data);
	return std::make_tuple( std::shared_ptr<uint8_t>(data) , len);
}
//// file type specilized check ; true for filesystem response
bool HttpAdapter::branchFilePathResp(Connection *conn , std::shared_ptr<HttpResponseBase> ret)
{
	auto file_resp = std::dynamic_pointer_cast<FileResponse> (ret);
	if( file_resp == nullptr ) 	return false;

	fs::path target = file_resp->getFilepath();
	if(target  == "" ) 			return false;
	
	auto [buff, slen] = file_resp->stringizeHeader();
	return sock->sendFileWithHeader(conn->getFD() , target.c_str() , buff.get() , slen);
}

void HttpAdapter::sendHttpData(Connection *conn , std::shared_ptr<HttpResponseBase> ret) 
{
	if (ret == nullptr) return ;
	// Successfully get response(even exception occurs)

	// specialized 
	if (branchFilePathResp(conn , ret)) 
						return ;
	// Send back to socket 
	auto [buff, slen] = ret->stringize();

	slen = sock->sendData(conn->getFD(), buff.get(), slen);
}
