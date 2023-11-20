files := main.cpp socket.cpp writeable.cpp connection.cpp http.cpp server.cpp
CC := g++
CFlags := -Wall -Wextra -Wpedantic -std=c++23 -I../include

build:
	cd src/;\
	$(CC) $(CFlags) $(files) -o ../bin/main;\
	cd ..

run:
	./bin/main

