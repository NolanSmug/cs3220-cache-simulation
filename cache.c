#include "cache.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct AddrComponents {
  uint32_t tag;
  uint32_t index;
  uint32_t offset;
};

static struct AddrComponents decode_addr(struct CacheConfig const *const config,
                                         uint32_t addr) {
  struct AddrComponents result;
  const int addr_len = config->memory_addr_len;
  const int num_blocks = config->cache_size / config->cache_blk_size;
  const int num_sets = num_blocks / config->associativity;

  const int offset_bits = (int)log2(config->cache_blk_size);
  const int index_bits = (int)log2(num_sets);
  const int tag_bits = addr_len - offset_bits - index_bits;

  result.offset = (addr >> 0) & ((1 << offset_bits) - 1);
  result.index = (addr >> offset_bits) & ((1 << index_bits) - 1);
  result.tag = (addr >> (offset_bits + index_bits));
  return result;
}

struct SetAssociativeCache
new_cache_backed_by(struct CacheConfig config,
                    struct MemoryRegion *const backing) {
  struct SetAssociativeCache cache;
  cache.config = config;
  cache.inner = backing;
  cache.last_access.addr = 0;
  cache.last_access.index = 0;

  // calculate number of blocks
  const int num_blocks = config.cache_size / config.cache_blk_size;
  const int num_sets = num_blocks / config.associativity;

  // allocate array size based on number of blocks
  cache.blocks = malloc(num_blocks * sizeof(struct CacheBlock));

  // initialize each block (empty/invalid)
  for (int i = 0; i < num_blocks; i++) {
    cache.blocks[i].tag = -1;
    cache.blocks[i].valid = false; // initialize as invalid
    cache.blocks[i].dirty = false; // initialize as clean
    cache.blocks[i].used_recency = 0;
    cache.blocks[i].data = malloc(config.cache_blk_size); // allocate memory
  }

  return cache;
}

struct BlockBits {
  int offset_bits;
  int index_bits;
  int tag_bits;
};

static struct BlockBits get_block_bits(struct CacheConfig config) {
  struct BlockBits bits;
  int num_sets =
      (config.cache_size / config.cache_blk_size) / config.associativity;

  bits.offset_bits = log2(config.cache_blk_size);
  bits.index_bits = log2(num_sets);
  bits.tag_bits = config.memory_addr_len - bits.offset_bits - bits.index_bits;

  return bits;
}

Word *access_memory(struct SetAssociativeCache *const cache,
                                        uint32_t addr, uint32_t word,
                                        bool write) {
  struct BlockBits bits = get_block_bits(cache->config);
  const uint32_t block_size = cache->config.cache_blk_size;

  // calculate addr values
  const uint32_t block_offset = addr & ((1 << bits.offset_bits) - 1);
  const uint32_t index =
      (addr >> bits.offset_bits) & ((1 << bits.index_bits) - 1);
  const uint32_t tag = addr >> (bits.offset_bits + bits.index_bits);

  struct CacheBlock *set_start = &cache->blocks[index];

  // Is this block in the set?
  struct CacheBlock *blk_hit = NULL;
  int start_idx = index * cache->config.associativity; // index of first block
  for (int i = 0; i < cache->config.associativity; i++) {
    struct CacheBlock *blk = &cache->blocks[start_idx + i];
    if (blk->valid && blk->tag == tag) { // if valid AND tag matches
      blk_hit = blk;
      break;
    }
  }

  const uint32_t block_start = (addr / block_size) * block_size;
  const uint32_t block_end = (addr / block_size + 1) * block_size - 1;

  cache->last_access.cache_hit = (bool)blk_hit;
  cache->last_access.addr = addr;
  cache->last_access.index = index;
  cache->last_access.tag = tag;
  cache->last_access.block_start = block_start;
  cache->last_access.block_end = block_end;
  cache->last_access.addr_len = cache->config.memory_addr_len;

  if (!blk_hit) {
    // Pick lru
    struct CacheBlock *best = NULL;
    int best_recency = 127;
    for (int i = 0; i < cache->config.associativity; i++) {
      struct CacheBlock *blk = &cache->blocks[start_idx + i];
      if (!blk->valid) {
        best = blk;
        break;
      }
      if (blk->used_recency <= best_recency) {
        best = blk;
        best_recency = blk->used_recency;
      }
    }

    // Handle write back
    cache->last_access.write_back = false;
    if (best->valid && best->dirty && cache->config.write_strat == WriteBack) {
      uint32_t tag_part = best->tag << (bits.offset_bits + bits.index_bits);
      uint32_t index_part = index << bits.offset_bits;

      uint32_t evicted_addr =
          tag_part | index_part; // combine parts for evicted address

      // Write back the block
      memcpy(&cache->inner->buffer[evicted_addr], best->data,
             cache->config.cache_blk_size);

      cache->last_access.write_back = true;
      cache->last_access.wb_start = evicted_addr;
      cache->last_access.wb_end =
          evicted_addr + cache->config.cache_blk_size - 1;
    }
    

    // Update use recency
    for (int i = 0; i < cache->config.associativity; i++) {
      struct CacheBlock *blk = &cache->blocks[start_idx + i];
      if (blk == best) {
        blk->used_recency = 0;
      } else {
        blk->used_recency =
            blk->used_recency == 127 ? 127 : blk->used_recency + 1;
      }
    }
    blk_hit = best;

    // Record eviction
    if (best->valid) {
      cache->last_access.eviction = true;
      cache->last_access.evicted_tag = blk_hit->tag;
      cache->last_access.evicted_block_index =
          index % cache->config.associativity;
    } else {
      cache->last_access.eviction = false;
    }

    // Get starting address of the block in main memory
    uint8_t *source_block =
        &cache->inner->buffer[block_start]; // block start is technically block offset
    // Read memory into cache block
    memcpy(blk_hit->data, source_block, block_size);
    blk_hit->tag = tag;
    blk_hit->valid = true;
    blk_hit->dirty = false;
  }

  cache->last_access.blk_index = index % cache->config.associativity;

  if (write) {
    blk_hit->data[block_offset] = word;

    if (cache->config.write_strat == WriteThrough) {
      uint8_t *source_block =
          &cache->inner->buffer[block_start]; // block start is technically block offset
      memcpy(source_block, blk_hit->data, block_size);
    } else {
      blk_hit->dirty = true;
    }
  }

  return &((Word *)blk_hit->data)[block_offset];
}


int read(struct SetAssociativeCache *const cache, uint32_t addr,
         Word *const target) {

  *target = *access_memory(cache, addr, 0, false);

  return 0;
}


int write(struct SetAssociativeCache *const cache, uint32_t addr,
          const Word *const value) {
  if (addr % 4 != 0) {
    return 1;
  }

  access_memory(cache, addr, *value, true);

  return 0;
}
