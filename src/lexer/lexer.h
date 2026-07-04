#ifndef R_LEXER_H
#define R_LEXER_H

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace r {

enum class TokenType {
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, BOOL_LITERAL, NIL_LITERAL,

    DEF, END, LET, CONST, MUT, IF, ELSIF, ELSE, CASE, WHEN, THEN,
    WHILE, FOR, IN, BREAK, CONTINUE, RETURN,
    STRUCT, CLASS, TRAIT, IMPL, ENUM,
    ASYNC, AWAIT, TRY, CATCH, THROW,
    IMPORT, EXPORT, MODULE, PUB, PRIV,
    AND, OR, NOT, AS, IS,

    INT, INT8, INT16, INT32, INT64,
    UINT, UINT8, UINT16, UINT32, UINT64,
    FLOAT, FLOAT32, FLOAT64,
    BOOL, CHAR, STRING, VOID, NEVER,

    IDENTIFIER,

    PLUS, MINUS, STAR, SLASH, PERCENT,
    ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN,
    STAR_ASSIGN, SLASH_ASSIGN, PERCENT_ASSIGN,
    EQ, NEQ, LT, GT, LEQ, GEQ,
    AND_AND, OR_OR, NOT_NOT,
    BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,
    SHL, SHR,
    ARROW, FAT_ARROW, COLON, COLON_COLON,
    DOT, DOT_DOT, DOT_DOT_DOT, DOT_DOT_EQ,
    QUESTION, BANG, AT, HASH, DOLLAR,

    LPAREN, RPAREN,
    LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    COMMA, SEMICOLON,

    NEWLINE, WHITESPACE, COMMENT,
    EOF_TOKEN, ERROR
};

struct Token {
    TokenType type;
    std::string text;
    uint32_t line;
    uint32_t column;
};

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source_;
    size_t pos_ = 0;
    uint32_t line_ = 1;
    uint32_t column_ = 1;

    char peek(size_t offset = 0) const;
    char advance();
    bool match(char expected);
    bool isAtEnd() const;

    Token makeToken(TokenType type, const std::string& text);
    Token makeError(const std::string& msg);

    void skipWhitespace();
    void skipComment();

    Token readNumber();
    Token readString();
    Token readIdentifier();

    static bool isAlpha(char c);
    static bool isDigit(char c);
    static bool isAlnum(char c);
};

} // namespace r

#endif // R_LEXER_H
