#ifndef _SRC_BITSTREAM_H
#define _SRC_BITSTREAM_H

#include <stddef.h>

typedef struct BitStream BitStream;

BitStream* create_bitstream(size_t count_bytes);
size_t print_bitstream(const BitStream* bitstream, FILE* fin);
void set_next_bit_bitstream(BitStream* bitstream, int value);
void del_bitstream(BitStream* bitstream);
int get_bit(const BitStream* bitstream, size_t cur_bit);
void set_byte(BitStream* bitstream, size_t cur_byte, char byte);
size_t get_cur_size_bytes(const BitStream* bitstream);

#endif //_SRC_BITSTREAM_H
