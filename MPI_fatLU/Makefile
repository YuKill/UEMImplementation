all: seq par 

seq: main.c
	gcc main.c -o seq

par: main_parallel.c
	mpicc main_parallel.c -o par

clean:
	rm seq par
