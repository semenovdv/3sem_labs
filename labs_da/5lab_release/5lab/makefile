prog: TST.h main.o TST.o
	g++ -std=c++11 -Wall -pedantic -Werror -g -Wall -Wno-sign-compare -Wno-long-long -lm -o 5lab main.o TST.o
main.o: main.cpp TST.h
	g++ -std=c++11 -Wall -pedantic -Werror -g -Wall -Wno-sign-compare -Wno-long-long -lm -c -o main.o main.cpp
TST.o: TST.h TST.cpp
	g++ -std=c++11 -Wall -pedantic -Werror -g -Wall -Wno-sign-compare -Wno-long-long -lm -c -o TST.o TST.cpp