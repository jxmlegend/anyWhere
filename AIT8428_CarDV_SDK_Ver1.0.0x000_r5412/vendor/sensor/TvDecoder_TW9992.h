//==============================================================================
//
//  File        : TvDecoder_TW9992.h
//  Description : Firmware Sensor Control File
//  Author      :
//  Revision    : 1.0
//
//==============================================================================

#ifndef _TV_DEC_TW9992_H_
#define _TV_DEC_TW9992_H_

//==============================================================================
//
//                              INCLUDE FILE
//
//==============================================================================

#include "includes_fw.h"

//==============================================================================
//
//                              MACRO DEFINE
//
//==============================================================================

#ifndef TVDEC_SNR_IF
#define TVDEC_SNR_IF                (SENSOR_IF_MIPI_1_LANE)
#endif

#ifndef TVDEC_SNR_PROI
#define TVDEC_SNR_PROI              (SCD_SENSOR)
#endif

#ifndef TEST_PATTERN_EN
#define TEST_PATTERN_EN             (0)
#endif

#define RES_IDX_PAL_25FPS           (0) // PAL  //720x576, 50FPS interlace.
#define RES_IDX_NTSC_30FPS          (1) // NTSC //720x240, 60FPS interlace.

//==============================================================================
//
//                              MACRO DEFINE (Resolution For UI)
//
//==============================================================================

#if (TVDEC_SNR_PROI == PRM_SENSOR)

#ifndef SENSOR_DRIVER_MODE_NOT_SUUPORT
#define SENSOR_DRIVER_MODE_NOT_SUUPORT              (0xFFFF)
#endif

// Index 0
#define SENSOR_DRIVER_MODE_VGA_30P_RESOLUTION       (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 640*360 30P
#define SENSOR_DRIVER_MODE_VGA_50P_RESOLUTION       (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 640*360 50P
#define SENSOR_DRIVER_MODE_VGA_60P_RESOLUTION       (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 640*360 60P
#define SENSOR_DRIVER_MODE_VGA_100P_RESOLUTION      (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 640*360 100P
#define SENSOR_DRIVER_MODE_VGA_120P_RESOLUTION      (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 640*360 120P

// Index 5
#define SENSOR_DRIVER_MODE_HD_24P_RESOLUTION        (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1280*720 24P
#define SENSOR_DRIVER_MODE_HD_30P_RESOLUTION        (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1280*720 30P
#define SENSOR_DRIVER_MODE_HD_50P_RESOLUTION        (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1280*720 50P
#define SENSOR_DRIVER_MODE_HD_60P_RESOLUTION        (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1280*720 60P
#define SENSOR_DRIVER_MODE_HD_100P_RESOLUTION       (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1280*720 100P

// Index 10
#define SENSOR_DRIVER_MODE_HD_120P_RESOLUTION       (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1280*720 120P
#define SENSOR_DRIVER_MODE_1600x900_30P_RESOLUTION	(SENSOR_DRIVER_MODE_NOT_SUUPORT)	// 1600*900 30P
#define SENSOR_DRIVER_MODE_FULL_HD_15P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1920*1080 15P
#define SENSOR_DRIVER_MODE_FULL_HD_24P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1920*1080 24P
#define SENSOR_DRIVER_MODE_FULL_HD_25P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1920*1080 25P

// Index 15
#define SENSOR_DRIVER_MODE_FULL_HD_30P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1920*1080 30P
#define SENSOR_DRIVER_MODE_FULL_HD_50P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1920*1080 50P
#define SENSOR_DRIVER_MODE_FULL_HD_60P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1920*1080 60P
#define SENSOR_DRIVER_MODE_SUPER_HD_30P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2304*1296 30P
#define SENSOR_DRIVER_MODE_SUPER_HD_25P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2304*1296 25P

