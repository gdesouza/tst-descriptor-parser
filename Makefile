parser: main.o
	gcc main.o -o parser -g

main.o: main.c
	gcc -c main.c -g

clean:
	rm *.o parser