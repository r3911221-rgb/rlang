#include "lexer.h"
#include <cctype>
#include <stdexcept>

namespace r {

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();
        skipComment();

        if (isAtEnd()) break;

        char c = peek();

        if (isDigit(c)) {
            tokens.push_back(readNumber());
        } else if (isAlpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
        } else if (c == '"') {
            tokens.push_back(readString());
        } else {
            switch (c) {
                case '(': advance(); tokens.push_back(makeToken(TokenType::LPAREN, "(")); break;
                case ')': advance(); tokens.push_back(makeToken(TokenType::RPAREN, ")")); break;
                case '{': advance(); tokens.push_back(makeToken(TokenType::LBRACE, "{")); break;
                case '}': advance(); tokens.push_back(makeToken(TokenType::RBRACE, "}")); break;
                case '[': advance(); tokens.push_back(makeToken(TokenType::LBRACKET, "[")); break;
                case ']': advance(); tokens.push_back(makeToken(TokenType::RBRACKET, "]")); break;
                case ',': advance(); tokens.push_back(makeToken(TokenType::COMMA, ",")); break;
                case ';': advance(); tokens.push_back(makeToken(TokenType::SEMICOLON, ";")); break;

                case '+':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::PLUS_ASSIGN, "+="));
                    else tokens.push_back(makeToken(TokenType::PLUS, "+"));
                    break;

                case '-':
                    advance();
                    if (match('>')) tokens.push_back(makeToken(TokenType::ARROW, "->"));
                    else if (match('=')) tokens.push_back(makeToken(TokenType::MINUS_ASSIGN, "-="));
                    else tokens.push_back(makeToken(TokenType::MINUS, "-"));
                    break;

                case '*':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::STAR_ASSIGN, "*="));
                    else tokens.push_back(makeToken(TokenType::STAR, "*"));
                    break;

                case '/':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::SLASH_ASSIGN, "/="));
                    else tokens.push_back(makeToken(TokenType::SLASH, "/"));
                    break;

                case '%':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::PERCENT_ASSIGN, "%="));
                    else tokens.push_back(makeToken(TokenType::PERCENT, "%"));
                    break;

                case '=':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::EQ, "=="));
                    else if (match('>')) tokens.push_back(makeToken(TokenType::FAT_ARROW, "=>"));
                    else tokens.push_back(makeToken(TokenType::ASSIGN, "="));
                    break;

                case '!':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::NEQ, "!="));
                    else tokens.push_back(makeToken(TokenType::BANG, "!"));
                    break;

                case '<':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::LEQ, "<="));
                    else if (match('<')) tokens.push_back(makeToken(TokenType::SHL, "<<"));
                    else tokens.push_back(makeToken(TokenType::LT, "<"));
                    break;

                case '>':
                    advance();
                    if (match('=')) tokens.push_back(makeToken(TokenType::GEQ, ">="));
                    else if (match('>')) tokens.push_back(makeToken(TokenType::SHR, ">>"));
                    else tokens.push_back(makeToken(TokenType::GT, ">"));
                    break;

                case '&':
                    advance();
                    if (match('&')) tokens.push_back(makeToken(TokenType::AND_AND, "&&"));
                    else tokens.push_back(makeToken(TokenType::BIT_AND, "&"));
                    break;

                case '|':
                    advance();
                    if (match('|')) tokens.push_back(makeToken(TokenType::OR_OR, "||"));
                    else tokens.push_back(makeToken(TokenType::BIT_OR, "|"));
                    break;

                case '^': advance(); tokens.push_back(makeToken(TokenType::BIT_XOR, "^")); break;
                case '~': advance(); tokens.push_back(makeToken(TokenType::BIT_NOT, "~")); break;

                case ':':
                    advance();
                    if (match(':')) tokens.push_back(makeToken(TokenType::COLON_COLON, "::"));
                    else tokens.push_back(makeToken(TokenType::COLON, ":"));
                    break;

                case '.':
                    advance();
                    if (match('.')) {
                        if (match('.')) tokens.push_back(makeToken(TokenType::DOT_DOT_DOT, "..."));
                        else if (match('=')) tokens.push_back(makeToken(TokenType::DOT_DOT_EQ, "..="));
                        else tokens.push_back(makeToken(TokenType::DOT_DOT, ".."));
                    } else {
                        tokens.push_back(makeToken(TokenType::DOT, "."));
                    }
                    break;

                case '?': advance(); tokens.push_back(makeToken(TokenType::QUESTION, "?")); break;
                case '@': advance(); tokens.push_back(makeToken(TokenType::AT, "@")); break;
                case '#': advance(); tokens.push_back(makeToken(TokenType::HASH, "#")); break;
                case '$': advance(); tokens.push_back(makeToken(TokenType::DOLLAR, "$")); break;

                default:
                    advance();
                    tokens.push_back(makeError(std::string("Unexpected character: ") + c));
                    break;
            }
        }
    }

    tokens.push_back(makeToken(TokenType::EOF_TOKEN, ""));
    return tokens;
}

