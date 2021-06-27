#!/bin/bash

clear
echo "Deleting test files..."

# rm test/*_modified.png > /dev/null 2>&1
# ./c-color-changer -i ffffff80 -c fea800ff -p ./test
rm single/*_modified.png > /dev/null 2>&1
./c-color-changer -i ffffff80 -c fea800ff -p ./single