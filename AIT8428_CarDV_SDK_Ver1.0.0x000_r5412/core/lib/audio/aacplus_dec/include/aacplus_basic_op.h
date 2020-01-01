/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |                                                                           |
 | $Id: basic_op.h,v 1.8 2005/02/20 12:07:26 ehr Exp $
 |___________________________________________________________________________|
*/

#ifndef __BASIC_OP_H
#define __BASIC_OP_H


#include "mmpf_audio_typedef.h"
//
//extern Flag Overflow;
//extern Flag Carry;

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for basic arithmetic operators                               |
 |___________________________________________________________________________|
*/

//Word16 add (Word16 var1, Word16 var2);    /* Short add,           1   */
//Word16 sub (Word16 var1, Word16 var2);    /* Short sub,           1   */
//Word16 abs_s (Word16 var1);               /* Short abs,           1   */
//Word16 shl (Word16 var1, Word16 var2);    /* Short shift left,    1   */
//Word16 shr (Word16 var1, Word16 var2);    /* Short shift right,   1   */
//Word16 mult (Word16 var1, Word16 var2);   /* Short mult,          1   */
//Word32 L_mult (Word16 var1, Word16 var2); /* Long mult,           1   */
//Word16 negate (Word16 var1);              /* Short negate,        1   */
//Word16 extract_h (Word32 L_var1);         /* Extract high,        1   */
//Word16 extract_l (Word32 L_var1);         /* Extract low,         1   */
//Word16 round (Word32 L_var1);             /* Round,               1   */
//Word32 L_mac (Word32 L_var3, Word16 var1, Word16 var2);   /* Mac,  1  */
//Word32 L_msu (Word32 L_var3, Word16 var1, Word16 var2);   /* Msu,  1  */
//Word32 L_macNs (Word32 L_var3, Word16 var1, Word16 var2); /* Mac without
//                                                             sat, 1   */
// L_msuNs (Word32 L_var3, Word16 var1, Word16 var2); /* Msu without
//                                                             sat, 1   */
Word32 L_add (Word32 L_var1, Word32 L_var2);    /* Long add,        2 */
Word32 L_sub (Word32 L_var1, Word32 L_var2);    /* Long sub,        2 */
//Word32 L_add_c (Word32 L_var1, Word32 L_var2);  /* Long add with c, 2 */
//Word32 L_sub_c (Word32 L_var1, Word32 L_var2);  /* Long sub with c, 2 */
//Word32 L_negate (Word32 L_var1);                /* Long negate,     2 */
// mult_r (Word16 var1, Word16 var2);       /* Mult with round, 2 */
//Word32 L_shl (Word32 L_var1, Word16 var2);      /* Long shift left, 2 */
Word32 L_shr (Word32 L_var1, Word16 var2);      /* Long shift right, 2*/
//Word16 shr_r (Word16 var1, Word16 var2);        /* Shift right with
//                                                   round, 2           */
//Word16 mac_r (Word32 L_var3, Word16 var1, Word16 var2); /* Mac with
//                                                           rounding,2 */
//Word16 msu_r (Word32 L_var3, Word16 var1, Word16 var2); /* Msu with
//                                                           rounding,2 */
//Word32 L_deposit_h (Word16 var1);        /* 16 bit var1 -> MSB,     2 */
// L_deposit_l (Word16 var1);        /* 16 bit var1 -> LSB,     2 */

//Word32 L_shr_r (Word32 L_var1, Word16 var2); /* Long shift right with
//                                                round,  3             */
//Word32 L_abs (Word32 L_var1);            /* Long abs,              3  */
//Word32 L_sat (Word32 L_var1);            /* Long saturation,       4  */
//Word16 norm_s (Word16 var1);             /* Short norm,           15  */
extern Word16 div_s (Word16 var1, Word16 var2); /* Short division,       18  */
//Word16 norm_l (Word32 L_var1);           /* Long norm,            30  */

#ifndef ARMDSP_WORD32_DEFINED
#define ARMDSP_WORD32_DEFINED
#define ARMDSP_WORD32_MAX ((Word32)0x7FFFFFFF)
#define ARMDSP_WORD32_MIN ((Word32)-0x80000000)
#define ARMDSP_WORD16_MAX ((Word16)0x7FFF)
#define ARMDSP_WORD16_MIN ((Word16)-0x8000)
#endif

