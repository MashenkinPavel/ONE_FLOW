#ifndef COMMON_H
#define COMMON_H

#include <string.h>

bool find_arg(const int argc, char *argv[] , const char* key, std::string& value );
bool find_arg(const int argc,  char *argv[], const char key[], std::string& value ){
 for (int i=0;i<argc;i++) {
     std::string str = argv[i];
     if (str == key) {
         value = argv[i + 1];
         return true;
     }
 }
 return false;
}


#endif