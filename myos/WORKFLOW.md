# MyOS Workflow Documentation

## Complete ISO Creation Workflow

This document details the step-by-step process of creating a bootable graphical OS ISO.

## Phase 1: Development Environment Setup

### 1.1 Install Required Tools

```bash
# Ubuntu/Debian
sudo apt-get install nasm gcc grub-common xorriso mtools qemu-system-x86

# Verify installations
nasm --version
gcc --version
grub-mkrescue --version
xorriso --version
qemu-system-x86_64 --version
```

### 1.2 Project Structure

```
myos/
├── kernel/
│   └── kernel.asm          # Main OS kernel with GUI
├── grub/
│   └── grub.cfg            # Bootloader configuration
├── boot/                    # Compiled kernel output
├── fonts/                   # Font data (future use)
├── isodir/                  # Temporary ISO build directory
│   └── boot/
│       ├── kernel.bin
│       └── grub/
│           └── grub.cfg
├── build.sh                 # Main build script
├── run.sh                   # QEMU test runner
├── complete_build.sh        # Full build + summary
├── README.md                # Project overview
└── INSTALL.md               # Installation guide
```

## Phase 2: Kernel Compilation Workflow

### Step 2.1: Assembly (NASM)

**Input:** `kernel/kernel.asm`
**Tool:** NASM (Netwide Assembler)
**Output:** `kernel.o` (ELF object file)

```bash
nasm -f elf32 kernel/kernel.asm -o kernel.o
```

**What happens:**
- Parses assembly code
- Converts instructions to machine code
- Creates symbol table
- Generates relocatable object file

### Step 2.2: Linking (LD)

**Input:** `kernel.o`
**Tool:** GNU LD (Linker)
**Output:** `boot/kernel.bin` (Binary kernel)

```bash
ld -m elf_i386 -Ttext 0x100000 kernel.o -o boot/kernel.bin --oformat binary
```

**What happens:**
- Resolves external symbols
- Sets load address (0x100000 = 1MB)
- Combines sections (.text, .data, .bss)
- Outputs raw binary (no ELF headers)

## Phase 3: Boot Media Preparation

### Step 3.1: Create ISO Directory Structure

```bash
rm -rf isodir
mkdir -p isodir/boot/grub
```

### Step 3.2: Copy Files

```bash
cp boot/kernel.bin isodir/boot/
cp grub/grub.cfg isodir/boot/grub/
```

**Result:**
```
isodir/
└── boot/
    ├── kernel.bin      # The OS kernel
    └── grub/
        └── grub.cfg    # Boot menu config
```

### Step 3.3: GRUB Configuration

The `grub.cfg` defines boot entries:

```cfg
set timeout=5
set default=0

menuentry "MyOS - Graphical Operating System" {
    multiboot /boot/kernel.bin
    boot
}

menuentry "MyOS (Safe Mode)" {
    multiboot /boot/kernel.bin safe
    boot
}
```

## Phase 4: ISO Creation

### Step 4.1: Generate Bootable ISO

**Tool:** GRUB mkrescue + xorriso

```bash
grub-mkrescue -o myos.iso isodir
```

**What happens internally:**

1. **Creates El Torito boot image**
   - Makes bootable CD/DVD structure
   - Adds GRUB stage 1.5 and 2

2. **Adds filesystem**
   - Creates ISO 9660 filesystem
   - Includes all files from isodir/

3. **Sets boot catalog**
   - Defines boot priority
   - Configures emulation mode

4. **Finalizes ISO**
   - Adds volume descriptors
   - Calculates checksums
   - Writes final ISO file

### Step 4.2: ISO Structure

The resulting `myos.iso` contains:

```
ISO 9660 Filesystem
├── [Boot Sector]          # Master Boot Record
├── [El Torito Boot]       # GRUB boot image
└── /boot/
    ├── kernel.bin         # OS Kernel (multiboot compliant)
    └── grub/
        └── grub.cfg       # Boot configuration
        ├── i386-pc/       # GRUB modules
        │   └── *.mod
        └── fonts/         # GRUB fonts
            └── unicode.pf2
```

## Phase 5: Testing & Validation

### Step 5.1: QEMU Emulation

```bash
qemu-system-x86_64 -cdrom myos.iso -boot d -m 512M -vga std
```

**Parameters explained:**
- `-cdrom myos.iso` - Use ISO as CD-ROM
- `-boot d` - Boot from CD-ROM drive
- `-m 512M` - Allocate 512MB RAM
- `-vga std` - Use standard VGA graphics

### Step 5.2: Boot Sequence

When the OS boots:

1. **BIOS/UEFI Initialization**
   - Hardware detection
   - Memory mapping

2. **GRUB Bootloader**
   - Loads stage 1 from MBR
   - Loads stage 2 from ISO
   - Displays boot menu
   - Loads kernel into memory

3. **Kernel Execution**
   - Entry point: `_start`
   - Sets up stack
   - Initializes VESA graphics mode
   - Draws GUI elements
   - Enters main loop

### Step 5.3: Graphics Initialization

The kernel initializes graphics:

```asm
; Set VESA mode 0x411B (1024x768x32bpp)
mov ax, 0x411B
int 0x10

; Get framebuffer address
mov di, vesa_info
mov ax, 0x4F00
int 0x10

; Framebuffer is now at [vesa_info + 16]
```

### Step 5.4: GUI Rendering

The kernel draws:

1. **Clear screen** - Fill with background color
2. **Draw desktop icons** - Three sample applications
3. **Draw taskbar** - Bottom bar with start button
4. **Draw windows** - Sample application windows
5. **Draw cursor** - Mouse pointer

