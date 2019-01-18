#include "fbgc.h"


int main(){
	




	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

	/*struct fbgc_object * head = new_fbgc_ll_object();
	
	//2+4*(8)
	//x = 1+2
	head = push_back_fbgc_ll_object(head,new_fbgc_str_object("fbgencer"));
	head = push_back_fbgc_ll_object(head,new_fbgc_object(ASSIGN));
	head = push_back_fbgc_ll_object(head,new_fbgc_int_object(2));
	head = push_back_fbgc_ll_object(head,new_fbgc_object(PLUS));
	head = push_back_fbgc_ll_object(head,new_fbgc_int_object(10));
	head = push_back_fbgc_ll_object(head,new_fbgc_object(MULT));
	head = push_back_fbgc_ll_object(head,new_fbgc_int_object(8));	

	head = push_back_fbgc_ll_object(head,new_fbgc_object(PLUS));
	//head = push_back_fbgc_ll_object(head,new_fbgc_object(LPARA));
	head = push_back_fbgc_ll_object(head,new_fbgc_int_object(8));	
	//head = push_back_fbgc_ll_object(head,new_fbgc_object(RPARA));	
	head = push_back_fbgc_ll_object(head,new_fbgc_object(MULT));
	head = push_back_fbgc_ll_object(head,new_fbgc_int_object(4));


	print_fbgc_ll_object(head);
	
	//head = parser(head);

	print_fbgc_ll_object(head);

	free_fbgc_ll_object(head); */
	struct fbgc_object * head = new_fbgc_ll_object();
	regex_lexer(&head,"3.2+1 ");
	head = parser(head);
	print_fbgc_ll_object(head);
	free_fbgc_ll_object(head);

	cprintf(110,"[=======================================================================]\n\n\n\n\n\n"); 
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