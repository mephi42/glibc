/* Definitions of inline math functions implemented by the m68881/2.
   Copyright (C) 1991-2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _MATH_H
# error "Never use <bits/mathinline.h> directly; include <math.h> instead."
#endif

#ifndef __extern_inline
# define __MATH_INLINE __inline
#else
# define __MATH_INLINE __extern_inline
#endif

#ifdef	__GNUC__

#ifdef __USE_ISOC99
/* GCC 3.1 and up have builtins that actually can be used.  */
# if !__GNUC_PREREQ (3,1)
/* ISO C99 defines some macros to perform unordered comparisons.  The
   m68k FPU supports this with special opcodes and we should use them.
   These must not be inline functions since we have to be able to handle
   all floating-point types.  */
#  undef isgreater
#  undef isgreaterequal
#  undef isless
#  undef islessequal
#  undef islessgreater
#  undef isunordered
#  define isgreater(x, y)					\
   __extension__					\
   ({ char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsogt %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      __result != 0; })

#  define isgreaterequal(x, y)				\
   __extension__					\
   ({ char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsoge %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      __result != 0; })

#  define isless(x, y)					\
   __extension__					\
   ({ char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsolt %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      __result != 0; })

#  define islessequal(x, y)				\
   __extension__					\
   ({ char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsole %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      __result != 0; })

#  define islessgreater(x, y)				\
   __extension__					\
   ({ char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsogl %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      __result != 0; })

