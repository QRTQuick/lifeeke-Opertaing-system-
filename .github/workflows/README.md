# GitHub Actions Workflow for Graphical OS

This directory contains the GitHub Actions workflow to automatically build your Graphical OS ISO.

## How It Works

The workflow (`.github/workflows/build-os.yml`) performs the following steps:

### 1. **Build Job** (`build-iso`)
- Installs required tools (NASM, GCC, xorriso, GRUB)
- Assembles the boot sector and kernel from assembly files
- Creates a bootable ISO with GRUB bootloader
- Uploads the ISO as an artifact (available for 30 days)
- Automatically creates a GitHub Release when you push a tag

### 2. **Test Job** (`test-qemu`)
- Runs on every push to main/master branches
- Builds the OS and tests it in QEMU emulator
- Ensures the build process works correctly

## Usage

### Automatic Build on Push
Every time you push to `main` or `master` branch, the workflow will:
1. Build the ISO
2. Make it available as a downloadable artifact
3. Run basic tests

### Manual Trigger
You can manually trigger the build:
1. Go to your repository's "Actions" tab
2. Select "Build Graphical OS ISO" workflow
3. Click "Run workflow"

### Create a Release
To create an official release with the ISO attached:
```bash
git tag v1.0.0
git push origin v1.0.0
```

This will automatically:
- Build the ISO
- Create a GitHub release
- Attach the ISO file to the release

## Downloading the ISO

### From Artifacts (for each build)
1. Go to Actions tab
2. Select the workflow run
3. Download the `graphical-os-iso` artifact

### From Releases (for tagged versions)
1. Go to Releases page
2. Download the ISO from the latest release

## Customization

You can modify the workflow to:
- Change the ISO name and volume label
- Add more build steps
- Include additional files in the ISO
- Change the GRUB configuration
- Add more test scenarios

## Required Repository Secrets

No secrets are required for basic functionality. The workflow uses the default `GITHUB_TOKEN` for creating releases.

## Troubleshooting

If the build fails:
1. Check the Actions log for error messages
2. Ensure your assembly files (`boot.asm`, `kernel.asm`) are in the root directory
3. Verify the syntax of your assembly code
4. Check that all required files are committed to the repository
