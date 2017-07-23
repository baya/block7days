#include <stdio.h>

void set_a(int a[]);
    
int main(void)
{
    int a[5];
    set_a(a);

    printf("a[0]:%d\n", a[0]);
    printf("a[1]:%d\n", a[1]);
}

void set_a(int a[])
{
    a[0] = 1;
    a[1] = 2;
}
