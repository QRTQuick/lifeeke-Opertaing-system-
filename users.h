#ifndef USERS_H
#define USERS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_USERNAME_LEN 32
#define MAX_PASSWORD_HASH_LEN 64 // For a simple hash
#define MAX_USERS 8

enum user_permissions {
    PERM_NONE = 0,
    PERM_USER = 1 << 0,
    PERM_ADMIN = 1 << 1,
};

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password_hash[MAX_PASSWORD_HASH_LEN];
    uint8_t permissions;
    uint8_t in_use;
} user_t;

void init_users();
user_t* create_user(const char* username, const char* password, uint8_t permissions);
user_t* find_user(const char* username);
int authenticate_user(const char* username, const char* password);
void delete_user(const char* username);

#endif
