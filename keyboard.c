#include "keyboard.h"
#include "io.h"
#include "vga.h"

#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64

// Scancode to ASCII mapping for a US keyboard layout
// Only handles basic characters for now
static unsigned char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
    '\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,   0,   0, ' ',
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

static char keyboard_buffer[256];
static int buffer_head = 0;
static int buffer_tail = 0;

void keyboard_handler(struct regs *r) {
    uint8_t scancode;

    // Read from the keyboard's data port
    scancode = inb(KBD_DATA_PORT);

    // If the top bit of the byte we read is set, that means that a key has just been released
    if (scancode & 0x80) {
        // Key release, ignore for now
    } else {
        // Key press
        char c = kbd_us[scancode];
        if (c != 0) {
            keyboard_buffer[buffer_head] = c;
            buffer_head = (buffer_head + 1) % sizeof(keyboard_buffer);
        }
    }

    // Send EOI to PIC
    outb(0x20, 0x20);
}

void init_keyboard() {
    // Register our keyboard handler with the IDT
    // This will be done in idt.c by setting up IRQ1
    terminal_writestring("Keyboard initialized.\n");
}

char get_char() {
    while (buffer_head == buffer_tail) {
        // Wait for a character to be available
        asm volatile ("hlt");
    }

    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % sizeof(keyboard_buffer);
    return c;
}
