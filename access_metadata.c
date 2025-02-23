#include "access_metadata.h"
#include <stdio.h>
#include <stdlib.h>


static char* addr_bits(uint32_t addr, uint32_t addr_len) {
    char *result = malloc(sizeof(char) * addr_len);
    for (int i = addr_len; i >= 0; i--) {
        result[addr_len - i] = (addr & (1 << i)) ? '1' : '0';
    }
    return result;
}


char* to_string(const struct AccessMetadata * const data) {
    char *buf = malloc(sizeof(char) * 200);
    char *addr_bits_str = addr_bits(data->addr, data->addr_len);
    char *eviction_str = malloc(sizeof(char) * 100);
    if (data->eviction) {
        sprintf(eviction_str, "\nevict tag %i in block_index %i", data->evicted_tag, data->evicted_block_index);
    }
    sprintf(buf, "read %s [addr=%i index=%i block_index=%i tag=%i: word=%i (%i - %i)\n=> address = %i <%s>; word = %i%s"
            , (data->cache_hit ? "hit" : "miss")
            , data->addr
            , data->index
            , data->blk_index
            , data->tag
            , data->word
            , data->block_start
            , data->block_end
            , data->addr
            , addr_bits_str
            , data->word
            , eviction_str
            );
    if (data->eviction) {
        free(eviction_str);
    }
    free(addr_bits_str);
    return buf;
}

// read miss [addr=0 index=0 block_index=0 tag=0: word=0 (0 - 63)]
// => address = 0 <0000000000000000>; word = 0
// evict tag 0 in block_index 0
