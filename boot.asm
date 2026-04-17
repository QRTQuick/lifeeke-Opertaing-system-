; boot.asm
; 32-bit Multiboot entry for GRUB (NASM syntax)

[bits 32]

; Multiboot v1 header constants
MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ (1 << 0) | (1 << 1)   ; page align modules + request memory map
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384                         ; 16 KiB stack
stack_top:

section .text
global start
extern kernel_main

start:
    ; GRUB already put us in 32-bit protected mode.
    cli                                ; interrupts off (we have no IDT yet)
    mov esp, stack_top                 ; initialize stack pointer
    cld                                ; C code expects DF=0
    call kernel_main                   ; jump into C kernel

.hang:
    hlt                                ; stop CPU until next external event
    jmp .hang
