/*******************************************************************************
 * include/error.h
 * Control of the error messages output by the system 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include<cassert>

#ifndef ERR_MSGRi
#define ERR_MSGR(x,y)	{ fprintf(stderr, x);fflush(stdout);assert(0);return y;}
#endif

#ifndef ERR_MSG
#define ERR_MSG(x)	{ fprintf(stderr, x);fflush(stdout); assert(0);}
#endif
