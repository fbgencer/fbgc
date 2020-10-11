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
#ifndef GRAMMAR_H
#define GRAMMAR_H

#ifdef __cplusplus
extern "C" {
#endif


typedef int8_t fbgc_grammar;
//this is not an object no need to derive from base object 
#include "parser.h"
void gm_convert_left(struct parser_packet * p);
uint8_t gm_seek_left(struct parser_packet * p);
uint8_t gm_seek_right(struct parser_packet * p);

const char * gm2str(fbgc_grammar gm);

/*! \cond DOCUMENT_EVERYTHING
	Lets not document this part, just log definitions nothing important 
*/
#ifdef LOG_GRAMMAR
#define GRAMMAR_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define GRAMMAR_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _GRAMMAR_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _GRAMMAR_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define GRAMMAR_LOGV(...)
#define GRAMMAR_LOGD(...)
#define _GRAMMAR_LOGV(...)
#define _GRAMMAR_LOGD(...)
#endif


#ifdef  __cplusplus
}
#endif


#endif