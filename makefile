
SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

CC=gcc
CFLAGS=-c -Wall -lm
LDFLAGS= 


OUT = fbgc
all: fbgc

$(OUT): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@ 
	$(info    "Succesfully compiled")

clean:
	rm -f fbgc $(OBJ)