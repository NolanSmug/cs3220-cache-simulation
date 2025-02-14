#pragma once

#include <stdint.h>
#include "access_metadata.h"

typedef uint8_t* NobodyKnows;
typedef uint32_t Word;

struct MemoryRegion {
	uint32_t size; // In bytes
	NobodyKnows* buffer;
};

enum WriteStrategy {
	WriteBack,
	WriteThrough,
};

struct CacheConfig {
	uint32_t memory_addr_len;
	uint32_t cache_size;
	uint32_t cache_blk_size;
	uint32_t associativity;
	enum WriteStrategy write_strat;
};

// Does not own the memory region.
struct SetAssociativeCache {
	struct CacheConfig config;
	struct MemoryRegion *inner;
	struct AccessMetadata last_access;
};

// Constructs and returns a set associative cache backed by the given memory region.
//
// Verifies that the given configuration is valid.
struct SetAssociativeCache new_cache_backed_by(struct CacheConfig config, struct MemoryRegion *const backing);

// Reads the given address from the cache. Updates the state of the cache if
// it's not a hit or whatever. Updates the last_access member.
//
// Returns non-zero if there was a problem during reading.
int read(struct SetAssociativeCache *const cache, uint32_t addr, Word *const target);

// Updates last_access member.
// Returns non-zero if there was a problem writing.
int write(struct SetAssociativeCache *const cache, uint32_t addr, const Word *const value);
