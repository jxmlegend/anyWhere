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
* Source file: Icon_HdmiVideoPlay_FF
* Dimensions:  20 * 19
* NumColors:   2
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

static GUI_CONST_STORAGE GUI_COLOR ColorsIcon_HdmiVideoPlay_FF[] = {
     0x101010,0xC3C3C3
};

static GUI_CONST_STORAGE GUI_LOGPALETTE PalIcon_HdmiVideoPlay_FF = {
  2,	/* number of entries */
  1, 	/* Has transparency */
  &ColorsIcon_HdmiVideoPlay_FF[0]
};

static GUI_CONST_STORAGE unsigned char acIcon_HdmiVideoPlay_FF[] = {
  ________, ________, ________,
  _X______, __X_____, ________,
  _XX_____, __XX____, ________,
  _XXX____, __XXX___, ________,
  _XXXX___, __XXXX__, ________,
  _XXXXX__, __XXXXX_, ________,
  _XXXXXX_, __XXXXXX, ________,
  _XXXXXXX, __XXXXXX, X_______,
  _XXXXXXX, X_XXXXXX, XX______,
  _XXXXXXX, XXXXXXXX, XXX_____,
  _XXXXXXX, X_XXXXXX, XX______,
  _XXXXXXX, __XXXXXX, X_______,
  _XXXXXX_, __XXXXXX, ________,
  _XXXXX__, __XXXXX_, ________,
  _XXXX___, __XXXX__, ________,
  _XXX____, __XXX___, ________,
  _XX_____, __XX____, ________,
  _X______, __X_____, ________,
  ________, ________, ________
};

GUI_CONST_STORAGE GUI_BITMAP bmIcon_HdmiVideoPlay_FF = {
  20, /* XSize */
  19, /* YSize */
  3, /* BytesPerLine */
  1, /* BitsPerPixel */
  acIcon_HdmiVideoPlay_FF,  /* Pointer to picture data (indices) */
  &PalIcon_HdmiVideoPlay_FF  /* Pointer to palette */
};

/* *** End of file *** */
