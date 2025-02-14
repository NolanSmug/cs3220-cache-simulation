// PA2 Simulation of a Hardware Cache
// Authors: Nolan Cyr and Eric Edlund

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cache.h"
#include "cli.h"


int main(int argc, char* argv[]) {
    struct CliArgs args;
    parse_cli(argc, argv, &args);

    struct MemoryRegion mem;
    mem.size = args.memory_size;
    mem.buffer = malloc(sizeof(uint8_t) * args.memory_size);

    struct SetAssociativeCache cache = new_cache_backed_by(args.cache_config, &mem);

    free(mem.buffer);
}
