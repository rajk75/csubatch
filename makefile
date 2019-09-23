csu-batch.o : 
	gcc csu-batch.c command-parser.c job-queue.c help2.c scheduling.c dispatching.c -lpthread -o main -Wall

compile-d :
	gcc csu-batch.c command-parser.c job-queue.c help2.c scheduling.c dispatching.c -lpthread -o main -Wall -g

run :
	./main

run-t:
	./main < test

clean :
	rm main