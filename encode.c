#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


// header files
#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"

#define OPTIONS "hvi:o:"

uint64_t bytes_read = 0, bytes_written = 0;
uint8_t post_array[BLOCK] = { 0 };

// print the help message
void print_help_message(void){
fprintf(stderr,  
    "SYNOPSIS\n"
    "  A Huffman encoder.\n"
    "  Compresses a file using the Huffman coding algorithm.\n"
    "\n"
    "USAGE\n"
    "  ./encode [-h] [-i infile] [-o outfile]\n"
    "\n"
    "OPTIONS\n"
    "  -h             Program usage and help.\n"
    "  -v             Print compression statistics.\n"
    "  -i infile      Input file to compress.\n"
    "  -o outfile     Output of compressed data.\n");    
       return;
}

void making_trees(Node *root);

// main function to encode the string/input  for the input file
int main(int argc, char *argv[]) {

    // keep track and save the input and outfile files names
    char *in_filename = malloc(ALPHABET * sizeof(char));
    char *out_filename = malloc(ALPHABET * sizeof(char));

    // var for the output and input files, stdin and stdout for the default values
    int infile = STDIN_FILENO; // 0
    int outfile = STDOUT_FILENO; // 1
    
    
    bool stat_flag = false;
    int opt = 0;
    struct stat symbol;

    // parse through the command line
    while ((opt = getopt(argc, argv, "hvi:o:")) != -1 ) {
        switch (opt) {
            case 'h':  // helper message
                print_help_message();
                free(in_filename);
            free(out_filename);
                return 0;
            case 'i':
                in_filename = strdup(optarg);
                if (in_filename){
                    infile = open(in_filename, O_RDONLY);  // O_RDONLY is read only
                    
                    if (infile == -1) {
                        //fprintf(stderr, "File does not exist or can't read the content from file.\n");
                        
                        free(in_filename);
                        free(out_filename);
                        return -1;
                    }
                    // grab the stats for the infile
                    fstat(infile, &symbol);
                }
                break;
            case 'o':
                out_filename = strdup(optarg);
                if (out_filename){
                
                    /*
                    souce of information  https://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html
                    O_WRONLY - write only
                    O_CREAT  - if the the files being opened doesn't exit it is created
                    O_TRUNC  - causes the file to be truncated (shorted) if it exits          
                    */
                    
                    outfile = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, symbol.st_mode);
                    // grab the stats for the change mod
                    fchmod(outfile, symbol.st_mode);
                }
                break;
            case 'v':
                stat_flag = true;
                break;
            default:
                print_help_message();
                return -1;
            }
    }
    
    // buffer to read/write the symbols to/from the files
    uint8_t buffer[BLOCK] = { 0 };
    
    // histogram of symbols from the input file
    uint64_t histogram[ALPHABET] = { 0 };
    
    // initialzie the histogram values to 0 and increment the index 0 and 255
    // the input which is 0-255 corresponds to a the ASCII character
    histogram[0]++;
    histogram[255]++;
    
    int unit_bytes;

    while ((unit_bytes = read_bytes(infile, buffer, BLOCK)) != 0) {
        for (int i = 0; i < unit_bytes; i++) {
            histogram[buffer[i]]++;
        }
    }
    // for stats 
    uint64_t origin_size = bytes_read;

    // histogram[i] > 0 means the i-th ascii is the leaf
    uint32_t leaf_num = 0;
    for (int i = 0; i < ALPHABET; i++) {
        if (histogram[i]) {
            leaf_num++;
        }
    }

    // build the tree with the histogram
    Node *root = build_tree(histogram);

    // add the info into the code_table
    Code code_table[ALPHABET] = { 0 };
    build_codes(root, code_table);

    // make the head with the leaf numbers and bytes
    Header header;
    header.magic = MAGIC;
    header.permissions = symbol.st_mode;
    header.tree_size = (3 * leaf_num) - 1;
    header.file_size = bytes_read;

    // write the header to the out file
    write_bytes(outfile, (uint8_t *) &header, sizeof(header));

    making_trees(root); // same as dump_tree
    
    write_bytes(outfile, post_array, header.tree_size);

    // lseek is a system call that is used to change the location of the read/write pointer of a file
    lseek(infile, 0, 0);
    unit_bytes = 0;
    
    while (true) {
        if (!(unit_bytes = read_bytes(infile, buffer, BLOCK))){
            break;
        }
        
        for (int i = 0; i < unit_bytes; i++) {
            Code *c = &code_table[buffer[i]];
            write_code(outfile, c);
        }
    }
    flush_codes(outfile);

    // stats information
    if (stat_flag) {
        fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes \n", origin_size);
        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes \n", bytes_written);
        fprintf(stderr, "Space saving:  %.2f%% \n", 100.0 * (1.0 - (bytes_written / (double) origin_size)));
    }

    // free all the used memory
    delete_tree(&root);
    free(in_filename);
    free(out_filename);
    close(infile);
    close(outfile);
    return 0;
}

// make/build the huffman tree to the post_array global var
// root the node to be used to make the tree

/*
This function is a recursive function that traverses a Huffman tree and builds 
an array that represents the tree in postorder. It takes a pointer to the root Node of the tree as an argument.
The function has a static local variable index that keeps track of the next index in the array where a value 
should be stored. If the Node passed as an argument is not a leaf node 
(iff the left and right chilren aren't NULL), the function recursively calls itself on the left and right children 
of the Node. If the Node is a leaf node, the function stores the value 76 (the ASCII code for the letter 'L') in 
the array, followed by the symbol associated with the Node. The function then returns. This function is used to 
serialize a Huffman tree for storage in a file.
*/

void making_trees(Node *root) {
    static uint32_t index;
    // if the left or right aren't NULL make/build the tree
    if (root->left != NULL || root->right != NULL) {
        making_trees(root->left);
        making_trees(root->right);

        post_array[index++] = 73; // capital "I" in ASCIi
        
        return;
    } else {
        post_array[index++] = 76; // capital "L" in ASCII
        post_array[index++] = root->symbol;
        
        return;
    }
}





