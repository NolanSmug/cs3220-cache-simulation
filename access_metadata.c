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


static char* queue_string(const struct AccessMetadata *const data) {
    char *buf = malloc(sizeof(char) * 100);
    int j=0;
    j += sprintf(&buf[j], "[");
    j += sprintf(&buf[j], "%d", data->lru_queue[0]);
    for (int i=1; i<data->set_size; i++) {
        j += sprintf(&buf[j], ", %d", data->lru_queue[i]);
    }
    j += sprintf(&buf[j], "]");

    return buf;
}


char* to_string(const struct AccessMetadata * const data) {
    char *buf = malloc(sizeof(char) * 300);
    char *addr_bits_str = addr_bits(data->addr, data->addr_len);
    char *eviction_str = malloc(sizeof(char) * 100);
    char *queue = queue_string(data);
    if (data->eviction) {
        sprintf(eviction_str, "\nevict tag %i in block_index %i", data->evicted_tag, data->blk_index);
    }
    sprintf(buf, "read %s [addr=%i index=%i block_index=%i tag=%i: word=%i (%i - %i)\n=> address = %i <%s>; word = %i\n%s\n%s"
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
            , queue
            , eviction_str
            );
    if (data->eviction) {
        free(eviction_str);
    }
    free(addr_bits_str);
    free(queue);
    return buf;
}

// read miss [addr=0 index=0 block_index=0 tag=0: word=0 (0 - 63)]
// => address = 0 <0000000000000000>; word = 0
// evict tag 0 in block_index 0
