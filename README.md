# Educational C Compiler for Windows x64 
***(This is an ongoing Project - Features will be added gradually)***
# Current Situation:
- Implement all types of lexemes
- Implement UTF-8 multi-byte decoding logic
- Enhance parser data structures

# Roadmap  
[x] Lexer -> Improve token struct  
[ ] Parser -> Follow cppreference as a source  
[ ] Code generation -> heap managed stack based expression evaluator with state machine parser   
[ ] X64 opcode -> Emit native x64 opcode bytes into memory buffers  
[ ] PE linker -> implement PE sections allocation strategy  


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


