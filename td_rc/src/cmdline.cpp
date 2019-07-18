#include "cmdline.h"


#include <iostream>
#include <string>
#include <cstring>

std::ostream& operator<<(std::ostream& stream, const gen_output& out) {
    if (out.prefix) {
        stream << "<prefix: " << out.prefix << ">";
    } else if (out.cpp && out.header) {
        stream << "<header: " << out.header << ", cpp: " << out.cpp << ">";
    } else {
        stream << "<empty>";
    }
    return stream;
}

void process_input(gen_input& i, gen_def& d) {
    d.inputs.push_back(i);
    i = {};
}

void process_def(gen_input& i, gen_def& d, gen_conf& conf) {
    if (i.file) {
        d.inputs.push_back(i);
        i = {};
    } else {
        if (i.ns) {
            std::cerr << "ignored '-n' value due to missing input file: " << i.ns << std::endl;
        }
        if (i.var) {
            std::cerr << "ignored '-v' value due to missing input file: " << i.var << std::endl;
        }
    }
    if (d.inputs.empty()) {
        std::cerr << "empty inputs for output " << d.output;
    } else {
        conf.defs.push_back(d);
    }

    d.inputs.clear();
    d.output = {};
}

void print_duplicate_msg(const char opt, const char* cur_val, const char* new_val) {
    std::cerr << "duplicate '-" << opt << "' option (original: " << cur_val << "): " << new_val << std::endl;
}

gen_conf parse_cmdline(int argc, const char* argv[]) {

    gen_conf conf = {};
    gen_def d = {};
    gen_input i = {};
    char opt = '\0';

    for (int n = 1; n < argc; n++) {
        const char* arg = argv[n];

        if (opt == '\0' && strlen(arg) == 2 && arg[0] == '-') {
            opt = arg[1];
            continue;
        }

        switch (opt) {
            case 'd':
                if (conf.dir) {
                    print_duplicate_msg(opt, conf.dir, arg);
                }
                conf.dir = arg;
                break;

            case 'p':
                if (conf.ns) {
                    print_duplicate_msg(opt, conf.ns, arg);
                }
                conf.ns = arg;
                break;

            case 'i':
                if (i.file) {
                    process_input(i, d);
                }

                i.file = arg;
                break;

            case 'v':
                if (i.var && i.file) {
                    process_input(i, d);
                } else if (i.var) {
                    print_duplicate_msg(opt, i.var, arg);
                }
                i.var = arg;
                break;

            case 'n':
                if (i.ns && i.file) {
                    process_input(i, d);
                } else if (i.ns) {
                    print_duplicate_msg(opt, i.ns, arg);
                }
                i.ns = arg;
                break;

            case 'o':
                d.output.prefix = arg;
                break;

            case 'h':
                if (d.output.header) {
                    print_duplicate_msg(opt, d.output.header, arg);
                }
                d.output.header = arg;
                break;

            case 'c':
                if (d.output.cpp) {
                    print_duplicate_msg(opt, d.output.cpp, arg);
                }
                d.output.cpp = arg;
                break;

            case '\0':
                std::cerr << "illegal cmd (missing option?): " << arg << std::endl;
                break;

            default:
                std::cerr << "unknown option '-" << opt << "'" << std::endl;
                break;
        }

        opt = '\0';

        if (d.output.prefix != nullptr || (d.output.header != nullptr && d.output.cpp != nullptr)) {
            process_def(i, d, conf);
        }
    }

    return conf;
}

void print_usage() {
    std::cout << "Usage: " << std::endl
              << std::endl
              << "    [-d] - output directory (default to current working directory)" << std::endl
              << "    [-p] - default namespace (defaults to no namespace)" << std::endl
              << "    *(" << std::endl
              << "        *(" << std::endl
              << "            -i - input file" << std::endl
              << "            [-v] - optional variable name (defaults to uppercase filename without extension)" << std::endl
              << "            [-n] - optional namespace (defaults to '-p' namespace)" << std::endl
              << "        )" << std::endl
              << "        any(" << std::endl
              << "            (" << std::endl
              << "                -o - output file prefix (relative to output directory (see '-d') " << std::endl
              << "            )" << std::endl
              << "            (" << std::endl
              << "                -h - output header file " << std::endl
              << "                -c - output cpp file " << std::endl
              << "            )" << std::endl
              << "        )" << std::endl
              << "    )" << std::endl;
}