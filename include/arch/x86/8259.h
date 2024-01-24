#ifndef PIC_8259_H_
#define PIC_8259_H_

#include <stdint.h>

#define PIC1            0x20
#define PIC2            0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2 + 2)


void pic_remap(int offset1, int offset2);

void irq_set_mask(uint8_t line);
void irq_clear_mask(uint8_t line);

uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);
uint16_t pic_get_imr(void);

#endif
