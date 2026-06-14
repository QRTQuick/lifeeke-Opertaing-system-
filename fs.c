#include "fs.h"
#include "vga.h" // For terminal_writestring
#include <string.h> // For memcpy, memset, strcmp

static file_t files[MAX_FILES];

void init_fs() {
    for (int i = 0; i < MAX_FILES; i++) {
        files[i].in_use = 0;
        memset(files[i].name, 0, MAX_FILENAME_LEN);
        files[i].size = 0;
        memset(files[i].content, 0, MAX_FILE_CONTENT_SIZE);
    }
    terminal_writestring("In-memory file system initialized.\n");
}

file_t* create_file(const char* name) {
    if (strlen(name) >= MAX_FILENAME_LEN) {
        terminal_writestring("Error: Filename too long.\n");
        return NULL;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].in_use && strcmp(files[i].name, name) == 0) {
            terminal_writestring("Error: File already exists.\n");
            return NULL;
        }
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].in_use) {
            strcpy(files[i].name, name);
            files[i].size = 0;
            files[i].in_use = 1;
            terminal_writestring("File created: ");
            terminal_writestring(name);
            terminal_writestring("\n");
            return &files[i];
        }
    }
    terminal_writestring("Error: No space for new file.\n");
    return NULL;
}

file_t* find_file(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].in_use && strcmp(files[i].name, name) == 0) {
            return &files[i];
        }
    }
    return NULL;
}

void write_file(file_t* file, const char* data, uint32_t len) {
    if (!file) return;
    if (len > MAX_FILE_CONTENT_SIZE) {
        terminal_writestring("Error: Content too large for file.\n");
        len = MAX_FILE_CONTENT_SIZE;
    }
    memcpy(file->content, data, len);
    file->size = len;
    terminal_writestring("Wrote ");
    terminal_writehex(len);
    terminal_writestring(" bytes to file: ");
    terminal_writestring(file->name);
    terminal_writestring("\n");
}

void read_file(file_t* file, char* buffer, uint32_t len) {
    if (!file) return;
    if (len > file->size) {
        len = file->size;
    }
    memcpy(buffer, file->content, len);
    buffer[len] = '\0'; // Null-terminate the buffer
    terminal_writestring("Read ");
    terminal_writehex(len);
    terminal_writestring(" bytes from file: ");
    terminal_writestring(file->name);
    terminal_writestring("\n");
}

void delete_file(file_t* file) {
    if (!file) return;
    file->in_use = 0;
    memset(file->name, 0, MAX_FILENAME_LEN);
    file->size = 0;
    memset(file->content, 0, MAX_FILE_CONTENT_SIZE);
    terminal_writestring("File deleted.\n");
}
