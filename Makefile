#Users don't need debugging information in their binaries
#Omitting them makes the executable size noticably smaller.
#
#It also makes sense to use the same names for the object files and 
#executables, since noone will have both "versions" in the same folder

user: u_stackutils.o u_bftomips.o
	gcc -Wall stackutils.o bftomips.o -o bftomips	  

u_stackutils.o: stackutils.c stackutils.h
	gcc -Wall -c stackutils.c

u_bftomips.o: bftomips.c stackutils.h
	gcc -Wall -c bftomips.c

dev: stackutils.o bftomips.o
	gcc -Wall -g stackutils.o bftomips.o -o bftomips	

stackutils.o: stackutils.c stackutils.h
	gcc -Wall -g -c stackutils.c

bftomips.o: bftomips.c stackutils.h
	gcc -Wall -g -c bftomips.c
	
.PHONY: clean
#delete object files and executable
clean:
	rm *.o bftomips
