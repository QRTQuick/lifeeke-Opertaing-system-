#!/bin/bash

# ============================================================================
# MyOS Run Script
# Runs the OS in QEMU emulator
# ============================================================================

ISO_NAME="myos.iso"

echo "=========================================="
echo "MyOS - Running in QEMU"
echo "=========================================="
echo ""

if [ ! -f "$ISO_NAME" ]; then
    echo "Error: ISO file not found. Please run ./build.sh first."
    exit 1
fi

# Check if QEMU is installed
if ! command -v qemu-system-x86_64 &> /dev/null; then
    echo "Error: QEMU not found. Please install: sudo apt-get install qemu-system-x86"
    exit 1
fi

echo "Starting MyOS in QEMU..."
echo "Press Ctrl+Alt+G to release mouse capture"
echo "Press Ctrl+Alt+X to exit QEMU"
echo ""

# Run QEMU with appropriate settings
qemu-system-x86_64 \
    -cdrom "$ISO_NAME" \
    -boot d \
    -m 512M \
    -vga std \
    -enable-kvm 2>/dev/null || \
qemu-system-x86_64 \
    -cdrom "$ISO_NAME" \
    -boot d \
    -m 512M \
    -vga std
