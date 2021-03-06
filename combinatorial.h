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
 * This file defines macros to iterate over number sets and permutations. The
 * usage resembles a for loop:
 *
 * TYPE array[len];
 * for each_combination(array, min, max, len)
 *     foo(array, len);
 *
 * TYPE array[len];
 * for each_subset(array, min, max, len) {
 *     foo(array, len);
 * }
 *
 * TYPE array[len];
 * for each_ordered_subset(array, min, max, len)
 *     foo(array, len);
 *
 * TYPE array[len];
 * for each_multiset(array, min, max, len) {
 *     foo(array, len);
 * }
 *
 * TYPE array[len];
 * initialize(array, len);
 * for each_simple_permutation(array, len) {
 *     foo(array, len);
 * }
 *
 * TYPE array[len];
 * initialize(array, len);
 * for each_permutation(array, len)
 *     foo(array, len);
 *
 *
 * === each_combination, each_multiset, each_ordered_subset, each_subset ===
 *
 * These loops generate all number combinations of length _LENGTH, in the array
 * starting at _PTR, using a range from _MIN (included) to _MAX (excluded).
 *
 * Macro                Repetition  Ordered
 * -------------------  ----------  -------
 * each_combination     Y           Y
 * each_multiset        Y           N
 * each_ordered_subset  N           Y
 * each_subset          N           N
 * -------------------  ----------  -------
 *
 * Examples:
 *  - each_combination(out,1,3,2)    generates all values of {1,1}, {1,2},
 *                                   {2,1}, and {2,2}
 *  - each_multiset(out,1,3,2)       generates {1,1}, {2,2}, and either {1,2}
 *                                   or {2,1}
 *  - each_ordered_subset(out,1,3,2) generates both {1,2} and {2,1}
 *  - each_subset(out,1,3,2)         genereates either {1,2} or {2,1}
 *
 * The loop body is invoked repeatedly, with the number set stored in the given
 * array. The loop terminates after executing the body statement or block with
 * all possible number combinations exactly once. When _LENGTH is zero, then
 * the loop body is executed exactly once for all macros, and the array is not
 * initialized. This loop iteration corresponds to the empty list, or the empty
 * set. After the last iteration, all array entries are reset to their initial
 * values.
 *
 * You can use the break and continue keywords inside the loop body.
 *
 * All number set generation macros are type-generic, as long as the type
 * supports addition and subtraction. In C, integers and non-void pointer types
 * can be used. In C++, the data type must support comparison, addition and
 * subtraction, and increment and decrement operators. The _PTR argument must
 * be a pointer to the first element of a non-const array of that type.
 *
 * All macro parameters must not have side effects, and must remain unchanged
 * at all loop iterations. The loop body must not modify the array data.
 *
 * === each_simple_permutation, each_permutation ===
 *
 * These macros generate all possible permutations of the given array, starting
 * at _PTR with the length _LENGTH. The array must be initialized before. The
 * each_simple_permutation macro assumes that each array entry is unique. In
 * contrast, the each_permutation macro automatically detects equal array
 * elements using the `==' operator.
 *
 * At each loop iteration, the array contains a permutation of the initial
 * array. The loop body is executed once for each possible permutation. When
 * _LENGTH is zero, then the loop body is executed exactly once, because the
 * empty set has exactly one possible permutation (which is the empty set).
 * When the each_permutation loop terminates, the array is reset to its initial
 * permutation. In contrast, the each_simple_permutation can leave the array in
 * any permutation.
 *
 * You can use the break and continue keywords inside the loop body. However,
 * the array is not reset to its initial value when the break keyword
 * terminates an each_permutation loop.
 *
 * Both permutation generation macros are generic for all types in C and C++.
 * The only limitation is that, for the each_permutation macro, the array
 * element type has to supports the `==' operator. The memcpy function is used
 * to swap array elements.
 *
 * All macro parameters must not have side effects, and must remain unchanged
 * at all loop iterations. The loop body must not modify the array data.
 */
#ifndef __COMBINATORIAL_H__
#define __COMBINATORIAL_H__

// need size_t
#ifdef __cplusplus
#  include <cstddef>
#  include <cstring>
#  define ___COMBINATORIAL__SIZE__TYPE   std::size_t
#  define ___COMBINATORIAL__MEMCPY__FUNC std::memcpy
#else
#  include <stddef.h>
#  include <string.h>
#  define ___COMBINATORIAL__SIZE__TYPE   size_t
#  define ___COMBINATORIAL__MEMCPY__FUNC memcpy
#endif

