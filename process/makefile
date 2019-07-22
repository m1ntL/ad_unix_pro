.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=hello exec_demo proc_term mydaemon wait mulfork system 8.6wait twofork
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
