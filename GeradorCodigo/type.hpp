#ifndef TYPE_HPP
#define TYPE_HPP

#include <vector>
#include <memory>

struct Type
{
    enum TypeEnum
    {
        INT,
        CHAR,
        VOID_POINTER,
        ARRAY_INT,
        ARRAY_CHAR
    };

private:
    TypeEnum type;
    std::unique_ptr<std::vector<int>>  dimensions;
    int pointerLevel;

public:
    Type(TypeEnum type, int pointerLevel, std::unique_ptr<std::vector<int>> dimensions);
};

#endif