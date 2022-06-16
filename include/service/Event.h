#ifndef __SCHEDULE_CONTROL_H__
#define __SCHEDULE_CONTROL_H__
#include<common.h>
#include<interfaces.h>
#define WEEK_NUM_TERM 20

#define SET_TIME(code,timenum) ({code |= (1 << (timenum-1));})
#define WEEK_CHECK(weeks,week) ({(weeks >> (week-1)) & 0x1;})

class Course{
private:
    std::string id;
    std::string name;
    std::string time;
    std::string intro;
public:
    Course(std::string id = "0");
    int Query(bool intro = true);
    SimpleJson::Object Format();
    int AddNew(std::string& detail,std::string& intro);
    Json getFile(std::string& prof);
    int Remove();
    int AddWork(std::string& prof,std::string& classid,std::string& detail);
    Json getWork(std::string& prof,std::string& classid);
    int AddExam(std::string& detail);
    Json getExam(std::string& school,bool all = false);
    string getName(){return name;}
    string getTime(){return time;}
    Json getAll();
};

class Event{
private:
    std::string id;
    std::string start;
    std::string end;
    std::string name;
    std::string location;
    std::string info;
public:
    Event(std::string id="0");
    int Parse(std::string detail);
    SimpleJson::Object Format();
};

#endif