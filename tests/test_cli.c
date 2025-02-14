#include "../cli.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void expect(bool expr, char *msg, ...) {
    va_list args;
    if (!expr) {
        printf(msg, args);
        printf("\n");
        raise(SIGINT);
    }
}


void test_some_args() {
    struct CliArgs args;

    {
        char* argv[] = {"program", "--mem-addr-size", "2"};
        parse_cli(3, argv, &args);
        expect(args.cache_config.memory_addr_len == 2, "Wrong address width %i", args.cache_config.memory_addr_len);
        expect(args.memory_size == 4, "Wrong memory size. Found %i", args.memory_size);
    }


    {
        char *argv[] = {"program", "--cache-size", "8", "--cache-block-size", "2", "--associativity", "4"};
        parse_cli(7, argv, &args);
        expect(args.cache_config.cache_size == 8, NULL);
        expect(args.cache_config.cache_blk_size == 2, "Cache blk size: %i", args.cache_config.cache_blk_size);
        expect(args.cache_config.associativity == 4, NULL);
    }
}


int main() {
    test_some_args();
}


