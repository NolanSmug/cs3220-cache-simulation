#pragma once

#include "access_metadata.h"
#include <stdint.h>

typedef uint32_t Word;

struct MemoryRegion {
  uint32_t size; // In bytes
  uint8_t *buffer;
};

enum WriteStrategy {
  WriteBack,
  WriteThrough,
};

struct CacheConfig {
  uint32_t memory_addr_len;
  uint32_t cache_size;
  // Bytes
  uint32_t cache_blk_size;
  uint32_t associativity;
  enum WriteStrategy write_strat;
};

struct CacheBlock {
  int32_t tag;        
  bool valid;         
  bool dirty;        
  int8_t used_recency; 
  // Owned memory
  uint8_t *data;  
};

// Does not own the memory region.
struct SetAssociativeCache {
  struct CacheConfig config;
  struct MemoryRegion *inner;
  struct AccessMetadata last_access;
  struct CacheBlock *blocks;
  uint32_t num_blocks;
};

// Constructs and returns a set associative cache backed by the given memory
// region.
//
// Verifies that the given configuration is valid.
struct SetAssociativeCache
new_cache_backed_by(struct CacheConfig config,
                    struct MemoryRegion *const backing);

// Reads the given address from the cache. Updates the state of the cache if
// it's not a hit or whatever. Updates the last_access member.
//
// Returns non-zero if there was a problem during reading.
int read(struct SetAssociativeCache *const cache, uint32_t addr,
         Word *const target);

// Updates last_access member.
// Returns non-zero if there was a problem writing.
int write(struct SetAssociativeCache *const cache, uint32_t addr,
          const Word *const value);

