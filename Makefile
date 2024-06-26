C_SOURCES = $(wildcard kernel/*.c drivers/*.c kernel/arch/x86/*.c lib/*.c) 
NASM_SOURCES = $(wildcard kernel/*.s kernel/arch/x86/*.s)
HEADERS = $(wildcard kernel/*.h include/*.h)
OBJ = ${C_SOURCES:.c=.o} ${NASM_SOURCES:.s=.o}

CCFLAGS = -m32 -std=c99 -Og -march=i386 \
	  -Wall -Wextra -Wpedantic \
	  -nostdlib -ffreestanding -nostdinc -nostartfiles -nodefaultlibs -lgcc\
	  -fno-pie -fno-stack-protector -fno-builtin -mno-sse \
	  -I include/ 

all: os-image

run: all
	qemu-system-i386 os-image

os-image: boot/bootloader.bin kernel.bin
	cat $^ > os-image

kernel.bin : kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 -e 0x1000 $^ --oformat binary

%.o : %.c ${HEADERS}
	gcc $(CCFLAGS) -c $< -o $@

%.o : %.s
	nasm $< -f elf32 -o $@

%.bin : %.s 
	nasm $< -f bin -o $@

kernel.o : kernel.c
	gcc $(CCFLAGS) -c $< -o $@

kernel_entry.o : kernel_entry.s
	nasm $< -f elf32 -o $@

bootloader.bin : bootloader.s
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.o *.dis *.map os-image
	rm -rf kernel/*.o boot/*.bin 
	rm -rf kernel/arch/x86/*.o 
