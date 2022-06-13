#include <common.h>
#include <interfaces.h>
#include <service/TAPSystem.h>

#include <sys/signal.h>
using namespace NEDBSTD;

void zipFile(std::string path) {
	::signal(SIGCHLD , SIG_IGN);
	::system( std::string{"gzip " + path}.c_str() );
	errno = 0;
}

def_HttpEntry(FileUpload , req) {
	auto filename = req.queryForm("payload").queryFilename();


	std::string path{SRC_DIR + "/file/" + std::string{filename}};
	std::cerr << filename << "\n" << path <<"\n";

	std::ofstream file {path , ios::binary | ios::out};

	file << req.queryForm("payload");
	file.close();

	zipFile(path);
	return new HttpResponse {"Upload and Zip Success"};
}