#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// header files
#include "node.h"

// basing node.c on my node.c from asgn6

// create a new note and initialize the symbols and frequency
// return the value of the node
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) calloc(1, sizeof(Node));
    // assert check to see if the condition is true. If the condition is true the program continues normally
    // if the condition is false, the program is terminated
    assert(n != NULL);
    if (n) {
        n->left = NULL;
        n->right = NULL;
        n->symbol = symbol;
        n->frequency = frequency;
    }
    return n;
}

// delete a node in the tree
void node_delete(Node **n) {
    if (*n) {
        (*n)->left = NULL;
        (*n)->right = NULL;
        free(*n);
        *n = NULL;
    }
    return;
}

// join the left and the right sides and return the newly created node
Node *node_join(Node *left, Node *right) {
    Node *n = node_create('$', left->frequency + right->frequency);
    n->left = left;
    n->right = right;
    return n;
}

// print the values (node) that have been created
// compare the two nodes
// n comparable node
// m nodes to be compared
// return true if two nodes are the same, otherwise false
void node_print(Node *n) {
    if (!n) {
        printf("NULL");
        return;
    } else {
        printf("(0x%02x, %" PRIu64 ")", n->symbol, n->frequency);
    }

    printf("\n   --> Left: ");
    // check the left side
    if (n->left) {
        printf("(0x%x, %" PRIu64 ")", n->left->symbol, n->left->frequency);
    } else {
        printf("NULL");
    }

    printf(" Right: ");
    // then the right
    if (n->right) {
        printf("(0x%x, %" PRIu64 ")", n->right->symbol, n->right->frequency);
    } else {
        printf("NULL");
    }
    
    printf("\n");

    return;
}

// compares the frequency of two nodes
// return true if the first node has a greater frequency than the second
bool node_cmp(Node *n, Node *m){
    return n->frequency == m->frequency;
}

// prints the symbols associated with a node
// biasing this code on the normal node_print
void node_print_sym(Node *n) {
    if (!n) {
        printf("NULL");
        return;
    } else {
        printf("(0x%02x, %" PRIu64 ")", n->symbol, n->frequency);
    }

    printf("\n   --> Left: ");

    if (n->left) {
        printf("(0x%x, %" PRIu64 ")", n->left->symbol, n->left->frequency);
    } else {
        printf("NULL");
    }

    printf(" Right: ");

    if (n->right) {
        printf("(0x%x, %" PRIu64 ")", n->right->symbol, n->right->frequency);
    } else {
        printf("NULL");
    }
    printf("\n");

    return;
}



