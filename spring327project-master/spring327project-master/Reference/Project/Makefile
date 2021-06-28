all: info change rankmoves edit

# executables
edit: edit.o helpers.o moves.o errors.o config.o render.o
	gcc -o edit edit.o helpers.o moves.o errors.o config.o render.o -ltermbox

rankmoves: rankmoves.o rank.o helpers.o moves.o errors.o config.o
	gcc -o rankmoves rankmoves.o rank.o helpers.o moves.o errors.o config.o

change: change.o config.o helpers.o moves.o errors.o
	gcc -o change change.o config.o helpers.o moves.o errors.o

info: info.o errors.o config.o helpers.o moves.o
	gcc -o info info.o errors.o config.o helpers.o

# main run files
edit.o: edit.c headers/config.h headers/helpers.h headers/errors.h  headers/moves.h headers/render.h
	gcc -c edit.c

rankmoves.o: rankmoves.c headers/rank.h headers/config.h headers/helpers.h headers/errors.h  headers/moves.h
	gcc -c rankmoves.c

change.o: change.c headers/config.h headers/helpers.h headers/errors.h  headers/moves.h
	gcc -c change.c

info.o: info.c headers/helpers.h headers/config.h headers/errors.h headers/moves.h
	gcc -c info.c

# functionaliity code
render.o: render.c headers/render.h
render.o: headers/config.h headers/helpers.h headers/errors.h headers/moves.h
	gcc -c render.c

rank.o: rank.c headers/rank.h
rank.o: headers/moves.h headers/config.h headers/helpers.h headers/errors.h
	gcc -c rank.c

moves.o: moves.c headers/moves.h
moves.o: headers/config.h headers/helpers.h headers/errors.h
	gcc -c moves.c

config.o: config.c headers/config.h
config.o: headers/errors.h  headers/helpers.h headers/moves.h
	gcc -c config.c

# support code
errors.o: errors.c headers/errors.h
	gcc -c errors.c

helpers.o: helpers.c headers/helpers.h headers/config.h headers/moves.h
	gcc -c helpers.c

clean:
	rm info.o errors.o helpers.o config.o info change.o change moves.o rankmoves.o rank.o rankmoves edit.o edit render.o
