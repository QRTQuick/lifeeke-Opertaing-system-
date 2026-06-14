CC = gcc
AS = nasm
LD = ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I.
ASFLAGS = -f elf
LDFLAGS = -T linker.ld

BUILD_DIR = build

# Source files
C_SRCS = kernel.c gdt.c idt.c vga.c io.c fs.c string.c users.c keyboard.c shell.c
ASM_SRCS = boot.asm gdt.asm idt_handlers.asm

# Object files
C_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SRCS))
ASM_OBJS = $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASM_SRCS))

ALL_OBJS = $(C_OBJS) $(ASM_OBJS)

KERNEL_ELF = $(BUILD_DIR)/kernel.elf

.PHONY: all clean run iso

all: $(KERNEL_ELF)

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_ELF): $(ALL_OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(ALL_OBJS)

iso: $(KERNEL_ELF)
	mkdir -p iso/boot/grub
	cp $(KERNEL_ELF) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o os.iso iso

run: iso
	qemu-system-i386 -kernel $(KERNEL_ELF)

clean:
	rm -rf $(BUILD_DIR) iso os.iso
