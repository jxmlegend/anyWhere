//==============================================================================
//
//  File        : StatePhotoPBFunc.h
//  Description : INCLUDE File for the StateCameraFunc function porting.
//  Author      : 
//  Revision    : 1.0
//
//==============================================================================

#ifndef _STATEPHOTOPBFUNC_H_
#define _STATEPHOTOPBFUNC_H_

/*===========================================================================
 * Include files
 *===========================================================================*/ 

#include "MenuCommon.h"

/*===========================================================================
 * Function prototype
 *===========================================================================*/

DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_KEY_UP);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_KEY_DOWN);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_KEY_LEFT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_KEY_RIGHT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_KEY_SET);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_JPGPB_TOUCH_RETURN);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_KEY_MENU);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_JPGPB_TOUCH_PREV_PRESS);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_JPGPB_TOUCH_NEXT_PRESS);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_VIDEO_PREVIEW);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_CAMERA_PREVIEW);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_FILE_DELETING);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_LOCK_FILE_M);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_DC_CABLE_IN);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_DC_CABLE_OUT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_USB_DETECT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_USB_REMOVED);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_SD_DETECT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_SD_REMOVED);
#if (HDMI_ENABLE)
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_HDMI_DETECT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_HDMI_REMOVED);
#endif
#if (TVOUT_ENABLE)
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_TV_DETECT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_TV_REMOVED);
#endif
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_JPGPB_UPDATE_MESSAGE);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_SUB_MODE_ENTER);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_SUB_MODE_EXIT);
DECL_AHC_EV_HANDLER(STATE_PHOTOPB_MODE_EVENT_ENTER_NET_PLAYBACK);


AHC_BOOL 	PhotoPBMode_Start(void);
void 		PhotoPBFunc_SetOSDShowStatus(AHC_BOOL state);
AHC_BOOL 	PhotoPBFunc_GetOSDShowStatus(void);
UINT32 StatePhotoPlaybackModeHandler(UINT32 ulMsgId, UINT32 ulEvent, UINT32 ulParam);
void 		StatePhotoPlaybackMode(UINT32 ulEvent);
AHC_BOOL 	PhotoPBTimer_Start(UINT32 ulTime);
AHC_BOOL 	PhotoPBTimer_Stop(void);
void 		PhotoPB_SDMMC_Change(void);

AHC_BOOL  StatePhotoPlaybackModeInitLCD(void* pData);
AHC_BOOL  StatePhotoPlaybackModeInitHDMI(void* pData);
AHC_BOOL  StatePhotoPlaybackModenitTV(void* pData);
AHC_BOOL  StatePhotoPlaybackModeShutDown(void* pData);

#endif //_STATEPHOTOPBFUNC_H_
