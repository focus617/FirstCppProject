#! /bin/sh

cmake --build build/debug --config Debug --target install
# cmake --build build/release --target install

# Other command:
# cmake --build build/debug -t code_clangformat
# cmake --build build/debug -t doc_doxygen