# Dragoon

Dragoon is a compiler in-progress for a toy language.

The full extent of features is yet to be determined.  Currently the parser is operational and semantic analysis is underway.

Direct support of x86 code generation is a goal, however LLVM (which will no doubt generate better x86 code) may be supported either before or after x86.

I'm using the [Dragon Book](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools) as a primary resource, though not everything I do follows it precisely and I consider this repository exploratory.  Some decisions, both at the low level and high level, are primarily because I find them to be more interesting.

## Building
To compile, simply make:
```bash
make
```
Or to save time:
```bash
make -j`nproc`
```
The makefile currently compiles with debug symbols and no optimizations by default.
