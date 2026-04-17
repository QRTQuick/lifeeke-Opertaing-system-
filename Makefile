AS      = nasm
CC      = gcc
LD      = ld

CFLAGS  = -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra -O2 -c
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

all: os.iso

boot.o: boot.asm
	$(AS) -f elf32 boot.asm -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

kernel.elf: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) boot.o kernel.o -o kernel.elf

os.iso: kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso

check: kernel.elf
	grub-file --is-x86-multiboot kernel.elf && echo "Multiboot header OK"

clean:
	rm -rf *.o *.elf iso os.iso

.PHONY: all clean check
