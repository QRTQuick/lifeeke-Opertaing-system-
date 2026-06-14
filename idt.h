#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// A struct to describe an interrupt gate. 
struct idt_entry {
    uint16_t base_low;    // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;         // Kernel segment selector.
    uint8_t  always0;     // This must always be zero.
    uint8_t  flags;       // More flags. See the IDT page.
    uint16_t base_high;   // The upper 16 bits of the address to jump to.
} __attribute__((packed));

// A struct to describe the IDT register. This is how our IDT is loaded by the processor.
struct idt_ptr {
    uint16_t limit;       // The upper 16 bits of all selector limits.
    uint32_t base;        // The address of the first idt_entry structure.
} __attribute__((packed));

// These are function prototypes for the assembly interrupt handlers.
// Each is defined in idt_handlers.asm
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQ handlers
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// A common handler for all interrupts.
void isr_handler(struct regs *r);

// A common handler for all IRQs.
void irq_handler(struct regs *r);

// Function to initialize the IDT.
void init_idt();

// Assembly function to load the IDT.
extern void idt_flush();

#endif
