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
#ifndef CMODULES_H
#define CMODULES_H
//stl and io should be loaded
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_stl.h"


#include "../cmodules/fbgc_math.h"
#include "../cmodules/fbgc_file.h"
#include "../cmodules/fbgc_time.h"
#include "../cmodules/example_cpp.h"

#ifdef __cplusplus
extern "C" {
#endif



const struct fbgc_object_property_holder  * __property_holders[] =
{
	&_fbgc_math_property_holder,
	&_fbgc_file_cstruct_property_holder,
	&_example_cpp_cstruct_property_holder,
	//&fbgc_time_module			
};




#ifdef  __cplusplus
}
#endif

#endif