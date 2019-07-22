.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=signal sendsig alarm sigstatus sigaction sigqueue unblock relsig
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
