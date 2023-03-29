#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// header files
#include "pq.h"

/*
The next macro is used to find the position of the next element in the queue, given the current position and the queue capacity. It does this by adding 1 to the current position and taking the modulo of the result with the queue capacity. This ensures that the position remains within the bounds of the queue, even if the current position is the last element in the queue.

The prev macro is used to find the position of the previous element in the queue, given the current position and the queue capacity. It does this by adding the queue capacity minus 1 to the current position and taking the modulo of the result with the queue capacity. This ensures that the position remains within the bounds of the queue, even if the current position is the first element in the queue.
*/

#define next(i,k) ((i + 1) % k)          // macro to get the next element, i mean element position, k mean capacity
#define prev(i,k) ((i + k-1) % k)        // macro to get the previous element position

struct PriorityQueue {
    uint32_t begin;    // position of the first element in the queue
    uint32_t end;      // position of the last element in the queue
    uint32_t size;     // number of elements in the queue
    uint32_t capacity; // maximum number of elements the queue can hold
    Node **nodes;      // array of pointers to Node objects
};

PriorityQueue *pq_create(uint32_t capacity) {
    // allocate memory for the queue
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (pq) {
        pq->begin = 0;
        pq->end = 0;
        pq->size = 0;
        pq->capacity = capacity + 1; // add an extra element to the capacity to distinguish between a full and an empty queue
        pq->nodes = (Node **) calloc(pq->capacity, sizeof(Node *));
        if (!pq->nodes) {
            free(pq);
            pq = NULL;
        }
    }

    return pq;
}

// the destructor for a priority queue. Make sure to set the pointer to NULL after freeing the memory for the pq
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->nodes) {
        free((*q)->nodes);
        free(*q);
        *q = NULL;
    }
    return;
}

// return true if the priority queue is empty
bool pq_empty(PriorityQueue *q) {
    return !q->size;
}

// return true if the priority queue is full
bool pq_full(PriorityQueue *q) {
    return next(q->end, q->capacity) == q->begin;
}

// returns the size of the priority queue
uint32_t pq_size(PriorityQueue *q){
    return q->size;
}

// Enqueues a node into the priority queue.
// Returns false if the priority queue if full prior to adding to the queue
// otherwise true to indicate that enqueuing was successful
// use selection sort 
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false; // check to see if the queue is empty 
    }
    
    if (pq_empty(q)) {
        q->nodes[q->end] = n;
        q->end = next(q->end, q->capacity);
        q->size++;
    } else {
        uint32_t pos = q->end;
        while (true) {
            if (pos == q->begin) {
                // insert the node at the beginning of the queue
                q->nodes[pos] = n;
                q->end = next(q->end, q->capacity);
                q->size++;
                break;
            } else if (q->nodes[prev(pos, q->capacity)]->frequency > n->frequency) {
                q->nodes[pos] = q->nodes[prev(pos, q->capacity)];
                pos = prev(pos, q->capacity);
            } else {
                // move the current node to the next position 
                q->nodes[pos] = n;
                q->end = next(q->end, q->capacity);
                q->size++;
                break;
            }
        }
    }
    return true;
}

// the nodes dequeued should have the highest priority over all nodes in the priority queue
// return false if the priority queue is empty prior to dequeuing a node
// return true if the dequeuing was successful
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false; // queue is full
    }
    *n = q->nodes[q->begin];
    q->begin = next(q->begin, q->capacity);
    q->size--;
    return true;
}

// loop through the queue and print the elements
void pq_print(PriorityQueue *q) {
    uint32_t i = q->begin;
    while (i != q->end) {
        node_print(q->nodes[i]);
        
        i = next(i, q->capacity);
    }
}

















