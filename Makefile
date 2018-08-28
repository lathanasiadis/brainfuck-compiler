#Users don't need debugging information in their binaries
#Omitting them makes the executable size noticably smaller.
#
#It also makes sense to use the same names for the object files and 
#executables, since noone will have both "versions" in the same folder

user: u_listutils.o u_bftomips.o
	gcc -Wall listutils.o bftomips.o -o bftomips	  

u_listutils.o: listutils.c listutils.h
	gcc -Wall -c listutils.c

u_bftomips.o: bftomips.c listutils.h
	gcc -Wall -c bftomips.c

dev: listutils.o bftomips.o
	gcc -Wall -g listutils.o bftomips.o -o bftomips	

listutils.o: listutils.c listutils.h
	gcc -Wall -g -c listutils.c

bftomips.o: bftomips.c listutils.h
	gcc -Wall -g -c bftomips.c
	
.PHONY: clean
#delete object files and executable
clean:
	rm *.o bftomips
