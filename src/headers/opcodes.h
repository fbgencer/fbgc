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

#define ID_FLAG_GLOBAL 0x01 //0b00000001
#define ID_FLAG_LOCAL 0x02 //0b00000010
#define ID_FLAG_MEMBER 0x04 //0b00000100
#define ID_FLAG_PUSH_ITSELF 0x10 // x=y=10 like enterance can be solved with this but it could be a problem..

//This flags will be used by the assignment operator!, It uses MSB 4-bit of the flag





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
