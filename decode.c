
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <unistd.h>

// header files
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"

// global var to get the size of the read and write file
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

// print the help message
void print_help_message(void){
    fprintf(stderr,    
        "SYNOPSIS\n"
        "  A Huffman decoder.\n"
        "  Decompresses a file using the Huffman coding algorithm.\n"
        "\n"
        "USAGE\n"
        "  ./decode [-h] [-i infile] [-o outfile]\n"
        "\n"
        "OPTIONS\n"
        "  -h             Program usage and help.\n"
        "  -v             Print compression statistics.\n"
        "  -i infile      Input file to decompress.\n"
        "  -o outfile     Output of decompressed data.\n");        
        return;
}

int main(int argc, char *argv[]) {
    char *in_filename =  malloc(ALPHABET * sizeof(char));
    char *out_filename =  malloc(ALPHABET * sizeof(char));
    
    // var to save the input and output file name
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    
    // used to keep track of the stats for the program
    bool stat_flag = false;
    int opt = 0;
    struct stat symbol;

    // parse through the command options
    while ((opt = getopt(argc, argv, "hvi:o:")) != -1){
        switch (opt) {
            case 'h':
                print_help_message();
                free(in_filename);
                free(out_filename);
                return 0;
                     
            case 'i':
                in_filename = strdup(optarg);
                if (in_filename){
                    infile = open(optarg, O_RDONLY);
                    
                    if (infile == -1) {
                        //fprintf(stderr, "File does not exist or can't read the content from file.\n");
                        free(in_filename);
                        free(out_filename);
                        return -1;
                    }
                    // read the stats from the infile
                    fstat(infile, &symbol);
                }
                break;
            case 'o':
                out_filename = strdup(optarg);
                if (out_filename) {
                
                    /*
                    souce of information  https://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html
                    O_WRONLY - write only
                    O_CREAT  - if the the files being opened doesn't exit it is created
                    O_TRUNC  - causes the file to be truncated (shorted) if it exits          
                    */                
                
                    outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, symbol.st_mode);
                    // read the stats from change mod
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

    // read the header file
    Header header;
    read_bytes(infile, (uint8_t *) &header, sizeof(Header));

    // check whether header is part of the huffman compression and has a Magic number
    if (header.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number.\n");
        free(in_filename);
        free(out_filename);
        return -1;
        
    }

    // get the tree dump to reconstruct the tree
    uint8_t *tree_dump = (uint8_t *) calloc(header.tree_size, sizeof(uint8_t));
    read_bytes(infile, tree_dump, header.tree_size);

    Node *root = rebuild_tree(header.tree_size, tree_dump);

    // get the buffer to save the symbols
    uint8_t *out_buf = (uint8_t *) calloc(header.file_size, sizeof(uint8_t));

    uint8_t bit;
    Node *curr_node = root;   // keep track of the current node
    uint32_t symbols_written = 0;
    
    while (true) {
        read_bit(infile, &bit);

    // read each bit from the file
        if (!bit) {
            curr_node = curr_node->left;
        } else {
            curr_node = curr_node->right;
        }

    // if it reaches the leaf node then save the current node's symbols to the buffer
    // add the symbol to the buffer
        if ((curr_node->left == NULL) || (curr_node->right == NULL)) {
            out_buf[symbols_written] = curr_node->symbol;
            symbols_written++;
            curr_node = root;
        }
    
    // exit the loop when the written symbol's size reaches the header file size
        if (symbols_written == header.file_size) {
            break;
        }
    }
    
    // output the reconstructed tree in the output file
    write_bytes(outfile, out_buf, header.file_size);

    // print the stats
    if (stat_flag) {
        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes \n", bytes_read);
        fprintf(stderr, "Decompressed file size: %" PRIu64 " bytes \n", header.file_size);
        fprintf(stderr, "Space saving:  %.2f%% \n", 100.0 * (1.0 - (bytes_read / (double) header.file_size)));
    }

    // free all the memory used and return 0
    delete_tree(&root);
    free(tree_dump);
    free(out_buf);
    free(in_filename);
    free(out_filename);
    
    return 0;
}




