#ifndef __KYK_UTILS_H
#define __KYK_UTILS_H

void kyk_print_hex(const char *label, const uint8_t *v, size_t len);
uint8_t kyk_hex2byte(const char ch);
void kyk_parse_hex(uint8_t *v, const char *str);
uint8_t *kyk_alloc_hex(const char *str, size_t *len);
void kyk_reverse(uint8_t *dst, size_t len);

#endif
