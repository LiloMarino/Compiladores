#ifndef TOKENS_HPP
#define TOKENS_HPP

/**
 * @brief Define os tokens
 */
enum class Token {
    END_OF_FILE = -1,
    LINE,
    BLANKLINE,
    COMMENTLINE,
    NB_NC_LINE,
    SEMICOLON,
    PREPROCDIR,
    ERROR,
};

#endif