
SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

CC=gcc
CFLAGS=-c -Wall
LDFLAGS= 


OUT = fbgc
all: fbgc

#fbgc: tokens.o double_object.o fbgc_object.o fbgc.o
#	$(CC) tokens.o fbgc_object.o double_object.o fbgc.o -o fbgc

$(OUT): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@
	$(info    "Succesfully compiled")

clean:
	rm -f fbgc $(OBJ)