#  define isunordered(x, y)				\
   __extension__					\
   ({ char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsun %0"			\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      __result != 0; })
# endif /* GCC 3.1 */

/* Test for negative number.  Used in the signbit() macro.  */
__MATH_INLINE int
__NTH (__signbitf (float __x))
{
  __extension__ union { float __f; int __i; } __u = { __f: __x };
  return __u.__i < 0;
}
__MATH_INLINE int
__NTH (__signbit (double __x))
{
  __extension__ union { double __d; int __i[2]; } __u = { __d: __x };
  return __u.__i[0] < 0;
}
__MATH_INLINE int
__NTH (__signbitl (long double __x))
{
  __extension__ union { long double __d; int __i[3]; } __u = { __d: __x };
  return __u.__i[0] < 0;
}
#endif


#if (!defined __NO_MATH_INLINES && defined __OPTIMIZE__) \
    || defined __LIBC_INTERNAL_MATH_INLINES

#ifdef	__LIBC_INTERNAL_MATH_INLINES
/* This is used when defining the functions themselves.  Define them with
   __ names, and with `static inline' instead of `extern inline' so the
   bodies will always be used, never an external function call.
   Note: GCC 6 objects to __attribute__ ((__leaf__)) on static functions.  */
# define __m81_u(x)		__CONCAT(__,x)
# define __m81_inline		static __inline
# define __m81_nth(fn)		__NTHNL (fn)
#else
# define __m81_u(x)		x
# define __m81_inline		__MATH_INLINE
# define __m81_nth(fn)		__NTH (fn)
# define __M81_MATH_INLINES	1
#endif

/* Define a const math function.  */
#define __m81_defun(rettype, func, args)				      \
  __m81_inline rettype __attribute__((__const__))			      \
  __m81_nth (__m81_u(func) args)

/* Define the three variants of a math function that has a direct
   implementation in the m68k fpu.  FUNC is the name for C (which will be
   suffixed with f and l for the float and long double version, resp).  OP
   is the name of the fpu operation (without leading f).  */

#ifdef __USE_ISOC99
# define __inline_mathop(func, op)			\
  __inline_mathop1(double, func, op)			\
  __inline_mathop1(float, __CONCAT(func,f), op)	\
  __inline_mathop1(long double, __CONCAT(func,l), op)
#else
# define __inline_mathop(func, op)			\
  __inline_mathop1(double, func, op)
#endif

#define __inline_mathop1(float_type,func, op)				      \
  __m81_defun (float_type, func, (float_type __mathop_x))		      \
  {									      \
    float_type __result;						      \
    __asm __volatile__ ("f" __STRING(op) "%.x %1, %0"			      \
			: "=f" (__result) : "f" (__mathop_x));		      \
    return __result;							      \
  }

__inline_mathop(__atan, atan)
__inline_mathop(__cos, cos)
__inline_mathop(__sin, sin)
__inline_mathop(__tan, tan)
__inline_mathop(__tanh, tanh)
__inline_mathop(__fabs, abs)

#if defined __USE_XOPEN_EXTENDED || defined __USE_ISOC99
__inline_mathop(__rint, int)
__inline_mathop(__expm1, etoxm1)
__inline_mathop(__log1p, lognp1)
#endif

#ifdef __USE_MISC
__inline_mathop(__significand, getman)
#endif

#ifdef __USE_ISOC99
__inline_mathop(__trunc, intrz)
#endif

#if !defined __NO_MATH_INLINES && defined __OPTIMIZE__

__inline_mathop(atan, atan)
__inline_mathop(tanh, tanh)

# if defined __USE_XOPEN_EXTENDED || defined __USE_ISOC99
__inline_mathop(rint, int)
__inline_mathop(log1p, lognp1)
# endif

# ifdef __USE_MISC
__inline_mathop(significand, getman)
# endif

# ifdef __USE_ISOC99
__inline_mathop(trunc, intrz)
# endif

#endif /* !__NO_MATH_INLINES && __OPTIMIZE__ */

/* This macro contains the definition for the rest of the inline
   functions, using FLOAT_TYPE as the domain type and M as a macro
   that adds the suffix for the function names.  */

#define __inline_functions(float_type, m)				  \
__m81_defun (float_type, m(__floor), (float_type __x))			  \
{									  \
  float_type __result;							  \
  unsigned long int __ctrl_reg;						  \
  __asm __volatile__ ("fmove%.l %!, %0" : "=dm" (__ctrl_reg));		  \
  /* Set rounding towards negative infinity.  */			  \
  __asm __volatile__ ("fmove%.l %0, %!" : /* No outputs.  */		  \
		      : "dmi" ((__ctrl_reg & ~0x10) | 0x20));		  \
  /* Convert X to an integer, using -Inf rounding.  */			  \
  __asm __volatile__ ("fint%.x %1, %0" : "=f" (__result) : "f" (__x));	  \
  /* Restore the previous rounding mode.  */				  \
  __asm __volatile__ ("fmove%.l %0, %!" : /* No outputs.  */		  \
		      : "dmi" (__ctrl_reg));				  \
  return __result;							  \
}									  \
									  \
__m81_defun (float_type, m(__ceil), (float_type __x))			  \
{									  \
  float_type __result;							  \
  unsigned long int __ctrl_reg;						  \
  __asm __volatile__ ("fmove%.l %!, %0" : "=dm" (__ctrl_reg));		  \
  /* Set rounding towards positive infinity.  */			  \
  __asm __volatile__ ("fmove%.l %0, %!" : /* No outputs.  */		  \
		      : "dmi" (__ctrl_reg | 0x30));			  \
  /* Convert X to an integer, using +Inf rounding.  */			  \
  __asm __volatile__ ("fint%.x %1, %0" : "=f" (__result) : "f" (__x));	  \
  /* Restore the previous rounding mode.  */				  \
  __asm __volatile__ ("fmove%.l %0, %!" : /* No outputs.  */		  \
		      : "dmi" (__ctrl_reg));				  \
  return __result;							  \
}

#define __CONCAT_d(arg) arg
#define __CONCAT_f(arg) arg ## f
#define __CONCAT_l(arg) arg ## l
__inline_functions(double, __CONCAT_d)
#ifdef __USE_ISOC99
__inline_functions(float, __CONCAT_f)
__inline_functions(long double, __CONCAT_l)
#endif
#undef __inline_functions

#ifdef __USE_MISC

# define __inline_functions(float_type, m)				  \
__m81_defun (int, m(__isinf), (float_type __value))			  \
{									  \
  /* There is no branch-condition for infinity,				  \
     so we must extract and examine the condition codes manually.  */	  \
  unsigned long int __fpsr;						  \
  __asm ("ftst%.x %1\n"							  \
	 "fmove%.l %/fpsr, %0" : "=dm" (__fpsr) : "f" (__value));	  \
  return (__fpsr & (2 << 24)) ? (__fpsr & (8 << 24) ? -1 : 1) : 0;	  \
}									  \
									  \
__m81_defun (int, m(__finite), (float_type __value))			  \
{									  \
  /* There is no branch-condition for infinity, so we must extract and	  \
     examine the condition codes manually.  */				  \
  unsigned long int __fpsr;						  \
  __asm ("ftst%.x %1\n"							  \
	 "fmove%.l %/fpsr, %0" : "=dm" (__fpsr) : "f" (__value));	  \
  return (__fpsr & (3 << 24)) == 0;					  \
}									  \
									  \
__m81_defun (float_type, m(__scalbn),					  \
	     (float_type __x, int __n))					  \
{									  \
  float_type __result;							  \
  __asm __volatile__  ("fscale%.l %1, %0" : "=f" (__result)		  \
		       : "dmi" (__n), "0" (__x));			  \
  return __result;							  \
}

__inline_functions(double, __CONCAT_d)
__inline_functions(float, __CONCAT_f)
__inline_functions(long double, __CONCAT_l)
# undef __inline_functions

#endif /* Use misc.  */

#if defined __USE_MISC || defined __USE_XOPEN

# define __inline_functions(float_type, m)				  \
__m81_defun (int, m(__isnan), (float_type __value))		  	  \
{									  \
  char __result;							  \
  __asm ("ftst%.x %1\n"							  \
	 "fsun %0" : "=dm" (__result) : "f" (__value));			  \
  return __result;							  \
}

__inline_functions(double, __CONCAT_d)
# ifdef __USE_MISC
__inline_functions(float, __CONCAT_f)
__inline_functions(long double, __CONCAT_l)
# endif
# undef __inline_functions

#endif

#ifdef __USE_ISOC99

# define __inline_functions(float_type, m)				  \
__m81_defun (float_type, m(__scalbln),					  \
	     (float_type __x, long int __n))				  \
{									  \
  return m(__scalbn) (__x, __n);					  \
}									  \
									  \
__m81_defun (float_type, m(__nearbyint), (float_type __x))		  \
{									  \
  float_type __result;							  \
  unsigned long int __ctrl_reg;						  \
  __asm __volatile__ ("fmove%.l %!, %0" : "=dm" (__ctrl_reg));		  \
  /* Temporarily disable the inexact exception.  */			  \
  __asm __volatile__ ("fmove%.l %0, %!" : /* No outputs.  */		  \
		      : "dmi" (__ctrl_reg & ~0x200));			  \
  __asm __volatile__ ("fint%.x %1, %0" : "=f" (__result) : "f" (__x));	  \
  __asm __volatile__ ("fmove%.l %0, %!" : /* No outputs.  */		  \
		      : "dmi" (__ctrl_reg));				  \
  return __result;							  \
}									  \
									  \
__m81_defun (long int, m(__lrint), (float_type __x))			  \
{									  \
  long int __result;							  \
  __asm __volatile__ ("fmove%.l %1, %0" : "=dm" (__result) : "f" (__x));  \
  return __result;							  \
}

__inline_functions (double, __CONCAT_d)
__inline_functions (float, __CONCAT_f)
__inline_functions (long double, __CONCAT_l)
# undef __inline_functions

#endif /* Use ISO C9x */

#ifdef __USE_GNU

# define __inline_functions(float_type, m)				\
__m81_inline void							\
__m81_nth (__m81_u(m(__sincos))						\
	   (float_type __x, float_type *__sinx, float_type *__cosx))	\
{									\
  __asm __volatile__ ("fsincos%.x %2,%1:%0"				\
		      : "=f" (*__sinx), "=f" (*__cosx) : "f" (__x));	\
}

__inline_functions (double, __CONCAT_d)
__inline_functions (float, __CONCAT_f)
__inline_functions (long double, __CONCAT_l)
# undef __inline_functions

#endif

#undef __CONCAT_d
#undef __CONCAT_f
#undef __CONCAT_l

#if !defined __NO_MATH_INLINES && defined __OPTIMIZE__

/* Define inline versions of the user visible functions.  */

# define __inline_forward_c(rettype, name, args1, args2)	\
__MATH_INLINE rettype __attribute__((__const__))		\
__NTH (name args1)						\
{								\
  return __CONCAT(__,name) args2;				\
}

# define __inline_forward(rettype, name, args1, args2)	\
__MATH_INLINE rettype __NTH (name args1)		\
{							\
  return __CONCAT(__,name) args2;			\
}

__inline_forward_c(double,floor, (double __x), (__x))
__inline_forward_c(double,ceil, (double __x), (__x))
# ifdef __USE_MISC
#  ifndef __USE_ISOC99 /* Conflict with macro of same name.  */
__inline_forward_c(int,isinf, (double __value), (__value))
#  endif
__inline_forward_c(int,finite, (double __value), (__value))
__inline_forward_c(double,scalbn, (double __x, int __n), (__x, __n))
# endif
# if defined __USE_MISC || defined __USE_XOPEN
#  ifndef __USE_ISOC99 /* Conflict with macro of same name.  */
__inline_forward_c(int,isnan, (double __value), (__value))
#  endif
# endif
# ifdef __USE_ISOC99
__inline_forward_c(double,scalbln, (double __x, long int __n), (__x, __n))
__inline_forward_c(double,nearbyint, (double __value), (__value))
__inline_forward_c(long int,lrint, (double __value), (__value))
# endif
# ifdef __USE_GNU
__inline_forward(void,sincos, (double __x, double *__sinx, double *__cosx),
		 (__x, __sinx, __cosx))
# endif

# ifdef __USE_ISOC99

__inline_forward_c(float,floorf, (float __x), (__x))
__inline_forward_c(float,ceilf, (float __x), (__x))
#  ifdef __USE_MISC
__inline_forward_c(int,isinff, (float __value), (__value))
__inline_forward_c(int,finitef, (float __value), (__value))
__inline_forward_c(float,scalbnf, (float __x, int __n), (__x, __n))
__inline_forward_c(int,isnanf, (float __value), (__value))
#  endif
# ifdef __USE_ISOC99
__inline_forward_c(float,scalblnf, (float __x, long int __n), (__x, __n))
__inline_forward_c(float,nearbyintf, (float __value), (__value))
__inline_forward_c(long int,lrintf, (float __value), (__value))
# endif
# ifdef __USE_GNU
__inline_forward(void,sincosf, (float __x, float *__sinx, float *__cosx),
		 (__x, __sinx, __cosx))
# endif

__inline_forward_c(long double,floorl, (long double __x), (__x))
__inline_forward_c(long double,ceill, (long double __x), (__x))
# ifdef __USE_MISC
__inline_forward_c(int,isinfl, (long double __value), (__value))
__inline_forward_c(int,finitel, (long double __value), (__value))
__inline_forward_c(long double,scalbnl, (long double __x, int __n), (__x, __n))
__inline_forward_c(int,isnanl, (long double __value), (__value))
# endif
# ifdef __USE_ISOC99
__inline_forward_c(long double,scalblnl, (long double __x, long int __n),
		   (__x, __n))
__inline_forward_c(long double,nearbyintl, (long double __value), (__value))
__inline_forward_c(long int,lrintl, (long double __value), (__value))
# endif
# ifdef __USE_GNU
__inline_forward(void,sincosl,
		 (long double __x, long double *__sinx, long double *__cosx),
		 (__x, __sinx, __cosx))
# endif

#endif /* Use misc or ISO C99 */

#undef __inline_forward
#undef __inline_forward_c

#endif /* !__NO_MATH_INLINES && __OPTIMIZE__ */

#endif
#endif	/* GCC.  */
