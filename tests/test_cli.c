#include "../cli.h"


void test_some_args() {

    struct CliArgs args;
    char* argv[] = {"program", "--mem-addr-size", "10"};
    parse_cli(3, argv, &args);
}


int main() {
    test_some_args();
}


