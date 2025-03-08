#pragma once

#include <stdbool.h> // Crazy
#include <stdint.h>

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
  bool write_back;
  uint32_t wb_start;
  uint32_t wb_end;
  uint32_t set_size;
  uint32_t *lru_queue;
};

// Creates string matching the expected output format.
char *to_string(const struct AccessMetadata *const data);
