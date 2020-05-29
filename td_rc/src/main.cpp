#include "cmdline.h"
#include "targets.h"
#include "gen.h"


int main(int argc, const char* argv[]) {

    if (argc < 2) {
        print_usage();
        return 0;
    }

    gen_conf conf = parse_cmdline(argc, argv);
    target_map targets = make_targets(conf);
    generate_files(targets);
    return 0;
}