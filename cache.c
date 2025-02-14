#include "cache.h"

struct SetAssociativeCache new_cache_backed_by(struct CacheConfig config, struct MemoryRegion *const backing) {
    return (struct SetAssociativeCache){};
}

int read(struct SetAssociativeCache *const cache, uint32_t addr, Word *const target) {
    return 0;
}

int write(struct SetAssociativeCache *const cache, uint32_t addr, const Word *const value) {
    return 0;
}
