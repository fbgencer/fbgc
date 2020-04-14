
parser.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <compare_operators>:
       0:	f3 0f 1e fa          	endbr64 
       4:	b0 01                	mov    al,0x1
       6:	40 80 ff 10          	cmp    dil,0x10
       a:	74 59                	je     65 <compare_operators+0x65>
       c:	8d 46 dd             	lea    eax,[rsi-0x23]
       f:	24 fd                	and    al,0xfd
      11:	74 52                	je     65 <compare_operators+0x65>
      13:	8d 56 e0             	lea    edx,[rsi-0x20]
      16:	31 c0                	xor    eax,eax
      18:	80 fa 2f             	cmp    dl,0x2f
      1b:	77 48                	ja     65 <compare_operators+0x65>
      1d:	8d 57 e0             	lea    edx,[rdi-0x20]
      20:	80 fa 2f             	cmp    dl,0x2f
      23:	77 40                	ja     65 <compare_operators+0x65>
      25:	40 0f b6 ff          	movzx  edi,dil
      29:	40 0f b6 f6          	movzx  esi,sil
      2d:	48 8d 05 00 00 00 00 	lea    rax,[rip+0x0]        # 34 <compare_operators+0x34>	30: R_X86_64_PC32	precedence_table-0x4
      34:	83 ef 20             	sub    edi,0x20
      37:	83 ee 20             	sub    esi,0x20
      3a:	48 63 ff             	movsxd rdi,edi
      3d:	48 63 f6             	movsxd rsi,esi
      40:	40 8a 3c 38          	mov    dil,BYTE PTR [rax+rdi*1]
      44:	8a 04 30             	mov    al,BYTE PTR [rax+rsi*1]
      47:	89 f9                	mov    ecx,edi
      49:	89 c2                	mov    edx,eax
      4b:	83 e1 7f             	and    ecx,0x7f
      4e:	83 e2 7f             	and    edx,0x7f
      51:	38 d1                	cmp    cl,dl
      53:	77 05                	ja     5a <compare_operators+0x5a>
      55:	40 38 c7             	cmp    dil,al
      58:	74 06                	je     60 <compare_operators+0x60>
      5a:	38 d1                	cmp    cl,dl
      5c:	0f 97 c0             	seta   al
      5f:	c3                   	ret    
      60:	f7 d0                	not    eax
      62:	c0 e8 07             	shr    al,0x7
      65:	c3                   	ret    

0000000000000066 <handle_function_args>:
      66:	f3 0f 1e fa          	endbr64 
      6a:	48 8b 4f 09          	mov    rcx,QWORD PTR [rdi+0x9]
      6e:	48 89 f8             	mov    rax,rdi
      71:	48 8b 51 01          	mov    rdx,QWORD PTR [rcx+0x1]
      75:	48 8b 52 09          	mov    rdx,QWORD PTR [rdx+0x9]
      79:	48 8b 52 09          	mov    rdx,QWORD PTR [rdx+0x9]
      7d:	88 51 09             	mov    BYTE PTR [rcx+0x9],dl
      80:	48 39 f7             	cmp    rdi,rsi
      83:	0f 84 ce 00 00 00    	je     157 <handle_function_args+0xf1>
      89:	48 8b 7f 01          	mov    rdi,QWORD PTR [rdi+0x1]
      8d:	44 8a 07             	mov    r8b,BYTE PTR [rdi]
      90:	41 80 f8 10          	cmp    r8b,0x10
      94:	74 ea                	je     80 <handle_function_args+0x1a>
      96:	41 80 f8 29          	cmp    r8b,0x29
      9a:	74 e4                	je     80 <handle_function_args+0x1a>
      9c:	41 51                	push   r9
      9e:	41 80 f8 52          	cmp    r8b,0x52
      a2:	75 5b                	jne    ff <handle_function_args+0x99>
      a4:	48 39 77 01          	cmp    QWORD PTR [rdi+0x1],rsi
      a8:	74 46                	je     f0 <handle_function_args+0x8a>
      aa:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # b1 <handle_function_args+0x4b>	ad: R_X86_64_PC32	.rodata+0x12c
      b1:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # b8 <handle_function_args+0x52>	b4: R_X86_64_PC32	.LC0-0x4
      b8:	bf 01 00 00 00       	mov    edi,0x1
      bd:	31 c0                	xor    eax,eax
      bf:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # c6 <handle_function_args+0x60>	c2: R_X86_64_PC32	.LC1-0x4
      c6:	41 b8 76 00 00 00    	mov    r8d,0x76
      cc:	e8 00 00 00 00       	call   d1 <handle_function_args+0x6b>	cd: R_X86_64_PLT32	__printf_chk-0x4
      d1:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # d8 <handle_function_args+0x72>	d4: R_X86_64_PC32	.rodata+0x10c
      d8:	ba 77 00 00 00       	mov    edx,0x77
      dd:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # e4 <handle_function_args+0x7e>	e0: R_X86_64_PC32	.LC0-0x4
      e4:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # eb <handle_function_args+0x85>	e7: R_X86_64_PC32	.LC2-0x4
      eb:	e8 00 00 00 00       	call   f0 <handle_function_args+0x8a>	ec: R_X86_64_PLT32	__assert_fail-0x4
      f0:	f7 da                	neg    edx
      f2:	88 51 09             	mov    BYTE PTR [rcx+0x9],dl
      f5:	80 3e 29             	cmp    BYTE PTR [rsi],0x29
      f8:	75 53                	jne    14d <handle_function_args+0xe7>
      fa:	48 89 f7             	mov    rdi,rsi
      fd:	eb 4e                	jmp    14d <handle_function_args+0xe7>
      ff:	e8 00 00 00 00       	call   104 <handle_function_args+0x9e>	100: R_X86_64_PLT32	lltp2str-0x4
     104:	bf 01 00 00 00       	mov    edi,0x1
     109:	41 b8 87 00 00 00    	mov    r8d,0x87
     10f:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 116 <handle_function_args+0xb0>	112: R_X86_64_PC32	.rodata+0x12c
     116:	49 89 c1             	mov    r9,rax
     119:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 120 <handle_function_args+0xba>	11c: R_X86_64_PC32	.LC0-0x4
     120:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 127 <handle_function_args+0xc1>	123: R_X86_64_PC32	.LC3-0x4
     127:	31 c0                	xor    eax,eax
     129:	e8 00 00 00 00       	call   12e <handle_function_args+0xc8>	12a: R_X86_64_PLT32	__printf_chk-0x4
     12e:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # 135 <handle_function_args+0xcf>	131: R_X86_64_PC32	.rodata+0x10c
     135:	ba 88 00 00 00       	mov    edx,0x88
     13a:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 141 <handle_function_args+0xdb>	13d: R_X86_64_PC32	.LC0-0x4
     141:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # 148 <handle_function_args+0xe2>	144: R_X86_64_PC32	.LC2-0x4
     148:	e8 00 00 00 00       	call   14d <handle_function_args+0xe7>	149: R_X86_64_PLT32	__assert_fail-0x4
     14d:	48 8b 57 01          	mov    rdx,QWORD PTR [rdi+0x1]
     151:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     155:	58                   	pop    rax
     156:	c3                   	ret    
     157:	48 8b 56 01          	mov    rdx,QWORD PTR [rsi+0x1]
     15b:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     15f:	c3                   	ret    

