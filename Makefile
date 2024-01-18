C_SOURCES = $(wildcard kernel/*.c)
HEADERS = $(wildcard kernel/*.h include/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: os-image

run: all
	qemu-system-i386 os-image

os-image: boot/bootloader.bin kernel.bin
	cat $^ > os-image

kernel.bin : kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 -e 0x1000 $^ --oformat binary

%.o : %.c ${HEADERS}
	gcc -Wall -Wextra -std=c90 -ffreestanding -fno-pie -m32 -c $< -o $@

%.o : %.s
	nasm $< -f elf32 -o $@

%.bin : %.s 
	nasm $< -f bin -o $@

kernel.o : kernel.c
	gcc -Wall -Wextra -std=c90 -ffreestanding -fno-pie -m32 -c $< -o $@

kernel_entry.o : kernel_entry.s
	nasm $< -f elf32 -o $@

bootloader.bin : bootloader.s
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.o *.dis *.map os-image
	rm -rf kernel/*.o boot/*.bin 
