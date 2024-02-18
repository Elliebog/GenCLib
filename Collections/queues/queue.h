#include <sys/types.h>

struct gen_node {
    void* data;
    struct gen_node* next;
};

struct gen_queue {
    int size;
    size_t datasize;
    struct gen_node* head;
    struct gen_node* tail;
};

typedef struct gen_node gen_node_t;
typedef struct gen_queue gen_queue_t;
