#include<service/Event.h>
#include <service/TAPSystem.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

Event::Event(string id){
    this->id = id;
}

int Event::Parse(string detail){
    int length;
    string *info = Split(detail,',',length);
    if(detail == "" || length != 6){
        delete [] info;
        return PARAM_FORM_ERROR;
    }
    /* Fill Details */
    //ID
    this->id = info[0];

    //Start
    this->start = info[1];

    //End
    this->end = info[2];

    //Get Location Name
    int count;
    string retVal;
    __DATABASE.Select("landmark","name","id="+info[3],count,retVal);
    this->location = retVal.substr(retVal.find(';')+1);

    //Name
    this->name = info[4];
    
    //Describe
    this->info = info[5];

    delete [] info;
    return NO_ERROR;
}

SimpleJson::Object Event::Format(){
    SimpleJson::Object J({
        {"id",id.c_str()},
        {"name",name.c_str()},
        {"start",start.c_str()},
        {"end",end.c_str()},
        {"loc",location.c_str()},
        {"info",info.c_str()}
    });
    return J;
}

