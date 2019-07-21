#include <iostream>
#include <algorithm>
#include <unordered_set>
#include "targets.h"

std::vector<std::string> parse_namespace(const std::string& str) {
    std::vector<std::string> ns;

    size_t pos = 0;
    while (pos < str.size()) {
        size_t n = str.find("::", pos);
        if (n == std::string::npos) {
            n = str.size();
        }
        if (n > pos) {
            ns.push_back(str.substr(pos, n - pos));
        }
        pos = n + 2;
    }
    return ns;
}

std::string namespace_str(const std::vector<std::string>& ns_vec) {
    std::string str;
    if (!ns_vec.empty()) {
        str += ns_vec[0];
        for (size_t i = 1; i < ns_vec.size(); i++) {
            str += "::";
            str += ns_vec[i];
        }
    }
    return str;
}

target_input_map make_target_inputs(const gen_conf& conf, const gen_def& def, const std::filesystem::path& input_dir) {
    target_input_map inputs;
    for (gen_input input: def.inputs) {

        std::filesystem::path input_path = input_dir / input.file;

        std::string ns_raw;
        if (input.ns) {
            ns_raw = input.ns;
        } else if (conf.default_ns) {
            ns_raw = conf.default_ns;
        }

        std::vector<std::string> ns = parse_namespace(ns_raw);


        std::string var;
        if (input.var) {
            var = input.var;
        } else {
            var = input_path.stem().string();
            std::transform( var.begin(), var.end(), var.begin(), toupper );
        }

        std::map<std::string, std::filesystem::path>& var_map = inputs[ns];
        if (var_map.count(var) > 0) {
            std::cout << "[WARNING] Duplicate variable '" << var << "' for input "
                << input_path << " will overwrite input from " << var_map[var] << std::endl;
        }
        var_map[var] = input_path;
    }
    return inputs;
}


struct path_hasher {
    size_t operator()(const std::filesystem::path& p) const {
        return std::filesystem::hash_value(p);
    }
};

typedef std::unordered_map<std::filesystem::path, std::reference_wrapper<target>, path_hasher> target_ref_map;

std::vector<target> make_targets(const gen_conf& conf) {

    std::filesystem::path out_dir;
    if (conf.output_dir) {
        out_dir = conf.output_dir;
    }

    std::filesystem::path header_dir = out_dir;
    if (conf.header_dir) {
        header_dir /= std::filesystem::path(conf.header_dir);
    }

    std::filesystem::path src_dir = out_dir;
    if (conf.src_dir) {
        src_dir /= std::filesystem::path(conf.src_dir);
    }

    std::filesystem::path input_dir;
    if (conf.input_dir) {
        input_dir = std::filesystem::path(conf.input_dir);
    }


    std::vector<target> targets;
    std::unordered_set<std::filesystem::path, path_hasher> used_paths;
    for (const gen_def& def: conf.defs) {

        std::filesystem::path header_path = header_dir;
        std::filesystem::path cpp_path = src_dir;

        if (def.output.prefix != nullptr) {

            header_path /= def.output.prefix;
            header_path += ".hpp";

            cpp_path /= def.output.prefix;
            cpp_path += ".cpp";

        } else {

            header_path /= def.output.header;
            cpp_path /= def.output.cpp;

        }


        if (used_paths.count(header_path) > 0) {
            std::cout << "[ERROR] Header file " << header_path << " already used. Check options." << std::endl;
            continue;
        }
        if (used_paths.count(cpp_path) > 0) {
            std::cout << "[ERROR] CPP file " << cpp_path << " already used. Check options." << std::endl;
            continue;
        }

        target_input_map inputs = make_target_inputs(conf, def, input_dir);
        targets.push_back({ header_path, cpp_path, inputs });

        used_paths.insert(header_path);
        used_paths.insert(cpp_path);

    }

    return targets;
}

