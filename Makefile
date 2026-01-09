CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -I/usr/include/postgresql
LIBS = -lpq

TARGET = student_app
SRC = src/main.c src/db.c

all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
