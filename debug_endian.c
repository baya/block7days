#include <stdio.h>

int main()
{
  int x = 1;

  char *y = (char*)&x;

  printf("%d\n", *(char*)(&x));
}
