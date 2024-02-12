/* At the moment a *very* under construction PS/2 keyboard driver 
 * only works under the assumption of a scan set 1 keyboard being present 
 * AKA "US QWERTY" */
#include <keyboard.h>
#include <stdbool.h>
#include <arch/x86/io.h>
#include <vga.h>
#include <libc.h>
#include <stdint.h>
#include <arch/x86/interrupts.h>


/* Interrupt handler in kernel/arch/x86/isr.s
 * Only needed here for registering, calls isr_0x21 and handles iret
 *
 * TODO: maybe stop relying on separate asm handlers
 * and switch to using compiler attributes */
extern void _isr_0x21();


/* States of modifier keys */
static bool shift_state = 0;
static bool ctrl_state  = 0;
static bool alt_state   = 0;

/* TODO: move these to keyboard.h
 * and actually give them values */

/* Function keys */
#define F1  0
#define F2  0
#define F3  0
#define F4  0
#define F5  0
#define F6  0
#define F7  0
#define F8  0
#define F9  0
#define F10 0
#define F11 0
#define F12 0

#define ARROW_UP    0
#define ARROW_DOWN  0
#define ARROW_LEFT  0
#define ARROW_RIGHT 0

#define KB_HOME 0
#define KB_PGUP 0
#define KB_PGDN 0
#define KB_END  0

/* Map scancodes to ASCII values */ 
/* Could do with better formatting probably, breaks 80 column rule */
static unsigned char kbmap[] = {
    0,              /* 0x00, not a valid scancode */
    27,             /* 0x01 esc */
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* 0x02-0x0E */
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* 0x0F-0x1C */
    0,              /* 0x1D lctrl */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', /* 0x1E-0x28 */
    '`',            /* 0x29 backtick */
    '0',            /* 0x2A lshift */
    '\\',           /* 0x2B backslash */
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', /* 0x2C-0x34 */
    '/',            /* 0x35 slash */
    0,              /* 0x36 rshift */
    '*',            /* 0x37 keypad '*' */
    0,              /* 0x38 lalt */
    ' ',            /* 0x39 space */
    0,              /* 0x3A capslock */
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, /* 0x3B-0x0x44 */
    0,              /* 0x45 numlock */
    0,              /* 0x46 scrolllock */
    KB_HOME,        /* 0x47 home */
    ARROW_UP,       /* 0x48 up */
    KB_PGUP,        /* 0x49 pgup */
    '-',            /* 0x4A kp - */
    ARROW_LEFT,     /* 0x4B left */
    0,              /* 0x4C kp 5 */
    ARROW_RIGHT,    /* 0x4D right */
    '+',            /* 0x4E kp + */
    KB_END,         /* 0x4F end */
    ARROW_DOWN,     /* 0x50 down */
    KB_PGDN,        /* 0x51 pgdn */
    0,              /* 0x52 kp 0 */
    '.',            /* 0x53 kp . */
    0, 0, 0,        /* 0x54-0x56 unused */
    F11, F12        /* 0x57-0x58 */
};


void isr_0x21(){
    char buff[2];
    uint8_t kc = 0;

    kc = port_in_b(0x60);

    if(kc < 0x58){
        buff[0] = kbmap[kc];
        buff[1] = '\0';
        print(buff);
    }


}

void register_kb(){
    /* Note: casting a fp* to a void* is technically UB
     * but this should work for now even if it generates an ugly warning 
     * maybe replace with a separate function to register interrupt handlers */
    idt_set_descriptor(0x21, (void*)_isr_0x21, 0x8E);
}

