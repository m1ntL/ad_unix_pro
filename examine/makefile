.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=sigexm mulpro mulpro_std sigtest sigtest1
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
