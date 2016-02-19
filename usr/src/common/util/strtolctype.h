/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2016 Gary Mills
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

#ifndef	_COMMON_UTIL_STRTOLCTYPE_H
#define	_COMMON_UTIL_STRTOLCTYPE_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * This is a private header file containing a collection of macros that
 * the strtou?ll? functions in common/util use to test characters.  Which
 * macros are defined depends on which of _KERNEL or _BOOT are defined.
 * New code should use the kernel version of ctype.h: <sys/ctype.h>.
 *
 * NOTE: These macros are used within several DTrace probe context functions.
 * They must not be altered to make function calls or perform actions not
 * safe in probe context.
 */

/*
 * Cases that define these macros:
 *   _KERNEL && !_BOOT: Used by kernel functions
 *   !_KERNEL && _BOOT: Used by dboot_startkern.c for x86 boot
 * Cases that omit these macros:
 *   _KERNEL && _BOOT: Used by common/util/strtol.c for SPARC boot library
 *   !_KERNEL && !_BOOT: Used by strtou?ll? functions in libc
 */
#if	defined(_KERNEL) ^ defined(_BOOT)

#define	isalnum(ch)	(isalpha(ch) || isdigit(ch))
#define	isalpha(ch)	(isupper(ch) || islower(ch))
#define	isdigit(ch)	((ch) >= '0' && (ch) <= '9')
#define	islower(ch)	((ch) >= 'a' && (ch) <= 'z')
#define	isspace(ch)	(((ch) == ' ') || ((ch) == '\r') || ((ch) == '\n') || \
			((ch) == '\t') || ((ch) == '\f'))
#define	isupper(ch)	((ch) >= 'A' && (ch) <= 'Z')
#define	isxdigit(ch)	(isdigit(ch) || ((ch) >= 'a' && (ch) <= 'f') || \
			((ch) >= 'A' && (ch) <= 'F'))

#endif	/* _KERNEL ^ _BOOT */

/*
 * The following macros are defined unconditionally.
 */

#define	DIGIT(x)	\
	(isdigit(x) ? (x) - '0' : islower(x) ? (x) + 10 - 'a' : (x) + 10 - 'A')

#define	MBASE	('z' - 'a' + 1 + 10)

/*
 * The following macro is a version of isalnum() that limits alphabetic
 * characters to the ranges a-z and A-Z; locale dependent characters will not
 * return 1.  The members of a-z and A-Z are assumed to be in ascending order
 * and contiguous.
 */
#define	lisalnum(x)	\
	(isdigit(x) || ((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z'))

#ifdef	__cplusplus
}
#endif

#endif	/* _COMMON_UTIL_STRTOLCTYPE_H */
