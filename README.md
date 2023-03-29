
In order to run this program you need to use $ Make in order to build the program.
After building the program you need to specify which test you want to run on the Command Line using the Command Line arguments.

Encryptor:

-h: Prints out a help message describing the purpose of the program and the command-line op-
tions it accepts, exiting the program afterwards. Refer to the reference program provided to you,
for an idea of what to print.\
-i infile: Specifies the input file to encode using Huffman coding. The default input should be
set as stdin.\
-o outfile: Specifies the output file to write the compressed input to. The default output should
be set as stdout.\
-v: Prints compression statistics to stderr. These statistics include the uncompressed file size,
the compressed file size, and space saving.

Decryptor:

-h: Prints out a help message describing the purpose of the program and the command-line op-
tions it accepts, exiting the program afterwards. Refer to the reference program provided to you,
for an idea of what to print.\
-i infile: Specifies the input file to decode using Huffman coding. The default input should be
set as stdin.\
-o outfile: Specifies the output file to write the decompressed input to. The default output
should be set as stdout.\
-v: Prints decompression statistics to stderr. These statistics include the compressed file size,
the decompressed file size, and space saving.

$ ./encode


Example:\
$ Make clean\
$ Make\
$ ./encode

Included files:
 * encode.c - This file will contain your implementation of the Huffman encoder.
 * decode.c -This file will contain your implementation of the Huffman decoder.
 * defines.h - This file will contain the macro definitions used throughout the assignment.
 * header.h - This will will contain the struct definition for a file header.
 * node.h - This file will contain the node ADT interface. 
 * node.c - This file will contain your implementation of the node ADT.
 * pq.h - This file will contain the priority queue ADT interface. 
 * pq.c - This file will contain your implementation of the priority queue ADT.
 * code.h - This file will contain the code ADT interface.
 * code.c - This file will contain your implementation of the code ADT.
 * io.h - This file will contain the I/O module interface.
 * io.c - This file will contain your implementation of the I/O module.
 * stack.h - This file will contain the stack ADT interface.
 * stack.c - This file will contain your implementaion of the stack ADT.
 * huffman.h - This file will contain a Huffman coding modules interface.
 * huffman.c - This file will contain your implementation of the Huffman coding module interface.

Makefile - Contains all command to compile and clean the files
 * CC = clang must be specified.
 * CFLAGS = -Wall -Wextra -Werror -Wpedantic must be included.
 * make should build the encoder and the decoder, as should make all.
 * make encode should build just the encoder.
 * make decode should build just the decoder.
 * make clean must remove all files that are compiler generated.
 * make spotless must remove all files that are compiler generated as well as the executable.
 * make format should format all your source code, including the header files. 
  
  
  
  
README.md
 * Contains information about all the files in the program 
 
DESIGN.pdf
 * Contains information about my design process
