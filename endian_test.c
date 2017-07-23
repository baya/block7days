#include <stdio.h>

int is_little_endian();

int main()
{
    int i;
    unsigned int val = 0x12345678;
    unsigned int lx;
    unsigned int rx;
    printf("[0]:0x%x\n",*((char*)&val + 0));
    printf("[1]:0x%x\n",*((char*)&val + 1));
    printf("[2]:0x%x\n",*((char*)&val + 2));
    printf("[3]:0x%x\n",*((char*)&val + 3));

    lx = val << 8;
    rx = val >> 8;
    printf("val << 8: 0x%02x\n", lx);
    printf("val >> 8: 0x%02x\n", rx);

    printf("[0]:0x%02x\n",*((char*)&rx + 0));
    printf("[1]:0x%02x\n",*((char*)&rx + 1));
    printf("[2]:0x%02x\n",*((char*)&rx + 2));
    printf("[3]:0x%02x\n",*((char*)&rx + 3));

    lx = (val >> 8) & 0xff;
    printf("lx: 0x%02x\n", lx);
    

    i = is_little_endian();
    if(i){
	printf("litte endian\n");
    } else {
	printf("big endian\n");
    }
}


int is_little_endian()
{
    union{
	unsigned int a;
	unsigned char b;
    } c;

    c.a = 1;
    return (c.b == 1);
}










