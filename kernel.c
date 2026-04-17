// kernel.c
#include <stddef.h>
#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

static volatile uint16_t* const vga_buffer = (uint16_t*)VGA_ADDRESS;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void clear_screen(uint8_t color) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t i = y * VGA_WIDTH + x;
            vga_buffer[i] = vga_entry(' ', color);
        }
    }
}

static void write_string_at(const char* s, uint8_t color, size_t row, size_t col) {
    size_t i = row * VGA_WIDTH + col;
    while (*s && i < (VGA_WIDTH * VGA_HEIGHT)) {
        vga_buffer[i++] = vga_entry(*s++, color);
    }
}

void kernel_main(void) {
    const uint8_t white_on_black = 0x0F;   // fg=white, bg=black

    clear_screen(white_on_black);
    write_string_at("Hello from my custom OS!", white_on_black, 12, 26);

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
