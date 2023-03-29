#include <inttypes.h>
#include <stdlib.h>

// header files
#include "stack.h"

// create the struct for the stack
struct Stack {
    uint32_t top;  // tracks the position of the next item to be added to the stack
    uint32_t capacity; // the maximum number of items that can be stored in the stack
    Node **nodes; // a pointer to an array of pointers to nodes
};

// initialize the stack
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s){
        s->top = 0; // set the top to 0
        s->capacity = capacity; // set the capacity to the given capacity 
        // allocate memoery for the arry of pointers to nodes 
        s->nodes = (Node **) calloc(s->capacity, sizeof(Node *));
        // check to see if the array of pointer was created successfully 
        if (!s->nodes) {
            free(s);  // free the memory used for the stack 
            s = NULL;  // set the stack to NULL
        }
    }
    // return the stack 
    return s;
}

// delete the items from the stack and set it (the pointer) to NULL
void stack_delete(Stack **s) {
     // check to see if the stack and the array of pointers exists
    if (*s && (*s)->nodes) {
        free((*s)->nodes); // free the array of pointers
        free(*s); // free the memory used for the stack
        *s = NULL; // set the stack to NULL
    }
    return;
}


// check to see if the stack is empty iff return true
bool stack_empty(Stack *s) {
    return !s->top;
}


// check to see if the stack is full iff return true
bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

// checks the size of the stack and returns that number
uint32_t stack_size(Stack *s) {
    return s->top;
}


// add an items to the stack 
bool stack_push(Stack *s, Node *n) {
    // check to see if the stack is full 
    if (stack_full(s)) {
        return false;
    }
    
    s->nodes[s->top] = n;
    s->top++;
    return true;
}

// remove and item from the stack 
// return the item that was removed from the stack 
bool stack_pop(Stack *s, Node **n) {
    // check to see if the stack is empty
    if (stack_empty(s)) {
        return false;
    }
    
    *n = s->nodes[s->top - 1];
    s->top--;
    return true;
}

// print the values in the stack 
void stack_print(Stack *s) {
    int32_t i = s->top - 1;
    while (i >= 0) {
        node_print(s->nodes[i]);
        i -= 1;
    }
    return;
}

