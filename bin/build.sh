#!/bin/bash

clear
gcc -o output/checks.o src/util/checks.c -c -g
gcc -o output/args.o src/util/args.c -c -g
gcc -o output/main.o src/main.c -c -g
gcc output/checks.o output/args.o output/main.o -o c-color-checker -g