0000000000000160 <handle_before_paranthesis>:
     160:	f3 0f 1e fa          	endbr64 
     164:	41 54                	push   r12
     166:	55                   	push   rbp
     167:	53                   	push   rbx
     168:	48 89 fb             	mov    rbx,rdi
     16b:	48 8b 7f 18          	mov    rdi,QWORD PTR [rdi+0x18]
     16f:	48 8b 73 10          	mov    rsi,QWORD PTR [rbx+0x10]
     173:	48 8b 57 01          	mov    rdx,QWORD PTR [rdi+0x1]
     177:	8a 0e                	mov    cl,BYTE PTR [rsi]
     179:	8a 02                	mov    al,BYTE PTR [rdx]
     17b:	80 f9 29             	cmp    cl,0x29
     17e:	0f 84 2e 01 00 00    	je     2b2 <handle_before_paranthesis+0x152>
     184:	44 8a 43 29          	mov    r8b,BYTE PTR [rbx+0x29]
     188:	3c 20                	cmp    al,0x20
     18a:	77 27                	ja     1b3 <handle_before_paranthesis+0x53>
     18c:	3c 17                	cmp    al,0x17
     18e:	0f 86 fa 00 00 00    	jbe    28e <handle_before_paranthesis+0x12e>
     194:	83 e8 18             	sub    eax,0x18
     197:	3c 08                	cmp    al,0x8
     199:	0f 87 ef 00 00 00    	ja     28e <handle_before_paranthesis+0x12e>
     19f:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # 1a6 <handle_before_paranthesis+0x46>	1a2: R_X86_64_PC32	.rodata-0x4
     1a6:	0f b6 c0             	movzx  eax,al
     1a9:	48 63 04 87          	movsxd rax,DWORD PTR [rdi+rax*4]
     1ad:	48 01 f8             	add    rax,rdi
     1b0:	3e ff e0             	notrack jmp rax
     1b3:	3c 52                	cmp    al,0x52
     1b5:	0f 85 d3 00 00 00    	jne    28e <handle_before_paranthesis+0x12e>
     1bb:	31 c0                	xor    eax,eax
     1bd:	41 80 f8 02          	cmp    r8b,0x2
     1c1:	0f 95 c0             	setne  al
     1c4:	01 42 09             	add    DWORD PTR [rdx+0x9],eax
     1c7:	e8 00 00 00 00       	call   1cc <handle_before_paranthesis+0x6c>	1c8: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     1cc:	e9 cf 00 00 00       	jmp    2a0 <handle_before_paranthesis+0x140>
     1d1:	48 8b 42 09          	mov    rax,QWORD PTR [rdx+0x9]
     1d5:	80 38 51             	cmp    BYTE PTR [rax],0x51
     1d8:	0f 84 11 01 00 00    	je     2ef <handle_before_paranthesis+0x18f>
     1de:	80 f9 43             	cmp    cl,0x43
     1e1:	74 09                	je     1ec <handle_before_paranthesis+0x8c>
     1e3:	c6 43 28 02          	mov    BYTE PTR [rbx+0x28],0x2
     1e7:	e9 03 01 00 00       	jmp    2ef <handle_before_paranthesis+0x18f>
     1ec:	48 89 c5             	mov    rbp,rax
     1ef:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     1f3:	48 39 c6             	cmp    rsi,rax
     1f6:	75 f4                	jne    1ec <handle_before_paranthesis+0x8c>
     1f8:	bf 51 00 00 00       	mov    edi,0x51
     1fd:	e8 00 00 00 00       	call   202 <handle_before_paranthesis+0xa2>	1fe: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
     202:	48 89 ef             	mov    rdi,rbp
     205:	48 89 c6             	mov    rsi,rax
     208:	48 8b 43 18          	mov    rax,QWORD PTR [rbx+0x18]
     20c:	48 8b 58 01          	mov    rbx,QWORD PTR [rax+0x1]
     210:	e8 00 00 00 00       	call   215 <handle_before_paranthesis+0xb5>	211: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     215:	48 89 43 09          	mov    QWORD PTR [rbx+0x9],rax
     219:	e9 d1 00 00 00       	jmp    2ef <handle_before_paranthesis+0x18f>
     21e:	48 8b 6a 09          	mov    rbp,QWORD PTR [rdx+0x9]
     222:	48 8b 45 09          	mov    rax,QWORD PTR [rbp+0x9]
     226:	80 78 09 40          	cmp    BYTE PTR [rax+0x9],0x40
     22a:	0f 8e bf 00 00 00    	jle    2ef <handle_before_paranthesis+0x18f>
     230:	48 89 ef             	mov    rdi,rbp
     233:	e8 00 00 00 00       	call   238 <handle_before_paranthesis+0xd8>	234: R_X86_64_PLT32	handle_function_args-0x4
     238:	48 89 6b 10          	mov    QWORD PTR [rbx+0x10],rbp
     23c:	e9 ae 00 00 00       	jmp    2ef <handle_before_paranthesis+0x18f>
     241:	48 8b 43 20          	mov    rax,QWORD PTR [rbx+0x20]
     245:	48 8b 6a 09          	mov    rbp,QWORD PTR [rdx+0x9]
     249:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     24d:	4c 8b 65 09          	mov    r12,QWORD PTR [rbp+0x9]
     251:	4c 39 60 09          	cmp    QWORD PTR [rax+0x9],r12
     255:	0f 84 94 00 00 00    	je     2ef <handle_before_paranthesis+0x18f>
     25b:	4c 89 e7             	mov    rdi,r12
     25e:	e8 00 00 00 00       	call   263 <handle_before_paranthesis+0x103>	25f: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     263:	48 8b 7b 20          	mov    rdi,QWORD PTR [rbx+0x20]
     267:	48 89 c6             	mov    rsi,rax
     26a:	e8 00 00 00 00       	call   26f <handle_before_paranthesis+0x10f>	26b: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     26f:	48 8b 45 01          	mov    rax,QWORD PTR [rbp+0x1]
     273:	80 38 10             	cmp    BYTE PTR [rax],0x10
     276:	48 0f 44 e8          	cmove  rbp,rax
     27a:	49 89 6c 24 01       	mov    QWORD PTR [r12+0x1],rbp
     27f:	eb 6e                	jmp    2ef <handle_before_paranthesis+0x18f>
     281:	48 83 7a 09 00       	cmp    QWORD PTR [rdx+0x9],0x0
     286:	75 67                	jne    2ef <handle_before_paranthesis+0x18f>
     288:	48 89 72 09          	mov    QWORD PTR [rdx+0x9],rsi
     28c:	eb 61                	jmp    2ef <handle_before_paranthesis+0x18f>
     28e:	41 80 f8 02          	cmp    r8b,0x2
     292:	75 5b                	jne    2ef <handle_before_paranthesis+0x18f>
     294:	31 f6                	xor    esi,esi
     296:	bf 53 00 00 00       	mov    edi,0x53
     29b:	e8 00 00 00 00       	call   2a0 <handle_before_paranthesis+0x140>	29c: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     2a0:	48 8b 7b 10          	mov    rdi,QWORD PTR [rbx+0x10]
     2a4:	48 89 c6             	mov    rsi,rax
     2a7:	e8 00 00 00 00       	call   2ac <handle_before_paranthesis+0x14c>	2a8: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     2ac:	48 89 43 10          	mov    QWORD PTR [rbx+0x10],rax
     2b0:	eb 3d                	jmp    2ef <handle_before_paranthesis+0x18f>
     2b2:	3c 20                	cmp    al,0x20
     2b4:	77 24                	ja     2da <handle_before_paranthesis+0x17a>
     2b6:	3c 17                	cmp    al,0x17
     2b8:	77 05                	ja     2bf <handle_before_paranthesis+0x15f>
     2ba:	c6 06 53             	mov    BYTE PTR [rsi],0x53
     2bd:	eb 30                	jmp    2ef <handle_before_paranthesis+0x18f>
     2bf:	83 e8 18             	sub    eax,0x18
     2c2:	3c 08                	cmp    al,0x8
     2c4:	77 f4                	ja     2ba <handle_before_paranthesis+0x15a>
     2c6:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # 2cd <handle_before_paranthesis+0x16d>	2c9: R_X86_64_PC32	.rodata+0x20
     2cd:	0f b6 c0             	movzx  eax,al
     2d0:	48 63 04 87          	movsxd rax,DWORD PTR [rdi+rax*4]
     2d4:	48 01 f8             	add    rax,rdi
     2d7:	3e ff e0             	notrack jmp rax
     2da:	3c 52                	cmp    al,0x52
     2dc:	75 dc                	jne    2ba <handle_before_paranthesis+0x15a>
     2de:	c6 06 52             	mov    BYTE PTR [rsi],0x52
     2e1:	8b 42 09             	mov    eax,DWORD PTR [rdx+0x9]
     2e4:	01 46 09             	add    DWORD PTR [rsi+0x9],eax
     2e7:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     2eb:	48 89 47 01          	mov    QWORD PTR [rdi+0x1],rax
     2ef:	5b                   	pop    rbx
     2f0:	5d                   	pop    rbp
     2f1:	41 5c                	pop    r12
     2f3:	c3                   	ret    

00000000000002f4 <handle_before_brackets>:
     2f4:	f3 0f 1e fa          	endbr64 
     2f8:	53                   	push   rbx
     2f9:	48 8b 47 10          	mov    rax,QWORD PTR [rdi+0x10]
     2fd:	48 89 fb             	mov    rbx,rdi
     300:	40 b6 02             	mov    sil,0x2
     303:	80 38 29             	cmp    BYTE PTR [rax],0x29
     306:	74 08                	je     310 <handle_before_brackets+0x1c>
     308:	80 7f 29 02          	cmp    BYTE PTR [rdi+0x29],0x2
     30c:	40 0f 95 c6          	setne  sil
     310:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     314:	8a 12                	mov    dl,BYTE PTR [rdx]
     316:	83 ea 43             	sub    edx,0x43
     319:	80 fa 0a             	cmp    dl,0xa
     31c:	76 72                	jbe    390 <handle_before_brackets+0x9c>
     31e:	48 8b 7b 18          	mov    rdi,QWORD PTR [rbx+0x18]
     322:	48 8b 57 01          	mov    rdx,QWORD PTR [rdi+0x1]
     326:	8a 0a                	mov    cl,BYTE PTR [rdx]
     328:	80 f9 10             	cmp    cl,0x10
     32b:	74 05                	je     332 <handle_before_brackets+0x3e>
     32d:	80 f9 57             	cmp    cl,0x57
     330:	75 31                	jne    363 <handle_before_brackets+0x6f>
     332:	40 80 fe 02          	cmp    sil,0x2
     336:	75 0d                	jne    345 <handle_before_brackets+0x51>
     338:	c6 00 57             	mov    BYTE PTR [rax],0x57
     33b:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     33f:	48 89 47 01          	mov    QWORD PTR [rdi+0x1],rax
     343:	eb 4b                	jmp    390 <handle_before_brackets+0x9c>
     345:	40 fe ce             	dec    sil
     348:	75 13                	jne    35d <handle_before_brackets+0x69>
     34a:	e8 00 00 00 00       	call   34f <handle_before_brackets+0x5b>	34b: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     34f:	48 8b 7b 10          	mov    rdi,QWORD PTR [rbx+0x10]
     353:	48 89 c6             	mov    rsi,rax
     356:	e8 00 00 00 00       	call   35b <handle_before_brackets+0x67>	357: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     35b:	eb 2f                	jmp    38c <handle_before_brackets+0x98>
     35d:	c6 43 28 02          	mov    BYTE PTR [rbx+0x28],0x2
     361:	eb 2d                	jmp    390 <handle_before_brackets+0x9c>
     363:	40 80 fe 02          	cmp    sil,0x2
     367:	75 05                	jne    36e <handle_before_brackets+0x7a>
     369:	c6 00 54             	mov    BYTE PTR [rax],0x54
     36c:	eb 22                	jmp    390 <handle_before_brackets+0x9c>
     36e:	40 0f b6 f6          	movzx  esi,sil
     372:	bf 54 00 00 00       	mov    edi,0x54
     377:	e8 00 00 00 00       	call   37c <handle_before_brackets+0x88>	378: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     37c:	48 8b 53 10          	mov    rdx,QWORD PTR [rbx+0x10]
     380:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     384:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
     388:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
     38c:	48 89 43 10          	mov    QWORD PTR [rbx+0x10],rax
     390:	5b                   	pop    rbx
     391:	c3                   	ret    

