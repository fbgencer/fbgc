#ifndef FBGC_OPCODES_OBJECTH
#define FBGC_OPCODES_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

//load and assign opcode object

struct fbgc_id_opcode{
    struct fbgc_object base;
    int loc;
    uint8_t flag;
};

#define sizeof_fbgc_id_opcode(x)(sizeof(struct fbgc_id_opcode))

#define ID_FLAG_GLOBAL 1
#define ID_FLAG_LOCAL 2
#define ID_FLAG_MEMBER 3 
#define ID_FLAG_PUSH_ITSELF 4 // x=y=10 like enterance can be solved with this but it could be a problem..

//This flags will be used by the assignment operator!, It uses MSB 4-bit of the flag
//Notice that we use the same numbers as the operators


#define ID_RSHIFT_ASSIGN R_SHIFT
#define ID_LSHIFT_ASSIGN L_SHIFT
#define ID_STARSTAR_ASSIGN STARSTAR
#define ID_SLASHSLASH_ASSIGN SLASHSLASH
#define ID_PLUS_ASSIGN PLUS
#define ID_MINUS_ASSIGN MINUS
#define ID_STAR_ASSIGN STAR
#define ID_SLASH_ASSIGN SLASH
#define ID_CARET_ASSIGN CARET
#define ID_PERCENT_ASSIGN PERCENT


#define cast_fbgc_object_as_id_opcode(x)((struct fbgc_id_opcode *) x)
#define get_id_opcode_loc(x)(cast_fbgc_object_as_id_opcode(x)->loc)
#define set_id_opcode_loc(x,y)(cast_fbgc_object_as_id_opcode(x)->loc = y)



#define set_id_flag_GLOBAL(x)(cast_fbgc_object_as_id_opcode(x)->flag = ID_FLAG_GLOBAL )
#define set_id_flag_LOCAL(x)(cast_fbgc_object_as_id_opcode(x)->flag = ID_FLAG_LOCAL )
#define set_id_flag_MEMBER(x)(cast_fbgc_object_as_id_opcode(x)->flag = ID_FLAG_MEMBER )
#define set_id_flag_PUSH_ITSELF(x)(cast_fbgc_object_as_id_opcode(x)->flag |= ID_FLAG_PUSH_ITSELF )


#define is_id_flag_GLOBAL(x)(cast_fbgc_object_as_id_opcode(x)->flag & ID_FLAG_GLOBAL )
#define is_id_flag_LOCAL(x)(cast_fbgc_object_as_id_opcode(x)->flag & ID_FLAG_LOCAL )
#define is_id_flag_MEMBER(x)(cast_fbgc_object_as_id_opcode(x)->flag & ID_FLAG_MEMBER )
#define is_id_flag_PUSH_ITSELF(x)(cast_fbgc_object_as_id_opcode(x)->flag & ID_FLAG_PUSH_ITSELF )

#define get_id_flag(x)(cast_fbgc_object_as_id_opcode(x)->flag)

struct fbgc_object * new_fbgc_id_opcode(int loc);



#ifdef  __cplusplus
}
#endif

#endif
