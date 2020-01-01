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
* Source file: Icon_ProtectKeyM
* Dimensions:  20 * 26
* NumColors:   16
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

static GUI_CONST_STORAGE GUI_COLOR ColorsIcon_ProtectKeyM[] = {
     0x030303,0x000080,0x008000,0x008080
    ,0x800000,0x800080,0x808000,0x808080
    ,0xC0C0C0,0x0000FF,0x00FF00,0x00FFFF
    ,0xFF0000,0xFF00FF,0xFFFF00,0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE PalIcon_ProtectKeyM = {
  16,	/* number of entries */
  1, 	/* Has transparency */
  &ColorsIcon_ProtectKeyM[0]
};

static GUI_CONST_STORAGE unsigned char acIcon_ProtectKeyM[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x88, 0x77, 0x77, 0x88, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x08, 0x87, 0x00, 0x00, 0x78, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x88, 0x70, 0x00, 0x00, 0x07, 0x88, 0x00, 0x00,
  0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x87, 0x88, 0x88, 0x88, 0x88, 0x78, 0x00, 0x00,
  0x00, 0x88, 0x87, 0x77, 0x77, 0x77, 0x77, 0x78, 0x88, 0x00,
  0x08, 0x87, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x78, 0x80,
  0x08, 0x77, 0x99, 0x97, 0x77, 0x77, 0x79, 0x99, 0x77, 0x80,
  0x08, 0x77, 0x79, 0x99, 0x70, 0x07, 0x99, 0x97, 0x77, 0x80,
  0x08, 0x77, 0x79, 0x99, 0x00, 0x00, 0x99, 0x97, 0x77, 0x80,
  0x08, 0x77, 0x79, 0x99, 0x90, 0x09, 0x99, 0x97, 0x77, 0x80,
  0x08, 0x77, 0x79, 0x79, 0x90, 0x09, 0x79, 0x97, 0x77, 0x80,
  0x08, 0x77, 0x79, 0x79, 0x90, 0x99, 0x79, 0x97, 0x77, 0x80,
  0x08, 0x77, 0x79, 0x79, 0x99, 0x97, 0x79, 0x97, 0x77, 0x80,
  0x08, 0x87, 0x79, 0x77, 0x99, 0x97, 0x79, 0x97, 0x78, 0x80,
  0x00, 0x88, 0x79, 0x77, 0x99, 0x77, 0x79, 0x97, 0x88, 0x00,
  0x00, 0x89, 0x99, 0x97, 0x79, 0x77, 0x99, 0x99, 0x88, 0x00,
  0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GUI_CONST_STORAGE GUI_BITMAP bmIcon_ProtectKeyM = {
  20, /* XSize */
  26, /* YSize */
  10, /* BytesPerLine */
  4, /* BitsPerPixel */
  acIcon_ProtectKeyM,  /* Pointer to picture data (indices) */
  &PalIcon_ProtectKeyM  /* Pointer to palette */
};

/* *** End of file *** */
