#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cache.h"

int main() {
    struct MemoryRegion memory;
    memory.size = 2 << 14;
    uint8_t buf[memory.size];
    memory.buffer = buf;

    struct CacheConfig config;
    config.cache_size = 1024;
    config.cache_blk_size = 64;
    config.associativity = 1;
    config.write_strat = WriteThrough;
    config.memory_addr_len = 14;

    struct SetAssociativeCache cache = new_cache_backed_by(config, &memory);

    for (int i = 0; i < memory.size; i += 4) {
        *( (uint32_t *) &memory.buffer[i]) = (uint32_t)(i / 4);
        if (i < 10) {
            printf("word %i is %i, ", i, (uint32_t)memory.buffer[i]);
        }
    }
    printf("...\n");

    int addrs[] = {0, 0, 60, 64, 1000, 1028, 12920, 12924, 12928};
    struct AccessMetadata accesses[9];
    for (int i = 0; i < 9; i++) {
        int addr = addrs[i];
        uint32_t value;

        int err = read(&cache, addr, &value);
        assert(err == 0);

        accesses[i] = cache.last_access;
        printf("%s\n", to_string(&accesses[i]));
    }

}
