#ifndef BYTECODE_H
#define BYTECODE_H
typedef unsigned char bytecode;

enum BYTECODE_LIST {
   nop, // 0 
   ipush,ipush_0,ipush_1,ipush_2,ipush_3,
   iload,iload_0,iload_1,iload_2,iload_3,
   istore,istore_0,istore_1,istore_2,istore_3,
   iarray,
   iastore,iastore_0,iastore_1,iastore_2,iastore_3,
   iaload,iaload_0,iaload_1,iaload_2,iaload_3,
   //MATH
   add, sub, mult, divd, inc1, dec1,power,ineg,
   //STACK
   pop,dup,swap,
   //COMPARISION
   eq,ne,lt,le,gt,ge,
   //CONTROL
   jump,stop,
   ireturn,irange,
   bclast
};
const unsigned char bc_operand[] = { 
   0,
   2,0,0,0,0,
   2,0,0,0,0,
   2,0,0,0,0,
   0,
   2,0,0,0,0,
   2,0,0,0,0,
   0,0,0,0,2,2,0,0,
   0,0,0,
   2,2,2,2,2,2,
   2,0,
   0,6,
   0
};
const  char *bc_strings[] = {
   "nop", // 0 
   "ipush","ipush_0","ipush_1","ipush_2","ipush_3",
   "iload","iload_0","iload_1","iload_2","iload_3",
   "istore","istore_0","istore_1","istore_2","istore_3",
   "iarray",
   "iastore","iastore_0","iastore_1","iastore_2","iastore_3",
   "iaload","iaload_0","iaload_1","iaload_2","iaload_2",
   //MATH
   "add", "sub", "mult", "divd", "inc1", "dec1","power","ineg",
   //STACK
   "pop","dup","swap",
   //COMPARISION
   "eq","ne","lt","le","gt","ge",
   //CONTROL
   "jump","stop",
   "ireturn","irange", 
   "bclast"
};
const char* bc2s(const int bcode){return bc_strings[bcode];}

#endif