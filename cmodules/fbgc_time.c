// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
// #include "../src/fbgc.h"
// #include "fbgc_time.h"
// #include <time.h>



// new_fbgc_cfunction(fbgc_tic,"tic")
// {	
	
// 	/*if(argc != 0){
// 		cprintf(100,"<tic> requires no argument!\n");
// 		return NULL;
// 	}

// 	struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *)new_fbgc_cstruct_object(sizeof(struct time_struct), &fbgc_time_module);
// 	struct time_struct * ts = (struct time_struct *) so->cstruct; 	
// 	ts->clk = clock();
// 	return (struct fbgc_object *) so;*/
// }

// new_fbgc_cfunction(fbgc_toc,"toc")
// {	
// 	if(argc != 1){
// 		cprintf(100,"<toc> requires 1 argument!\n");
// 		return NULL;
// 	}

// 	struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
// 	assert(so->parent == &fbgc_time_module);

// 	struct time_struct * ts = (struct time_struct *) so->cstruct; 	

// 	double tm = (double)(clock()-ts->clk) / CLOCKS_PER_SEC; 	

// 	return (struct fbgc_object *) new_fbgc_double_object(tm);
	
// }







// new_fbgc_cfunction(fbgc_time_initializer,"time")
// {	
// 	return NULL;
// }
// const struct fbgc_cmodule fbgc_time_module = 
// {
// 	.initializer = &fbgc_time_initializer_struct,
// 	.functions = (const struct fbgc_cfunction*[])
// 	{
// 		&fbgc_tic_struct,
// 		&fbgc_toc_struct,
// 		NULL
// 	}
// };

