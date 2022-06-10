#include<service/Event.h>
#include <service/TAPSystem.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

Course::Course(string id){
    this->id = id;
}

int Course::Query(){
    int count;
    string retVal;
    int res = __LSR__.Select("courses","*","id=" + id,count,retVal) != NO_ERROR;
    if(res != NO_ERROR){
        return res;
    }
    int length;
    string *str = Split(retVal,';',length);
    if(retVal == "" || count == 0 || str == nullptr){
        delete [] str;
        return PARAM_FORM_ERROR;
    }
    string *info = Split(str[1],',',length);
    delete [] str;
    if(length != 6){
        return PARAM_FORM_ERROR;
    }

    /* Fill Details */
    //Name
    this->name = info[1];

    //Time
    try{
        this->time = stoi(info[2]);
    }catch(exception &e){
        delete [] info;
        return TYPE_INT_MISMATCH;
    }
    //Get Professor Name
    __LSR__.Select("users","name","id="+info[3],count,retVal);
    this->professor = retVal.substr(retVal.find(';')+1);

    //Get Location Name
    __LSR__.Select("landmark","name","id="+info[4],count,retVal);
    this->location = retVal.substr(retVal.find(';')+1);

    //Room
    this->room = info[5];

    delete [] info;
    return NO_ERROR;
}

SimpleJson::Object Course::Format(){
    SimpleJson::Object J({
        {"id",id.c_str()},
        {"name",name.c_str()},
        {"time",time},
        {"prof",professor.c_str()},
        {"loc",location.c_str()},
        {"room",room.c_str()}
    });
    return J;
}


// Json Course::getTimeTable(){
//     int count;
//     string retVal;
//     NEDB TempDB(USER_DIR + "/" + schoolid + "/" + classid);
//     TempDB.Mount("timetable");
//     int errCode = TempDB.Select("timetable","*","",count,retVal);
//     int length;
//     Json J;
//     string * str = Split(retVal,';',length);
//     //fields ; a ; b ;
//     //fields: id,mon,tue,wed,thur,fri
//     if(retVal == "" || str == nullptr){
//         TempDB.Close();
//         return J;
//     }
//     int length_temp;
//     vector<SimpleJson::Object> courseInfo;
//     vector<SimpleJson::Object> timeCode;
//     for(int i = 1;i < length;i++){
//         string * info = Split(str[i],',',length_temp);
//         SimpleJson::Object obj = getCourseInfo(info[0]);
//         int daycode[6];
//         for(int j=1;j<=5;j++){
//             daycode[j]=stoi(info[j]);
//         }
//         vector<int> code(daycode+1,daycode+6);
//         timeCode.push_back(SimpleJson::Object({{"pos",i},{"timeCode",code}}));
//         //tempJson.push_back({"daycode",code});
//         courseInfo.push_back(obj);
//         delete[] info;
//     }
//     J.push_back({"basic",timeCode});
//     J.push_back({"detail",courseInfo});
//     delete[] str;
//     TempDB.Close();
//     return J;
//     /*
//     string * field = Split(str[0],',',templ);
//     for(int i = 1; i < length; i++){
//         int l;
//         string * classes = Split(str[i],',',l);
//         cout<<"Push Back Success 0"<<endl;
//         for(int j = 1; j < l; j++){
//             string classinfo = "";
//             if(classes[j]!="0"){
//                 classinfo = getCourseInfo(classes[j]).stringize();
//             }
//             CONSOLE_LOG(0,1,1,"Class Info '%s'\n",classinfo.c_str());
//             J.push_back({field[j].c_str(),classinfo.c_str()});
//             cout<<"Push Back Success 1"<<endl;
//         }
//         delete[] classes;
//     }
//     TempDB.Close();
//     delete[] str;
//     delete[] field;
//     return J;
//     */
// }

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
