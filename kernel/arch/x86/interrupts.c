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
static idt_entry_t idt[256];

static bool vectors[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;

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

void isr21(){
    char buff[16];
    uint8_t kc = 0;

    /* reads out kb buffer and prints out scancode */
    kc = port_in_b(0x60);
    itoa(kc, buff, 16);
    print(buff); 
}

void isr80(){
    print("Got int 0x80\n");
}

void idt_init(){
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for(uint8_t vector = 0; vector < 32; ++vector){
       idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    /* Note: casting a fp* to a void* is technically UB
     * but this should work for now even if it generates an ugly warning */
    idt_set_descriptor(0x21, (void*)isr_0x21, 0x8E);

    /* Load IDT */
    __asm__ volatile("\tlidt %0"
                    :
                    : "m"(idtr));
    /* and enable interrupts */
    __asm__ volatile("sti");


}


