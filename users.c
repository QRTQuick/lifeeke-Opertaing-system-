#include "users.h"
#include "vga.h"
#include "string.h"

static user_t users[MAX_USERS];

// Simple placeholder hash function
static void simple_hash(const char* password, char* hash_output) {
    uint32_t hash = 0;
    for (size_t i = 0; i < strlen(password); i++) {
        hash += password[i];
    }
    // Convert hash to a string representation (very basic)
    // This is not secure and for demonstration purposes only
    char temp[11]; // Max 10 digits for uint32_t + null terminator
    int j = 0;
    if (hash == 0) {
        temp[j++] = '0';
    } else {
        while (hash > 0) {
            temp[j++] = (hash % 10) + '0';
            hash /= 10;
        }
    }
    temp[j] = '\0';
    
    // Reverse the string
    for (int k = 0; k < j / 2; k++) {
        char c = temp[k];
        temp[k] = temp[j - 1 - k];
        temp[j - 1 - k] = c;
    }
    
    strncpy(hash_output, temp, MAX_PASSWORD_HASH_LEN - 1);
    hash_output[MAX_PASSWORD_HASH_LEN - 1] = '\0';
}

void init_users() {
    for (int i = 0; i < MAX_USERS; i++) {
        users[i].in_use = 0;
        memset(users[i].username, 0, MAX_USERNAME_LEN);
        memset(users[i].password_hash, 0, MAX_PASSWORD_HASH_LEN);
        users[i].permissions = PERM_NONE;
    }
    // Create a default admin user
    create_user("admin", "adminpass", PERM_ADMIN | PERM_USER);
    terminal_writestring("User management initialized. Default admin created.\n");
}

user_t* create_user(const char* username, const char* password, uint8_t permissions) {
    if (strlen(username) >= MAX_USERNAME_LEN || strlen(password) >= MAX_PASSWORD_HASH_LEN) {
        terminal_writestring("Error: Username or password too long.\n");
        return NULL;
    }

    if (find_user(username) != NULL) {
        terminal_writestring("Error: User already exists.\n");
        return NULL;
    }

    for (int i = 0; i < MAX_USERS; i++) {
        if (!users[i].in_use) {
            strcpy(users[i].username, username);
            simple_hash(password, users[i].password_hash);
            users[i].permissions = permissions;
            users[i].in_use = 1;
            terminal_writestring("User created: ");
            terminal_writestring(username);
            terminal_writestring("\n");
            return &users[i];
        }
    }
    terminal_writestring("Error: No space for new user.\n");
    return NULL;
}

user_t* find_user(const char* username) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].in_use && strcmp(users[i].username, username) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

int authenticate_user(const char* username, const char* password) {
    user_t* user = find_user(username);
    if (user == NULL) {
        return 0; // User not found
    }

    char provided_password_hash[MAX_PASSWORD_HASH_LEN];
    simple_hash(password, provided_password_hash);

    if (strcmp(user->password_hash, provided_password_hash) == 0) {
        return 1; // Authentication successful
    } else {
        return 0; // Incorrect password
    }
}

void delete_user(const char* username) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].in_use && strcmp(users[i].username, username) == 0) {
            users[i].in_use = 0;
            memset(users[i].username, 0, MAX_USERNAME_LEN);
            memset(users[i].password_hash, 0, MAX_PASSWORD_HASH_LEN);
            users[i].permissions = PERM_NONE;
            terminal_writestring("User deleted: ");
            terminal_writestring(username);
            terminal_writestring("\n");
            return;
        }
    }
    terminal_writestring("Error: User not found for deletion.\n");
}
