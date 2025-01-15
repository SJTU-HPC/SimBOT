CC=mpicc

all:
	$(CC) *.c -o pfhsim -lgsl -lopenblas -lLogGOPSim -O3 -lm
clean:
	rm -f *.o
	rm -f pfhsim
