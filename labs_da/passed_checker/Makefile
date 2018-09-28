1lab: main.o vector.o KeyValue.h
	g++ -std=c++11 -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o 1lab main.o vector.o

main.o: main.cpp KeyValue.h
	g++ -c -std=c++11 -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm main.cpp

vector.o: vector.cpp KeyValue.h
	g++ -c -std=c++11 -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm vector.cpp