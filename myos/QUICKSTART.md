# MyOS Quick Start Guide

## 3-Step Build Process

### Step 1: Install Dependencies (One Time)
```bash
sudo apt-get install nasm gcc grub-common xorriso mtools qemu-system-x86
```

### Step 2: Build the OS
```bash
cd /workspace/myos
./build.sh
```

### Step 3: Run in QEMU
```bash
./run.sh
```

## What You Get

A bootable graphical OS with:
- 🖥️ 1024x768 resolution, 32-bit color
- 🎨 Dark blue theme desktop
- 📱 Taskbar with start button
- 🪟 Sample application windows
- 🖱️ Mouse cursor
- 💻 Desktop icons

## Files Created

After building:
- `myos.iso` - Bootable ISO file (~5MB)
- Ready to run in QEMU or burn to USB

## Create Bootable USB

```bash
sudo dd if=myos.iso of=/dev/sdX bs=4M status=progress
```
⚠️ Replace `/dev/sdX` with your USB device!

## Full Documentation

- `README.md` - Project overview
- `INSTALL.md` - Detailed installation guide  
- `WORKFLOW.md` - Complete ISO creation workflow

## Need Help?

Run the complete build script:
```bash
./complete_build.sh
```

Or with test mode:
```bash
./complete_build.sh --test
```
