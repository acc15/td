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

void print_duplicate_msg(const std::string& opt, const char* cur_val, const char* new_val) {
    std::cerr << "duplicate '-" << opt << "' option (original: " << cur_val << "): " << new_val << std::endl;
}

void process_opt(const std::string& opt, const char* arg, gen_conf& conf, gen_def& d, gen_input& i) {

    if (opt == "hd") {
        if (conf.header_dir) {
            print_duplicate_msg(opt, conf.header_dir, arg);
        }
        conf.header_dir = arg;
    } else if (opt == "id") {
        if (conf.input_dir) {
            print_duplicate_msg(opt, conf.input_dir, arg);
        }
        conf.input_dir = arg;
    } else if (opt == "cd") {
        if (conf.src_dir) {
            print_duplicate_msg(opt, conf.src_dir, arg);
        }
        conf.src_dir = arg;
    } else if (opt == "d") {
        if (conf.output_dir) {
            print_duplicate_msg(opt, conf.output_dir, arg);
        }
        conf.output_dir = arg;
    } else if (opt == "dn") {
        if (conf.default_ns) {
            print_duplicate_msg(opt, conf.default_ns, arg);
        }
        conf.default_ns = arg;
    } else if (opt == "i") {
        if (i.file) {
            process_input(i, d);
        }
        i.file = arg;
    } else if (opt == "v") {
        if (i.var && i.file) {
            process_input(i, d);
        } else if (i.var) {
            print_duplicate_msg(opt, i.var, arg);
        }
        i.var = arg;
    } else if (opt == "n") {
        if (i.ns && i.file) {
            process_input(i, d);
        } else if (i.ns) {
            print_duplicate_msg(opt, i.ns, arg);
        }
        i.ns = arg;
    } else if (opt == "o") {
        d.output.prefix = arg;
    } else if (opt == "h") {
        if (d.output.header) {
            print_duplicate_msg(opt, d.output.header, arg);
        }
        d.output.header = arg;
    } else if (opt == "c") {
        if (d.output.cpp) {
            print_duplicate_msg(opt, d.output.cpp, arg);
        }
        d.output.cpp = arg;
    } else if (opt.empty()) {
        std::cerr << "illegal cmd (missing option?): " << arg << std::endl;
    } else {
        std::cerr << "unknown option '-" << opt << "'" << std::endl;
    }
}


gen_conf parse_cmdline(int argc, const char* argv[]) {

    gen_conf conf = {};
    gen_def d = {};
    gen_input i = {};
    std::string opt;

    for (int n = 1; n < argc; n++) {
        const char* arg = argv[n];

        if ((opt.empty() || opt == "i") && arg[0] == '-') {
            opt = arg;
            opt.erase(0, 1);
            continue;
        }

        process_opt(opt, arg, conf, d, i);
        if (opt != "i") {
            // allow multiple files with -i <file1> <file2> <file3>
            opt.clear();
        }

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
              << "    [-id] - input directory" << std::endl
              << "    [-hd] - output headers directory" << std::endl
              << "    [-cd] - output cpp directory" << std::endl
              << "    [-dn] - default namespace (defaults to no namespace)" << std::endl
              << "    *(" << std::endl
              << "        *(" << std::endl
              << "            -i - input file(s), allows multiple sequence of input files (multiple arguments)" << std::endl
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
              << "    )" << std::endl
              << std::endl
              << "Examples:" << std::endl
              << std::endl
              << "    td_rc -p my::ns -i abc.gif xyz.jpeg def.txt -i custom_var.git -v CUST_VAR -n cust::ns -d out -o rc" << std::endl
              << "    td_rc -n abc -i abc.gif -v abc_val -o out/abc -n xyz -i xyz.jpeg -v xyz_val -o out/xyz.jpeg" << std::endl;
}