#ifndef __SJSON__
#define __SJSON__
#include <string>
#include <vector>
#include <cstring>

namespace SimpleJson {
//guess length of the string concated to buff and for realloc()
const size_t LENGTH_GUESS = 128;
//json-item type identifiers
enum class ItemType {EMPTY , INT , DOUBLE , BOOL , STRING , OBJECT , 
					INT_ARRAY , DOUBLE_ARRAY , BOOL_ARRAY , STRING_ARRAY , OBJECT_ARRAY};	

//json-item base class definition  for polymorphism
class JsonItemBase 
{
protected : 
	ItemType type;
public :  
	std::string key;
	//constructor with 'key'
	JsonItemBase(std::string& _key , ItemType _type)  : type(_type) , key( std::move(_key) ){} ;
	//without 'key'
	explicit JsonItemBase(ItemType _type)  : type(_type) ,  key( "" ) {} ;
	//for distinguishing 
	virtual ItemType getType() const {return ItemType::EMPTY;}
	virtual ~JsonItemBase() {};
};
//pointer set -- a kind of abstraction  
using JsonItemClusterBase = std::vector<JsonItemBase*> ;

#define def_Helper(type, name, keyword)                                                                                       \
class JsonItem##name : public JsonItemBase                                                                                    \
{                                                                                                                             \
public:                                                                                                                       \
	type value;                                                                                                               \
	explicit JsonItem##name(std::string &_key, type &_val) : JsonItemBase(_key, ItemType::keyword), value(std::move(_val)) {} \
	ItemType getType() const override { return ItemType::keyword; }                                                           \
}

//common type definitions
def_Helper(int 			, Int 		, INT);
def_Helper(double 		, Double 	, DOUBLE );
def_Helper(bool 		, Bool 		, BOOL );
def_Helper(std::string 	, String 	, STRING );

//array definitions
def_Helper(std::vector<int> 		, IntArray 		, INT_ARRAY);
def_Helper(std::vector<double> 		, DoubleArray 	, DOUBLE_ARRAY);
def_Helper(std::vector<bool> 		, BoolArray 	, BOOL_ARRAY);
def_Helper(std::vector<std::string> , StringArray 	, STRING_ARRAY);

//json-object and its array 's definitions
class JsonItemCluster: public JsonItemBase 
{
public : 
	JsonItemClusterBase value;
	//constructor with 'key'
	explicit JsonItemCluster(std::string& _key , JsonItemClusterBase& _val , ItemType _is_array)  :
			JsonItemBase( _key , _is_array)  , value( std::move(_val) ) {}
	//constructor without 'key'
	explicit JsonItemCluster(JsonItemClusterBase& _val)  : 
			JsonItemBase(ItemType::OBJECT)          , value( std::move(_val) ) {}

	ItemType getType() const override {return type;}
	~JsonItemCluster() { for(auto& it :value) delete it; }
};

/* ** abstract json-item constructor and memory allocator ** */
class JsonCtor 
{
public :
	JsonItemBase *ptr;

	JsonCtor(std::string _key , int _val){
		ptr = new JsonItemInt(_key , _val);
	}	
	JsonCtor(std::string _key , double _val){
		ptr = new JsonItemDouble(_key , _val);
	}	
	JsonCtor(std::string _key , bool _val){
		ptr = new JsonItemBool(_key , _val);
	}
	JsonCtor(std::string _key , const char _val[]){
		std::string tmp {_val};
		ptr = new JsonItemString(_key , tmp);
	}
	JsonCtor(std::string _key , std::string _val){
		ptr = new JsonItemString(_key , _val);
	}
	JsonCtor(std::initializer_list<JsonCtor> _val) {
		JsonItemClusterBase tmp;
		for(auto& it : _val) tmp.push_back(it.ptr);
		ptr = new JsonItemCluster(tmp);
	}
	JsonCtor(std::string _key, std::vector<JsonCtor>& _val) {
		JsonItemClusterBase tmp;
		for(auto& it : _val) tmp.push_back(it.ptr);
		ptr = new JsonItemCluster(_key , tmp , ItemType::OBJECT_ARRAY);
	}
	JsonCtor(std::string _key, std::vector<JsonCtor>&& _val) {
		JsonItemClusterBase tmp;
		for(auto& it : _val) tmp.push_back(it.ptr);
		ptr = new JsonItemCluster(_key , tmp , ItemType::OBJECT);
	}

	//array constructor below
	JsonCtor(std::string _key , std::vector<int>& _val){
		ptr = new JsonItemIntArray(_key , _val);
	}	
	JsonCtor(std::string _key , std::vector<double>& _val){
		ptr = new JsonItemDoubleArray(_key , _val);
	}	
	JsonCtor(std::string _key , std::vector<bool>& _val){
		ptr = new JsonItemBoolArray(_key , _val);
	}	
	JsonCtor(std::string _key , std::vector<std::string>& _val){
		ptr = new JsonItemStringArray(_key , _val );
	}
	
};
// just for easy use...
using Object = JsonCtor;

#define BUFF_REP(x) {buff[buff_cur-1] = (x);}
#define BUFF_ADD(x)                                          \
	{                                                        \
		if (buff_cur >= JSON_BUFF - LENGTH_GUESS)            \
			buff = (char *)realloc(buff, (JSON_BUFF <<= 1)); \
		buff[buff_cur++] = (x);                              \
	}

class Json 
{
public :
	Json() = default;
	Json( const Json& ) = delete;
	Json( Json&& _json ) : 
		buff_cur(_json.buff_cur) , item_cur(_json.item_cur) , buff( std::move(_json.buff) ) , items( std::move(_json.items) ) {}
	Json& operator = ( const Json& ) = delete;
	Json& operator = ( Json&& _json )
		{ buff_cur = _json.buff_cur , item_cur = _json.item_cur , buff = std::move(_json.buff) , items = std::move(_json.items) ;
		  return *this;}

