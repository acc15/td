#pragma once

#include <filesystem>
#include <map>
#include <set>
#include <unordered_map>

#include "cmdline.h"


enum target_type {
    HEADER,
    CPP
};

struct path_hasher {
    size_t operator()(const std::filesystem::path& p) const {
        return std::filesystem::hash_value(p);
    }
};

typedef std::map<std::vector<std::string>, std::map<std::string, std::filesystem::path>> target_input_map;
typedef std::set<std::filesystem::path> dependency_set;

struct target {
    target_type type;
    dependency_set dependencies;
    target_input_map inputs;
};

typedef std::unordered_map<std::filesystem::path, target, path_hasher> target_map;

target_map make_targets(const gen_conf& conf);

std::string namespace_str(const std::vector<std::string>& vec);



