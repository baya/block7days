#include <stdio.h>

#include "test.h"

//#define A "a"

//static char *B="bbb";
char *B = "bbbbb";

static void b();
// void b();

void f()
{
    printf("f\n");
    b();
    printf("++++++%s\n", A);
    printf("?????%s\n", B);
}

void b()
{
    printf("b\n");
}
