#include "lib.h"

size_t mystrlen(char *str){
  
  size_t size = 0;

  while (*str != '\0' && *str != '\n')
  {
    ++size;
    ++str;
  }
  
  return size;
}

char *find_substring(char *str, char *sub_str)
{

    if (mystrlen(str) < mystrlen(sub_str))
        return NULL;

    char *ptr = NULL;

    while (*str != 0)
    {
        int res = 1;
        char *temp_str = str;
        char *temp_substr = sub_str;
        while (*temp_substr != 0 && *temp_str != 0)
        {
            if ((*temp_str) - (*temp_substr) != 0)
            {
                res = 0;
                break;
            }
            ++temp_str;
            ++temp_substr;
        }
        if (res && (*temp_str == 0 || *temp_substr == 0)){
            ptr = str;
            break;
        }
        ++str;
    }

    return ptr;
}
