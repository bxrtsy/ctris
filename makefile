CC = gcc
CFLAGS = -ansi -Wall -Wextra

TARGET = ctris

all:
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	rm $(TARGET)
