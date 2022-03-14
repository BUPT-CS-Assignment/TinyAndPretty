#include<iostream>
#include "Praser.hpp"
#include "Table.cpp"

Table* __DataBase[100];
int __Cursor = 0;

class Actuator{
private:

public:
    void execute(InputStream inputStream){
        __OPERATE__ operation = inputStream.getOperation();
        string statement = inputStream.getStatement();
        if(operation == __INSERT){
            insert_row(statement);
        }else if(operation == __TABLE_CREATE){
            create_table(statement);
        }else if(operation == __PRINT){
            print_table(statement);
        }
    }
    /*
        CREATE table_name{
        ID:int,
        Name:varchar32,
        School:varchar255
        };
    */
    void create_table(string statement){
        int index_front = statement.find('{');
        int index_tail = statement.find('}');
        string name = statement.substr(0,index_front);
        statement = statement.substr(index_front+1,index_tail-index_front-1);
        Table* table = new Table(__Cursor+1,name);
        __DataBase[__Cursor++] = table;
        if(table->Init(statement)){
            cout<<"Create Table '"<<name<<"' Success."<<endl;
        }else{
            cout<<"Create Table '"<<name<<"' Failed."<<endl;
            __Cursor--;
        }
        
    }
    /*
        INSERT table_name(
            2020212000, :@ID
            Student, :@Name
            SCS :@School
        );
        //CREATE test{ID:int,Name:varchar32,School:varchar255};
        //INSERT test(100,n_01,s_001);
    */
    void insert_row(string statement){
        int index_front = statement.find('(');
        int index_tail = statement.find(')');
        string name = statement.substr(0,index_front);
        statement = statement.substr(index_front+1,index_tail-index_front-1);
        for(int i = 0;i<__Cursor;i++){
            if(__DataBase[i]->getName().compare(name) == 0){
                __DataBase[i]->Insert(statement);
                break;
            }
        }
        cout<<"Insert To '"<<name<<"' Success."<<endl;
    }
    /*
        PRINT table_name;
    */
    void print_table(string statement){
        for(int i = 0;i<__Cursor;i++){
            if(__DataBase[i]->getName().compare(statement) == 0){
                __DataBase[i]->Print();
                break;
            }
        }
    }




};