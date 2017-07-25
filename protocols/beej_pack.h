/*
**  http://beej.us/guide/bgnet/output/html/multipage/advanced.html#serialization
*/

#ifndef BEEJ_PACK_H
#define BEEJ_PACK_H

unsigned int beej_pack(unsigned char *buf, char *format, ...);
void beej_unpack(unsigned char *buf, char *format, ...);

#endif
