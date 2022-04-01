all: banker.c
	gcc -Wall -Wextra -std=c99 -pthread banker.c -o banker
	./banker 10 5 7