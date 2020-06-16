#pragma once

#include <vector>
#include <string>

namespace td {

bool is_gl_debug_enabled(const std::vector<std::string>& args);

void enable_gl_debug();
void disable_gl_debug();
void activate_gl_debug();

}
