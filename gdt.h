#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// Each segment descriptor is 8 bytes
struct gdt_entry {
    uint16_t limit_low;     // The lower 16 bits of the limit.
    uint16_t base_low;      // The lower 16 bits of the base.
    uint8_t  base_middle;   // The next 8 bits of the base.
    uint8_t  access;        // Access flags, determine what ring this segment can be used in.
    uint8_t  granularity;   // Granularity, and higher 4 bits of the limit (total 20 bits).
    uint8_t  base_high;     // The last 8 bits of the base.
} __attribute__((packed));

// Pointer to the GDT, used by the LGDT instruction
struct gdt_ptr {
    uint16_t limit;         // The upper 16 bits of all selector limits.
    uint32_t base;          // The address of the first gdt_entry structure.
} __attribute__((packed));

// Function to initialize the GDT
void init_gdt();

// Assembly function to load the GDT
extern void gdt_flush();

#endif