char Lexer::peek(size_t offset) const {
    if (pos_ + offset >= source_.size()) return '\0';
    return source_[pos_ + offset];
}

char Lexer::advance() {
    char c = peek();
    pos_++;
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return c;
}

bool Lexer::match(char expected) {
    if (peek() == expected) {
        advance();
        return true;
    }
    return false;
}

bool Lexer::isAtEnd() const {
    return pos_ >= source_.size();
}

Token Lexer::makeToken(TokenType type, const std::string& text) {
    return Token{type, text, line_, column_ - static_cast<uint32_t>(text.length())};
}

Token Lexer::makeError(const std::string& msg) {
    return Token{TokenType::ERROR, msg, line_, column_};
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

void Lexer::skipComment() {
    if (peek() == '/' && peek(1) == '/') {
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
    }
}

Token Lexer::readNumber() {
    uint32_t startLine = line_;
    uint32_t startCol = column_;
    std::string numStr;

    bool isFloat = false;

    while (!isAtEnd() && (isDigit(peek()) || peek() == '.')) {
        if (peek() == '.') {
            if (isFloat) break;
            isFloat = true;
        }
        numStr += advance();
    }

    TokenType type = isFloat ? TokenType::FLOAT_LITERAL : TokenType::INT_LITERAL;
    return Token{type, numStr, startLine, startCol};
}

Token Lexer::readString() {
    uint32_t startLine = line_;
    uint32_t startCol = column_;
    std::string str;
    advance(); // opening "

    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\\') {
            advance();
            char escaped = advance();
            switch (escaped) {
                case 'n': str += '\n'; break;
                case 't': str += '\t'; break;
                case 'r': str += '\r'; break;
                case '\\': str += '\\'; break;
                case '"': str += '"'; break;
                case '0': str += '\0'; break;
                default: str += escaped; break;
            }
        } else {
            str += advance();
        }
    }

    if (isAtEnd()) {
        return Token{TokenType::ERROR, "Unterminated string", startLine, startCol};
    }

    advance(); // closing "
    return Token{TokenType::STRING_LITERAL, str, startLine, startCol};
}

Token Lexer::readIdentifier() {
    uint32_t startLine = line_;
    uint32_t startCol = column_;
    std::string ident;

    while (!isAtEnd() && (isAlnum(peek()) || peek() == '_')) {
        ident += advance();
    }

    static const std::unordered_map<std::string, TokenType> keywords = {
        {"fn", TokenType::FN}, {"let", TokenType::LET},
        {"const", TokenType::CONST}, {"mut", TokenType::MUT},
        {"if", TokenType::IF}, {"else", TokenType::ELSE},
        {"match", TokenType::MATCH}, {"while", TokenType::WHILE},
        {"for", TokenType::FOR}, {"in", TokenType::IN},
        {"break", TokenType::BREAK}, {"continue", TokenType::CONTINUE},
        {"return", TokenType::RETURN}, {"struct", TokenType::STRUCT},
        {"class", TokenType::CLASS}, {"trait", TokenType::TRAIT},
        {"impl", TokenType::IMPL}, {"enum", TokenType::ENUM},
        {"async", TokenType::ASYNC}, {"await", TokenType::AWAIT},
        {"try", TokenType::TRY}, {"catch", TokenType::CATCH},
        {"throw", TokenType::THROW}, {"import", TokenType::IMPORT},
        {"export", TokenType::EXPORT}, {"module", TokenType::MODULE},
        {"pub", TokenType::PUB}, {"priv", TokenType::PRIV},
        {"and", TokenType::AND}, {"or", TokenType::OR},
        {"not", TokenType::NOT}, {"as", TokenType::AS},
        {"is", TokenType::IS}, {"true", TokenType::BOOL_LITERAL},
        {"false", TokenType::BOOL_LITERAL}, {"nil", TokenType::NIL_LITERAL},
        {"int", TokenType::INT}, {"int8", TokenType::INT8},
        {"int16", TokenType::INT16}, {"int32", TokenType::INT32},
        {"int64", TokenType::INT64}, {"uint", TokenType::UINT},
        {"uint8", TokenType::UINT8}, {"uint16", TokenType::UINT16},
        {"uint32", TokenType::UINT32}, {"uint64", TokenType::UINT64},
        {"float", TokenType::FLOAT}, {"float32", TokenType::FLOAT32},
        {"float64", TokenType::FLOAT64}, {"bool", TokenType::BOOL},
        {"char", TokenType::CHAR}, {"string", TokenType::STRING},
        {"void", TokenType::VOID}, {"never", TokenType::NEVER}
    };

    auto it = keywords.find(ident);
    if (it != keywords.end()) {
        return Token{it->second, ident, startLine, startCol};
    }

    return Token{TokenType::IDENTIFIER, ident, startLine, startCol};
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlnum(char c) {
    return isAlpha(c) || isDigit(c);
}

} // namespace r
