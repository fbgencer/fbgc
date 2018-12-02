#include <cstdio>
#include <cstring>
void my_int_func(int x){
    printf( "fptr:%d\n", x );
}

bool is_num(const char *s){
	return (*s>= '0' && *s<='9' );
}

bool is_letter(const char *s){
	return (*s>= 'a' && *s<='z' );
}

bool is_fbg(const char *s){
	return !strcmp(s,"fbg");
}

bool (*comparator[3])(const char*) = {is_num,is_letter,is_fbg};

/*
struct number {
    struct {
        int (*sum)(struct number *, int);
        int (*product)(struct number *, int);
    } * methods;
    void * data;
};

struct number * num;
int sum = num->methods->sum(num, 3);
*/
  
typedef struct {
  int a;
  int b;
  int *array;
} g_thing;
 
g_thing thing = { 1, 2, (int[]){ 3, 4, 5 } };  

int main()
{
   /* void (*foo)(int);
    foo = &my_int_func;
    foo( 2 );
   
    (*foo)( 2 );
    const char *s1 = "fbg";
    const char *l1 = "f";
    const char *n1 = "8";
    	
    printf("STR:[%s] num:[%d] letter:[%d] is_fbg:[%d]\n",s1,comparator[0](s1),comparator[1](s1),comparator[2](s1));	
    printf("STR:[%s] num:[%d] letter:[%d] is_fbg:[%d]\n",l1,comparator[0](l1),comparator[1](l1),comparator[2](l1));
    printf("STR:[%s] num:[%d] letter:[%d] is_fbg:[%d]\n",n1,comparator[0](n1),comparator[1](n1),comparator[2](n1));	
    */

	for (int i = 0; i < 3; ++i)
	{
		printf("%d\n",thing.array[i]);
	}

    
    return 0;
}
