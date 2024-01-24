#include <arch/x86/interrupts.h>
#include <arch/x86/io.h>
#include <stdint.h>
#include <stdbool.h>

#define ICW1_ICW4       0x01    /* ICW4 will be present */
#define ICW1_SINGLE     0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04    /* Call address interval 4(8) */
#define ICW1_LEVEL      0x08    /* Level triggered (edge) mode */
#define ICW1_INIT       0x10    /* Initialization */

#define ICW4_8086       0x01    /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02    /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08    /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C    /* Buffered mode/master */
#define ICW4_SFNM       0x10    /* Special fully nested (not) */


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


/* offset 1 is vector offset for master PIC
 * offset 2 is vector offset for slave PIC */
void pic_remap(int offset1, int offset2){
    uint8_t a1, a2;

    /* Save current masks */
    a1 = port_in_b(PIC1_DATA);
    a2 = port_in_b(PIC2_DATA);

    /* Start init sequence in cascade mode for both */
    port_out_b(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    port_out_b(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    /* Offsets */
    port_out_b(PIC1_DATA, offset1); /* Master PIC vector offset */
    io_wait();
    port_out_b(PIC2_DATA, offset2); /* Slave PIC vector offset */
    io_wait();

    port_out_b(PIC1_DATA, 0x4);     /* Tell master that there's a slave at IRQ2 (0000 0100) */
    io_wait();
    port_out_b(PIC2_DATA, 0x2);     /* Tell slave that its cascade identity is (0000 0010) */
    io_wait();

    /* Set both PICs to 8086 mode */
    port_out_b(PIC1_DATA, ICW4_8086);
    io_wait();
    port_out_b(PIC2_DATA, ICW4_8086);
    io_wait();

    /* Restore saved masks */
    port_out_b(PIC1_DATA, a1);
    port_out_b(PIC2_DATA, a2);

}


void irq_set_mask(uint8_t line){
    uint16_t port;
    uint8_t val;

    if(line < 8){
        port = PIC1_DATA;
    }else{
        port = PIC2_DATA;
        line -= 8;
    }

    val = port_in_b(port) | (1 << line);
    port_out_b(port, val);
}


void irq_clear_mask(uint8_t line){
    uint16_t port;
    uint8_t val;

    if(line < 8){
        port = PIC1_DATA;
    }else{
        port = PIC2_DATA;
        line -= 8;
    }

    val = port_in_b(port) & ~(1 << line);
    port_out_b(port, val);

}


static uint16_t _pic_get_irq_reg(int ocw3){
    port_out_b(PIC1_COMMAND, ocw3);
    port_out_b(PIC2_COMMAND, ocw3);
    return (port_in_b(PIC2_COMMAND) << 8) | port_in_b(PIC1_COMMAND);
}


/* Read irq request register of PICs */
uint16_t pic_get_irr(){
    return (_pic_get_irq_reg(0x0a));
}


/* Read in-service register of PICs */
uint16_t pic_get_isr(){
    return (_pic_get_irq_reg(0x0b));
}

/* Reads IMR of both PICs */
uint16_t pic_get_imr(){
    return (port_in_b(PIC2_DATA) << 8) |port_in_b(PIC1_DATA);
}
