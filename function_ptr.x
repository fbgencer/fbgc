#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
//function ptr
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
*/
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
  
/*
//decleration
typedef struct {
  int a;
  int b;
  int *array;
} g_thing;
 

g_thing thing = { 1, 2, (int[]){ 3, 4, 5 } };  
*/


#define OBJECT 0
#define DOUBLE 1
#define INT 2
#define STRING 3
#define VARIABLE 4

struct fbgc_object{
    int type;
    struct fbgc_object * next;
};


struct fbgc_int_object{
    struct fbgc_object base;
    int data;  
};


struct fbgc_double_object{
    struct fbgc_object base;
    double data;  
};

struct fbgc_string_object{
    struct fbgc_object base;
    char * data;  
};

struct fbgc_variable_object{
    struct fbgc_object base;
    struct fbgc_object * data;  
};

void push_fbgc_object(struct fbgc_object * main_obj,struct fbgc_object * inc_next ){
    struct fbgc_object * obj = main_obj;

    while(obj->next != NULL){
        obj = obj->next;
    }
    obj->next = (struct fbgc_object *)  inc_next;
    obj->next->next = NULL;
   // return (struct fbgc_object *) obj;
}

struct
fbgc_object * new_fbgc_double_object(double inc_data){
    struct  fbgc_double_object * dbo =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->data = inc_data;  
    return (struct fbgc_object*) dbo;
}
struct
fbgc_object * new_fbgc_int_object(int inc_data){
    struct  fbgc_int_object * io =  (struct fbgc_int_object*) malloc(sizeof(struct fbgc_int_object));
    io->base.type = INT;
    io->base.next = NULL;
    io->data = inc_data;  
    return (struct fbgc_object*) io;
}

struct
fbgc_object * new_fbgc_string_object(char * inc_data){
    struct  fbgc_string_object * stro =  (struct fbgc_string_object*) malloc(sizeof(struct fbgc_string_object));
    stro->base.type = STRING;
    stro->base.next = NULL;
    stro->data = (char *) malloc(sizeof(char)*(1 + strlen(inc_data) ) );
    strcpy(stro->data,inc_data);
    return (struct fbgc_object*) stro;
}
/*
struct
fbgc_object * assign_variable(struct fbgc_object * obj){
    struct fbgc_variable_object * varobj = (struct fbgc_variable_object *) malloc(sizeof(struct(fbgc_variable_object)));
    varobj->type = VARIABLE;
    varobj->next = NULL;
    varobj->data = (struct fbgc_object *) obj;
    return (struct fbgc_object*) varobj;
}
*/
void free_fbgc_object(struct fbgc_object * main_obj){
   
   struct fbgc_object * tmp;

   while (main_obj != NULL){
       tmp = main_obj;
       main_obj = main_obj->next;
       if(tmp->type == STRING){
            struct fbgc_string_object *dummy_str = (struct fbgc_string_object*)tmp;
            free(dummy_str->data);
       }
       free(tmp);
    }
}
void print_fbgc_object(struct fbgc_object * obj){
    printf("Just object\n");
}
void print_fbgc_double_object(struct fbgc_object * obj){
    struct fbgc_double_object *dummy_db = (struct fbgc_double_object*) obj;
    printf("Double Value : %f\n",dummy_db->data);
}

void print_fbgc_int_object(struct fbgc_object * obj){
    struct fbgc_int_object *dummy_int = (struct fbgc_int_object*)obj;
    printf("Int Value : %d\n",dummy_int->data);    
}

void print_fbgc_string_object(struct fbgc_object * obj){
    struct fbgc_string_object *dummy_str = (struct fbgc_string_object*)obj;
    printf("Str Value : %s\n",dummy_str->data); 
}

void (*printer[4])(struct fbgc_object *) = 
{
    print_fbgc_object,
    print_fbgc_double_object,
    print_fbgc_int_object,
    print_fbgc_string_object,
};