/*
 * This global variable needs to exist somewhere in the compiled
 * program in order for the flag-using functions to work. You can
 * either include the declaration
 *
 *   armdsp_flagdata_union armdsp_flagdata;
 *
 * in at least one of your source files (that includes this
 * header), or compile in "dspfns.c" from the examples\dsp
 * directory.
 */
typedef union {
    unsigned int armdsp_flags_word;
    struct {
#ifdef __BIG_ENDIAN
        Flag armdsp_n:1, armdsp_z:1, armdsp_c:1, armdsp_v:1, armdsp_q:1, armdsp_unused:27;
#else
        Flag armdsp_unused:27, armdsp_q:1, armdsp_v:1, armdsp_c:1, armdsp_z:1, armdsp_n:1;
#endif
    } armdsp_bitfields;
} armdsp_flagdata_union;
extern armdsp_flagdata_union armdsp_flagdata;


#define Carry ( armdsp_flagdata.armdsp_bitfields.armdsp_c )
#define Overflow ( armdsp_flagdata.armdsp_bitfields.armdsp_q )

/*
 * Convert a 32-bit signed integer into a 16-bit signed integer by
 * saturation.
 */
__inline Word16 saturate(Word32 x)
{
    if (x > ARMDSP_WORD16_MAX)
        Overflow = 1, x = ARMDSP_WORD16_MAX;
    else if (x < ARMDSP_WORD16_MIN)
        Overflow = 1, x = ARMDSP_WORD16_MIN;
    return (Word16) x;
}

__inline Word16  add(Word16 x, Word16 y)
{
//    Word32 xs, ys, rs;
//    __asm {
//        mov xs, x, lsl #16;
//        mov ys, y, lsl #16;
//        qadd rs, xs, ys;
//
//    }
//
//    return (Word16) (rs >> 16);
        Word16 var_out;
    Word32 L_sum;

    L_sum = (Word32) x + y;
    var_out = saturate (L_sum);

    return (var_out);
}





/*
 * Subtract one 32-bit signed integer from another with saturation.
 */
__inline Word32 L_sub (Word32 L_var1, Word32 L_var2)
{
    Word32 L_var_out;

    L_var_out = L_var1 - L_var2;

    if (((L_var1 ^ L_var2) & MIN_32) != 0)
    {
        if ((L_var_out ^ L_var1) & MIN_32)
        {
            L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
            Overflow = 1;
        }
    }

    return (L_var_out);
}
/*
 * Add together the Carry variable and two 32-bit signed integers,
 * without saturation.
 */
//__inline Word32 L_add_c(Word32 x, Word32 y)
//{
//    Word32 result, flags;
//
//    __asm {
//        msr CPSR_f, armdsp_flagdata.armdsp_flags_word;
//        adcs result, x, y;
//        mrs flags, CPSR;
//        orrvs flags, flags, #0x08000000; // set Q if V is set
//        mov armdsp_flagdata.armdsp_flags_word, flags;
//    }
//    return result;
//}

/*
 * Subtract one 32-bit signed integer, together with the Carry
 * variable, from another 32-bit signed integer, without
 * saturation.
 */
//__inline Word32 L_sub_c(Word32 x, Word32 y)
//{
//    Word32 result, flags;
//
//    __asm {
//        msr CPSR_f, armdsp_flagdata.armdsp_flags_word;
//        sbcs result, x, y;
//        mrs flags, CPSR;
//        orrvs flags, flags, #0x08000000; // set Q if V is set
//        mov armdsp_flagdata.armdsp_flags_word, flags;
//    }
//    return result;
//}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one _with_ saturation, and add
 * with carry to another 32-bit integer _without_ saturation.
 */
//__inline Word32 L_macNs(Word32 accumulator, Word16 x, Word16 y)
//{
//    return L_add_c(accumulator, L_mult(x, y));
//}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one _with_ saturation, and
 * subtract with carry from another 32-bit integer _without_
 * saturation.
 */
//__inline Word32 L_msuNs(Word32 accumulator, Word16 x, Word16 y)
//{
//    return L_sub_c(accumulator, L_mult(x, y));
//}
/*
 * Negate a 32-bit signed integer, with saturation. (Saturation is
 * only necessary when the input is -0x80000000.)
 */
__inline Word32 L_negate(Word32 x)
{
    if (x == ARMDSP_WORD32_MIN)
        return ARMDSP_WORD32_MAX;
    return -x;
}

