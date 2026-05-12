#!/bin/bash

# Workflow to compile C code and package it into an ISO file

set -e

echo "=== Step 1: Compile the C program ==="
gcc -o hello hello.c
echo "Compiled hello.c to hello executable"

echo ""
echo "=== Step 2: Create directory structure for ISO ==="
mkdir -p iso_root
cp hello iso_root/
echo "Copied executable to iso_root/"

echo ""
echo "=== Step 3: Create ISO file ==="
# Using genisoimage (or mkisofs) to create the ISO
# Install with: sudo apt-get install genisoimage (Debian/Ubuntu)
# or: sudo yum install genisoimage (RHEL/CentOS)
genisoimage -o hello.iso -R -J iso_root/

echo ""
echo "=== Step 4: Verify ISO creation ==="
if [ -f hello.iso ]; then
    echo "✓ ISO file created successfully: hello.iso"
    ls -lh hello.iso
else
    echo "✗ Failed to create ISO file"
    exit 1
fi

echo ""
echo "=== Step 5: Cleanup (optional) ==="
# Uncomment the next line if you want to remove temporary files
# rm -rf iso_root hello

echo ""
echo "Done! You can now:"
echo "  - Mount the ISO: sudo mount -o loop hello.iso /mnt"
echo "  - Or burn it to a CD/DVD"
echo "  - Or use it in a virtual machine"
