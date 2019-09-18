compile : 
	gcc csu-batch.c command-parser.c job-queue.c -o main -Wall

compile-d :
	gcc csu-batch.c command-parser.c job-queue.c -o main -Wall -g

run :
	./main

run-t:
	./main < test

clean :
	rm main