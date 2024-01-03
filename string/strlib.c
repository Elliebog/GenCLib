#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include "strlib.h"
#include <stdlib.h>

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

/// @brief Checks whether `prefix` is a prefix of `str` (none of the strings have to be null terminated)
/// @param prefix the prefix to be checked
/// @param prefix_len the length of the prefix
/// @param str the string to be checked
/// @return 1 if the prefix is a prefix -> else 0
int is_prefix(char* prefix, size_t prefix_len, char* str) {
    for(size_t i = 0; i < prefix_len; i++) {
        if(prefix[i] != str[i]) {
            return 0;
        }
    }
    return 1;
}

/// @brief Checks whether `suffix` is a suffix of `str` (none of the strings have to be null terminated)
/// @param suffix the suffix to be checked
/// @param suffix_len the length of the suffix
/// @param str the string to be checked
/// @param str_len the length of the string to be checked
/// @return 1 if the suffix is a suffix -> else 0
int is_suffix(char* suffix, size_t suffix_len, char* str, size_t str_len) {
    for(int i = 0; i < suffix_len; i++) {
        if(suffix[i] != str[str_len - suffix_len + i]) {
            return 0;
        }
    }
    return 1;
}

/// @brief Checks if a string contains a character
/// @param arr the string to check
/// @param len the length of the string
/// @param key the character to look for
/// @return 1 if the key is in the string else 0
int contains(char* arr, size_t len, char key) {
    for(size_t i = 0; i < len; i++) {
        if(arr[i] == key) {
            return 1;
        }
    }
    return 0;
}

/// @brief Remove the newline at the end of the string
/// @param str 
void remove_newline(char* str) {
    int len = strlen(str);
    str[len-1] = 0;
}



/// @brief Removes leading and trailing whitespaces
/// @param str the input string
/// @param buf buffer in which new string will be put. PLease instantiate with calloc(1, strlen(str)),
/// or ensure that it is init to zero
int remove_whitespaces(char* str, char* buf) {
    int len = strlen(str);

    size_t start_i = 0;
    size_t i = 0;
    size_t end_i = len;
    bool found = false;
    while(str[i] != 0) {
        if(str[i] != ' ') {
            start_i = i;
            found = true;
            break;
        }
        i++;
    }
    if(found) {
        for(int i = len - 1; i > start_i; i--) {
            if(str[i] != ' ') {
                end_i = i;
                break;
            }
        }
        strncpy(buf, str+start_i, end_i - start_i+1);
    }
    else {
        perror("No other characters except whitespaces was found");
        return -1; 
    }
    return 0;
}

/// @brief Split the string into 2 strings on the first occurence of a specific character
/// @param str the original string
/// @param str1 buffer for the first part of the string
/// @param str2 buffer for the second part of the string
/// @param c the character at which to split
/// @return -1 if the character wasn't found. else 0
int split_at_char(char* str, char* str1, char* str2, char c) {
    size_t i = 0;
    int splitindex = -1;
    while(str[i] != 0) {
        if(str[i] == c) {
            splitindex = i;
        }
        i++;
    }

    if(splitindex != -1) {
        strncpy(str1, str, splitindex);
        strncpy(str2, str+splitindex+1, i - splitindex-1);
    }
    else {
        perror("Character was not found in array");
        return 0;
    }
    return 1;
}


/// @brief Search and replace the first occurence of a key within a string
/// @param str the string which contains the key (Null terminated)
/// @param key the key
/// @param replace replace sequence
/// @param buffer Where the output will be written
/// @return 1 if the replacing was successful. 0 if not. 
int search_replace(char* str, char* key, char* replace, char* buffer) {
    size_t len_replace = strlen(replace);
    char* idx;
    if((idx = strstr(str, key)) != NULL) {
        int len_prefix = idx - str;
        strncpy(buffer, str, len_prefix);
        strncpy(buffer+len_prefix, replace, len_replace);
        strcpy(buffer+len_prefix+len_replace, str+len_prefix+strlen(key));
        return 1;
    }    
    else {
        return 0;
    }
}