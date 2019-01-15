#include "fbgc.h"


int main(){
	
	struct fbgc_object * head = new_fbgc_ll_object();
	for(int i = 0; i<30; i++)
	head = push_back_fbgc_ll_object(head,new_fbgc_int_object(i));
	for(int i = 50; i<100; i++)
	head = push_front_fbgc_ll_object(head,new_fbgc_double_object(i));
	print_fbgc_ll_object(head);
	free_fbgc_ll_object(head); 



	printf("\033[1;33m[%s]\033[0m\n","======================================================================="); 
	/*struct fbgc_object * head, * tail, * iter;

	head = new_fbgc_int_object(0);
	tail =  new_fbgc_int_object(100);
	iter = head; */


	//2+4*(8)
	//push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	/*push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(2));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(4));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(8));	
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));	*/	
	//push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));

	//8 * ( 5 + 10 / ( 2 - 3 ) ) 
	//Expected :8,5,10,2,3,-,/,+,*
	/*
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(8));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(5));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(10));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(DIV));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(2));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MINUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(3));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));		
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));	
	*/

	//((2+3))
	/*
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(2));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MINUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(3));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));		
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));	
	*/


	//3.14/8*78+112
	
	/*push_fbgc_object_ll(&head,&tail,new_fbgc_double_object(3.14));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(DIV));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(8));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(78));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(112));
	*/



	//3.14/((8*78+112)-5)
	
	/*
	push_fbgc_object_ll(&head,&tail,new_fbgc_double_object(3.14));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(DIV));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));	
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(8));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(78));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(112));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MINUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(5));			
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));		
	*/

	//(-(-10*20+32)-40)
	//10,-,20,*,32,+,-,40,-
	/*push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(UMINUS));	
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(UMINUS));		
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(10));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(20));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(32));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MINUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(40));			
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));*/



	/*print_fbgc_object_ll(head);

	head = parser(head,tail);

	print_fbgc_object_ll(head);

	free_fbgc_object_ll(head);

	free_fbgc_int_object(head);
	free_fbgc_int_object(tail);*/

	/*double dbar[] = {3.14,2.727585};
	struct fbgc_object * head, * iter;
	head = new_fbgc_double_object(10.0);
	iter = head;
	for(int i = 0; i<sizeof(dbar)/sizeof(double); i++){
		struct fbgc_object *db = new_fbgc_double_object(dbar[i]); 
		iter->next = db; 
		iter = iter->next;
	}

	fbgc_token tokenlist[] = {PLUS,MINUS,MULT,DIV};

	for(int i = 0; i<sizeof(tokenlist)/sizeof(fbgc_token); i++){
		struct fbgc_object *o = new_fbgc_object(tokenlist[i]);
		iter->next = o;
		iter = iter->next;
	}
	
	print_fbgc_object_ll(head);
	free_fbgc_object_ll(head);*/


	printf("\033[1;33m[%s]\033[0m\n","=======================================================================");
	return 0;
}

/*

a^-2

M:a,2,-^
O:^,
G:expr


a-^2

M:a,
O:- -> ^ cannot come because G has no id or something..
G:

Birden fazla
a+2*3
b-^10

It couldnt catch invalid input
M:a,2,3,*,+,b,10,^,-
O:
G:exp

parsed like (a+2*3)-(b^10)

How to solve ?

Always reduce the grammar stack !!
M:a,2,3,*,+
O:         now we have b as an input but we have also a grammar top expression, delete the expr and push id
G:expr

M:a,2,3,*,+,b,
O:-,
G:


x = [1+8,0;3,sin(30)]

virgül ve nokt virgül grammerde exp,num vs varsa bakar sonra siler

M:x,1,8,+,0,3,30,sin,2,2,make matrix,=
O:
G:assgn,

x,9,0,3,0.5,2,2,make_matrix,=
x,[9,0;3,0.5],=


##Nested if

if(a)
	if(b)
		if(c)
			d = 100
		elif(d)
			f = 8
		end
	end
end


M:a,if,b,if,c,if,d,100,=,d,elif,f,8,=,END,END,END
O:
G:


if(a=b)
	c = 10
end

M:a,b,=,
O:if,
G:cassign



void doo(fbgc_token op, fbgc_token left){
	switch(op){
		case PLUS: case MINUS: case MULT: case DIV:
			switch(left){
				case NUM: case EXPR: case ID:
					return EXPR;
			}
	}
}


y = ++x; expected: y,x,++,=

M:y,x,++
O:=,
G:unary


x[y] = z

[[1,2],[3,4]]

M:1,2,list(2),3,4,list(2),list(2)
O:
G:list,

b = 10
c = 7


*/