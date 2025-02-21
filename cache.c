#include "cache.h"
#include "math.h"

struct SetAssociativeCache new_cache_backed_by(struct CacheConfig config, struct MemoryRegion *const backing) {
    struct SetAssociativeCache cache;
    cache.config = config;
    cache.inner = backing;
    cache.last_access.addr = 0;
    cache.last_access.index = 0;
    
    // calculate number of blocks
    int num_blocks = config.cache_size / config.cache_blk_size;
    
    // allocate array size based on number of blocks
    cache.blocks = malloc(num_blocks * sizeof(struct CacheBlock));
    
    // initialize each block (empty/invalid)
    for(int i = 0; i < num_blocks; i++) {
        cache.blocks[i].tag = -1;
        cache.blocks[i].value = malloc(config.cache_blk_size);
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
    int num_sets = (config.cache_size / config.cache_blk_size) / config.associativity;
    
    bits.offset_bits = log2(config.cache_blk_size);
    bits.index_bits = log2(num_sets);
    bits.tag_bits = config.memory_addr_len - bits.offset_bits - bits.index_bits;
    
    return bits;
}

int read(struct SetAssociativeCache *const cache, uint32_t addr, Word *const target) {
    struct BlockBits bits = get_block_bits(cache->config);
    uint32_t block_size = cache->config.cache_blk_size;
    
    // calculate addr values
    uint32_t block_offset = addr & ((1 << bits.offset_bits) - 1);
    uint32_t index = (addr >> bits.offset_bits) & ((1 << bits.index_bits) - 1);
    uint32_t tag = addr >> (bits.offset_bits + bits.index_bits);

    struct CacheBlock *block = &cache->blocks[index];
    
    // calculate memory range where block is located
    uint32_t block_start = addr - block_offset;
    uint32_t block_end = block_start + block_size - 1;

    cache->last_access.addr = addr;
    cache->last_access.index = index;
    cache->last_access.tag = tag;
    // TODO: put these in last_access?
    // cache->last_access.block_start = block_start;
    // cache->last_access.block_end = block_end;

    //TODO: cache miss or cache hit logic:
    

    
    return 0; 
}

int write(struct SetAssociativeCache *const cache, uint32_t addr, const Word *const value) {
    return 0;
}
