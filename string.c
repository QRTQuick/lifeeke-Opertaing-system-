#include "string.h"

void* memcpy(void* dest, const void* src, size_t count)
{
    char* dst8 = (char*)dest;
    char* src8 = (char*)src;
    while (count--) {
        *dst8++ = *src8++;
    }
    return dest;
}

void* memset(void* dest, int val, size_t count)
{
    char* temp = (char*)dest;
    while (count--) {
        *temp++ = (char)val;
    }
    return dest;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

int strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

char* strcpy(char* dest, const char* src)
{
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

char* strncpy(char* dest, const char* src, size_t count)
{
    size_t i;
    for (i = 0; i < count && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < count; i++) {
        dest[i] = '\0';
    }
    return dest;
}

static char* strtok_last = NULL;

char* strtok(char* str, const char* delim) {
    char* token_start;

    if (str != NULL) {
        strtok_last = str;
    }

    if (strtok_last == NULL) {
        return NULL;
    }

    // Skip leading delimiters
    while (*strtok_last != '\0' && strchr(delim, *strtok_last) != NULL) {
        strtok_last++;
    }

    if (*strtok_last == '\0') {
        return NULL;
    }

    token_start = strtok_last;

    // Find the end of the token
    while (*strtok_last != '\0' && strchr(delim, *strtok_last) == NULL) {
        strtok_last++;
    }

    // Null-terminate the token and update strtok_last
    if (*strtok_last != '\0') {
        *strtok_last = '\0';
        strtok_last++;
    }

    return token_start;
}

static char* strchr(const char* s, int c) {
    while (*s != '\0' && *s != (char)c) {
        s++;
    }
    if (*s == (char)c) {
        return (char*)s;
    }
    return NULL;
}
