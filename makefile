SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

CC = gcc
PREFIX = /usr/local
CFLAGS = -Wall
LDFLAGS += -lm

# deps

#CFLAGS += -I deps/linenoise
#OBJ += deps/linenoise/linenoise.o
#OBJ += deps/linenoise/utf8.o

# test

#TEST_SRC = $(shell find src/*.c test/*.c | sed '/fbgc/d')
#TEST_OBJ = ${TEST_SRC:.c=.o}

CFLAGS += -I src


OUT = fbgc
ifdef SystemRoot
	OUT = fbgc.exe
endif

$(OUT): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@
	@printf "Finished succesfully..\n" $@


#test: test_runner
#	@./$<

#test_runner: $(TEST_OBJ)
#	$(CC) $^ $(LDFLAGS) -o $@

install: fbgc
	install fbgc $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/bin/fbgc

clean:
	rm -f fbgc test_runner $(OBJ) $(TEST_OBJ)

.PHONY: clean test install uninstall