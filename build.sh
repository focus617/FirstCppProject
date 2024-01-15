#! /bin/sh

cmake --build build/debug --target install
# cmake --build build/release --target install
cmake --build build/debug -t doc_doxygen