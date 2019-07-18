#include "cmdline.h"

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        print_usage();
        return 0;
    }

    gen_conf conf = parse_cmdline(argc, argv);
    return 0;
}