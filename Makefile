all: malloc.o
	 gcc malloc.o -o malloc

malloc.o: malloc.c
		 gcc -c malloc.c -o malloc.o

clean: 
	  rm malloc.o malloc a.out