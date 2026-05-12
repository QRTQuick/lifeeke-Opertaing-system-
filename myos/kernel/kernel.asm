; ============================================================================
; MyOS Kernel - Graphical Operating System
; A simple x86 OS with GUI support using VESA graphics mode
; ============================================================================

[BITS 32]
[GLOBAL _start]
[EXTERN main]

; VESA Graphics Mode Constants
VESA_MODE equ 0x411B        ; 1024x768x32bpp
SCREEN_WIDTH equ 1024
SCREEN_HEIGHT equ 768
BYTES_PER_PIXEL equ 4

; Color Constants (ARGB format)
COLOR_BACKGROUND equ 0xFF1a1a2e      ; Dark blue background
COLOR_TASKBAR equ 0xFF16213e          ; Taskbar color
COLOR_WINDOW_BG equ 0xFF0f3460        ; Window background
COLOR_TEXT equ 0xFFffffff             ; White text
COLOR_HIGHLIGHT equ 0xFFe94560        ; Accent color

SECTION .text

_start:
    ; Set up stack
    mov esp, stack_top
    
    ; Check for VESA support and set graphics mode
    call setup_vesa_mode
    
    ; Clear screen with background color
    call clear_screen
    
    ; Draw the GUI
    call draw_desktop
    call draw_taskbar
    call draw_windows
    call draw_mouse_cursor
    
    ; Main loop - keep the system running
.halt_loop:
    hlt
    jmp .halt_loop

; ============================================================================
; Setup VESA Graphics Mode
; ============================================================================
setup_vesa_mode:
    pusha
    
    ; Set VESA mode
    mov ax, VESA_MODE
    int 0x10
    
    ; Get VESA information
    mov di, vesa_info
    mov ax, 0x4F00
    int 0x10
    
    popa
    ret

; ============================================================================
; Clear Screen with Background Color
; ============================================================================
clear_screen:
    pusha
    
    mov edi, [vesa_info + 16]     ; Get framebuffer address (LFB pointer)
    mov ecx, SCREEN_WIDTH * SCREEN_HEIGHT
    
.clear_loop:
    mov dword [edi], COLOR_BACKGROUND
    add edi, BYTES_PER_PIXEL
    dec ecx
    jnz .clear_loop
    
    popa
    ret

; ============================================================================
; Draw Desktop Environment
; ============================================================================
draw_desktop:
    pusha
    
    ; Draw some desktop icons (simplified as rectangles)
    mov ecx, 3                      ; Number of icons
    mov esi, icon_positions         ; Icon positions array
    
.draw_icon_loop:
    push ecx
    
    ; Get icon position
    mov eax, [esi]
    mov ebx, [esi + 4]
    
    ; Draw icon rectangle
    push eax
    push ebx
    push 64                         ; width
    push 64                         ; height
    push COLOR_WINDOW_BG
    call draw_rectangle
    add esp, 20
    
    ; Draw icon label area
    push eax
    push ebx
    push 64
    push 20
    push 0x80000000                 ; Semi-transparent
    call draw_rectangle
    add esp, 20
    
    add esi, 8                      ; Next icon position
    pop ecx
    loop .draw_icon_loop
    
    popa
    ret

; ============================================================================
; Draw Taskbar at Bottom
; ============================================================================
draw_taskbar:
    pusha
    
    ; Taskbar dimensions
    mov eax, 0                      ; x
    mov ebx, SCREEN_HEIGHT - 40     ; y
    push eax
    push ebx
    push SCREEN_WIDTH               ; width
    push 40                         ; height
    push COLOR_TASKBAR
    call draw_rectangle
    add esp, 20
    
    ; Draw start button
    mov eax, 10
    mov ebx, SCREEN_HEIGHT - 35
    push eax
    push ebx
    push 80                         ; width
    push 30                         ; height
    push COLOR_HIGHLIGHT
    call draw_rectangle
    add esp, 20
    
    ; Draw clock area
    mov eax, SCREEN_WIDTH - 100
    mov ebx, SCREEN_HEIGHT - 35
    push eax
    push ebx
    push 90                         ; width
    push 30                         ; height
    push 0xFF0f3460
    call draw_rectangle
    add esp, 20
    
    popa
    ret

