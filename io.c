#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "io.h"
#include "defines.h"
#include "code.h"

// buffer for storing bits
static uint8_t buffer[BLOCK] = { 0 };

// index to track which bit to return
static uint32_t bit_idx = 0;
 
// ------------------------------------------------------------------------------------------------------------- //

// for syscalls
// write a wrapper function to loop calls to read() until we have itehr read all the bytes that we specified(nbytes) into the buffer (buf), or there are no more bytes to read

// read bytes from infile and store them in buf
// nbytes specifies the number of bytes to read
// returns the total number of bytes read

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int total_bytes = 0, unit_bytes = 0;             // number of bytes

    for (;;) {                                     // while true
        // read bytes from infile and store them in buf + total_bytes
        unit_bytes = read(infile, buf + total_bytes, nbytes);
        if (unit_bytes <= 0) {
            break;
        }

        total_bytes += unit_bytes;
        bytes_read += unit_bytes;

        if (total_bytes == nbytes) {
            break;
        }
    }

    return total_bytes;
}


// for syscalls
// through until we have either written out all the bytes specified from the byte buffer (buf), or no bytes were written
// set to output file -> outfile
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int tot_bytes = 0;
    int unit_bytes = 0;

    for (;;) {                                       // while true
        unit_bytes = write(outfile, buf + tot_bytes, nbytes);
        if (unit_bytes <= 0) {
            break;
        }
        
        tot_bytes += unit_bytes;
        bytes_written += unit_bytes;

        if (tot_bytes == nbytes) {
            break;
        }
    }

    return tot_bytes;
}

// return one bit at a time through the pointer for bit
// you will maintain a static buffer of bytes and an index into the buffer that tracks which bit to return through the pointer bit

bool read_bit(int infile, uint8_t *bit) {
    if (!bit_idx) {
        // at the end return false
        // only read if we have finished the the buffer
        if (!read_bytes(infile, buffer, BLOCK)) {
            return false;
        }
    }

    *bit = 1 & (buffer[bit_idx / 8] >> (bit_idx % 8));
    bit_idx++;
    
    // check to see if the buffer is full iff right it
    if (bit_idx == (BLOCK * 8)) {
        bit_idx = 0;
    }

    return true;
}


void write_code(int outfile, Code *c) {
    uint32_t i = 0;
    while (i < code_size(c)) {              // check the size of the code (c->top)
        uint8_t bit = 1 & (c->bits[i / 8] >> (i % 8));

        if (!bit) {
            buffer[bit_idx / 8] &= ~(0x1 << (bit_idx % 8));
        }
        else {
            buffer[bit_idx / 8] |= 0x1 << (bit_idx % 8);
        }

        i++;
        bit_idx++;                          // increase the index of the bit
        
        // check to see if the buffer is full iff write it
        if (bit_idx >= 8 * BLOCK) {
            write_bytes(outfile, buffer, BLOCK);
            bit_idx = 0;  // reset the index
        }
    }

    return;
}

// in the case that we have leftover bits from the buffered used by write_code()
// we want to zero out the remainder of the bits that we found in the buffer
// also we need to make sure  that any extra bits in the last byte are zeroed out before flushing the codes
void flush_codes(int outfile) {
    uint32_t byte_num;
    if ((bit_idx - 1) % 8 == 0) {               //
        byte_num = (bit_idx - 1) / 8;
    } else {
        byte_num = ((bit_idx - 1) / 8) + 1;     // mask the bits that we want to preserve
    }
    
    bit_idx = 0; // zero the extra bits after we call encode

    write_bytes(outfile, buffer, byte_num);    
    
    return;
}








