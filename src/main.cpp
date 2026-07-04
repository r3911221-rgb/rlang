#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

    // Lex
    r::Lexer lexer(source);
    std::vector<r::Token> tokens = lexer.tokenize();

    // Parse
    try {
        r::Parser parser(std::move(tokens));
        auto program = parser.parse();
        program->print(std::cout, 0);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
