#!/bin/bash

# ============================================================================
# MyOS Complete Build & Test Script
# This script builds the OS and optionally tests it
# ============================================================================

set -e

echo "=========================================="
echo "MyOS - Complete Build System"
echo "=========================================="
echo ""

# Check if we should run tests
RUN_TEST=false
if [ "$1" == "--test" ] || [ "$1" == "-t" ]; then
    RUN_TEST=true
fi

# Step 1: Build the OS
echo "Building MyOS..."
./build.sh

echo ""
echo "=========================================="
echo "Build Summary"
echo "=========================================="
echo ""
echo "Files created:"
ls -lh myos.iso 2>/dev/null || echo "  ISO not created (check build output)"
echo ""
echo "Project structure:"
echo "  myos/"
echo "  ├── kernel/kernel.asm    - Main kernel with GUI code"
echo "  ├── grub/grub.cfg        - Bootloader configuration"
echo "  ├── build.sh             - Build script"
echo "  ├── run.sh               - QEMU runner"
echo "  ├── README.md            - Project overview"
echo "  └── INSTALL.md           - Installation guide"
echo ""
echo "GUI Features:"
echo "  ✓ VESA graphics mode (1024x768, 32-bit color)"
echo "  ✓ Desktop background (dark blue theme)"
echo "  ✓ Taskbar with start button"
echo "  ✓ Sample application windows"
echo "  ✓ Desktop icons"
echo "  ✓ Mouse cursor"
echo ""
echo "ISO Creation Workflow:"
echo "  1. NASM assembles kernel.asm → kernel.o"
echo "  2. LD links kernel.o → kernel.bin"
echo "  3. GRUB directory structure created"
echo "  4. Kernel and config copied to isodir/"
echo "  5. xorriso creates bootable ISO"
echo ""

if [ "$RUN_TEST" = true ]; then
    echo "Starting QEMU test in 3 seconds..."
    sleep 3
    ./run.sh
else
    echo "To test the OS:"
    echo "  ./run.sh"
    echo ""
    echo "Or run with test flag:"
    echo "  $0 --test"
    echo ""
    echo "To create bootable USB:"
    echo "  sudo dd if=myos.iso of=/dev/sdX bs=4M status=progress"
    echo ""
fi

echo "Done!"