/*
 * Multiply two 16-bit signed integers, shift the result right by
 * 15 with rounding, and saturate it. (Saturation is only necessary
 * if both inputs were -0x8000, in which case the result "should"
 * be 0x8000 and is saturated to 0x7FFF.)
 */
__inline Word16 mult_r (Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_product_arr;

    L_product_arr = (Word32) var1 *(Word32) var2;       /* product */
    L_product_arr += (Word32) 0x00004000L;      /* round */
    L_product_arr &= (Word32) 0xffff8000L;
    L_product_arr >>= 15;       /* shift */

    if (L_product_arr & (Word32) 0x00010000L)   /* sign extend when necessary */
    {
        L_product_arr |= (Word32) 0xffff0000L;
    }
    var_out = saturate (L_product_arr);

    return (var_out);
}

/*
 * Shift a 32-bit signed integer left (or right, if the shift count
 * is negative). Saturate on overflow.
 */
__inline Word32 L_shl (Word32 L_var1, Word16 var2)
{
    Word32 L_var_out;

    if (var2 <= 0)
    {
        if (var2 < -32)
            var2 = -32;
        L_var_out = L_shr (L_var1, -var2);

    }
    else
    {
        for (; var2 > 0; var2--)
        {
            if (L_var1 > (Word32) 0X3fffffffL)
            {
                Overflow = 1;
                L_var_out = MAX_32;
                break;
            }
            else
            {
                if (L_var1 < (Word32) 0xc0000000L)
                {
                    Overflow = 1;
                    L_var_out = MIN_32;
                    break;
                }
            }
            L_var1 *= 2;
            L_var_out = L_var1;
        }
    }

    return (L_var_out);
}
/*
 * Shift a 32-bit signed integer right (or left, if the shift count
 * is negative). Saturate on overflow.
 */
__inline Word32 L_shr (Word32 L_var1, Word16 var2)
{
    Word32 L_var_out;

    if (var2 < 0)
    {
        L_var_out = L_shl (L_var1, -var2);

    }
    else
    {
//        if (var2 >= 31)
//        {
//            L_var_out = (L_var1 < 0L) ? -1 : 0;
//        }
//        else
//        {
//            if (L_var1 < 0)
//            {
//                L_var_out = ~((~L_var1) >> var2);
//            }
//            else
//            {
                L_var_out = L_var1 >> var2;
//            }
//        }

    }

    return (L_var_out);
}


/*
 * Return a saturated value appropriate to the most recent carry-
 * affecting operation (L_add_c, L_macNs, L_sub_c, L_msuNs).
 *
 * In other words: return the argument if the Q flag is clear.
 * Otherwise, return -0x80000000 or +0x7FFFFFFF depending on
 * whether the Carry flag is set or clear (respectively).
 */
//__inline Word32 L_sat(Word32 x)
//{
//    int qflag;
//    __asm {
//        mrs qflag, CPSR;
//    }
//    if (!(qflag & 0x08000000))
//        return x;
//    if (Carry)
//        return ARMDSP_WORD32_MIN;
//    else
//        return ARMDSP_WORD32_MAX;
//}

/*
 * Return the number of bits of left shift needed to arrange for a
 * 16-bit signed integer to have value >= 0x4000 or <= -0x4001.
 *
 * Returns zero if x is zero.
 */
__inline Word16 norm_s (Word16 var1)
{
    Word16 var_out;

    if (var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (var1 == (Word16) 0xffff)
        {
            var_out = 15;
        }
        else
        {
            if (var1 < 0)
            {
                var1 = ~var1;
            }
            for (var_out = 0; var1 < 0x4000; var_out++)
            {
                var1 <<= 1;
            }
        }
    }
    return (var_out);
}


/*
 * Return the number of bits of left shift needed to arrange for a
 * 32-bit signed integer to have value >= 0x40000000 (if
 *
 * Returns zero if x is zero.
 */
__inline Word16 norm_l (Word32 L_var1)
{
    Word16 var_out;

    if (L_var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (L_var1 == (Word32) 0xffffffffL)
        {
            var_out = 31;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var1 = ~L_var1;
            }
            for (var_out = 0; L_var1 < (Word32) 0x40000000L; var_out++)
            {
                L_var1 <<= 1;
            }
        }
    }


    return (var_out);
}

#if 0
__inline Word32 fixmul_32x16b (const Word32 a, const Word16 b)
{
  Word32 rH;

    __asm
        {
               smulwb rH,a,b;
               qadd    rH,rH,rH;
      }

  return rH;
}

