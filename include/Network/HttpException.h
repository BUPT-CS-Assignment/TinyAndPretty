#ifndef __HTTP_EXCEPT__
#define __HTTP_EXCEPT__

enum class HttpException
{
	NON_POS,
	ERROR_LEN,
	OUT_OF_LIMIT,
	NON_PATH,
	NON_CONN,
	NON_FORM
};

#endif