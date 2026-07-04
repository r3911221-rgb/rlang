#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "semantic/semantic.h"
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
    r::Parser parser(std::move(tokens));
    auto program = parser.parse();

    // Semantic Analysis
    r::semantic::SemanticAnalyzer analyzer;
    auto errors = analyzer.analyze(program.get());

    if (!errors.empty()) {
        std::cerr << "Semantic errors (" << errors.size() << "):\n";
        for (const auto& e : errors) {
            std::cerr << "  [Line " << e.line << "] " << e.message << "\n";
        }
        return 1;
    }

    std::cout << "Semantic analysis passed successfully.\n";
    program->print(std::cout, 0);
    return 0;
}
