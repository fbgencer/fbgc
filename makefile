#fbgc makefile, written by fbgencer
#In order to compile normal fbgc just write "make"

SRC = $(shell find src/*.c cmodules/*.c)
OBJ = ${SRC:.c=.o}

TEST_SRC = $(shell find src/*.c cmodules/*.c module_test/*.c )
TEST_OBJ = ${TEST_SRC:.c=.o}

#put -g in order to see good assembly output
#-Wno-error=incompatible-pointer-types -Wno-error=discarded-qualifiers
CC=gcc
#-Wunused-variable
OPTIMIZATION_FLAG = 
WARNING_FLAG = -Werror -Wno-error=incompatible-pointer-types 
DEBUG_FLAG = -s

CFLAGS = -c
LDFLAGS += -lm
CFLAGS += $(OPTIMIZATION_FLAG)
CFLAGS += $(WARNING_FLAG)
CFLAGS += $(DEBUG_FLAG)


#GSL_CFLAG = -I /home/fbgencer/gsl/include
#GSL_LDFLAG = -L /home/fbgencer/gsl/lib -lgsl -lgslcblas

OPENBLAS_CFLAG = -I /home/fbgencer/openblas/include
OPENBLAS_LDFLAG = -L /home/fbgencer/openblas/lib -lopenblas -lpthread -lgfortran

LIB_CFLAGS += $(OPENBLAS_CFLAG)
LIB_LDFLAGS += $(OPENBLAS_LDFLAG)

CFLAGS += $(LIB_CFLAGS)
LDFLAGS += $(LIB_LDFLAGS)

#DELETE THE LAST UNDERSCORE TO OPEN LOG PRINTING FOR A SPECIFIC H FILE
CFLAGS += -DLOG_ARRAY_OBJECT_
CFLAGS += -DLOG_CLASS_OBJECT_
CFLAGS += -DLOG_CMODULE_OBJECT_
CFLAGS += -DLOG_COMPLEX_OBJECT_
CFLAGS += -DLOG_CSTRUCT_OBJECT_
CFLAGS += -DLOG_DOUBLE_OBJECT_
CFLAGS += -DLOG_FBGC_OBJECT_
CFLAGS += -DLOG_FIELD_OBJECT_
CFLAGS += -DLOG_FUN_OBJECT_
CFLAGS += -DLOG_GRAMMAR_
CFLAGS += -DLOG_INT_OBJECT_
CFLAGS += -DLOG_INTERPRETER_
CFLAGS += -DLOG_LINKEDLIST_
CFLAGS += -DLOG_LOGIC_OBJECT_
CFLAGS += -DLOG_MATRIX_OBJECT_
CFLAGS += -DLOG_MEMORY_
CFLAGS += -DLOG_OPERATOR_
CFLAGS += -DLOG_PARSER_
CFLAGS += -DLOG_RANGE_OBJECT_
CFLAGS += -DLOG_RELEXER
CFLAGS += -DLOG_STR_OBJECT_
CFLAGS += -DLOG_SYMBOL_TABLE_
CFLAGS += -DLOG_TOKENS_
CFLAGS += -DLOG_TUPLE_OBJECT_


#In order run "make test" open this flag
CFLAGS += -DMODULE_TEST_


CFLAGS += -DFBGC_LOGO_

#LDFLAGS += -lm 
#OPTIMIZATION_FLAGS = -foptimize-strlen 

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

%.o:%.c
	@$(CC) $(CFLAGS) $< -o $@


OUT = fbgc
all: fbgc
$(OUT): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) 

#$(info "Succesfully compiled:")


TEST_OUT = fbgc_test
test: fbgc_test
#TESTFLAGS += -DTEST

$(TEST_OUT): $(TEST_OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) $(TESTFLAGS) 

#$(info "Succesfully compiled:")



valgrind:
	make clean && make && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./fbgc
clean:
	@rm -f $(OBJ)

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
