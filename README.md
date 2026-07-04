# R Language

> Fast as C. Safe as Rust. Expressive as Ruby.

[![Build Status](https://github.com/YOUR_USERNAME/rlang/workflows/CI/badge.svg)](https://github.com/YOUR_USERNAME/rlang/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-0.1.0-blue.svg)](https://github.com/YOUR_USERNAME/rlang/releases)

R is a modern systems programming language designed for performance, safety, and developer productivity.

## Features

- **Native Performance** — Compiles to machine code via LLVM
- **Memory Safety** — Ownership system prevents bugs at compile time
- **Type Inference** — Write `x := 5` instead of `Int x = 5`
- **Pattern Matching** — Powerful `match` expressions
- **Native Async** — First-class `async/await`
- **Easy FFI** — Call C libraries directly

## Quick Start

```bash
# Clone
git clone https://github.com/YOUR_USERNAME/rlang.git
cd rlang

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Hello World
echo 'fn main() { println("Hello, R!") }' > hello.r
./r build hello.r
./hello
