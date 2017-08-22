#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "kyk_tx.h"

int main()
{
    struct kyk_tx tx0;
    tx0.version = 1;

    printf("++++++++%u\n", tx0.version);
}
    
