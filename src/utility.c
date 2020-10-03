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
#include "fbgc.h"

size_t fbgc_round_to_closest_power_of_two(const size_t size){
/*
        Below algorithm calculates the capacity for the given size
        Basically capacity is the closest two's power
        0,1 : 1
        2 : 2
        3,4 : 4
        5,6,7,8 : 8
        9,10,11,12,13,14,15 : 16
        and so on

        Take 5 for example, in binary its 0b00101
        take z = 1
        0b0001 <= 0b0101 , shift z left
        0b0010 <= 0b0101 , (ditto)
        0b0100 <= 0b0101 , (ditto)
        0b1000 <= 0b0101 , stop here don't shift, z is 8, the closest two's power for 5
*/
    size_t z = 1;
    while(z < size)
        z <<= 1;
    return z;	
}
