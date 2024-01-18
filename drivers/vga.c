/* Some (very) basic 80x25 text mode control */
#include <vga.h>
#include <io.h>
#include <stdint.h>

#define SCREEN_WIDTH   80
#define SCREEN_HEIGHT  25

/* I/O registers */
#define SCREEN_CTRL_REG 0x3d4
#define SCREEN_DATA_REG 0x3d5


typedef enum{
    BLACK   = 0x00,
    BLUE    = 0x01,
    GREEN   = 0x02,
    CYAN    = 0x03,
    RED     = 0x04,
    MAGENTA = 0x05,
    BROWN   = 0x06,
    WHITE   = 0x07,
    GRAY    = 0x08,
    LBLUE   = 0x09,
    LGREEN  = 0x0a,
    LCYAN   = 0x0b,
    LRED    = 0x0c,
    LMAGENTA= 0x0d,
    YELLOW  = 0x0e,
    BWHITE  = 0x0f
}vga_color_t;


/* Get value for a given foreground/background pair */
static inline uint8_t get_color(vga_color_t fg, vga_color_t bg){
    return (bg << 4 | fg);
}

/* Get a CHARACTER offset for given col, row position */
static inline int get_offset(int col, int row){
    return (row * SCREEN_WIDTH + col);
}

/* Get current CHARACTER offset of cursor */
static inline uint16_t get_cursor(){
    uint16_t offset = 0;

    /* get high byte of cursor offset */
    port_out_b(SCREEN_CTRL_REG, 14);
    offset = (port_in_b(SCREEN_DATA_REG) << 8);

    /* get low byte of cursor offset */
    port_out_b(SCREEN_CTRL_REG, 15);
    offset |= port_in_b(SCREEN_DATA_REG);

    return offset;
}

static inline uint16_t set_cursor(uint16_t offset){
    /* set high byte of cursor offset */
    port_out_b(SCREEN_CTRL_REG, 14);
    port_out_b(SCREEN_DATA_REG, (uint8_t)(offset >> 8));

    /* set low byte of cursor offset */
    port_out_b(SCREEN_CTRL_REG, 15);
    port_out_b(SCREEN_DATA_REG, (uint8_t)(offset));

    return offset;
}

/* Print a character at col, row */
/* Specify -1 for white on black at current cursor position */
void print_char(uint8_t character, int color, int col, int row){
    uint16_t *VGA_MEM = (uint16_t*)0xb8000;
    int offset = 0;

    /* Default to bright white on black if color is not specified */
    if(!color){
        color = get_color(BWHITE, BLACK);
    }

    /* Default to cursor position if negative position is specified 
     * TODO: maybe change this to allow specifying only row or column */
    if(col < 0 || row < 0){
        offset = get_cursor();
    }else{
        offset = get_offset(col, row);
        set_cursor(offset);
    }

    /* Write out character with specified bg and fg colors */
    /* and point to next character */
    *(VGA_MEM + offset++) = (color << 8) | character;
}

/* Clears the screen */
void clear_screen(){
    /*      fight the powa */
    for(int row = 0; row < SCREEN_HEIGHT; ++row){
        for(int col = 0; col < SCREEN_WIDTH; ++col){
            print_char(' ', 0, col, row);
        }
    }

    set_cursor(get_offset(0, 0));
}


void test_print(){
    clear_screen();
    print_char('D', get_color(RED, BLUE),40, 2);


}

