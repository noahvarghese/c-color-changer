#!/bin/bash

clear
echo "Deleting test files..."

rm test/*_modified.png > /dev/null 2>&1

gcc -o output/str.o src/util/str.c -c -g
gcc -o output/checks.o src/util/checks.c -c -g
gcc -o output/args.o src/util/args.c -c -g
gcc -o output/string_node.o src/linked_list/string_node.c -c -g
gcc -o output/string_linked_list.o src/linked_list/string_linked_list.c -c -g
gcc -o output/color_node.o src/linked_list/color_node.c -c -g
gcc -o output/color_linked_list.o src/linked_list/color_linked_list.c -c -g
gcc -o output/image.o src/image/image.c -c -g
gcc -o output/png.o src/image/png.c -c -g
gcc -o output/pixel.o src/image/pixel.c -c -g
gcc -o output/color_changer.o src/util/color_changer.c -c -g
gcc -o output/path.o src/path/path.c -c -g
gcc -o output/main.o src/main.c -c -g

gcc output/str.o output/checks.o output/args.o output/string_node.o    \
output/string_linked_list.o output/color_node.o output/color_linked_list.o  \
output/pixel.o output/png.o output/image.o output/color_changer.o \
output/path.o output/main.o \
-o c-color-changer -g -lpng -lm

echo "Build completed"
