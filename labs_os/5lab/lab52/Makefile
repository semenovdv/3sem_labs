#prog: main2.o libqueue.so
#	gcc -o prog2 main2.o -L. -lqueue -rdynamic -ldl -Wl,-rpath,. #передать опцию с аргументом, добавляет текущий каталог в поиск
	# L - где искать библиотеку

main2: main2.c
	gcc -ldl main2.c