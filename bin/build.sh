#!/bin/bash

clear
echo "Deleting test files..."

rm test/*_modified.png > /dev/null 2>&1

gcc -o output/str.o src/util/str.c -c -g
gcc -o output/checks.o src/util/checks.c -c -g
gcc -o output/args.o src/util/args.c -c -g
gcc -o output/node.o src/linked-list/node.c -c -g
gcc -o output/linked_list.o src/linked-list/linked_list.c -c -g
gcc -o output/image.o src/util/image.c -c -g
gcc -o output/color_changer.o src/util/color_changer.c -c -g
gcc -o output/path.o src/path/path.c -c -g
gcc -o output/main.o src/main.c -c -g
gcc output/str.o output/checks.o output/args.o output/node.o output/linked_list.o output/image.o output/color_changer.o output/path.o output/main.o -o c-color-checker -g -lpng

echo "Build completed"
