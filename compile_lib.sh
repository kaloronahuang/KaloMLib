# compile_lib.sh
clang++ -c -o KaloMLib/matrix.o KaloMLib/matrix.cpp -g3

ar rcs KaloMLib/libkaloml.a KaloMLib/matrix.o