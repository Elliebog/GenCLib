#pragma once
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


void remove_newline(char* str);
int remove_whitespaces(char* str, char* buf);
int split_at_char(char* str, char* str1, char* str2, char c);
char* str_replace(char* str, char* key, char* replace);
int get_reqbuffer_len(const char* format, ...);
char* alloc_stringf(const char* format, ...);