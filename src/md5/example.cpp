#include "md5.h"
#include <iostream>

int main() {
  std::cout<<"Input the string for MD5 process"<<std::endl;
  std::string f;
  std::cin >> f;

  std::cout << "After MD5 process, string now= ";
  std::string output = md5(f);
  std::cout << output << std::endl;
  return 0;
}