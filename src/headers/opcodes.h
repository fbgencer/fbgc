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


#define ID_FLAG_GLOBAL 0x01 //0b00000001
#define ID_FLAG_LOCAL 0x02 //0b00000010
#define ID_FLAG_SUBSCRIPT 0x04 //0b00000100
#define ID_FLAG_UNUSED 0x08 //0b00001000

#define cast_fbgc_object_as_id_opcode(x)((struct fbgc_id_opcode *) x)

#define set_id_flag_GLOBAL(x)(cast_fbgc_object_as_id_opcode(x)->flag = ID_FLAG_GLOBAL )
#define set_id_flag_LOCAL(x)(cast_fbgc_object_as_id_opcode(x)->flag = ID_FLAG_LOCAL )
#define set_id_flag_SUBSCRIPT(x)(cast_fbgc_object_as_id_opcode(x)->flag = ID_FLAG_SUBSCRIPT )

#define is_id_flag_GLOBAL(x)(cast_fbgc_object_as_id_opcode(x)->flag & ID_FLAG_GLOBAL )
#define is_id_flag_LOCAL(x)(cast_fbgc_object_as_id_opcode(x)->flag & ID_FLAG_LOCAL )
#define is_id_flag_SUBSCRIPT(x)(cast_fbgc_object_as_id_opcode(x)->flag & ID_FLAG_SUBSCRIPT )



struct fbgc_object * new_fbgc_id_opcode(int loc);



#ifdef  __cplusplus
}
#endif

#endif
