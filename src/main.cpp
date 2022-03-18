#include<Basic.h>


int main(/*int argv , char* argc[]*/) {
    __START__();
}

/*
测试数据  @  
CREATE test{ID:int,Name:varchar32,School:varchar255};
INSERT test(1001,Name_01,School_01);
INSERT test(1002,Name_02,School_02);
INSERT test(1003,Name_03,School_03);
INSERT test(1004,Name_04,School_04);
INSERT test(1005,Name_05,School_05);
INSERT test(1006,Name_06,School_06);
INSERT test(1007,Name_07,School_07);
PRINT test;
DELETE test(1:0);
DELETE test(0);
REMOVE test;

*/