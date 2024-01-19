#include <libc.h>

size_t strlen(const char *s){
    size_t len = 0;

    for(; s[len]; ++len);

    return len;
}


size_t strcpy(char *dst, const char *src){
    size_t len = 0;

    for(; src[len]; ++len){
        *dst++ = *src++;
    }

    dst[len] = '\0';

    return len;
}


/* TODO: THIS ONLY WORKS WITH SINGLE BYTES RIGHT NOW 
 * NOT COMPLETE IN ANY WAY */
size_t memcpy(uint8_t *dst, uint8_t *src, size_t n){
    size_t i;
    for(i = 0; i < n; ++i){
        *dst++ = *src++;
    }
    return i;
}



