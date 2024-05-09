CC=gcc
CFLAGS=-Wall -Wextra -g   

SRCS=USPassignment.c functions.c
OBJS=$(SRCS:.c=.o)
DEPS=functions.h

TARGET=mssv

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
