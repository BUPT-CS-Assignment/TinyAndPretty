#include <service/Event.h>
#include <service/TAPSystem.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

Course::Course(string id){
    this->id = id;
}

int Course::Query(){
    int count,len;
    string retVal;
    int res = __DATABASE.Select("courses","*","id=" + id,count,retVal) != NO_ERROR;
    if(res != NO_ERROR) return res;
    string *str = Split(retVal,';',len);
    if(retVal == "" || count == 0 || str == nullptr){
        delete [] str;
        return PARAM_FORM_ERROR;
    }
    string *info = Split(str[1],',',len);
    delete [] str;
    if(len != 3)    return PARAM_FORM_ERROR;

    /* Fill Details */
    this->name = info[1];
    this->time = info[2];
    delete [] info;
    ifstream ifs((SRC_DIR + "/course/" + this->id + "/intro.txt").c_str());
    if(!ifs.is_open()){
        intro = "null";
    }else{
        ifs >> noskipws;
        unsigned char c; this->intro = "";
        while(!ifs.eof()){
            ifs >> c;
            this->intro += c;
        }
    }

    return NO_ERROR;
}

SimpleJson::Object Course::Format(){
    SimpleJson::Object J({
        {"id",id.c_str()},
        {"name",name.c_str()},
        {"time",stoi(time)},
        {"intro",intro.c_str()},
    });
    return J;
}

int Course::AddNew(string& detail,string& intro){
    //System DataBase
    int errCode = __DATABASE.Insert("courses","",detail);
    if(errCode != NO_ERROR) return errCode;

    string dir = SRC_DIR + "/course/" + detail.substr(0,detail.find_first_of(",")) + "/";
    
    //Homework
    NEDB _DB(dir);
    if(_DB.DirInit() != NO_ERROR) return DIR_ERROR;
    errCode = _DB.Create("exam","id int,school int,name text,start int,len int,location int,room int");
    if(errCode != NO_ERROR) return errCode;
    _DB.SetDir(dir + "/professor");
    _DB.DirInit();
    _DB.Close();
    
    //Introduction
    ofstream ofs(dir + "/intro.txt",ios::trunc|ios::out);
    ofs << intro;
    ofs.close();

    return errCode;
}

int Course::Remove(){
    int count;
    int errCode = __DATABASE.Delete("course","id="+this->id,count);
    if(errCode != NO_ERROR) return errCode;
    string dir = SRC_DIR + "/course/" + this->id;
    if(rmdir(dir.c_str()) != 0) return DIR_ERROR;
    return NO_ERROR;
}

int Course::AddWork(string& detail){
    string dir = SRC_DIR + "/course/" + id;
    NEDB _DB(dir);
    _DB.Mount("homework");
    int errCode = _DB.Insert("homework","",detail);
    _DB.Close();
    return errCode;
}

int Course::AddExam(string& detail){
    string dir = SRC_DIR + "/course/" + id;
    NEDB _DB(dir);
    _DB.Mount("exam");
    int errCode = _DB.Insert("exam","",detail);
    _DB.Close();
    return errCode;
}


/*

{"detail":
    [
        {
            "id":xx,...
        },
        {
            "id":xx,...
        }
    ]

}


*/
