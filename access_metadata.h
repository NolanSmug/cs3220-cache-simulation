#pragma once

#include <stdint.h>
#include <stdbool.h> // Crazy


struct AccessMetadata {
	uint32_t addr;
	uint32_t index;
	uint32_t blk_index;
	uint32_t tag;
	uint32_t word;
	bool cache_hit;
};

// Creates string matching the expected output format.
//
// 		read miss [addr=0 index=0 block_index=0 tag=0: word=0 (0 - 63)]
// 		
char* to_string(const struct AccessMetadata *const data);
