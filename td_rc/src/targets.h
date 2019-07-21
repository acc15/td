#pragma once

#include <filesystem>
#include <map>

#include "cmdline.h"

typedef std::map<std::vector<std::string>, std::map<std::string, std::filesystem::path>> target_input_map;

struct target {
    std::filesystem::path header_path;
    std::filesystem::path cpp_path;
    target_input_map inputs;
};

std::string namespace_str(const std::vector<std::string>& vec);
std::vector<target> make_targets(const gen_conf& conf);



