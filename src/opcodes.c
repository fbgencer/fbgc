#include "fbgc.h"



struct fbgc_object * new_fbgc_id_opcode(int loc){
	struct fbgc_id_opcode * o = (struct fbgc_id_opcode *) fbgc_malloc(sizeof(struct fbgc_id_opcode));
	o->base.type = IDENTIFIER;
	o->loc = loc;
	o->flag = ID_FLAG_GLOBAL;
	
	return (struct fbgc_object *) o;
}	
