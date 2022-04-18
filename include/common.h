#ifndef __COMMON__
#define __COMMON__

#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <memory>
#include <tuple>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

#ifdef DEBUG
#define IFDEBUG(...) (__VA_ARGS__)
#else
#define IFDEBUG(...) 
#endif

#endif