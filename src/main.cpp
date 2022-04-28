#include<main.h>
#include<Interface/NEDB.h>
#include<pthread.h>
using namespace std;
using namespace NEDBnamespace;

 
int main(){
    __START__();    //Console Run
    return 0;
}

/*
string sql[16] = {"","","","","","","","""","","","","","","",""};
NEDB DB("/home/jianxf/.nesrc/");
void* SQLthread(void* arg);
static int num = 0;

// Thread And Lock Test
int main(){
    NEDB_DEBUG(2);
    NEDB_SETTING(64, 400, 50, 5);
    pthread_t thread[64];
    void* threadRet;
    DB.Openall();
    
    for(int i = 0; i < 16; i++){
        for(int j = 1; j <= 50; j++){
            sql[i] += "insert into test2 values (" + to_string(i * 50 + j) + ");";
        }
        sql[i] += "delete from test2 where id>40 and id<160;select * from test2;";
    }
    // Thread Test 
    for(int i = 0; i < 5; i++){
        pthread_create(&thread[i], NULL, SQLthread, &i);
    }
    for(int i = 0; i < 5; i++){
        pthread_join(thread[i], &threadRet);
    }
    cout<<"Thread OK "<<endl;
    DB.Exec("select * from test2;");
    cout << DB.ErrCode() << endl;
    //cout << DB.ReturnVal() << endl;
}

void* SQLthread(void* arg){
    int thread_num = num++;
    cout << "Query " << thread_num << " Begin" << endl;
    DB.Exec("drop table test2;");
    cout << "Pretest: " << DB.Exec("create table test2 (id int);") << endl;
    int t = DB.Exec(sql[thread_num].c_str());
    cout << "Query " << thread_num << " OK with Value " << t << endl;
    pthread_exit(NULL);
}

*/