struct
fbgc_object * add_two_fbgc_object(struct fbgc_object *obj,struct fbgc_object *obj2){
    if(obj->type == INT && obj2->type == DOUBLE)
        ((struct fbgc_int_object*) obj)->data += ((struct fbgc_double_object*) obj2)->data;
    else if(obj->type == DOUBLE && obj2->type == INT)
         ((struct fbgc_double_object*) obj)->data += ((struct fbgc_int_object*) obj2)->data;
    else if(obj->type == INT && obj2->type == INT)
         ((struct fbgc_int_object*) obj)->data += ((struct fbgc_int_object*) obj2)->data;
    else if(obj->type == DOUBLE && obj2->type == DOUBLE)
         ((struct fbgc_double_object*) obj)->data += ((struct fbgc_double_object*) obj2)->data;    
    return (struct fbgc_object *) obj;
}
struct
fbgc_object * add_two_fbgc_int_object(struct fbgc_object *obj,struct fbgc_object *obj2){
    struct  fbgc_int_object * io1 =  (struct fbgc_int_object*) obj;
    struct  fbgc_int_object * io2 =  (struct fbgc_int_object*) obj2;
    io1->data += io2->data;
    return (struct fbgc_object *) io1;
}
struct
fbgc_object * add_two_fbgc_string_object(struct fbgc_object *obj,struct fbgc_object *obj2){
    struct  fbgc_string_object * io1 =  (struct fbgc_string_object*) obj;
    struct  fbgc_string_object * io2 =  (struct fbgc_string_object*) obj2;
    io1->data = (char *) realloc(io1->data,(sizeof(char)*(strlen(io1->data)+strlen(io2->data) + 1 )  ));
    strcat(io1->data,io2->data);
    return (struct fbgc_object *) obj;
}



int main(){

    printf("Lets start ! :)\n \n");

    struct fbgc_object * o1 = new_fbgc_double_object(350);
    struct fbgc_object * o2 = new_fbgc_double_object(723);
    struct fbgc_object * o3 = new_fbgc_string_object("fBGencer");
    struct fbgc_object * o4 = new_fbgc_string_object(" <3 elif :)))) ");
    struct fbgc_object * o5 = new_fbgc_int_object(13);

    o1 = add_two_fbgc_object(o1,o5);
    
    push_fbgc_object((struct fbgc_object *) o1,(struct fbgc_object *)o2);
    push_fbgc_object((struct fbgc_object *) o1,(struct fbgc_object *)o3);
    push_fbgc_object((struct fbgc_object *) o1,(struct fbgc_object *)o4);
    push_fbgc_object((struct fbgc_object *) o4,(struct fbgc_object *)o5);
    
    struct fbgc_object *iter = o1;

    while(iter != NULL){
        printf("--Type : %d\n",iter->type );
        printer[iter->type](iter);
        iter = iter->next;
    }
    

    struct fbgc_object * var1 = assign_variable()

    free_fbgc_object(o1); 

   /* struct fbgc_double_object * d1_db = (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    d1_db->base.type = DOUBLE;
    d1_db->base.next = NULL;
    d1_db->data = 3.1415972;


    struct  fbgc_double_object * d2_db =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    d2_db->base.type = DOUBLE;
    d2_db->base.next = NULL;
    d2_db->data = 812.652;

    struct fbgc_int_object * i1_int =  (struct fbgc_int_object*) malloc(sizeof(struct  fbgc_int_object));
    i1_int->base.type = INT;
    i1_int->base.next = NULL;
    i1_int->data = 230996;

    struct fbgc_string_object * s1_str =(struct fbgc_string_object*) malloc(sizeof(struct  fbgc_string_object));
    s1_str->base.type = STRING;
    s1_str->base.next = NULL;
    s1_str->data = (char *) malloc(sizeof(char)*9);
    strcpy(s1_str->data,"fbgencer");
    

    struct fbgc_object *obj1 = (struct fbgc_object*)  d1_db;
    obj1->next = (struct fbgc_object*) d2_db;
    obj1->next->next = (struct fbgc_object*) i1_int;
    obj1->next->next->next = (struct fbgc_object*) s1_str;
    obj1->next->next->next->next =  NULL;
    
    struct fbgc_object *iter = obj1;

    while(iter != NULL){
        printf("Type : %d\n",iter->type );
        
        if(iter->type == DOUBLE){
            struct fbgc_double_object *dummy_db = (struct fbgc_double_object*) iter;
            printf("Value : %f\n",dummy_db->data);
        }
        else if(iter->type == INT){
            struct fbgc_int_object *dummy_int = (struct fbgc_int_object*)iter;
            printf("Value : %d\n",dummy_int->data);
        }
        else if(iter->type == STRING){
            struct fbgc_string_object *dummy_str = (struct fbgc_string_object*)iter;
            printf("Value : %s\n",dummy_str->data);            
        }
        else
            printf("error!\n");

        iter = iter->next;

    }

    free(d1_db);
    free(d2_db);
    free(i1_int);
    free(s1_str->data);
    free(s1_str);*/

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


    
    return 0;
}
