#include "cache.h"
#include "math.h"

struct SetAssociativeCache new_cache_backed_by(struct CacheConfig config, struct MemoryRegion *const backing) {
    return (struct SetAssociativeCache){};
}

struct BlockBits {
    int offset_bits;
    int index_bits;
    int tag_bits;
};

static struct BlockBits initialize_bit_lengths(struct CacheConfig config) {
    int num_sets = (config.cache_size / config.cache_blk_size) / config.associativity;
    
    int offset_bits = log2(config.cache_blk_size);
    int index_bits = log2(num_sets);
    int tag_bits = config.memory_addr_len - (offset_bits + index_bits);

    return (struct BlockBits) {
        offset_bits,
        index_bits,
        tag_bits
    };
}

int read(struct SetAssociativeCache *const cache, uint32_t addr, Word *const target) {
    return 0; 
}

int write(struct SetAssociativeCache *const cache, uint32_t addr, const Word *const value) {
    return 0;
}
