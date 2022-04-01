#!/bin/sh

cd build
cmake ..
make
echo "Removing cache files..."
rm -rf *