#!/bin/bash

clear
gcc -o output/str.o src/util/str.c -c -g
gcc -o output/checks.o src/util/checks.c -c -g
gcc -o output/args.o src/util/args.c -c -g
gcc -o output/node.o src/util/node.c -c -g
gcc -o output/linked_list.o src/util/linked_list.c -c -g
gcc -o output/color_changer.o src/util/color_changer.c -c -g
gcc -o output/main.o src/main.c -c -g
gcc output/str.o output/checks.o output/args.o output/node.o output/linked_list.o output/color_changer.o output/main.o -o c-color-checker -g
