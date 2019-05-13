CFLAGS = -Wall -g -lm
CC = gcc
EXEC = 

compil: echeancier.o
	$(CC) file1.c *.o -o file1 $(CFLAGS)
	$(CC) file2.c *.o -o file2 $(CFLAGS)
	$(CC) file3.c *.o -o file3 $(CFLAGS)

courbeR:
	R CMD BATCH script.R

run-simul: compil
	./file1
	./file2
	./file3

echeancier.o: echeancier.h
	$(CC) -c echeancier.c -o echeancier.o

clean:
	rm -f file1 file2 file3 echeancier.o
	rm -f .RData script.Rout
