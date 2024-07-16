#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitstream.h"

struct BitStream {
	char* bitstream;
	size_t max_size_bytes;
	size_t cur_byte;
	int cur_bit;
};

BitStream* create_bitstream(size_t count_bytes){
	BitStream* bitstream = (BitStream*)malloc(sizeof(BitStream));
	if (!bitstream) return NULL;

	bitstream->bitstream = (char*)calloc(count_bytes, sizeof(char));
	if (!bitstream->bitstream){
		free(bitstream);
		return NULL;
	}

	bitstream->max_size_bytes = count_bytes;
	bitstream->cur_byte = 0;
	bitstream->cur_bit = 0;

	return bitstream;
}
size_t print_bitstream(const BitStream* bitstream, FILE* fin){
	if (!fin || !bitstream || (!bitstream->cur_byte && !bitstream->cur_bit)) return 0;

	return fwrite(bitstream->bitstream, sizeof(char), bitstream->cur_byte + (bitstream->cur_bit ? 1 : 0), fin);
}
static void set_null_bitstream(BitStream* bitstream){
	if (!bitstream) return;

	memset(bitstream->bitstream, 0, bitstream->max_size_bytes);
	bitstream->cur_byte = 0;
	bitstream->cur_bit = 0;
}
void set_next_bit_bitstream(BitStream* bitstream, int value){
	if (!bitstream) return;

	if (bitstream->cur_bit == CHAR_BIT){
		bitstream->cur_bit = 0;
		bitstream->cur_byte++;
	}

	if (bitstream->cur_byte == bitstream->max_size_bytes) set_null_bitstream(bitstream);

	bitstream->bitstream[bitstream->cur_byte] = bitstream->bitstream[bitstream->cur_byte] | ((value ? 1 : 0) << (CHAR_BIT - bitstream->cur_bit - 1));
	bitstream->cur_bit++;	
}
int get_bit(const BitStream* bitstream, size_t cur_bit){
	if (!bitstream) return -1;

	size_t cur_byte = (cur_bit / CHAR_BIT);
	if (cur_byte > bitstream->cur_byte) return -1;

	return  (bitstream->bitstream[cur_byte] >> (CHAR_BIT - (cur_bit % CHAR_BIT) - 1)) & 1;
}
size_t get_cur_size_bytes(const BitStream* bitstream){
	if (!bitstream) return 0;

	return bitstream->cur_byte + 1;
}
void del_bitstream(BitStream* bitstream){
	if (!bitstream) return;

	free(bitstream->bitstream);
	free(bitstream);
}

void set_byte(BitStream* bitstream, size_t cur_byte, char byte){
	if (!bitstream) return;

	if (cur_byte >= bitstream->max_size_bytes) return;
	bitstream->bitstream[cur_byte] = byte;
	bitstream->cur_byte = cur_byte;
}