// Index 20
#define SENSOR_DRIVER_MODE_SUPER_HD_24P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2304*1296 24P
#define SENSOR_DRIVER_MODE_1440_30P_RESOLUTION  	(SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2560*1440 30P
#define SENSOR_DRIVER_MODE_2D7K_15P_RESOLUTION      (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2704*1524 15P
#define SENSOR_DRIVER_MODE_2D7K_30P_RESOLUTION      (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2704*1524 30P
#define SENSOR_DRIVER_MODE_4K2K_15P_RESOLUTION      (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 3840*2160 15P

// Index 25
#define SENSOR_DRIVER_MODE_4K2K_30P_RESOLUTION      (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 3840*2160 30P
#define SENSOR_DRIVER_MODE_4TO3_VGA_30P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 640*480   30P
#define SENSOR_DRIVER_MODE_4TO3_1D2M_30P_RESOLUTION (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1280*960  30P
#define SENSOR_DRIVER_MODE_4TO3_1D5M_30P_RESOLUTION (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 1440*1080 30P
#define SENSOR_DRIVER_MODE_4TO3_3M_15P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2016*1512 15P

// Index 30
#define SENSOR_DRIVER_MODE_4TO3_3M_30P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2016*1512 30P
#define SENSOR_DRIVER_MODE_4TO3_5M_15P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2560*1920 15P
#define SENSOR_DRIVER_MODE_4TO3_5M_30P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 2560*1920 30P
#define SENSOR_DRIVER_MODE_4TO3_8M_15P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 3264*2448 15P
#define SENSOR_DRIVER_MODE_4TO3_8M_30P_RESOLUTION   (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 3264*2448 30P

// Index 35
#define SENSOR_DRIVER_MODE_4TO3_10M_15P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 3648*2736 15P
#define SENSOR_DRIVER_MODE_4TO3_10M_30P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 3648*2736 30P
#define SENSOR_DRIVER_MODE_4TO3_12M_15P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 4032*3024 15P
#define SENSOR_DRIVER_MODE_4TO3_12M_30P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 4032*3024 30P
#define SENSOR_DRIVER_MODE_4TO3_14M_15P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 4352*3264 15P

// Index 40
#define SENSOR_DRIVER_MODE_4TO3_14M_30P_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)    // 4352*3264 30P
#define SENSOR_DRIVER_MODE_4K2K_24P_RESOLUTION      (SENSOR_DRIVER_MODE_NOT_SUUPORT)

// For Camera Preview
#if (LCD_MODEL_RATIO_X == 16) && (LCD_MODEL_RATIO_Y == 9)
#define SENSOR_DRIVER_MODE_BEST_CAMERA_PREVIEW_RESOLUTION       (SENSOR_DRIVER_MODE_NOT_SUUPORT)
#else
#define SENSOR_DRIVER_MODE_BEST_CAMERA_PREVIEW_RESOLUTION       (SENSOR_DRIVER_MODE_NOT_SUUPORT)
#endif

#define SENSOR_DRIVER_MODE_BEST_CAMERA_CAPTURE_16TO9_RESOLUTION (SENSOR_DRIVER_MODE_NOT_SUUPORT)
#define SENSOR_DRIVER_MODE_BEST_CAMERA_CAPTURE_4TO3_RESOLUTION  (SENSOR_DRIVER_MODE_NOT_SUUPORT)

#define SENSOR_DRIVER_MODE_FULL_HD_30P_RESOLUTION_HDR           (SENSOR_DRIVER_MODE_NOT_SUUPORT)

#define SENSOR_DRIVER_MODE_PAL_25P_RESOLUTION       (RES_IDX_PAL_25FPS)    
#define SENSOR_DRIVER_MODE_NTSC_30P_RESOLUTION      (RES_IDX_NTSC_30FPS)  

#else // #if (TVDEC_SNR_PROI == PRM_SENSOR)

#define SENSOR_DRIVER_MODE_PAL_25P_RESOLUTION       (RES_IDX_PAL_25FPS)  
#define SENSOR_DRIVER_MODE_NTSC_30P_RESOLUTION      (RES_IDX_NTSC_30FPS) 

#endif // #if (TVDEC_SNR_PROI == PRM_SENSOR)

#endif // _TV_DEC_TW9992_H_
