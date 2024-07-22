#ifndef TOKENS_HPP
#define TOKENS_HPP

/**
 * @brief Define os tokens
 */
enum class Token {
    END_OF_FILE = -1,
    ID = 0,
    IF,
    WHITE_SPACE,
    NUM,
    REAL,
    COMMENT,
    ERROR,
};

#endif