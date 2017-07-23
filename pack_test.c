#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>


void packi16(unsigned char *buf, unsigned int i);
void packi32(unsigned char *buf, unsigned long int i);

int main()
{
    unsigned char buf[1024];
    int16_t i = 0x4321;
    char *cptr = (char *)&i;
    int32_t j = 0x43215678;

    printf("[0]:0x%x\n",*cptr);
    printf("[1]:0x%x\n",*(cptr+1));

    printf("\n");
    packi16(buf, i);
    printf("pk16: [0]:0x%x\n", buf[0]);
    printf("pk16: [1]:0x%x\n", buf[1]);

    printf("\n");
    packi32(buf, j);
    printf("pk32: [0]:0x%x\n", buf[0]);
    printf("pk32: [1]:0x%x\n", buf[1]);
    printf("pk32: [2]:0x%x\n", buf[2]);
    printf("pk32: [3]:0x%x\n", buf[3]);
    
}

/*
** packi16() -- store a 16-bit int into a char buffer (like htons())
*/ 
void packi16(unsigned char *buf, unsigned int i)
{
    *buf++ = i>>8; *buf++ = i;
}

void packi32(unsigned char *buf, unsigned long int i)
{
    *buf++ = i>>24; *buf++ = i>>16;
    *buf++ = i>>8;  *buf++ = i;
}
