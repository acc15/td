#include <iostream>
#include <algorithm>

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

void merge_inputs(target_map& tm, const std::filesystem::path& p, const target_type& type, const target_input_map& inputs, const dependency_set& deps) {
    if (tm.count(p) == 0) {
        tm[p] = {type, deps, inputs};
        return;
    }

    target& t = tm[p];
    if (t.type != type) {
        std::cout << "[WARNING] " << (type == HEADER ? "Header" : "CPP") << " file " << p << " output duplicated. Check input options" << std::endl;
        tm[p] = {type, deps, inputs};
        return;
    }

    t.dependencies.insert(deps.begin(), deps.end());

    for (const auto& ns_entry: inputs) {
        auto& t_vars = t.inputs[ns_entry.first];
        for (const auto& var_entry: ns_entry.second) {
            if (t_vars.count(var_entry.first)) {
                std::cout << "[WARNING] Duplicate variable '" << var_entry.first << "' for input "
                          << var_entry.second << " will overwrite input from " << t_vars[var_entry.first] << std::endl;
            }
            t_vars[var_entry.first] = var_entry.second;
        }
    }

}


target_map make_targets(const gen_conf& conf) {

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


    target_map tm;
    for (const gen_def& def: conf.defs) {

        std::filesystem::path header_path = header_dir;
        std::filesystem::path cpp_path = src_dir;

        header_path /= def.output.header ? def.output.header : std::filesystem::path(def.output.prefix).concat(".hpp");
        cpp_path /= def.output.cpp ? def.output.cpp : std::filesystem::path(def.output.prefix).concat(".cpp");

        target_input_map inputs = make_target_inputs(conf, def, input_dir);
        merge_inputs(tm, header_path, HEADER, inputs, {});
        merge_inputs(tm, cpp_path, CPP, inputs, { header_path });

    }

    return tm;
}

