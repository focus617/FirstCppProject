#! /bin/sh

# cmake -DCMAKE_CXX_COMPILER=clang-15 -DCMAKE_BUILD_TYPE=debug -S . -B build/debug

cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=debug -S . -B build/debug
# cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=release -S . -B build/release