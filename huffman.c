#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>


// header files
#include "huffman.h"
#include "pq.h"
#include "stack.h"
#include "io.h" // needed for write/read byte


// build a huffman tree
// histogram will have ALPHABET indices, one index for each possible symbol
// it informs the compiler the histogram (hist) has at least ALPHABET numbers of indices
Node *build_tree(uint64_t hist[static ALPHABET]) {
    Node *n, *root;                                     // n and root node
    PriorityQueue *pq = pq_create(ALPHABET);            // create empty queue

    // loop through the alphabet
    for (int i = 0; i < ALPHABET; i++) {
        // create a node that holds a character and enqueue that value
        // check if there is a histogram entry in the first place then create anode for the character and enqueue it
        if (hist[i]) {
            n = node_create((uint8_t) i, hist[i]);
            enqueue(pq, n);
        }
    }
    
    // dequeue the left and right sides and enqueue the joined nodes
    // the last node should be the left node
    while (pq_size(pq) > 1) {
        
        Node *l, *r;                                 // left and right sides of the tree
        dequeue(pq, &l);
        dequeue(pq, &r);

        Node *joined = node_join(l, r);             // join the two sides x = (L+R)
        enqueue(pq, joined);                        // enqueue the joined values
    }

    dequeue(pq, &root);
    pq_delete(&pq);

    return root;
}

// build the tree (code) give there is a root node and a table of code for it
// populates a code table, building the code for each symbol in the Huffman tree. The constructed code are copied to the code table, table, which has ALPHABET indices, one index for each possible symbol
void build_codes(Node *root, Code table[static ALPHABET]) {
    static Code c_code; // for the table
    
    // recursively call the left and right sides of the tree
    // push and pop the bits to and from the table
    // check if there is a root
    // while at the leaf node, set the index of symbol for the code
    if (root->left == NULL && root->right == NULL) {
        table[root->symbol] = c_code;
        return;
        
    } else {
        uint8_t popped_bit;

        // recursively call the left side. (in-order traversal)
        // left is represented by a 0 and the right is represented by a 1
        code_push_bit(&c_code, 0);              // left = 0
        build_codes(root->left, table);
        code_pop_bit(&c_code, &popped_bit);
      
        // recursively call the right side. (in-order traversal)
        // left is represented by a 0 and the right is represented by a 1
        code_push_bit(&c_code, 1);              // right = 1
        build_codes(root->right, table);
        code_pop_bit(&c_code, &popped_bit);
        return;
    }
}

void dump_tree(int outfile, Node *root) {
    if (root) {
        uint8_t leaf = 76;                 // ASCII for capital L or 'L'
        uint8_t p_node = 73;                 // ASCII for capital I or 'I'
        
        if (root->left == NULL && root->right == NULL) {
            write_bytes(outfile, &leaf, 1);
            write_bytes(outfile, &root->symbol, 1);
            
        } else {
            dump_tree(outfile, root->left);
            dump_tree(outfile, root->right);
            write_bytes(outfile, &p_node, 1);
        }
    }
}


// use tree_dump instead of tree
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(nbytes);

    for (int i = 0; i < nbytes; i++) {

        if (tree[i] == 76) {
            Node *n = node_create(tree[++i], 1);
            stack_push(s, n);
        } else if (tree[i] == 73){
            Node *r, *l;
            stack_pop(s, &r);
            stack_pop(s, &l);

            Node *joined = node_join(l, r);
            stack_push(s, joined);
        }
    }

    Node *root;
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}   


/*
//void dump_tree(int outfile, Node *root);

// use tree_dump instead of tree
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(nbytes);

    for (int i = 0; i < nbytes; i++) {

        if (tree[i] == 76) {                            // ASCII for capital L or 'L'
            Node *n = node_create(tree[++i], 1);
            stack_push(s, n);
            
        } else if (tree[i] == 73) {                     // ASCII for Capital I or 'I'
            Node *r, *l;                                // left and right side of the tree
            stack_pop(s, &r);
            stack_pop(s, &l);

            Node *joined = node_join(l, r);             // join the two sides x = (L+R)
            stack_push(s, joined);
        }
    }

    Node *root;
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}

*/

// delete the node on the tree
void delete_tree(Node **root) {
    if (*root) {
        if ((*root)->left != NULL){
            delete_tree(&((*root)->left));          // left side of the tree
        }
        
        if ((*root)->right != NULL){
            delete_tree(&((*root)->right));         // right side of the tree
        }
        
        node_delete(&(*root));
        return;
    }
}



