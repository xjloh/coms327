all: advance



advance: project2.o formatChecker.o utils.o
	gcc project2.o formatChecker.o utils.o -o advance

project2.o: project2.c
	gcc -c project2.c

formatChecker.o: formatChecker.c
	gcc -c formatChecker.c

utils.o: utils.c
	gcc -c utils.c

clean:
	rm advance project2.o formatChecker.o utils.o
