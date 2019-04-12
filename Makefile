sqlfunctions.o:
	gcc -std=c99 -c src/sqlfunctions.c -o build/sqlfunctions.o

userinterface.o:
	gcc -std=c99 -c src/userinterface.c -o build/userinterface.o

build: sqlfunctions.o userinterface.o
	gcc -std=c99 -o bin/task_5 build/sqlfunctions.o build/userinterface.o -lsqlite3

run: build
	bin/task_5

clean:
	rm bin/* build/*

