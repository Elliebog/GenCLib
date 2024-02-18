#include <stdlib.h>
#define MAX_KEY_LENGTH 128

typedef struct gcmap_node_t {
    char* key;
    void* value;
    struct gcmap_node_t* next;
} gcmap_node;

typedef struct gcmap_t {
    size_t max_size;
    gcmap_node** map; 
    size_t size;
} gcmap;

gcmap* gcmap_create(size_t max_size);
void gcmap_free(gcmap* gcmap);
int gcmap_hash(size_t maxsize, char* key);
void* gcmap_get(gcmap* gcmap, char* key);
int gcmap_put(gcmap* gcmap, char* key, void* value);
int gcmap_remove(gcmap* gcmap, char* key);
gcmap_node* new_node(char* key, void* value, gcmap_node* next);