#include <connect/Timer/TimeLine.h>


const std::vector<int>
TimeLine::getVirtualTime(void)
{
	tag_T server_now = ct::system_clock::now();
	tag_T virtual_now;

	{
		std::shared_lock grd {t_lock};
		auto real_dur = ct::duration_cast<ct::seconds>
							(server_now - save_real);

		virtual_now = save_virl + 
					ct::seconds{real_dur.count() * runtime_r};
	}

	time_t timep = ct::system_clock::to_time_t(
			virtual_now + ct::hours{TIME_ZONE}
		);

	struct tm p {0};
	gmtime_r(&timep, &p);

	return {p.tm_year + 1900,
			p.tm_mon  + 1,
			p.tm_mday,
			p.tm_wday,
			p.tm_hour,
			p.tm_min ,
			p.tm_sec};
}

void TimeLine::changeRate(uint32_t new_R)
{

	tag_T server_now = ct::system_clock::now();

	{
		std::unique_lock grd {t_lock};
		auto real_dur = ct::duration_cast<ct::seconds>
							(server_now - save_real);

		this->save_virl += ct::seconds{real_dur.count() * runtime_r};
		this->save_real =  server_now;
		this->runtime_r =  new_R;
	}

}

uint32_t TimeLine::getRate(void) 
{
	std::shared_lock grd {t_lock};
	return runtime_r;
}
