#ifndef __SCHEDULE_CONTROL_H__
#define __SCHEDULE_CONTROL_H__
#include<common.h>
#include<interfaces.h>

class Course{
private:
    int id_;
    std::string name_;
    int prof_id_;

public:
    Course();
    Course(int,std::string,int);
    Course(int);
    std::string GetProf();
    std::string GetName();
    int AddCourse();
    int DeleteCourse();

};


#endif