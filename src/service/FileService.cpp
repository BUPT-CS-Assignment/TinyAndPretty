#include <common.h>
#include <interfaces.h>
#include <service/TAPSystem.h>

#include <sys/signal.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

void zipFile(std::string path)
{
    ::signal(SIGCHLD, SIG_IGN);
    ::system(std::string{"gzip " + path}.c_str());
    errno = 0;
}

def_HttpEntry(FileUpload, req)
{
    auto filename = req.queryForm("payload").queryFilename();
    std::string path{SRC_DIR + "/file/" + std::string{filename}};

    CONSOLE_LOG(true,"api/upload called [filename:%s  path:%s]\n",filename.data(),path.c_str());

    std::ofstream file{path , ios::binary | ios::out};

    file << req.queryForm("payload");
    file.close();

    zipFile(path);
    return new HttpResponse{"Upload and Zip Success"};
}

def_HttpEntry(API_File, req)
{
    std::string userid(req.queryHeader("userid"));
    std::string token(req.queryHeader("token"));


    // if(TokenCheck(userid, token) != TOKEN_ACCESS)
    // {
    //     return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    // }

    string basic_name(req.queryForm("file").queryFilename());
    string filepath(req.queryHeader("upload_path"));
    string filename(req.queryHeader("upload_name"));
    string type(req.queryHeader("upload_type"));

    //Rename
    basic_name = basic_name.substr(basic_name.find_last_of("."));
    filename = filename + basic_name;

    std::string path(SRC_DIR + "/course/" + filepath);
    path += (type == "1" ?"res/":(userid+"/"));

    NEDB _DB(path);
    _DB.DirInit();

    std::ofstream file{path + filename , ios::binary | ios::out};

    CONSOLE_LOG(true,"api/file called [user:%s  file:%s]\n",userid.c_str(),(path+filename).c_str());

    file << req.queryForm("file");
    file.close();

    zipFile(path);
    return new HttpResponse{
        "Upload and Zip Success","NO_ERROR",HTTP_STATUS_200
    };

}