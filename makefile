compile : 
	gcc csu-batch.c command-parser.c job-queue.c help2.c scheduling.c dispatching.c benchmark.c -lpthread -o main -Wall

debug :
	gcc csu-batch.c command-parser.c job-queue.c help2.c scheduling.c dispatching.c benchmark.c -lpthread -o main -Wall -g

clean :
	rm main