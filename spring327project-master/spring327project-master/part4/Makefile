all: advance check winnable game

check: project1.o
	gcc project1.o -o check

winnable: project3.o pt3utils.o
	gcc project3.o pt3utils.o -o winnable

project3.o: ../part3/project3.c ../part3/pt3utils.h
	gcc -c ../part3/project3.c

pt3utils.o: ../part3/pt3utils.c ../part3/pt3utils.h
	gcc -c ../part3/pt3utils.c

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

game: project4.o pt4utils.o
	gcc project4.o pt4utils.o -o game /share/cs327/lib/libtermbox.a

pt4utils.o: pt4utils.c pt4utils.h
	gcc -c pt4utils.c

project4.o: project4.c
	gcc -c -I/share/cs327/include project4.c

clean:
	rm *.o check advance winnable game
