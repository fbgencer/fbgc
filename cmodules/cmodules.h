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
#include "fbgc_io.h"
#include "fbgc_stl.h"
#include "fbgc_math.h"
#include "fbgc_file.h"
#include "fbgc_time.h"


#ifdef __cplusplus
extern "C" {

#include "../cmodules/example_cpp.h"
#endif



#define CMODULE_PROPERTY_HOLDER_ARRAY_LEN 2
extern const struct fbgc_object_property_holder  * cmodule_property_holder_array[CMODULE_PROPERTY_HOLDER_ARRAY_LEN];




#ifdef  __cplusplus
}
#endif

#endif