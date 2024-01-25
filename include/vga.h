#ifndef VGA_H_
#define VGA_H_
#include <stdint.h>

/* Print out a string at current cursor position */
void print(char*);

/* Print out a string at given cursor position */
void print_at(char*, int col, int row);

/* Print out given character at given offset */
void print_cell(uint8_t character, uint8_t color, int offset);

/* Print char at given col, row */
void print_char(uint8_t character, int color, int col, int row);

void clear_screen();
#endif /* VGA_H_ */