; ============================================================================
; Draw Sample Windows
; ============================================================================
draw_windows:
    pusha
    
    ; Main window
    mov eax, 100
    mov ebx, 50
    push eax
    push ebx
    push 600                        ; width
    push 400                        ; height
    push COLOR_WINDOW_BG
    call draw_rectangle
    add esp, 20
    
    ; Window title bar
    mov eax, 100
    mov ebx, 50
    push eax
    push ebx
    push 600                        ; width
    push 30                         ; height
    push COLOR_HIGHLIGHT
    call draw_rectangle
    add esp, 20
    
    ; Second window (smaller)
    mov eax, 200
    mov ebx, 150
    push eax
    push ebx
    push 400                        ; width
    push 300                        ; height
    push 0xFF0f3460
    call draw_rectangle
    add esp, 20
    
    ; Second window title bar
    mov eax, 200
    mov ebx, 150
    push eax
    push ebx
    push 400                        ; width
    push 30                         ; height
    push 0xFFe94560
    call draw_rectangle
    add esp, 20
    
    popa
    ret

; ============================================================================
; Draw Mouse Cursor
; ============================================================================
draw_mouse_cursor:
    pusha
    
    ; Simple arrow cursor
    mov eax, SCREEN_WIDTH / 2
    mov ebx, SCREEN_HEIGHT / 2
    
    ; Draw cursor (white arrow shape - simplified as triangle)
    mov ecx, 20                     ; cursor height
    
.cursor_loop:
    push ecx
    mov edx, ecx                    ; width increases with each row
    mov esi, 0
    
.cursor_row_loop:
    cmp esi, edx
    jge .cursor_next_row
    
    ; Calculate pixel position
    push eax
    push ebx
    push ecx
    push edx
    push esi
    
    mov edi, [vesa_info + 16]       ; framebuffer
    mov ebp, ebx
    imul ebp, SCREEN_WIDTH
    add ebp, eax
    imul ebp, BYTES_PER_PIXEL
    add edi, ebp
    
    mov dword [edi], 0xFFFFFFFF     ; white cursor
    
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    
    inc eax
    inc esi
    jmp .cursor_row_loop
    
.cursor_next_row:
    inc ebx
    pop ecx
    loop .cursor_loop
    
    popa
    ret

; ============================================================================
; Draw Rectangle Helper Function
; Parameters: x, y, width, height, color (all on stack)
; ============================================================================
draw_rectangle:
    pusha
    
    mov ebp, esp
    mov eax, [ebp + 44]             ; x
    mov ebx, [ebp + 40]             ; y
    mov ecx, [ebp + 36]             ; width
    mov edx, [ebp + 32]             ; height
    mov esi, [ebp + 28]             ; color
    
    mov edi, [vesa_info + 16]       ; framebuffer address
    
.outer_loop:
    push edx
    mov esi_temp, ecx
    
.inner_loop:
    push esi_temp
    
    ; Calculate pixel offset
    mov ebp, ebx
    imul ebp, SCREEN_WIDTH
    add ebp, eax
    imul ebp, BYTES_PER_PIXEL
    
    mov [edi + ebp], esi            ; set pixel color
    
    inc eax
    pop esi_temp
    dec esi_temp
    jnz .inner_loop
    
    ; Move to next row
    mov eax, [ebp + 44]             ; reset x
    inc ebx
    pop edx
    dec edx
    jnz .outer_loop
    
    popa
    ret

; ============================================================================
; Data Section
; ============================================================================
SECTION .data

vesa_info:
    times 512 db 0                  ; VESA information buffer

icon_positions:
    dd 50, 100                      ; Icon 1: (50, 100)
    dd 50, 200                      ; Icon 2: (50, 200)
    dd 50, 300                      ; Icon 3: (50, 300)

esi_temp: resd 1                    ; Temporary storage for rectangle drawing

; ============================================================================
; Stack Section
; ============================================================================
SECTION .bss

stack:
    resb 8192                       ; 8KB stack
stack_top:
