#ifndef __CLOCK_SYS__
#define __CLOCK_SYS__

#include <common.h>
#include <interfaces.h>

namespace TimeStatus 
{
	enum TimeStatus {
		SUCC ,
		BROAD,
		DUP,
		ERR
	};
}

#endif