## Phase 6: Deployment Options

### Option A: USB Drive (Live USB)

```bash
# Identify USB device (CAREFUL: replaces X with correct letter!)
lsblk

# Write ISO to USB
sudo dd if=myos.iso of=/dev/sdX bs=4M status=progress

# Sync and eject
sudo sync
sudo eject /dev/sdX
```

### Option B: Virtual Machine

**VirtualBox:**
1. Create new VM (Other/Unknown, 512MB RAM)
2. Add optical drive with myos.iso
3. Set boot order: Optical first
4. Start VM

**VMware:**
1. Create new VM (Guest OS: Other)
2. Configure CD/DVD with myos.iso
3. Set BIOS to boot from CD-ROM
4. Power on

### Option C: Network Boot (PXE)

Requires additional setup:
1. Extract kernel from ISO
2. Configure TFTP server
3. Setup PXE boot menu
4. Network boot client machines

## Workflow Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                     COMPLETE BUILD WORKFLOW                      │
└─────────────────────────────────────────────────────────────────┘

[Source Code]
     │
     ├─ kernel/kernel.asm ─────────────────┐
     │                                      │
     └─ grub/grub.cfg ─────────────────────┤
                                            │
                                            ▼
┌───────────────────────────────────────────────────────────────┐
│ PHASE 1: COMPILATION                                          │
├───────────────────────────────────────────────────────────────┤
│                                                               │
│  NASM Assembler                                               │
│  kernel.asm ──────► kernel.o                                  │
│                                                               │
│  GNU Linker                                                   │
│  kernel.o ──────► boot/kernel.bin                             │
│                                                               │
└───────────────────────────────────────────────────────────────┘
                                            │
                                            ▼
┌───────────────────────────────────────────────────────────────┐
│ PHASE 2: BOOT MEDIA                                           │
├───────────────────────────────────────────────────────────────┤
│                                                               │
│  Create Directory Structure                                   │
│  isodir/boot/                                                 │
│  isodir/boot/grub/                                            │
│                                                               │
│  Copy Files                                                   │
│  boot/kernel.bin ──► isodir/boot/                             │
│  grub/grub.cfg ────► isodir/boot/grub/                        │
│                                                               │
└───────────────────────────────────────────────────────────────┘
                                            │
                                            ▼
┌───────────────────────────────────────────────────────────────┐
│ PHASE 3: ISO CREATION                                         │
├───────────────────────────────────────────────────────────────┤
│                                                               │
│  GRUB mkrescue + xorriso                                      │
│  isodir/ ──────► myos.iso                                     │
│                                                               │
│  - Creates El Torito boot image                               │
│  - Adds ISO 9660 filesystem                                   │
│  - Embeds GRUB bootloader                                     │
│                                                               │
└───────────────────────────────────────────────────────────────┘
                                            │
                                            ▼
┌───────────────────────────────────────────────────────────────┐
│ PHASE 4: TESTING                                              │
├───────────────────────────────────────────────────────────────┤
│                                                               │
│  QEMU Emulation                                               │
│  qemu-system-x86_64 -cdrom myos.iso                           │
│                                                               │
│  Boot Sequence:                                               │
│  1. BIOS init                                                 │
│  2. GRUB loads                                                │
│  3. Kernel executes                                           │
│  4. VESA graphics init                                        │
│  5. GUI renders                                               │
│                                                               │
└───────────────────────────────────────────────────────────────┘
                                            │
                                            ▼
┌───────────────────────────────────────────────────────────────┐
│ PHASE 5: DEPLOYMENT                                           │
├───────────────────────────────────────────────────────────────┤
│                                                               │
│  Choose deployment method:                                    │
│                                                               │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐           │
│  │  Live USB   │  │   VM Disk   │  │  Network    │           │
│  │  (dd)       │  │  (VBox/     │  │  Boot (PXE) │           │
│  │             │  │   VMware)   │  │             │           │
│  └─────────────┘  └─────────────┘  └─────────────┘           │
│                                                               │
└───────────────────────────────────────────────────────────────┘
```

## Build Time Estimates

| Phase | Operation | Typical Time |
|-------|-----------|--------------|
| 1 | NASM Assembly | < 1 second |
| 1 | LD Linking | < 1 second |
| 2 | File Copy | < 1 second |
| 3 | ISO Creation | 2-5 seconds |
| 4 | QEMU Boot | 3-10 seconds |
| **Total** | **Complete Build** | **~10 seconds** |

## Common Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| `nasm: command not found` | NASM not installed | `sudo apt-get install nasm` |
| `grub-mkrescue: command not found` | GRUB tools missing | `sudo apt-get install grub-common xorriso` |
| Black screen in QEMU | Wrong VESA mode | Try different mode in kernel.asm |
| ISO too large | Too many GRUB modules | Use minimal GRUB configuration |
| Boot fails on real hardware | UEFI/BIOS mismatch | Check firmware settings |

## Next Steps

After mastering this workflow, you can:

1. **Add more GUI elements** - Buttons, menus, dialogs
2. **Implement input handling** - Keyboard and mouse drivers
3. **Add filesystem support** - Read/write files
4. **Create applications** - Text editor, calculator, etc.
5. **Add networking** - TCP/IP stack
6. **Support multiple architectures** - x86_64, ARM

## Resources

- [OSDev.org](https://wiki.osdev.org/) - Operating system development wiki
- [GRUB Manual](https://www.gnu.org/software/grub/manual/grub/) - Official GRUB documentation
- [NASM Docs](https://www.nasm.us/doc/) - NASM assembler reference
- [Intel Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html) - CPU architecture reference
