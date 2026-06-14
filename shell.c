#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"
#include "users.h"
#include "fs.h"

#define MAX_COMMAND_LEN 256
#define MAX_ARGS 16

static char command_buffer[MAX_COMMAND_LEN];
static int command_buffer_idx = 0;

static user_t* current_user = NULL;

void print_prompt() {
    if (current_user) {
        terminal_writestring(current_user->username);
        terminal_writestring("@lifeOS> ");
    } else {
        terminal_writestring("lifeOS> ");
    }
}

void execute_command(char* command) {
    char* argv[MAX_ARGS];
    int argc = 0;
    char* token = strtok(command, " ");
    while (token != NULL && argc < MAX_ARGS) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if (argc == 0) {
        return;
    }

    if (strcmp(argv[0], "login") == 0) {
        if (argc == 3) {
            if (authenticate_user(argv[1], argv[2])) {
                current_user = find_user(argv[1]);
                terminal_writestring("Login successful!\n");
            } else {
                terminal_writestring("Login failed: Invalid username or password.\n");
            }
        } else {
            terminal_writestring("Usage: login <username> <password>\n");
        }
    } else if (strcmp(argv[0], "logout") == 0) {
        if (current_user) {
            current_user = NULL;
            terminal_writestring("Logged out.\n");
        } else {
            terminal_writestring("Not logged in.\n");
        }
    } else if (strcmp(argv[0], "createuser") == 0) {
        if (!current_user || !(current_user->permissions & PERM_ADMIN)) {
            terminal_writestring("Permission denied. Admin privileges required.\n");
            return;
        }
        if (argc == 4) {
            uint8_t permissions = PERM_USER;
            if (strcmp(argv[3], "admin") == 0) {
                permissions |= PERM_ADMIN;
            }
            create_user(argv[1], argv[2], permissions);
        } else {
            terminal_writestring("Usage: createuser <username> <password> [admin]\n");
        }
    } else if (strcmp(argv[0], "createfile") == 0) {
        if (!current_user) {
            terminal_writestring("Permission denied. Please login.\n");
            return;
        }
        if (argc == 2) {
            create_file(argv[1]);
        } else {
            terminal_writestring("Usage: createfile <filename>\n");
        }
    } else if (strcmp(argv[0], "readfile") == 0) {
        if (!current_user) {
            terminal_writestring("Permission denied. Please login.\n");
            return;
        }
        if (argc == 2) {
            file_t* file = find_file(argv[1]);
            if (file) {
                char buffer[MAX_FILE_CONTENT_SIZE + 1];
                read_file(file, buffer, file->size);
                terminal_writestring("\n");
                terminal_writestring(buffer);
                terminal_writestring("\n");
            } else {
                terminal_writestring("Error: File not found.\n");
            }
        } else {
            terminal_writestring("Usage: readfile <filename>\n");
        }
    } else if (strcmp(argv[0], "editfile") == 0) {
        if (!current_user) {
            terminal_writestring("Permission denied. Please login.\n");
            return;
        }
        if (argc == 2) {
            file_t* file = find_file(argv[1]);
            if (file) {
                terminal_writestring("Entering editor mode. Type your content. Press Ctrl+S (not implemented) or Ctrl+D to save and exit.\n");
                terminal_writestring("Current content:\n");
                char buffer[MAX_FILE_CONTENT_SIZE + 1];
                read_file(file, buffer, file->size);
                terminal_writestring(buffer);
                terminal_writestring("\n");

                char editor_buffer[MAX_FILE_CONTENT_SIZE];
                int editor_buffer_idx = 0;
                char c;
                while (1) {
                    c = get_char();
                    if (c == 4) { // Ctrl+D for now to exit editor
                        break;
                    }
                    if (c == '\b') { // Backspace
                        if (editor_buffer_idx > 0) {
                            editor_buffer_idx--;
                            terminal_putchar('\b');
                            terminal_putchar(' ');
                            terminal_putchar('\b');
                        }
                    } else if (editor_buffer_idx < MAX_FILE_CONTENT_SIZE - 1) {
                        editor_buffer[editor_buffer_idx++] = c;
                        terminal_putchar(c);
                    }
                }
                editor_buffer[editor_buffer_idx] = '\0';
                write_file(file, editor_buffer, editor_buffer_idx);
                terminal_writestring("Exited editor. File saved.\n");
            } else {
                terminal_writestring("Error: File not found.\n");
            }
        } else {
            terminal_writestring("Usage: editfile <filename>\n");
        }
    } else if (strcmp(argv[0], "ls") == 0) {
        if (!current_user) {
            terminal_writestring("Permission denied. Please login.\n");
            return;
        }
        terminal_writestring("Files:\n");
        for (int i = 0; i < MAX_FILES; i++) {
            if (files[i].in_use) {
                terminal_writestring("  ");
                terminal_writestring(files[i].name);
                terminal_writestring(" (Size: ");
                terminal_writehex(files[i].size);
                terminal_writestring(" bytes)\n");
            }
        }
    } else if (strcmp(argv[0], "clear") == 0) {
        init_vga(); // Re-initialize VGA to clear screen
    } else if (strcmp(argv[0], "help") == 0) {
        terminal_writestring("Available commands:\n");
        terminal_writestring("  login <username> <password> - Log in to the system\n");
        terminal_writestring("  logout - Log out from the system\n");
        terminal_writestring("  createuser <username> <password> [admin] - Create a new user (admin only)\n");
        terminal_writestring("  createfile <filename> - Create a new file\n");
        terminal_writestring("  readfile <filename> - Read content of a file\n");
        terminal_writestring("  editfile <filename> - Edit content of a file (Ctrl+D to save and exit)\n");
        terminal_writestring("  ls - List all files\n");
        terminal_writestring("  clear - Clear the screen\n");
        terminal_writestring("  help - Display this help message\n");
    } else {
        terminal_writestring("Unknown command: ");
        terminal_writestring(argv[0]);
        terminal_writestring("\n");
    }
}

void start_shell() {
    terminal_writestring("Welcome to lifeOS! Please login.\n");
    print_prompt();

    char c;
    while (1) {
        c = get_char();
        if (c == '\n') {
            command_buffer[command_buffer_idx] = '\0';
            terminal_putchar('\n');
            execute_command(command_buffer);
            command_buffer_idx = 0;
            memset(command_buffer, 0, MAX_COMMAND_LEN);
            print_prompt();
        } else if (c == '\b') {
            if (command_buffer_idx > 0) {
                command_buffer_idx--;
                terminal_putchar('\b');
                terminal_putchar(' ');
                terminal_putchar('\b');
            }
        } else if (command_buffer_idx < MAX_COMMAND_LEN - 1) {
            command_buffer[command_buffer_idx++] = c;
            terminal_putchar(c);
        }
    }
}
