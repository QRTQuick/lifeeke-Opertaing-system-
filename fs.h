#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_FILENAME_LEN 32
#define MAX_FILE_CONTENT_SIZE 1024
#define MAX_FILES 16

typedef struct {
    char name[MAX_FILENAME_LEN];
    uint32_t size;
    char content[MAX_FILE_CONTENT_SIZE];
    uint8_t in_use;
} file_t;

void init_fs();
file_t* create_file(const char* name);
file_t* find_file(const char* name);
void write_file(file_t* file, const char* data, uint32_t len);
void read_file(file_t* file, char* buffer, uint32_t len);
void delete_file(file_t* file);

#endif
