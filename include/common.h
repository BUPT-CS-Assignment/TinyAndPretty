#ifndef __COMMON__
#define __COMMON__

#include <vector>
#include <string>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>

#define ERROR(cond , msg , ...) if(!(cond)) {\
    fprintf(stderr ,"\033[1;41m" msg "\033[0m""\n", ## __VA_ARGS__);\
    assert(cond);\
}

#endif