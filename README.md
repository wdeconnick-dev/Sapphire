# Sapphire 💎

A custom programming language interpreter built completely from scratch in modern C++.

## 🎯 Project Goals
Sapphire is designed as an architectural exploration into custom language design, syntax parsing, and efficient low-level resource management. The goal is to build an extensible, robust engine without relying on external libraries or standard containers.

## 🚀 Current Architecture & Finished Components
* **`sapphire::String`**: A fully custom, dynamic, heap-allocated string container compliant with the Rule of Three, featuring automatic resizing, element access (`front()`, `back()`, `at()`), and single-character stack operations (`push_back()`, `pop_back()`).
* **`sapphire::Stack<T>`**: A production-grade template link-frame stack engineered with specialized in-place pointer manipulation for structural reversals without memory footprint allocation overhead.
* **`sapphire::Queue<T>`**: A production-grade doubly-linked template queue engineered for sequential FIFO processing.
* **`sapphire::token`**: A production-grade unified compiler token framework utilizing a memory-shared numeric `union` and dynamic `String` lexemes.
* **`sapphire::utility`**: A low-level stream I/O and numeric conversion suite featuring dynamic input/output routines, overflow-checked integer parsing (`chars_to_int`), floating-point parsing (`chars_to_float`), and character classification helpers.
* **`sapphire::Lexer`**: A deterministic lexical analyzer that scans raw source text into a FIFO queue of typed `Token` objects, featuring line-number tracking, reserved keyword resolution, numeric literal parsing, and single-character lookahead for multi-character operators.
* **`sapphire::Lexer`**: A recursive-descent Abstract Syntax Tree (AST) parser supporting full operator precedence, right-associated exponentiation, unary signs, block-scoped control flow (`if` / `eld` / `else`, `loop`), typed declarations and assignment (`<-`), parameterized subroutines (`subr:`), and RAII post-order AST memory cleanup.

## 🗺️ Project Roadmap
- [X] Memory-Safe Core Containers (String, Stack, Queue)
- [X] Low-Level Character Stream & Numeric Conversion Utilities
- [X] Lexical Analysis (Tokenizer Engine)
- [X] Abstract Syntax Tree (AST) Parsing
- [ ] Runtime Execution Engine
