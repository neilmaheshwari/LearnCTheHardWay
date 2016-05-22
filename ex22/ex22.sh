#!/bin/sh

set -e

# Compile ex22.c and ex22_main.c
cc -Wall -g -DNDEBUG -c -o ex22.o ex22.c
cc -Wall -g -DNDEBUG ex22_main.c ex22.o -o ex22_main

# Run ex22
./ex22_main


