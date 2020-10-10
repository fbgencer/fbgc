// #ifdef __GNUG__
// #ifdef __cplusplus

// #include "../src/fbgc.h"
// #include "example_cpp.h"

// #include <iostream>

// static std::unordered_map<std::string,int> mymap;


// foo::foo(const char* nm, int age){
//     name = nm;
//     this->age  = age;
// }

// void foo::print(){
//     std::cout << "Name : " << name << "Age : "<< age << std::endl;
// }


// // static struct fbgc_object * fbgc_file_print(struct fbgc_cfun_arg * arg){
	
// // 	struct fbgc_cstruct_object * so; 
// // 	struct fbgc_str_object * payload;

// // 	if(parse_tuple_content(arg,".s",&so,&payload) == -1)
// // 		return NULL;
	
// // 	struct file_struct * fs = (struct file_struct *) so->cstruct; 
// // 	fprintf(fs->fp,"%s", payload->content);
		
// // 	return fbgc_nil_object;
// // }


// // static struct fbgc_object_method _fbgc_file_cstruct_methods = {
// // 	.len = 1,
// // 	.method = {
// // 		{.name = "print", .function = &fbgc_file_print}
// // 	}
// // };



// static struct fbgc_object * example_cpp_constructor(struct fbgc_cfun_arg * arg){
	
// 	struct fbgc_object * name;
// 	struct fbgc_object * age;
// 	if(parse_tuple_content(arg,"si",&name,&age) == -1)
// 		return NULL;

// 	struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *) new_fbgc_cstruct_object(sizeof(foo), &_example_cpp_cstruct_property_holder);
// 	foo * fp = (foo *) so->cstruct;
//     foo temp();
// 	//foo

// 	//fs->fp = fopen(content_fbgc_str_object(fname),content_fbgc_str_object(open_type));
// 	//assert(fs->fp != NULL);

// 	return (struct fbgc_object *)so;
// };

// static struct fbgc_cfunction _example_cpp_constructor_struct =  {.name = "foo", .function = &example_cpp_constructor};

// const struct fbgc_object_property_holder _example_cpp_cstruct_property_holder = {
// 	.bits = 
// 	_BIT_NAME |
// 	_BIT_CONSTRUCTOR
// 	,
// 	.properties ={
// 		//{.methods = &_fbgc_file_cstruct_methods},
// 		{.name = "example_cpp"},
// 		{.constructor = &_example_cpp_constructor_struct}
// 	}
// };




// #endif
// #endif