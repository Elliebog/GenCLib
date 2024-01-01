#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "gdhmap.h"

#define DEL_MARKER ((void*) 1)

int gdhmap_int_stringsum(char* str) {
    int sum = 0;
    size_t i = 0;
    while(str[i] != 0) {
        sum += str[i];
        i++;
    }
    return sum;
}

int gdhmap_int_is_prime(int n) {
    if(n == 2 || n == 3)
        return 1;

    if(n <= 1 || n % 2 == 0 || n % 3 == 0)
        return 0;

    for(int i = 5; i <= sqrt((double)n); i += 6) {
        if(n%i == 0 || n%(i+2) == 0) {
            return 0;
        }
    }
    return 1;
}

int gdhmap_int_prime(int n) {
    //nearest odd integer that is less than n
    int odd_m = (n % 2 == 0) ? n-1 : n-2; 
    for(int i = odd_m; i >= 1; i -= 2) {
        if(gdhmap_int_is_prime(i)) {
            return i;
        }
    }
    return 1;
}

gdhmap* gdhmap_create(size_t max_size) {
    gdhmap* map = malloc(sizeof(gdhmap));
    map->size = 0;
    map->max_size = max_size;
    map->keys = calloc(max_size, sizeof(char*));
    map->values = calloc(max_size, sizeof(void*));
    map->__prime = gdhmap_int_prime(map->max_size);
    return map;
}

void gdhmap_free(gdhmap* map, bool rem_Content) {
    if(rem_Content) {
        //free the memory blocks holding values and keys 
        for(int i = 0; i < map->max_size; i++) {
            free(map->keys[i]);
            free(map->values[i]);
        }
    }

    //free the rest of the pointers and map itself
    free(map->keys);
    free(map->values);
    free(map);
}

int gdhmap_hash1(size_t maxsize, char* key) {   
    return gdhmap_int_stringsum(key) % maxsize;
}

int gdhmap_nhash1(size_t maxsize, int key) {
    return key % maxsize;
}

int gdhmap_hash2(int prime, char* key) {
    return prime - (gdhmap_int_stringsum(key) % prime);  
}

void* gdhmap_get(gdhmap* gdhmap, char* key) {
    int hash = gdhmap_hash1(gdhmap->max_size, key);
    int offset = gdhmap_hash2(gdhmap->__prime, key);

    for(size_t i = 0; i < gdhmap->max_size; i++) {
        if(gdhmap->keys[hash] == NULL) {
            return NULL;
        }
        
        if(gdhmap->keys[hash] != DEL_MARKER && strcmp(gdhmap->keys[hash], key) == 0) {
            return gdhmap->values[hash];
        }

        hash = gdhmap_nhash1(gdhmap->max_size, hash + offset);  
    }

    return NULL;
}


int gdhmap_put(gdhmap* gdhmap, char* key, void* value, size_t valuesize) {
    //build hash and offsets for probing
    int hash = gdhmap_hash1(gdhmap->max_size, key);
    int offset= gdhmap_hash2(gdhmap->__prime, key);

    for(size_t i = 0; i < gdhmap->max_size; i++) {
        if(gdhmap->keys[hash] == NULL || gdhmap->keys[hash] == DEL_MARKER) {
            //insert if empty
            // We want to store keys and values ourselves and not rely on pointers to variables that may get freed later 
            // -> malloc and copy
            gdhmap->keys[hash] = malloc(strlen(key));
            gdhmap->keys[hash] = strcpy(gdhmap->keys[hash], key);
            gdhmap->values[hash] = malloc(valuesize); 
            memcpy(gdhmap->values[hash], value, valuesize);
            gdhmap->size++;
            return 1;
        }

        if(strcmp(gdhmap->keys[hash], key) == 0) {
            //key already exists
            return 0;
        }
        
        hash = gdhmap_nhash1(gdhmap->max_size, hash + offset);  
    }
    
    return 0;
}

int gdhmap_remove(gdhmap* gdhmap, char* key) {
    //build hash and offsets for probing
    int hash = gdhmap_hash1(gdhmap->max_size, key);
    int offset= gdhmap_hash2(gdhmap->__prime, key);

    for(size_t i = 0; i < gdhmap->max_size; i++) {
        if(gdhmap->keys[hash] == NULL) {
            return 0; 
        }

        if(gdhmap->keys[hash] != DEL_MARKER && strcmp(gdhmap->keys[hash], key) == 0) {
            //key is found -> remove
            //free value and key
            free(gdhmap->keys[hash]);
            free(gdhmap->values[hash]);
            gdhmap->keys[hash] = DEL_MARKER;
            gdhmap->values[hash] = DEL_MARKER;
           
            gdhmap->size--;
            return 1;
        }
        
        hash = gdhmap_nhash1(gdhmap->max_size, hash + offset);  
    }

   return 0;
}

gdhmap* gdhmap_resize(gdhmap* map, size_t new_size, size_t typesize) {
    char** keys = map->keys;
    void** values = map->values;
    size_t oldsize = map->max_size;

    //Create map with new size
    gdhmap* new_map = gdhmap_create(new_size);
    for(size_t i = 0; i < oldsize; i++) {
        if(keys[i] != NULL && keys[i] != DEL_MARKER)
            gdhmap_put(new_map, keys[i], values[i], typesize);
    }

    gdhmap_free(map,false);
    return new_map;
}