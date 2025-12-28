#include "../lib/lib.h"

#define MAX_LEN 100

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