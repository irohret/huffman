#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// header files
#include "code.h"

/*
 Ternary Operator:
 (condition) ? (if true, do this) : (otherwise, do this);
 */

/*
 code found in defines.h which is given to us (these are macros fined in deines.h)
 
 #define BLOCK         4096                 // 4KB blocks.
 #define ALPHABET      256                  // ASCII + Extended ASCII.
 #define MAGIC         0xBEEFBBAD           // 32-bit magic number.
 #define MAX_CODE_SIZE (ALPHABET / 8)       // Bytes for a maximum, 256-bit code.
 #define MAX_TREE_SIZE (3 * ALPHABET - 1)   // Maximum Huffman tree dump size.
 */


// initializing the new Code
Code code_init(void) {
    Code tmp;  
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        tmp.bits[i] = 0;
    }
    
    tmp.top = 0;
    return tmp;
}

// check the size of the code and return that value
uint32_t code_size(Code *c) {
    return c->top;
}

// check if the code is empty and return that value
bool code_empty(Code *c) {
    return !(c->top);
}

// check if the code is full and return that value
bool code_full(Code *c) {
    return c->top == ALPHABET;
}

// using a bit vector and the bit index set the particular bit
// check to see if the bit is in range or not
// if it is return false else true
// get the i-th bit of teh code
bool code_set_bit(Code *c, uint32_t i) {
    c->bits[i / 8] |= 0x1 << (i % 8);
    
    if ((uint32_t)(i / 8) >= c->top){
        return false;
    }
    return true;
    
    
    //((uint32_t)(i / 8) >= c->top) ? (return false) : (return true);
}


// clear the bits from the bit vector at index i in the code
// if i out of range return false else true
// clear the i-th bit of the code
bool code_clr_bit(Code *c, uint32_t i) {
    c->bits[i / 8] &= ~(0x1 << (i % 8));
    
    if ((uint32_t)(i / 8) >= c->top){
        return false;
    }
    return true;
     
    // ((uint32_t)(i / 8) >= c->top) ? (return false) : (return true);
}

// get the index i of the code
// if i is out of range or is equal to zero return false
bool code_get_bit(Code *c, uint32_t i) {
    return 1 & (c->bits[i / 8] >> (i % 8));
}


// push a bit into the code
// value of bit is given by bit
// return false if the code is full prior top pushing a bit else true
bool code_push_bit(Code *c, uint8_t bit) {
    // check to see if the code is full
    if (code_full(c)) {
        return false;
    } else {
        if (bit) {
            code_set_bit(c, c->top);
        } else {
            code_clr_bit(c, c->top);
        }
        c->top++;
        return true;
    }
}

// remove a bit from the code
// the value popped is passed back with the pointer
// return false if the code is empty prior to popping a bit else true
bool code_pop_bit(Code *c, uint8_t *bit) {
    // check if the code is empty
    if (code_empty(c)) {
        return false;
    } else {
        *bit = code_get_bit(c, c->top - 1);
        c->top--;
        return true;
    }
}


// print the popped and pushed bits form the code
void code_print(Code *c) {
    // check to see if the code is empty
    if (code_empty(c)) {
        return;
    } else {
        for (int i = 0; i < (int) c->top; i++) {
            printf("%c", '0' + code_get_bit(c, i));
        }
    }
    printf("\n");

    return;
}

/*
int main(){

   printf(" --------- code test --------\n" );
   
   //will result with an output of 1101 far right being the top -- this is what I get 
   //will result with an output of 1011 far left being the top
    
   Code bit = code_init();
 
   code_push_bit(&bit, 1);
   code_push_bit(&bit, 1);
   code_push_bit(&bit, 0);
   code_push_bit(&bit, 1);
   
   code_print(&bit);
   //printf("popped values\n");
   
   //code_pop_bit(&bit, 1);
   //code_pop_bit(&bit, 0);
   //code_print(&bit);
    
   return 0;
}
*/

