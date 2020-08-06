CC = g++
COV = -fprofile-arcs -ftest-coverage

.PHONY: dev test clean

dev: main

main : main.cpp
		${CC} main.cpp -o ${@}

test: main_test

main_test: main_test.cpp
		${CC} ${COV} main_test.cpp -o ${@} -L /usr/lib -I/usr/include

clean :
		rm -f main
		rm -f main_test
