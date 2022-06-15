#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <filesystem>

/* split str by token string */
char *nsplit(char *str, const char *token, int n)
{
	char *ret = str;
	while ((strncmp(++str, token, n) || (*str = 0)) && (*str != 0)) ;
	return ret;
}

/* get GMT time */ 
static const char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char *mons[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
std::string getGMTtime(uint32_t offset)
{
	time_t timep;
	time(&timep);
    timep += offset;
	struct tm p;
	gmtime_r(&timep, &p);

	char buff[32] = {0};
	// Fri, 01 Apr 2022 07:26:49 GMT
	sprintf(buff , "%s, %02d %s %d %02d:%02d:%02d GMT",
			wday[p.tm_wday] , p.tm_mday	, mons[p.tm_mon] , (1900 + p.tm_year),
			p.tm_hour		 , p.tm_min	, p.tm_sec );
	return buff;
}


std::string estimateFileType(std::filesystem::path p) 
{
	if (p.extension() == ".gz") p.replace_extension();

	if	   (p.extension() == ".html") 	return "text/html";
    else if(p.extension() == ".css") 	return "text/css";
	else if(p.extension() == ".txt") 	return "text/plain";
	else if(p.extension() == ".jpg") 	return "image/jpeg";
	else if(p.extension() == ".png") 	return "image/png";
	else if(p.extension() == ".gif") 	return "image/gif";
	else if(p.extension() == ".json") 	return "application/json";
	else if(p.extension() == ".js") 	return "application/javascript";
	else if(p.extension() == ".pdf") 	return "application/pdf";
	else if(p.extension() == ".mp4") 	return "video/mpeg4";
	else if(p.extension() == ".mp3") 	return "audio/mp3";
	else return "application/octet-stream";
}

/* get http body's length in http header */
size_t preFetchLength(const char * str) 
{
	char buff[32] = {0};
	size_t cur = 0;
	while ( strncasecmp(++str, "Content-Length: " , 16) && (*str)) ;
	if(*str) while ( (buff[cur++] = (str + 16)[cur]) != '\r' ) ;

	return cur == 0 ? -1ULL : atoll(buff);
}

/* get http header's length */
size_t preFetchHeader(const char * str)
{
	size_t cur = 0;
	while ( strncmp((char *)str + cur , "\r\n\r\n" , 4) && str[cur++]);
	return cur;
}
