CC = gcc
CFLAGS = -Wall -g

all: USPassignment

USPassignment: USPassignment.o functions.o
	$(CC) $(CFLAGS) -o USPassignment USPassignment.o functions.o

USPassignment.o: USPassignment.c
	$(CC) $(CFLAGS) -c USPassignment.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f USPassignment USPassignment.o functions.o

.PHONY: all clean
