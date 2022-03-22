#include <common.h>

char *nsplit(char *str ,const char *token , int n) {
    static char *ptr = str;
    if(str != nullptr) ptr = str;
    if(n == 0) return ptr;

    char *ret = ptr ;
    while( (strncmp( ++ptr , token , n ) || (*ptr = 0)) && (*ptr != 0)  ) ;
    ptr += n;
    return ret;
}