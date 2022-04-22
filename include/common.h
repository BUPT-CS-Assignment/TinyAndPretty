#ifndef __COMMON__
#define __COMMON__
/*
	This header file mainly contains std library
	and might be included in the whole project.
*/

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

namespace fs = std::filesystem;

#ifdef DEBUG
#define IFDEBUG(...) __VA_ARGS__
#else
#define IFDEBUG(...) 
#endif

#endif