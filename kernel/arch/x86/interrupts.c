#include <arch/x86/interrupts.h>
#include <arch/x86/io.h>
#include <stdint.h>
#include <stdbool.h>


/* General exception handler */
__attribute__((noreturn))
void exception_handler(void);

/* IDT functions */
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
void idt_init();


/* Aligned IDT */
__attribute__((aligned(0x10)))
static idt_entry_t idt[256];


static idtr_t idtr;
extern void *isr_stub_table[];


/* Just hangs on exception */
void exception_handler(){
    __asm__ volatile("cli; hlt");
}

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags){
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low     = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs   = 0x08; /* CS offset from GDT */
    descriptor->attributes  = flags;
    descriptor->isr_high    = (uint32_t)isr >> 16;
    descriptor->reserved    = 0;
}


void idt_init(){
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for(uint8_t vector = 0; vector < 32; ++vector){
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    /* Load IDT */
    __asm__ volatile("\tlidt %0"
                    :
                    : "m"(idtr));
    /* and enable interrupts */
    __asm__ volatile("sti");
}

