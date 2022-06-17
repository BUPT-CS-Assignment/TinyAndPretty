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
    ::system(std::string{"gzip \"" + path + "\""}.c_str());
    errno = 0;
}

def_HttpEntry(FileUpload, req)
{
    auto filename = req.queryForm("payload").queryFilename();
    std::string path{SRC_DIR + "/file/" + std::string{filename}};

    CONSOLE_LOG(true, "api/upload called [filename:%s  path:%s]\n", filename.data(), path.c_str());

    std::ofstream file{path , ios::binary | ios::out};

    std::cerr << std::string{req.queryForm("fil")} << endl;
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
    path += (type == "1" ? "res/" : (userid + "/"));

    NEDB _DB(path);
    _DB.DirInit();
    if(_DB.Mount("FILE") == FILE_NOT_FOUND)
    {
        _DB.Create("FILE", "id int,name text,md5 longtext");
    }
    _DB.Close();

    //std::fstream file{path + filename , ios::binary | ios::out};

    CONSOLE_LOG(true, "api/file called [user:%s  file:%s]\n", userid.c_str(), (path + filename).c_str());

    string md_5 = md5(req.queryForm("file"));
    bool flag = true;
    NEDB DB(path);
    DB.Mount("FILE");
    int count, len, length; string ret;
    DB.Select("FILE", "md5", "", count, ret);
    if(count != 0)
    {
        string* md5s = Split(ret, ';', len);
        for(int i = 1; i < len + 1; i++)
        {
            if(md5s[i] == md_5)
            {
                flag = false;
                break;
            }
        }
    }
    
    if(!flag)
    {
        DB.Close();

        return new HttpResponse{
            "","FILE EXISTED",HTTP_STATUS_200
        };
    }

    std::ofstream ofile{path + filename , ios::binary | ios::out};
    DB.Insert("FILE", "", to_string(count + 1) + "," + filename + "," + md_5);
    ofile << req.queryForm("file");
    ofile.close();
    DB.Close();

    zipFile(path);
    return new HttpResponse{
        "Upload and Zip Success","NO_ERROR",HTTP_STATUS_200
    };

}