#include<DataBase.h>
#include<Data.h>
#include<Process.h>


void DataBase::Run(){
    InputStream inputStream;
    Executor executor(this);
    while(true){
        printf("LiteDB > ");
        switch (inputStream.Read()){
            case __EXIT : 
                printf("~\n");
                return;
            case __UNK :
                printf("Unknown Command '%s'\n",inputStream.getInput().c_str());
                break;
            case __HELP :
                printf("This is a help prompt.\n");
                break;
            case __OPERATE :
                executor.Execute(inputStream.getOperation(),inputStream.getStatement());
                break;
            default :
                break;
        }
        inputStream.ClearStatement();
    }
}


int DataBase::LocateByName(string name){
    for(int i=  0; i < __Cursor__; i++){
        string temp = __DataBase__[i]->getName();
        if(temp.compare(name) == 0) return i; 
    }
    return -1;
}

DataBase::DataBase(){
    __DataBase__ = new Table*[256];
    __Cursor__ = 0;
}