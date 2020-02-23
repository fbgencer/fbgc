
SRC = $(shell find src/*.c cmodules/*.c)
#MODULES = $(wildcard cmodules/*.c)
OBJ = ${SRC:.c=.o}

#put -g in order to see good assembly output
CC=gcc
CFLAGS=-c -Os
LDFLAGS += -lm 
#OPTIMIZATION_FLAGS = -foptimize-strlen 


#CFLAGS += $(OPTIMIZATION_FLAGS)

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

#CFLAGS += -DTIME_DEBUG

#CFLAGS += -DDEBUG_TO_FILE


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


valgrind:
	make clean && make && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./fbgc
clean:
	rm -f $(OBJ)

# blas and lapack libraries
#libs = -L/usr/lib/ -llapack -lblas


# rm -r *.expand
# rm -r *.vregs
# rm -r *.info_cfglayout
# rm -r *.into_cfglayout
# rm -r *.jump
# rm -r *.subreg1
# rm -r *.dfinit
# rm -r *.cse1
# rm -r *.ce1
# rm -r *.fwprop1
# rm -r *.mach
# rm -r *.rtl_dce
# rm -r *.bbro
# rm -r *.split4
# rm -r *.gcse2
# rm -r *.split1
# rm -r *.split2
# rm -r *.split3
# rm -r *.ree
# rm -r *.cprop1
# rm -r *.pre
# rm -r *.cse_local
# rm -r *.loop2
# rm -r *.loop2_invariant
# rm -r *.stv1
# rm -r *.cse2
# rm -r *.dse1
# rm -r *.dse2
# rm -r *.cprop2
# rm -r *.loop2_init
# rm -r *.cprop3
# rm -r *.combine
# rm -r *.ce2
# rm -r *.reginfo
# rm -r *.loop2_done
# rm -r *.fwprop2
# rm -r *.init-regs
# rm -r *ud_dce
# rm -r *.bbpart
# rm -r *.outof_cfglayout
# rm -r *.subreg2
# rm -r *.mode_sw
# rm -r *.ira
# rm -r *.asmcons
# rm -r *.reload
# rm -r *.cmpelim
# rm -r *.postreload
# rm -r *.postreload_jump
# rm -r *.csa
# rm -r *.jump2
# rm -r *.ce3
# rm -r *.peephole2
# rm -r *.stack
# rm -r *.vartrack
# rm -r *.barriers
# rm -r *.cet
# rm -r *.final
# rm -r *.dfinish
# rm -r *.nothrow
# rm -r *.shorten
# rm -r *.alignments
# rm -r *.sched2
# rm -r *.dwarf2
# rm -r *.compgotos
# rm -r *.pro_and_epilogue
# rm -r *.cprop_hardreg
