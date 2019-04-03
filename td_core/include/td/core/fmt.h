//
// Created by acc15 on 03.04.19.
//

#pragma once

#include <sstream>

namespace td {

    typedef std::ostringstream fmts;

    std::string fmt_str(const std::ostream &stream) {
        return dynamic_cast<const std::ostringstream&>(stream).str();
    }

}
