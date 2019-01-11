

progr: main.o Octagon.o TNode.o TBTree.o Triangle.o Squarefig.o
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm main.o Octagon.o Triangle.o Squarefig.o TNode.o TBTree.o -o progr

main.o: main.cpp Figure.h Octagon.h Triangle.h Squarefig.h TBTree.h TNode.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c main.cpp

TBTree.o: TBTree.cpp TBTree.h TNode.o
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c TBTree.cpp

TNode.o: TNode.cpp TNode.h Octagon.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c TNode.cpp

Octagon.o: Octagon.cpp Octagon.h Figure.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o Octagon.o -c Octagon.cpp 

Triangle.o: Triangle.cpp Triangle.h Figure.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o Triangle.o -c Triangle.cpp

Squarefig.o: Squarefig.cpp Squarefig.h Figure.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o Squarefig.o -c Squarefig.cpp

clean:
	rm -rf *.o progr