#include <common.h>
#include <interfaces.h>
#include <NEdb.h>
#include<iostream>

int main(int argv , char* argc[]) {
    NEdb ne("");
    //dir test
    printf("%s\n",ne.getDir());
    ne.setDir("/home/jianxf/.nesrc/");
    ne.setDefaultPageSize(100);
    //state msg return test 
    printf("%s\n",ne.getMsg());
    //create table test
    ne.exec("create table test (id int);");
    printf("%s\n",ne.getMsg());
    //insert test
    std::string sql = "";
    for(int i =0; i < 50; i++){
        sql = sql + "insert into test values ("+std::to_string(i)+");";
    }
    //execute test
    ne.exec(sql.c_str());
    printf("%s\n",ne.getMsg());
    //describe and select test
    ne.exec("describe table test;select * from test where id>10 and id<36;");
    printf("%s\n",ne.getMsg());
    //datareturn test
    printf("%s\n",ne.getData());
    //delete test
    ne.exec("delete from test where id < 44;select * from test;drop table test;");
    printf("%s\n",ne.getMsg());
    printf("%s\n",ne.getData());
    //closet test
    ne.close();
    ///////////////////////////////////////////////////////////
    HttpServer server(9006 , 5);
    server.start();
    return 0;
}