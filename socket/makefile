.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=endian ipbase bind server client concursrv shortconn p2pclt p2psrv
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
