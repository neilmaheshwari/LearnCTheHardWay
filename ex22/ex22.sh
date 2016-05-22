#!/bin/sh

set -e

# Compile ex22.c and ex22_main.c
# -g flag generates debug information
# -c flag  runs preprocessor, parser, type checking, LLVM generation,
# optimization, target-spceific code generation, produces an assembly file,
# and assembler, generating a target ".o" object file
# -o <file_name> flag writes output to file
# -DNDEBUG sets the NDEBUG flag

cc -Wall -g -DNDEBUG -c -o ex22.o ex22.c
cc -Wall -g -DNDEBUG ex22_main.c ex22.o -o ex22_main

# Run ex22
./ex22_main


