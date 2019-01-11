prog: main.o TRBTree.o Node.h TRBTree.h 
	g++ -std=c++11 -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g -o 2lab main.o TRBTree.o
TRBTree.o: TRBTree.cpp Node.h TRBTree.h
	g++ -std=c++11 -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g -c  -o TRBTree.o TRBTree.cpp
main.o: main.cpp Node.h TRBTree.h
	g++ -std=c++11 -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g -c -o main.o  main.cpp