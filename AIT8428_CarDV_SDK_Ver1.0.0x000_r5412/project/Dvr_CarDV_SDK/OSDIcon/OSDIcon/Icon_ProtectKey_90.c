/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*
* C-file generated by
*
*        �C/GUI-BitmapConvert V5.04.
*        Compiled Jan  8 2010, 16:46:31
*          (c) 2002-2005  Micrium, Inc.
  www.micrium.com

  (c) 1998-2005  Segger
  Microcontroller Systeme GmbH
  www.segger.com
*
**********************************************************************
*
* Source file: Icon_ProtectKey_90
* Dimensions:  26 * 20
* NumColors:   3
*
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*   Palette
The following are the entries of the palette table.
Every entry is a 32-bit value (of which 24 bits are actually used)
the lower   8 bits represent the Red component,
the middle  8 bits represent the Green component,
the highest 8 bits (of the 24 bits used) represent the Blue component
as follows:   0xBBGGRR
*/

static GUI_CONST_STORAGE GUI_COLOR ColorsIcon_ProtectKey_90[] = {
     0x000000,0x7F7F7F,0xC3C3C3
};

static GUI_CONST_STORAGE GUI_LOGPALETTE PalIcon_ProtectKey_90 = {
  3,	/* number of entries */
  1, 	/* Has transparency */
  &ColorsIcon_ProtectKey_90[0]
};

static GUI_CONST_STORAGE unsigned char acIcon_ProtectKey_90[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xAA, 0xAA, 0x80, 0x00, 0x00, 0x00,
  0x0A, 0x95, 0x55, 0xA0, 0x00, 0x00, 0x00,
  0x0A, 0x55, 0x55, 0x60, 0x00, 0x00, 0x00,
  0x29, 0x55, 0x55, 0x6A, 0xAA, 0xA0, 0x00,
  0x25, 0x55, 0x55, 0x55, 0x55, 0x68, 0x00,
  0x25, 0x55, 0x55, 0x58, 0x00, 0x1A, 0x00,
  0x25, 0x55, 0x55, 0x58, 0x00, 0x06, 0x80,
  0x25, 0x50, 0x05, 0x58, 0x00, 0x01, 0x80,
  0x25, 0x00, 0x01, 0x58, 0x00, 0x01, 0x80,
  0x25, 0x00, 0x01, 0x58, 0x00, 0x01, 0x80,
  0x25, 0x50, 0x05, 0x58, 0x00, 0x01, 0x80,
  0x25, 0x55, 0x55, 0x58, 0x00, 0x06, 0x80,
  0x25, 0x55, 0x55, 0x58, 0x00, 0x1A, 0x00,
  0x25, 0x55, 0x55, 0x55, 0x55, 0x68, 0x00,
  0x29, 0x55, 0x55, 0x6A, 0xAA, 0xA0, 0x00,
  0x0A, 0x55, 0x55, 0x60, 0x00, 0x00, 0x00,
  0x0A, 0x95, 0x55, 0xA0, 0x00, 0x00, 0x00,
  0x00, 0xAA, 0xAA, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GUI_CONST_STORAGE GUI_BITMAP bmIcon_ProtectKey_90 = {
  26, /* XSize */
  20, /* YSize */
  7, /* BytesPerLine */
  2, /* BitsPerPixel */
  acIcon_ProtectKey_90,  /* Pointer to picture data (indices) */
  &PalIcon_ProtectKey_90  /* Pointer to palette */
};

/* *** End of file *** */
