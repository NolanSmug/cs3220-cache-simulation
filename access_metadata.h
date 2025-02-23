#pragma once

#include <stdint.h>
#include <stdbool.h> // Crazy


struct AccessMetadata {
	uint32_t addr;
	uint32_t addr_len;
	uint32_t index;
	uint32_t blk_index;
	uint32_t tag;
	uint32_t word;
	bool cache_hit;
	uint32_t block_start;
	uint32_t block_end;
	bool eviction;
	uint32_t evicted_tag;
	uint32_t evicted_block_index;
	bool write_back;
	uint32_t wb_start;
	uint32_t wb_end;	
};

// Creates string matching the expected output format.
char* to_string(const struct AccessMetadata *const data);
