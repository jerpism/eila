#ifndef IO_H_
#define IO_H_

#include <stdint.h>

uint8_t port_in_b(uint16_t);
uint16_t port_in_w(uint16_t);
uint32_t port_in_dw(uint16_t);

void port_out_b(uint16_t, uint8_t);
void port_out_w(uint16_t, uint16_t);
void port_out_dw(uint16_t, uint32_t);

#endif
