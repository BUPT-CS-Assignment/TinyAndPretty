#include <connect/HttpProtocal/HttpProtocal.h>
#include <connect/HttpProtocal/HttpException.h>
#include <connect/HttpProtocal/HttpManager.h>

/* handle HTTP Exception */
HttpResponse* HttpManager::dispatchException(const HttpException &e)
{
	switch (e) // near future
	{
	case HttpException::ERROR_LEN:
		std::cerr << "ERROR_LEN\n";
		return new HttpResponse{"Error occur : ERROR_LEN"};

	case HttpException::OUT_OF_LIMIT:
		std::cerr << "OUT_OF_LIMIT\n";
		return new HttpResponse{"Error occur : FILE_OUT_OF_LIMIT"};

	case HttpException::NON_PATH:
		std::cerr << "NON_PATH\n";
		return new HttpResponse{"Error occur : NON_PATH"};

	case HttpException::NON_CONN:
		return nullptr;

	default:
		std::cerr << "ELSE" << (int)e << "\n";
		return nullptr;
		
	}
}