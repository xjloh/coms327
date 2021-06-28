all: advance check winnable

check: project1.o 
	gcc project1.o -o check

winnable: project3.o pt3utils.o
	gcc project3.o pt3utils.o -o winnable

project3.o: project3.c pt3utils.h
	gcc -c project3.c

pt3utils.o: pt3utils.c pt3utils.h
	gcc -c pt3utils.c

project1.o: ../project1.c
	gcc -c ../project1.c

advance: project2.o formatChecker.o utils.o
	gcc project2.o formatChecker.o utils.o -o advance

project2.o: ../part2/project2.c
	gcc -c ../part2/project2.c

formatChecker.o: ../part2/formatChecker.c
	gcc -c ../part2/formatChecker.c

utils.o: ../part2/utils.c
	gcc -c ../part2/utils.c

clean:
	rm *.o check advance winnable
