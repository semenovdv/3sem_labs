

progr: main.o Octagon.o Squarefig.o Triangle.o
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm main.o Octagon.o Squarefig.o Triangle.o -o progr

main.o: main.cpp
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c main.cpp

Octagon.o: Octagon.cpp Octagon.h Figure.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c Octagon.cpp

Squarefig.o: Squarefig.cpp Squarefig.h Figure.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c Squarefig.cpp

Triangle.o: Triangle.cpp Triangle.h Figure.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c Triangle.cpp

clean:
	rm -rf *.o progr