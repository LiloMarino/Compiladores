#ifndef TYPE_HPP
#define TYPE_HPP

#include <deque>
#include <memory>

enum class TypeEnum
{
    INT,
    CHAR,
    VOID_POINTER,
    ARRAY_INT,
    ARRAY_CHAR
};

class Type
{
private:
    TypeEnum type;
    std::unique_ptr<std::deque<int>>  dimensions;
    int pointerLevel;

public:
    Type(TypeEnum type, int pointerLevel, std::unique_ptr<std::deque<int>> dimensions);

    /**
     * @brief Obtém o tipo do tipo.
     * @return O tipo do tipo.
     */
    TypeEnum getType() const;

    /**
     * @brief Obtém as dimensões do tipo.
     * @return As dimensões do tipo.
     */
    std::deque<int> getDimensions() const;

    /**
     * @brief Obtém o nível de ponteiro do tipo.
     * @return O nível de ponteiro do tipo.
     */
    int getPointerLevel() const;
};

#endif