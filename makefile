compile : 
	gcc command-parser.c -o main -Wall

compile-d :
	gcc command-parser.c -o main -Wall -g

run :
	./main

run-t:
	./main < test

clean :
	rm main