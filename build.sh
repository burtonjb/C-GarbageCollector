clang-format-3.4 -style=llvm -i gc.c
clang-format-3.4 -style=llvm -i gc.h
clang-format-3.4 -style=llvm -i tests.c
gcc -static -g -std=c99 -o bin/out gc.c tests.c && ./bin/out && \
valgrind --leak-check=full --track-origins=yes -v ./bin/out
