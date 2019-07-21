#include <string>
#include <map>
#include <unordered_map>
#include <filesystem>

#include <iostream>
#include <algorithm>

#include "gen.h"
#include "cmdline.h"

using std::filesystem::path;

struct path_hasher {
    size_t operator()(const path& p) const {
        return std::filesystem::hash_value(p);
    }
};

enum target_type {
    HEADER,
    CPP
};

struct target_input {

    path input_file;
    std::string var;
    std::string ns;

};

struct target {

    target_type type;
    std::vector<target_input> inputs;

};


typedef std::unordered_map<path, target, path_hasher> target_map;

std::vector<target_input> make_target_inputs(const gen_conf& conf, const gen_def& def, const path& input_dir) {
    std::vector<target_input> inputs;
    for (gen_input input: def.inputs) {

        target_input i;

        i.input_file = input_dir / input.file;

        if (input.ns) {
            i.ns = input.ns;
        } else if (conf.default_ns) {
            i.ns = conf.default_ns;
        }

        if (input.var) {
            i.var = input.var;
        } else {
            i.var = i.input_file.stem().string();
            std::transform( i.var.begin(), i.var.end(), i.var.begin(), toupper );
        }

        inputs.push_back(i);
    }
    return inputs;
}

target_map make_target_map(const gen_conf& conf) {

    target_map output_map;

    path out_dir;
    if (conf.output_dir) {
        out_dir = conf.output_dir;
    }

    path header_dir = out_dir;
    if (conf.header_dir) {
        header_dir /= path(conf.header_dir);
    }

    path src_dir = out_dir;
    if (conf.src_dir) {
        src_dir /= path(conf.src_dir);
    }

    path input_dir;
    if (conf.input_dir) {
        input_dir = path(conf.input_dir);
    }

    for (const gen_def& def: conf.defs) {

        path header_path = header_dir;
        path cpp_path = src_dir;

        if (def.output.prefix != nullptr) {

            header_path /= def.output.prefix;
            header_path += ".hpp";

            cpp_path /= def.output.prefix;
            cpp_path += ".cpp";

        } else {

            header_path /= def.output.header;
            cpp_path /= def.output.cpp;

        }

        std::vector<target_input> inputs = make_target_inputs(conf, def, input_dir);

        if (output_map.count(header_path) > 0) {
            std::cout << "[WARNING] Header file " << header_path << " output duplicated. Check input options" << std::endl;
        }
        output_map[header_path] = { HEADER, inputs };

        if (output_map.count(cpp_path) > 0) {
            std::cout << "[WARNING] CPP file " << cpp_path << " output duplicated. Check input options" << std::endl;
        }
        output_map[cpp_path] = { CPP, inputs };

    }

    return output_map;
}

void generate_target(const path& output_file, const target& t) {
    std::cout << "Generating to " << output_file << " of type " << t.type << std::endl;
    for (const target_input& i: t.inputs) {
        std::cout << "    ...including file " << i.input_file << "; variable '" << i.var << "' with namespace '" << i.ns << "'" << std::endl;
    }
    std::cout << std::endl;
}

void generate_files(const gen_conf& conf) {
    std::unordered_map<path, target, path_hasher> m = make_target_map(conf);
    for (const std::pair<const path, target>& e: m) {
        generate_target(e.first, e.second);
    }
}