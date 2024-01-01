#include "gcmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gcmap_hash(size_t maxsize, char *key) {
    // Use a normal modulo hash function

    int hash = 0;
    size_t i = 0;
    while (key[i] != 0) {
        // abort if max keylength is violated
        if (i >= MAX_KEY_LENGTH)
            return -1;
        // not possible to integer overflow because of max_key_length
        hash += key[i];
        i++;
    }
    return hash % maxsize;
}

void *gcmap_get(gcmap *gcmap_p, char *key) {
    int hash = gcmap_hash(gcmap_p->max_size, key);
    if (hash == -1) {
        perror("Maximum key size violated");
        return NULL;
    }

    gcmap_node *curNode = *(gcmap_p->map + hash);
    if (strcmp(curNode->key, key) == 0) {
        return curNode->value;
    }

    while (curNode->next != NULL) {
        curNode = curNode->next;
        if (strcmp(curNode->key, key) == 0) {
            return curNode->value;
        }
    }

    return NULL;
}

int gcmap_put(gcmap *gcmap, char *key, void *value) {
    int hash = gcmap_hash(gcmap->max_size, key);
    if (hash == -1) {
        perror("Maximum key size violated");
        return -1;
    }
    gcmap_node *curNode = *(gcmap->map + hash);

    if (curNode == NULL) {
        gcmap->map[hash] = new_node(key, value, NULL);
        gcmap->size++;
        return 1;
    }

    if (strcmp(curNode->key, key) == 0) {
        return 0;
    }

    while (curNode->next != NULL) {
        curNode = curNode->next;
        if (strcmp(curNode->key, key) == 0) {
            return 0;
        }
    }
    curNode->next = new_node(key, value, NULL);
    gcmap->size++;
    return 1;
}

gcmap_node* new_node(char *key, void *value, gcmap_node *next) {
    gcmap_node *node = malloc(sizeof(gcmap_node));
    node->key = key;
    node->value = value;
    node->next = next;
    return node;
}

gcmap *gcmap_create(size_t max_size) {
    gcmap *new_gcmap = malloc(sizeof(gcmap));
    new_gcmap->max_size = max_size;
    new_gcmap->size = 0; // TODO Update size and add remove method
    new_gcmap->map = calloc(max_size, sizeof(gcmap_node *));
    return new_gcmap;
}

void gcmap_free(gcmap *gcmap_p) {
    free(gcmap_p->map);
    free(gcmap_p);
}

int gcmap_remove(gcmap *gcmap, char *key) {
    int hash = gcmap_hash(gcmap->max_size, key);
    if (hash == -1) {
        // value with such a key cannot exist because the key exceeds the
        // maximum allowed length
        return 0;
    }

    gcmap_node *curNode = *(gcmap->map + hash);
    if (curNode == NULL) {
        return 0;
    }

    if (strcmp(curNode->key, key) == 0) {
        *(gcmap->map+hash) = curNode->next;
        return 1;
    }

    while (curNode->next != NULL) {
        if (strcmp(curNode->next->key, key) == 0) {
            curNode->next = curNode->next->next;
            return 1;
        }
        curNode->next = curNode;
    }
    return 0;
}