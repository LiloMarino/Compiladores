#include "type.hpp"

Type::Type(TypeEnum type, int pointerLevel, std::unique_ptr<std::deque<int>> dimensions)
    : type(type), dimensions(std::move(dimensions)), pointerLevel(pointerLevel) {}

TypeEnum Type::getType() const
{
    return type;
}

int Type::getPointerLevel() const
{
    return pointerLevel;
}

std::deque<int> Type::getDimensions() const
{
    return *dimensions;
}
