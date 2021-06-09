#!/bin/bash

clear
gcc -o output/checks.o src/util/checks.c -c -g
gcc -o output/args.o src/util/args.c -c -g
gcc -o output/node.o src/util/node.c -c -g
gcc -o output/linked_list.o src/util/linked_list.c -c -g
gcc -o output/main.o src/main.c -c -g
gcc output/checks.o output/args.o output/node.o output/linked_list.o output/main.o -o c-color-checker -g
