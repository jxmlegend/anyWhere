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
* Source file: Icon_Delete
* Dimensions:  24 * 32
* NumColors:   11
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

static GUI_CONST_STORAGE GUI_COLOR ColorsIcon_Delete[] = {
     0xFF00FF,0xFFFFFF,0x030303,0xFFFFFF
    ,0x030303,0xFE00FE,0xFEFEFE,0xFFFFFF
    ,0x030303,0xFFFFFF,0x000000
};

static GUI_CONST_STORAGE GUI_LOGPALETTE PalIcon_Delete = {
  11,	/* number of entries */
  1, 	/* Has transparency */
  &ColorsIcon_Delete[0]
};

static GUI_CONST_STORAGE unsigned char acIcon_Delete[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x22, 0x22, 0x21, 0x11, 0x22, 0x22, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x02, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x00, 0x00,
  0x00, 0x00, 0x02, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x00, 0x00,
  0x00, 0x00, 0x02, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x00, 0x00,
  0x00, 0x00, 0x02, 0x11, 0x22, 0x22, 0x82, 0x22, 0x21, 0x12, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x22, 0x22, 0x82, 0x22, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x21, 0x21, 0x81, 0x21, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x22, 0x22, 0x82, 0x22, 0x21, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x11, 0x11, 0x11, 0x11, 0x11, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x81, 0x11, 0x11, 0x11, 0x11, 0x11, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x82, 0x22, 0x22, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GUI_CONST_STORAGE GUI_BITMAP bmIcon_Delete = {
  24, /* XSize */
  32, /* YSize */
  12, /* BytesPerLine */
  4, /* BitsPerPixel */
  acIcon_Delete,  /* Pointer to picture data (indices) */
  &PalIcon_Delete  /* Pointer to palette */
};

/* *** End of file *** */
