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

def_HttpEntry(API_Work , req) {
	std::string filename(req.queryForm("file").queryFilename());
    std::string filepath(req.queryForm("filepath"));
    //string filename(req.queryForm("filename"));
    std::string userid(req.queryHeader("userid"));
    std::string path = SRC_DIR + "/course/" + filepath + userid + "/";
    NEDB _DB(path);
    _DB.DirInit();
    //filename = md5(filename);
    UTILSTD::CONSOLE_LOG(0,1,1,"FULL PATH: %s\n",path.c_str()); 

	std::ofstream file {path+filename , ios::binary | ios::out};

	file << req.queryForm("file");
	file.close();

	zipFile(path);
    HttpResponse* HResp = new HttpResponse{"Upload and Zip Success"};
    HResp->appendHeader("msg","NO_ERROR");
    return HResp;

}