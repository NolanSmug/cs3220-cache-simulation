#include "access_metadata.h"
#include <stdio.h>
#include <stdlib.h>


char* to_string(const struct AccessMetadata * const data) {
    // This function has quadratic complexity
    char *buf = malloc(sizeof(char) * 100);
    sprintf(buf, "read %s [addr=%i index=%i block_index=%i tag=%i: word=%i (%i - %i)"
            , (data->cache_hit ? "hit" : "miss")
            , data->addr
            , data->index
            , data->blk_index
            , data->tag
            , data->word
            , 0 // TODO: What should these values be?
            , 63
            );
    return buf;
}
