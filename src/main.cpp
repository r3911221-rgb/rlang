#include "lexer/lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>

void printToken(const r::Token& token) {
    std::cout << "Token(";

    switch (token.type) {
        case r::TokenType::INT_LITERAL: std::cout << "INT"; break;
        case r::TokenType::FLOAT_LITERAL: std::cout << "FLOAT"; break;
        case r::TokenType::STRING_LITERAL: std::cout << "STRING"; break;
        case r::TokenType::BOOL_LITERAL: std::cout << "BOOL"; break;
        case r::TokenType::NIL_LITERAL: std::cout << "NIL"; break;
        case r::TokenType::IDENTIFIER: std::cout << "IDENT"; break;
        case r::TokenType::FN: std::cout << "FN"; break;
        case r::TokenType::LET: std::cout << "LET"; break;
        case r::TokenType::CONST: std::cout << "CONST"; break;
        case r::TokenType::MUT: std::cout << "MUT"; break;
        case r::TokenType::IF: std::cout << "IF"; break;
        case r::TokenType::ELSE: std::cout << "ELSE"; break;
        case r::TokenType::MATCH: std::cout << "MATCH"; break;
        case r::TokenType::WHILE: std::cout << "WHILE"; break;
        case r::TokenType::FOR: std::cout << "FOR"; break;
        case r::TokenType::IN: std::cout << "IN"; break;
        case r::TokenType::BREAK: std::cout << "BREAK"; break;
        case r::TokenType::CONTINUE: std::cout << "CONTINUE"; break;
        case r::TokenType::RETURN: std::cout << "RETURN"; break;
        case r::TokenType::STRUCT: std::cout << "STRUCT"; break;
        case r::TokenType::CLASS: std::cout << "CLASS"; break;
        case r::TokenType::TRAIT: std::cout << "TRAIT"; break;
        case r::TokenType::IMPL: std::cout << "IMPL"; break;
        case r::TokenType::ENUM: std::cout << "ENUM"; break;
        case r::TokenType::ASYNC: std::cout << "ASYNC"; break;
        case r::TokenType::AWAIT: std::cout << "AWAIT"; break;
        case r::TokenType::TRY: std::cout << "TRY"; break;
        case r::TokenType::CATCH: std::cout << "CATCH"; break;
        case r::TokenType::THROW: std::cout << "THROW"; break;
        case r::TokenType::IMPORT: std::cout << "IMPORT"; break;
        case r::TokenType::EXPORT: std::cout << "EXPORT"; break;
        case r::TokenType::MODULE: std::cout << "MODULE"; break;
        case r::TokenType::PUB: std::cout << "PUB"; break;
        case r::TokenType::PRIV: std::cout << "PRIV"; break;
        case r::TokenType::AND: std::cout << "AND"; break;
        case r::TokenType::OR: std::cout << "OR"; break;
        case r::TokenType::NOT: std::cout << "NOT"; break;
        case r::TokenType::AS: std::cout << "AS"; break;
        case r::TokenType::IS: std::cout << "IS"; break;
        case r::TokenType::INT: std::cout << "TYPE_INT"; break;
        case r::TokenType::STRING: std::cout << "TYPE_STRING"; break;
        case r::TokenType::BOOL: std::cout << "TYPE_BOOL"; break;
        case r::TokenType::VOID: std::cout << "TYPE_VOID"; break;
        case r::TokenType::LPAREN: std::cout << "LPAREN"; break;
        case r::TokenType::RPAREN: std::cout << "RPAREN"; break;
        case r::TokenType::LBRACE: std::cout << "LBRACE"; break;
        case r::TokenType::RBRACE: std::cout << "RBRACE"; break;
        case r::TokenType::LBRACKET: std::cout << "LBRACKET"; break;
        case r::TokenType::RBRACKET: std::cout << "RBRACKET"; break;
        case r::TokenType::COMMA: std::cout << "COMMA"; break;
        case r::TokenType::SEMICOLON: std::cout << "SEMICOLON"; break;
        case r::TokenType::PLUS: std::cout << "PLUS"; break;
        case r::TokenType::MINUS: std::cout << "MINUS"; break;
        case r::TokenType::STAR: std::cout << "STAR"; break;
        case r::TokenType::SLASH: std::cout << "SLASH"; break;
        case r::TokenType::PERCENT: std::cout << "PERCENT"; break;
        case r::TokenType::ASSIGN: std::cout << "ASSIGN"; break;
        case r::TokenType::PLUS_ASSIGN: std::cout << "PLUS_ASSIGN"; break;
        case r::TokenType::MINUS_ASSIGN: std::cout << "MINUS_ASSIGN"; break;
        case r::TokenType::STAR_ASSIGN: std::cout << "STAR_ASSIGN"; break;
        case r::TokenType::SLASH_ASSIGN: std::cout << "SLASH_ASSIGN"; break;
        case r::TokenType::PERCENT_ASSIGN: std::cout << "PERCENT_ASSIGN"; break;
        case r::TokenType::EQ: std::cout << "EQ"; break;
        case r::TokenType::NEQ: std::cout << "NEQ"; break;
        case r::TokenType::LT: std::cout << "LT"; break;
        case r::TokenType::GT: std::cout << "GT"; break;
        case r::TokenType::LEQ: std::cout << "LEQ"; break;
        case r::TokenType::GEQ: std::cout << "GEQ"; break;
        case r::TokenType::AND_AND: std::cout << "AND_AND"; break;
        case r::TokenType::OR_OR: std::cout << "OR_OR"; break;
        case r::TokenType::ARROW: std::cout << "ARROW"; break;
        case r::TokenType::FAT_ARROW: std::cout << "FAT_ARROW"; break;
        case r::TokenType::COLON: std::cout << "COLON"; break;
        case r::TokenType::COLON_COLON: std::cout << "COLON_COLON"; break;
        case r::TokenType::DOT: std::cout << "DOT"; break;
        case r::TokenType::DOT_DOT: std::cout << "DOT_DOT"; break;
        case r::TokenType::DOT_DOT_DOT: std::cout << "DOT_DOT_DOT"; break;
        case r::TokenType::DOT_DOT_EQ: std::cout << "DOT_DOT_EQ"; break;
        case r::TokenType::QUESTION: std::cout << "QUESTION"; break;
        case r::TokenType::BANG: std::cout << "BANG"; break;
        case r::TokenType::AT: std::cout << "AT"; break;
        case r::TokenType::HASH: std::cout << "HASH"; break;
        case r::TokenType::DOLLAR: std::cout << "DOLLAR"; break;
        case r::TokenType::EOF_TOKEN: std::cout << "EOF"; break;
        case r::TokenType::ERROR: std::cout << "ERROR"; break;
        default: std::cout << "UNKNOWN"; break;
    }

    if (!token.text.empty()) {
        std::cout << ", \"" << token.text << "\"";
    }
    std::cout << ")" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file.r>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: Cannot open file: " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    r::Lexer lexer(source);
    std::vector<r::Token> tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        printToken(token);
    }

    return 0;
}
