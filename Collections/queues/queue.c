/*
 * @file queue.c
 * @brief Implementation of Queues
 * */
#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates a generic Queue which can store values of size datasize.
 * If the Queue cannot be created a NULL pointer is returned
 *
 * @param size_t datasize
 * @return the gen_queue_t pointer. If a queue couldn't be created this is NULL
 */
gen_queue_t *create_gen_queue(size_t datasize) {
    gen_queue_t *queue = (gen_queue_t *)malloc(sizeof(gen_queue_t));
    if (queue == NULL)
        return NULL;
    queue->size = 0;
    queue->datasize = datasize;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

/**
* @brief Create a for the queue 
* @param void* data The data to be stored
* @param size_t datasize sizeof(data)
* @param gen_node_t* next Pointer to the next element in the queue
* */
gen_node_t *create_node(void *data, size_t datasize, gen_node_t *next) {
    gen_node_t *nodep = malloc(datasize);
    nodep->next = next;
    nodep->data = data;
    return nodep;
}

/**
 * @brief Enqueue an item at the back
 * @param gen_queue_t* queue pointer to the queue
 * @param void* item data to be stored
 * @param bool copy should the data management copy the contents or just use the pointer
 * */
int add_item_back(gen_queue_t *queue, void *item, bool copy) {
    void *datap = NULL;
    // If copy is set we assume that the item pointer is not stable and we copy
    // its content
    if (copy) {
        datap = malloc(queue->datasize);
        memcpy(datap, item, queue->datasize);
    } else {
        datap = item;
    }

    // Add item to tail
    gen_node_t *nodep = create_node(datap, queue->datasize, NULL);
    if (queue->head == NULL && queue->tail == NULL) {
        queue->head = nodep;
        queue->tail = nodep;
    } else if (queue->head != NULL) {
        queue->tail->next = nodep;
        queue->tail = nodep;
    } else {
        // Last option left is that head is NULL but tail is not -> inconsistent
        // state and alert with error code
        return -1;
    }
    queue->size++;
    return 0;
}

/**
 * @brief Enqueue an item at the front of the queue
 * @param gen_queue_t* queue pointer to the queue
 * @param void* item data to be stored
 * @param bool copy should the data management copy the contents or just use the pointer
 * */
int add_item_front(gen_queue_t *queue, void *item, bool copy) {
    void *datap = NULL;
    // If copy is set we assume that the item pointer is not stable and we copy
    // its content
    if (copy) {
        datap = malloc(queue->datasize);
        memcpy(datap, item, queue->datasize);
    } else {
        datap = item;
    }

    // Add item to head
    gen_node_t *nodep = create_node(datap, queue->datasize, queue->head);
    if (queue->head == NULL && queue->tail == NULL) {
        queue->head = nodep;
        queue->tail = nodep;
    } else if (queue->head != NULL) {
        queue->head = nodep;
    } else {
        // Last option left is that head is NULL but tail is not -> inconsistent
        // state and alert with error code
        return -1;
    }
    queue->size++;
    return 0;
}


/**
 * @brief Pop the front (get and remove) 
 * @param gen_queue_t* queue pointer to queue
 * */
void *pop_front(gen_queue_t *queue) {
    if (queue->head == NULL) {
        return NULL;
    }

    void *datap = queue->head->data;
    free(queue->head);
    queue->head = queue->head->next;
    queue->size--;
    return datap;
}

/**
 * @brief Pop the back (get and remove) 
 * @param gen_queue_t* queue pointer to queue
 * */
void *pop_back(gen_queue_t *queue) {
    if (queue->tail == NULL)
        return NULL;

    gen_node_t *cur_nodep = queue->head;
    while (cur_nodep->next != queue->tail) {
        cur_nodep = cur_nodep->next;
    }
    void *datap = queue->tail->data;
    free(queue->tail);
    queue->tail = cur_nodep;
    queue->size--;
    return datap;
}

/**
 * @brief Get item at the front 
 * @param gen_queue_t* queue pointer to queue
 * */
void *peek_front(gen_queue_t *queue) {
    if (queue->head == NULL) {
        return NULL;
    }

    void *datap = queue->head->data;
    return datap;
}

/**
 * @brief Get item at the back 
 * @param gen_queue_t* queue pointer to queue
 * */
void *peek_back(gen_queue_t* queue) {
    if (queue->tail == NULL)
        return NULL;

    void *datap = queue->tail->data;
    return datap;
}
