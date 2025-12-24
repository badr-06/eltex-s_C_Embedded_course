#include <stdio.h>

#define MAX_LEN 100

size_t mystrlen(char* str){
  
  size_t size = 0;

  while (*str != '\0' && *str != '\n')
  {
    ++size;
    ++str;
  }
  
  return size;
}

char* find_substring(char* str, char* sub_str){
  
  if(mystrlen(str) < mystrlen(sub_str)) return NULL;

  char* ptr = NULL;

  while(*str != 0){
    int res = 1;
    char* temp_str = str;
    char* temp_substr = sub_str;
    while(*temp_substr != 0 && *temp_str != 0){
      if((*temp_str) - (*temp_substr) != 0){ 
        res = 0;
        break;
      }
      ++temp_str;
      ++temp_substr;
    }
    if(res && (*temp_str == 0)) ptr = str;
    ++str;
  }

  return ptr;
}

int main(){

  char str[MAX_LEN];
  fgets(str, MAX_LEN, stdin);
  (*find_substring(str, "\n")) = '\0';

  char sub_str[MAX_LEN];
  fgets(sub_str, MAX_LEN, stdin);
  (*find_substring(sub_str, "\n")) = '\0';
  
  printf("%s\n", str);
  printf("%s\n", sub_str);
  char *ptr = find_substring(str, sub_str);

  ptr == NULL ? printf("%p\n", ptr) : printf("%c\n", *ptr);

  return 0;
}