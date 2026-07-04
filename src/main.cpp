#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "semantic/semantic.h"
#include "bytecode/compiler.h"
#include "bytecode/vm.h"
#include "repl/repl.h"
#include <iostream>
#include <fstream>
#include <sstream>

void runFile(const char* path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: Cannot open file: " << path << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    r::Lexer lexer(source);
    auto tokens = lexer.tokenize();

    r::Parser parser(std::move(tokens));
    auto program = parser.parse();

    r::semantic::SemanticAnalyzer analyzer;
    auto errors = analyzer.analyze(program.get());
    if (!errors.empty()) {
        std::cerr << "Semantic errors (" << errors.size() << "):\n";
        for (const auto& e : errors) {
            std::cerr << "  [Line " << e.line << "] " << e.message << "\n";
        }
        exit(1);
    }

    r::vm::Chunk chunk;
    r::vm::Compiler compiler(&chunk);
    compiler.compile(program.get());

    r::vm::VM vm;
    vm.execute(&chunk);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        r::REPL repl;
        repl.run();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        std::cerr << "Usage: " << argv[0] << " [file.r]" << std::endl;
        return 1;
    }
    return 0;
}
