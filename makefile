CC = gcc
CFLAGS = -g -Wall -std=c99

main: main.c
	$(CC) CFLAGS main.c -I.
