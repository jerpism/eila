#ifndef VGA_H_
#define VGA_H_
#include <stdint.h>

/* TODO: remove this at some point when no longer necessary */
void test_print();

void print(char*);
void print_char(uint8_t character, int color, int col, int row);

void clear_screen();
#endif /* VGA_H_ */
