#include "type.hpp"

Type::Type(TypeEnum type, int pointerLevel, std::unique_ptr<std::vector<int>> dimensions)
    : type(type), dimensions(std::move(dimensions)), pointerLevel(pointerLevel) {}
