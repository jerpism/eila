#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

#define PIC1            0x20
#define PIC2            0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)

/* Taken from wiki.osdev.org */
typedef struct {
    uint16_t    isr_low;    /* lower 16 bits of ISR address */
    uint16_t    kernel_cs;  /* GDT segment selector to be loaded into CS */
    uint8_t     reserved;   /* This must always be 0 */
    uint8_t     attributes; /* Type and attributes */
    uint16_t    isr_high;   /* higher 16 bits of ISR address */
}__attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t    limit; 
    uint32_t   base; 
}__attribute__((packed)) idtr_t;

void idt_init(void);
void pic_remap(int, int);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

/* TODO: these maybe don't need to be visible */
void irq_clear_mask(uint8_t);
void irq_set_mask(uint8_t);

#endif



