#include <libc.h>

size_t strlen(const char *s){
    size_t len = 0;

    for(; s[len]; ++len);

    return len;
}


size_t strcpy(char *dst, const char *src){
    size_t len = 0;

    for(len = 0; src[len]; ++len){
        dst[len] = src[len];
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


/* Mostly copied from wiki.osdev.org
 * but only supports up to base 16 for now */
char *itoa(int value, char *s, int base){
    char *rc;
    char *ptr;
    char *low;

    /* See if base is supported */
    if(base < 2 || base > 16){
        *s = '\0';
        return s;
    }

    rc = ptr = s;

    /* Set minus sign if negative decimal */
    if(value < 0 && base == 10){
        *ptr++ = '-';
    }

    /* Start point of the actual numbers */
    low = ptr;

    do{
        /* Clever trick to get rid of abs() */
        *ptr++ = "fedcba9876543210123456789abcdef"[15 + value % base];
        value /= base;
    }while(value);

    /* Terminate string */
    *ptr-- = '\0';

    /* And reverse it so it's the right way around 
     * Maybe replace this with a clever */
    while(low < ptr){
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}



