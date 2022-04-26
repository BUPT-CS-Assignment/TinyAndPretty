#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string>

////split str by token string
char *nsplit(char *str, const char *token, int n)
{
	char *ret = str;
	while ((strncmp(++str, token, n) || (*str = 0)) && (*str != 0)) ;
	return ret;
}

////get GMT time 
static const char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char *mons[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
std::string getGMTtime()
{
	time_t timep;
	time(&timep);
	struct tm p;
	gmtime_r(&timep, &p);

	char buff[32] = {0};
	// Fri, 01 Apr 2022 07:26:49 GMT
	sprintf(buff , "%s, %02d %s %d %02d:%02d:%02d GMT",
			wday[p.tm_wday] , p.tm_mday	, mons[p.tm_mon] , (1900 + p.tm_year),
			p.tm_hour		 , p.tm_min	, p.tm_sec );
	return buff;
}

size_t preFetchLength(const char * str) {
	char buff[32] = {0};
	size_t cur = 0;
	while ( strncmp(++str, "Content-Length: " , 16) && (*str)) ;
	if(*str) while ( (buff[cur++] = (str + 16)[cur]) != '\r' )  ;

	return cur == 0 ? -1ULL : atoll(buff);
}
