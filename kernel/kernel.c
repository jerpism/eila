#include <stdint.h>
#include <stdbool.h>
#include <vga.h>
#include <libc.h>

#include <arch/x86/io.h>


void main(){
    int test = 0xBEEF;
    char buff[8];

    print("Hello you're in ther kernel now\n");
    itoa(test, buff, 16);
    print(buff);


}
