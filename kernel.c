#include <stddef.h>
#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "fs.h"
#include "users.h"
#include "shell.h"

void kernel_main(void) {
    init_gdt();
    init_idt();
    init_vga();
    init_fs();
    init_users();
    init_keyboard();

    terminal_writestring("Hello from lifeOS!\n");
    terminal_writestring("GDT and IDT initialized.\n");

    start_shell();

    // Enable interrupts
    asm volatile ("sti");

    for (;;) {
        asm volatile("hlt");
    }
}
