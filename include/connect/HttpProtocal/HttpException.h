#ifndef __HTTP_EXCEPT__
#define __HTTP_EXCEPT__

//Definition of all exception might occur in http module; 
enum class HttpException
{
	NON_POS,		//not found as a result of searching instrution
	ERROR_LEN,		//verification error when Content-length differs from actual
	OUT_OF_LIMIT,	//the size of request or file is too large
	NON_PATH,		//url does not exists
	NON_CONN,		//connection offline ahead of time
	NON_FORM		//request body is not type of multipart/form-data 
};

#endif