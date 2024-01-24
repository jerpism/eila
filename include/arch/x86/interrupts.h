#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

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


#endif



