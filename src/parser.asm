
parser.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <compare_operators>:
	RIGHT_ASSOC | 17,//PERCENT_ASSIGN
	RIGHT_ASSOC | 17,//ASSIGN_SUBSCRIPT
	47,//LEN
};

uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
       0:	f3 0f 1e fa          	endbr64 
	


	_info("Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]);
       4:	48 8d 05 00 00 00 00 	lea    rax,[rip+0x0]        # b <compare_operators+0xb>	7: R_X86_64_PC32	object_name_array-0x4
       b:	40 0f b6 d6          	movzx  edx,sil
uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
       f:	41 56                	push   r14
	_info("Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]);
      11:	44 0f b6 f7          	movzx  r14d,dil
uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
      15:	41 55                	push   r13
	_info("Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]);
      17:	44 0f b6 ee          	movzx  r13d,sil
uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
      1b:	41 54                	push   r12
	// stack_top >= obj_type => return 1;
	// stack_top < obj_type => return 0;

	uint8_t result = 0;
	if(stack_top == IDENTIFIER) 
		result = 1;
      1d:	41 b4 01             	mov    r12b,0x1
uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
      20:	55                   	push   rbp
      21:	89 f5                	mov    ebp,esi
	_info("Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]);
      23:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 2a <compare_operators+0x2a>	26: R_X86_64_PC32	.LC0-0x4
uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
      2a:	53                   	push   rbx
	_info("Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]);
      2b:	48 8b 0c d0          	mov    rcx,QWORD PTR [rax+rdx*8]
      2f:	40 0f b6 d7          	movzx  edx,dil
uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
      33:	89 fb                	mov    ebx,edi
	_info("Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]);
      35:	48 8b 14 d0          	mov    rdx,QWORD PTR [rax+rdx*8]
      39:	bf 6f 00 00 00       	mov    edi,0x6f
      3e:	31 c0                	xor    eax,eax
      40:	e8 00 00 00 00       	call   45 <compare_operators+0x45>	41: R_X86_64_PLT32	cprintf-0x4
	if(stack_top == IDENTIFIER) 
      45:	80 fb 10             	cmp    bl,0x10
      48:	74 5e                	je     a8 <compare_operators+0xa8>
	//x=(1,2) requires low precedence!
	else if(obj_type == LBRACK || obj_type == LPARA) result =  0;
      4a:	44 8d 65 dd          	lea    r12d,[rbp-0x23]
      4e:	41 80 e4 fd          	and    r12b,0xfd
      52:	74 54                	je     a8 <compare_operators+0xa8>

	else if(obj_type >= IF && obj_type <= LEN && stack_top >= IF && stack_top <= LEN ){
      54:	83 ed 20             	sub    ebp,0x20
	else if(obj_type == LBRACK || obj_type == LPARA) result =  0;
      57:	45 31 e4             	xor    r12d,r12d
	else if(obj_type >= IF && obj_type <= LEN && stack_top >= IF && stack_top <= LEN ){
      5a:	40 80 fd 2f          	cmp    bpl,0x2f
      5e:	77 48                	ja     a8 <compare_operators+0xa8>
      60:	83 eb 20             	sub    ebx,0x20
      63:	80 fb 2f             	cmp    bl,0x2f
      66:	77 40                	ja     a8 <compare_operators+0xa8>
		//precedence of the operators have to change according to their positions
		//result = (operator_precedence(stack_top) >= operator_precedence(obj_type));
		stack_top = precedence_table[stack_top - IF];
      68:	41 83 ee 20          	sub    r14d,0x20
		obj_type = precedence_table[obj_type - IF];
      6c:	41 83 ed 20          	sub    r13d,0x20
		stack_top = precedence_table[stack_top - IF];
      70:	48 8d 05 00 00 00 00 	lea    rax,[rip+0x0]        # 77 <compare_operators+0x77>	73: R_X86_64_PC32	precedence_table-0x4
      77:	4d 63 f6             	movsxd r14,r14d
		obj_type = precedence_table[obj_type - IF];
      7a:	4d 63 ed             	movsxd r13,r13d
		stack_top = precedence_table[stack_top - IF];
      7d:	42 8a 0c 30          	mov    cl,BYTE PTR [rax+r14*1]
		obj_type = precedence_table[obj_type - IF];
      81:	46 8a 24 28          	mov    r12b,BYTE PTR [rax+r13*1]
		result = (0x7F & stack_top) > (0x7F & obj_type); // is there another way to perform this calculation ?? 
      85:	89 ca                	mov    edx,ecx
      87:	44 89 e0             	mov    eax,r12d
      8a:	83 e2 7f             	and    edx,0x7f
      8d:	83 e0 7f             	and    eax,0x7f
		if(result == 0 && obj_type == stack_top){
      90:	38 c2                	cmp    dl,al
      92:	77 05                	ja     99 <compare_operators+0x99>
      94:	44 38 e1             	cmp    cl,r12b
      97:	74 08                	je     a1 <compare_operators+0xa1>
		result = (0x7F & stack_top) > (0x7F & obj_type); // is there another way to perform this calculation ?? 
      99:	38 c2                	cmp    dl,al
      9b:	41 0f 97 c4          	seta   r12b
      9f:	eb 07                	jmp    a8 <compare_operators+0xa8>
			result = !(0x80 & obj_type); 
      a1:	41 f7 d4             	not    r12d
      a4:	41 c0 ec 07          	shr    r12b,0x7
		}
	}


	_info("%d\n",result&&1);
      a8:	41 0f b6 d4          	movzx  edx,r12b
      ac:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # b3 <compare_operators+0xb3>	af: R_X86_64_PC32	.LC1-0x4
      b3:	bf 6f 00 00 00       	mov    edi,0x6f
      b8:	31 c0                	xor    eax,eax
      ba:	e8 00 00 00 00       	call   bf <compare_operators+0xbf>	bb: R_X86_64_PLT32	cprintf-0x4

	return result;
}
      bf:	5b                   	pop    rbx
      c0:	44 89 e0             	mov    eax,r12d
      c3:	5d                   	pop    rbp
      c4:	41 5c                	pop    r12
      c6:	41 5d                	pop    r13
      c8:	41 5e                	pop    r14
      ca:	c3                   	ret    

00000000000000cb <handle_function_args>:


void handle_function_args(struct fbgc_ll_base * fun_holder,struct fbgc_ll_base * arg_end){
      cb:	f3 0f 1e fa          	endbr64 
      cf:	41 56                	push   r14
}

__fortify_function int
printf (const char *__restrict __fmt, ...)
{
  return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
      d1:	4c 8d 35 00 00 00 00 	lea    r14,[rip+0x0]        # d8 <handle_function_args+0xd>	d4: R_X86_64_PC32	.LC3-0x4
      d8:	41 55                	push   r13
      da:	41 54                	push   r12
      dc:	49 89 fc             	mov    r12,rdi
      df:	55                   	push   rbp
	//if(cast_fbgc_object_as_fun(fun_obj)->no_arg == 1)
	//	arg_end = arg_end->next;

	//Validate the arguments are they normal ?
	struct fbgc_ll_base * iter = fun_holder; //args start is iter->next
	while(iter != arg_end){
      e0:	4d 89 e5             	mov    r13,r12
void handle_function_args(struct fbgc_ll_base * fun_holder,struct fbgc_ll_base * arg_end){
      e3:	53                   	push   rbx
	struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;
      e4:	48 8b 6f 09          	mov    rbp,QWORD PTR [rdi+0x9]
void handle_function_args(struct fbgc_ll_base * fun_holder,struct fbgc_ll_base * arg_end){
      e8:	48 89 f3             	mov    rbx,rsi
      eb:	bf 01 00 00 00       	mov    edi,0x1
      f0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # f7 <handle_function_args+0x2c>	f3: R_X86_64_PC32	.LC2-0x4
	struct fbgc_object * tp = _cast_llbase_as_llconstant(cast_fbgc_object_as_fun(fun_obj)->code)->content;
      f7:	48 8b 45 01          	mov    rax,QWORD PTR [rbp+0x1]
	cast_fbgc_object_as_fun(fun_obj)->no_arg = size_fbgc_tuple_object(tp); 
      fb:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
      ff:	48 8b 50 09          	mov    rdx,QWORD PTR [rax+0x9]
     103:	31 c0                	xor    eax,eax
     105:	88 55 09             	mov    BYTE PTR [rbp+0x9],dl
	FBGC_LOGD(PARSER,"Function arg no is:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);
     108:	0f be d2             	movsx  edx,dl
     10b:	e8 00 00 00 00       	call   110 <handle_function_args+0x45>	10c: R_X86_64_PLT32	__printf_chk-0x4
	while(iter != arg_end){
     110:	49 39 dd             	cmp    r13,rbx
     113:	74 6a                	je     17f <handle_function_args+0xb4>
		if(iter->next->type == IDENTIFIER || iter->next->type == COMMA){
     115:	49 8b 7d 01          	mov    rdi,QWORD PTR [r13+0x1]
     119:	8a 07                	mov    al,BYTE PTR [rdi]
     11b:	3c 10                	cmp    al,0x10
     11d:	74 04                	je     123 <handle_function_args+0x58>
     11f:	3c 29                	cmp    al,0x29
     121:	75 15                	jne    138 <handle_function_args+0x6d>
     123:	4c 89 f6             	mov    rsi,r14
     126:	bf 01 00 00 00       	mov    edi,0x1
     12b:	31 c0                	xor    eax,eax
     12d:	e8 00 00 00 00       	call   132 <handle_function_args+0x67>	12e: R_X86_64_PLT32	__printf_chk-0x4
			FBGC_LOGV(PARSER,"Ok moving, typical function arg");
			iter = iter->next;
     132:	4d 8b 6d 01          	mov    r13,QWORD PTR [r13+0x1]
     136:	eb d8                	jmp    110 <handle_function_args+0x45>
		}
		else if(iter->next->type == FUN_CALL){
     138:	3c 52                	cmp    al,0x52
     13a:	75 48                	jne    184 <handle_function_args+0xb9>
			//this is variadic template, it must be the last arguments
			if(iter->next->next != arg_end){
     13c:	48 39 5f 01          	cmp    QWORD PTR [rdi+0x1],rbx
     140:	74 35                	je     177 <handle_function_args+0xac>
     142:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 149 <handle_function_args+0x7e>	145: R_X86_64_PC32	.rodata+0x11c
     149:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 150 <handle_function_args+0x85>	14c: R_X86_64_PC32	.LC4-0x4
     150:	31 c0                	xor    eax,eax
     152:	41 b8 88 00 00 00    	mov    r8d,0x88
     158:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 15f <handle_function_args+0x94>	15b: R_X86_64_PC32	.LC5-0x4
     15f:	bf 01 00 00 00       	mov    edi,0x1
     164:	e8 00 00 00 00       	call   169 <handle_function_args+0x9e>	165: R_X86_64_PLT32	__printf_chk-0x4
				_FBGC_LOGE("Variadic argmust be the last arg!");
				assert(0);	
     169:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 170 <handle_function_args+0xa5>	16c: R_X86_64_PC32	.rodata+0xfc
     170:	ba 89 00 00 00       	mov    edx,0x89
     175:	eb 48                	jmp    1bf <handle_function_args+0xf4>
			}

			//Make it negative so we will understand that it has variadic template
			cast_fbgc_object_as_fun(fun_obj)->no_arg *= -1;
     177:	f6 5d 09             	neg    BYTE PTR [rbp+0x9]

			arg_end = iter->next;
			
			if(arg_end->next->type == COMMA){
     17a:	80 3b 29             	cmp    BYTE PTR [rbx],0x29
     17d:	75 53                	jne    1d2 <handle_function_args+0x107>
	while(iter != arg_end){
     17f:	48 89 df             	mov    rdi,rbx
     182:	eb 4e                	jmp    1d2 <handle_function_args+0x107>

			break;

		}
		else{
			_FBGC_LOGE("Syntax error in function definition! iter_type :%s",lltp2str(iter->next));
     184:	e8 00 00 00 00       	call   189 <handle_function_args+0xbe>	185: R_X86_64_PLT32	lltp2str-0x4
     189:	41 b8 99 00 00 00    	mov    r8d,0x99
     18f:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 196 <handle_function_args+0xcb>	192: R_X86_64_PC32	.rodata+0x11c
     196:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 19d <handle_function_args+0xd2>	199: R_X86_64_PC32	.LC4-0x4
     19d:	49 89 c1             	mov    r9,rax
     1a0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1a7 <handle_function_args+0xdc>	1a3: R_X86_64_PC32	.LC7-0x4
     1a7:	bf 01 00 00 00       	mov    edi,0x1
     1ac:	31 c0                	xor    eax,eax
     1ae:	e8 00 00 00 00       	call   1b3 <handle_function_args+0xe8>	1af: R_X86_64_PLT32	__printf_chk-0x4
			assert(0);
     1b3:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 1ba <handle_function_args+0xef>	1b6: R_X86_64_PC32	.rodata+0xfc
     1ba:	ba 9a 00 00 00       	mov    edx,0x9a
     1bf:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1c6 <handle_function_args+0xfb>	1c2: R_X86_64_PC32	.LC4-0x4
     1c6:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # 1cd <handle_function_args+0x102>	1c9: R_X86_64_PC32	.LC6-0x4
     1cd:	e8 00 00 00 00       	call   1d2 <handle_function_args+0x107>	1ce: R_X86_64_PLT32	__assert_fail-0x4
		}
	}

	fun_holder->next = arg_end->next;
     1d2:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]



	_info("Handling function args, # of args:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);	
}
     1d6:	5b                   	pop    rbx
	_info("Handling function args, # of args:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);	
     1d7:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1de <handle_function_args+0x113>	1da: R_X86_64_PC32	.LC8-0x4
     1de:	bf 6f 00 00 00       	mov    edi,0x6f
	fun_holder->next = arg_end->next;
     1e3:	49 89 44 24 01       	mov    QWORD PTR [r12+0x1],rax
	_info("Handling function args, # of args:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);	
     1e8:	0f be 55 09          	movsx  edx,BYTE PTR [rbp+0x9]
     1ec:	31 c0                	xor    eax,eax
}
     1ee:	5d                   	pop    rbp
     1ef:	41 5c                	pop    r12
     1f1:	41 5d                	pop    r13
     1f3:	41 5e                	pop    r14
	_info("Handling function args, # of args:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);	
     1f5:	e9 00 00 00 00       	jmp    1fa <handle_before_paranthesis>	1f6: R_X86_64_PLT32	cprintf-0x4

00000000000001fa <handle_before_paranthesis>:

void handle_before_paranthesis(struct parser_packet * p){
     1fa:	f3 0f 1e fa          	endbr64 
     1fe:	41 54                	push   r12
     200:	55                   	push   rbp
     201:	40 b5 02             	mov    bpl,0x2
     204:	53                   	push   rbx
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));
     205:	48 8b 47 18          	mov    rax,QWORD PTR [rdi+0x18]
void handle_before_paranthesis(struct parser_packet * p){
     209:	48 89 fb             	mov    rbx,rdi
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));
     20c:	48 8b 78 01          	mov    rdi,QWORD PTR [rax+0x1]
     210:	e8 00 00 00 00       	call   215 <handle_before_paranthesis+0x1b>	211: R_X86_64_PLT32	lltp2str-0x4
     215:	41 b8 a6 00 00 00    	mov    r8d,0xa6
     21b:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 222 <handle_before_paranthesis+0x28>	21e: R_X86_64_PC32	.LC4-0x4
     222:	bf 01 00 00 00       	mov    edi,0x1
     227:	49 89 c1             	mov    r9,rax
     22a:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 231 <handle_before_paranthesis+0x37>	22d: R_X86_64_PC32	.rodata+0xdc
     231:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 238 <handle_before_paranthesis+0x3e>	234: R_X86_64_PC32	.LC9-0x4
     238:	31 c0                	xor    eax,eax
     23a:	e8 00 00 00 00       	call   23f <handle_before_paranthesis+0x45>	23b: R_X86_64_PLT32	__printf_chk-0x4
	
	const fbgc_token top_type = TOP_LL(p->op)->type;
     23f:	48 8b 43 18          	mov    rax,QWORD PTR [rbx+0x18]

	uint8_t	state = 0;
	if(p->iter_prev->type == COMMA) state = 2;
     243:	4c 8b 43 10          	mov    r8,QWORD PTR [rbx+0x10]
	const fbgc_token top_type = TOP_LL(p->op)->type;
     247:	48 8b 78 01          	mov    rdi,QWORD PTR [rax+0x1]
	if(p->iter_prev->type == COMMA) state = 2;
     24b:	41 8a 10             	mov    dl,BYTE PTR [r8]
	const fbgc_token top_type = TOP_LL(p->op)->type;
     24e:	8a 07                	mov    al,BYTE PTR [rdi]
	if(p->iter_prev->type == COMMA) state = 2;
     250:	80 fa 29             	cmp    dl,0x29
     253:	74 08                	je     25d <handle_before_paranthesis+0x63>
	else if(p->gm != GM_ATOM) state = 1;
     255:	80 7b 29 02          	cmp    BYTE PTR [rbx+0x29],0x2
     259:	40 0f 95 c5          	setne  bpl

	if(top_type == FUN_CALL){
     25d:	3c 52                	cmp    al,0x52
     25f:	75 76                	jne    2d7 <handle_before_paranthesis+0xdd>
		
		_print("Function call template, p->iter_prev:%s\n",lltp2str(p->iter_prev));
     261:	4c 89 c7             	mov    rdi,r8
     264:	e8 00 00 00 00       	call   269 <handle_before_paranthesis+0x6f>	265: R_X86_64_PLT32	lltp2str-0x4
     269:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 270 <handle_before_paranthesis+0x76>	26c: R_X86_64_PC32	.LC10-0x4
     270:	bf 65 00 00 00       	mov    edi,0x65
     275:	48 89 c2             	mov    rdx,rax
     278:	31 c0                	xor    eax,eax
     27a:	e8 00 00 00 00       	call   27f <handle_before_paranthesis+0x85>	27b: R_X86_64_PLT32	cprintf-0x4

		//state == 2, p->iter_prev is comma
		if(state == 2){
     27f:	40 80 fd 02          	cmp    bpl,0x2
     283:	75 09                	jne    28e <handle_before_paranthesis+0x94>
			p->iter_prev->type = FUN_CALL;
     285:	48 8b 43 10          	mov    rax,QWORD PTR [rbx+0x10]
     289:	c6 00 52             	mov    BYTE PTR [rax],0x52
     28c:	eb 22                	jmp    2b0 <handle_before_paranthesis+0xb6>
		}
		else{

			struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(FUN_CALL,state==1);
     28e:	40 0f b6 f5          	movzx  esi,bpl
     292:	bf 52 00 00 00       	mov    edi,0x52
     297:	e8 00 00 00 00       	call   29c <handle_before_paranthesis+0xa2>	298: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
			ito->next = p->iter_prev->next;
     29c:	48 8b 53 10          	mov    rdx,QWORD PTR [rbx+0x10]
     2a0:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     2a4:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
			p->iter_prev->next = ito;
     2a8:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
			p->iter_prev = ito;
     2ac:	48 89 43 10          	mov    QWORD PTR [rbx+0x10],rax
		}
		_cast_llbase_as_llopcode_int(p->iter_prev)->content += _cast_llbase_as_llopcode_int(TOP_LL(p->op))->content;
     2b0:	48 8b 53 18          	mov    rdx,QWORD PTR [rbx+0x18]
     2b4:	48 8b 43 10          	mov    rax,QWORD PTR [rbx+0x10]
     2b8:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     2bc:	8b 71 09             	mov    esi,DWORD PTR [rcx+0x9]

		//gm_seek_right(gm,TOP_LL(p->op));
		struct fbgc_ll_base * iter = p->iter_prev->next;
		//Insert top p->op to the list  
		//p->iter_prev->next = TOP_LL(p->op);
		POP_LL(p->op);
     2bf:	48 8b 49 01          	mov    rcx,QWORD PTR [rcx+0x1]
		_cast_llbase_as_llopcode_int(p->iter_prev)->content += _cast_llbase_as_llopcode_int(TOP_LL(p->op))->content;
     2c3:	01 70 09             	add    DWORD PTR [rax+0x9],esi
		struct fbgc_ll_base * iter = p->iter_prev->next;
     2c6:	48 8b 70 01          	mov    rsi,QWORD PTR [rax+0x1]
		POP_LL(p->op);
     2ca:	48 89 4a 01          	mov    QWORD PTR [rdx+0x1],rcx
		if(put_pop_top){
			p->iter_prev->next = _new_fbgc_ll_base(POP_TOP);
			p->iter_prev = p->iter_prev->next;			
		}

		p->iter_prev->next = iter;
     2ce:	48 89 70 01          	mov    QWORD PTR [rax+0x1],rsi
     2d2:	e9 e4 02 00 00       	jmp    5bb <handle_before_paranthesis+0x3c1>
	}
	else if(top_type == FUN_MAKE){
     2d7:	3c 18                	cmp    al,0x18
     2d9:	0f 85 a3 00 00 00    	jne    382 <handle_before_paranthesis+0x188>

		_print("Stack top is FUN_MAKE, function make template!\n");
     2df:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 2e6 <handle_before_paranthesis+0xec>	2e2: R_X86_64_PC32	.LC11-0x4
     2e6:	bf 65 00 00 00       	mov    edi,0x65
     2eb:	31 c0                	xor    eax,eax
     2ed:	e8 00 00 00 00       	call   2f2 <handle_before_paranthesis+0xf8>	2ee: R_X86_64_PLT32	cprintf-0x4
		struct fbgc_ll_base * fun_holder =  _cast_llbase_as_lljumper(TOP_LL(p->op))->content; 
     2f2:	48 8b 43 18          	mov    rax,QWORD PTR [rbx+0x18]
     2f6:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     2fa:	48 8b 68 09          	mov    rbp,QWORD PTR [rax+0x9]
		struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;

		//Is it possible this is not useful*
		if(cast_fbgc_object_as_fun(fun_obj)->no_arg < 65){
     2fe:	48 8b 45 09          	mov    rax,QWORD PTR [rbp+0x9]
     302:	0f be 50 09          	movsx  edx,BYTE PTR [rax+0x9]
     306:	80 fa 40             	cmp    dl,0x40
     309:	7f 17                	jg     322 <handle_before_paranthesis+0x128>
		_print_op_stack();
		assert(0);*/
	}

	_info("==================================\n");
}
     30b:	5b                   	pop    rbx
			_print("already created function,arg :%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);
     30c:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 313 <handle_before_paranthesis+0x119>	30f: R_X86_64_PC32	.LC12-0x4
}
     313:	5d                   	pop    rbp
			_print("already created function,arg :%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);
     314:	bf 65 00 00 00       	mov    edi,0x65
     319:	31 c0                	xor    eax,eax
}
     31b:	41 5c                	pop    r12
			_print("already created function,arg :%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);
     31d:	e9 00 00 00 00       	jmp    322 <handle_before_paranthesis+0x128>	31e: R_X86_64_PLT32	cprintf-0x4
		_print("Function Arg Start:%s\n",lltp2str(fun_holder->next));
     322:	48 8b 7d 01          	mov    rdi,QWORD PTR [rbp+0x1]
     326:	e8 00 00 00 00       	call   32b <handle_before_paranthesis+0x131>	327: R_X86_64_PLT32	lltp2str-0x4
     32b:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 332 <handle_before_paranthesis+0x138>	32e: R_X86_64_PC32	.LC13-0x4
     332:	bf 65 00 00 00       	mov    edi,0x65
     337:	48 89 c2             	mov    rdx,rax
     33a:	31 c0                	xor    eax,eax
     33c:	e8 00 00 00 00       	call   341 <handle_before_paranthesis+0x147>	33d: R_X86_64_PLT32	cprintf-0x4
		_print_fbgc_ll_code(fun_holder->next,p->iter_prev);
     341:	48 8b 73 10          	mov    rsi,QWORD PTR [rbx+0x10]
     345:	48 8b 7d 01          	mov    rdi,QWORD PTR [rbp+0x1]
     349:	e8 00 00 00 00       	call   34e <handle_before_paranthesis+0x154>	34a: R_X86_64_PLT32	_print_fbgc_ll_code-0x4
		_print("Function Arg End:%s\n",lltp2str(p->iter_prev));
     34e:	48 8b 7b 10          	mov    rdi,QWORD PTR [rbx+0x10]
     352:	e8 00 00 00 00       	call   357 <handle_before_paranthesis+0x15d>	353: R_X86_64_PLT32	lltp2str-0x4
     357:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 35e <handle_before_paranthesis+0x164>	35a: R_X86_64_PC32	.LC14-0x4
     35e:	bf 65 00 00 00       	mov    edi,0x65
     363:	48 89 c2             	mov    rdx,rax
     366:	31 c0                	xor    eax,eax
     368:	e8 00 00 00 00       	call   36d <handle_before_paranthesis+0x173>	369: R_X86_64_PLT32	cprintf-0x4
		handle_function_args(fun_holder,p->iter_prev);
     36d:	48 8b 73 10          	mov    rsi,QWORD PTR [rbx+0x10]
     371:	48 89 ef             	mov    rdi,rbp
     374:	e8 00 00 00 00       	call   379 <handle_before_paranthesis+0x17f>	375: R_X86_64_PLT32	handle_function_args-0x4
		p->iter_prev = fun_holder;
     379:	48 89 6b 10          	mov    QWORD PTR [rbx+0x10],rbp
     37d:	e9 39 02 00 00       	jmp    5bb <handle_before_paranthesis+0x3c1>
	else if(top_type == CLASS_MAKE){
     382:	3c 19                	cmp    al,0x19
     384:	0f 85 b3 00 00 00    	jne    43d <handle_before_paranthesis+0x243>
     38a:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 391 <handle_before_paranthesis+0x197>	38d: R_X86_64_PC32	.LC15-0x4
     391:	bf 01 00 00 00       	mov    edi,0x1
     396:	31 c0                	xor    eax,eax
     398:	e8 00 00 00 00       	call   39d <handle_before_paranthesis+0x1a3>	399: R_X86_64_PLT32	__printf_chk-0x4
		struct fbgc_ll_base * cls_holder =  _cast_llbase_as_lljumper(TOP_LL(p->op))->content; 
     39d:	48 8b 43 18          	mov    rax,QWORD PTR [rbx+0x18]
     3a1:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     3a5:	48 8b 68 09          	mov    rbp,QWORD PTR [rax+0x9]
		if(_cast_llbase_as_llconstant(TOP_LL(p->scope_list))->content == cls_obj)
     3a9:	48 8b 43 20          	mov    rax,QWORD PTR [rbx+0x20]
     3ad:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
		struct fbgc_object * cls_obj = _cast_llbase_as_llconstant(cls_holder)->content;
     3b1:	4c 8b 65 09          	mov    r12,QWORD PTR [rbp+0x9]
		if(_cast_llbase_as_llconstant(TOP_LL(p->scope_list))->content == cls_obj)
     3b5:	4c 39 60 09          	cmp    QWORD PTR [rax+0x9],r12
     3b9:	0f 84 13 02 00 00    	je     5d2 <handle_before_paranthesis+0x3d8>
		_print("Class Arg Start:%s\n",lltp2str(cls_holder->next));
     3bf:	48 8b 7d 01          	mov    rdi,QWORD PTR [rbp+0x1]
     3c3:	e8 00 00 00 00       	call   3c8 <handle_before_paranthesis+0x1ce>	3c4: R_X86_64_PLT32	lltp2str-0x4
     3c8:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 3cf <handle_before_paranthesis+0x1d5>	3cb: R_X86_64_PC32	.LC16-0x4
     3cf:	bf 65 00 00 00       	mov    edi,0x65
     3d4:	48 89 c2             	mov    rdx,rax
     3d7:	31 c0                	xor    eax,eax
     3d9:	e8 00 00 00 00       	call   3de <handle_before_paranthesis+0x1e4>	3da: R_X86_64_PLT32	cprintf-0x4
		_print("Class Arg End:%s\n",lltp2str(p->iter_prev));
     3de:	48 8b 7b 10          	mov    rdi,QWORD PTR [rbx+0x10]
     3e2:	e8 00 00 00 00       	call   3e7 <handle_before_paranthesis+0x1ed>	3e3: R_X86_64_PLT32	lltp2str-0x4
     3e7:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 3ee <handle_before_paranthesis+0x1f4>	3ea: R_X86_64_PC32	.LC17-0x4
     3ee:	bf 65 00 00 00       	mov    edi,0x65
     3f3:	48 89 c2             	mov    rdx,rax
     3f6:	31 c0                	xor    eax,eax
     3f8:	e8 00 00 00 00       	call   3fd <handle_before_paranthesis+0x203>	3f9: R_X86_64_PLT32	cprintf-0x4
		_push_front_fbgc_ll(p->scope_list,_new_fbgc_ll_constant(cls_obj));
     3fd:	4c 89 e7             	mov    rdi,r12
     400:	e8 00 00 00 00       	call   405 <handle_before_paranthesis+0x20b>	401: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     405:	48 8b 7b 20          	mov    rdi,QWORD PTR [rbx+0x20]
     409:	48 89 c6             	mov    rsi,rax
     40c:	e8 00 00 00 00       	call   411 <handle_before_paranthesis+0x217>	40d: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
		if(cls_holder->next->type == IDENTIFIER){
     411:	48 8b 45 01          	mov    rax,QWORD PTR [rbp+0x1]
     415:	80 38 10             	cmp    BYTE PTR [rax],0x10
     418:	48 0f 44 e8          	cmove  rbp,rax
		cast_fbgc_object_as_class(cls_obj)->code = cls_holder;
     41c:	49 89 6c 24 01       	mov    QWORD PTR [r12+0x1],rbp
		_print("Class code :%s\n",lltp2str(cast_fbgc_object_as_class(cls_obj)->code));
     421:	48 89 ef             	mov    rdi,rbp
     424:	e8 00 00 00 00       	call   429 <handle_before_paranthesis+0x22f>	425: R_X86_64_PLT32	lltp2str-0x4
     429:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 430 <handle_before_paranthesis+0x236>	42c: R_X86_64_PC32	.LC18-0x4
     430:	bf 65 00 00 00       	mov    edi,0x65
     435:	48 89 c2             	mov    rdx,rax
     438:	e9 10 01 00 00       	jmp    54d <handle_before_paranthesis+0x353>
	else if(top_type == FOR){
     43d:	3c 1d                	cmp    al,0x1d
     43f:	0f 85 b2 00 00 00    	jne    4f7 <handle_before_paranthesis+0x2fd>
		if(_cast_llbase_as_lljumper(TOP_LL(p->op))->content->type == FOR_BEGIN)
     445:	48 8b 47 09          	mov    rax,QWORD PTR [rdi+0x9]
     449:	80 38 51             	cmp    BYTE PTR [rax],0x51
     44c:	0f 84 80 01 00 00    	je     5d2 <handle_before_paranthesis+0x3d8>
		_print("Stack top is FOR, for loop template!\n");
     452:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 459 <handle_before_paranthesis+0x25f>	455: R_X86_64_PC32	.LC19-0x4
     459:	bf 65 00 00 00       	mov    edi,0x65
     45e:	31 c0                	xor    eax,eax
     460:	e8 00 00 00 00       	call   465 <handle_before_paranthesis+0x26b>	461: R_X86_64_PLT32	cprintf-0x4
		_print("For loop content:%s\n",lltp2str(_cast_llbase_as_lljumper(TOP_LL(p->op))->content));
     465:	48 8b 43 18          	mov    rax,QWORD PTR [rbx+0x18]
     469:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     46d:	48 8b 78 09          	mov    rdi,QWORD PTR [rax+0x9]
     471:	e8 00 00 00 00       	call   476 <handle_before_paranthesis+0x27c>	472: R_X86_64_PLT32	lltp2str-0x4
     476:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 47d <handle_before_paranthesis+0x283>	479: R_X86_64_PC32	.LC20-0x4
     47d:	bf 65 00 00 00       	mov    edi,0x65
     482:	48 89 c2             	mov    rdx,rax
     485:	31 c0                	xor    eax,eax
     487:	e8 00 00 00 00       	call   48c <handle_before_paranthesis+0x292>	488: R_X86_64_PLT32	cprintf-0x4
		_print("Iterator previous:%s\n",lltp2str(p->iter_prev));
     48c:	48 8b 7b 10          	mov    rdi,QWORD PTR [rbx+0x10]
     490:	e8 00 00 00 00       	call   495 <handle_before_paranthesis+0x29b>	491: R_X86_64_PLT32	lltp2str-0x4
     495:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 49c <handle_before_paranthesis+0x2a2>	498: R_X86_64_PC32	.LC21-0x4
     49c:	bf 65 00 00 00       	mov    edi,0x65
     4a1:	48 89 c2             	mov    rdx,rax
     4a4:	31 c0                	xor    eax,eax
     4a6:	e8 00 00 00 00       	call   4ab <handle_before_paranthesis+0x2b1>	4a7: R_X86_64_PLT32	cprintf-0x4
		if(p->iter_prev->type != ASSIGN){
     4ab:	48 8b 53 10          	mov    rdx,QWORD PTR [rbx+0x10]
     4af:	80 3a 43             	cmp    BYTE PTR [rdx],0x43
     4b2:	0f 85 1a 01 00 00    	jne    5d2 <handle_before_paranthesis+0x3d8>
		struct fbgc_ll_base * d = _cast_llbase_as_lljumper(TOP_LL(p->op))->content;
     4b8:	48 8b 43 18          	mov    rax,QWORD PTR [rbx+0x18]
     4bc:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     4c0:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
		while(d->next != p->iter_prev)
     4c4:	48 89 c5             	mov    rbp,rax
     4c7:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     4cb:	48 39 c2             	cmp    rdx,rax
     4ce:	75 f4                	jne    4c4 <handle_before_paranthesis+0x2ca>
		struct fbgc_ll_base * fb = _new_fbgc_ll_base(FOR_BEGIN);
     4d0:	bf 51 00 00 00       	mov    edi,0x51
     4d5:	e8 00 00 00 00       	call   4da <handle_before_paranthesis+0x2e0>	4d6: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
		fb->next = d->next;
     4da:	48 8b 55 01          	mov    rdx,QWORD PTR [rbp+0x1]
     4de:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
		_cast_llbase_as_lljumper(TOP_LL(p->op))->content = fb;
     4e2:	48 8b 53 18          	mov    rdx,QWORD PTR [rbx+0x18]
		d->next = fb;
     4e6:	48 89 45 01          	mov    QWORD PTR [rbp+0x1],rax
		_cast_llbase_as_lljumper(TOP_LL(p->op))->content = fb;
     4ea:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     4ee:	48 89 42 09          	mov    QWORD PTR [rdx+0x9],rax
     4f2:	e9 c4 00 00 00       	jmp    5bb <handle_before_paranthesis+0x3c1>
	else if( (top_type == IF || top_type == ELIF || top_type == WHILE) && _cast_llbase_as_lljumper(TOP_LL(p->op))->content == NULL){
     4f7:	8d 48 e6             	lea    ecx,[rax-0x1a]
     4fa:	80 e1 fd             	and    cl,0xfd
     4fd:	74 04                	je     503 <handle_before_paranthesis+0x309>
     4ff:	3c 20                	cmp    al,0x20
     501:	75 53                	jne    556 <handle_before_paranthesis+0x35c>
     503:	48 83 7f 09 00       	cmp    QWORD PTR [rdi+0x9],0x0
     508:	75 4c                	jne    556 <handle_before_paranthesis+0x35c>
		_print("Stack top is %s, conditional template!\n",lltp2str(TOP_LL(p->op)));
     50a:	e8 00 00 00 00       	call   50f <handle_before_paranthesis+0x315>	50b: R_X86_64_PLT32	lltp2str-0x4
     50f:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 516 <handle_before_paranthesis+0x31c>	512: R_X86_64_PC32	.LC22-0x4
     516:	bf 65 00 00 00       	mov    edi,0x65
     51b:	48 89 c2             	mov    rdx,rax
     51e:	31 c0                	xor    eax,eax
     520:	e8 00 00 00 00       	call   525 <handle_before_paranthesis+0x32b>	521: R_X86_64_PLT32	cprintf-0x4
		_cast_llbase_as_lljumper(TOP_LL(p->op))->content = p->iter_prev;
     525:	48 8b 53 18          	mov    rdx,QWORD PTR [rbx+0x18]
     529:	48 8b 43 10          	mov    rax,QWORD PTR [rbx+0x10]
     52d:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
		_warning("If shows %s\n",lltp2str(p->iter_prev->next));	
     531:	48 8b 78 01          	mov    rdi,QWORD PTR [rax+0x1]
		_cast_llbase_as_lljumper(TOP_LL(p->op))->content = p->iter_prev;
     535:	48 89 42 09          	mov    QWORD PTR [rdx+0x9],rax
		_warning("If shows %s\n",lltp2str(p->iter_prev->next));	
     539:	e8 00 00 00 00       	call   53e <handle_before_paranthesis+0x344>	53a: R_X86_64_PLT32	lltp2str-0x4
     53e:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 545 <handle_before_paranthesis+0x34b>	541: R_X86_64_PC32	.LC23-0x4
     545:	bf 6e 00 00 00       	mov    edi,0x6e
     54a:	48 89 c2             	mov    rdx,rax
     54d:	31 c0                	xor    eax,eax
     54f:	e8 00 00 00 00       	call   554 <handle_before_paranthesis+0x35a>	550: R_X86_64_PLT32	cprintf-0x4
     554:	eb 65                	jmp    5bb <handle_before_paranthesis+0x3c1>
	else if(p->iter_prev->type == COMMA || p->gm == GM_ATOM){
     556:	80 fa 29             	cmp    dl,0x29
     559:	74 06                	je     561 <handle_before_paranthesis+0x367>
     55b:	80 7b 29 02          	cmp    BYTE PTR [rbx+0x29],0x2
     55f:	75 5a                	jne    5bb <handle_before_paranthesis+0x3c1>
		if(state == 2){
     561:	40 80 fd 02          	cmp    bpl,0x2
     565:	75 1c                	jne    583 <handle_before_paranthesis+0x389>
			_debug("State number is 2, changing COMMA to BUILD_TUPLE\n");
     567:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 56e <handle_before_paranthesis+0x374>	56a: R_X86_64_PC32	.LC24-0x4
     56e:	bf 09 00 00 00       	mov    edi,0x9
     573:	31 c0                	xor    eax,eax
     575:	e8 00 00 00 00       	call   57a <handle_before_paranthesis+0x380>	576: R_X86_64_PLT32	cprintf-0x4
			p->iter_prev->type = BUILD_TUPLE;
     57a:	48 8b 43 10          	mov    rax,QWORD PTR [rbx+0x10]
     57e:	c6 00 53             	mov    BYTE PTR [rax],0x53
     581:	eb 38                	jmp    5bb <handle_before_paranthesis+0x3c1>
		else if(state == 0){
     583:	40 84 ed             	test   bpl,bpl
     586:	75 33                	jne    5bb <handle_before_paranthesis+0x3c1>
			_debug("State number is 0, creating new object BUILD_TUPLE with 0 size\n");
     588:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 58f <handle_before_paranthesis+0x395>	58b: R_X86_64_PC32	.LC25-0x4
     58f:	bf 09 00 00 00       	mov    edi,0x9
     594:	31 c0                	xor    eax,eax
     596:	e8 00 00 00 00       	call   59b <handle_before_paranthesis+0x3a1>	597: R_X86_64_PLT32	cprintf-0x4
			struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(BUILD_TUPLE,0);
     59b:	31 f6                	xor    esi,esi
     59d:	bf 53 00 00 00       	mov    edi,0x53
     5a2:	e8 00 00 00 00       	call   5a7 <handle_before_paranthesis+0x3ad>	5a3: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
			ito->next = p->iter_prev->next;
     5a7:	48 8b 53 10          	mov    rdx,QWORD PTR [rbx+0x10]
     5ab:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     5af:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
			p->iter_prev->next = ito;
     5b3:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
			p->iter_prev = ito;
     5b7:	48 89 43 10          	mov    QWORD PTR [rbx+0x10],rax
}
     5bb:	5b                   	pop    rbx
	_info("==================================\n");
     5bc:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 5c3 <handle_before_paranthesis+0x3c9>	5bf: R_X86_64_PC32	.LC26-0x4
}
     5c3:	5d                   	pop    rbp
	_info("==================================\n");
     5c4:	bf 6f 00 00 00       	mov    edi,0x6f
     5c9:	31 c0                	xor    eax,eax
}
     5cb:	41 5c                	pop    r12
	_info("==================================\n");
     5cd:	e9 00 00 00 00       	jmp    5d2 <handle_before_paranthesis+0x3d8>	5ce: R_X86_64_PLT32	cprintf-0x4
}
     5d2:	5b                   	pop    rbx
     5d3:	5d                   	pop    rbp
     5d4:	41 5c                	pop    r12
     5d6:	c3                   	ret    

00000000000005d7 <handle_before_brackets>:

void handle_before_brackets(struct parser_packet *p){
     5d7:	f3 0f 1e fa          	endbr64 
     5db:	53                   	push   rbx
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));
     5dc:	48 8b 47 18          	mov    rax,QWORD PTR [rdi+0x18]
void handle_before_brackets(struct parser_packet *p){
     5e0:	48 89 fb             	mov    rbx,rdi
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));
     5e3:	48 8b 78 01          	mov    rdi,QWORD PTR [rax+0x1]
     5e7:	e8 00 00 00 00       	call   5ec <handle_before_brackets+0x15>	5e8: R_X86_64_PLT32	lltp2str-0x4
     5ec:	41 b8 49 01 00 00    	mov    r8d,0x149
     5f2:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 5f9 <handle_before_brackets+0x22>	5f5: R_X86_64_PC32	.LC9-0x4
     5f9:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 600 <handle_before_brackets+0x29>	5fc: R_X86_64_PC32	.rodata+0xbc
     600:	49 89 c1             	mov    r9,rax
     603:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 60a <handle_before_brackets+0x33>	606: R_X86_64_PC32	.LC4-0x4
     60a:	31 c0                	xor    eax,eax
     60c:	bf 01 00 00 00       	mov    edi,0x1
     611:	e8 00 00 00 00       	call   616 <handle_before_brackets+0x3f>	612: R_X86_64_PLT32	__printf_chk-0x4

	
	uint8_t	state = 0;
	if(p->iter_prev->type == COMMA) state = 2;
     616:	48 8b 43 10          	mov    rax,QWORD PTR [rbx+0x10]
     61a:	40 b6 02             	mov    sil,0x2
     61d:	80 38 29             	cmp    BYTE PTR [rax],0x29
     620:	74 08                	je     62a <handle_before_brackets+0x53>
	else if(p->gm != GM_ATOM) state = 1;
     622:	80 7b 29 02          	cmp    BYTE PTR [rbx+0x29],0x2
     626:	40 0f 95 c6          	setne  sil


	
	if(is_fbgc_ASSIGNMENT_OPERATOR(p->iter_prev->next->type)) return;
     62a:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     62e:	8a 12                	mov    dl,BYTE PTR [rdx]
     630:	83 ea 43             	sub    edx,0x43
     633:	80 fa 0a             	cmp    dl,0xa
     636:	0f 86 96 00 00 00    	jbe    6d2 <handle_before_brackets+0xfb>
	
	if(TOP_LL(p->op)->type == IDENTIFIER || TOP_LL(p->op)->type == LOAD_SUBSCRIPT){	
     63c:	48 8b 53 18          	mov    rdx,QWORD PTR [rbx+0x18]
     640:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     644:	8a 12                	mov    dl,BYTE PTR [rdx]
     646:	80 fa 10             	cmp    dl,0x10
     649:	74 05                	je     650 <handle_before_brackets+0x79>
     64b:	80 fa 57             	cmp    dl,0x57
     64e:	75 41                	jne    691 <handle_before_brackets+0xba>
		_info("Operator stack top load_local or global, this is a subscript call template!\n");
     650:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 657 <handle_before_brackets+0x80>	653: R_X86_64_PC32	.LC27-0x4
     657:	bf 6f 00 00 00       	mov    edi,0x6f
     65c:	31 c0                	xor    eax,eax
     65e:	e8 00 00 00 00       	call   663 <handle_before_brackets+0x8c>	65f: R_X86_64_PLT32	cprintf-0x4

		struct fbgc_ll_base * iter = p->iter_prev->next;
		//Insert top op to the list  
		p->iter_prev->next = TOP_LL(p->op);
     663:	48 8b 53 18          	mov    rdx,QWORD PTR [rbx+0x18]
		struct fbgc_ll_base * iter = p->iter_prev->next;
     667:	48 8b 43 10          	mov    rax,QWORD PTR [rbx+0x10]
		p->iter_prev->next = TOP_LL(p->op);
     66b:	48 8b 72 01          	mov    rsi,QWORD PTR [rdx+0x1]
		struct fbgc_ll_base * iter = p->iter_prev->next;
     66f:	48 8b 48 01          	mov    rcx,QWORD PTR [rax+0x1]
		p->iter_prev->next = TOP_LL(p->op);
     673:	48 89 70 01          	mov    QWORD PTR [rax+0x1],rsi
		POP_LL(p->op);
     677:	48 8b 72 01          	mov    rsi,QWORD PTR [rdx+0x1]
     67b:	48 8b 76 01          	mov    rsi,QWORD PTR [rsi+0x1]
     67f:	48 89 72 01          	mov    QWORD PTR [rdx+0x1],rsi
		p->iter_prev = p->iter_prev->next;
     683:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     687:	48 89 43 10          	mov    QWORD PTR [rbx+0x10],rax
		p->iter_prev->next = iter;
     68b:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
     68f:	eb 2d                	jmp    6be <handle_before_brackets+0xe7>
	}
	else{
		if(state == 2) p->iter_prev->type = BUILD_MATRIX; 
     691:	40 80 fe 02          	cmp    sil,0x2
     695:	75 05                	jne    69c <handle_before_brackets+0xc5>
     697:	c6 00 54             	mov    BYTE PTR [rax],0x54
     69a:	eb 22                	jmp    6be <handle_before_brackets+0xe7>
		else {
			struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(BUILD_MATRIX,state==1);
     69c:	40 0f b6 f6          	movzx  esi,sil
     6a0:	bf 54 00 00 00       	mov    edi,0x54
     6a5:	e8 00 00 00 00       	call   6aa <handle_before_brackets+0xd3>	6a6: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
			ito->next = p->iter_prev->next;
     6aa:	48 8b 53 10          	mov    rdx,QWORD PTR [rbx+0x10]
     6ae:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     6b2:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
			p->iter_prev->next = ito;
     6b6:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
			p->iter_prev = ito;
     6ba:	48 89 43 10          	mov    QWORD PTR [rbx+0x10],rax
		}		
	}
	
	_info("==================================\n");
     6be:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 6c5 <handle_before_brackets+0xee>	6c1: R_X86_64_PC32	.LC26-0x4
     6c5:	bf 6f 00 00 00       	mov    edi,0x6f
     6ca:	31 c0                	xor    eax,eax
}
     6cc:	5b                   	pop    rbx
	_info("==================================\n");
     6cd:	e9 00 00 00 00       	jmp    6d2 <handle_before_brackets+0xfb>	6ce: R_X86_64_PLT32	cprintf-0x4
}
     6d2:	5b                   	pop    rbx
     6d3:	c3                   	ret    

00000000000006d4 <parser>:

#define PARSER_CHECK_ERROR(p_error_code)( {if(p_error_code != _FBGC_NO_ERROR) goto PARSER_ERROR_LABEL;} )

uint8_t parser(struct fbgc_object ** field_obj, FILE * input_file){
     6d4:	f3 0f 1e fa          	endbr64 
     6d8:	41 57                	push   r15
     6da:	41 56                	push   r14


	char line[1000] = {0};
	int line_no = 0;

	for(int i = 0; fbgc_error(p.error_code,line_no) ; i++){
     6dc:	45 31 f6             	xor    r14d,r14d
uint8_t parser(struct fbgc_object ** field_obj, FILE * input_file){
     6df:	41 55                	push   r13
	int line_no = 0;
     6e1:	45 31 ed             	xor    r13d,r13d
uint8_t parser(struct fbgc_object ** field_obj, FILE * input_file){
     6e4:	41 54                	push   r12
     6e6:	49 89 fc             	mov    r12,rdi
     6e9:	bf 01 00 00 00       	mov    edi,0x1
     6ee:	55                   	push   rbp
     6ef:	53                   	push   rbx
     6f0:	48 81 ec 48 04 00 00 	sub    rsp,0x448
     6f7:	48 89 34 24          	mov    QWORD PTR [rsp],rsi
     6fb:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 702 <parser+0x2e>	6fe: R_X86_64_PC32	.LC28-0x4
     702:	64 48 8b 04 25 28 00 00 00 	mov    rax,QWORD PTR fs:0x28
     70b:	48 89 84 24 38 04 00 00 	mov    QWORD PTR [rsp+0x438],rax
     713:	31 c0                	xor    eax,eax
     715:	e8 00 00 00 00       	call   71a <parser+0x46>	716: R_X86_64_PLT32	__printf_chk-0x4
		.head = _cast_llbase_as_ll( cast_fbgc_object_as_field(*field_obj)->head ),
     71a:	49 8b 04 24          	mov    rax,QWORD PTR [r12]
     71e:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
	struct parser_packet p = {
     722:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     726:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     72b:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
		.op = _new_fbgc_ll(),
     730:	31 c0                	xor    eax,eax
	struct parser_packet p = {
     732:	48 89 54 24 2e       	mov    QWORD PTR [rsp+0x2e],rdx
		.op = _new_fbgc_ll(),
     737:	e8 00 00 00 00       	call   73c <parser+0x68>	738: R_X86_64_PLT32	_new_fbgc_ll-0x4
	struct parser_packet p = {
     73c:	48 89 44 24 3e       	mov    QWORD PTR [rsp+0x3e],rax
		.scope_list = _new_fbgc_ll(),
     741:	31 c0                	xor    eax,eax
     743:	e8 00 00 00 00       	call   748 <parser+0x74>	744: R_X86_64_PLT32	_new_fbgc_ll-0x4
	_push_front_fbgc_ll(p.scope_list,_new_fbgc_ll_constant(*field_obj));
     748:	49 8b 3c 24          	mov    rdi,QWORD PTR [r12]
	struct parser_packet p = {
     74c:	66 c7 44 24 4e 00 01 	mov    WORD PTR [rsp+0x4e],0x100
     753:	48 89 44 24 46       	mov    QWORD PTR [rsp+0x46],rax
	_push_front_fbgc_ll(p.scope_list,_new_fbgc_ll_constant(*field_obj));
     758:	e8 00 00 00 00       	call   75d <parser+0x89>	759: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     75d:	48 8b 7c 24 46       	mov    rdi,QWORD PTR [rsp+0x46]
     762:	48 89 c6             	mov    rsi,rax
     765:	e8 00 00 00 00       	call   76a <parser+0x96>	766: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
	char line[1000] = {0};
     76a:	48 8d 7c 24 60       	lea    rdi,[rsp+0x60]
     76f:	31 c0                	xor    eax,eax
     771:	0f 57 c0             	xorps  xmm0,xmm0
     774:	b9 f6 00 00 00       	mov    ecx,0xf6
     779:	0f 11 44 24 50       	movups XMMWORD PTR [rsp+0x50],xmm0
     77e:	f3 ab                	rep stos DWORD PTR es:[rdi],eax
	for(int i = 0; fbgc_error(p.error_code,line_no) ; i++){
     780:	0f b6 7c 24 4e       	movzx  edi,BYTE PTR [rsp+0x4e]
     785:	44 89 ee             	mov    esi,r13d
     788:	e8 00 00 00 00       	call   78d <parser+0xb9>	789: R_X86_64_PLT32	fbgc_error-0x4
     78d:	85 c0                	test   eax,eax
     78f:	74 2f                	je     7c0 <parser+0xec>
		if(p.iter == p.head->tail){
     791:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     796:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     79a:	48 39 44 24 2e       	cmp    QWORD PTR [rsp+0x2e],rax
     79f:	0f 85 ac 00 00 00    	jne    851 <parser+0x17d>
			if(fbgc_getline_from_file(line, sizeof(line), input_file)){
     7a5:	48 8d 6c 24 50       	lea    rbp,[rsp+0x50]
     7aa:	48 8b 14 24          	mov    rdx,QWORD PTR [rsp]
     7ae:	be e8 03 00 00       	mov    esi,0x3e8
     7b3:	48 89 ef             	mov    rdi,rbp
     7b6:	e8 00 00 00 00       	call   7bb <parser+0xe7>	7b7: R_X86_64_PLT32	fbgc_getline_from_file-0x4
     7bb:	48 85 c0             	test   rax,rax
     7be:	75 2a                	jne    7ea <parser+0x116>
	_print_fbgc_ll(p.op,"O");
	cprintf(101,"[GM]:{%s}\n\n",gm2str(p.gm));

	}
	//make the linked list connection proper
	p.head->tail->next = p.iter_prev;
     7c0:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     7c5:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
     7ca:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     7ce:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx

	if(!is_empty_fbgc_ll(p.op)){
     7d2:	48 8b 54 24 3e       	mov    rdx,QWORD PTR [rsp+0x3e]
     7d7:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     7db:	48 39 42 09          	cmp    QWORD PTR [rdx+0x9],rax
     7df:	0f 85 29 10 00 00    	jne    180e <parser+0x113a>
     7e5:	e9 83 10 00 00       	jmp    186d <parser+0x1199>
				if(line[0] == '#' || line[0] == '\0' || line[0] == '\n') {
     7ea:	8a 44 24 50          	mov    al,BYTE PTR [rsp+0x50]
				++line_no;
     7ee:	41 ff c5             	inc    r13d
				if(line[0] == '#' || line[0] == '\0' || line[0] == '\n') {
     7f1:	3c 23                	cmp    al,0x23
     7f3:	77 18                	ja     80d <parser+0x139>
     7f5:	48 ba 01 04 00 00 08 00 00 00 	movabs rdx,0x800000401
     7ff:	48 0f a3 c2          	bt     rdx,rax
     803:	73 08                	jae    80d <parser+0x139>
					--i;
     805:	41 ff ce             	dec    r14d
					continue; //fast passing the comment
     808:	e9 f9 0f 00 00       	jmp    1806 <parser+0x1132>
				_debug("Processed line[%d]:{%s}\n",line_no,line);
     80d:	48 89 e9             	mov    rcx,rbp
     810:	44 89 ea             	mov    edx,r13d
     813:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 81a <parser+0x146>	816: R_X86_64_PC32	.LC29-0x4
     81a:	31 c0                	xor    eax,eax
     81c:	bf 09 00 00 00       	mov    edi,0x9
     821:	e8 00 00 00 00       	call   826 <parser+0x152>	822: R_X86_64_PLT32	cprintf-0x4
				p.head->tail->next = p.iter_prev;
     826:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
				regex_lexer(field_obj,line);
     82b:	48 89 ee             	mov    rsi,rbp
     82e:	4c 89 e7             	mov    rdi,r12
				p.head->tail->next = p.iter_prev;
     831:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
     836:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     83a:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
				regex_lexer(field_obj,line);
     83e:	e8 00 00 00 00       	call   843 <parser+0x16f>	83f: R_X86_64_PLT32	regex_lexer-0x4
				p.iter = p.iter_prev->next;
     843:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
     848:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     84c:	48 89 44 24 2e       	mov    QWORD PTR [rsp+0x2e],rax
	FBGC_LOGD(PARSER,"-----[%d]{%s}-------",i,lltp2str(p.iter));
     851:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     856:	48 8d 6c 24 26       	lea    rbp,[rsp+0x26]
     85b:	e8 00 00 00 00       	call   860 <parser+0x18c>	85c: R_X86_64_PLT32	lltp2str-0x4
     860:	bf 01 00 00 00       	mov    edi,0x1
     865:	44 89 f2             	mov    edx,r14d
     868:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 86f <parser+0x19b>	86b: R_X86_64_PC32	.LC30-0x4
     86f:	48 89 c1             	mov    rcx,rax
     872:	31 c0                	xor    eax,eax
     874:	e8 00 00 00 00       	call   879 <parser+0x1a5>	875: R_X86_64_PLT32	__printf_chk-0x4
	switch(p.iter->type){
     879:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     87e:	8a 07                	mov    al,BYTE PTR [rdi]
     880:	3c 26                	cmp    al,0x26
     882:	77 27                	ja     8ab <parser+0x1d7>
     884:	3c 01                	cmp    al,0x1
     886:	0f 86 fe 0e 00 00    	jbe    178a <parser+0x10b6>
     88c:	83 e8 02             	sub    eax,0x2
     88f:	3c 24                	cmp    al,0x24
     891:	0f 87 f3 0e 00 00    	ja     178a <parser+0x10b6>
     897:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 89e <parser+0x1ca>	89a: R_X86_64_PC32	.rodata-0x4
     89e:	0f b6 c0             	movzx  eax,al
     8a1:	48 63 04 82          	movsxd rax,DWORD PTR [rdx+rax*4]
     8a5:	48 01 d0             	add    rax,rdx
     8a8:	3e ff e0             	notrack jmp rax
     8ab:	3c 42                	cmp    al,0x42
     8ad:	77 0d                	ja     8bc <parser+0x1e8>
     8af:	3c 28                	cmp    al,0x28
     8b1:	0f 86 d3 0e 00 00    	jbe    178a <parser+0x10b6>
     8b7:	e9 0d 09 00 00       	jmp    11c9 <parser+0xaf5>
     8bc:	83 e8 43             	sub    eax,0x43
     8bf:	3c 0a                	cmp    al,0xa
     8c1:	0f 86 c3 0d 00 00    	jbe    168a <parser+0xfb6>
     8c7:	e9 be 0e 00 00       	jmp    178a <parser+0x10b6>
		gm_seek_left(&p);
     8cc:	48 89 ef             	mov    rdi,rbp
     8cf:	e8 00 00 00 00       	call   8d4 <parser+0x200>	8d0: R_X86_64_PLT32	gm_seek_left-0x4
		PARSER_CHECK_ERROR(p.error_code);
     8d4:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
     8d9:	0f 85 00 10 00 00    	jne    18df <parser+0x120b>
		p.iter_prev = p.iter;
     8df:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     8e4:	e9 e3 00 00 00       	jmp    9cc <parser+0x2f8>
		gm_seek_left(&p);
     8e9:	48 89 ef             	mov    rdi,rbp
     8ec:	e8 00 00 00 00       	call   8f1 <parser+0x21d>	8ed: R_X86_64_PLT32	gm_seek_left-0x4
		PARSER_CHECK_ERROR(p.error_code);
     8f1:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
     8f6:	0f 85 e3 0f 00 00    	jne    18df <parser+0x120b>
		FBGC_LOGV(PARSER,"Current Scope for identifier:(%s)",objtp2str(current_scope));
     8fc:	48 8b 44 24 46       	mov    rax,QWORD PTR [rsp+0x46]
     901:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     905:	48 8b 78 09          	mov    rdi,QWORD PTR [rax+0x9]
     909:	e8 00 00 00 00       	call   90e <parser+0x23a>	90a: R_X86_64_PLT32	objtp2str-0x4
     90e:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 915 <parser+0x241>	911: R_X86_64_PC32	.LC31-0x4
     915:	bf 01 00 00 00       	mov    edi,0x1
     91a:	48 89 c2             	mov    rdx,rax
     91d:	31 c0                	xor    eax,eax
     91f:	e8 00 00 00 00       	call   924 <parser+0x250>	920: R_X86_64_PLT32	__printf_chk-0x4
		struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,get_ll_identifier_loc(p.iter));
     924:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     929:	48 8b 3d 00 00 00 00 	mov    rdi,QWORD PTR [rip+0x0]        # 930 <parser+0x25c>	92c: R_X86_64_PC32	general_symbols-0x4
     930:	8b 70 09             	mov    esi,DWORD PTR [rax+0x9]
     933:	e8 00 00 00 00       	call   938 <parser+0x264>	934: R_X86_64_PLT32	get_object_in_fbgc_tuple_object-0x4
     938:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 93f <parser+0x26b>	93b: R_X86_64_PC32	.LC32-0x4
     93f:	bf 01 00 00 00       	mov    edi,0x1
		FBGC_LOGV(PARSER,"Symbol name as a cstring object:%s",content_fbgc_cstr_object(cstr_obj));
     944:	4c 8d 78 01          	lea    r15,[rax+0x1]
		struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,get_ll_identifier_loc(p.iter));
     948:	48 89 c5             	mov    rbp,rax
     94b:	31 c0                	xor    eax,eax
     94d:	4c 89 fa             	mov    rdx,r15
     950:	e8 00 00 00 00       	call   955 <parser+0x281>	951: R_X86_64_PLT32	__printf_chk-0x4
		if(!is_empty_fbgc_ll(p.op) && TOP_LL(p.op)->type == DOT){
     955:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
     95a:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     95e:	48 39 50 09          	cmp    QWORD PTR [rax+0x9],rdx
     962:	74 39                	je     99d <parser+0x2c9>
     964:	80 3a 2a             	cmp    BYTE PTR [rdx],0x2a
     967:	75 34                	jne    99d <parser+0x2c9>
			POP_LL(p.op); //pop the dot object
     969:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
			struct fbgc_ll_base * cstr_holder = _new_fbgc_ll_constant(cstr_obj);
     96d:	48 89 ef             	mov    rdi,rbp
			POP_LL(p.op); //pop the dot object
     970:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
			struct fbgc_ll_base * cstr_holder = _new_fbgc_ll_constant(cstr_obj);
     974:	e8 00 00 00 00       	call   979 <parser+0x2a5>	975: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
			p.iter_prev->next = cstr_holder;
     979:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
     97e:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
			cstr_holder->next = p.iter->next;
     982:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
     987:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     98b:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
			p.iter_prev = cstr_holder;
     98f:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
			set_id_flag_MEMBER(p.iter);
     994:	c6 42 0d 04          	mov    BYTE PTR [rdx+0xd],0x4
		if(!is_empty_fbgc_ll(p.op) && TOP_LL(p.op)->type == DOT){
     998:	e9 3e 02 00 00       	jmp    bdb <parser+0x507>
		else if((cf = new_cfun_object_from_str(*field_obj,content_fbgc_cstr_object(cstr_obj))) != NULL){
     99d:	49 8b 3c 24          	mov    rdi,QWORD PTR [r12]
     9a1:	4c 89 fe             	mov    rsi,r15
     9a4:	e8 00 00 00 00       	call   9a9 <parser+0x2d5>	9a5: R_X86_64_PLT32	new_cfun_object_from_str-0x4
     9a9:	48 89 c7             	mov    rdi,rax
     9ac:	48 85 c0             	test   rax,rax
     9af:	74 25                	je     9d6 <parser+0x302>
			struct fbgc_ll_base * cfun_holder = _new_fbgc_ll_constant(cf);
     9b1:	e8 00 00 00 00       	call   9b6 <parser+0x2e2>	9b2: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
			p.iter_prev->next =  cfun_holder;
     9b6:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
     9bb:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
			cfun_holder->next = p.iter->next;
     9bf:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
     9c4:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     9c8:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
			p.iter_prev = cfun_holder;
     9cc:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
			break;	
     9d1:	e9 e0 0d 00 00       	jmp    17b6 <parser+0x10e2>
		else if(current_scope->type == FIELD || current_scope->type == CLASS){
     9d6:	48 8b 44 24 46       	mov    rax,QWORD PTR [rsp+0x46]
     9db:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     9df:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     9e3:	8a 10                	mov    dl,BYTE PTR [rax]
     9e5:	8d 4a f2             	lea    ecx,[rdx-0xe]
     9e8:	80 e1 f7             	and    cl,0xf7
     9eb:	75 5f                	jne    a4c <parser+0x378>
			if(current_scope->type == FIELD) local_array = cast_fbgc_object_as_field(current_scope)->locals;
     9ed:	80 fa 16             	cmp    dl,0x16
     9f0:	75 06                	jne    9f8 <parser+0x324>
     9f2:	4c 8b 40 11          	mov    r8,QWORD PTR [rax+0x11]
     9f6:	eb 04                	jmp    9fc <parser+0x328>
			else local_array = cast_fbgc_object_as_class(current_scope)->locals;
     9f8:	4c 8b 40 09          	mov    r8,QWORD PTR [rax+0x9]
			for(int i = 0; i<size_fbgc_array_object(local_array); i++){
     9fc:	45 31 ff             	xor    r15d,r15d
     9ff:	44 89 fb             	mov    ebx,r15d
     a02:	4d 39 78 11          	cmp    QWORD PTR [r8+0x11],r15
     a06:	0f 86 fc 0e 00 00    	jbe    1908 <parser+0x1234>
				temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_array,i);
     a0c:	44 89 fe             	mov    esi,r15d
     a0f:	4c 89 c7             	mov    rdi,r8
     a12:	4c 89 44 24 08       	mov    QWORD PTR [rsp+0x8],r8
     a17:	49 ff c7             	inc    r15
     a1a:	e8 00 00 00 00       	call   a1f <parser+0x34b>	a1b: R_X86_64_PLT32	get_address_in_fbgc_array_object-0x4
				if(temp_id->name == cstr_obj) {
     a1f:	4c 8b 44 24 08       	mov    r8,QWORD PTR [rsp+0x8]
     a24:	48 39 28             	cmp    QWORD PTR [rax],rbp
     a27:	75 d6                	jne    9ff <parser+0x32b>
				_info_green("Symbol is already defined @ [%d]\n",where);
     a29:	89 da                	mov    edx,ebx
     a2b:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # a32 <parser+0x35e>	a2e: R_X86_64_PC32	.LC79-0x4
     a32:	bf 08 00 00 00       	mov    edi,0x8
				_info("Symbol is created @ [%d]\n",where);
     a37:	31 c0                	xor    eax,eax
     a39:	e8 00 00 00 00       	call   a3e <parser+0x36a>	a3a: R_X86_64_PLT32	cprintf-0x4
			set_id_flag_GLOBAL(p.iter);
     a3e:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     a43:	c6 40 0d 01          	mov    BYTE PTR [rax+0xd],0x1
			_cast_fbgc_object_as_llidentifier(p.iter)->loc = where;
     a47:	e9 8c 01 00 00       	jmp    bd8 <parser+0x504>
		else if(current_scope->type == FUN){				
     a4c:	80 fa 0d             	cmp    dl,0xd
     a4f:	0f 85 86 01 00 00    	jne    bdb <parser+0x507>
			struct fbgc_object * local_tuple = 
     a55:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
			int where = index_fbgc_tuple_object(local_tuple,cstr_obj);
     a59:	48 89 ee             	mov    rsi,rbp
			struct fbgc_object * local_tuple = 
     a5c:	4c 8b 78 09          	mov    r15,QWORD PTR [rax+0x9]
			int where = index_fbgc_tuple_object(local_tuple,cstr_obj);
     a60:	4c 89 ff             	mov    rdi,r15
     a63:	e8 00 00 00 00       	call   a68 <parser+0x394>	a64: R_X86_64_PLT32	index_fbgc_tuple_object-0x4
     a68:	89 c3                	mov    ebx,eax
			set_id_flag_LOCAL(p.iter);
     a6a:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     a6f:	c6 40 0d 02          	mov    BYTE PTR [rax+0xd],0x2
			if(where == -1) {
     a73:	83 fb ff             	cmp    ebx,0xffffffff
     a76:	0f 85 57 01 00 00    	jne    bd3 <parser+0x4ff>
				_print("p.iter->next type(%s)\n",lltp2str(p.iter->next));
     a7c:	48 8b 78 01          	mov    rdi,QWORD PTR [rax+0x1]
     a80:	e8 00 00 00 00       	call   a85 <parser+0x3b1>	a81: R_X86_64_PLT32	lltp2str-0x4
     a85:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # a8c <parser+0x3b8>	a88: R_X86_64_PC32	.LC34-0x4
     a8c:	bf 65 00 00 00       	mov    edi,0x65
     a91:	48 89 c2             	mov    rdx,rax
     a94:	31 c0                	xor    eax,eax
     a96:	e8 00 00 00 00       	call   a9b <parser+0x3c7>	a97: R_X86_64_PLT32	cprintf-0x4
				if(p.iter->next->type == ASSIGN  || cast_fbgc_object_as_fun(current_scope)->no_arg == 65){
     a9b:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     aa0:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     aa4:	80 38 43             	cmp    BYTE PTR [rax],0x43
     aa7:	74 13                	je     abc <parser+0x3e8>
     aa9:	48 8b 44 24 46       	mov    rax,QWORD PTR [rsp+0x46]
     aae:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     ab2:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     ab6:	80 78 09 41          	cmp    BYTE PTR [rax+0x9],0x41
     aba:	75 6f                	jne    b2b <parser+0x457>
					_info("Id object is an argument of function or a new definition inside the function\n");
     abc:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # ac3 <parser+0x3ef>	abf: R_X86_64_PC32	.LC35-0x4
     ac3:	bf 6f 00 00 00       	mov    edi,0x6f
     ac8:	31 c0                	xor    eax,eax
     aca:	e8 00 00 00 00       	call   acf <parser+0x3fb>	acb: R_X86_64_PLT32	cprintf-0x4
					local_tuple = push_back_fbgc_tuple_object(local_tuple,cstr_obj);
     acf:	48 89 ee             	mov    rsi,rbp
     ad2:	4c 89 ff             	mov    rdi,r15
     ad5:	e8 00 00 00 00       	call   ada <parser+0x406>	ad6: R_X86_64_PLT32	push_back_fbgc_tuple_object-0x4
					_println_object("Fun local tuple:",local_tuple);
     ada:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # ae1 <parser+0x40d>	add: R_X86_64_PC32	.LC36-0x4
     ae1:	bf 01 00 00 00       	mov    edi,0x1
					local_tuple = push_back_fbgc_tuple_object(local_tuple,cstr_obj);
     ae6:	48 89 c5             	mov    rbp,rax
					where = size_fbgc_tuple_object(local_tuple)-1;
     ae9:	8b 40 09             	mov    eax,DWORD PTR [rax+0x9]
     aec:	8d 58 ff             	lea    ebx,[rax-0x1]
					_cast_llbase_as_llconstant(cast_fbgc_object_as_fun(current_scope)->code)->content = local_tuple;
     aef:	48 8b 44 24 46       	mov    rax,QWORD PTR [rsp+0x46]
     af4:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     af8:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     afc:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     b00:	48 89 68 09          	mov    QWORD PTR [rax+0x9],rbp
					_println_object("Fun local tuple:",local_tuple);
     b04:	31 c0                	xor    eax,eax
     b06:	e8 00 00 00 00       	call   b0b <parser+0x437>	b07: R_X86_64_PLT32	cprintf-0x4
     b0b:	48 89 ef             	mov    rdi,rbp
     b0e:	e8 00 00 00 00       	call   b13 <parser+0x43f>	b0f: R_X86_64_PLT32	print_fbgc_object-0x4
     b13:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # b1a <parser+0x446>	b16: R_X86_64_PC32	.LC37-0x4
     b1a:	bf 01 00 00 00       	mov    edi,0x1
     b1f:	31 c0                	xor    eax,eax
     b21:	e8 00 00 00 00       	call   b26 <parser+0x452>	b22: R_X86_64_PLT32	cprintf-0x4
     b26:	e9 a8 00 00 00       	jmp    bd3 <parser+0x4ff>
					_info("Searching ID object in global (class or field)\n");
     b2b:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # b32 <parser+0x45e>	b2e: R_X86_64_PC32	.LC38-0x4
     b32:	bf 6f 00 00 00       	mov    edi,0x6f
     b37:	31 c0                	xor    eax,eax
					for(int i = 0; i<size_fbgc_array_object(local_tuple); i++){
     b39:	45 31 ff             	xor    r15d,r15d
					_info("Searching ID object in global (class or field)\n");
     b3c:	e8 00 00 00 00       	call   b41 <parser+0x46d>	b3d: R_X86_64_PLT32	cprintf-0x4
					local_tuple = cast_fbgc_object_as_field(*field_obj)->locals;
     b41:	49 8b 04 24          	mov    rax,QWORD PTR [r12]
     b45:	48 8b 50 11          	mov    rdx,QWORD PTR [rax+0x11]
					for(int i = 0; i<size_fbgc_array_object(local_tuple); i++){
     b49:	4c 39 7a 11          	cmp    QWORD PTR [rdx+0x11],r15
     b4d:	76 21                	jbe    b70 <parser+0x49c>
						temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_tuple,i);
     b4f:	44 89 fe             	mov    esi,r15d
     b52:	48 89 d7             	mov    rdi,rdx
     b55:	48 89 54 24 08       	mov    QWORD PTR [rsp+0x8],rdx
     b5a:	e8 00 00 00 00       	call   b5f <parser+0x48b>	b5b: R_X86_64_PLT32	get_address_in_fbgc_array_object-0x4
						if(temp_id->name == cstr_obj) where = i; 
     b5f:	48 8b 54 24 08       	mov    rdx,QWORD PTR [rsp+0x8]
     b64:	48 39 28             	cmp    QWORD PTR [rax],rbp
     b67:	41 0f 44 df          	cmove  ebx,r15d
					for(int i = 0; i<size_fbgc_array_object(local_tuple); i++){
     b6b:	49 ff c7             	inc    r15
     b6e:	eb d9                	jmp    b49 <parser+0x475>
					if(where == -1){
     b70:	83 fb ff             	cmp    ebx,0xffffffff
     b73:	75 40                	jne    bb5 <parser+0x4e1>
						_info("Creating a variable inside a previous global\n");
     b75:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # b7c <parser+0x4a8>	b78: R_X86_64_PC32	.LC39-0x4
     b7c:	bf 6f 00 00 00       	mov    edi,0x6f
     b81:	31 c0                	xor    eax,eax
     b83:	e8 00 00 00 00       	call   b88 <parser+0x4b4>	b84: R_X86_64_PLT32	cprintf-0x4
						local_array = cast_fbgc_object_as_field(*field_obj)->locals;
     b88:	49 8b 04 24          	mov    rax,QWORD PTR [r12]
						local_array = push_back_fbgc_array_object(local_array,&id);
     b8c:	48 8d 74 24 16       	lea    rsi,[rsp+0x16]
						local_array = cast_fbgc_object_as_field(*field_obj)->locals;
     b91:	48 8b 78 11          	mov    rdi,QWORD PTR [rax+0x11]
						id.name = cstr_obj; id.content = NULL;
     b95:	48 89 6c 24 16       	mov    QWORD PTR [rsp+0x16],rbp
     b9a:	48 c7 44 24 1e 00 00 00 00 	mov    QWORD PTR [rsp+0x1e],0x0
						local_array = push_back_fbgc_array_object(local_array,&id);
     ba3:	e8 00 00 00 00       	call   ba8 <parser+0x4d4>	ba4: R_X86_64_PLT32	push_back_fbgc_array_object-0x4
						cast_fbgc_object_as_field(*field_obj)->locals = local_array;
     ba8:	49 8b 14 24          	mov    rdx,QWORD PTR [r12]
						where = size_fbgc_array_object(local_array)-1;
     bac:	8b 58 11             	mov    ebx,DWORD PTR [rax+0x11]
						cast_fbgc_object_as_field(*field_obj)->locals = local_array;
     baf:	48 89 42 11          	mov    QWORD PTR [rdx+0x11],rax
						where = size_fbgc_array_object(local_array)-1;
     bb3:	ff cb                	dec    ebx
					_print("Id object is found @ [%d]",where);
     bb5:	89 da                	mov    edx,ebx
     bb7:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # bbe <parser+0x4ea>	bba: R_X86_64_PC32	.LC40-0x4
     bbe:	bf 65 00 00 00       	mov    edi,0x65
     bc3:	31 c0                	xor    eax,eax
     bc5:	e8 00 00 00 00       	call   bca <parser+0x4f6>	bc6: R_X86_64_PLT32	cprintf-0x4
					set_id_flag_GLOBAL(p.iter);
     bca:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     bcf:	c6 40 0d 01          	mov    BYTE PTR [rax+0xd],0x1
			set_ll_identifier_loc(p.iter,where);
     bd3:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     bd8:	89 58 09             	mov    DWORD PTR [rax+0x9],ebx
		p.iter_prev->next = p.iter->next;
     bdb:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
     be0:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
     be5:	48 8b 56 01          	mov    rdx,QWORD PTR [rsi+0x1]
     be9:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
		_push_front_fbgc_ll(p.op,p.iter);
     bed:	e9 89 0a 00 00       	jmp    167b <parser+0xfa7>
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
     bf2:	48 89 ef             	mov    rdi,rbp
     bf5:	e8 00 00 00 00       	call   bfa <parser+0x526>	bf6: R_X86_64_PLT32	gm_seek_left-0x4
     bfa:	8a 4c 24 4e          	mov    cl,BYTE PTR [rsp+0x4e]
     bfe:	84 c9                	test   cl,cl
     c00:	0f 85 d9 0c 00 00    	jne    18df <parser+0x120b>
		p.iter_prev->next = p.iter->next;
     c06:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     c0b:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
		fbgc_token top_type = TOP_LL(p.op)->type;
     c10:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
		p.iter_prev->next = p.iter->next;
     c15:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     c19:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
		fbgc_token top_type = TOP_LL(p.op)->type;
     c1d:	48 8b 77 01          	mov    rsi,QWORD PTR [rdi+0x1]
     c21:	8a 06                	mov    al,BYTE PTR [rsi]
		switch(top_type){
     c23:	3c 20                	cmp    al,0x20
     c25:	77 27                	ja     c4e <parser+0x57a>
     c27:	3c 17                	cmp    al,0x17
     c29:	0f 86 86 02 00 00    	jbe    eb5 <parser+0x7e1>
     c2f:	83 e8 18             	sub    eax,0x18
     c32:	3c 08                	cmp    al,0x8
     c34:	0f 87 7b 02 00 00    	ja     eb5 <parser+0x7e1>
     c3a:	4c 8d 05 00 00 00 00 	lea    r8,[rip+0x0]        # c41 <parser+0x56d>	c3d: R_X86_64_PC32	.rodata+0x90
     c41:	0f b6 c0             	movzx  eax,al
     c44:	49 63 04 80          	movsxd rax,DWORD PTR [r8+rax*4]
     c48:	4c 01 c0             	add    rax,r8
     c4b:	3e ff e0             	notrack jmp rax
     c4e:	3c 50                	cmp    al,0x50
     c50:	0f 84 77 02 00 00    	je     ecd <parser+0x7f9>
     c56:	e9 5a 02 00 00       	jmp    eb5 <parser+0x7e1>
				struct fbgc_ll_base * if_obj = _top_and_pop_front_fbgc_ll(p.op);
     c5b:	e8 00 00 00 00       	call   c60 <parser+0x58c>	c5c: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(if_obj)->content,if_obj);
     c60:	48 8b 78 09          	mov    rdi,QWORD PTR [rax+0x9]
     c64:	48 89 c6             	mov    rsi,rax
				struct fbgc_ll_base * if_obj = _top_and_pop_front_fbgc_ll(p.op);
     c67:	48 89 c3             	mov    rbx,rax
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(if_obj)->content,if_obj);
     c6a:	e8 00 00 00 00       	call   c6f <parser+0x59b>	c6b: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				_cast_llbase_as_lljumper(if_obj)->content = p.iter_prev; //now assign where to jump
     c6f:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
     c74:	48 89 43 09          	mov    QWORD PTR [rbx+0x9],rax
				break;
     c78:	e9 50 02 00 00       	jmp    ecd <parser+0x7f9>
				struct fbgc_ll_base * wh_obj = _top_and_pop_front_fbgc_ll(p.op);
     c7d:	e8 00 00 00 00       	call   c82 <parser+0x5ae>	c7e: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(wh_obj)->content,wh_obj);
     c82:	48 8b 78 09          	mov    rdi,QWORD PTR [rax+0x9]
     c86:	48 89 c6             	mov    rsi,rax
				struct fbgc_ll_base * wh_obj = _top_and_pop_front_fbgc_ll(p.op);
     c89:	48 89 c3             	mov    rbx,rax
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(wh_obj)->content,wh_obj);
     c8c:	e8 00 00 00 00       	call   c91 <parser+0x5bd>	c8d: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
     c91:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
     c96:	e8 00 00 00 00       	call   c9b <parser+0x5c7>	c97: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,jump_obj);
     c9b:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     ca0:	48 89 c6             	mov    rsi,rax
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
     ca3:	48 89 c5             	mov    rbp,rax
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,jump_obj);
     ca6:	e8 00 00 00 00       	call   cab <parser+0x5d7>	ca7: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     cab:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
				_cast_llbase_as_lljumper(wh_obj)->content = jump_obj;
     cb0:	eb 3b                	jmp    ced <parser+0x619>
				struct fbgc_ll_base * for_obj = _top_and_pop_front_fbgc_ll(p.op);
     cb2:	e8 00 00 00 00       	call   cb7 <parser+0x5e3>	cb3: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(for_obj)->content,for_obj);
     cb7:	48 8b 78 09          	mov    rdi,QWORD PTR [rax+0x9]
     cbb:	48 89 c6             	mov    rsi,rax
				struct fbgc_ll_base * for_obj = _top_and_pop_front_fbgc_ll(p.op);
     cbe:	48 89 c3             	mov    rbx,rax
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(for_obj)->content,for_obj);
     cc1:	e8 00 00 00 00       	call   cc6 <parser+0x5f2>	cc2: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
     cc6:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
     ccb:	e8 00 00 00 00       	call   cd0 <parser+0x5fc>	ccc: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,jump_obj);
     cd0:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     cd5:	48 89 c6             	mov    rsi,rax
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
     cd8:	48 89 c5             	mov    rbp,rax
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,jump_obj);
     cdb:	e8 00 00 00 00       	call   ce0 <parser+0x60c>	cdc: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     ce0:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
				_cast_llbase_as_lljumper(jump_obj)->content = _cast_llbase_as_lljumper(for_obj)->content;	
     ce5:	48 8b 43 09          	mov    rax,QWORD PTR [rbx+0x9]
     ce9:	48 89 45 09          	mov    QWORD PTR [rbp+0x9],rax
				_cast_llbase_as_lljumper(for_obj)->content = jump_obj;
     ced:	48 89 6b 09          	mov    QWORD PTR [rbx+0x9],rbp
				break;
     cf1:	e9 d7 01 00 00       	jmp    ecd <parser+0x7f9>
				if(is_constant_and_token(p.iter_prev,FUN) ){
     cf6:	80 3a 02             	cmp    BYTE PTR [rdx],0x2
     cf9:	75 1a                	jne    d15 <parser+0x641>
     cfb:	48 8b 42 09          	mov    rax,QWORD PTR [rdx+0x9]
     cff:	80 38 0d             	cmp    BYTE PTR [rax],0xd
     d02:	75 11                	jne    d15 <parser+0x641>
					p.error_code = _FBGC_SYNTAX_ERROR;
     d04:	c6 44 24 4e 02       	mov    BYTE PTR [rsp+0x4e],0x2
     d09:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # d10 <parser+0x63c>	d0c: R_X86_64_PC32	.LC41-0x4
     d10:	e9 c2 00 00 00       	jmp    dd7 <parser+0x703>
				struct fbgc_ll_base * fun_holder = _cast_llbase_as_lljumper(TOP_LL(p.op))->content;
     d15:	48 8b 5e 09          	mov    rbx,QWORD PTR [rsi+0x9]
     d19:	bf 01 00 00 00       	mov    edi,0x1
     d1e:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # d25 <parser+0x651>	d21: R_X86_64_PC32	.LC42-0x4
				struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;
     d25:	48 8b 6b 09          	mov    rbp,QWORD PTR [rbx+0x9]
				struct fbgc_object * tp = _cast_llbase_as_llconstant(cast_fbgc_object_as_fun(fun_obj)->code)->content;
     d29:	48 8b 45 01          	mov    rax,QWORD PTR [rbp+0x1]
				cast_fbgc_object_as_fun(fun_obj)->no_locals = size_fbgc_tuple_object(tp);
     d2d:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     d31:	48 8b 50 09          	mov    rdx,QWORD PTR [rax+0x9]
     d35:	31 c0                	xor    eax,eax
     d37:	88 55 0a             	mov    BYTE PTR [rbp+0xa],dl
     d3a:	e8 00 00 00 00       	call   d3f <parser+0x66b>	d3b: R_X86_64_PLT32	__printf_chk-0x4
				cast_fbgc_object_as_fun(fun_obj)->code = fun_holder->next;
     d3f:	48 8b 43 01          	mov    rax,QWORD PTR [rbx+0x1]
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(__fbgc_nil));
     d43:	48 8b 3d 00 00 00 00 	mov    rdi,QWORD PTR [rip+0x0]        # d4a <parser+0x676>	d46: R_X86_64_PC32	__fbgc_nil-0x4
				cast_fbgc_object_as_fun(fun_obj)->code = fun_holder->next;
     d4a:	48 89 45 01          	mov    QWORD PTR [rbp+0x1],rax
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(__fbgc_nil));
     d4e:	e8 00 00 00 00       	call   d53 <parser+0x67f>	d4f: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     d53:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     d58:	48 89 c6             	mov    rsi,rax
     d5b:	e8 00 00 00 00       	call   d60 <parser+0x68c>	d5c: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_base(RETURN));
     d60:	bf 21 00 00 00       	mov    edi,0x21
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(__fbgc_nil));
     d65:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_base(RETURN));
     d6a:	e8 00 00 00 00       	call   d6f <parser+0x69b>	d6b: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
     d6f:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     d74:	48 89 c6             	mov    rsi,rax
     d77:	e8 00 00 00 00       	call   d7c <parser+0x6a8>	d78: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				p.iter_prev->next = NULL; //last token in function code shows NULL
     d7c:	48 c7 40 01 00 00 00 00 	mov    QWORD PTR [rax+0x1],0x0
				POP_LL(p.op); //Remove fun_make
     d84:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
				p.iter_prev = fun_holder;
     d89:	48 89 5c 24 36       	mov    QWORD PTR [rsp+0x36],rbx
				POP_LL(p.op); //Remove fun_make
     d8e:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     d92:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     d96:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
				POP_LL(p.scope_list); //remove this function from scope list
     d9a:	48 8b 44 24 46       	mov    rax,QWORD PTR [rsp+0x46]
     d9f:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     da3:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     da7:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
				p.iter_prev->next = p.iter->next;
     dab:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     db0:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     db4:	48 89 43 01          	mov    QWORD PTR [rbx+0x1],rax
				break;
     db8:	e9 10 01 00 00       	jmp    ecd <parser+0x7f9>
				if(is_constant_and_token(p.iter_prev,CLASS)){
     dbd:	80 3a 02             	cmp    BYTE PTR [rdx],0x2
     dc0:	75 26                	jne    de8 <parser+0x714>
     dc2:	48 8b 42 09          	mov    rax,QWORD PTR [rdx+0x9]
     dc6:	80 38 0e             	cmp    BYTE PTR [rax],0xe
     dc9:	75 1d                	jne    de8 <parser+0x714>
					p.error_code = _FBGC_SYNTAX_ERROR;
     dcb:	c6 44 24 4e 02       	mov    BYTE PTR [rsp+0x4e],0x2
     dd0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # dd7 <parser+0x703>	dd3: R_X86_64_PC32	.LC43-0x4
     dd7:	bf 01 00 00 00       	mov    edi,0x1
     ddc:	31 c0                	xor    eax,eax
     dde:	e8 00 00 00 00       	call   de3 <parser+0x70f>	ddf: R_X86_64_PLT32	__printf_chk-0x4
					goto PARSER_ERROR_LABEL;
     de3:	e9 f7 0a 00 00       	jmp    18df <parser+0x120b>
				struct fbgc_ll_base * cls_holder = _cast_llbase_as_lljumper(TOP_LL(p.op))->content;
     de8:	48 8b 46 09          	mov    rax,QWORD PTR [rsi+0x9]
				struct fbgc_object * cls_obj = _cast_llbase_as_llconstant(cls_holder)->content;
     dec:	48 8b 68 09          	mov    rbp,QWORD PTR [rax+0x9]
				while(cls_holder != cast_fbgc_object_as_class(cls_obj)->code){
     df0:	48 8b 55 01          	mov    rdx,QWORD PTR [rbp+0x1]
     df4:	48 39 c2             	cmp    rdx,rax
     df7:	74 08                	je     e01 <parser+0x72d>
					cls_holder = cls_holder->next;
     df9:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
					arg_no++;
     dfd:	ff c1                	inc    ecx
     dff:	eb f3                	jmp    df4 <parser+0x720>
				FBGC_LOGV(PARSER,"Class inheritance arg no:%d\n",arg_no);
     e01:	44 0f b6 f9          	movzx  r15d,cl
     e05:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # e0c <parser+0x738>	e08: R_X86_64_PC32	.LC44-0x4
     e0c:	bf 01 00 00 00       	mov    edi,0x1
     e11:	31 c0                	xor    eax,eax
     e13:	44 89 fa             	mov    edx,r15d
     e16:	e8 00 00 00 00       	call   e1b <parser+0x747>	e17: R_X86_64_PLT32	__printf_chk-0x4
				cls_holder = cast_fbgc_object_as_class(cls_obj)->code;
     e1b:	48 8b 5d 01          	mov    rbx,QWORD PTR [rbp+0x1]
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(cls_obj));
     e1f:	48 89 ef             	mov    rdi,rbp
				cast_fbgc_object_as_class(cls_obj)->code = cast_fbgc_object_as_class(cls_obj)->code->next;
     e22:	48 8b 43 01          	mov    rax,QWORD PTR [rbx+0x1]
     e26:	48 89 45 01          	mov    QWORD PTR [rbp+0x1],rax
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(cls_obj));
     e2a:	e8 00 00 00 00       	call   e2f <parser+0x75b>	e2b: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     e2f:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     e34:	48 89 c6             	mov    rsi,rax
     e37:	e8 00 00 00 00       	call   e3c <parser+0x768>	e38: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_base(RETURN));
     e3c:	bf 21 00 00 00       	mov    edi,0x21
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(cls_obj));
     e41:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_base(RETURN));
     e46:	e8 00 00 00 00       	call   e4b <parser+0x777>	e47: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
     e4b:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     e50:	48 89 c6             	mov    rsi,rax
     e53:	e8 00 00 00 00       	call   e58 <parser+0x784>	e54: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_opcode_int(CLASS_CALL,arg_no));
     e58:	44 89 fe             	mov    esi,r15d
     e5b:	bf 58 00 00 00       	mov    edi,0x58
				p.iter_prev->next = NULL;
     e60:	48 c7 40 01 00 00 00 00 	mov    QWORD PTR [rax+0x1],0x0
				POP_LL(p.op); //Remove class_make
     e68:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
     e6d:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     e71:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     e75:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
				POP_LL(p.scope_list); //remove class from scope list
     e79:	48 8b 44 24 46       	mov    rax,QWORD PTR [rsp+0x46]
     e7e:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     e82:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     e86:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
				p.iter_prev = cls_holder; //Start from cls_holder
     e8a:	48 89 5c 24 36       	mov    QWORD PTR [rsp+0x36],rbx
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_opcode_int(CLASS_CALL,arg_no));
     e8f:	e8 00 00 00 00       	call   e94 <parser+0x7c0>	e90: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     e94:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     e99:	48 89 c6             	mov    rsi,rax
     e9c:	e8 00 00 00 00       	call   ea1 <parser+0x7cd>	e9d: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
				p.iter_prev->next = p.iter->next;
     ea1:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_opcode_int(CLASS_CALL,arg_no));
     ea6:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
				p.iter_prev->next = p.iter->next;
     eab:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     eaf:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
				break;
     eb3:	eb 18                	jmp    ecd <parser+0x7f9>
     eb5:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # ebc <parser+0x7e8>	eb8: R_X86_64_PC32	.LC45-0x4
     ebc:	bf 01 00 00 00       	mov    edi,0x1
     ec1:	31 c0                	xor    eax,eax
     ec3:	e8 00 00 00 00       	call   ec8 <parser+0x7f4>	ec4: R_X86_64_PLT32	__printf_chk-0x4
				p.error_code = _FBGC_SYNTAX_ERROR;
     ec8:	e9 f2 02 00 00       	jmp    11bf <parser+0xaeb>
		while(TOP_LL(p.op)->type == JUMP){
     ecd:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
     ed2:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
     ed6:	80 38 50             	cmp    BYTE PTR [rax],0x50
     ed9:	0f 85 d7 08 00 00    	jne    17b6 <parser+0x10e2>
			struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
     edf:	e8 00 00 00 00       	call   ee4 <parser+0x810>	ee0: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
			jump_obj->next = _cast_llbase_as_lljumper(jump_obj)->content->next;
     ee4:	48 8b 50 09          	mov    rdx,QWORD PTR [rax+0x9]
     ee8:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     eec:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
			_cast_llbase_as_lljumper(jump_obj)->content->next = jump_obj;
     ef0:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
			_cast_llbase_as_lljumper(jump_obj)->content = p.iter_prev;	
     ef4:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
     ef9:	48 89 50 09          	mov    QWORD PTR [rax+0x9],rdx
     efd:	eb ce                	jmp    ecd <parser+0x7f9>
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
     eff:	48 89 ef             	mov    rdi,rbp
     f02:	e8 00 00 00 00       	call   f07 <parser+0x833>	f03: R_X86_64_PLT32	gm_seek_left-0x4
     f07:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
     f0c:	0f 85 cd 09 00 00    	jne    18df <parser+0x120b>
		struct fbgc_object * fun_obj = new_fbgc_fun_object();
     f12:	31 c0                	xor    eax,eax
     f14:	e8 00 00 00 00       	call   f19 <parser+0x845>	f15: R_X86_64_PLT32	new_fbgc_fun_object-0x4
		struct fbgc_ll_base * fun_holder = _new_fbgc_ll_constant(fun_obj);
     f19:	48 89 c7             	mov    rdi,rax
		struct fbgc_object * fun_obj = new_fbgc_fun_object();
     f1c:	48 89 c5             	mov    rbp,rax
		struct fbgc_ll_base * fun_holder = _new_fbgc_ll_constant(fun_obj);
     f1f:	e8 00 00 00 00       	call   f24 <parser+0x850>	f20: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,fun_holder);
     f24:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
     f29:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     f2e:	48 89 c2             	mov    rdx,rax
		struct fbgc_ll_base * fun_holder = _new_fbgc_ll_constant(fun_obj);
     f31:	48 89 c3             	mov    rbx,rax
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,fun_holder);
     f34:	e8 00 00 00 00       	call   f39 <parser+0x865>	f35: R_X86_64_PLT32	_insert_fbgc_ll-0x4
		_cast_llbase_as_lljumper(p.iter)->content = fun_holder;
     f39:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
		_push_front_fbgc_ll(p.op,p.iter);
     f3e:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,fun_holder);
     f43:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
		_cast_llbase_as_lljumper(p.iter)->content = fun_holder;
     f48:	48 89 5e 09          	mov    QWORD PTR [rsi+0x9],rbx
		_push_front_fbgc_ll(p.op,p.iter);
     f4c:	e8 00 00 00 00       	call   f51 <parser+0x87d>	f4d: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
		_push_front_fbgc_ll(p.scope_list,_new_fbgc_ll_constant(fun_obj));
     f51:	48 89 ef             	mov    rdi,rbp
     f54:	e8 00 00 00 00       	call   f59 <parser+0x885>	f55: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     f59:	48 8b 7c 24 46       	mov    rdi,QWORD PTR [rsp+0x46]
     f5e:	48 89 c6             	mov    rsi,rax
     f61:	e8 00 00 00 00       	call   f66 <parser+0x892>	f62: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
		cast_fbgc_object_as_fun(fun_obj)->code = _new_fbgc_ll_constant(new_fbgc_tuple_object(0));
     f66:	31 ff                	xor    edi,edi
     f68:	e8 00 00 00 00       	call   f6d <parser+0x899>	f69: R_X86_64_PLT32	new_fbgc_tuple_object-0x4
     f6d:	48 89 c7             	mov    rdi,rax
     f70:	e8 00 00 00 00       	call   f75 <parser+0x8a1>	f71: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     f75:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # f7c <parser+0x8a8>	f78: R_X86_64_PC32	.LC46-0x4
     f7c:	48 89 45 01          	mov    QWORD PTR [rbp+0x1],rax
     f80:	eb 56                	jmp    fd8 <parser+0x904>
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
     f82:	48 89 ef             	mov    rdi,rbp
     f85:	e8 00 00 00 00       	call   f8a <parser+0x8b6>	f86: R_X86_64_PLT32	gm_seek_left-0x4
     f8a:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
     f8f:	0f 85 4a 09 00 00    	jne    18df <parser+0x120b>
		struct fbgc_object * cls_obj = new_fbgc_class_object();
     f95:	31 c0                	xor    eax,eax
     f97:	e8 00 00 00 00       	call   f9c <parser+0x8c8>	f98: R_X86_64_PLT32	new_fbgc_class_object-0x4
     f9c:	48 89 c7             	mov    rdi,rax
		struct fbgc_ll_base * cls_holder = _new_fbgc_ll_constant(cls_obj);
     f9f:	e8 00 00 00 00       	call   fa4 <parser+0x8d0>	fa0: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,cls_holder);
     fa4:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
     fa9:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     fae:	48 89 c2             	mov    rdx,rax
		struct fbgc_ll_base * cls_holder = _new_fbgc_ll_constant(cls_obj);
     fb1:	48 89 c3             	mov    rbx,rax
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,cls_holder);
     fb4:	e8 00 00 00 00       	call   fb9 <parser+0x8e5>	fb5: R_X86_64_PLT32	_insert_fbgc_ll-0x4
		_cast_llbase_as_lljumper(p.iter)->content = cls_holder;
     fb9:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
		_push_front_fbgc_ll(p.op,p.iter);
     fbe:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,cls_holder);
     fc3:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
		_cast_llbase_as_lljumper(p.iter)->content = cls_holder;
     fc8:	48 89 5e 09          	mov    QWORD PTR [rsi+0x9],rbx
		_push_front_fbgc_ll(p.op,p.iter);
     fcc:	e8 00 00 00 00       	call   fd1 <parser+0x8fd>	fcd: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     fd1:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # fd8 <parser+0x904>	fd4: R_X86_64_PC32	.LC47-0x4
     fd8:	bf 01 00 00 00       	mov    edi,0x1
     fdd:	31 c0                	xor    eax,eax
     fdf:	e8 00 00 00 00       	call   fe4 <parser+0x910>	fe0: R_X86_64_PLT32	__printf_chk-0x4
     fe4:	e9 cd 07 00 00       	jmp    17b6 <parser+0x10e2>
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
     fe9:	48 89 ef             	mov    rdi,rbp
     fec:	e8 00 00 00 00       	call   ff1 <parser+0x91d>	fed: R_X86_64_PLT32	gm_seek_left-0x4
     ff1:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
     ff6:	0f 85 e3 08 00 00    	jne    18df <parser+0x120b>
     ffc:	e9 be 01 00 00       	jmp    11bf <parser+0xaeb>
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
    1001:	48 89 ef             	mov    rdi,rbp
    1004:	e8 00 00 00 00       	call   1009 <parser+0x935>	1005: R_X86_64_PLT32	gm_seek_left-0x4
    1009:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
    100e:	0f 85 cb 08 00 00    	jne    18df <parser+0x120b>
		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
    1014:	bf 50 00 00 00       	mov    edi,0x50
    1019:	e8 00 00 00 00       	call   101e <parser+0x94a>	101a: R_X86_64_PLT32	_new_fbgc_ll_jumper-0x4
		if(TOP_LL(p.op)->type != WHILE){
    101e:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
    1023:	48 89 c6             	mov    rsi,rax
		if(TOP_LL(p.op)->type != WHILE){
    1026:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
    102a:	80 38 1c             	cmp    BYTE PTR [rax],0x1c
    102d:	74 36                	je     1065 <parser+0x991>
			_cast_llbase_as_lljumper(jump_obj)->content = p.iter_prev;
    102f:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
    1034:	48 89 46 09          	mov    QWORD PTR [rsi+0x9],rax
		_push_front_fbgc_ll(p.op,jump_obj);
    1038:	e8 00 00 00 00       	call   103d <parser+0x969>	1039: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
		p.iter_prev->next = p.iter->next;
    103d:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
    1042:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
		_push_front_fbgc_ll(p.op,p.iter);	
    1047:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
		p.iter_prev->next = p.iter->next;
    104c:	48 8b 56 01          	mov    rdx,QWORD PTR [rsi+0x1]
    1050:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
		_push_front_fbgc_ll(p.op,p.iter);	
    1054:	e8 00 00 00 00       	call   1059 <parser+0x985>	1055: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
    1059:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1060 <parser+0x98c>	105c: R_X86_64_PC32	.LC48-0x4
    1060:	e9 73 ff ff ff       	jmp    fd8 <parser+0x904>
			assert(0);
    1065:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 106c <parser+0x998>	1068: R_X86_64_PC32	.rodata+0xb4
    106c:	ba 5f 03 00 00       	mov    edx,0x35f
    1071:	e9 bc 01 00 00       	jmp    1232 <parser+0xb5e>
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
    1076:	48 89 ef             	mov    rdi,rbp
    1079:	e8 00 00 00 00       	call   107e <parser+0x9aa>	107a: R_X86_64_PLT32	gm_seek_left-0x4
    107e:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
    1083:	0f 85 56 08 00 00    	jne    18df <parser+0x120b>
		_cast_llbase_as_lljumper(p.iter)->content = p.iter_prev;
    1089:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
    108e:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
    1093:	bf 50 00 00 00       	mov    edi,0x50
		_cast_llbase_as_lljumper(p.iter)->content = p.iter_prev;
    1098:	48 89 50 09          	mov    QWORD PTR [rax+0x9],rdx
		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
    109c:	e8 00 00 00 00       	call   10a1 <parser+0x9cd>	109d: R_X86_64_PLT32	_new_fbgc_ll_jumper-0x4
		_push_front_fbgc_ll(p.op,jump_obj);
    10a1:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
    10a6:	48 89 c6             	mov    rsi,rax
		_cast_llbase_as_lljumper(jump_obj)->content = p.iter_prev;
    10a9:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
    10ae:	48 89 46 09          	mov    QWORD PTR [rsi+0x9],rax
		_push_front_fbgc_ll(p.op,jump_obj);
    10b2:	e8 00 00 00 00       	call   10b7 <parser+0x9e3>	10b3: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
		p.iter_prev->next = p.iter->next;	
    10b7:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
    10bc:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
		_push_front_fbgc_ll(p.op,p.iter);
    10c1:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
		p.iter_prev->next = p.iter->next;	
    10c6:	48 8b 56 01          	mov    rdx,QWORD PTR [rsi+0x1]
    10ca:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
		_push_front_fbgc_ll(p.op,p.iter);
    10ce:	e8 00 00 00 00       	call   10d3 <parser+0x9ff>	10cf: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
    10d3:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 10da <parser+0xa06>	10d6: R_X86_64_PC32	.LC49-0x4
    10da:	e9 f9 fe ff ff       	jmp    fd8 <parser+0x904>
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
    10df:	48 89 ef             	mov    rdi,rbp
    10e2:	e8 00 00 00 00       	call   10e7 <parser+0xa13>	10e3: R_X86_64_PLT32	gm_seek_left-0x4
    10e7:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
    10ec:	0f 85 ed 07 00 00    	jne    18df <parser+0x120b>
		struct fbgc_ll_base * op_top = TOP_LL(p.op);
    10f2:	48 8b 54 24 3e       	mov    rdx,QWORD PTR [rsp+0x3e]
    10f7:	48 8b 5a 01          	mov    rbx,QWORD PTR [rdx+0x1]
			if(op_top->type == WHILE || op_top->type == FOR){
    10fb:	8a 03                	mov    al,BYTE PTR [rbx]
    10fd:	83 e8 1c             	sub    eax,0x1c
    1100:	3c 01                	cmp    al,0x1
    1102:	0f 87 89 00 00 00    	ja     1191 <parser+0xabd>
				FBGC_LOGV(PARSER,"%s found %s in op-stack",lltp2str(p.iter),lltp2str(op_top));
    1108:	48 89 df             	mov    rdi,rbx
    110b:	e8 00 00 00 00       	call   1110 <parser+0xa3c>	110c: R_X86_64_PLT32	lltp2str-0x4
    1110:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
    1115:	49 89 c7             	mov    r15,rax
    1118:	e8 00 00 00 00       	call   111d <parser+0xa49>	1119: R_X86_64_PLT32	lltp2str-0x4
    111d:	bf 01 00 00 00       	mov    edi,0x1
    1122:	4c 89 f9             	mov    rcx,r15
    1125:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 112c <parser+0xa58>	1128: R_X86_64_PC32	.LC50-0x4
    112c:	48 89 c2             	mov    rdx,rax
    112f:	31 c0                	xor    eax,eax
    1131:	e8 00 00 00 00       	call   1136 <parser+0xa62>	1132: R_X86_64_PLT32	__printf_chk-0x4
				if(p.iter->type == CONT){
    1136:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
    113b:	80 3f 1f             	cmp    BYTE PTR [rdi],0x1f
    113e:	75 28                	jne    1168 <parser+0xa94>
					assert(op_top->next->type == JUMP); //Is there any way that we cant see jump here?
    1140:	48 8b 5b 01          	mov    rbx,QWORD PTR [rbx+0x1]
    1144:	80 3b 50             	cmp    BYTE PTR [rbx],0x50
    1147:	74 1f                	je     1168 <parser+0xa94>
    1149:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 1150 <parser+0xa7c>	114c: R_X86_64_PC32	.rodata+0xb4
    1150:	ba 8c 03 00 00       	mov    edx,0x38c
    1155:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 115c <parser+0xa88>	1158: R_X86_64_PC32	.LC4-0x4
    115c:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # 1163 <parser+0xa8f>	115f: R_X86_64_PC32	.LC51-0x4
    1163:	e8 00 00 00 00       	call   1168 <parser+0xa94>	1164: R_X86_64_PLT32	__assert_fail-0x4
				_cast_llbase_as_lljumper(p.iter)->content = op_top;
    1168:	48 89 5f 09          	mov    QWORD PTR [rdi+0x9],rbx
		p.iter_prev = p.iter;
    116c:	48 89 7c 24 36       	mov    QWORD PTR [rsp+0x36],rdi
		FBGC_LOGV(PARSER,"Moving in the list %s stays",lltp2str(p.iter));
    1171:	e8 00 00 00 00       	call   1176 <parser+0xaa2>	1172: R_X86_64_PLT32	lltp2str-0x4
    1176:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 117d <parser+0xaa9>	1179: R_X86_64_PC32	.LC52-0x4
    117d:	bf 01 00 00 00       	mov    edi,0x1
    1182:	48 89 c2             	mov    rdx,rax
    1185:	31 c0                	xor    eax,eax
    1187:	e8 00 00 00 00       	call   118c <parser+0xab8>	1188: R_X86_64_PLT32	__printf_chk-0x4
    118c:	e9 25 06 00 00       	jmp    17b6 <parser+0x10e2>
			op_top = op_top->next;
    1191:	48 8b 5b 01          	mov    rbx,QWORD PTR [rbx+0x1]
			if(op_top == _cast_llbase_as_ll(p.op)->tail){
    1195:	48 39 5a 09          	cmp    QWORD PTR [rdx+0x9],rbx
    1199:	0f 85 5c ff ff ff    	jne    10fb <parser+0xa27>
				FBGC_LOGE("%s is not inside a loop",lltp2str(p.iter));
    119f:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
    11a4:	e8 00 00 00 00       	call   11a9 <parser+0xad5>	11a5: R_X86_64_PLT32	lltp2str-0x4
    11a9:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 11b0 <parser+0xadc>	11ac: R_X86_64_PC32	.LC53-0x4
    11b0:	bf 01 00 00 00       	mov    edi,0x1
    11b5:	48 89 c2             	mov    rdx,rax
    11b8:	31 c0                	xor    eax,eax
    11ba:	e8 00 00 00 00       	call   11bf <parser+0xaeb>	11bb: R_X86_64_PLT32	__printf_chk-0x4
				p.error_code = _FBGC_SYNTAX_ERROR;
    11bf:	c6 44 24 4e 02       	mov    BYTE PTR [rsp+0x4e],0x2
				goto PARSER_ERROR_LABEL;
    11c4:	e9 16 07 00 00       	jmp    18df <parser+0x120b>
		p.iter_prev->next = p.iter->next;
    11c9:	48 8b 57 01          	mov    rdx,QWORD PTR [rdi+0x1]
    11cd:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
    11d2:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
		while( is_not_empty_fbgc_ll(p.op) && compare_operators(get_fbgc_object_type(TOP_LL(p.op)),p.iter->type) ){
    11d6:	e9 20 01 00 00       	jmp    12fb <parser+0xc27>
					cprintf(111,"Syntax Error :");
    11db:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 11e2 <parser+0xb0e>	11de: R_X86_64_PC32	.LC57-0x4
    11e2:	bf 6f 00 00 00       	mov    edi,0x6f
    11e7:	31 c0                	xor    eax,eax
    11e9:	e8 00 00 00 00       	call   11ee <parser+0xb1a>	11ea: R_X86_64_PLT32	cprintf-0x4
					cprintf(011,"Op Top :[%s], p.iter:[%s]\n",object_name_array[get_fbgc_object_type(TOP_LL(p.op))],object_name_array[p.iter->type]);
    11ee:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
    11f3:	bf 09 00 00 00       	mov    edi,0x9
    11f8:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 11ff <parser+0xb2b>	11fb: R_X86_64_PC32	object_name_array-0x4
    11ff:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1206 <parser+0xb32>	1202: R_X86_64_PC32	.LC58-0x4
    1206:	0f b6 00             	movzx  eax,BYTE PTR [rax]
    1209:	48 8b 0c c2          	mov    rcx,QWORD PTR [rdx+rax*8]
    120d:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    1212:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    1216:	8a 00                	mov    al,BYTE PTR [rax]
    1218:	83 e0 7f             	and    eax,0x7f
    121b:	48 8b 14 c2          	mov    rdx,QWORD PTR [rdx+rax*8]
    121f:	31 c0                	xor    eax,eax
    1221:	e8 00 00 00 00       	call   1226 <parser+0xb52>	1222: R_X86_64_PLT32	cprintf-0x4
					assert(0);
    1226:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 122d <parser+0xb59>	1229: R_X86_64_PC32	.rodata+0xb4
    122d:	ba 06 04 00 00       	mov    edx,0x406
    1232:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1239 <parser+0xb65>	1235: R_X86_64_PC32	.LC4-0x4
    1239:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # 1240 <parser+0xb6c>	123c: R_X86_64_PC32	.LC6-0x4
    1240:	e9 1e ff ff ff       	jmp    1163 <parser+0xa8f>
		while( is_not_empty_fbgc_ll(p.op) && compare_operators(get_fbgc_object_type(TOP_LL(p.op)),p.iter->type) ){
    1245:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
    124a:	40 8a 38             	mov    dil,BYTE PTR [rax]
    124d:	0f b6 32             	movzx  esi,BYTE PTR [rdx]
    1250:	83 e7 7f             	and    edi,0x7f
    1253:	e8 00 00 00 00       	call   1258 <parser+0xb84>	1254: R_X86_64_PLT32	compare_operators-0x4
    1258:	84 c0                	test   al,al
    125a:	0f 84 44 01 00 00    	je     13a4 <parser+0xcd0>
			if( (p.error_code = gm_seek_right(&p)) != _FBGC_NO_ERROR ){
    1260:	48 89 ef             	mov    rdi,rbp
    1263:	e8 00 00 00 00       	call   1268 <parser+0xb94>	1264: R_X86_64_PLT32	gm_seek_right-0x4
    1268:	88 44 24 4e          	mov    BYTE PTR [rsp+0x4e],al
    126c:	84 c0                	test   al,al
    126e:	0f 85 6b 06 00 00    	jne    18df <parser+0x120b>
			if(is_pushable_in_main(get_fbgc_object_type(TOP_LL(p.op)))){
    1274:	48 8b 54 24 3e       	mov    rdx,QWORD PTR [rsp+0x3e]
    1279:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
    127d:	8a 01                	mov    al,BYTE PTR [rcx]
    127f:	89 c6                	mov    esi,eax
    1281:	83 e0 7f             	and    eax,0x7f
    1284:	83 e8 23             	sub    eax,0x23
    1287:	83 e6 7f             	and    esi,0x7f
    128a:	83 f8 03             	cmp    eax,0x3
    128d:	0f 86 80 00 00 00    	jbe    1313 <parser+0xc3f>
    1293:	40 80 fe 4f          	cmp    sil,0x4f
    1297:	74 7a                	je     1313 <parser+0xc3f>
				p.iter_prev->next = TOP_LL(p.op);
    1299:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
			_info_green("Checking the operator stack, inside the while loop\n");
    129e:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 12a5 <parser+0xbd1>	12a1: R_X86_64_PC32	.LC54-0x4
    12a5:	bf 08 00 00 00       	mov    edi,0x8
				p.iter_prev->next = TOP_LL(p.op);
    12aa:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
				POP_LL(p.op);
    12ae:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
    12b2:	48 8b 49 01          	mov    rcx,QWORD PTR [rcx+0x1]
    12b6:	48 89 4a 01          	mov    QWORD PTR [rdx+0x1],rcx
				p.iter_prev->next->next = p.iter->next; 
    12ba:	48 8b 4c 24 2e       	mov    rcx,QWORD PTR [rsp+0x2e]
    12bf:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
    12c3:	48 8b 49 01          	mov    rcx,QWORD PTR [rcx+0x1]
    12c7:	48 89 4a 01          	mov    QWORD PTR [rdx+0x1],rcx
				p.iter_prev = p.iter_prev->next;
    12cb:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    12cf:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
			_info_green("Checking the operator stack, inside the while loop\n");
    12d4:	31 c0                	xor    eax,eax
    12d6:	e8 00 00 00 00       	call   12db <parser+0xc07>	12d7: R_X86_64_PLT32	cprintf-0x4
			_print("[GM]:{%s}\n",gm2str(p.gm));
    12db:	0f be 7c 24 4f       	movsx  edi,BYTE PTR [rsp+0x4f]
    12e0:	e8 00 00 00 00       	call   12e5 <parser+0xc11>	12e1: R_X86_64_PLT32	gm2str-0x4
    12e5:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 12ec <parser+0xc18>	12e8: R_X86_64_PC32	.LC55-0x4
    12ec:	bf 65 00 00 00       	mov    edi,0x65
    12f1:	48 89 c2             	mov    rdx,rax
    12f4:	31 c0                	xor    eax,eax
    12f6:	e8 00 00 00 00       	call   12fb <parser+0xc27>	12f7: R_X86_64_PLT32	cprintf-0x4
		while( is_not_empty_fbgc_ll(p.op) && compare_operators(get_fbgc_object_type(TOP_LL(p.op)),p.iter->type) ){
    12fb:	48 8b 54 24 3e       	mov    rdx,QWORD PTR [rsp+0x3e]
    1300:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
    1304:	48 39 42 09          	cmp    QWORD PTR [rdx+0x9],rax
    1308:	0f 85 37 ff ff ff    	jne    1245 <parser+0xb71>
    130e:	e9 91 00 00 00       	jmp    13a4 <parser+0xcd0>
				_warning("Top object is not pushable to main list\n");
    1313:	31 c0                	xor    eax,eax
    1315:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 131c <parser+0xc48>	1318: R_X86_64_PC32	.LC56-0x4
    131c:	bf 6e 00 00 00       	mov    edi,0x6e
    1321:	e8 00 00 00 00       	call   1326 <parser+0xc52>	1322: R_X86_64_PLT32	cprintf-0x4
				if(p.iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(p.op)) == LBRACK){
    1326:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
    132b:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    1330:	8a 0a                	mov    cl,BYTE PTR [rdx]
    1332:	80 f9 2b             	cmp    cl,0x2b
    1335:	75 10                	jne    1347 <parser+0xc73>
    1337:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
    133b:	8a 12                	mov    dl,BYTE PTR [rdx]
    133d:	83 e2 7f             	and    edx,0x7f
    1340:	80 fa 25             	cmp    dl,0x25
    1343:	75 45                	jne    138a <parser+0xcb6>
    1345:	eb 5d                	jmp    13a4 <parser+0xcd0>
				else if(p.iter->type == PIPE && get_fbgc_object_type(TOP_LL(p.op)) == LEN){
    1347:	80 f9 3d             	cmp    cl,0x3d
    134a:	75 3e                	jne    138a <parser+0xcb6>
    134c:	48 8b 48 01          	mov    rcx,QWORD PTR [rax+0x1]
    1350:	8a 09                	mov    cl,BYTE PTR [rcx]
    1352:	83 e1 7f             	and    ecx,0x7f
    1355:	80 f9 4f             	cmp    cl,0x4f
    1358:	75 30                	jne    138a <parser+0xcb6>
					p.iter->next = p.iter_prev->next;
    135a:	48 8b 4c 24 36       	mov    rcx,QWORD PTR [rsp+0x36]
    135f:	48 8b 71 01          	mov    rsi,QWORD PTR [rcx+0x1]
    1363:	48 89 72 01          	mov    QWORD PTR [rdx+0x1],rsi
					p.iter_prev->next = TOP_LL(p.op);
    1367:	48 8b 70 01          	mov    rsi,QWORD PTR [rax+0x1]
    136b:	48 89 71 01          	mov    QWORD PTR [rcx+0x1],rsi
					POP_LL(p.op);
    136f:	48 8b 70 01          	mov    rsi,QWORD PTR [rax+0x1]
    1373:	48 8b 76 01          	mov    rsi,QWORD PTR [rsi+0x1]
    1377:	48 89 70 01          	mov    QWORD PTR [rax+0x1],rsi
					p.iter_prev = p.iter_prev->next;
    137b:	48 8b 41 01          	mov    rax,QWORD PTR [rcx+0x1]
					p.iter_prev->next = p.iter->next;
    137f:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
					p.iter_prev = p.iter_prev->next;
    1383:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
					p.iter_prev->next = p.iter->next;
    1388:	eb 16                	jmp    13a0 <parser+0xccc>
				else if(get_fbgc_object_type(TOP_LL(p.op)) == LPARA){
    138a:	48 8b 48 01          	mov    rcx,QWORD PTR [rax+0x1]
    138e:	8a 11                	mov    dl,BYTE PTR [rcx]
    1390:	83 e2 7f             	and    edx,0x7f
    1393:	80 fa 23             	cmp    dl,0x23
    1396:	0f 85 3f fe ff ff    	jne    11db <parser+0xb07>
					POP_LL(p.op);
    139c:	48 8b 51 01          	mov    rdx,QWORD PTR [rcx+0x1]
    13a0:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
		if( (p.error_code = gm_seek_left(&p)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }
    13a4:	48 89 ef             	mov    rdi,rbp
		fbgc_grammar gm_prev = p.gm;
    13a7:	8a 5c 24 4f          	mov    bl,BYTE PTR [rsp+0x4f]
		if( (p.error_code = gm_seek_left(&p)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }
    13ab:	e8 00 00 00 00       	call   13b0 <parser+0xcdc>	13ac: R_X86_64_PLT32	gm_seek_left-0x4
    13b0:	88 44 24 4e          	mov    BYTE PTR [rsp+0x4e],al
    13b4:	84 c0                	test   al,al
    13b6:	0f 85 23 05 00 00    	jne    18df <parser+0x120b>
		if(p.iter->type == LPARA){
    13bc:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
    13c1:	8a 06                	mov    al,BYTE PTR [rsi]
    13c3:	3c 23                	cmp    al,0x23
    13c5:	0f 85 9d 00 00 00    	jne    1468 <parser+0xd94>
			_warning("p.iter is LPARA, p.iter_prev %s\n",lltp2str(p.iter_prev));
    13cb:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
    13d0:	e8 00 00 00 00       	call   13d5 <parser+0xd01>	13d1: R_X86_64_PLT32	lltp2str-0x4
    13d5:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 13dc <parser+0xd08>	13d8: R_X86_64_PC32	.LC59-0x4
    13dc:	bf 6e 00 00 00       	mov    edi,0x6e
    13e1:	48 89 c2             	mov    rdx,rax
    13e4:	31 c0                	xor    eax,eax
    13e6:	e8 00 00 00 00       	call   13eb <parser+0xd17>	13e7: R_X86_64_PLT32	cprintf-0x4
			if((gm_prev == GM_ID || gm_prev == GM_MATRIX) && (p.iter_prev->type == IDENTIFIER || p.iter_prev->type == LOAD_SUBSCRIPT || is_constant_and_token(p.iter_prev,CFUN) )) {
    13eb:	80 fb 03             	cmp    bl,0x3
    13ee:	74 09                	je     13f9 <parser+0xd25>
    13f0:	80 fb 0f             	cmp    bl,0xf
    13f3:	0f 85 b9 00 00 00    	jne    14b2 <parser+0xdde>
    13f9:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
    13fe:	8a 02                	mov    al,BYTE PTR [rdx]
    1400:	3c 10                	cmp    al,0x10
    1402:	74 1b                	je     141f <parser+0xd4b>
    1404:	3c 57                	cmp    al,0x57
    1406:	74 17                	je     141f <parser+0xd4b>
    1408:	3c 02                	cmp    al,0x2
    140a:	0f 85 a2 00 00 00    	jne    14b2 <parser+0xdde>
    1410:	48 8b 42 09          	mov    rax,QWORD PTR [rdx+0x9]
    1414:	80 38 0c             	cmp    BYTE PTR [rax],0xc
    1417:	0f 85 95 00 00 00    	jne    14b2 <parser+0xdde>
    141d:	eb 0d                	jmp    142c <parser+0xd58>
				if(p.iter_prev->type == CONSTANT && _cast_llbase_as_llconstant(p.iter_prev)->content->type == CFUN){
    141f:	3c 02                	cmp    al,0x2
    1421:	75 1c                	jne    143f <parser+0xd6b>
    1423:	48 8b 42 09          	mov    rax,QWORD PTR [rdx+0x9]
    1427:	80 38 0c             	cmp    BYTE PTR [rax],0xc
    142a:	75 13                	jne    143f <parser+0xd6b>
					_warning("match!\n");
    142c:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1433 <parser+0xd5f>	142f: R_X86_64_PC32	.LC60-0x4
    1433:	bf 6e 00 00 00       	mov    edi,0x6e
    1438:	31 c0                	xor    eax,eax
    143a:	e8 00 00 00 00       	call   143f <parser+0xd6b>	143b: R_X86_64_PLT32	cprintf-0x4
				if(p.iter_prev->type == IDENTIFIER && is_id_flag_MEMBER(p.iter_prev)){
    143f:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
    1444:	80 38 10             	cmp    BYTE PTR [rax],0x10
    1447:	75 11                	jne    145a <parser+0xd86>
    1449:	f6 40 0d 04          	test   BYTE PTR [rax+0xd],0x4
    144d:	74 0b                	je     145a <parser+0xd86>
					set_id_flag_MEMBER_METHOD(p.iter_prev);
    144f:	c6 40 0d 0c          	mov    BYTE PTR [rax+0xd],0xc
					_push_front_fbgc_ll(p.op,_new_fbgc_ll_opcode_int(FUN_CALL,1));
    1453:	be 01 00 00 00       	mov    esi,0x1
    1458:	eb 02                	jmp    145c <parser+0xd88>
				else _push_front_fbgc_ll(p.op,_new_fbgc_ll_opcode_int(FUN_CALL,0));
    145a:	31 f6                	xor    esi,esi
    145c:	bf 52 00 00 00       	mov    edi,0x52
    1461:	e8 00 00 00 00       	call   1466 <parser+0xd92>	1462: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
    1466:	eb 3d                	jmp    14a5 <parser+0xdd1>
		else if(p.iter->type == LBRACK){
    1468:	3c 25                	cmp    al,0x25
    146a:	75 50                	jne    14bc <parser+0xde8>
			_warning("p.iter is LBRACK, p.iter_prev %s\n",lltp2str(p.iter_prev));
    146c:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
    1471:	e8 00 00 00 00       	call   1476 <parser+0xda2>	1472: R_X86_64_PLT32	lltp2str-0x4
    1476:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 147d <parser+0xda9>	1479: R_X86_64_PC32	.LC61-0x4
    147d:	bf 6e 00 00 00       	mov    edi,0x6e
    1482:	48 89 c2             	mov    rdx,rax
    1485:	31 c0                	xor    eax,eax
    1487:	e8 00 00 00 00       	call   148c <parser+0xdb8>	1488: R_X86_64_PLT32	cprintf-0x4
			if(p.iter_prev->type == IDENTIFIER || p.iter_prev->type == FUN_CALL){
    148c:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
    1491:	8a 00                	mov    al,BYTE PTR [rax]
    1493:	3c 10                	cmp    al,0x10
    1495:	74 04                	je     149b <parser+0xdc7>
    1497:	3c 52                	cmp    al,0x52
    1499:	75 17                	jne    14b2 <parser+0xdde>
				_push_front_fbgc_ll(p.op,_new_fbgc_ll_base(LOAD_SUBSCRIPT));
    149b:	bf 57 00 00 00       	mov    edi,0x57
    14a0:	e8 00 00 00 00       	call   14a5 <parser+0xdd1>	14a1: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
    14a5:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
    14aa:	48 89 c6             	mov    rsi,rax
    14ad:	e8 00 00 00 00       	call   14b2 <parser+0xdde>	14ae: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
			_push_front_fbgc_ll(p.op,p.iter);
    14b2:	48 8b 74 24 2e       	mov    rsi,QWORD PTR [rsp+0x2e]
    14b7:	e9 bf 01 00 00       	jmp    167b <parser+0xfa7>
		else if(p.iter->type == ROW ){
    14bc:	3c 56                	cmp    al,0x56
    14be:	0f 84 f2 02 00 00    	je     17b6 <parser+0x10e2>
		else if(p.iter->type == COMMA){
    14c4:	3c 29                	cmp    al,0x29
    14c6:	75 4f                	jne    1517 <parser+0xe43>
			if(TOP_LL(p.op)->type == COMMA){
    14c8:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    14cd:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
    14d1:	8a 02                	mov    al,BYTE PTR [rdx]
    14d3:	3c 29                	cmp    al,0x29
    14d5:	75 08                	jne    14df <parser+0xe0b>
				_cast_llbase_as_llopcode_int(TOP_LL(p.op))->content++;
    14d7:	ff 42 09             	inc    DWORD PTR [rdx+0x9]
    14da:	e9 d7 02 00 00       	jmp    17b6 <parser+0x10e2>
				if(get_fbgc_object_type(TOP_LL(p.op)) != LPARA && get_fbgc_object_type(TOP_LL(p.op)) != LBRACK){
    14df:	83 e0 7f             	and    eax,0x7f
    14e2:	83 e8 23             	sub    eax,0x23
    14e5:	a8 fd                	test   al,0xfd
    14e7:	74 17                	je     1500 <parser+0xe2c>
					_push_front_fbgc_ll(p.op,_new_fbgc_ll_base(LPARA));
    14e9:	bf 23 00 00 00       	mov    edi,0x23
    14ee:	e8 00 00 00 00       	call   14f3 <parser+0xe1f>	14ef: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
    14f3:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
    14f8:	48 89 c6             	mov    rsi,rax
    14fb:	e8 00 00 00 00       	call   1500 <parser+0xe2c>	14fc: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
				_push_front_fbgc_ll(p.op, _new_fbgc_ll_opcode_int(COMMA,2));
    1500:	be 02 00 00 00       	mov    esi,0x2
    1505:	bf 29 00 00 00       	mov    edi,0x29
    150a:	e8 00 00 00 00       	call   150f <parser+0xe3b>	150b: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
    150f:	48 89 c6             	mov    rsi,rax
    1512:	e9 64 01 00 00       	jmp    167b <parser+0xfa7>
		else if( p.iter->type == RBRACK){
    1517:	3c 26                	cmp    al,0x26
    1519:	75 3c                	jne    1557 <parser+0xe83>
			if(TOP_LL(p.op)->type == LBRACK){
    151b:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
    1520:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
    1524:	80 38 25             	cmp    BYTE PTR [rax],0x25
    1527:	75 1d                	jne    1546 <parser+0xe72>
				_pop_front_fbgc_ll(p.op);
    1529:	e8 00 00 00 00       	call   152e <parser+0xe5a>	152a: R_X86_64_PLT32	_pop_front_fbgc_ll-0x4
			handle_before_brackets(&p);
    152e:	48 89 ef             	mov    rdi,rbp
    1531:	e8 00 00 00 00       	call   1536 <parser+0xe62>	1532: R_X86_64_PLT32	handle_before_brackets-0x4
			if( p.error_code != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }
    1536:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
    153b:	0f 84 75 02 00 00    	je     17b6 <parser+0x10e2>
    1541:	e9 99 03 00 00       	jmp    18df <parser+0x120b>
			else assert(0);
    1546:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 154d <parser+0xe79>	1549: R_X86_64_PC32	.rodata+0xb4
    154d:	ba 49 04 00 00       	mov    edx,0x449
    1552:	e9 db fc ff ff       	jmp    1232 <parser+0xb5e>
		else if(p.iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(p.op)) == LBRACK){
    1557:	3c 2b                	cmp    al,0x2b
    1559:	75 52                	jne    15ad <parser+0xed9>
    155b:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    1560:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    1564:	8a 00                	mov    al,BYTE PTR [rax]
    1566:	83 e0 7f             	and    eax,0x7f
    1569:	3c 25                	cmp    al,0x25
    156b:	75 4b                	jne    15b8 <parser+0xee4>
			if(p.iter_prev->type == COMMA) p.iter_prev->type = ROW; 
    156d:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
    1572:	8a 02                	mov    al,BYTE PTR [rdx]
    1574:	3c 29                	cmp    al,0x29
    1576:	75 08                	jne    1580 <parser+0xeac>
    1578:	c6 02 56             	mov    BYTE PTR [rdx],0x56
    157b:	e9 36 02 00 00       	jmp    17b6 <parser+0x10e2>
			else if(p.iter_prev->type != ROW){
    1580:	3c 56                	cmp    al,0x56
    1582:	0f 84 2e 02 00 00    	je     17b6 <parser+0x10e2>
				struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(ROW,1);
    1588:	be 01 00 00 00       	mov    esi,0x1
    158d:	bf 56 00 00 00       	mov    edi,0x56
    1592:	e8 00 00 00 00       	call   1597 <parser+0xec3>	1593: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
				ito->next = p.iter_prev->next;
    1597:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
    159c:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
    15a0:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
				p.iter_prev->next = ito;
    15a4:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
				p.iter_prev = ito;
    15a8:	e9 1f f4 ff ff       	jmp    9cc <parser+0x2f8>
		else if(p.iter->type == RPARA || p.iter->type == NEWLINE || p.iter->type == SEMICOLON){
    15ad:	83 e8 22             	sub    eax,0x22
    15b0:	a8 fd                	test   al,0xfd
    15b2:	0f 85 c3 00 00 00    	jne    167b <parser+0xfa7>
			_warning("%s out of while loop\n",lltp2str(p.iter));
    15b8:	48 89 f7             	mov    rdi,rsi
    15bb:	e8 00 00 00 00       	call   15c0 <parser+0xeec>	15bc: R_X86_64_PLT32	lltp2str-0x4
    15c0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 15c7 <parser+0xef3>	15c3: R_X86_64_PC32	.LC62-0x4
    15c7:	bf 6e 00 00 00       	mov    edi,0x6e
    15cc:	48 89 c2             	mov    rdx,rax
    15cf:	31 c0                	xor    eax,eax
    15d1:	e8 00 00 00 00       	call   15d6 <parser+0xf02>	15d2: R_X86_64_PLT32	cprintf-0x4
			_warning("Grammar top %s\n",gm2str(p.gm));
    15d6:	0f be 7c 24 4f       	movsx  edi,BYTE PTR [rsp+0x4f]
    15db:	e8 00 00 00 00       	call   15e0 <parser+0xf0c>	15dc: R_X86_64_PLT32	gm2str-0x4
    15e0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 15e7 <parser+0xf13>	15e3: R_X86_64_PC32	.LC63-0x4
    15e7:	bf 6e 00 00 00       	mov    edi,0x6e
    15ec:	48 89 c2             	mov    rdx,rax
    15ef:	31 c0                	xor    eax,eax
    15f1:	e8 00 00 00 00       	call   15f6 <parser+0xf22>	15f2: R_X86_64_PLT32	cprintf-0x4
			if(p.iter->type == RPARA && TOP_LL(p.op)->type == LPARA) _pop_front_fbgc_ll(p.op);
    15f6:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
    15fb:	8a 00                	mov    al,BYTE PTR [rax]
    15fd:	3c 24                	cmp    al,0x24
    15ff:	74 04                	je     1605 <parser+0xf31>
			else if(p.iter->type == NEWLINE && TOP_LL(p.op)->type == LPARA) _pop_front_fbgc_ll(p.op);
    1601:	3c 22                	cmp    al,0x22
    1603:	75 13                	jne    1618 <parser+0xf44>
    1605:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
    160a:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
    160e:	80 38 23             	cmp    BYTE PTR [rax],0x23
    1611:	75 05                	jne    1618 <parser+0xf44>
    1613:	e8 00 00 00 00       	call   1618 <parser+0xf44>	1614: R_X86_64_PLT32	_pop_front_fbgc_ll-0x4
			handle_before_paranthesis(&p);
    1618:	48 89 ef             	mov    rdi,rbp
    161b:	e8 00 00 00 00       	call   1620 <parser+0xf4c>	161c: R_X86_64_PLT32	handle_before_paranthesis-0x4
			if( p.error_code != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }
    1620:	80 7c 24 4e 00       	cmp    BYTE PTR [rsp+0x4e],0x0
    1625:	0f 85 b4 02 00 00    	jne    18df <parser+0x120b>
			if(p.iter->type == NEWLINE || p.iter->type == SEMICOLON) p.gm = GM_NEWLINE;
    162b:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
    1630:	8a 02                	mov    al,BYTE PTR [rdx]
    1632:	3c 22                	cmp    al,0x22
    1634:	0f 94 c1             	sete   cl
    1637:	3c 2b                	cmp    al,0x2b
    1639:	74 04                	je     163f <parser+0xf6b>
    163b:	84 c9                	test   cl,cl
    163d:	74 05                	je     1644 <parser+0xf70>
    163f:	c6 44 24 4f 01       	mov    BYTE PTR [rsp+0x4f],0x1
			if(p.iter_prev->type == BUILD_TUPLE){
    1644:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
				if(p.iter->type == NEWLINE && !is_empty_fbgc_ll(p.op)){
    1649:	80 38 53             	cmp    BYTE PTR [rax],0x53
    164c:	0f 85 64 01 00 00    	jne    17b6 <parser+0x10e2>
    1652:	84 c9                	test   cl,cl
    1654:	0f 84 5c 01 00 00    	je     17b6 <parser+0x10e2>
    165a:	48 8b 4c 24 3e       	mov    rcx,QWORD PTR [rsp+0x3e]
    165f:	48 8b 59 01          	mov    rbx,QWORD PTR [rcx+0x1]
    1663:	48 39 59 09          	cmp    QWORD PTR [rcx+0x9],rbx
    1667:	0f 84 49 01 00 00    	je     17b6 <parser+0x10e2>
					p.iter_prev->next = p.iter;
    166d:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
					p.gm = GM_EXPRESSION;
    1671:	c6 44 24 4f 08       	mov    BYTE PTR [rsp+0x4f],0x8
    1676:	e9 3b 01 00 00       	jmp    17b6 <parser+0x10e2>
			_push_front_fbgc_ll(p.op,p.iter);
    167b:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
    1680:	e8 00 00 00 00       	call   1685 <parser+0xfb1>	1681: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
    1685:	e9 2c 01 00 00       	jmp    17b6 <parser+0x10e2>
		if( (p.error_code = gm_seek_left(&p)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }
    168a:	48 89 ef             	mov    rdi,rbp
    168d:	e8 00 00 00 00       	call   1692 <parser+0xfbe>	168e: R_X86_64_PLT32	gm_seek_left-0x4
    1692:	88 44 24 4e          	mov    BYTE PTR [rsp+0x4e],al
    1696:	84 c0                	test   al,al
    1698:	0f 85 41 02 00 00    	jne    18df <parser+0x120b>
		if(TOP_LL(p.op)->type != IDENTIFIER && TOP_LL(p.op)->type != LOAD_SUBSCRIPT){
    169e:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    16a3:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    16a7:	8a 10                	mov    dl,BYTE PTR [rax]
    16a9:	80 fa 10             	cmp    dl,0x10
    16ac:	74 22                	je     16d0 <parser+0xffc>
    16ae:	80 fa 57             	cmp    dl,0x57
    16b1:	74 1d                	je     16d0 <parser+0xffc>
			cprintf(100,"Assignment operator requires identifier before itself\n");
    16b3:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 16ba <parser+0xfe6>	16b6: R_X86_64_PC32	.LC64-0x4
    16ba:	bf 64 00 00 00       	mov    edi,0x64
    16bf:	31 c0                	xor    eax,eax
			p.error_code = _FBGC_SYNTAX_ERROR;
    16c1:	c6 44 24 4e 02       	mov    BYTE PTR [rsp+0x4e],0x2
			cprintf(100,"Assignment operator requires identifier before itself\n");
    16c6:	e8 00 00 00 00       	call   16cb <parser+0xff7>	16c7: R_X86_64_PLT32	cprintf-0x4
			goto PARSER_ERROR_LABEL;
    16cb:	e9 0f 02 00 00       	jmp    18df <parser+0x120b>
		if(TOP_LL(p.op)->next->type == ASSIGN || TOP_LL(p.op)->next->type == LPARA || TOP_LL(p.op)->next->type == COMMA){
    16d0:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    16d4:	8a 00                	mov    al,BYTE PTR [rax]
    16d6:	8d 50 dd             	lea    edx,[rax-0x23]
    16d9:	80 e2 df             	and    dl,0xdf
    16dc:	74 04                	je     16e2 <parser+0x100e>
    16de:	3c 29                	cmp    al,0x29
    16e0:	75 32                	jne    1714 <parser+0x1040>
			_info_green("Opening push itself flag\n");
    16e2:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 16e9 <parser+0x1015>	16e5: R_X86_64_PC32	.LC65-0x4
    16e9:	bf 08 00 00 00       	mov    edi,0x8
    16ee:	31 c0                	xor    eax,eax
    16f0:	e8 00 00 00 00       	call   16f5 <parser+0x1021>	16f1: R_X86_64_PLT32	cprintf-0x4
			if(TOP_LL(p.op)->next->next == NULL || TOP_LL(p.op)->next->next->type != FOR)
    16f5:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    16fa:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    16fe:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
    1702:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
    1706:	48 85 d2             	test   rdx,rdx
    1709:	74 05                	je     1710 <parser+0x103c>
    170b:	80 3a 1d             	cmp    BYTE PTR [rdx],0x1d
    170e:	74 04                	je     1714 <parser+0x1040>
				set_id_flag_PUSH_ITSELF(TOP_LL(p.op));
    1710:	80 48 0d 10          	or     BYTE PTR [rax+0xd],0x10
		p.iter_prev->next = p.iter->next;
    1714:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
    1719:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
    171d:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
    1722:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
		if(TOP_LL(p.op)->type == IDENTIFIER){
    1726:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    172b:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    172f:	8a 10                	mov    dl,BYTE PTR [rax]
    1731:	80 fa 10             	cmp    dl,0x10
    1734:	75 2e                	jne    1764 <parser+0x1090>
			_info("Stack top is [%x]ID<%d>\n",get_ll_identifier_flag(TOP_LL(p.op)),get_ll_identifier_loc(TOP_LL(p.op)));
    1736:	0f b6 50 0d          	movzx  edx,BYTE PTR [rax+0xd]
    173a:	8b 48 09             	mov    ecx,DWORD PTR [rax+0x9]
    173d:	bf 6f 00 00 00       	mov    edi,0x6f
    1742:	31 c0                	xor    eax,eax
    1744:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 174b <parser+0x1077>	1747: R_X86_64_PC32	.LC66-0x4
    174b:	e8 00 00 00 00       	call   1750 <parser+0x107c>	174c: R_X86_64_PLT32	cprintf-0x4
			TOP_LL(p.op)->type = p.iter->type;				
    1750:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
    1755:	8a 10                	mov    dl,BYTE PTR [rax]
    1757:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    175c:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    1760:	88 10                	mov    BYTE PTR [rax],dl
    1762:	eb 52                	jmp    17b6 <parser+0x10e2>
		else if(TOP_LL(p.op)->type == LOAD_SUBSCRIPT){
    1764:	80 fa 57             	cmp    dl,0x57
    1767:	75 4d                	jne    17b6 <parser+0x10e2>
			_info("Stack top is load subscript, converting to assign\n");
    1769:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1770 <parser+0x109c>	176c: R_X86_64_PC32	.LC67-0x4
    1770:	bf 6f 00 00 00       	mov    edi,0x6f
    1775:	31 c0                	xor    eax,eax
    1777:	e8 00 00 00 00       	call   177c <parser+0x10a8>	1778: R_X86_64_PLT32	cprintf-0x4
			TOP_LL(p.op)->type = ASSIGN_SUBSCRIPT;
    177c:	48 8b 44 24 3e       	mov    rax,QWORD PTR [rsp+0x3e]
    1781:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    1785:	c6 00 4e             	mov    BYTE PTR [rax],0x4e
    1788:	eb 2c                	jmp    17b6 <parser+0x10e2>
		_warning("Undefined object type[%s] in parser\n",lltp2str(p.iter));
    178a:	e8 00 00 00 00       	call   178f <parser+0x10bb>	178b: R_X86_64_PLT32	lltp2str-0x4
    178f:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1796 <parser+0x10c2>	1792: R_X86_64_PC32	.LC68-0x4
    1796:	bf 6e 00 00 00       	mov    edi,0x6e
    179b:	48 89 c2             	mov    rdx,rax
    179e:	31 c0                	xor    eax,eax
    17a0:	e8 00 00 00 00       	call   17a5 <parser+0x10d1>	17a1: R_X86_64_PLT32	cprintf-0x4
		assert(0);
    17a5:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 17ac <parser+0x10d8>	17a8: R_X86_64_PC32	.rodata+0xb4
    17ac:	ba a9 04 00 00       	mov    edx,0x4a9
    17b1:	e9 7c fa ff ff       	jmp    1232 <parser+0xb5e>
	p.iter = p.iter_prev->next;
    17b6:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
	_print_fbgc_ll(p.head,"M");
    17bb:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
    17c0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 17c7 <parser+0x10f3>	17c3: R_X86_64_PC32	.LC69-0x4
	p.iter = p.iter_prev->next;
    17c7:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
    17cb:	48 89 44 24 2e       	mov    QWORD PTR [rsp+0x2e],rax
	_print_fbgc_ll(p.head,"M");
    17d0:	e8 00 00 00 00       	call   17d5 <parser+0x1101>	17d1: R_X86_64_PLT32	_print_fbgc_ll-0x4
	_print_fbgc_ll(p.op,"O");
    17d5:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
    17da:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 17e1 <parser+0x110d>	17dd: R_X86_64_PC32	.LC70-0x4
    17e1:	e8 00 00 00 00       	call   17e6 <parser+0x1112>	17e2: R_X86_64_PLT32	_print_fbgc_ll-0x4
	cprintf(101,"[GM]:{%s}\n\n",gm2str(p.gm));
    17e6:	0f be 7c 24 4f       	movsx  edi,BYTE PTR [rsp+0x4f]
    17eb:	e8 00 00 00 00       	call   17f0 <parser+0x111c>	17ec: R_X86_64_PLT32	gm2str-0x4
    17f0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 17f7 <parser+0x1123>	17f3: R_X86_64_PC32	.LC71-0x4
    17f7:	bf 65 00 00 00       	mov    edi,0x65
    17fc:	48 89 c2             	mov    rdx,rax
    17ff:	31 c0                	xor    eax,eax
    1801:	e8 00 00 00 00       	call   1806 <parser+0x1132>	1802: R_X86_64_PLT32	cprintf-0x4
	for(int i = 0; fbgc_error(p.error_code,line_no) ; i++){
    1806:	41 ff c6             	inc    r14d
    1809:	e9 72 ef ff ff       	jmp    780 <parser+0xac>
		//Check the stack and give proper error
		if(is_fbgc_FUNCTIONABLE(get_fbgc_object_type(TOP_LL(p.op)))){
    180e:	8a 00                	mov    al,BYTE PTR [rax]
    1810:	89 c2                	mov    edx,eax
    1812:	83 e0 7f             	and    eax,0x7f
    1815:	83 e2 7f             	and    edx,0x7f
    1818:	8d 4a e8             	lea    ecx,[rdx-0x18]
    181b:	80 e1 f7             	and    cl,0xf7
    181e:	74 08                	je     1828 <parser+0x1154>
    1820:	83 ea 1a             	sub    edx,0x1a
    1823:	80 fa 03             	cmp    dl,0x3
    1826:	77 19                	ja     1841 <parser+0x116d>
			cprintf(100,"Missing <END> : <%s>\n",object_type_as_str(TOP_LL(p.op)));
    1828:	48 98                	cdqe   
    182a:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 1831 <parser+0x115d>	182d: R_X86_64_PC32	object_name_array-0x4
    1831:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1838 <parser+0x1164>	1834: R_X86_64_PC32	.LC72-0x4
    1838:	48 8b 14 c2          	mov    rdx,QWORD PTR [rdx+rax*8]
    183c:	e9 b7 00 00 00       	jmp    18f8 <parser+0x1224>
			return 0;
		}

		cprintf(100,"Syntax error in line %d, Non-empty operator stack!\n",line_no);
    1841:	44 89 ea             	mov    edx,r13d
    1844:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 184b <parser+0x1177>	1847: R_X86_64_PC32	.LC73-0x4
    184b:	bf 64 00 00 00       	mov    edi,0x64
    1850:	31 c0                	xor    eax,eax
    1852:	e8 00 00 00 00       	call   1857 <parser+0x1183>	1853: R_X86_64_PLT32	cprintf-0x4
		_print_fbgc_ll(p.op,"O");
    1857:	48 8b 7c 24 3e       	mov    rdi,QWORD PTR [rsp+0x3e]
    185c:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1863 <parser+0x118f>	185f: R_X86_64_PC32	.LC70-0x4
    1863:	e8 00 00 00 00       	call   1868 <parser+0x1194>	1864: R_X86_64_PLT32	_print_fbgc_ll-0x4
		return 0;
    1868:	e9 97 00 00 00       	jmp    1904 <parser+0x1230>
    186d:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1874 <parser+0x11a0>	1870: R_X86_64_PC32	.LC74-0x4
    1874:	bf 01 00 00 00       	mov    edi,0x1
    1879:	31 c0                	xor    eax,eax
    187b:	e8 00 00 00 00       	call   1880 <parser+0x11ac>	187c: R_X86_64_PLT32	__printf_chk-0x4
	}

	FBGC_LOGV(PARSER,"==========================\nParser finished job.");
	_println_object("Defined symbols in the parser:",fbgc_symbols);
    1880:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1887 <parser+0x11b3>	1883: R_X86_64_PC32	.LC75-0x4
    1887:	bf 01 00 00 00       	mov    edi,0x1
    188c:	31 c0                	xor    eax,eax
    188e:	e8 00 00 00 00       	call   1893 <parser+0x11bf>	188f: R_X86_64_PLT32	cprintf-0x4
    1893:	48 8b 3d 00 00 00 00 	mov    rdi,QWORD PTR [rip+0x0]        # 189a <parser+0x11c6>	1896: R_X86_64_PC32	general_symbols-0x4
    189a:	e8 00 00 00 00       	call   189f <parser+0x11cb>	189b: R_X86_64_PLT32	print_fbgc_object-0x4
    189f:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 18a6 <parser+0x11d2>	18a2: R_X86_64_PC32	.LC37-0x4
    18a6:	bf 01 00 00 00       	mov    edi,0x1
    18ab:	31 c0                	xor    eax,eax
    18ad:	e8 00 00 00 00       	call   18b2 <parser+0x11de>	18ae: R_X86_64_PLT32	cprintf-0x4
	_print("Global field locals:"); _print_field_locals(*field_obj);
    18b2:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 18b9 <parser+0x11e5>	18b5: R_X86_64_PC32	.LC76-0x4
    18b9:	bf 65 00 00 00       	mov    edi,0x65
    18be:	31 c0                	xor    eax,eax
    18c0:	e8 00 00 00 00       	call   18c5 <parser+0x11f1>	18c1: R_X86_64_PLT32	cprintf-0x4
    18c5:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 18cc <parser+0x11f8>	18c8: R_X86_64_PC32	.LC77-0x4
    18cc:	bf 01 00 00 00       	mov    edi,0x1
    18d1:	31 c0                	xor    eax,eax
    18d3:	e8 00 00 00 00       	call   18d8 <parser+0x1204>	18d4: R_X86_64_PLT32	__printf_chk-0x4
	FBGC_LOGV(PARSER,"==========================");

	return 1;
    18d8:	b0 01                	mov    al,0x1
    18da:	e9 95 00 00 00       	jmp    1974 <parser+0x12a0>


	PARSER_ERROR_LABEL:
	fbgc_error(p.error_code,line_no);
    18df:	0f b6 7c 24 4e       	movzx  edi,BYTE PTR [rsp+0x4e]
    18e4:	44 89 ee             	mov    esi,r13d
    18e7:	e8 00 00 00 00       	call   18ec <parser+0x1218>	18e8: R_X86_64_PLT32	fbgc_error-0x4
		cprintf(100,"%s\n",line);
    18ec:	48 8d 54 24 50       	lea    rdx,[rsp+0x50]
    18f1:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 18f8 <parser+0x1224>	18f4: R_X86_64_PC32	.LC78-0x4
    18f8:	bf 64 00 00 00       	mov    edi,0x64
    18fd:	31 c0                	xor    eax,eax
    18ff:	e8 00 00 00 00       	call   1904 <parser+0x1230>	1900: R_X86_64_PLT32	cprintf-0x4
	return 0;
    1904:	31 c0                	xor    eax,eax
    1906:	eb 6c                	jmp    1974 <parser+0x12a0>
				_info("Creating a variable inside a field obj\n");
    1908:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 190f <parser+0x123b>	190b: R_X86_64_PC32	.LC80-0x4
    190f:	bf 6f 00 00 00       	mov    edi,0x6f
    1914:	31 c0                	xor    eax,eax
    1916:	4c 89 44 24 08       	mov    QWORD PTR [rsp+0x8],r8
    191b:	e8 00 00 00 00       	call   1920 <parser+0x124c>	191c: R_X86_64_PLT32	cprintf-0x4
				local_array = push_back_fbgc_array_object(local_array,&id);
    1920:	4c 8b 44 24 08       	mov    r8,QWORD PTR [rsp+0x8]
    1925:	48 8d 74 24 16       	lea    rsi,[rsp+0x16]
				id.name = cstr_obj; id.content = NULL;
    192a:	48 89 6c 24 16       	mov    QWORD PTR [rsp+0x16],rbp
    192f:	48 c7 44 24 1e 00 00 00 00 	mov    QWORD PTR [rsp+0x1e],0x0
				local_array = push_back_fbgc_array_object(local_array,&id);
    1938:	4c 89 c7             	mov    rdi,r8
    193b:	e8 00 00 00 00       	call   1940 <parser+0x126c>	193c: R_X86_64_PLT32	push_back_fbgc_array_object-0x4
				if(current_scope->type == FIELD) cast_fbgc_object_as_field(current_scope)->locals = local_array;
    1940:	48 8b 54 24 46       	mov    rdx,QWORD PTR [rsp+0x46]
				where = size_fbgc_array_object(local_array)-1;
    1945:	8b 58 11             	mov    ebx,DWORD PTR [rax+0x11]
				if(current_scope->type == FIELD) cast_fbgc_object_as_field(current_scope)->locals = local_array;
    1948:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
				where = size_fbgc_array_object(local_array)-1;
    194c:	ff cb                	dec    ebx
				if(current_scope->type == FIELD) cast_fbgc_object_as_field(current_scope)->locals = local_array;
    194e:	48 8b 52 09          	mov    rdx,QWORD PTR [rdx+0x9]
    1952:	80 3a 16             	cmp    BYTE PTR [rdx],0x16
    1955:	75 06                	jne    195d <parser+0x1289>
    1957:	48 89 42 11          	mov    QWORD PTR [rdx+0x11],rax
    195b:	eb 04                	jmp    1961 <parser+0x128d>
				else cast_fbgc_object_as_class(current_scope)->locals = local_array;
    195d:	48 89 42 09          	mov    QWORD PTR [rdx+0x9],rax
				_info("Symbol is created @ [%d]\n",where);
    1961:	89 da                	mov    edx,ebx
    1963:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 196a <parser+0x1296>	1966: R_X86_64_PC32	.LC33-0x4
    196a:	bf 6f 00 00 00       	mov    edi,0x6f
    196f:	e9 c3 f0 ff ff       	jmp    a37 <parser+0x363>

    1974:	48 8b 9c 24 38 04 00 00 	mov    rbx,QWORD PTR [rsp+0x438]
    197c:	64 48 33 1c 25 28 00 00 00 	xor    rbx,QWORD PTR fs:0x28
    1985:	74 05                	je     198c <parser+0x12b8>
    1987:	e8 00 00 00 00       	call   198c <parser+0x12b8>	1988: R_X86_64_PLT32	__stack_chk_fail-0x4
    198c:	48 81 c4 48 04 00 00 	add    rsp,0x448
    1993:	5b                   	pop    rbx
    1994:	5d                   	pop    rbp
    1995:	41 5c                	pop    r12
    1997:	41 5d                	pop    r13
    1999:	41 5e                	pop    r14
    199b:	41 5f                	pop    r15
    199d:	c3                   	ret    
