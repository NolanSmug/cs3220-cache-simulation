#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"


void expect(bool expr, char *msg) {
    if (!expr) {
        printf("%s\n", msg);
        exit(1);
    }
}


int parse_cli(int argc, char* argv[], struct CliArgs *args) {
    // printf("Args: \n");
    for (int i = 1; i < argc; i++) {
        // printf("%s \n", argv[i]);

        if (!strcmp(argv[i], "--mem-addr-size")) {
            expect(i + 1 < argc, "Expecting value to come after --mem-addr-size");
            int l = atoi(argv[i+1]);
            args->cache_config.memory_addr_len = l;
            args->memory_size = (uint32_t)pow(2, l);
            expect(args->memory_size > 0, "--mem-addr-size must be a positive integer");
            i++;
        } else if (!strcmp(argv[i], "--cache-size")) {
            expect(i + 1 < argc, "Expecting value to come after --cache-size");
            args->cache_config.cache_size = atoi(argv[i+1]);
            expect(args->cache_config.cache_size > 0, "--cache-size must be a positive integer");
            i++;
        } else if (!strcmp(argv[i], "--cache-block-size")) {
            expect(i + 1 < argc, "Expecting value to come after --cache-block-size");
            args->cache_config.cache_blk_size = atoi(argv[i+1]);
            expect(args->cache_config.cache_blk_size > 0, "--cache-block-size must be a positive integer");
            i++;
        } else if (!strcmp(argv[i], "--associativity")) {
            expect(i + 1 < argc, "Expecting value to come after --associativity");
            args->cache_config.associativity = atoi(argv[i+1]);
            expect(args->cache_config.associativity > 0, "--associativity must be a positive integer");
            i++;
        } else {
            char* msg = malloc(sizeof(char) * 1024); // Please let this be big enough
            sprintf(msg, "Unexpected argument: %s", argv[i]);
            expect(false, msg);
            free(msg);
        }
    }

    return 0;
}


enum TokenType {
    FMemoryAddrSize,
    FCacheSize,
    FCacheBlockSize,
    FAssociativityConstant,
    FWriteSetting,
};

struct Token {
    enum TokenType type;
    uint32_t pos;
    char *raw;
};

struct ParseError {
    char *message;
    char *raw;
};

struct LexResult {
    bool is_ok;
    union {
        struct Token *token;
        struct ParseError err;
    } content;
};


struct LexResult* lex(int argc, char *argv[]) {
    return 0;
}