__inline void
butterfly32a_16( Word32 ro, Word32 io, Word16 rwh, Word16 iwh, Word32 *rx, Word32 *ix )
{
  Word32 acc,temp1;
  __asm{

//
//  acc = L_mult(roh, rwh);
//  acc = L_msu(acc, ioh, iwh);
//  acc = L_mac(acc, mult(rol, rwh), 1);
//  acc = L_msu(acc, mult(iol, iwh), 1);
//  *rx = acc;
       smulwb acc,ro,rwh;
       smulwb temp1,io,iwh;
       qsub   acc,acc,temp1;
       qadd acc,acc,acc;
       mov  *rx,acc

//  acc = L_mult(roh, iwh);
//  acc = L_mac(acc, ioh, rwh);
//  acc = L_mac(acc, mult(rol, iwh), 1);
//  acc = L_mac(acc, mult(iol, rwh), 1);
//  *ix = acc;
       smulwb acc,ro,iwh;
       smlawb acc,io,rwh,acc;
       qadd acc,acc,acc;
       mov  *ix,acc
  }
}

__inline void
butterfly32b_16( Word32 ro, Word32 io, Word16 ch, Word16 sh, Word32 *rx, Word32 *ix )
{
    Word32 acc,temp1;

//
//  acc = (roh* ch);
//  acc = acc+ (ioh* sh);
//  acc = acc+ ((rol* ch)>>15);
//  acc = acc+ ((iol* sh)>>15);
//  acc = acc <<1;
//  *rx = acc;
   __asm{

       smulwb acc,ro,ch;
       smlawb acc,io,sh,acc;
       qadd acc,acc,acc;
       mov  *rx,acc

//  acc = (ioh* ch);
//  acc = acc- (roh* sh);
//  acc = acc+((iol* ch)>>15);
//  acc = acc- ((rol* sh)>>15);
//  acc =acc<<1;
//
//  *ix = acc;
       smulwb acc,io,ch;
       smulwb temp1,ro,sh;
       qsub   acc,acc,temp1;
       qadd acc,acc,acc;
       mov  *ix,acc

  }

}
__inline Word32 fixmuldiv2_32x16b (const Word32 a, const Word16 b)
{
        Word32 rH;
    __asm
        {
               smulwb rH,a,b;
      }
  return rH;
}

__inline Word32 fixmul_32x16t (const Word32 a, const Word32 b)
{

    Word32 rH;
    __asm
        {
               smulwt rH,a,b;
               qadd   rH,rH,rH;
      }
  return rH;
}

__inline Word32 fixmuldiv2_32x16t (const Word32 a, const Word32 b)
{
    Word32 rH;
    __asm
        {
               smulwt rH,a,b;
      }
  return rH;
}

__inline void fixmadd_32x16t (Word32 *a, const Word32 x, const Word32 y)
{
    __asm
        {
               smlawt *a,x,y,*a;
      }

}

__inline Word32 fixmul (const Word32 a, const Word32 b)
{
  Word32 RdLo,RdHi;
    __asm
        {
               smull RdLo,RdHi,a,b;
             qadd  RdHi,RdHi,RdHi;
      }
  return RdHi;
}

__inline Word32 fixmuldiv2 (const Word32 a, const Word32 b)
{
  Word32 RdLo,RdHi;
    __asm
        {
               smull RdLo,RdHi,a,b;

      }
     return RdHi;
}

__inline void fixmadd (Word32 *a, const Word32 x, const Word32 y)
{
    Word32 RdLo=0;

    __asm
        {
               smlal RdLo,*a,x,y;

      }

}
__inline Word16 ffr_norm32(Word32 x)
{
    int lz;

    x ^= (x >> 31);                    /* invert x if it's negative */
    __asm {
        clz lz, x;
    }
    return (Word16) (lz - 1);
}
#endif
#ifdef ARM_OPT
__inline Word16 S_max( Word16 a, Word16 b ) {


  if ( a<b )
    return b;
  else
    return a;
}

__inline Word32 L_max( Word32 a, Word32 b ) {

  if ( a<b )
    return b;
  else
    return a;
}


__inline Word16 S_min( Word16 a, Word16 b ) {


  if ( a<b )
    return a;
  else
    return b;
}

__inline Word32 L_min( Word32 a, Word32 b ) {

  if ( a<b )
    return a;
  else
    return b;
}

#endif

void memcpy32(void *des,void *src,unsigned int length);
#endif
