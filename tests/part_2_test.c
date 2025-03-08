#include <stdio.h>
#include <string.h>
#include "../cache.h"


static void log(struct SetAssociativeCache *const cache) {
    struct AccessMetadata accesses = cache->last_access;
    printf("%s\n\n", to_string(&accesses));
}

static void apply_test_1_accesses(struct SetAssociativeCache *const cache) {
    uint32_t word;
    
    read(cache,1152, &word);
    log(cache);
    read(cache,2176, &word);
    log(cache);
    read(cache,3200, &word);
    log(cache);
    read(cache,4224, &word);
    log(cache);
    read(cache,5248, &word);
    log(cache);
    read(cache,7296, &word);
    log(cache);
    read(cache,4224, &word);
    log(cache);
    read(cache,3200, &word);
    log(cache);
    word = 7;
    write(cache,7312, &word);
    log(cache);
    read(cache,7320, &word);
    log(cache);
    read(cache,4228, &word);
    log(cache);
    read(cache,3212, &word);
    log(cache);
    word = 5;
    write(cache,5248, &word);
    log(cache);
    read(cache,5248, &word);
    log(cache);
    word = 7;
    write(cache,8320, &word);
    log(cache);
    read(cache,8324, &word);
    log(cache);
    read(cache,9344, &word);
    log(cache);
    read(cache,11392, &word);
    log(cache);
    read(cache,16512, &word);
    log(cache);
    read(cache,17536, &word);
    log(cache);
    read(cache,8320, &word);
    log(cache);
    read(cache,17532, &word);
    log(cache);
}


static void apply_test_2_accesses(struct SetAssociativeCache *const cache) {
    uint32_t word;

    read(cache, 46916, &word);
    log(cache);
    read(cache, 46932, &word);
    log(cache);
    read(cache, 12936, &word);
    log(cache);
    read(cache, 13964, &word);
    log(cache);
    word = 40;
    write(cache, 46956, &word);
    log(cache);
    read(cache, 46956, &word);
    log(cache);
    read(cache, 56132, &word);
    log(cache);
}


static void apply_test_3_accesses(struct SetAssociativeCache *const cache) {
    uint32_t word;
    read(cache, 0, &word);
    log(cache);
    read(cache, 13432, &word);
    log(cache);
    read(cache, 13388, &word);
    log(cache);
    read(cache, 62540, &word);
    log(cache);
    read(cache, 4, &word);
    log(cache);
    read(cache, 16452, &word);
    log(cache);
    read(cache, 32900, &word);
    log(cache);
    read(cache, 49348, &word);
    log(cache);
    read(cache, 260, &word);
    log(cache);
    read(cache, 16708, &word);
    log(cache);
    read(cache, 33156, &word);
    log(cache);
    read(cache, 49604, &word);
    log(cache);
    read(cache, 516, &word);
    log(cache);
}


static void init_mem(struct MemoryRegion *memory) {
    memset(memory->buffer, 0, memory->size);
    for (int i = 0; i < memory->size; i += 4) {
        *( (uint32_t *) &memory->buffer[i]) = (uint32_t)(i / 4);
        if (i < 10) {
            printf("word %i is %i, ", i, (uint32_t)memory->buffer[i]);
        }
    }
    printf("...\n");
}


int main() {
    struct MemoryRegion memory;
    memory.size = 2 << 14;
    uint8_t buf[memory.size];
    memory.buffer = buf;

    struct CacheConfig config;
    config.cache_size = 1024;
    config.cache_blk_size = 64;
    config.associativity = 4;
    config.memory_addr_len = 16;
    struct SetAssociativeCache cache;

    init_mem(&memory);
    config.write_strat = WriteThrough;
    cache = new_cache_backed_by(config, &memory);
    printf("----- part-two-test-one-wt -----\n\n");
    apply_test_1_accesses(&cache);

    init_mem(&memory);
    config.write_strat = WriteBack;
    cache = new_cache_backed_by(config, &memory);
    printf("----- part-two-test-one-wb -----\n\n");
    apply_test_1_accesses(&cache);

    init_mem(&memory);
    config.write_strat = WriteThrough;
    cache = new_cache_backed_by(config, &memory);
    printf("----- part-two-test-two-wt -----\n\n");
    apply_test_2_accesses(&cache);

    init_mem(&memory);
    config.write_strat = WriteBack;
    cache = new_cache_backed_by(config, &memory);
    printf("----- part-two-test-two-wb -----\n\n");
    apply_test_2_accesses(&cache);

    init_mem(&memory);
    config.write_strat = WriteThrough;
    cache = new_cache_backed_by(config, &memory);
    printf("----- part-two-test-three-wt -----\n\n");
    apply_test_3_accesses(&cache);

    init_mem(&memory);
    config.write_strat = WriteBack;
    cache = new_cache_backed_by(config, &memory);
    printf("----- part-two-test-three-wb -----\n\n");
    apply_test_3_accesses(&cache);
}

