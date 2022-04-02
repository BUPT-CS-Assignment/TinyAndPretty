#!/bin/sh

mkdir build
cd build
cmake ..
make
echo "Removing cache files..."
rm -rf *
cd ..
cd bin
./taptap