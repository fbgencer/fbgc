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
