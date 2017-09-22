#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    FILE *fp;
    uint32_t magic_no;
    
    fp = fopen(argv[1], "rb");
    fread(&magic_no, sizeof(uint32_t), 1, fp);

    printf("%x\n", magic_no);
}
