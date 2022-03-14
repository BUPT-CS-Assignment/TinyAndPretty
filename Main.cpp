#include<iostream>
#include "Actuator.cpp"
using namespace std;


int main(/*int argc, char* argv[]*/){
    InputStream inputStream;
    Actuator ac;
    while(true){
        printf("LiteDB > ");
        switch (inputStream.Read()){
        case __EXIT : 
            printf("~\n");
            return 0;
        case __UNK :
            printf("Unknown Command '%s'\n",inputStream.getInput().c_str());
            break;
        case __HELP :
            printf("This is a help prompt.\n");
            break;
        case __OPERATE :
            ac.execute(inputStream);
        default :
            break;
        }
        inputStream.clearStatement();
    }
}