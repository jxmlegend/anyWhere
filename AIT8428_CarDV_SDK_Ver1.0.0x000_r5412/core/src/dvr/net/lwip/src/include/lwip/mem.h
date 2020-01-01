/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIP_MEM_H__
#define __LWIP_MEM_H__

#include "lwip/opt.h"

#ifdef __cplusplus
extern "C" {
#endif

#if MEM_LIBC_MALLOC

#include <stddef.h> /* for size_t */

typedef size_t mem_size_t;
#define MEM_SIZE_F SZT_F

/* aliases for C library malloc() */
#define mem_init()
/* in case C library malloc() needs extra protection,
 * allow these defines to be overridden.
 */
#ifndef mem_free
#define mem_free free
#endif
#ifndef mem_malloc
#define mem_malloc malloc
#endif
#ifndef mem_calloc
#define mem_calloc calloc
#endif
/* Since there is no C library allocation function to shrink memory without
   moving it, define this to nothing. */
#ifndef mem_trim
#define mem_trim(mem, size) (mem)
#endif
#else /* MEM_LIBC_MALLOC */

/* MEM_SIZE would have to be aligned, but using 64000 here instead of
 * 65535 leaves some room for alignment...
 */
#if MEM_SIZE > 64000L
typedef u32_t mem_size_t;
#define MEM_SIZE_F U32_F
#else
typedef u16_t mem_size_t;
#define MEM_SIZE_F U16_F
#endif /* MEM_SIZE > 64000 */

#if MEM_USE_POOLS
/** mem_init is not used when using pools instead of a heap */
#define mem_init()
/** mem_trim is not used when using pools instead of a heap:
    we can't free part of a pool element and don't want to copy the rest */
#define mem_trim(mem, size) (mem)
#else /* MEM_USE_POOLS */
/* lwIP alternative malloc */

//debug level: 0:none 1: allocated and free task priority. 2: function name and lines
#define LWIP_DEBUG_LEVEL 0

bool  mem_is_ready(void);
void  mem_init(void);
void *mem_trim(void *mem, mem_size_t size);
#endif /* MEM_USE_POOLS */
#if LWIP_DEBUG_LEVEL >= 2
void *_mem_malloc(mem_size_t size, char* func_name, unsigned short line);
void *_mem_calloc(mem_size_t count, mem_size_t size, char* func_name, unsigned short line);
void  _mem_free(void *mem, char* func_name, unsigned short line);
#ifndef MEM_C
#define mem_malloc(size) _mem_malloc(size, __prettyfunc__, __LINE__)
#define mem_calloc(count, size) _mem_calloc(count, size, __prettyfunc__, __LINE__)
#define mem_free(size) _mem_free(size, __prettyfunc__, __LINE__)
#endif
#else
void *mem_malloc(mem_size_t size);
void *mem_calloc(mem_size_t count, mem_size_t size);
void  mem_free(void *mem);
#endif
#if LWIP_DEBUG_LEVEL >= 1
bool mem_is_leagal(void* rmem);
#else
#define mem_is_leagal(rmem) (true)
#endif
#endif /* MEM_LIBC_MALLOC */
bool mem_snapshot(bool reset);
bool mem_check(bool print);
/*
 * The MEM_ALIGNMENT_HEAP must be align with size of cache line, for ARM 926EJS the value is 32,
 * If it is not, after the function osal_invalidate_dcache(..) will cause
 * some value in D-CACHE will not write back to physical RAM
 */
#define	MEM_ALIGNMENT_HEAP	32	
/** Calculate memory size for an aligned buffer - returns the next highest
 * multiple of MEM_ALIGNMENT_HEAP (e.g. LWIP_MEM_ALIGN_SIZE(3) and
 * LWIP_MEM_ALIGN_SIZE(4) will both yield 4 for MEM_ALIGNMENT_HEAP == 4).
 */
#ifndef LWIP_MEM_ALIGN_SIZE
#define LWIP_MEM_ALIGN_SIZE(size) (((size) + MEM_ALIGNMENT_HEAP - 1) & ~(MEM_ALIGNMENT_HEAP-1))
#endif

/** Calculate safe memory size for an aligned buffer when using an unaligned
 * type as storage. This includes a safety-margin on (MEM_ALIGNMENT_HEAP - 1) at the
 * start (e.g. if buffer is u8_t[] and actual data will be u32_t*)
 */
#ifndef LWIP_MEM_ALIGN_BUFFER
#define LWIP_MEM_ALIGN_BUFFER(size) (((size) + MEM_ALIGNMENT_HEAP - 1))
#endif

/** Align a memory pointer to the alignment defined by MEM_ALIGNMENT_HEAP
 * so that ADDR % MEM_ALIGNMENT_HEAP == 0
 */
#ifndef LWIP_MEM_ALIGN
#define LWIP_MEM_ALIGN(addr) ((void *)(((mem_ptr_t)(addr) + MEM_ALIGNMENT_HEAP - 1) & ~(mem_ptr_t)(MEM_ALIGNMENT_HEAP-1)))
#endif

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_MEM_H__ */
