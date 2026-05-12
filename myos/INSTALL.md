# MyOS Installation & Usage Guide

## Quick Start

### 1. Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install nasm gcc grub-common xorriso mtools qemu-system-x86
```

**Fedora/RHEL:**
```bash
sudo dnf install nasm gcc grub2-tools xorriso mtools qemu-system-x86
```

**Arch Linux:**
```bash
sudo pacman -S nasm gcc grub xorriso mtools qemu
```

**macOS (with Homebrew):**
```bash
brew install nasm gcc grub mtools qemu
```

### 2. Build the OS

```bash
cd myos
chmod +x build.sh run.sh
./build.sh
```

### 3. Run in QEMU

```bash
./run.sh
```

Or manually:
```bash
qemu-system-x86_64 -cdrom myos.iso -boot d -m 512M -vga std
```

## Creating a Bootable USB

**⚠️ WARNING: This will erase all data on the USB drive!**

1. Insert your USB drive
2. Identify the device name:
   ```bash
   lsblk
   # or
   sudo fdisk -l
   ```
3. Create bootable USB (replace `/dev/sdX` with your device):
   ```bash
   sudo dd if=myos.iso of=/dev/sdX bs=4M status=progress
   sudo sync
   ```

## ISO Creation Workflow

The build process follows these steps:

```
┌─────────────────────────────────────────────────────────────┐
│                    MyOS Build Workflow                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  1. Source Code                                              │
│     └── kernel/kernel.asm                                    │
│         ↓ (NASM Assembler)                                   │
│                                                              │
│  2. Object File                                              │
│     └── kernel.o                                             │
│         ↓ (LD Linker)                                        │
│                                                              │
│  3. Kernel Binary                                            │
│     └── boot/kernel.bin                                      │
│         ↓ (Copy to ISO directory)                            │
│                                                              │
│  4. Boot Structure                                           │
│     isodir/                                                  │
│     ├── boot/                                                │
│     │   ├── kernel.bin                                       │
│     │   └── grub/                                            │
│     │       └── grub.cfg                                     │
│         ↓ (GRUB mkrescue)                                    │
│                                                              │
│  5. Final ISO                                                │
│     └── myos.iso                                             │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

## GUI Features

MyOS includes a graphical desktop environment with:

- **Desktop Background**: Dark blue gradient (#1a1a2e)
- **Taskbar**: Bottom bar with start button and clock area
- **Windows**: Sample application windows with title bars
- **Desktop Icons**: Three sample application icons
- **Mouse Cursor**: Custom arrow cursor
- **Color Scheme**: Modern dark theme with accent colors

### Color Palette

| Element | Color Code | Description |
|---------|-----------|-------------|
| Background | #1a1a2e | Dark blue |
| Taskbar | #16213e | Navy blue |
| Windows | #0f3460 | Ocean blue |
| Accent | #e94560 | Red accent |
| Text | #ffffff | White |

## Resolution Settings

Default resolution: **1024x768 @ 32-bit color**

To change resolution, edit `kernel/kernel.asm`:

```asm
; Change these values
SCREEN_WIDTH equ 1024
SCREEN_HEIGHT equ 768
VESA_MODE equ 0x411B    ; See VESA mode list below
```

### Common VESA Modes

| Mode | Resolution | Colors |
|------|-----------|--------|
| 0x111 | 640x480 | 32-bit |
| 0x114 | 800x600 | 32-bit |
| 0x117 | 1024x768 | 32-bit |
| 0x11A | 1280x1024 | 32-bit |
| 0x11E | 1920x1080 | 32-bit |

## Troubleshooting

### Build Fails - Missing Dependencies
```bash
# Install all required packages
sudo apt-get install nasm gcc grub-common xorriso mtools
```

### QEMU Shows Black Screen
- Ensure VESA graphics are supported in your VM
- Try different VESA modes in kernel.asm
- Increase VM memory: `-m 1024M`

### ISO Won't Boot on Real Hardware
- Use UEFI-compatible GRUB configuration
- Check BIOS/UEFI boot order
- Try different USB ports

### Graphics Issues
- Update QEMU: `sudo apt-get update && sudo apt-get upgrade qemu-system-x86`
- Use `-vga std` or `-vga vmware` for better compatibility
- Check VESA mode compatibility

## Advanced Customization

### Adding Applications

1. Create new assembly functions in `kernel/kernel.asm`
2. Add drawing routines for new UI elements
3. Implement keyboard/mouse input handling
4. Rebuild with `./build.sh`

### Changing Boot Menu

Edit `grub/grub.cfg`:
```
set timeout=10
set default=0

menuentry "MyOS - Normal" {
    multiboot /boot/kernel.bin
    boot
}

menuentry "MyOS - Debug Mode" {
    multiboot /boot/kernel.bin debug
    boot
}
```

### Adding Sound Support

Sound requires additional drivers and is beyond this basic implementation. Consider:
- PC Speaker driver for simple beeps
- AC97 or Intel HDA drivers for advanced audio

## Development Tips

1. **Use QEMU for testing** - Fast iteration without rebooting
2. **Enable logging** - Add debug output to track execution
3. **Version control** - Use git to track changes
4. **Document changes** - Keep comments in assembly code
5. **Test frequently** - Build and test after each major change

## Resources

- [OSDev Wiki](https://wiki.osdev.org/)
- [NASM Documentation](https://www.nasm.us/doc/)
- [GRUB Manual](https://www.gnu.org/software/grub/manual/grub/)
- [VESA Standards](https://www.vesa.org/)

## License

MIT License - Free to use, modify, and distribute.
