#include <stdint.h>
#include <stdbool.h>
#include <vga.h>
#include <libc.h>
#include <arch/x86/io.h>
#include <arch/x86/interrupts.h>
#include <arch/x86/8259.h>

void pictest(){
    uint16_t test = 0;
    char buff[8];
    pic_remap(0x20, 0x28);
    idt_init();

    print("ISR register: ");
    test = pic_get_isr();
    itoa(test, buff, 16);
    print(buff);
    print("\n");

    print("IRR register: ");
    test = pic_get_irr();
    itoa(test, buff, 16);
    print(buff);
    print("\n");

    print("IMR register: ");
    test = pic_get_imr();
    itoa(test, buff, 16);
    print(buff);
    print("\n");
}

void main(){
    print("Hello you're in the kernel now\n");

    /* mask out all but keyboard */
    port_out_b(0x21, 0xFD);

    pictest();
    while(1) __asm__ volatile ("hlt");

}
