#include "fbgc.h"

void keypair_test();
void some_arbitrary_struct_test();

void array_test(){
    keypair_test();
    //some_arbitrary_struct_test();
}


struct keypair{
    struct fbgc_object * key; //fbgc_cstr_object
    struct fbgc_object * value; //any fbgc_object
};

void keypair_test(){
    
    struct keypair kp;
    struct fbgc_array_object * ao = new_fbgc_array_object(0,sizeof(struct keypair));

    struct fbgc_object * array  = ao;

    //Lets push the following key&value pairs into our array

    kp.key = new_fbgc_cstr_object("fbgencer");
    kp.value = new_fbgc_int_object(314151619);
    _push_back_fbgc_array_object(&ao,&kp);
   
    kp.key = new_fbgc_cstr_object("__fbgc__");
    kp.value = new_fbgc_double_object(3.14151619);
    _push_back_fbgc_array_object(&ao,&kp);

    kp.key = new_fbgc_cstr_object("xXxX:)");
    kp.value = new_fbgc_logic_object(1);
    _push_back_fbgc_array_object(&ao,&kp);


    kp.key = new_fbgc_cstr_object("0123456");
    kp.value = new_fbgc_double_object(-273.15);
    _push_back_fbgc_array_object(&ao,&kp);

    struct keypair * kk = (struct keypair *)get_back_in_fbgc_array_object(ao);
    printf("Last data in the array\n");
    printf("Key : "); print_fbgc_object(kk->key); printf("\n");
    printf("Value : "); print_fbgc_object(kk->value); printf("\n");
    
    kk = (struct keypair *)get_front_in_fbgc_array_object(ao);
    printf("First data in the array\n");
    printf("Key : "); print_fbgc_object(kk->key); printf("\n");
    printf("Value : "); print_fbgc_object(kk->value); printf("\n");


    for(int i = 0; i<size_fbgc_array_object(array); ++i){
        kk = get_address_in_fbgc_array_object(array,i);
        printf("Data[%d]\n",i);
        printf("Key : "); print_fbgc_object(kk->key); printf("\n");
        printf("Value : "); print_fbgc_object(kk->value); printf("\n");        
    }   
}


struct some_arbitrary_struct{
    double * data;
    int len;
};


void some_arbitrary_struct_test(){
    struct some_arbitrary_struct sas;
    //Lets allocate at least 3 capacity array object
    struct fbgc_array_object * ao = new_fbgc_array_object(3,sizeof(struct some_arbitrary_struct));

    //First irrationals
    double irrational[] = {2.72,3.14,3.14/2};
    sas.len = sizeof(irrational)/sizeof(irrational[0]);
    sas.data = irrational;
    set_in_fbgc_array_object(ao,&sas,1); // we set at the second value in the array

    double odd[] = {3,5,7,9,11,13};
    sas.len = sizeof(odd)/sizeof(odd[0]); 
    sas.data = odd;
    set_in_fbgc_array_object(ao,&sas,2); // we set at last value in the array

    double even[] = {0,2,4,6,8,10};
    sas.len = sizeof(even)/sizeof(even[0]); 
    sas.data = even;
    set_in_fbgc_array_object(ao,&sas,0); // we set at first value in the array

    //when we use set function we NEED to assign the size of the array, for push_back we do not need this

    size_fbgc_array_object(ao) = 3;


    for(int i = 0; i<size_fbgc_array_object(ao); ++i){
        struct some_arbitrary_struct * kk = get_address_in_fbgc_array_object(ao,i);
        printf("Data in array[%d]\n",i);
        printf("Length:%d\n",kk->len);
        printf("Data:");
        for(int j = 0; j<kk->len; ++j)
            printf("%g,",kk->data[j]);
        printf("\n");
    }

}