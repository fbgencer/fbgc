
SRC = $(shell find src/*.c cmodules/*.c)
#MODULES = $(wildcard cmodules/*.c)
OBJ = ${SRC:.c=.o}

CC=gcc
CFLAGS=-c -Wall -lm
LDFLAGS += -lm 

TEST_OBJ = ${TEST_SRC:.c=.o}

OUT = fbgc
all: fbgc

$(OUT): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

#$(OUT): $(TEST_OBJ)
#	$(CC) $^ $(LDFLAGS) -o $@


%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@ 
	$(info    "Succesfully compiled")


clean:
	rm -f fbgc $(OBJ)