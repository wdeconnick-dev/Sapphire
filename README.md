# Sapphire 💎

A custom programming language interpreter built completely from scratch in modern C++.

## 🎯 Project Goals
Sapphire is designed as an architectural exploration into custom language design, syntax parsing, and efficient low-level resource management. The goal is to build an extensible, robust engine without relying on external libraries or standard containers.

## 🚀 Current Architecture & Finished Components
* **`sapphire::String`**: A fully custom, dynamic, heap-allocated string container compliant with the Rule of Three, featuring automatic resizing and bounds safety checks (`at()`).
* **`sapphire::Stack<T>`**: A production-grade template link-frame stack engineered with specialized in-place pointer manipulation for structural reversals without memory footprint allocation overhead.

## 🗺️ Project Roadmap
- [x] Memory-Safe Core Containers (String, Stack)
- [ ] Low-Level Character Stream Utilities
- [ ] Lexical Analysis (Tokenizer Engine)
- [ ] Abstract Syntax Tree (AST) Parsing
- [ ] Runtime Execution Engine