	~Json() {
		if(buff != nullptr) 	delete buff; 
		for(auto& it : items) 	delete it;
	}
/**
*	@brief 向创建的Json文件中添加一条记录。
*	@param _item 记录K-V对。请用 ' , ' 代替Json中的 ' : '，并使用'{ }'包括起来。
*/ 
	void push_back(JsonCtor _item)    { items.push_back( _item.ptr ); }

/**
*	@brief Json文件的拼接
*/
	void push_back(const Json& _json) { items.insert(items.end() , _json.items.begin() , _json.items.end()); }

/**
*	@brief 将Json文件字符串序列化
*   @return const char* 指向结果的字符串
*/ 
	const char* stringize() {
		// buffer size initialize
		if(buff == nullptr) {
			buff = static_cast<char *>( calloc(1 , JSON_BUFF) );
			BUFF_ADD('{'); BUFF_ADD('}');
		}

		// check whether it is up to date 
		if( item_cur == items.size())
			return buff;
		
		// recursively stringize
		if( item_cur != 0 ) 
			 BUFF_REP(',')
		else buff_cur--;

		handleItemBaseVector(items);

		BUFF_REP('}');
		item_cur = items.size();

		return buff;
	}

/**
*	@brief 返回Json文件字符串化后，该字符串的长度。如没有stringize()，则为0
*   @return size_t 结果字符串的长度
*/  
	size_t length() const {return buff_cur;}


private : 
	size_t buff_cur  = 0;    // buff cur will stay at the end of CLOSED char interval
	size_t item_cur  = 0;
	size_t JSON_BUFF = 4096; // set length of json char buff area for stringize()
	char *buff 		 = nullptr;
	JsonItemClusterBase items;
	
	void handleItemBaseVector(JsonItemClusterBase& items) noexcept;
};

//helper function for base-ptr classification  and stringize()
inline void 
	Json::handleItemBaseVector
	(JsonItemClusterBase& items) noexcept 
{
	for(auto& it : items) 
	{
		if(it->key.length()) 
		{
			BUFF_ADD('"');
			strcpy(buff + buff_cur , it->key.c_str()); buff_cur += it->key.length();
			BUFF_ADD('"'); BUFF_ADD(':');
		}

		switch (it->getType())
		{
			case ItemType::INT : 
			{
				JsonItemInt* p = static_cast<JsonItemInt*>(it);
				buff_cur += sprintf(buff + buff_cur ,"%d" , p->value);
				break;
			}
			case ItemType::DOUBLE : 
			{
				JsonItemDouble* p = static_cast<JsonItemDouble *>(it);
				buff_cur += sprintf(buff + buff_cur ,"%lf" , p->value);
				break;
			}
			case ItemType::BOOL : 
			{
				JsonItemBool* p = static_cast<JsonItemBool *>(it);
				if(p->value) {strcpy(buff + buff_cur , "true");  buff_cur += 4;}
				else         {strcpy(buff + buff_cur , "false"); buff_cur += 5;}
				break;                    
			}
			case ItemType::STRING : 
			{
				JsonItemString* p = static_cast<JsonItemString *>(it);
				BUFF_ADD('"');
				strcpy(buff + buff_cur , p->value.c_str()); buff_cur += p->value.length();
				BUFF_ADD('"');
				break;
			}
			case ItemType::OBJECT : 
			{
				JsonItemCluster* p = static_cast<JsonItemCluster *>(it);
				BUFF_ADD('{');
				handleItemBaseVector(p->value);
				BUFF_REP('}');
				break;
			}
			case ItemType::INT_ARRAY : 
			{
				JsonItemIntArray* p = static_cast<JsonItemIntArray *>(it);
				BUFF_ADD('[');
				for(auto& val : p->value) 
				{
					buff_cur += sprintf(buff + buff_cur ,"%d" , val);
					BUFF_ADD(',');
				}
				BUFF_REP(']');
				break;
			}
			case ItemType::DOUBLE_ARRAY : 
			{
				JsonItemDoubleArray* p = static_cast<JsonItemDoubleArray *>(it);
				BUFF_ADD('[');
				for(auto& val : p->value) 
				{
					buff_cur += sprintf(buff + buff_cur ,"%lf" , val);
					BUFF_ADD(',');
				}
				BUFF_REP(']');
				break;
			}
			case ItemType::BOOL_ARRAY : 
			{
				JsonItemBoolArray* p = static_cast<JsonItemBoolArray *>(it);
				BUFF_ADD('[');
				for(auto val : p->value) 
				{
					if(val) {strcpy(buff + buff_cur , "true");  buff_cur += 4;}
					else    {strcpy(buff + buff_cur , "false"); buff_cur += 5;}                	
					BUFF_ADD(',');
				}
				BUFF_REP(']');
				break;
			}
			case ItemType::STRING_ARRAY : 
			{
				JsonItemStringArray* p = static_cast<JsonItemStringArray *>(it);
				BUFF_ADD('[');
				for(auto& val : p->value) 
				{
					BUFF_ADD('"');
					strcpy(buff + buff_cur , val.c_str()); buff_cur += val.length();
					BUFF_ADD('"'); BUFF_ADD(',');
				}
				BUFF_REP(']');
				break;
			}
			case ItemType::OBJECT_ARRAY :
			{
				JsonItemCluster* p = static_cast<JsonItemCluster *>(it);
				BUFF_ADD('[');
				handleItemBaseVector(p->value);
				BUFF_REP(']');
				break;
			}
			default : break;
		}
		BUFF_ADD(',');
	}
}


};

#endif
