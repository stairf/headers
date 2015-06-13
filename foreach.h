/*****
 * Copyright (c) 2015, Stefan Reif
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****/

/*
 * foreach.h
 *
 * The each macro iterates over a fixed number of elements. It takes a data
 * type, an identifier, and a list of values that are converted to the given
 * data type. The loop body is executed repeatedly, and the variable (given by
 * the identifier) iterates over the given list of values.
 *
 * for each(int, i, 2, 3, 5, 7, 11, 13) {
 *   printf("%d\n", i);
 * }
 *
 * You can use the break and continue keywords in the loop body.
 *
 * For syntax reasons, the type cannot be a function type, a function pointer,
 * or an array type. You can use typedef for these types.
 */
#ifndef __FOREACH_H__
#define __FOREACH_H__

// variables: _  === array
//            __ === pointer (iterator)
#define each(_TYPE, _VARNAME, ...) \
	( \
		_TYPE _[] = { __VA_ARGS__ }, *__ = _, _VARNAME = *__;\
		__ < _ + sizeof(_)/sizeof(*_); \
		_VARNAME = *++__ \
	)

#endif /* __FOREACH_H__ */
