
SRC = $(shell find src/*.c cmodules/*.c)
#MODULES = $(wildcard cmodules/*.c)
OBJ = ${SRC:.c=.o}

#put -g in order to see good assembly output
CC=gcc
CFLAGS=-c -lm -Os -g -w
LDFLAGS += -lm 

#CFLAGS += -DCLOSE_CPRINTF
#CFLAGS += -DLEXER_DETAILED_DEBUG
#CFLAGS += -DLEXER_DEBUG
#CFLAGS += -DGRAMMAR_DEBUG
#CFLAGS += -DPARSER_DEBUG
#CFLAGS += -DINTERPRETER_DEBUG
#CFLAGS += -DFREE_DEBUG
#CFLAGS += -DMEM_DEBUG
#CFLAGS += -DSYMBOL_TABLE_DEBUG
#CFLAGS += -DTUPLE_DEBUG
#CFLAGS += -DARRAY_DEBUG
#CFLAGS += -DFUN_PRINT_DETAILED

TEST_OBJ = ${TEST_SRC:.c=.o}

OUT = fbgc
all: fbgc

$(OUT): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

#$(OUT): $(TEST_OBJ)
#	$(CC) $^ $(LDFLAGS) -o $@


%.o: %.c
	@$(CC) $(CFLAGS) $< -o $@ 
	$(info "Succesfully compiled:" $<)

clean:
	rm -f $(OBJ)


#make clean && make && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./fbgc
