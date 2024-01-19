#ifndef STDINT_H
#define STDINT_H

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

/* Define size_t as a 32bit uint on this system */
typedef unsigned int        size_t;
typedef signed int          ssize_t;


#endif /* STDINT_H */
