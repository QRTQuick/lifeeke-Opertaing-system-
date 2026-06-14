# lifeOS Design Document

## Phase 1: OS Architecture and Core Components

### 1.1 Global Descriptor Table (GDT)

The GDT is a data structure used by Intel x86-family processors starting with the 80286 to define the characteristics of the various memory areas used during program execution, including the base address, size, and access privileges of memory segments. For lifeOS, we will implement a minimal GDT to switch from real mode to protected mode and define segments for code and data.

### 1.2 Interrupt Descriptor Table (IDT)

The IDT is a data structure that maps interrupts and exceptions to their corresponding interrupt service routines (ISRs). It is crucial for handling hardware interrupts (like keyboard input) and software exceptions. We will set up a basic IDT to handle essential interrupts.

### 1.3 Keyboard Input

Keyboard input will be handled via interrupts. We will program the Programmable Interrupt Controller (PIC) to receive keyboard interrupts and write an ISR to read scan codes from the keyboard controller and convert them into ASCII characters.

### 1.4 VGA Output

The existing `kernel.c` already includes basic VGA text mode output. We will enhance this to provide more robust text output capabilities, including scrolling and cursor management.

### 1.5 Bootloader (boot.asm)

The `boot.asm` file will be responsible for:
- Setting up the initial CPU state.
- Enabling A20 line.
- Loading the GDT.
- Switching to 32-bit protected mode.
- Jumping to the C kernel entry point (`kernel_main`).

### 1.6 Kernel Entry (kernel.c)

The `kernel.c` will be the main entry point for the C code. It will initialize the IDT, keyboard, and VGA, and then enter an infinite loop, waiting for interrupts or user input.

## Phase 2: File System and Basic I/O Operations

### 2.1 Disk I/O (ATA Driver)

To interact with storage devices, lifeOS will implement a basic ATA (Advanced Technology Attachment) driver in PIO (Programmed I/O) mode. This driver will allow reading and writing of sectors to and from the hard disk. Key functionalities will include:

-   **ATA Register Access**: Functions to read from and write to ATA command and control block registers.
-   **Sector Read/Write**: Routines to read a specified number of sectors from a logical block address (LBA) and write data to sectors.
-   **Error Handling**: Basic error checking for ATA operations.

### 2.2 Simple In-Memory File System (Initial Concept)

For initial development and to facilitate the login and file editing features without immediately diving into a complex on-disk file system, we will implement a very basic **in-memory file system**. This will allow us to simulate file creation, reading, and editing within the kernel's memory space. This approach will be temporary and will be replaced by a persistent on-disk file system in a later, more advanced phase of development.

Key features of the in-memory file system:

-   **File Table**: A simple array or linked list to store file metadata (name, size, content pointer).
-   **Content Storage**: A region of memory allocated for file content.
-   **Basic Operations**: `create_file`, `read_file`, `write_file`, `delete_file` functions that operate on the in-memory structures.

This in-memory file system will serve as a placeholder to develop the higher-level shell and editor components, allowing us to focus on user interaction and command processing before tackling the complexities of persistent storage.

## Phase 3: Login System and User Management

### 3.1 User Structure

We will define a simple `user_t` structure to hold user information:

-   `username`: A fixed-size string for the username.
-   `password_hash`: A fixed-size string to store a simple hash of the password (for security, even in a basic OS, storing plaintext passwords is not advisable).
-   `permissions`: A flag or set of flags to define user privileges (e.g., admin, regular user).
-   `in_use`: A boolean to indicate if the user slot is active.

### 3.2 In-Memory User Database

Similar to the file system, user data will initially be stored in an in-memory array of `user_t` structures. This allows for quick prototyping of the login functionality without immediate reliance on persistent storage.

### 3.3 Authentication Module

A core function `authenticate_user(username, password)` will be implemented. This function will:

-   Look up the user by username in the in-memory database.
-   Compute the hash of the provided password.
-   Compare the computed hash with the stored `password_hash`.
-   Return a success/failure status and potentially the user's permissions.

### 3.4 User Management Functions

Basic user management functions will be provided:

-   `create_user(username, password, permissions)`: Adds a new user to the system.
-   `delete_user(username)`: Removes an existing user.
-   `modify_user_password(username, new_password)`: Changes a user's password.
-   `modify_user_permissions(username, new_permissions)`: Updates user privileges.

These functions will be accessible through the shell, after a successful login by an administrative user.

## Phase 4: Shell and File Editor Interface

### 4.1 Command-Line Interface (Shell)

The shell will be the primary interface for user interaction after successful login. It will provide a prompt, accept user input, parse commands, and execute corresponding functions. Key features include:

-   **Command Parsing**: A simple parser to extract command names and arguments from user input.
-   **Command Dispatch**: A mechanism to map commands to their respective handler functions.
-   **Input Buffer**: A buffer to store user input from the keyboard.
-   **Command History**: (Optional, for future enhancement) A basic command history.

Supported commands will include:

-   `login <username> <password>`: Authenticates a user.
-   `logout`: Logs out the current user.
-   `createuser <username> <password> [permissions]`: Creates a new user (admin only).
-   `createfile <filename>`: Creates a new file in the in-memory file system.
-   `editfile <filename>`: Opens a simple text editor for the specified file.
-   `readfile <filename>`: Displays the content of a file.
-   `ls`: Lists all files in the in-memory file system.
-   `clear`: Clears the terminal screen.
-   `help`: Displays a list of available commands and their usage.

### 4.2 Simple Text Editor

A basic line-oriented text editor will be implemented to allow users to edit file content. This editor will:

-   Load the content of a specified file.
-   Allow character-by-character input.
-   Provide basic navigation (e.g., backspace).
-   Save the modified content back to the in-memory file system.
