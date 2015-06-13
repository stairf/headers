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
 * combinatorial.h
 *
 * This file defines macros to iterate over number sets. The usage resembles a
 * for loop:
 *
 * for each_combination(array, min, max, len)
 *     foo(array, len);
 *
 * for each_subset(array, min, max, len) {
 *     bar(array, len);
 * }
 *
 * These loops generate all number combinations of length _LENGTH, in the array
 * starting at _PTR, using a range from _MIN (included) to _MAX (excluded).
 * The each_subset macro restricts the array values such that each number
 * appears at most once, and the order of the numbers in the array does not
 * matter.
 *
 * Examples:
 *  - each_combination(out,1,3,2) generates all values of {1,1}, {1,2}, {2,1},
 *    and {2,2}, in any order
 *  - each_subset(out,1,3,2) genereates either {1,2} or {2,1}
 *
 * The loop body is invoked repeatedly, with the number set stored in the given
 * array. The loop terminates after executing the body statement or block with
 * all possible number combinations exactly once. When _LENGTH is zero, then
 * the loop body is executed exactly once for both macros, and the array is not
 * initialized. This loop iteration corresponds to the empty list, or the empty
 * set.
 *
 * You can use the break and continue keywords inside the loop body.
 *
 * Both macros are type-generic, as long as the type supports addition and
 * subtraction. In C, integers and non-void pointer types can be used. In C++,
 * the data type must support comparison, addition and subtraction, and
 * increment and decrement operators. The _PTR argument must be a pointer to
 * the first element of a non-const array of that type.
 *
 * All macro parameters must not have side effects, and must remain unchanged
 * at all loop iterations. The loop body must not modify the array data.
 */
#ifndef __COMBINATORIAL_H__
#define __COMBINATORIAL_H__

// need size_t
#ifdef __cplusplus
#  include <cstddef>
#  define ___COMBINATORIAL__SIZE__TYPE std::size_t
#else
#  include <stddef.h>
#  define ___COMBINATORIAL__SIZE__TYPE size_t
#endif

#define each_combination(_PTR, _MIN, _MAX, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _, __ = 0;; --_) \
		if (!__) { \
			for (_ = 0; _ < (_LENGTH); ++_) \
				(_PTR)[_] = (_MAX); \
			_ = __ = 1; \
		} else if (!(_LENGTH) && _) { \
			break; \
		} else if (_ < (_LENGTH)) { \
			if ((_PTR)[_] <= (_MIN)) { \
				(_PTR)[_] = (_MAX); \
				if (_) \
					continue; \
				else \
					break; \
			} else { \
				(_PTR)[_]--; \
				_ += 2; \
			} \
		} else

#define each_subset(_PTR, _MIN, _MAX, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _, __ = 0;; --_) \
		if (!__) { \
			for (_ = 0; _ < (_LENGTH); ++_) \
				(_PTR)[_] = (_MAX); \
			_ = __ = 1; \
		} else if (!(_LENGTH) && _) { \
			break; \
		} else if (_ < (_LENGTH)) { \
			if ((_PTR)[_] <= (_MIN)) { \
				(_PTR)[_] = (_MAX); \
				if (_) \
					continue; \
				else \
					break; \
			} else { \
				(_PTR)[_]--; \
				_++; \
				if (_ < (_LENGTH) && (_PTR)[_] > (_PTR)[_-1]) \
					(_PTR)[_] = (_PTR)[_-1]; \
				_++; \
			} \
		} else

#endif /* __COMBINATORIAL_H__ */
