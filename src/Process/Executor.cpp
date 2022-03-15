#include<Data.h>
#include<Process.h>
#include<Implement.h>
//#include<DataBase.h>

//__OPERATE__{__UNKNOWN,__SAVE,__TABLE_CREATE,__INSERT,__DELETE,__REPLACE,__PRINT};

Executor::Executor(DataBase* database){
    this->__DataBase = database;
    __Index_front = -1;
    __Index_tail = -1;
    __Statement = "NULL";
    __Name = "NULL";
}

void Executor::Execute(__OPERATE__ operate,string statement){
    __Statement = statement;
    switch (operate){
        case __TABLE_CREATE :
            execute_create_table();
            break;
        case __INSERT :
            execute_insert_row();
            break;
        case __PRINT :
            execute_print_table();
            break;
        case __DELETE :
            execute_delete_row();
            break;
        case __TABLE_REMOVE :
            execute_remove_table();
            break;
        default:
            break;
    }
}

bool Executor::Analyse(string left){
    if(left.compare("{") == 0){
        __Index_front = __Statement.find('{');
        __Index_tail = __Statement.find('}');
    }else if(left.compare("(") == 0){
        __Index_front = __Statement.find('(');
        __Index_tail = __Statement.find(')');
    }
    if(__Index_front < 0 || __Index_tail < 0){
        cout<<"Unknown Statement."<<endl;
        return false;
    }
    __Name = __Statement.substr(0,__Index_front);
    __Statement = __Statement.substr(__Index_front+1, __Index_tail - __Index_front - 1);
    return true;
}

void Executor::execute_create_table(){
    if(!Analyse("{")) return;
    Table* table = new Table(__DataBase->getCursor()+1,__Name);
    if(table->Init(__Statement)){
        __DataBase->AddTable(table);
        cout<<"Create Table '"<<__Name<<"' Success."<<endl;
    }else{
        cout<<"Create Table '"<<__Name<<"' Failed."<<endl;
    }
}

void Executor::execute_insert_row(){
    if(!Analyse("(")) return;
    int index = __DataBase->LocateByName(__Name);
    if(index!=-1){
        if( ! __DataBase->getDataBase()[index]->Insert(__Statement)){
            cout<<"Insert To '"<<__Name<<"' Failed."<<endl;
            return;
        }
        cout<<"Insert To '"<<__Name<<"' Success."<<endl;
        return ;
    }
    cout<<"No Such Table Named '"<<__Name<<"'."<<endl;
}

/*
    DELETE table_name(page_number:row_number);

*/


void Executor::execute_delete_row(){
    if(!Analyse("(")) return;
    int index = __DataBase->LocateByName(__Name);
    if(index!=-1){
        int length,page_number,row_number;
        string* words = Split(__Statement,':',length);
        if(!to_Int(words[0],page_number)) return;
        if(length == 1){
            if(!__DataBase->getDataBase()[index]->DeletePage(page_number)){
                cout<<"Delete Page From '"<<__Name<<"' Failed."<<endl;
                return;
            }
            cout<<"Delete Page From '"<<__Name<<"' Success."<<endl;
            return;
        }
        if(length != 2){
            cout<<"Parameter Mismatch."<<endl;
            return;
        }
        if(!to_Int(words[1],row_number)) return;
        if( ! __DataBase->getDataBase()[index]->DeleteRow(page_number,row_number)){
            cout<<"Delete Row From '"<<__Name<<"' Failed."<<endl;
            return;
        }
        cout<<"Delete Row From '"<<__Name<<"' Success."<<endl;
        return ;
    }
    cout<<"No Such Table Named '"<<__Name<<"'."<<endl;
}


void Executor::execute_remove_table(){
    int index = __DataBase->LocateByName(__Statement);
    if(index !=-1){
        (__DataBase->RemoveTable(index))->Remove();
        cout<<"Remove Table '"<<__Statement<<"' Success."<<endl;
        return;
    }
    cout<<"No Such Table Named '"<<__Statement<<"'."<<endl;
}


void Executor::execute_print_table(){
    int index = __DataBase->LocateByName(__Statement);
    if(index!=-1){
        __DataBase->getDataBase()[index]->Print();
        return ;
    }
    cout<<"No Such Table Named '"<<__Statement<<"'."<<endl;
}

