/* Some (very) basic 80x25 text mode control */
#include <vga.h>
#include <arch/x86/io.h>
#include <stdint.h>
#include <libc.h>

#define SCREEN_WIDTH   80
#define SCREEN_HEIGHT  25

/* I/O registers */
#define SCREEN_CTRL_REG 0x3d4
#define SCREEN_DATA_REG 0x3d5

/* Just a fun little test */
struct color_string{
    char *str;
    uint8_t color;
};

enum vga_color{
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
}vga_color;


/* Get value for a given foreground/background pair */
static inline uint8_t get_color(enum vga_color fg, enum vga_color bg){
    return (bg << 4 | fg);
}

/* Get a CHARACTER offset for given col, row position */
static inline int get_offset(int col, int row){
    return (row * SCREEN_WIDTH + col);
}

/* Probably not the best implementation ever but at least it works */
static inline int scroll(int offset){
    uint16_t *VGA_MEM = (uint16_t*)0xb8000;
    int bufflen = SCREEN_WIDTH * SCREEN_HEIGHT;
    int lastrow = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH;

    if(offset > bufflen - 1){
        for(int i = 0; i < bufflen; ++i){ 
            VGA_MEM[i] = VGA_MEM[i + SCREEN_WIDTH];
        }

        /* Write out spaces for last row */
        for(int i = lastrow; i < bufflen; ++i){
            VGA_MEM[i] =  0x20;
        }
        return lastrow;
    }
    return offset;
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

/* Print out something at a given offset 
 * absolutely no checking on anything */
void print_cell(uint8_t character, uint8_t color, int offset){
    uint16_t *VGA_MEM = (uint16_t*)0xb8000;
    *(VGA_MEM + offset) = (color << 8) | character;
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


    /* TODO: not really happy with this at the moment, should be changed to a switch */
    /* Skip to the end of the current row if we hit a newline
     * this will automatically advance us to the next one on offset increment */
    if(character == '\n'){
        int cur_row = offset / SCREEN_WIDTH;
        offset = get_offset(79, cur_row);
    }else if(character == '\b'){
        offset = get_cursor();
        if(offset > 1){
            offset -= 2;
        }
    }else{
        /* Write out character with specified bg and fg colors 
         * This could maybe be replaced with a call to print_cell()
         * but there's not much point wasting 
         * overhead on replacing a line with a line */
        *(VGA_MEM + offset) = (color << 8) | character;
    }
    offset++;

    set_cursor(scroll(offset));
}


void print_at(char *s, int col, int row){
    /* TODO: this is janky, I don't like repeating 
     * the same check as in print_char() here
     * but this is a lazy initial version to get a print working
     * so it'll just have to do for now */
    if(col >= 0 && row >= 0){
        set_cursor(get_offset(col, row));
    }

    for(; *s; s++){
        print_char(*s, 0, -1, -1);
    }
}


/* Prints a string at current cursor position */
void print(char *s){
    print_at(s, -1, -1);
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


/* Prints a colored string at cursor position */
void print_color(struct color_string *s){
    for(int i = 0; s->str[i]; ++i){
        print_char(s->str[i], s->color, -1, -1);
    }
}

