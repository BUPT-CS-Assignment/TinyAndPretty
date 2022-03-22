#include <Network/HttpProtocal.h>

char *nsplit(char *str ,const char *token , int n);

void StringDict::__init__(char *str , const char *token_1 , const char *token_2) {
    char *fir , *sec; 
    len = strlen(str);
    size_t len_1 = strlen(token_1) , len_2 = strlen(token_2);
    nsplit(str , "" , 0);
    while(true) {
        fir = nsplit(nullptr , token_1 , len_1);
        sec = nsplit(nullptr , token_2 , len_2);
        if(fir >= len + str || sec >= len + str) break;
        item.push_back(std::make_pair(std::string(fir) , std::string(sec)));
    }
}

std::string& StringDict::get(const std::string& str) {
    for(auto &it : item) {
        if (it.first == str) return it.second;
    }
    throw (-1);
}

void StringDict::show() {
    for(auto &it : item) {
        std::cout << it.first <<"---" <<it.second << "\n";
    }
}

std::string& StringDict::operator[] (const std::string& str) {
    try { return get(str); }
    catch(int e) {
        item.push_back(std::make_pair(str , ""));
        return (item.end() - 1)->second;
    }
}

size_t HttpHeader::stringize(char *buff) {
    size_t cur = 0;

    for(auto &it : item) {
        strcpy(buff + cur, it.first.c_str()); cur += it.first.length();
        strcpy(buff + cur , "\r\n"); cur += 2;
        strcpy(buff + cur, it.second.c_str());cur += it.second.length();
    }
    strcpy(buff + cur ,"\r\n\r\n"); cur += 4;
    return cur;
}