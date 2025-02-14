#include <stdio.h>
#include "cli.h"

int parse_cli(int argc, char* argv[], struct CliArgs *args) {
    printf("Args: \n");
    for (int i = 0; i < argc; i++) {
        printf("%s \n", argv[i]);
    }



    return 0;
}
