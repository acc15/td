#pragma once

#include <vector>

struct gen_input {
    const char* file = nullptr;
    const char* var = nullptr;
    const char* ns = nullptr;
};

struct gen_output {
    const char* prefix = nullptr;
    const char* header = nullptr;
    const char* cpp = nullptr;
};

struct gen_def {
    std::vector<gen_input> inputs;
    gen_output output;
};

struct gen_conf {

    const char* output_dir;
    const char* header_dir;
    const char* src_dir;
    const char* input_dir;
    const char* default_ns;

    std::vector<gen_def> defs;
};

gen_conf parse_cmdline(int argc, const char* argv[]);
void print_usage();