#ifndef __SJSON__
#define __SJSON__
#include <bits/stdc++.h>


namespace SimpleJson {

//set length of json char buff area for stringize()
static size_t JSON_BUFF = 2048;
//guess length of the string concated to buff and for realloc()
const size_t LENGTH_GUESS = 128;
//json-item type identifiers
enum class ItemType {EMPTY , INT , DOUBLE , BOOL , STRING , OBJECT , 
					INT_ARRAY , DOUBLE_ARRAY , BOOL_ARRAY , STRING_ARRAY , OBJECT_ARRAY};	

//json-item base class definition  for polymorphism
class JsonItemBase {
    ItemType type;
public :  
    std::string key;
    //constructor with 'key'
    JsonItemBase(std::string& _key , ItemType _type)  : type(_type) , key( std::move(_key) ){} ;
    //without 'key'
    explicit JsonItemBase(ItemType _type)  : type(_type) ,  key( "" ) {} ;
    //for distinguishing 
    virtual ItemType get_type() const {return ItemType::EMPTY;}
    virtual ~JsonItemBase() {};
};
//pointer set -- a kind of abstraction  
using JsonItemCluster = std::vector<JsonItemBase*> ;

#define def_Helper(type , name , keyword) \
class JsonItem##name: public JsonItemBase {\
public : \
    type value;\
    explicit JsonItem##name(std::string& _key , type &_val)  : JsonItemBase( _key , ItemType::keyword) , value( std::move(_val) ){}\
    ItemType get_type() const override {return ItemType::keyword;}\
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
def_Helper(std::vector<std::string> , StringArray 	, INT_ARRAY);

//json-object definition
class JsonItemObject: public JsonItemBase {
public : 
    JsonItemCluster value;
    //constructor with 'key'
    explicit JsonItemObject(std::string& _key , JsonItemCluster &_val)  :
            JsonItemBase( _key , ItemType::OBJECT)  , value( std::move(_val) ) {}
	//constructor without 'key'
    explicit JsonItemObject(JsonItemCluster &_val)  : 
			JsonItemBase(ItemType::OBJECT)          , value( std::move(_val) ) {}

    ItemType get_type() const override {return ItemType::OBJECT;}
    ~JsonItemObject() {for(auto &it :value) delete it;}
};

/* ** abstract json-item constructor and memory allocator ** */
class JsonCtor {
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
		std::string tmp(_val);
		ptr = new JsonItemString(_key , tmp );
	}
	JsonCtor(std::string _key, std::vector<JsonCtor> _val) {
		JsonItemCluster tmp;
		for(auto& it : _val) tmp.push_back(it.ptr);
		ptr = new JsonItemObject(_key , tmp);
	}
	JsonCtor(std::initializer_list<JsonCtor> _val) {
		JsonItemCluster tmp;
		for(auto& it : _val) tmp.push_back(it.ptr);
		ptr = new JsonItemObject(tmp);
	}
	//array constructor below
	JsonCtor(std::string _key , std::vector<int> &_val){
		ptr = new JsonItemIntArray(_key , _val);
	}	
	JsonCtor(std::string _key , std::vector<double> &_val){
		ptr = new JsonItemDoubleArray(_key , _val);
	}	
	JsonCtor(std::string _key , std::vector<bool> &_val){
		ptr = new JsonItemBoolArray(_key , _val);
	}	
	JsonCtor(std::string _key , std::vector<std::string> &_val){
		ptr = new JsonItemStringArray(_key , _val );
	}
};
// just for easy use...
using Object = JsonCtor;

#define BUFF_ADD(x) {if(cur >= JSON_BUFF-128) buff = (char *)realloc(buff , (JSON_BUFF <<= 1) ); buff[cur++]=(x);}


