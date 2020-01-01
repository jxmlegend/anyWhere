//------------------------------------------------------------------------------
//
//  File        : mdtc_cfg.h
//  Description : Header file of motion detection configuration
//  Author      : Alterman
//  Revision    : 0.0
//
//------------------------------------------------------------------------------
#ifndef _MDTC_CFG_H_
#define _MDTC_CFG_H_

//==============================================================================
//
//                              INCLUDE FILE
//
//==============================================================================

#include "md.h"

//==============================================================================
//
//                              CONSTANTS
//
//==============================================================================

/* Width of source image */
#define MDTC_WIDTH              (160)
/* Heigth of source image */
#define MDTC_HEIGHT             (90)

/* Width of rear source image */
#define REAR_MDTC_WIDTH              (160)
/* Heigth of rear source image */
#define REAR_MDTC_HEIGHT             (120)

/* The number of divisions of window in horizontal direction */
#define MDTC_WIN_W_DIV          (1)
/* The number of divisions of window in vertical direction */
#define MDTC_WIN_H_DIV          (1)
/* The gap of every two frames to do motion detection */
#define MDTC_FRAME_GAP          (0)
/* Color format of source image */
#define MDTC_COLOR_Y            (1)

#define UVC_DEV_MD_MAX_DIV_W            (16)
#define UVC_DEV_MD_MAX_DIV_H            (12)
#define UVC_DEV_MD_MAX_SENSITIVITY      (40)

#define UVC_DEV_MD_DIV_W                (5)
#define UVC_DEV_MD_DIV_H                (5)
#define UVC_DEV_MD_SENSITIVITY          (17)

//==============================================================================
//
//                              STRUCTURES
//
//==============================================================================

typedef struct {
	unsigned int 	width;     // Width of source image
	unsigned int 	height;    // Height of source image
	unsigned char  	FrameGap;
	unsigned char  	CpuX;
    unsigned short 	x_lt;    // Horizontal position of window left-top point
    unsigned short 	y_lt;    // Vertical position of window left-top point
    unsigned short 	x_rb;    // Horizontal position of window right-bottom point
    unsigned short 	y_rb;    // Vertical position of window right-bottom point
    unsigned char  	x_div;   // The number of divisions of window in X direction
    unsigned char  	y_div;   // The number of divisions of window in Y direction
} MDTC_CFG;

//==============================================================================
//
//                              EXTERN VARIABLES
//
//==============================================================================

extern MDTC_CFG     	gstMdtcCfg[2];
extern MD_params_in_t   gstMdtcWinParam[MDTC_WIN_W_DIV][MDTC_WIN_H_DIV];
extern MD_params_out_t  gstMdtcWinResult[MDTC_WIN_W_DIV][MDTC_WIN_H_DIV];

#endif // _MDTC_CFG_H_

