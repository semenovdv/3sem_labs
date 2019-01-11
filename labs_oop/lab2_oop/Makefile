

progr: main.o Octagon.o TNode.o TBTree.o
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm main.o Octagon.o TNode.o TBTree.o -o progr

main.o: main.cpp 
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c main.cpp

TBTree.o: TBTree.cpp TBTree.h TNode.o
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c TBTree.cpp

TNode.o: TNode.cpp TNode.h Octagon.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c TNode.cpp

Octagon.o: Octagon.cpp Octagon.h Figure.h
	g++ -std=c++11 -pedantic -Wall -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c Octagon.cpp

clean:
	rm -rf *.o progr