#ifndef UTILITY_H
#define UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#define fbgc_is_power_of_two(x)	((x) != 0 && (((x) & ((x) - 1)) == 0))
size_t fbgc_round_to_closest_power_of_two(const size_t num);


#ifdef  __cplusplus
}
#endif


#endif
