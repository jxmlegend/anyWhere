/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*
* C-file generated by
*
*        �C/GUI-BitmapConvert V3.96.
*        Compiled Jul 19 2005, 13:50:35
*          (c) 2002-2005  Micrium, Inc.
  www.micrium.com

  (c) 1998-2005  Segger
  Microcontroller Systeme GmbH
  www.segger.com
*
**********************************************************************
*
* Source file: Icon_KeyMap_OK
* Dimensions:  80 * 32
* NumColors:   3
*
**********************************************************************
*/

#include "stdlib.h"

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

static GUI_CONST_STORAGE GUI_COLOR ColorsIcon_KeyMap_OK[] = {
     0xFFFFFF,0x808080,0x101010
};

static GUI_CONST_STORAGE GUI_LOGPALETTE PalIcon_KeyMap_OK = {
  3,	/* number of entries */
  0, 	/* No transparency */
  &ColorsIcon_KeyMap_OK[0]
};

static GUI_CONST_STORAGE unsigned char acIcon_KeyMap_OK[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x05, 0x55, 0x54, 0x05, 0x50, 0x05, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x50, 0x00, 0x00, 0x55, 0x54, 0x0A, 0x40, 0x2A, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x40, 0x2A, 0x80, 0x29, 0x54, 0x0A, 0x00, 0xA9, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0xA9, 0x50, 0x0A, 0x54, 0x08, 0x02, 0xA5, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x02, 0xA5, 0x54, 0x0A, 0x54, 0x00, 0x0A, 0x95, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x02, 0x95, 0x54, 0x0A, 0x54, 0x00, 0x0A, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x02, 0x95, 0x54, 0x0A, 0x54, 0x02, 0x01, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0xA5, 0x50, 0x0A, 0x54, 0x0A, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x40, 0x25, 0x40, 0x2A, 0x54, 0x0A, 0x40, 0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x50, 0x00, 0x00, 0xAA, 0x54, 0x0A, 0x50, 0x05, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x02, 0xA9, 0x54, 0x0A, 0x54, 0x01, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x5A, 0xAA, 0x95, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x56, 0xA9, 0x55, 0x55, 0x6A, 0x55, 0xA9, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GUI_CONST_STORAGE GUI_BITMAP bmIcon_KeyMap_OK = {
  80, /* XSize */
  32, /* YSize */
  20, /* BytesPerLine */
  2, /* BitsPerPixel */
  acIcon_KeyMap_OK,  /* Pointer to picture data (indices) */
  &PalIcon_KeyMap_OK  /* Pointer to palette */
};

/* *** End of file *** */
