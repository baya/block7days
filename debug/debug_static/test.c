#include <stdio.h>

#include "test.h"

static void b();
// void b();

void f()
{
    printf("f\n");
    b();
}

void b()
{
    printf("b\n");
}
