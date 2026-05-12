# MyOS - A Simple Graphical Operating System

This is a basic x86 operating system with a graphical user interface (GUI) that boots using GRUB and displays a desktop environment.

## Project Structure

```
myos/
├── boot/           # Bootloader files
├── kernel/         # Kernel source code
├── grub/           # GRUB configuration
├── fonts/          # Font data for GUI
└── build.sh        # Build script to create ISO
```

## Features

- VESA graphics mode support
- Basic GUI with desktop background
- Window management system
- Mouse cursor rendering
- Simple application launcher

## Building the OS

### Prerequisites

You'll need the following tools installed:

```bash
# On Ubuntu/Debian
sudo apt-get install nasm gcc grub-common xorriso mtools qemu-system-x86

# On Fedora/RHEL
sudo dnf install nasm gcc grub2-tools xorriso mtools qemu-system-x86

# On macOS (with Homebrew)
brew install nasm gcc grub mtools qemu
```

### Build Process

1. **Make the build script executable:**
   ```bash
   chmod +x build.sh
   ```

2. **Build the OS:**
   ```bash
   ./build.sh
   ```

3. **Test with QEMU:**
   ```bash
   ./run.sh
   ```

4. **Create bootable USB (optional):**
   ```bash
   sudo dd if=myos.iso of=/dev/sdX bs=4M status=progress
   ```
   Replace `/dev/sdX` with your USB device (e.g., `/dev/sdb`).

## Workflow for ISO Creation

The build process follows these steps:

1. **Compile Assembly Code** → NASM assembles the kernel
2. **Link Objects** → GCC links all object files
3. **Create Boot Directory** → Set up GRUB structure
4. **Copy Kernel** → Place kernel in boot directory
5. **Generate GRUB Config** → Create boot menu
6. **Create ISO** → Use xorriso to make bootable ISO
7. **Test** → Run in QEMU emulator

## Running the OS

After building, you can run the OS in QEMU:

```bash
qemu-system-x86_64 -cdrom myos.iso -boot d -m 512M -vga std
```

Or use the provided `run.sh` script.

## Customization

- Modify `kernel/kernel.asm` to change GUI colors and layout
- Edit `grub/grub.cfg` to change boot menu
- Adjust resolution in kernel code (default: 1024x768)

## License

MIT License - Feel free to modify and distribute.
