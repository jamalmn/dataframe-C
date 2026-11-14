CC      ?= gcc
CFLAGS  ?= -std=gnu11 -Wall -Wextra -g
TARGET  := programa
SRCS    := main.c lib.c
OBJS    := $(SRCS:.c=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
