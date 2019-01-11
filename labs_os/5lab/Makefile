prog: main.o libqueue.so
	gcc -o prog main.o -L. -lqueue -Wl,-rpath,. #передать опцию с аргументом, добавляет текущий каталог в поиск
	# L - где искать библиотеку

main.o: main.c
	gcc -c main.c

libqueue.so: queue.o
	gcc -shared -o libqueue.so queue.o

queue.o: queue.c
	gcc -c -fPIC queue.c #позиционно независимый код, плавающие адреса

clean:
	rm -f *.o *.so binary