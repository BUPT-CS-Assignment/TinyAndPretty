#include <connect/HttpProtocal/HttpProtocal.h>
#include <connect/HttpProtocal/HttpException.h>

/*------------------------StringDict------------------------*/

char *nsplit(char *str, const char *token, int n);

#define CUR_MOV(offset) do {\
		cur += offset;  \
		if (cur >= len) \
			return ;      \
	} while (0)

//Initialize. the first token is the separator between two string in one pair
//			  the second one is between two different pairs       
void StringDict::__init__(
	char *str, 
	const char *token_1, 
	const char *token_2) noexcept
{
	char *fir, *sec;
	len = strlen(str);

	size_t cur = 0;
	size_t len_1 = strlen(token_1) , len_2 = strlen(token_2);

	for(;;) {
		fir = nsplit(str + cur, token_1, len_1);
		CUR_MOV(strlen(fir) + len_1); 

		sec = nsplit(str + cur, token_2, len_2); 
		item.push_back( std::make_pair(std::string{fir}, std::string{sec}) );
		CUR_MOV(strlen(sec) + len_2);
	}
}
/* Query and Fetch origin data in this dict. HttpException::NON_POS would occur when not found */
std::string& StringDict::get(std::string_view key , bool is_insen)
{
	for (auto& it : item) {

		if(
			std::equal(
				it.first.begin() , it.first.end() , 
				key.begin() , 
				[=](char a , char b){ 
					return (!is_insen) ? a == b : 
						std::tolower(a) == std::tolower(b);
			})
		)	return it.second;

	}
	throw HttpException::NON_POS;
}
/* Append one pair to dict in the end */
void StringDict::push(std::string _fir, std::string _sec) noexcept
{
	len += _fir.length() + 2 + _sec.length() + 2;
	item.push_back(std::make_pair(std::move(_fir), std::move(_sec)));
}

/* Display this stringdict on stdout */
void StringDict::show()
{
	for (auto& it : item) {
		std::cout << it.first << "---" << it.second << "\n";
	}
}

//query and fetch origin data in this dict. HttpException::NON_POS would occur when not found
std::string& StringDict::operator[](std::string_view str) noexcept
{
	try {
		return get(str);
	} catch (const HttpException& e) {
		item.push_back(std::make_pair( std::string{str} , ""));
		return (item.end() - 1)->second;
	}
}

#define BUFF_CPY(src , len) do {\
		strncpy(buff + cur, src , len); \
		cur += len;\
	} while (0)
//stringize to byte stream / string
size_t StringDict::stringize(char* buff)
{
	size_t cur = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
	for (auto& it : item) {
		//key string
		BUFF_CPY(it.first.c_str() , it.first.length());
		BUFF_CPY(": " , 2);

		//value string
		BUFF_CPY(it.second.c_str() , it.second.length());
		BUFF_CPY("\r\n" , 2);
	}
	BUFF_CPY("\r\n" , 2);
#pragma GCC diagnostic pop
	return cur;
}
//Initialize. Constructor by byte range. it could be given by FormData ctor.
//			  this range is from last boundary to next boundary
FormItem::FormItem(uint8_t *_begin, uint8_t *_end)
{
	//split the header info and create it
	nsplit((char *)_begin, "\r\n\r\n", 4);
	headers = std::make_unique<StringDict>((char *)_begin, ": ", "\r\n");
	
	std::string& disposition = headers->get("Content-Disposition" , true);

	//split the filename* info
	if (size_t t_pos = disposition.find(" filename*"); 
		t_pos != disposition.npos )
	{
		// filename = disposition.substr(t_pos + 9); // 9 : sizeof "filename="
		// filename = filename.substr(1, filename.length() - 2); // erase ""
		disposition = disposition.substr(0, t_pos - 1); // 2 : sizeof "; "
	}

	//split the filename info
	if (size_t t_pos = disposition.find(" filename"); 
		t_pos != disposition.npos )
	{
		std::cerr << t_pos << "\n";
		filename = disposition.substr(t_pos + 10); // 10 : sizeof " filename="
		filename = filename.substr(1, filename.length() - 2); // erase ""
		disposition = disposition.substr(0, t_pos - 1); // 1 : sizeof ";"
	}
	
	//split the key name info
	if (size_t t_pos = disposition.find(" name"); 
		t_pos != disposition.npos )
	{
		name = disposition.substr(t_pos + 6); // 6 : sizeof " name="
		name = name.substr(1, name.length() - 2);// erase ""
		disposition = disposition.substr(0, t_pos - 1);// 1 : sizeof ";"
	}
	//the reminder is copied to body as form item data as byte stream.
	_begin += headers->length() + 4;

	len = _end - _begin;
	data = std::make_unique<uint8_t[]>(len);
	memcpy(data.get(), _begin, len);
}

//method of inverting to filestream.	NOT guarantee correctness 
std::fstream  &operator<<(std::fstream&  out, const FormItem& _this)
{
	out.write((char *)(_this.data.get()), _this.len);
	return out;
}
std::ofstream &operator<<(std::ofstream& out, const FormItem& _this)
{
	out.write((char *)(_this.data.get()), _this.len);
	return out;
}

//initialize. arguments could be given by HttpRequest ctor.
FormData::FormData(
	std::string& _boundary, 
	uint8_t *body, 
	size_t len) 
: boundary(std::move(_boundary))
{
	uint8_t *fir = nullptr;
	
	for (size_t cur = 0; cur < len; cur++) {
		//detected boundary in body
		if (memcmp(body + cur , "--" , 2)  ||
			memcmp(body + cur + 2, boundary.c_str(), boundary.length()) )
			continue;
		
		//skip the first one;
		if (fir != nullptr) 
			form.emplace_back(fir, body + cur - 1);

		cur += 4 + boundary.length(); // 4 : sizeof "--" "\r\n"
		fir = body + cur;
	}
}

//query form item by key name
FormItem& FormData::queryItem(std::string_view _name)
{
	for (auto& it : form) {
		if (it.name == _name)
			return it;
	}
	throw HttpException::NON_POS;
}
