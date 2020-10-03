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