#define each_combination(_PTR, _MIN, _MAX, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _ = 0, __ = 0;; --_) \
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

#define each_multiset(_PTR, _MIN, _MAX, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _ = 0, __ = 0;; --_) \
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
					(_PTR)[_] = (_PTR)[_-1]+1; \
				_++; \
			} \
		} else

#define each_subset(_PTR, _MIN, _MAX, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _ = 0, __ = 0;; --_) \
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

#define each_ordered_subset(_PTR, _MIN, _MAX, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _ = 0, __,  ___ = 0;; --_) \
		if (!___) { \
			for (_ = 0; _ < (_LENGTH); ++_) \
				(_PTR)[_] = (_MAX); \
			_ = ___ = 1; \
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
				for (__ = 0; __ < _ && (_PTR)[__] != (_PTR)[_]; ++__); \
				if (__ == _) \
					_++; \
				_++; \
			} \
		} else

#define each_simple_permutation(_PTR, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _ = 0, __[(_LENGTH) ? (_LENGTH) : 1], ___, ____[1 + sizeof((_PTR)[0]) / sizeof(___COMBINATORIAL__SIZE__TYPE)], _____ = 0;;) \
		if (!_____) { \
			__[0] = 0; \
			_____ = 1; \
		} else if (!(_LENGTH) && __[_]) { \
			break; \
		} else if (((_LENGTH) - _) >= 2 && __[_] < ((_LENGTH) - _)) { \
			_++; \
			__[_] = 0; \
		} else if ((_LENGTH) && __[_] >= ((_LENGTH) - _)) { \
			if (!_) \
				break; \
			_--; \
			if (__[_] < ((_LENGTH) - _ - 1)) { \
				___ = (((_LENGTH) - _) % 2) ? 0 : __[_]; \
				___COMBINATORIAL__MEMCPY__FUNC(____, &(_PTR)[(_LENGTH) - _ - 1], sizeof((_PTR)[0])); \
				___COMBINATORIAL__MEMCPY__FUNC(&(_PTR)[(_LENGTH) - _ - 1], &(_PTR)[___], sizeof((_PTR)[0])); \
				___COMBINATORIAL__MEMCPY__FUNC(&(_PTR)[___], ____, sizeof((_PTR)[0])); \
			} \
			__[_]++; \
		} else if (((__[_] = (_LENGTH) - _ + 1)) && 0) {} else

#define each_permutation(_PTR, _LENGTH) \
	(___COMBINATORIAL__SIZE__TYPE _ = 0, __, ___[(_LENGTH) ? (_LENGTH) : 1], ____[1 + (sizeof((_PTR)[0]) * (_LENGTH)) / sizeof(___COMBINATORIAL__SIZE__TYPE)], _____[(_LENGTH) ? (_LENGTH) : 1], ______ = 0;; --_) \
		if (!______) { \
			for (_ = 0; _ < (_LENGTH); ++_) {\
				for (__ = 0; __ < _ && !((_PTR)[_] == (_PTR)[__]); ++__); \
				_____[_] = __; \
				___[_] = (_LENGTH); \
				___COMBINATORIAL__MEMCPY__FUNC(((char*)____) + (sizeof((_PTR)[0]) * _), &(_PTR)[_], sizeof((_PTR)[0])); \
			} \
			_ = ______ = 1; \
		} else if (!(_LENGTH) && _) { \
			break; \
		} else if (_ < (_LENGTH)) { \
			if (!___[_]) { \
				___[_] = (_LENGTH); \
				if (_) \
					continue; \
				else \
					break; \
			} else { \
				___[_]--; \
				for (__ = 0; __ < _ && (_____[___[_]] != _____[___[__]] || ___[__] < ___[_]); ++__); \
				if (__ == _) { \
					___COMBINATORIAL__MEMCPY__FUNC(&(_PTR)[__], ((char*)____) + (sizeof((_PTR)[0]) * (___[__])), sizeof((_PTR)[0])); \
					_++; \
				} else if (___[__] > ___[_]) { \
					while (_ > __) \
						___[_--] = (_LENGTH); \
				} \
				_++; \
			} \
		} else

#endif /* __COMBINATORIAL_H__ */
