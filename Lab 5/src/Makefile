compile:
	gcc -c -Wall -Werror -fpic lib1.c
	gcc -c -Wall -Werror -fpic lib2.c
	gcc -shared -o liblib1.so lib1.o -lm
	gcc -shared -o liblib2.so lib2.o -lm
	gcc -Wall -o main.out main.c

clean:
	rm *.o
	rm *.so
	rm *.out