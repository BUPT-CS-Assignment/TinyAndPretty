#ifndef __TIME_L__
#define __TIME_L__

#include <common.h>
#include <shared_mutex>
#include "TimerConfig.h"

namespace ct = std::chrono;

/* 
TimeLine Struct

   Real_o->
	o--------*------*----->
	| ratio1  \ 2  /   3
	o----------*--*------->
   Virl_o->

   WARNING: NOT tested Muti-Thread Safety;
*/
class TimeLine
{
	using tag_T = ct::system_clock::time_point;
public:
	const std::vector<int> getVirtualTime(void);

	void 	 changeRate(uint32_t new_R);
	uint32_t getRate(void) ;

private:
	std::shared_mutex t_lock;

	tag_T server_epoch = ct::system_clock::now();
	tag_T save_real = server_epoch;
	tag_T save_virl = server_epoch;

	uint32_t runtime_r = INIT_RATIO;
};

#endif