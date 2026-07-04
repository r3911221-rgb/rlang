#ifndef R_VM_H
#define R_VM_H

#include "bytecode/chunk.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace r::vm {

class VM {
public:
    void execute(Chunk* chunk);

private:
    Chunk* chunk_ = nullptr;
    std::vector<Value> stack_;
    std::unordered_map<std::string, Value> globals_;
    size_t ip_ = 0;

    void reset();
    uint8_t readByte();
    uint16_t readShort();
    Value readConstant();
    uint32_t readLine();

    void push(const Value& value);
    Value pop();
    Value& peek(size_t distance = 0);

    void runtimeError(const std::string& msg);
    void run();
    bool callValue(const Value& callee, uint8_t argCount);
};

} // namespace r::vm

#endif
