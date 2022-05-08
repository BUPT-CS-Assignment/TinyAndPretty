#ifndef __SCHEDULE_CONTROL_H__
#define __SCHEDULE_CONTROL_H__
#include<common.h>
#include<interfaces.h>
#define WEEK_NUM_TERM 20

#define SET_TIME(code,timenum) ({code |= (1 << (timenum-1));})
#define WEEK_CHECK(weeks,week) ({(weeks >> (week-1)) & 0x1;})

std::string TimeTableFetch(std::string schoolid,std::string classid,std::string userid);
std::string PersonalTimeTableFetch(std::string classid, std::string userid);

class Course{
private:
    std::string schoolid;
    std::string classid;
    std::string userid;
public:
    Course(std::string id);
    void setSchool(std::string id);
    void setClass(std::string id);
    std::string getSchool();
    std::string getClass();
    std::string getUser();
    SimpleJson::Object getCourseInfo(std::string courseid);
    Json getTimeTable();
    Json getPersonalTimeTable();
};

#endif