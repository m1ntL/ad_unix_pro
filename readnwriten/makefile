.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=clt srv readlinesrv echosrv echoclt
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
