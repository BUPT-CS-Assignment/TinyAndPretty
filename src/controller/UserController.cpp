#include <service/User.h>
#include <service/TAPSystem.h>
#include <service/Event.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

User::User(string id)
{
    this->id = id;
    auth = USER_COMMON;
}

int User::Signin(string& passwd)
{
    int count;
    string retVal;
    int errCode = __DATABASE.Select("token", "passwd", "id=" + id, count, retVal);
    if(errCode != 0)    
{
        return errCode;
    }
    retVal = retVal.substr(retVal.find(";") + 1);
    if(passwd != retVal)
    {
        return -1;
    }
    Query();
    if(auth < USER_SCHOOL && (schoolid == "0" || classid == "0"))
    {
        return -2;
    }
    return NO_ERROR;
}

int User::Signup(string& passwd)
{
    int errCode1 = __DATABASE.Insert("token", "", id + "," + passwd + ",0");
    if(errCode1 != NO_ERROR)
    {
        return errCode1;
    }
    return __DATABASE.Insert("users", "id,auth", id + "," + to_string(auth));
}

int User::ChangePwd(std::string& pwd)
{
    int count;
    return __DATABASE.Update("token", "passwd = " + pwd, "id = " + id, count);

}

int User::Update(string& value)
{
    int count;
    return __DATABASE.Update("users", value, "id=" + id, count);
}

int User::Query(bool d_name)
{
    int count, length;
    string retVal;
    int errCode = __DATABASE.Select("users", "*", "id=" + id, count, retVal);
    if(errCode != NO_ERROR)
    {
        return errCode;
    }
    retVal = retVal.substr(retVal.find_first_of(';') + 1);
    string* str = Split(retVal, ',', length);
    if(length != 7)
    {
        delete[] str;
        return PARAM_FORM_ERROR;
    }
    auth = stoi(str[1]);
    name = str[2];
    gender = (str[3] == "0" ? "女" : "男");
    schoolid = str[4];
    majorid = str[5];
    classid = str[6];
    delete[] str;
    if(d_name)
    {
        //School Name
        __DATABASE.Select("schools", "name", "id=" + schoolid, count, retVal);
        schoolName = retVal.substr(retVal.find(";") + 1);

        //Major Name
        __DATABASE.Select("majors", "name", "id=" + majorid, count, retVal);
        majorName = retVal.substr(retVal.find(";") + 1);
    }
    return NO_ERROR;
}

Json User::Format()
{
    Json J;
    J.push_back({"id",stoi(id)});
    J.push_back({"auth",auth});
    J.push_back({"name",name});
    J.push_back({"email","test@noui.cloud"});
    J.push_back({"gender",gender});
    J.push_back({"schoolid",schoolid});
    J.push_back({"school",schoolName});
    J.push_back({"majorid",majorid});
    J.push_back({"major",majorName});
    J.push_back({"classid",classid});
    return J;
}

Json User::getTimeTable()
{
    Class c(classid);
    if(auth == USER_SCHOOL)
    {
        return c.getTimeTable(schoolid, id);
    }
    else
    {
        return c.getTimeTable(schoolid);
    }

}

Json User::getEvents()
{
    int count, temp;
    string ret, RET;
    NEDB _DB(USER_DIR + "/" + schoolid + "/" + classid);
    _DB.Mount("event");
    string condition = "id > " + id + "000 and id < " + id + "999";
    
    _DB.Select("event", "*", condition, count, ret);

    _DB.Select("event", "*", "id < 10000", temp, RET);

    //Manage Info
    int length, len;
    Json J;
    if(count == 0 && temp == 0) return J;
    string* str = Split(ret, ';', length), * STR = Split(RET, ';', len);
    vector<SimpleJson::Object> events;
    for(int i = 1; count > 0 && i < length; i++)
    {
        Event event;    event.Parse(str[i]);
        events.push_back(event.Format());
    }
    for(int j = 1; temp > 0 && j < len; j++)
    {
        Event event;    event.Parse(STR[j]);
        events.push_back(event.Format());
    }
    J.push_back({"events",events});

    delete[] str; delete[] STR;
    return J;
}

int User::addEvent(std::string& value)
{
    if(value[value.find_first_of(',')] == '0')
    {
        if(auth < USER_CLASS) return SYSTEM_ERROR;
    }
    string dir = USER_DIR + "/" + schoolid + "/" + classid;
    NEDB _DB(dir);

    if(_DB.Mount("event") == FILE_NOT_FOUND)
    {
        _DB.DirInit();
        _DB.SetDefaultPageSize(2000);
        _DB.Create("event", "id int64,start int64,end int64,notice int,loc int,name text,info longtext");
    }
    _DB.Close();


    NEDB DB(dir);
    DB.Mount("event");
    int errCode = DB.Insert("event", "", value);

    DB.Close();
    return errCode;
}

int User::delEvent(std::string& id)
{
    NEDB _DB(USER_DIR + "/" + schoolid + "/" + classid);
    _DB.Mount("event");
    int count;
    int res = _DB.Delete("event", "id=" + id, count);
    _DB.Close();
    return res;
}

int User::AddNew(std::string& detail)
{
    int errCode;
    errCode = __DATABASE.Insert("users", "", detail);
    if(errCode != NO_ERROR)
    {
        return errCode;
    }
    int idx = detail.find_first_of(',');
    string id = detail.substr(0, idx);
    string auth = detail.substr(idx + 1, 1);
    errCode = __DATABASE.Insert("token", "", id + "," + "123" + "," + auth);
    return errCode;
}

int User::addNotice(string& eventid, string& code){
    string dir = USER_DIR + "/" + schoolid + "/" + classid;
    NEDB _DB(dir);

    _DB.Mount("event");

    int count;
    int errCode = _DB.Update("event","notice="+code,"id="+eventid,count);

    _DB.Close();
    return errCode;

}
