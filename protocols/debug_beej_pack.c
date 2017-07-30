#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include "beej_pack.h"

typedef float float32_t;
typedef double float64_t;

int main(void)
{
    unsigned char buf[1024];
    int8_t magic;
    int16_t monkeycount = 0x2345;
    int32_t altitude = 0x12345678;
    int64_t sg = 0x1122334455667788;
    float32_t absurdityfactor;
    char *s = "Great unmitigated Zot!  You've found the Runestaff!";
    char s2[96];
    int16_t packetsize, ps2;

    /* packetsize = beej_pack(buf, "chhlsf", (int8_t)'B', (int16_t)0, (int16_t)37,  */
    /* 			   (int32_t)-5, s, (float32_t)-3490.6677); */
    /* packi16(buf+1, packetsize); // store packet size in packet for kicks */

    /* printf("packet is %" PRId32 " bytes\n", packetsize); */

    /* beej_unpack(buf, "chhl96sf", &magic, &ps2, &monkeycount, &altitude, s2, */
    /* 	   &absurdityfactor); */

    /* printf("'%c' %" PRId32" %" PRId16 " %" PRId32 */
    /* 	   " \"%s\" %f\n", magic, ps2, monkeycount, */
    /* 	   altitude, s2, absurdityfactor); */

    packetsize = beej_pack(buf, "<h", monkeycount);
    printf("monkeycount packetsize: %d\n", packetsize);
    printf("monkeycount <h[0]:0x%x\n", *(buf+0));
    printf("monkeycount <h[1]:0x%x\n", *(buf+1));

    packetsize = beej_pack(buf, "<L", altitude);
    printf("altitude packetsize: %d\n", packetsize);
    printf("altitude <h[0]:0x%x\n", *(buf+0));
    printf("altitude <h[1]:0x%x\n", *(buf+1));
    printf("altitude <h[2]:0x%x\n", *(buf+2));
    printf("altitude <h[3]:0x%x\n", *(buf+3));

    packetsize = beej_pack(buf, "<q", sg);
    printf("sg packetsize: %d\n", packetsize);
    printf("sg <h[0]:0x%x\n", *(buf+0));
    printf("sg <h[1]:0x%x\n", *(buf+1));
    printf("sg <h[2]:0x%x\n", *(buf+2));
    printf("sg <h[3]:0x%x\n", *(buf+3));
    printf("sg <h[4]:0x%x\n", *(buf+4));
    printf("sg <h[5]:0x%x\n", *(buf+5));
    printf("sg <h[6]:0x%x\n", *(buf+6));
    printf("sg <h[7]:0x%x\n", *(buf+7));
    

    return 0;
}
