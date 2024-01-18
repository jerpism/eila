#include <io.h>

/* C wrappers for port input */
uint8_t port_in_b(uint16_t port){
    uint8_t ret;
    __asm__ volatile("\tin %%dx, %%al"
                    : "=a" (ret)
                    : "d" (port));
    return ret;
}

uint16_t port_in_w(uint16_t port){
    uint16_t ret;
    __asm__ volatile("\tin %%dx, %%ax"
                    : "=a" (ret)
                    : "d" (port));
    return ret;
}

uint32_t port_in_dw(uint16_t port){
    uint32_t ret;
    __asm__ volatile("\tin %%dx, %%eax"
                    : "=a" (ret)
                    : "d" (port));
    return ret;
}

/* C wrappers for port output */
void port_out_b(uint16_t port, uint8_t data){
    __asm__ volatile ("\tout %%al, %%dx"
                    : 
                    : "a"(data), "d" (port));
}

void port_out_w(uint16_t port, uint16_t data){
    __asm__ volatile ("\tout %%ax, %%dx"
                    : 
                    : "a"(data), "d" (port));
}
void port_out_dw(uint16_t port, uint32_t data){
    __asm__ volatile ("\tout %%eax, %%dx"
                    : 
                    : "a"(data), "d" (port));
}

