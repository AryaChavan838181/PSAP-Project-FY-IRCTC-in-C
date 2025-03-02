CC = gcc
CXX = g++
CFLAGS = -I/usr/include/mysql -Wall -Wextra -g
LDFLAGS = -L/usr/lib/mysql -lmysqlclient

all: main

main: main.o user.o booking.o database.o
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

user.o: user.c
	$(CC) $(CFLAGS) -c $<

booking.o: booking.c
	$(CC) $(CFLAGS) -c $<

database.o: database.cpp
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -f *.o main
