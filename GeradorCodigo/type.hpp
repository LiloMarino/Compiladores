#ifndef TYPE_HPP
#define TYPE_HPP

#include <deque>
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
    std::unique_ptr<std::deque<int>>  dimensions;
    int pointerLevel;

public:
    Type(TypeEnum type, int pointerLevel, std::unique_ptr<std::deque<int>> dimensions);
};

#endif