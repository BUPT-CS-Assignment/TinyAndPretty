#ifndef __BASIC_UTILS_HPP__
#define __BASIC_UTILS_HPP__
#include <iostream>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

/**
 * @brief Basic Utils NameSpace
 * 
 */
namespace UTILSTD{

    /**
     * @brief LOG info at console
     *
     * @param returnVal return value
     * @param timeSwitch time print flag
     * @param level print minimal debug level
     * @param content printf content
     * @param ... printf args
     * @return int returnVal
     */
    inline int CONSOLE_LOG(int returnVal, bool timeSwitch, bool outputSwitch, std::string content, ...){
        /* Time Print */
        if(timeSwitch == 1){
            time_t timer;
            time(&timer);
            char* str = ctime(&timer);
            str[strlen(str) - 1] = '\0';
            printf("[%s CST] ", str);
        }
        /* Log Print */
        if(outputSwitch == 1){
            va_list args;
            va_start(args, content);
            vprintf(content.c_str(), args);
            va_end(args);
        }
        /* Return Value */
        return returnVal;
    }

    












    /**
     * @brief erase space at head and tail of a string
     *
     * @param str string
     * @return std::string
     */
    inline std::string Trim(std::string str){
        std::string temp = str;
        temp.erase(0, temp.find_first_not_of(" "));
        temp.erase(temp.find_last_not_of(" ") + 1);
        return temp;
    }

    /**
     * @brief split string by char
     *
     * @param str string
     * @param c split charactor
     * @param length splited number
     * @return std::string* splited string array
     */
    inline std::string* Split(std::string str, char c, int& length){
        if(str.length() == 0){
            length = 0;
            return NULL;
        }
        int count = 1;   // 拆分部分数量
        int index = 0;   // 字符索引位置
        int l = str.length();
        index = str.find(c, index);
        while(index < l && index >= 0){
            count++;
            index++;
            index = str.find(c, index);
        }
        std::string* words = new std::string[count];   //拆分部分数组
        int start = 0, end = str.find(c, start);
        for(int i = 0; i < count; i++){
            words[i] = Trim(str.substr(start, end - start));
            start = end + 1;
            end = str.find(c, start);
            if(end == -1) end = str.length();
        }
        length = count;
        return words;
    }




    /**
     * @brief quick sort template
     *
     * @tparam T
     * @param array array pointer
     * @param l sort start position
     * @param r sort end position
     */
    template<class T>
    void q_sort(T** array, int l, int r){
        if(l >= r) return;
        int blank = l + rand() % (r - l + 1);
        int i = l, j = r;
        T* key = array[blank];
        while(i < j){
            while(j > blank){
                if(*(array[j]) < *key){
                    array[blank] = array[j];
                    blank = j;
                }
                else j--;
            }
            while(i < blank){
                if(*(array[i]) > *key){
                    array[blank] = array[i];
                    blank = i;
                }
                else i++;
            }
        }
        array[blank] = key;
        q_sort(array, l, blank - 1);
        q_sort(array, blank + 1, r);
    }

}

#endif