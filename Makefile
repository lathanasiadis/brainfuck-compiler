bftomips: listutils.o bftomips.o
	gcc -Wall -g listutils.o bftomips.o -o bftomips	

listutils.o: listutils.c listutils.h
	gcc -Wall -g -c listutils.c

bftomips.o: bftomips.c listutils.h
	gcc -Wall -g -c bftomips.c