0000000000000392 <parser>:
     392:	f3 0f 1e fa          	endbr64 
     396:	41 57                	push   r15
     398:	41 56                	push   r14
     39a:	4c 8d 35 00 00 00 00 	lea    r14,[rip+0x0]        # 3a1 <parser+0xf>	39d: R_X86_64_PC32	.rodata+0x44
     3a1:	41 55                	push   r13
     3a3:	49 89 f5             	mov    r13,rsi
     3a6:	41 54                	push   r12
     3a8:	45 31 e4             	xor    r12d,r12d
     3ab:	55                   	push   rbp
     3ac:	53                   	push   rbx
     3ad:	48 89 fb             	mov    rbx,rdi
     3b0:	48 81 ec 38 04 00 00 	sub    rsp,0x438
     3b7:	64 48 8b 04 25 28 00 00 00 	mov    rax,QWORD PTR fs:0x28
     3c0:	48 89 84 24 28 04 00 00 	mov    QWORD PTR [rsp+0x428],rax
     3c8:	31 c0                	xor    eax,eax
     3ca:	48 8b 07             	mov    rax,QWORD PTR [rdi]
     3cd:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     3d1:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     3d5:	48 89 44 24 16       	mov    QWORD PTR [rsp+0x16],rax
     3da:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     3df:	31 c0                	xor    eax,eax
     3e1:	48 89 54 24 1e       	mov    QWORD PTR [rsp+0x1e],rdx
     3e6:	e8 00 00 00 00       	call   3eb <parser+0x59>	3e7: R_X86_64_PLT32	_new_fbgc_ll-0x4
     3eb:	48 89 44 24 2e       	mov    QWORD PTR [rsp+0x2e],rax
     3f0:	31 c0                	xor    eax,eax
     3f2:	e8 00 00 00 00       	call   3f7 <parser+0x65>	3f3: R_X86_64_PLT32	_new_fbgc_ll-0x4
     3f7:	48 8b 3b             	mov    rdi,QWORD PTR [rbx]
     3fa:	66 c7 44 24 3e 00 01 	mov    WORD PTR [rsp+0x3e],0x100
     401:	48 89 44 24 36       	mov    QWORD PTR [rsp+0x36],rax
     406:	e8 00 00 00 00       	call   40b <parser+0x79>	407: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     40b:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     410:	48 89 c6             	mov    rsi,rax
     413:	e8 00 00 00 00       	call   418 <parser+0x86>	414: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     418:	48 8d 7c 24 50       	lea    rdi,[rsp+0x50]
     41d:	31 c0                	xor    eax,eax
     41f:	0f 57 c0             	xorps  xmm0,xmm0
     422:	b9 f6 00 00 00       	mov    ecx,0xf6
     427:	0f 11 44 24 40       	movups XMMWORD PTR [rsp+0x40],xmm0
     42c:	f3 ab                	rep stos DWORD PTR es:[rdi],eax
     42e:	0f b6 7c 24 3e       	movzx  edi,BYTE PTR [rsp+0x3e]
     433:	44 89 e6             	mov    esi,r12d
     436:	e8 00 00 00 00       	call   43b <parser+0xa9>	437: R_X86_64_PLT32	fbgc_error-0x4
     43b:	85 c0                	test   eax,eax
     43d:	74 2e                	je     46d <parser+0xdb>
     43f:	48 8b 44 24 16       	mov    rax,QWORD PTR [rsp+0x16]
     444:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     448:	48 39 44 24 1e       	cmp    QWORD PTR [rsp+0x1e],rax
     44d:	0f 85 90 00 00 00    	jne    4e3 <parser+0x151>
     453:	48 8d 6c 24 40       	lea    rbp,[rsp+0x40]
     458:	4c 89 ea             	mov    rdx,r13
     45b:	be e8 03 00 00       	mov    esi,0x3e8
     460:	48 89 ef             	mov    rdi,rbp
     463:	e8 00 00 00 00       	call   468 <parser+0xd6>	464: R_X86_64_PLT32	fbgc_getline_from_file-0x4
     468:	48 85 c0             	test   rax,rax
     46b:	75 2c                	jne    499 <parser+0x107>
     46d:	48 8b 44 24 16       	mov    rax,QWORD PTR [rsp+0x16]
     472:	48 8b 54 24 26       	mov    rdx,QWORD PTR [rsp+0x26]
     477:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     47b:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     47f:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
     484:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     488:	48 39 42 09          	cmp    QWORD PTR [rdx+0x9],rax
     48c:	0f 85 2b 0b 00 00    	jne    fbd <parser+0xc2b>
     492:	b0 01                	mov    al,0x1
     494:	e9 b0 0b 00 00       	jmp    1049 <parser+0xcb7>
     499:	8a 44 24 40          	mov    al,BYTE PTR [rsp+0x40]
     49d:	41 ff c4             	inc    r12d
     4a0:	3c 23                	cmp    al,0x23
     4a2:	77 14                	ja     4b8 <parser+0x126>
     4a4:	48 ba 01 04 00 00 08 00 00 00 	movabs rdx,0x800000401
     4ae:	48 0f a3 c2          	bt     rdx,rax
     4b2:	0f 82 76 ff ff ff    	jb     42e <parser+0x9c>
     4b8:	48 8b 44 24 16       	mov    rax,QWORD PTR [rsp+0x16]
     4bd:	48 8b 54 24 26       	mov    rdx,QWORD PTR [rsp+0x26]
     4c2:	48 89 ee             	mov    rsi,rbp
     4c5:	48 89 df             	mov    rdi,rbx
     4c8:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     4cc:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     4d0:	e8 00 00 00 00       	call   4d5 <parser+0x143>	4d1: R_X86_64_PLT32	regex_lexer-0x4
     4d5:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     4da:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     4de:	48 89 44 24 1e       	mov    QWORD PTR [rsp+0x1e],rax
     4e3:	48 8b 54 24 1e       	mov    rdx,QWORD PTR [rsp+0x1e]
     4e8:	48 8d 6c 24 16       	lea    rbp,[rsp+0x16]
     4ed:	8a 02                	mov    al,BYTE PTR [rdx]
     4ef:	3c 26                	cmp    al,0x26
     4f1:	77 20                	ja     513 <parser+0x181>
     4f3:	3c 01                	cmp    al,0x1
     4f5:	0f 86 9e 0a 00 00    	jbe    f99 <parser+0xc07>
     4fb:	83 e8 02             	sub    eax,0x2
     4fe:	3c 24                	cmp    al,0x24
     500:	0f 87 93 0a 00 00    	ja     f99 <parser+0xc07>
     506:	0f b6 c0             	movzx  eax,al
     509:	49 63 04 86          	movsxd rax,DWORD PTR [r14+rax*4]
     50d:	4c 01 f0             	add    rax,r14
     510:	3e ff e0             	notrack jmp rax
     513:	3c 42                	cmp    al,0x42
     515:	77 0d                	ja     524 <parser+0x192>
     517:	3c 28                	cmp    al,0x28
     519:	0f 86 7a 0a 00 00    	jbe    f99 <parser+0xc07>
     51f:	e9 f4 06 00 00       	jmp    c18 <parser+0x886>
     524:	83 e8 43             	sub    eax,0x43
     527:	3c 0a                	cmp    al,0xa
     529:	0f 86 e9 09 00 00    	jbe    f18 <parser+0xb86>
     52f:	e9 65 0a 00 00       	jmp    f99 <parser+0xc07>
     534:	48 89 ef             	mov    rdi,rbp
     537:	e8 00 00 00 00       	call   53c <parser+0x1aa>	538: R_X86_64_PLT32	gm_seek_left-0x4
     53c:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     541:	0f 85 db 0a 00 00    	jne    1022 <parser+0xc90>
     547:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     54c:	e9 93 00 00 00       	jmp    5e4 <parser+0x252>
     551:	48 89 ef             	mov    rdi,rbp
     554:	e8 00 00 00 00       	call   559 <parser+0x1c7>	555: R_X86_64_PLT32	gm_seek_left-0x4
     559:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     55e:	0f 85 be 0a 00 00    	jne    1022 <parser+0xc90>
     564:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     569:	48 8b 3d 00 00 00 00 	mov    rdi,QWORD PTR [rip+0x0]        # 570 <parser+0x1de>	56c: R_X86_64_PC32	general_symbols-0x4
     570:	8b 70 09             	mov    esi,DWORD PTR [rax+0x9]
     573:	e8 00 00 00 00       	call   578 <parser+0x1e6>	574: R_X86_64_PLT32	get_object_in_fbgc_tuple_object-0x4
     578:	49 89 c7             	mov    r15,rax
     57b:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     580:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     584:	80 3a 2a             	cmp    BYTE PTR [rdx],0x2a
     587:	75 30                	jne    5b9 <parser+0x227>
     589:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     58d:	4c 89 ff             	mov    rdi,r15
     590:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     594:	e8 00 00 00 00       	call   599 <parser+0x207>	595: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     599:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     59e:	48 89 c6             	mov    rsi,rax
     5a1:	e8 00 00 00 00       	call   5a6 <parser+0x214>	5a2: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     5a6:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     5ab:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     5b0:	c6 40 0d 04          	mov    BYTE PTR [rax+0xd],0x4
     5b4:	e9 ad 05 00 00       	jmp    b66 <parser+0x7d4>
     5b9:	48 8b 3b             	mov    rdi,QWORD PTR [rbx]
     5bc:	49 8d 77 01          	lea    rsi,[r15+0x1]
     5c0:	e8 00 00 00 00       	call   5c5 <parser+0x233>	5c1: R_X86_64_PLT32	new_cfun_object_from_str-0x4
     5c5:	48 89 c7             	mov    rdi,rax
     5c8:	48 85 c0             	test   rax,rax
     5cb:	74 21                	je     5ee <parser+0x25c>
     5cd:	e8 00 00 00 00       	call   5d2 <parser+0x240>	5ce: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     5d2:	48 8b 74 24 1e       	mov    rsi,QWORD PTR [rsp+0x1e]
     5d7:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     5dc:	48 89 c2             	mov    rdx,rax
     5df:	e8 00 00 00 00       	call   5e4 <parser+0x252>	5e0: R_X86_64_PLT32	_insert_fbgc_ll-0x4
     5e4:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     5e9:	e9 bc 09 00 00       	jmp    faa <parser+0xc18>
     5ee:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
     5f3:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     5f7:	48 8b 72 09          	mov    rsi,QWORD PTR [rdx+0x9]
     5fb:	48 8b 54 24 1e       	mov    rdx,QWORD PTR [rsp+0x1e]
     600:	8a 0e                	mov    cl,BYTE PTR [rsi]
     602:	80 f9 0d             	cmp    cl,0xd
     605:	75 66                	jne    66d <parser+0x2db>
     607:	48 8b 6e 01          	mov    rbp,QWORD PTR [rsi+0x1]
     60b:	c6 42 0d 02          	mov    BYTE PTR [rdx+0xd],0x2
     60f:	4c 89 fe             	mov    rsi,r15
     612:	48 8b 7d 09          	mov    rdi,QWORD PTR [rbp+0x9]
     616:	e8 00 00 00 00       	call   61b <parser+0x289>	617: R_X86_64_PLT32	index_fbgc_tuple_object-0x4
     61b:	89 c2                	mov    edx,eax
     61d:	66 ff c0             	inc    ax
     620:	0f 85 ef 00 00 00    	jne    715 <parser+0x383>
     626:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     62b:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     62f:	80 3a 43             	cmp    BYTE PTR [rdx],0x43
     632:	74 13                	je     647 <parser+0x2b5>
     634:	48 8b 54 24 36       	mov    rdx,QWORD PTR [rsp+0x36]
     639:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     63d:	48 8b 52 09          	mov    rdx,QWORD PTR [rdx+0x9]
     641:	80 7a 09 41          	cmp    BYTE PTR [rdx+0x9],0x41
     645:	75 19                	jne    660 <parser+0x2ce>
     647:	48 8d 7d 09          	lea    rdi,[rbp+0x9]
     64b:	4c 89 fe             	mov    rsi,r15
     64e:	e8 00 00 00 00       	call   653 <parser+0x2c1>	64f: R_X86_64_PLT32	_push_back_fbgc_tuple_object-0x4
     653:	48 8b 45 09          	mov    rax,QWORD PTR [rbp+0x9]
     657:	48 8b 50 09          	mov    rdx,QWORD PTR [rax+0x9]
     65b:	e9 b3 00 00 00       	jmp    713 <parser+0x381>
     660:	48 8b 0b             	mov    rcx,QWORD PTR [rbx]
     663:	c6 40 0d 01          	mov    BYTE PTR [rax+0xd],0x1
     667:	48 8d 69 11          	lea    rbp,[rcx+0x11]
     66b:	eb 27                	jmp    694 <parser+0x302>
     66d:	80 f9 16             	cmp    cl,0x16
     670:	75 12                	jne    684 <parser+0x2f2>
     672:	c6 42 0d 01          	mov    BYTE PTR [rdx+0xd],0x1
     676:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     67a:	48 8b 68 09          	mov    rbp,QWORD PTR [rax+0x9]
     67e:	48 83 c5 11          	add    rbp,0x11
     682:	eb 10                	jmp    694 <parser+0x302>
     684:	c6 42 0d 08          	mov    BYTE PTR [rdx+0xd],0x8
     688:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     68c:	48 8b 68 09          	mov    rbp,QWORD PTR [rax+0x9]
     690:	48 83 c5 09          	add    rbp,0x9
     694:	48 8b 74 24 36       	mov    rsi,QWORD PTR [rsp+0x36]
     699:	48 8b 4c 24 1e       	mov    rcx,QWORD PTR [rsp+0x1e]
     69e:	48 8b 55 00          	mov    rdx,QWORD PTR [rbp+0x0]
     6a2:	31 c0                	xor    eax,eax
     6a4:	48 8b 7a 11          	mov    rdi,QWORD PTR [rdx+0x11]
     6a8:	4c 8d 42 19          	lea    r8,[rdx+0x19]
     6ac:	48 39 c7             	cmp    rdi,rax
     6af:	74 1c                	je     6cd <parser+0x33b>
     6b1:	4c 8b 4a 01          	mov    r9,QWORD PTR [rdx+0x1]
     6b5:	4c 0f af c8          	imul   r9,rax
     6b9:	4f 39 3c 08          	cmp    QWORD PTR [r8+r9*1],r15
     6bd:	75 09                	jne    6c8 <parser+0x336>
     6bf:	89 c2                	mov    edx,eax
     6c1:	66 ff c0             	inc    ax
     6c4:	74 07                	je     6cd <parser+0x33b>
     6c6:	eb 4d                	jmp    715 <parser+0x383>
     6c8:	48 ff c0             	inc    rax
     6cb:	eb df                	jmp    6ac <parser+0x31a>
     6cd:	48 8b 46 01          	mov    rax,QWORD PTR [rsi+0x1]
     6d1:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     6d5:	80 38 0e             	cmp    BYTE PTR [rax],0xe
     6d8:	75 16                	jne    6f0 <parser+0x35e>
     6da:	48 8b 41 01          	mov    rax,QWORD PTR [rcx+0x1]
     6de:	80 38 43             	cmp    BYTE PTR [rax],0x43
     6e1:	74 0d                	je     6f0 <parser+0x35e>
     6e3:	48 8b 03             	mov    rax,QWORD PTR [rbx]
     6e6:	c6 41 0d 01          	mov    BYTE PTR [rcx+0xd],0x1
     6ea:	48 8d 68 11          	lea    rbp,[rax+0x11]
     6ee:	eb ae                	jmp    69e <parser+0x30c>
     6f0:	48 8d 74 24 06       	lea    rsi,[rsp+0x6]
     6f5:	48 89 ef             	mov    rdi,rbp
     6f8:	4c 89 7c 24 06       	mov    QWORD PTR [rsp+0x6],r15
     6fd:	48 c7 44 24 0e 00 00 00 00 	mov    QWORD PTR [rsp+0xe],0x0
     706:	e8 00 00 00 00       	call   70b <parser+0x379>	707: R_X86_64_PLT32	_push_back_fbgc_array_object-0x4
     70b:	48 8b 45 00          	mov    rax,QWORD PTR [rbp+0x0]
     70f:	48 8b 50 11          	mov    rdx,QWORD PTR [rax+0x11]
     713:	ff ca                	dec    edx
     715:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     71a:	0f bf d2             	movsx  edx,dx
     71d:	89 50 09             	mov    DWORD PTR [rax+0x9],edx
     720:	e9 41 04 00 00       	jmp    b66 <parser+0x7d4>
     725:	48 89 ef             	mov    rdi,rbp
     728:	e8 00 00 00 00       	call   72d <parser+0x39b>	729: R_X86_64_PLT32	gm_seek_left-0x4
     72d:	44 8a 7c 24 3e       	mov    r15b,BYTE PTR [rsp+0x3e]
     732:	45 84 ff             	test   r15b,r15b
     735:	0f 85 e7 08 00 00    	jne    1022 <parser+0xc90>
     73b:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     740:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     745:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     749:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     74e:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     752:	e8 00 00 00 00       	call   757 <parser+0x3c5>	753: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     757:	48 89 c5             	mov    rbp,rax
     75a:	8a 00                	mov    al,BYTE PTR [rax]
     75c:	3c 20                	cmp    al,0x20
     75e:	77 27                	ja     787 <parser+0x3f5>
     760:	3c 17                	cmp    al,0x17
     762:	0f 86 48 02 00 00    	jbe    9b0 <parser+0x61e>
     768:	83 e8 18             	sub    eax,0x18
     76b:	3c 08                	cmp    al,0x8
     76d:	0f 87 3d 02 00 00    	ja     9b0 <parser+0x61e>
     773:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # 77a <parser+0x3e8>	776: R_X86_64_PC32	.rodata+0xd8
     77a:	0f b6 c0             	movzx  eax,al
     77d:	48 63 04 82          	movsxd rax,DWORD PTR [rdx+rax*4]
     781:	48 01 d0             	add    rax,rdx
     784:	3e ff e0             	notrack jmp rax
     787:	3c 50                	cmp    al,0x50
     789:	0f 84 39 02 00 00    	je     9c8 <parser+0x636>
     78f:	e9 1c 02 00 00       	jmp    9b0 <parser+0x61e>
     794:	48 8b 7d 09          	mov    rdi,QWORD PTR [rbp+0x9]
     798:	48 3b 7c 24 26       	cmp    rdi,QWORD PTR [rsp+0x26]
     79d:	0f 84 6b 04 00 00    	je     c0e <parser+0x87c>
     7a3:	48 89 ee             	mov    rsi,rbp
     7a6:	e8 00 00 00 00       	call   7ab <parser+0x419>	7a7: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     7ab:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     7b0:	48 89 45 09          	mov    QWORD PTR [rbp+0x9],rax
     7b4:	e9 0f 02 00 00       	jmp    9c8 <parser+0x636>
     7b9:	48 8b 7d 09          	mov    rdi,QWORD PTR [rbp+0x9]
     7bd:	48 3b 7c 24 26       	cmp    rdi,QWORD PTR [rsp+0x26]
     7c2:	0f 84 46 04 00 00    	je     c0e <parser+0x87c>
     7c8:	48 89 ee             	mov    rsi,rbp
     7cb:	e8 00 00 00 00       	call   7d0 <parser+0x43e>	7cc: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     7d0:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     7d5:	e8 00 00 00 00       	call   7da <parser+0x448>	7d6: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     7da:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     7df:	48 89 c6             	mov    rsi,rax
     7e2:	49 89 c7             	mov    r15,rax
     7e5:	e8 00 00 00 00       	call   7ea <parser+0x458>	7e6: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     7ea:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     7ef:	eb 42                	jmp    833 <parser+0x4a1>
     7f1:	48 8b 7d 09          	mov    rdi,QWORD PTR [rbp+0x9]
     7f5:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     7fa:	48 39 47 01          	cmp    QWORD PTR [rdi+0x1],rax
     7fe:	0f 84 0a 04 00 00    	je     c0e <parser+0x87c>
     804:	48 89 ee             	mov    rsi,rbp
     807:	e8 00 00 00 00       	call   80c <parser+0x47a>	808: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     80c:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     811:	e8 00 00 00 00       	call   816 <parser+0x484>	812: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     816:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     81b:	48 89 c6             	mov    rsi,rax
     81e:	49 89 c7             	mov    r15,rax
     821:	e8 00 00 00 00       	call   826 <parser+0x494>	822: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     826:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     82b:	48 8b 45 09          	mov    rax,QWORD PTR [rbp+0x9]
     82f:	49 89 47 09          	mov    QWORD PTR [r15+0x9],rax
     833:	4c 89 7d 09          	mov    QWORD PTR [rbp+0x9],r15
     837:	e9 8c 01 00 00       	jmp    9c8 <parser+0x636>
     83c:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     841:	80 38 02             	cmp    BYTE PTR [rax],0x2
     844:	75 1a                	jne    860 <parser+0x4ce>
     846:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     84a:	80 38 0d             	cmp    BYTE PTR [rax],0xd
     84d:	75 11                	jne    860 <parser+0x4ce>
     84f:	c6 44 24 3e 02       	mov    BYTE PTR [rsp+0x3e],0x2
     854:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 85b <parser+0x4c9>	857: R_X86_64_PC32	.LC4-0x4
     85b:	e9 a3 00 00 00       	jmp    903 <parser+0x571>
     860:	48 8b 6d 09          	mov    rbp,QWORD PTR [rbp+0x9]
     864:	48 8b 3d 00 00 00 00 	mov    rdi,QWORD PTR [rip+0x0]        # 86b <parser+0x4d9>	867: R_X86_64_PC32	__fbgc_nil-0x4
     86b:	48 8b 45 09          	mov    rax,QWORD PTR [rbp+0x9]
     86f:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     873:	48 8b 52 09          	mov    rdx,QWORD PTR [rdx+0x9]
     877:	48 8b 52 09          	mov    rdx,QWORD PTR [rdx+0x9]
     87b:	88 50 0a             	mov    BYTE PTR [rax+0xa],dl
     87e:	48 8b 55 01          	mov    rdx,QWORD PTR [rbp+0x1]
     882:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     886:	e8 00 00 00 00       	call   88b <parser+0x4f9>	887: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     88b:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     890:	48 89 c6             	mov    rsi,rax
     893:	e8 00 00 00 00       	call   898 <parser+0x506>	894: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     898:	bf 21 00 00 00       	mov    edi,0x21
     89d:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     8a2:	e8 00 00 00 00       	call   8a7 <parser+0x515>	8a3: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
     8a7:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     8ac:	48 89 c6             	mov    rsi,rax
     8af:	e8 00 00 00 00       	call   8b4 <parser+0x522>	8b0: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     8b4:	48 c7 40 01 00 00 00 00 	mov    QWORD PTR [rax+0x1],0x0
     8bc:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
     8c1:	48 89 6c 24 26       	mov    QWORD PTR [rsp+0x26],rbp
     8c6:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     8ca:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     8ce:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     8d2:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     8d7:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     8db:	48 89 45 01          	mov    QWORD PTR [rbp+0x1],rax
     8df:	e9 e4 00 00 00       	jmp    9c8 <parser+0x636>
     8e4:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     8e9:	80 38 02             	cmp    BYTE PTR [rax],0x2
     8ec:	75 26                	jne    914 <parser+0x582>
     8ee:	48 8b 40 09          	mov    rax,QWORD PTR [rax+0x9]
     8f2:	80 38 0e             	cmp    BYTE PTR [rax],0xe
     8f5:	75 1d                	jne    914 <parser+0x582>
     8f7:	c6 44 24 3e 02       	mov    BYTE PTR [rsp+0x3e],0x2
     8fc:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 903 <parser+0x571>	8ff: R_X86_64_PC32	.LC5-0x4
     903:	bf 01 00 00 00       	mov    edi,0x1
     908:	31 c0                	xor    eax,eax
     90a:	e8 00 00 00 00       	call   90f <parser+0x57d>	90b: R_X86_64_PLT32	__printf_chk-0x4
     90f:	e9 0e 07 00 00       	jmp    1022 <parser+0xc90>
     914:	48 8b 45 09          	mov    rax,QWORD PTR [rbp+0x9]
     918:	48 8b 78 09          	mov    rdi,QWORD PTR [rax+0x9]
     91c:	48 8b 6f 01          	mov    rbp,QWORD PTR [rdi+0x1]
     920:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     924:	48 39 c5             	cmp    rbp,rax
     927:	74 08                	je     931 <parser+0x59f>
     929:	41 ff c7             	inc    r15d
     92c:	48 89 d0             	mov    rax,rdx
     92f:	eb ef                	jmp    920 <parser+0x58e>
     931:	48 89 57 01          	mov    QWORD PTR [rdi+0x1],rdx
     935:	e8 00 00 00 00       	call   93a <parser+0x5a8>	936: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     93a:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     93f:	48 89 c6             	mov    rsi,rax
     942:	e8 00 00 00 00       	call   947 <parser+0x5b5>	943: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     947:	bf 21 00 00 00       	mov    edi,0x21
     94c:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     951:	e8 00 00 00 00       	call   956 <parser+0x5c4>	952: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
     956:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     95b:	48 89 c6             	mov    rsi,rax
     95e:	e8 00 00 00 00       	call   963 <parser+0x5d1>	95f: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     963:	41 0f b6 f7          	movzx  esi,r15b
     967:	bf 58 00 00 00       	mov    edi,0x58
     96c:	48 c7 40 01 00 00 00 00 	mov    QWORD PTR [rax+0x1],0x0
     974:	48 8b 44 24 36       	mov    rax,QWORD PTR [rsp+0x36]
     979:	48 8b 50 01          	mov    rdx,QWORD PTR [rax+0x1]
     97d:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     981:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     985:	48 89 6c 24 26       	mov    QWORD PTR [rsp+0x26],rbp
     98a:	e8 00 00 00 00       	call   98f <parser+0x5fd>	98b: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     98f:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     994:	48 89 c6             	mov    rsi,rax
     997:	e8 00 00 00 00       	call   99c <parser+0x60a>	998: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     99c:	48 8b 54 24 1e       	mov    rdx,QWORD PTR [rsp+0x1e]
     9a1:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     9a6:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     9aa:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     9ae:	eb 18                	jmp    9c8 <parser+0x636>
     9b0:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 9b7 <parser+0x625>	9b3: R_X86_64_PC32	.LC6-0x4
     9b7:	bf 01 00 00 00       	mov    edi,0x1
     9bc:	31 c0                	xor    eax,eax
     9be:	e8 00 00 00 00       	call   9c3 <parser+0x631>	9bf: R_X86_64_PLT32	__printf_chk-0x4
     9c3:	e9 46 02 00 00       	jmp    c0e <parser+0x87c>
     9c8:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     9cd:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
     9d1:	80 38 50             	cmp    BYTE PTR [rax],0x50
     9d4:	0f 85 d0 05 00 00    	jne    faa <parser+0xc18>
     9da:	e8 00 00 00 00       	call   9df <parser+0x64d>	9db: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     9df:	48 8b 50 09          	mov    rdx,QWORD PTR [rax+0x9]
     9e3:	48 8b 4a 01          	mov    rcx,QWORD PTR [rdx+0x1]
     9e7:	48 89 48 01          	mov    QWORD PTR [rax+0x1],rcx
     9eb:	48 89 42 01          	mov    QWORD PTR [rdx+0x1],rax
     9ef:	48 8b 54 24 26       	mov    rdx,QWORD PTR [rsp+0x26]
     9f4:	48 89 50 09          	mov    QWORD PTR [rax+0x9],rdx
     9f8:	eb ce                	jmp    9c8 <parser+0x636>
     9fa:	48 89 ef             	mov    rdi,rbp
     9fd:	e8 00 00 00 00       	call   a02 <parser+0x670>	9fe: R_X86_64_PLT32	gm_seek_left-0x4
     a02:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     a07:	0f 85 15 06 00 00    	jne    1022 <parser+0xc90>
     a0d:	31 c0                	xor    eax,eax
     a0f:	e8 00 00 00 00       	call   a14 <parser+0x682>	a10: R_X86_64_PLT32	new_fbgc_fun_object-0x4
     a14:	48 89 c7             	mov    rdi,rax
     a17:	48 89 c5             	mov    rbp,rax
     a1a:	e8 00 00 00 00       	call   a1f <parser+0x68d>	a1b: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     a1f:	48 8b 74 24 1e       	mov    rsi,QWORD PTR [rsp+0x1e]
     a24:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     a29:	48 89 c2             	mov    rdx,rax
     a2c:	49 89 c7             	mov    r15,rax
     a2f:	e8 00 00 00 00       	call   a34 <parser+0x6a2>	a30: R_X86_64_PLT32	_insert_fbgc_ll-0x4
     a34:	48 8b 74 24 1e       	mov    rsi,QWORD PTR [rsp+0x1e]
     a39:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     a3e:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     a43:	4c 89 7e 09          	mov    QWORD PTR [rsi+0x9],r15
     a47:	e8 00 00 00 00       	call   a4c <parser+0x6ba>	a48: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     a4c:	48 89 ef             	mov    rdi,rbp
     a4f:	e8 00 00 00 00       	call   a54 <parser+0x6c2>	a50: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     a54:	48 8b 7c 24 36       	mov    rdi,QWORD PTR [rsp+0x36]
     a59:	48 89 c6             	mov    rsi,rax
     a5c:	e8 00 00 00 00       	call   a61 <parser+0x6cf>	a5d: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     a61:	31 ff                	xor    edi,edi
     a63:	e8 00 00 00 00       	call   a68 <parser+0x6d6>	a64: R_X86_64_PLT32	new_fbgc_tuple_object-0x4
     a68:	48 89 c7             	mov    rdi,rax
     a6b:	e8 00 00 00 00       	call   a70 <parser+0x6de>	a6c: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     a70:	48 89 45 01          	mov    QWORD PTR [rbp+0x1],rax
     a74:	e9 31 05 00 00       	jmp    faa <parser+0xc18>
     a79:	48 89 ef             	mov    rdi,rbp
     a7c:	e8 00 00 00 00       	call   a81 <parser+0x6ef>	a7d: R_X86_64_PLT32	gm_seek_left-0x4
     a81:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     a86:	0f 85 96 05 00 00    	jne    1022 <parser+0xc90>
     a8c:	31 c0                	xor    eax,eax
     a8e:	e8 00 00 00 00       	call   a93 <parser+0x701>	a8f: R_X86_64_PLT32	new_fbgc_class_object-0x4
     a93:	48 89 c7             	mov    rdi,rax
     a96:	e8 00 00 00 00       	call   a9b <parser+0x709>	a97: R_X86_64_PLT32	_new_fbgc_ll_constant-0x4
     a9b:	48 8b 74 24 1e       	mov    rsi,QWORD PTR [rsp+0x1e]
     aa0:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     aa5:	48 89 c2             	mov    rdx,rax
     aa8:	48 89 c5             	mov    rbp,rax
     aab:	e8 00 00 00 00       	call   ab0 <parser+0x71e>	aac: R_X86_64_PLT32	_insert_fbgc_ll-0x4
     ab0:	48 8b 74 24 1e       	mov    rsi,QWORD PTR [rsp+0x1e]
     ab5:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     aba:	48 89 6e 09          	mov    QWORD PTR [rsi+0x9],rbp
     abe:	e9 46 04 00 00       	jmp    f09 <parser+0xb77>
     ac3:	48 89 ef             	mov    rdi,rbp
     ac6:	e8 00 00 00 00       	call   acb <parser+0x739>	ac7: R_X86_64_PLT32	gm_seek_left-0x4
     acb:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     ad0:	0f 85 4c 05 00 00    	jne    1022 <parser+0xc90>
     ad6:	e9 33 01 00 00       	jmp    c0e <parser+0x87c>
     adb:	48 89 ef             	mov    rdi,rbp
     ade:	e8 00 00 00 00       	call   ae3 <parser+0x751>	adf: R_X86_64_PLT32	gm_seek_left-0x4
     ae3:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     ae8:	0f 85 34 05 00 00    	jne    1022 <parser+0xc90>
     aee:	bf 50 00 00 00       	mov    edi,0x50
     af3:	e8 00 00 00 00       	call   af8 <parser+0x766>	af4: R_X86_64_PLT32	_new_fbgc_ll_jumper-0x4
     af8:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     afd:	48 89 c6             	mov    rsi,rax
     b00:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
     b04:	80 38 1c             	cmp    BYTE PTR [rax],0x1c
     b07:	74 0b                	je     b14 <parser+0x782>
     b09:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     b0e:	48 89 46 09          	mov    QWORD PTR [rsi+0x9],rax
     b12:	eb 4d                	jmp    b61 <parser+0x7cf>
     b14:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # b1b <parser+0x789>	b17: R_X86_64_PC32	.rodata+0xfc
     b1b:	ba 62 03 00 00       	mov    edx,0x362
     b20:	e9 a4 03 00 00       	jmp    ec9 <parser+0xb37>
     b25:	48 89 ef             	mov    rdi,rbp
     b28:	e8 00 00 00 00       	call   b2d <parser+0x79b>	b29: R_X86_64_PLT32	gm_seek_left-0x4
     b2d:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     b32:	0f 85 ea 04 00 00    	jne    1022 <parser+0xc90>
     b38:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     b3d:	48 8b 54 24 26       	mov    rdx,QWORD PTR [rsp+0x26]
     b42:	bf 50 00 00 00       	mov    edi,0x50
     b47:	48 89 50 09          	mov    QWORD PTR [rax+0x9],rdx
     b4b:	e8 00 00 00 00       	call   b50 <parser+0x7be>	b4c: R_X86_64_PLT32	_new_fbgc_ll_jumper-0x4
     b50:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     b55:	48 89 c6             	mov    rsi,rax
     b58:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     b5d:	48 89 46 09          	mov    QWORD PTR [rsi+0x9],rax
     b61:	e8 00 00 00 00       	call   b66 <parser+0x7d4>	b62: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     b66:	48 8b 74 24 1e       	mov    rsi,QWORD PTR [rsp+0x1e]
     b6b:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     b70:	48 8b 56 01          	mov    rdx,QWORD PTR [rsi+0x1]
     b74:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     b78:	e9 8c 03 00 00       	jmp    f09 <parser+0xb77>
     b7d:	48 89 ef             	mov    rdi,rbp
     b80:	e8 00 00 00 00       	call   b85 <parser+0x7f3>	b81: R_X86_64_PLT32	gm_seek_left-0x4
     b85:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     b8a:	0f 85 92 04 00 00    	jne    1022 <parser+0xc90>
     b90:	48 8b 4c 24 2e       	mov    rcx,QWORD PTR [rsp+0x2e]
     b95:	48 8b 41 01          	mov    rax,QWORD PTR [rcx+0x1]
     b99:	40 8a 30             	mov    sil,BYTE PTR [rax]
     b9c:	8d 56 e4             	lea    edx,[rsi-0x1c]
     b9f:	80 fa 01             	cmp    dl,0x1
     ba2:	77 40                	ja     be4 <parser+0x852>
     ba4:	48 8b 54 24 1e       	mov    rdx,QWORD PTR [rsp+0x1e]
     ba9:	80 3a 1f             	cmp    BYTE PTR [rdx],0x1f
     bac:	75 28                	jne    bd6 <parser+0x844>
     bae:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     bb2:	80 38 50             	cmp    BYTE PTR [rax],0x50
     bb5:	74 1f                	je     bd6 <parser+0x844>
     bb7:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # bbe <parser+0x82c>	bba: R_X86_64_PC32	.rodata+0xfc
     bbe:	ba 8f 03 00 00       	mov    edx,0x38f
     bc3:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # bca <parser+0x838>	bc6: R_X86_64_PC32	.LC0-0x4
     bca:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # bd1 <parser+0x83f>	bcd: R_X86_64_PC32	.LC7-0x4
     bd1:	e8 00 00 00 00       	call   bd6 <parser+0x844>	bd2: R_X86_64_PLT32	__assert_fail-0x4
     bd6:	48 89 42 09          	mov    QWORD PTR [rdx+0x9],rax
     bda:	48 89 54 24 26       	mov    QWORD PTR [rsp+0x26],rdx
     bdf:	e9 c6 03 00 00       	jmp    faa <parser+0xc18>
     be4:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     be8:	48 39 41 09          	cmp    QWORD PTR [rcx+0x9],rax
     bec:	75 ab                	jne    b99 <parser+0x807>
     bee:	48 8b 7c 24 1e       	mov    rdi,QWORD PTR [rsp+0x1e]
     bf3:	e8 00 00 00 00       	call   bf8 <parser+0x866>	bf4: R_X86_64_PLT32	lltp2str-0x4
     bf8:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # bff <parser+0x86d>	bfb: R_X86_64_PC32	.LC8-0x4
     bff:	bf 01 00 00 00       	mov    edi,0x1
     c04:	48 89 c2             	mov    rdx,rax
     c07:	31 c0                	xor    eax,eax
     c09:	e8 00 00 00 00       	call   c0e <parser+0x87c>	c0a: R_X86_64_PLT32	__printf_chk-0x4
     c0e:	c6 44 24 3e 02       	mov    BYTE PTR [rsp+0x3e],0x2
     c13:	e9 0a 04 00 00       	jmp    1022 <parser+0xc90>
     c18:	48 8b 52 01          	mov    rdx,QWORD PTR [rdx+0x1]
     c1c:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     c21:	48 89 ef             	mov    rdi,rbp
     c24:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     c28:	e8 00 00 00 00       	call   c2d <parser+0x89b>	c29: R_X86_64_PLT32	gm_convert_left-0x4
     c2d:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
     c32:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     c36:	48 39 42 09          	cmp    QWORD PTR [rdx+0x9],rax
     c3a:	75 19                	jne    c55 <parser+0x8c3>
     c3c:	48 89 ef             	mov    rdi,rbp
     c3f:	44 8a 7c 24 3f       	mov    r15b,BYTE PTR [rsp+0x3f]
     c44:	e8 00 00 00 00       	call   c49 <parser+0x8b7>	c45: R_X86_64_PLT32	gm_seek_left-0x4
     c49:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     c4e:	74 6b                	je     cbb <parser+0x929>
     c50:	e9 cd 03 00 00       	jmp    1022 <parser+0xc90>
     c55:	48 8b 54 24 1e       	mov    rdx,QWORD PTR [rsp+0x1e]
     c5a:	40 8a 38             	mov    dil,BYTE PTR [rax]
     c5d:	0f b6 32             	movzx  esi,BYTE PTR [rdx]
     c60:	83 e7 7f             	and    edi,0x7f
     c63:	e8 00 00 00 00       	call   c68 <parser+0x8d6>	c64: R_X86_64_PLT32	compare_operators-0x4
     c68:	84 c0                	test   al,al
     c6a:	74 d0                	je     c3c <parser+0x8aa>
     c6c:	48 89 ef             	mov    rdi,rbp
     c6f:	e8 00 00 00 00       	call   c74 <parser+0x8e2>	c70: R_X86_64_PLT32	gm_seek_right-0x4
     c74:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     c79:	0f 85 a3 03 00 00    	jne    1022 <parser+0xc90>
     c7f:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     c84:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
     c88:	8a 00                	mov    al,BYTE PTR [rax]
     c8a:	89 c2                	mov    edx,eax
     c8c:	83 e0 7f             	and    eax,0x7f
     c8f:	83 e8 23             	sub    eax,0x23
     c92:	83 e2 7f             	and    edx,0x7f
     c95:	83 f8 03             	cmp    eax,0x3
     c98:	76 a2                	jbe    c3c <parser+0x8aa>
     c9a:	80 fa 4f             	cmp    dl,0x4f
     c9d:	74 9d                	je     c3c <parser+0x8aa>
     c9f:	e8 00 00 00 00       	call   ca4 <parser+0x912>	ca0: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     ca4:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     ca9:	48 89 c6             	mov    rsi,rax
     cac:	e8 00 00 00 00       	call   cb1 <parser+0x91f>	cad: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     cb1:	48 89 44 24 26       	mov    QWORD PTR [rsp+0x26],rax
     cb6:	e9 72 ff ff ff       	jmp    c2d <parser+0x89b>
     cbb:	48 8b 44 24 1e       	mov    rax,QWORD PTR [rsp+0x1e]
     cc0:	8a 00                	mov    al,BYTE PTR [rax]
     cc2:	3c 23                	cmp    al,0x23
     cc4:	75 7a                	jne    d40 <parser+0x9ae>
     cc6:	41 80 ff 03          	cmp    r15b,0x3
     cca:	74 0a                	je     cd6 <parser+0x944>
     ccc:	41 80 ff 0f          	cmp    r15b,0xf
     cd0:	0f 85 2e 02 00 00    	jne    f04 <parser+0xb72>
     cd6:	48 8b 54 24 26       	mov    rdx,QWORD PTR [rsp+0x26]
     cdb:	8a 02                	mov    al,BYTE PTR [rdx]
     cdd:	3c 10                	cmp    al,0x10
     cdf:	74 19                	je     cfa <parser+0x968>
     ce1:	3c 57                	cmp    al,0x57
     ce3:	74 15                	je     cfa <parser+0x968>
     ce5:	3c 02                	cmp    al,0x2
     ce7:	0f 85 17 02 00 00    	jne    f04 <parser+0xb72>
     ced:	48 8b 42 09          	mov    rax,QWORD PTR [rdx+0x9]
     cf1:	80 38 0c             	cmp    BYTE PTR [rax],0xc
     cf4:	0f 85 0a 02 00 00    	jne    f04 <parser+0xb72>
     cfa:	31 f6                	xor    esi,esi
     cfc:	bf 52 00 00 00       	mov    edi,0x52
     d01:	e8 00 00 00 00       	call   d06 <parser+0x974>	d02: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     d06:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     d0b:	48 89 c6             	mov    rsi,rax
     d0e:	e8 00 00 00 00       	call   d13 <parser+0x981>	d0f: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     d13:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     d18:	80 38 10             	cmp    BYTE PTR [rax],0x10
     d1b:	0f 85 e3 01 00 00    	jne    f04 <parser+0xb72>
     d21:	f6 40 0d 04          	test   BYTE PTR [rax+0xd],0x4
     d25:	0f 84 d9 01 00 00    	je     f04 <parser+0xb72>
     d2b:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
     d30:	c6 40 0d 14          	mov    BYTE PTR [rax+0xd],0x14
     d34:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     d38:	ff 40 09             	inc    DWORD PTR [rax+0x9]
     d3b:	e9 c4 01 00 00       	jmp    f04 <parser+0xb72>
     d40:	3c 25                	cmp    al,0x25
     d42:	75 34                	jne    d78 <parser+0x9e6>
     d44:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     d49:	8a 00                	mov    al,BYTE PTR [rax]
     d4b:	3c 10                	cmp    al,0x10
     d4d:	74 08                	je     d57 <parser+0x9c5>
     d4f:	3c 52                	cmp    al,0x52
     d51:	0f 85 ad 01 00 00    	jne    f04 <parser+0xb72>
     d57:	be 01 00 00 00       	mov    esi,0x1
     d5c:	bf 57 00 00 00       	mov    edi,0x57
     d61:	e8 00 00 00 00       	call   d66 <parser+0x9d4>	d62: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     d66:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     d6b:	48 89 c6             	mov    rsi,rax
     d6e:	e8 00 00 00 00       	call   d73 <parser+0x9e1>	d6f: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     d73:	e9 8c 01 00 00       	jmp    f04 <parser+0xb72>
     d78:	3c 2b                	cmp    al,0x2b
     d7a:	75 41                	jne    dbd <parser+0xa2b>
     d7c:	48 8b 44 24 2e       	mov    rax,QWORD PTR [rsp+0x2e]
     d81:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     d85:	8a 00                	mov    al,BYTE PTR [rax]
     d87:	83 e0 7f             	and    eax,0x7f
     d8a:	3c 25                	cmp    al,0x25
     d8c:	75 52                	jne    de0 <parser+0xa4e>
     d8e:	48 8b 54 24 26       	mov    rdx,QWORD PTR [rsp+0x26]
     d93:	8a 02                	mov    al,BYTE PTR [rdx]
     d95:	3c 29                	cmp    al,0x29
     d97:	75 08                	jne    da1 <parser+0xa0f>
     d99:	c6 02 56             	mov    BYTE PTR [rdx],0x56
     d9c:	e9 09 02 00 00       	jmp    faa <parser+0xc18>
     da1:	3c 56                	cmp    al,0x56
     da3:	0f 84 01 02 00 00    	je     faa <parser+0xc18>
     da9:	be 01 00 00 00       	mov    esi,0x1
     dae:	bf 56 00 00 00       	mov    edi,0x56
     db3:	e8 00 00 00 00       	call   db8 <parser+0xa26>	db4: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     db8:	e9 35 01 00 00       	jmp    ef2 <parser+0xb60>
     dbd:	8d 50 de             	lea    edx,[rax-0x22]
     dc0:	80 e2 fd             	and    dl,0xfd
     dc3:	75 7e                	jne    e43 <parser+0xab1>
     dc5:	3c 24                	cmp    al,0x24
     dc7:	74 04                	je     dcd <parser+0xa3b>
     dc9:	3c 22                	cmp    al,0x22
     dcb:	75 13                	jne    de0 <parser+0xa4e>
     dcd:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     dd2:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
     dd6:	80 38 23             	cmp    BYTE PTR [rax],0x23
     dd9:	75 05                	jne    de0 <parser+0xa4e>
     ddb:	e8 00 00 00 00       	call   de0 <parser+0xa4e>	ddc: R_X86_64_PLT32	_pop_front_fbgc_ll-0x4
     de0:	48 89 ef             	mov    rdi,rbp
     de3:	e8 00 00 00 00       	call   de8 <parser+0xa56>	de4: R_X86_64_PLT32	handle_before_paranthesis-0x4
     de8:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     ded:	0f 85 2f 02 00 00    	jne    1022 <parser+0xc90>
     df3:	48 8b 54 24 1e       	mov    rdx,QWORD PTR [rsp+0x1e]
     df8:	8a 02                	mov    al,BYTE PTR [rdx]
     dfa:	3c 22                	cmp    al,0x22
     dfc:	0f 94 c1             	sete   cl
     dff:	3c 2b                	cmp    al,0x2b
     e01:	74 04                	je     e07 <parser+0xa75>
     e03:	84 c9                	test   cl,cl
     e05:	74 05                	je     e0c <parser+0xa7a>
     e07:	c6 44 24 3f 01       	mov    BYTE PTR [rsp+0x3f],0x1
     e0c:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     e11:	80 38 53             	cmp    BYTE PTR [rax],0x53
     e14:	0f 85 90 01 00 00    	jne    faa <parser+0xc18>
     e1a:	84 c9                	test   cl,cl
     e1c:	0f 84 88 01 00 00    	je     faa <parser+0xc18>
     e22:	48 8b 4c 24 2e       	mov    rcx,QWORD PTR [rsp+0x2e]
     e27:	48 8b 71 01          	mov    rsi,QWORD PTR [rcx+0x1]
     e2b:	48 39 71 09          	cmp    QWORD PTR [rcx+0x9],rsi
     e2f:	0f 84 75 01 00 00    	je     faa <parser+0xc18>
     e35:	48 89 50 01          	mov    QWORD PTR [rax+0x1],rdx
     e39:	c6 44 24 3f 08       	mov    BYTE PTR [rsp+0x3f],0x8
     e3e:	e9 67 01 00 00       	jmp    faa <parser+0xc18>
     e43:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     e48:	3c 29                	cmp    al,0x29
     e4a:	75 47                	jne    e93 <parser+0xb01>
     e4c:	48 8b 57 01          	mov    rdx,QWORD PTR [rdi+0x1]
     e50:	8a 02                	mov    al,BYTE PTR [rdx]
     e52:	3c 29                	cmp    al,0x29
     e54:	75 08                	jne    e5e <parser+0xacc>
     e56:	ff 42 09             	inc    DWORD PTR [rdx+0x9]
     e59:	e9 4c 01 00 00       	jmp    faa <parser+0xc18>
     e5e:	83 e0 7f             	and    eax,0x7f
     e61:	83 e8 23             	sub    eax,0x23
     e64:	a8 fd                	test   al,0xfd
     e66:	74 17                	je     e7f <parser+0xaed>
     e68:	bf 23 00 00 00       	mov    edi,0x23
     e6d:	e8 00 00 00 00       	call   e72 <parser+0xae0>	e6e: R_X86_64_PLT32	_new_fbgc_ll_base-0x4
     e72:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     e77:	48 89 c6             	mov    rsi,rax
     e7a:	e8 00 00 00 00       	call   e7f <parser+0xaed>	e7b: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     e7f:	be 02 00 00 00       	mov    esi,0x2
     e84:	bf 29 00 00 00       	mov    edi,0x29
     e89:	e8 00 00 00 00       	call   e8e <parser+0xafc>	e8a: R_X86_64_PLT32	_new_fbgc_ll_opcode_int-0x4
     e8e:	48 89 c6             	mov    rsi,rax
     e91:	eb 76                	jmp    f09 <parser+0xb77>
     e93:	3c 26                	cmp    al,0x26
     e95:	75 45                	jne    edc <parser+0xb4a>
     e97:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
     e9b:	80 38 25             	cmp    BYTE PTR [rax],0x25
     e9e:	75 1d                	jne    ebd <parser+0xb2b>
     ea0:	e8 00 00 00 00       	call   ea5 <parser+0xb13>	ea1: R_X86_64_PLT32	_pop_front_fbgc_ll-0x4
     ea5:	48 89 ef             	mov    rdi,rbp
     ea8:	e8 00 00 00 00       	call   ead <parser+0xb1b>	ea9: R_X86_64_PLT32	handle_before_brackets-0x4
     ead:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     eb2:	0f 84 f2 00 00 00    	je     faa <parser+0xc18>
     eb8:	e9 65 01 00 00       	jmp    1022 <parser+0xc90>
     ebd:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # ec4 <parser+0xb32>	ec0: R_X86_64_PC32	.rodata+0xfc
     ec4:	ba 6c 04 00 00       	mov    edx,0x46c
     ec9:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # ed0 <parser+0xb3e>	ecc: R_X86_64_PC32	.LC0-0x4
     ed0:	48 8d 3d 00 00 00 00 	lea    rdi,[rip+0x0]        # ed7 <parser+0xb45>	ed3: R_X86_64_PC32	.LC2-0x4
     ed7:	e9 f5 fc ff ff       	jmp    bd1 <parser+0x83f>
     edc:	3c 3d                	cmp    al,0x3d
     ede:	75 24                	jne    f04 <parser+0xb72>
     ee0:	48 8b 47 01          	mov    rax,QWORD PTR [rdi+0x1]
     ee4:	8a 00                	mov    al,BYTE PTR [rax]
     ee6:	83 e0 7f             	and    eax,0x7f
     ee9:	3c 4f                	cmp    al,0x4f
     eeb:	75 17                	jne    f04 <parser+0xb72>
     eed:	e8 00 00 00 00       	call   ef2 <parser+0xb60>	eee: R_X86_64_PLT32	_top_and_pop_front_fbgc_ll-0x4
     ef2:	48 8b 7c 24 26       	mov    rdi,QWORD PTR [rsp+0x26]
     ef7:	48 89 c6             	mov    rsi,rax
     efa:	e8 00 00 00 00       	call   eff <parser+0xb6d>	efb: R_X86_64_PLT32	_insert_next_fbgc_ll-0x4
     eff:	e9 e0 f6 ff ff       	jmp    5e4 <parser+0x252>
     f04:	48 8b 74 24 1e       	mov    rsi,QWORD PTR [rsp+0x1e]
     f09:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
     f0e:	e8 00 00 00 00       	call   f13 <parser+0xb81>	f0f: R_X86_64_PLT32	_push_front_fbgc_ll-0x4
     f13:	e9 92 00 00 00       	jmp    faa <parser+0xc18>
     f18:	48 89 ef             	mov    rdi,rbp
     f1b:	e8 00 00 00 00       	call   f20 <parser+0xb8e>	f1c: R_X86_64_PLT32	gm_seek_left-0x4
     f20:	80 7c 24 3e 00       	cmp    BYTE PTR [rsp+0x3e],0x0
     f25:	0f 85 f7 00 00 00    	jne    1022 <parser+0xc90>
     f2b:	48 8b 54 24 2e       	mov    rdx,QWORD PTR [rsp+0x2e]
     f30:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     f34:	8a 08                	mov    cl,BYTE PTR [rax]
     f36:	80 f9 10             	cmp    cl,0x10
     f39:	74 09                	je     f44 <parser+0xbb2>
     f3b:	80 f9 57             	cmp    cl,0x57
     f3e:	0f 85 ca fc ff ff    	jne    c0e <parser+0x87c>
     f44:	48 8b 48 01          	mov    rcx,QWORD PTR [rax+0x1]
     f48:	40 8a 31             	mov    sil,BYTE PTR [rcx]
     f4b:	8d 7e dd             	lea    edi,[rsi-0x23]
     f4e:	40 80 e7 df          	and    dil,0xdf
     f52:	74 06                	je     f5a <parser+0xbc8>
     f54:	40 80 fe 29          	cmp    sil,0x29
     f58:	75 12                	jne    f6c <parser+0xbda>
     f5a:	48 8b 49 01          	mov    rcx,QWORD PTR [rcx+0x1]
     f5e:	48 85 c9             	test   rcx,rcx
     f61:	74 05                	je     f68 <parser+0xbd6>
     f63:	80 39 1d             	cmp    BYTE PTR [rcx],0x1d
     f66:	74 04                	je     f6c <parser+0xbda>
     f68:	80 48 0d 80          	or     BYTE PTR [rax+0xd],0x80
     f6c:	48 8b 4c 24 1e       	mov    rcx,QWORD PTR [rsp+0x1e]
     f71:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     f76:	48 8b 71 01          	mov    rsi,QWORD PTR [rcx+0x1]
     f7a:	48 89 70 01          	mov    QWORD PTR [rax+0x1],rsi
     f7e:	48 8b 42 01          	mov    rax,QWORD PTR [rdx+0x1]
     f82:	8a 10                	mov    dl,BYTE PTR [rax]
     f84:	80 fa 10             	cmp    dl,0x10
     f87:	75 06                	jne    f8f <parser+0xbfd>
     f89:	8a 11                	mov    dl,BYTE PTR [rcx]
     f8b:	88 10                	mov    BYTE PTR [rax],dl
     f8d:	eb 1b                	jmp    faa <parser+0xc18>
     f8f:	80 fa 57             	cmp    dl,0x57
     f92:	75 16                	jne    faa <parser+0xc18>
     f94:	c6 00 4e             	mov    BYTE PTR [rax],0x4e
     f97:	eb 11                	jmp    faa <parser+0xc18>
     f99:	48 8d 0d 00 00 00 00 	lea    rcx,[rip+0x0]        # fa0 <parser+0xc0e>	f9c: R_X86_64_PC32	.rodata+0xfc
     fa0:	ba ac 04 00 00       	mov    edx,0x4ac
     fa5:	e9 1f ff ff ff       	jmp    ec9 <parser+0xb37>
     faa:	48 8b 44 24 26       	mov    rax,QWORD PTR [rsp+0x26]
     faf:	48 8b 40 01          	mov    rax,QWORD PTR [rax+0x1]
     fb3:	48 89 44 24 1e       	mov    QWORD PTR [rsp+0x1e],rax
     fb8:	e9 71 f4 ff ff       	jmp    42e <parser+0x9c>
     fbd:	8a 00                	mov    al,BYTE PTR [rax]
     fbf:	89 c2                	mov    edx,eax
     fc1:	83 e0 7f             	and    eax,0x7f
     fc4:	83 e2 7f             	and    edx,0x7f
     fc7:	8d 4a e8             	lea    ecx,[rdx-0x18]
     fca:	80 e1 f7             	and    cl,0xf7
     fcd:	74 08                	je     fd7 <parser+0xc45>
     fcf:	83 ea 1a             	sub    edx,0x1a
     fd2:	80 fa 03             	cmp    dl,0x3
     fd5:	77 22                	ja     ff9 <parser+0xc67>
     fd7:	48 98                	cdqe   
     fd9:	48 8d 15 00 00 00 00 	lea    rdx,[rip+0x0]        # fe0 <parser+0xc4e>	fdc: R_X86_64_PC32	object_name_array-0x4
     fe0:	bf 01 00 00 00       	mov    edi,0x1
     fe5:	48 8b 14 c2          	mov    rdx,QWORD PTR [rdx+rax*8]
     fe9:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # ff0 <parser+0xc5e>	fec: R_X86_64_PC32	.LC9-0x4
     ff0:	31 c0                	xor    eax,eax
     ff2:	e8 00 00 00 00       	call   ff7 <parser+0xc65>	ff3: R_X86_64_PLT32	__printf_chk-0x4
     ff7:	eb 4e                	jmp    1047 <parser+0xcb5>
     ff9:	44 89 e2             	mov    edx,r12d
     ffc:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 1003 <parser+0xc71>	fff: R_X86_64_PC32	.LC10-0x4
    1003:	bf 64 00 00 00       	mov    edi,0x64
    1008:	31 c0                	xor    eax,eax
    100a:	e8 00 00 00 00       	call   100f <parser+0xc7d>	100b: R_X86_64_PLT32	cprintf-0x4
    100f:	48 8b 7c 24 2e       	mov    rdi,QWORD PTR [rsp+0x2e]
    1014:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 101b <parser+0xc89>	1017: R_X86_64_PC32	.LC11-0x4
    101b:	e8 00 00 00 00       	call   1020 <parser+0xc8e>	101c: R_X86_64_PLT32	_print_fbgc_ll-0x4
    1020:	eb 25                	jmp    1047 <parser+0xcb5>
    1022:	0f b6 7c 24 3e       	movzx  edi,BYTE PTR [rsp+0x3e]
    1027:	44 89 e6             	mov    esi,r12d
    102a:	e8 00 00 00 00       	call   102f <parser+0xc9d>	102b: R_X86_64_PLT32	fbgc_error-0x4
    102f:	48 8d 54 24 40       	lea    rdx,[rsp+0x40]
    1034:	48 8d 35 00 00 00 00 	lea    rsi,[rip+0x0]        # 103b <parser+0xca9>	1037: R_X86_64_PC32	.LC12-0x4
    103b:	31 c0                	xor    eax,eax
    103d:	bf 64 00 00 00       	mov    edi,0x64
    1042:	e8 00 00 00 00       	call   1047 <parser+0xcb5>	1043: R_X86_64_PLT32	cprintf-0x4
    1047:	31 c0                	xor    eax,eax
    1049:	48 8b 9c 24 28 04 00 00 	mov    rbx,QWORD PTR [rsp+0x428]
    1051:	64 48 33 1c 25 28 00 00 00 	xor    rbx,QWORD PTR fs:0x28
    105a:	74 05                	je     1061 <parser+0xccf>
    105c:	e8 00 00 00 00       	call   1061 <parser+0xccf>	105d: R_X86_64_PLT32	__stack_chk_fail-0x4
    1061:	48 81 c4 38 04 00 00 	add    rsp,0x438
    1068:	5b                   	pop    rbx
    1069:	5d                   	pop    rbp
    106a:	41 5c                	pop    r12
    106c:	41 5d                	pop    r13
    106e:	41 5e                	pop    r14
    1070:	41 5f                	pop    r15
    1072:	c3                   	ret    
