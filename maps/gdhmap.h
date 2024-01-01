#include <stdlib.h>
#define GDH_MAX_KEY_SIZE 128

typedef struct gdhmap_t {
    char** keys;
    void** values;
    size_t max_size;
    size_t size;
    int __prime;
} gdhmap;


gdhmap* gdhmap_create(size_t max_size);
void gdhmap_free(gdhmap* gdhmap);
void* gdhmap_get(gdhmap* gdhmap, char* key);
int gdhmap_put(gdhmap* gdhmap, char* key, void* value, size_t valuesize);
int gdhmap_remove(gdhmap* gdhmap, char* key);
