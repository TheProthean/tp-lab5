sqlfunctions.o:
	gcc -c src/sqlfunctions.c -o build/sqlfunctions.o

userinterface.o:
	gcc -c src/userinterface.c -o build/userinterface.o

build: sqlfunctions.o userinterface.o
	gcc -o bin/task_5 build/sqlfunctions.o build/userinterface.o -lsqlite3

run: build
	bin/task_5

clean:
	rm bin/* build/*

