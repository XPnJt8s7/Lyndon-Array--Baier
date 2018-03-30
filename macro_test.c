#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  printf("hello\n");
  #if prints
    printf("world!\n");
  #endif
  
  return 0;
}
