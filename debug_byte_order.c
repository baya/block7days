#include <stdio.h>

int main()
{
    int a = 8;
    char p;
    char *k;
    
    p = (char)&a;

    a = a >> 1;
    k = &a;

    return 0;
}
