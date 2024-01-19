#ifndef LIBC_H_
#define LIBC_H_
#include <stdint.h>

size_t strlen(const char*);
size_t strcpy(char*, const char*);
size_t memcpy(uint8_t*, uint8_t*, size_t);
char *itoa(int, char*, int);

#endif /* LIBC_H_ */