//helper function for base-ptr classification  and stringize()
static void handleItemBaseVector(JsonItemCluster &items , char *buff , size_t &cur ) noexcept {
    for(auto&it : items) {
    	if(it->key.length()) {
	        BUFF_ADD('"');
	        strcpy(buff + cur , it->key.c_str()); cur += it->key.length();
	        BUFF_ADD('"');
	        BUFF_ADD(':');
		}
        
        switch (it->get_type())
        {
            case ItemType::INT : 
            {
                JsonItemInt* p = dynamic_cast<JsonItemInt*>(it);
                cur += sprintf(buff + cur ,"%d" , p->value);
                break;
            }
            case ItemType::DOUBLE : 
            {
                JsonItemDouble* p = dynamic_cast<JsonItemDouble *>(it);
                cur += sprintf(buff + cur ,"%lf" , p->value);
                break;
            }
            case ItemType::BOOL : 
            {
                JsonItemBool* p = dynamic_cast<JsonItemBool *>(it);
                if(p->value) {strcpy(buff + cur , "true");  cur += 4;}
                else         {strcpy(buff + cur , "false"); cur += 5;}
                break;                    
            }
            case ItemType::STRING : 
            {
                JsonItemString* p = dynamic_cast<JsonItemString *>(it);
                BUFF_ADD('"');
                strcpy(buff + cur , p->value.c_str()); cur += p->value.length();
                BUFF_ADD('"');
                break;
            }
            case ItemType::OBJECT : 
            {
                JsonItemObject* p= dynamic_cast<JsonItemObject *>(it);
                BUFF_ADD('{');
                handleItemBaseVector(p->value , buff , cur);
                buff[cur-1] = '}';
                break;
            }
            case ItemType::INT_ARRAY : 
            {
                JsonItemIntArray* p= dynamic_cast<JsonItemIntArray *>(it);
                BUFF_ADD('[');
                for(auto &val : p->value) {
                	cur += sprintf(buff + cur ,"%d" , val);
                	BUFF_ADD(',');
				}
                buff[cur-1] = ']';
                break;
            }
            case ItemType::DOUBLE_ARRAY : 
            {
                JsonItemDoubleArray* p= dynamic_cast<JsonItemDoubleArray *>(it);
                BUFF_ADD('[');
                for(auto &val : p->value) {
                	cur += sprintf(buff + cur ,"%lf" , val);
                	BUFF_ADD(',');
				}
                buff[cur-1] = ']';
                break;
            }
            case ItemType::BOOL_ARRAY : 
            {
                JsonItemIntArray* p= dynamic_cast<JsonItemIntArray *>(it);
                BUFF_ADD('[');
                for(auto &val : p->value) {
                	if(val) {strcpy(buff + cur , "true");  cur += 4;}
                	else    {strcpy(buff + cur , "false"); cur += 5;}                	
					BUFF_ADD(',');
				}
                buff[cur-1] = ']';
                break;
            }
            case ItemType::STRING_ARRAY : 
            {
                JsonItemStringArray* p= dynamic_cast<JsonItemStringArray *>(it);
                BUFF_ADD('[');
                for(auto &val : p->value) {
                	strcpy(buff + cur , val.c_str()); cur += val.length();
                	BUFF_ADD(',');
				}
                buff[cur-1] = ']';
                break;
            }
            default : break;
        }
        BUFF_ADD(',');
    }
}

class Json {
    size_t cur = 0;
    char *buff = nullptr;
	JsonItemCluster items;
public :
/**
*	@brief 向创建的Json文件中添加一条记录。
*	@param _item 记录K-V对。请用 ' , ' 代替Json中的 ' : '，并使用'{ }'包括起来。
*/ 
	void push_back(JsonCtor _item) {
		items.push_back( _item.ptr );
	}
/**
*	@brief 将Json文件字符串化
*   @return const char* 指向结果的字符串
*/ 
    const char* stringize() {
        buff = new char[JSON_BUFF];
        BUFF_ADD('{');
        handleItemBaseVector(items , buff , cur);
        buff[cur-1] = '}';  
        return buff;
    }
/**
*	@brief 将Json文件字符串化
*   @param _buff 直接向指定的字符数组中进行字符串化
*   @return size_t 指向结果的字符串的大小
*/ 
    size_t stringize(char *_buff) {
        BUFF_ADD('{');
        handleItemBaseVector(items , buff , cur);
        buff[cur-1] = '}';  
        return cur;
    }
/**
*	@brief 返回Json文件字符串化后，该字符串的长度。如没有stringize()，则为0
*   @return size_t 结果字符串的长度
*/    
    size_t length() const {return cur;}
    ~Json() {
        if(buff != nullptr) 	delete buff; 
        for(auto &it : items) 	delete it;
    }
};

};

#endif
