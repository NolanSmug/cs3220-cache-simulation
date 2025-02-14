#pragma once

#include <stdint.h>
#include "cache.h"

struct CliArgs {
    struct CacheConfig cache_config;
    uint32_t memory_size;
};

int parse_cli(int argc, char* argv[], struct CliArgs *args);

