# Educational C Compiler for Windows x64 
***(This is an ongoing Project - Features will be added gradually)***
# Current Situation:
- Implement all types of lexemes
- Implement UTF-8 multi-byte decoding logic
- Enhance parser data structures

### 1. Lexical Analysis (`lexer.h`,`lexer.c`, `id_lex.c`, `num_lex.c`)
* **State:** Under Active Development.
* **Details:** Implements robust UTF-8 multi-byte decoding logic and state-machine-driven tokenization for all standard C lexemes, minimizing allocation overhead during scanning.

### 2. Syntax & Expression Parsing (`parser.c`)
* **State:** Under Active Development.
* **Details:** Designing a heap-managed, stack-based expression evaluator driven by a formal state-machine parser. The grammar tracking strictly follows `cppreference` specifications to guarantee compliance.

### 3. Executable Generation & Linker (`link_pe.c`, `sections.h`)
* **State:** Architecture & Layout Defined.
* **Details:** Implementing a custom, minimal Windows PE (.exe / .dll) linker pipeline. Current work focuses on COFF/PE section allocation strategies, structural alignment rules, and defining the Import Address Table (IAT) layout for dynamic FFI linking.


# Purpose
C compiler project designed to help programmers understand:
* how compilers work internally
* low-level programming concepts
* type systems and expression parsing
* linking and executable generation
* Windows PE (.exe / .dll) structure
* calling conventions and ABI behavior
* Windows x64 PE generation
* minimal custom linker (dynamic linking only)
* .exe and .dll output support
  
This project is built **for learning and research purposes**

---

# Historical Context

C evolved from earlier typeless languages such as **BCPL** and **B**.
The B language treated variables as machine words without explicit types, which limited safety and expressiveness.

C introduced:
* explicit data types
* structured data (`struct`)
* function prototypes
* file inclusion model
* pointer-based abstractions
* better memory control
* modular compilation

These innovations made C both close to hardware and expressive enough for large systems.

---


