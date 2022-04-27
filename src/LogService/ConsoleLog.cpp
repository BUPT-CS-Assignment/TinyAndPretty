#include <common.h>
using namespace std;

int CONSOLE_LOG(int retVal,std::string content,...){
    /* Time Print */
    time_t timer;
    time(&timer);
    char * str = ctime(&timer);
    str[strlen(str)-1]='\0';
    printf("[%s CST] ", str);
    /* Log Print */
    va_list args;
    va_start(args,content);
    vprintf(content.c_str(),args);
    va_end(args);
    /* Return Value */
    return retVal;

}
