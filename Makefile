CC = g++
COV = -fprofile-arcs -ftest-coverage

main : main.cpp
		${CC} ${COV} main.cpp -o ${@}
