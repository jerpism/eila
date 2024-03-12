#include <arch/x86/interrupts.h>
#include <arch/x86/8259.h>
#include <arch/x86/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <vga.h>
#include <libc.h>

extern void *isr_stub_table[];
extern void isr_0x21();
extern void isr_0x80();

/* General exception handler */
__attribute__((noreturn))
void exception_handler(void);

/* IDT functions */
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
void idt_init();


/* Aligned IDT */
__attribute__((aligned(0x10)))
static struct idt_entry idt[256];

static bool vectors[IDT_MAX_DESCRIPTORS];
static struct idtr idtr;

/* Just hangs on exception */
void exception_handler(){
    __asm__ volatile("cli; hlt");
}

void isr_0x80(){
    print("Got int 0x80\n");
}

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags){
    struct idt_entry *descriptor = &idt[vector];

    descriptor->isr_low     = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs   = 0x08; /* CS offset from GDT */
    descriptor->attributes  = flags;
    descriptor->isr_high    = (uint32_t)isr >> 16;
    descriptor->reserved    = 0;
}


void idt_init(){
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;

    for(uint8_t vector = 0; vector < 32; ++vector){
       idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    /* Load IDT */
    __asm__ volatile("\tlidt %0"
                    :
                    : "m"(idtr));
    /* and enable interrupts */
    __asm__ volatile("sti");


}


