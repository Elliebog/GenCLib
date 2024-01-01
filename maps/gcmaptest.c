#include <stdio.h>
#include "gcmap.h"

void print_gcmap(gcmap *gcmap_p) {
    for (size_t i = 0; i < gcmap_p->max_size; i++) {
        gcmap_node *node = *(gcmap_p->map + i);
        if (node == NULL) {
            printf("[%d]: %s", (int)i, "NULL");
        } else {
            printf("[%d]: %s", (int)i, node->key);
            while (node->next != NULL) {
                node = node->next;
                printf("->%s", node->key);
            }
        }
        printf("\n");
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    gcmap *gcmap = gcmap_create(20);
    gcmap_put(gcmap, "ab", &arr[0]);
    gcmap_put(gcmap, "ac", &arr[1]);
    gcmap_put(gcmap, "ad", &arr[2]);
    gcmap_put(gcmap, "bc", NULL);
    gcmap_put(gcmap, "cb", NULL);
    gcmap_remove(gcmap, "bc");
    print_gcmap(gcmap);
    gcmap_free(gcmap);
}