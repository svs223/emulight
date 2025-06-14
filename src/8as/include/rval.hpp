#pragma once

#include <variant>

#include "reg.hpp"

class Rval {
    public:
        std::variant<Reg> v;

};