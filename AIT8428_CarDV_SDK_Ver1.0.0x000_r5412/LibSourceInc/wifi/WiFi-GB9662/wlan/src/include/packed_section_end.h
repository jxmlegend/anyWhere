/*
 * Declare directives for structure packing. No padding will be provided
 * between the members of packed structures, and therefore, there is no
 * guarantee that structure members will be aligned.
 *
 * Declaring packed structures is compiler specific. In order to handle all
 * cases, packed structures should be delared as:
 *
 * #include <packed_section_start.h>
 *
 * typedef BWL_PRE_PACKED_STRUCT struct foobar_t {
 *    some_struct_members;
 * } BWL_POST_PACKED_STRUCT foobar_t;
 *
 * #include <packed_section_end.h>
 *
 *
 * Copyright (C) 2011, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: packed_section_end.h,v 1.4 2008-12-09 23:43:22 Exp $
 */


/* Error check - BWL_PACKED_SECTION is defined in packed_section_start.h
 * and undefined in packed_section_end.h. If it is NOT defined at this
 * point, then there is a missing include of packed_section_start.h.
 */
#ifdef BWL_PACKED_SECTION
	#undef BWL_PACKED_SECTION
#else
	#error "BWL_PACKED_SECTION is NOT defined!"
#endif




/* Compiler-specific directives for structure packing are declared in
 * packed_section_start.h. This marks the end of the structure packing section,
 * so, undef them here.
 */
#undef	BWL_PRE_PACKED_STRUCT
#undef	BWL_POST_PACKED_STRUCT