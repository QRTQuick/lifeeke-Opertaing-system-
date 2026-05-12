#!/bin/bash

# ============================================================================
# MyOS Build Script
# Builds a bootable ISO with graphical OS
# ============================================================================

set -e  # Exit on error

echo "=========================================="
echo "MyOS - Building Graphical Operating System"
echo "=========================================="
echo ""

# Configuration
KERNEL_SRC="kernel/kernel.asm"
KERNEL_BIN="boot/kernel.bin"
GRUB_CFG="grub/grub.cfg"
ISO_NAME="myos.iso"
BOOT_DIR="boot"
ISO_DIR="isodir"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print status messages
print_status() {
    echo -e "${GREEN}[✓]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

print_error() {
    echo -e "${RED}[✗]${NC} $1"
}

# Check for required tools
check_dependencies() {
    print_status "Checking dependencies..."
    
    local missing_deps=0
    
    if ! command -v nasm &> /dev/null; then
        print_error "NASM not found. Please install: sudo apt-get install nasm"
        missing_deps=1
    fi
    
    if ! command -v grub-mkrescue &> /dev/null && ! command -v grub2-mkrescue &> /dev/null; then
        print_error "GRUB rescue tool not found. Please install: sudo apt-get install grub-common xorriso"
        missing_deps=1
    fi
    
    if ! command -v xorriso &> /dev/null; then
        print_error "xorriso not found. Please install: sudo apt-get install xorriso"
        missing_deps=1
    fi
    
    if [ $missing_deps -eq 1 ]; then
        exit 1
    fi
    
    print_status "All dependencies found!"
}

# Step 1: Assemble the kernel
build_kernel() {
    print_status "Step 1: Assembling kernel..."
    
    if [ ! -f "$KERNEL_SRC" ]; then
        print_error "Kernel source not found: $KERNEL_SRC"
        exit 1
    fi
    
    # Assemble with NASM
    nasm -f elf32 "$KERNEL_SRC" -o "${KERNEL_SRC%.asm}.o"
    
    print_status "Kernel assembled successfully!"
}

# Step 2: Link the kernel
link_kernel() {
    print_status "Step 2: Linking kernel..."
    
    # Create a simple linker script or use default
    ld -m elf_i386 -Ttext 0x100000 "${KERNEL_SRC%.asm}.o" -o "$KERNEL_BIN" --oformat binary
    
    print_status "Kernel linked successfully!"
}

# Step 3: Prepare boot directory
prepare_boot() {
    print_status "Step 3: Preparing boot directory..."
    
    # Clean and create boot directory structure
    rm -rf "$ISO_DIR"
    mkdir -p "$ISO_DIR/boot/grub"
    
    # Copy kernel to boot directory
    cp "$KERNEL_BIN" "$ISO_DIR/boot/"
    
    # Copy GRUB configuration
    cp "$GRUB_CFG" "$ISO_DIR/boot/grub/"
    
    print_status "Boot directory prepared!"
}

# Step 4: Create ISO
create_iso() {
    print_status "Step 4: Creating bootable ISO..."
    
    # Try grub-mkrescue first, fall back to grub2-mkrescue
    if command -v grub-mkrescue &> /dev/null; then
        grub-mkrescue -o "$ISO_NAME" "$ISO_DIR" 2>/dev/null || \
        grub2-mkrescue -o "$ISO_NAME" "$ISO_DIR"
    else
        grub2-mkrescue -o "$ISO_NAME" "$ISO_DIR"
    fi
    
    if [ -f "$ISO_NAME" ]; then
        print_status "ISO created successfully: $ISO_NAME"
        ls -lh "$ISO_NAME"
    else
        print_error "Failed to create ISO"
        exit 1
    fi
}

# Step 5: Cleanup
cleanup() {
    print_status "Cleaning up temporary files..."
    
    rm -f "${KERNEL_SRC%.asm}.o"
    rm -f "$KERNEL_BIN"
    
    print_status "Cleanup complete!"
}

# Main build process
main() {
    echo ""
    echo "Build Configuration:"
    echo "  Kernel Source: $KERNEL_SRC"
    echo "  Output ISO: $ISO_NAME"
    echo "  Resolution: 1024x768"
    echo "  Graphics Mode: VESA (32-bit color)"
    echo ""
    
    check_dependencies
    build_kernel
    link_kernel
    prepare_boot
    create_iso
    cleanup
    
    echo ""
    echo "=========================================="
    echo -e "${GREEN}Build Complete!${NC}"
    echo "=========================================="
    echo ""
    echo "To run the OS in QEMU:"
    echo "  qemu-system-x86_64 -cdrom $ISO_NAME -boot d -m 512M -vga std"
    echo ""
    echo "Or use the run.sh script:"
    echo "  ./run.sh"
    echo ""
    echo "To create a bootable USB:"
    echo "  sudo dd if=$ISO_NAME of=/dev/sdX bs=4M status=progress"
    echo "  (Replace /dev/sdX with your USB device)"
    echo ""
}

# Run main function
main "$@"
