#include "strlib.h"
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contains(char *arr, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        if (arr[i] == key) {
            return 1;
        }
    }
    return 0;
}

void remove_newline(char *str) {
    int len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = 0;
}

/// @brief Removes leading and trailing whitespaces
/// @param str the input string
/// @param buf buffer in which new string will be put. PLease instantiate with
/// calloc(1, strlen(str)), or ensure that it is init to zero
int remove_whitespaces(char *str, char *buf) {
    int len = strlen(str);

    int start_i = 0;
    int i = 0;
    int end_i = len;
    bool found = false;
    while (str[i] != 0) {
        if (str[i] != ' ') {
            start_i = i;
            found = true;
            break;
        }
        i++;
    }
    if (found) {
        for (int i = len - 1; i > start_i; i--) {
            if (str[i] != ' ') {
                end_i = i;
                break;
            }
        }
        strncpy(buf, str + start_i, end_i - start_i + 1);
    } else {
        perror("No other characters except whitespaces was found");
        return -1;
    }
    return 0;
}

/**
 * @brief Split a string into two strings on the first occurence of a character
 *
 * @param str the original string
 * @param str1 prefix
 * @param str2 suffix
 * @param c the character to split at
 * @return 0 if the character wasn't found. else 1
 */
int split_at_char(char *str, char *str1, char *str2, char c) {
    size_t i = 0;
    int splitindex = -1;
    int found = 0;
    // Let i do a complete run until end so that i = strlen(str)
    while (str[i] != 0) {
        if (str[i] == c && !found) {
            splitindex = i;
            found = 1;
        }
        i++;
    }

    if (splitindex != -1) {
        strncpy(str1, str, splitindex);
        strncpy(str2, str + splitindex + 1, i - splitindex - 1);
        // add termination char \0
        str1[splitindex] = 0;
        str2[i - splitindex - 1] = 0;
    } else {
        perror("Character was not found in array");
        return 0;
    }
    return 1;
}

/**
 * @brief Replace a key with another string and put it into buffer
 *
 * @param str the str key is contained in
 * @param key the key
 * @param replace what to replace with (null-terminated)
 * @return Pointer to a calloc'ed string with length strlen(str)+strlen(replace)
 */
char *str_replace(char *str, char *key, char *replace) {
    size_t len_replace = strlen(replace);
    char *buffer = calloc(strlen(str) + len_replace, 1);
    char *idx;
    if ((idx = strstr(str, key)) != NULL) {
        int len_prefix = idx - str;
        strncpy(buffer, str, len_prefix);
        strncpy(buffer + len_prefix, replace, len_replace);
        strcpy(buffer + len_prefix + len_replace,
               str + len_prefix + strlen(key));
        return buffer;
    } else {
        free(buffer);
        return NULL;
    }
}

/**
 * @brief Get the number of characters needed for a buffer if we store it with
 * sprintf
 *
 * @param format format of the str
 * @param ...
 * @return int the number of characters the buffer needs at least to contain the
 * result of sprintf
 */
int get_reqbuffer_len(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("%s", format);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);
    return len + 1; // accomodate for \0 termination byte
}

#define DEFAULT_STR_LEN 256

char *alloc_stringf(const char *format, ...) {
    int n;
    size_t size = DEFAULT_STR_LEN; 
    char *p = NULL, *np = NULL;
    va_list ap;

    if ((p = malloc(size)) == NULL)
        return NULL;

    while (1) {
        va_start(ap, format);
        n = vsnprintf(p, size, format, ap);
        va_end(ap);


        if (n < 0)
            return NULL;
        
        //return string if correct allocation
        if (n < size)
            return p;

        //+1 because of \0 char
        size = n + 1;

        if ((np = realloc(p, size)) == NULL) {
            free(p);
            return NULL;
        } else {
            p = np;
        }
    }
}