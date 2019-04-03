//
// Created by acc15 on 03.04.19.
//

#pragma once

#include <exception>
#include <string>

namespace td {

class exception : public std::exception {
public:
    explicit exception(const std::string& what);

    ~exception() override;

    const char *what() const noexcept override;

private:
    std::string _what;
};

}