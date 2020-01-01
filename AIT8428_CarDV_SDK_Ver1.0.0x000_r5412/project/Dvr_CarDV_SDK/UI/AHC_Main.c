/*===========================================================================
 * Include file 
 *===========================================================================*/ 

#include "customer_config.h" 
#include "AHC_Common.h"
#include "AHC_Capture.h"
#include "AHC_General.h"
#include "AHC_Message.h"
#include "AHC_Sensor.h"
#include "AHC_Utility.h"
#include "AHC_GUI.h"
#include "AHC_Os.h"
#include "AHC_Menu.h"
#include "AHC_Version.h"
#include "AHC_Fs.h"
#include "AHC_Media.h"
#include "AHC_Parameter.h"
#include "AHC_Warningmsg.h"
#include "AHC_Display.h"
#include "AHC_General_CarDV.h"
#include "AHC_Video.h"
#include "AHC_USB.h"
#include "LedControl.h"
#include "ZoomControl.h"
#include "StateCameraFunc.h"
#include "dsc_charger.h"
#if (SUPPORT_GSENSOR) 
#include "GSensor_ctrl.h"
#endif
#if (SUPPORT_IR_CONVERTER)
#include "IR_Ctrl.h"
#endif
#if (SUPPORT_GPS) 
#include "GPS_ctl.h"
#endif
#if (TOUCH_UART_FUNC_EN)
#include "Touch_Uart_ctrl.h"
#endif
#include "disp_drv.h"
#include "AIT_Init.h"
#include "AHC_task.h"

#include "StateTVFunc.h"
#include "StateHDMIFunc.h"
#include "hdr_cfg.h"
#include "snr_cfg.h"
#include "mmpf_system.h" //For task monitor //CarDV...
#include "PMUCtrl.h" 
#include "IconPosition.h"
#include "ColorDefine.h"
#include "MenuDrawCommon.h"
#include "AHC_Callback.h"
#include "AHC_UF.h"
#if (defined(SUPPORT_SPEECH_RECOG)&&(SUPPORT_SPEECH_RECOG))
#include <math.h>
#include "api_wakeup.h"
#include "aitu_ringbuf.h"
#include "mmpf_vmd.h"
#endif
/*===========================================================================
 * Global variable
 *===========================================================================*/ 
MMP_ULONG LcdBrightCount = 0;
MMP_ULONG OldDeskClock=0;
MMP_ULONG DeskClock = 0;
MMP_USHORT TEMP_flog=0;
 MMP_U_LONG SDFORMATE = 0;
MMP_ULONG Icon_Clear_Count=0;
AHC_OS_SEMID 	ahc_System_Ready;
AHC_BOOL		gAHC_InitialDone = AHC_FALSE;
AHC_BOOL        gAHC_UpdateMenuDone = AHC_FALSE;
AHC_BOOL		Icon_Clear_Flag = AHC_FALSE;    

#if (TASK_MONITOR_ENABLE)
MMPF_TASK_MONITOR gsTaskMonitorAhcMain;
MMPF_TASK_MONITOR gsTaskMonitorAHCWorkStack;
MMPF_TASK_MONITOR gsTaskMonitorAHCWorkJob;
#endif

/*===========================================================================
 * Extern variable
 *===========================================================================*/ 

extern UARTCOMMAND 	sAhcUartCommand[];

/*===========================================================================
 * Extern function
 *===========================================================================*/
 extern void start_logo(void); // lyj
extern void 	AIHC_SetTempDCFMemStartAddr(UINT32 addr);
extern void 	AIHC_SetTempBrowserMemStartAddr(UINT32 addr);
extern void		InitKeyPad(void);
extern int      pf_HDR_EnSet(AHC_BOOL value);
extern void 	AHC_PrintAllTask(void);
extern void		AHC_PrintUITask(void);
#if defined(SUPPORT_SPEECH_RECOG)&&(SUPPORT_SPEECH_RECOG)
extern MMP_ULONG LivePCMTransferCB(AUTL_RINGBUF *pPCMOutRing, MMP_ULONG ulUnReadSampleCnt);
extern MMP_ULONG SpeechPCMTransferCB(AUTL_RINGBUF *pPCMOutRing, MMP_ULONG ulUnReadSampleCnt);
#endif
/*===========================================================================
 * Main body
 *===========================================================================*/ 

#if (TVOUT_PREVIEW_EN)
void InitOSDCustom_Tv(U32 colorFormat, AHC_OSD_ROTATE_DRAW_MODE ahcOSDRotateDraw)
{
    UINT16		uwTVWidth, uwTVHeight;
    UINT16		uwTVOffX, uwTVOffY;
    UINT32		pos[2];
    UINT32		uwDispScale = 0;
    UINT16		uwTVTempWidth, uwTVTempHeight;
    UINT8       ubTVSystem = MenuSettingConfig()->uiTVSystem;
    UINT16 uwDisplayID = 0;
 
    AHC_Display_GetWidthHdight(&uwTVWidth, &uwTVHeight);

	if(ubTVSystem == TV_SYSTEM_PAL)
	{
		if ((uiGetCurrentState() == UI_CAMERA_STATE)||
			(uiGetCurrentState() == UI_VIDEO_STATE)||
			(uiGetCurrentState() == UI_CLOCK_SETTING_STATE)||
			(uiGetCurrentState() == UI_USBSELECT_MENU_STATE))
		{
			uwDispScale = 1;
			uwTVTempWidth = uwTVWidth / uwDispScale;
			uwTVTempHeight = uwTVHeight / uwDispScale;
			uwTVOffX = 0;
			uwTVOffY = 0;
		}else
		if (uiGetCurrentState() == UI_BROWSER_STATE)
		{
			uwDispScale = 1;
			uwTVTempWidth = POS_TV_PAL_BROWSER_OSD_W;
			uwTVTempHeight = POS_TV_PAL_BROWSER_OSD_H;
			uwTVOffX = 0;
			uwTVOffY = 0;//68;
		}else
		//if (uiGetCurrentState() == UI_PLAYBACK_STATE)	 //By default,All of other state use this setting, include UI_PLAYBACK_STATE
		{
			uwDispScale = 1;
			uwTVTempWidth = POS_TV_PLAY_OSD_W;
			uwTVTempHeight = POS_TV_PLAY_OSD_H;
			uwTVOffX = uwTVWidth- uwTVTempWidth - 40;
			uwTVOffY = 68;
		}
	}
	else if(ubTVSystem == TV_SYSTEM_NTSC)
	{
		if ((uiGetCurrentState() == UI_CAMERA_STATE)||
			(uiGetCurrentState() == UI_VIDEO_STATE)||
			(uiGetCurrentState() == UI_CLOCK_SETTING_STATE)||
			(uiGetCurrentState() == UI_USBSELECT_MENU_STATE))
		{
			uwDispScale = 1;
			uwTVTempWidth = uwTVWidth / uwDispScale;
			uwTVTempHeight = uwTVHeight / uwDispScale;
			uwTVOffX = 0;
			uwTVOffY = 0;
		}else
		if (uiGetCurrentState() == UI_BROWSER_STATE)
		{
			uwDispScale = 1;
			uwTVTempWidth = POS_TV_NTSC_BROWSER_OSD_W;
			uwTVTempHeight = POS_TV_NTSC_BROWSER_OSD_H;
			uwTVOffX = 0;
			uwTVOffY = 0;//20;
		}else
		//if (uiGetCurrentState() == UI_PLAYBACK_STATE)	 //All of other state use this setting, include UI_PLAYBACK_STATE
		{
			uwDispScale = 1;
			uwTVTempWidth = POS_TV_PLAY_OSD_W;
			uwTVTempHeight = POS_TV_PLAY_OSD_H;
			uwTVOffX = uwTVWidth- uwTVTempWidth - 20;
			uwTVOffY = 20;
		}
	}

    {
        //** Set TV OSD#1 by uwDisplayID = 17
        uwDisplayID = TVFunc_GetUImodeOsdID();   
        AHC_OSDCreateBuffer(uwDisplayID ,uwTVTempWidth, uwTVTempHeight, OSD_COLOR_RGB565);
        pos[0] = uwTVOffX;
        pos[1] = uwTVOffY;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        pos[0] = uwDispScale - 1;
        pos[1] = uwDispScale - 1;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        pos[0] = 1;
        pos[1] = OSD_COLOR_TRANSPARENT;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_TRANSPARENT_ENABLE, pos);

        pos[0] = OSD_ROTATE_DRAW_NONE;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_ROTATE_BY_GUI, pos);

        pos[0] = OSD_FLIP_DRAW_NONE_ENABLE;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_FLIP_BY_GUI, pos);

        //** Set TV OSD by uwDisplayID = 17
        //   	AHC_OSDCreateBuffer(TV_UI_OSD2_ID ,uwTVTempWidth, uwTVTempHeight, OSD_COLOR_RGB565);
        //   	pos[0] = uwTVOffX;
        //   	pos[1] = uwTVOffY;
        //   	AHC_OSDSetDisplayAttr(TV_UI_OSD2_ID, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        //   	pos[0] = uwDispScale - 1;
        //   	pos[1] = uwDispScale - 1;
        //   	AHC_OSDSetDisplayAttr(TV_UI_OSD2_ID, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        //   	pos[0] = 1;
        //   	pos[1] = OSD_COLOR_TRANSPARENT;
        //   	AHC_OSDSetDisplayAttr(TV_UI_OSD2_ID, AHC_OSD_ATTR_TRANSPARENT_ENABLE, pos);

        //   	pos[0] = OSD_ROTATE_DRAW_NONE;
        //   	AHC_OSDSetDisplayAttr(TV_UI_OSD2_ID, AHC_OSD_ATTR_ROTATE_BY_GUI, pos);

        //   	pos[0] = OSD_FLIP_DRAW_NONE_ENABLE;
        //    AHC_OSDSetDisplayAttr(TV_UI_OSD2_ID, AHC_OSD_ATTR_FLIP_BY_GUI, pos);
        //--------------------------------------------------------
    }

#if (TV_MENU_EN)
    {
        UINT32		uwDispScale = 0;
        UINT16		uwTVTempWidth, uwTVTempHeight;	
        UINT16 uwDisplayID = 0, uwDisplayID2 = 0;

        uwDispScale = OSD_DISPLAY_SCALE_TV;

        uwTVTempWidth = uwTVWidth / OSD_DISPLAY_SCALE_TV;
        uwTVTempHeight = uwTVHeight / OSD_DISPLAY_SCALE_TV;

        uwDisplayID = TVFunc_GetMenuOsdID();
        uwDisplayID2 = TVFunc_GetMenuOsd2ID();
        //** Set TV MENU#1 by uwDisplayID = 18,19
        AHC_OSDCreateBuffer(uwDisplayID, uwTVTempWidth, uwTVTempHeight, OSD_COLOR_RGB565);
        AHC_OSDCreateBuffer(uwDisplayID2, uwTVTempWidth, uwTVTempHeight, OSD_COLOR_RGB565);
        pos[0] = 0;//uwTVOffX;
        pos[1] = 0;//uwTVOffY;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        AHC_OSDSetDisplayAttr(uwDisplayID2, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        pos[0] = uwDispScale - 1;
        pos[1] = uwDispScale - 1;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        AHC_OSDSetDisplayAttr(uwDisplayID2, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        pos[0] = OSD_FLIP_DRAW_NONE_ENABLE;                
        AHC_OSDSetDisplayAttr(uwDisplayID,  AHC_OSD_ATTR_FLIP_BY_GUI, pos);
        AHC_OSDSetDisplayAttr(uwDisplayID2,  AHC_OSD_ATTR_FLIP_BY_GUI, pos);
        pos[0] = OSD_ROTATE_DRAW_NONE;
        AHC_OSDSetDisplayAttr(uwDisplayID,  AHC_OSD_ATTR_ROTATE_BY_GUI, pos);
        AHC_OSDSetDisplayAttr(uwDisplayID2,  AHC_OSD_ATTR_ROTATE_BY_GUI, pos);
        AHC_OSDClearBuffer(uwDisplayID);
        AHC_OSDClearBuffer(uwDisplayID2);
    }
#endif
}
#endif

#if (HDMI_ENABLE)
void InitOSDCustom_Hdmi(U32 colorFormat, AHC_OSD_ROTATE_DRAW_MODE ahcOSDRotateDraw)
{
    UINT16		uwHDMIWidth, uwHDMIHeight;
    UINT16		uwHDMIOffX, uwHDMIOffY;
    UINT32		pos[2];
    UINT32		uwDispScale = 0;
    UINT16		uwHDMITempWidth, uwHDMITempHeight;

    AHC_GetHdmiDisplayWidthHeight(&uwHDMIWidth, &uwHDMIHeight);


	if((MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_1080I)||(MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_1080P))
	{
		if ((uiGetCurrentState() == UI_CAMERA_STATE)||
			(uiGetCurrentState() == UI_VIDEO_STATE)||
			(uiGetCurrentState() == UI_CLOCK_SETTING_STATE)||
			(uiGetCurrentState() == UI_USBSELECT_MENU_STATE))
		{
			uwDispScale = OSD_DISPLAY_SCALE_HDMI_FHD;
			uwHDMITempWidth = uwHDMIWidth / uwDispScale;
			uwHDMITempHeight = uwHDMIHeight / uwDispScale;
			uwHDMIOffX = 0;
			uwHDMIOffY = 0;
		}else
		if (uiGetCurrentState() == UI_BROWSER_STATE)
		{
			uwDispScale = OSD_PREVIEW_SCALE_HDMI;
			uwHDMITempWidth = POS_HDMI_1080P_BROWSER_OSD_W;
			uwHDMITempHeight = POS_HDMI_1080P_BROWSER_OSD_H;
			uwHDMIOffX = 0;
			uwHDMIOffY = 15;
		}else
		//if (uiGetCurrentState() == UI_PLAYBACK_STATE)		//By default,All of other state use this setting, include UI_PLAYBACK_STATE
		{
			uwDispScale = OSD_PREVIEW_SCALE_HDMI;
			uwHDMITempWidth = POS_HDMI_PLAY_OSD_W;
			uwHDMITempHeight = POS_HDMI_PLAY_OSD_H;
			uwHDMIOffX = uwHDMIWidth- uwHDMITempWidth - 50;
			uwHDMIOffY = 25;
		}
	}
	else if(MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_720P)
	{
		if ((uiGetCurrentState() == UI_CAMERA_STATE)||
			(uiGetCurrentState() == UI_VIDEO_STATE)||
			(uiGetCurrentState() == UI_CLOCK_SETTING_STATE)||
			(uiGetCurrentState() == UI_USBSELECT_MENU_STATE))
		{
			uwDispScale = OSD_DISPLAY_SCALE_HDMI_HD;
			uwHDMITempWidth = uwHDMIWidth / uwDispScale;
			uwHDMITempHeight = uwHDMIHeight / uwDispScale;
			uwHDMIOffX = 0;
			uwHDMIOffY = 0;
		}else
		if (uiGetCurrentState() == UI_BROWSER_STATE)
		{
			uwDispScale = OSD_PREVIEW_SCALE_HDMI;
			uwHDMITempWidth = POS_HDMI_720P_BROWSER_OSD_W;
			uwHDMITempHeight = POS_HDMI_720P_BROWSER_OSD_H;
			uwHDMIOffX = 0;
			uwHDMIOffY = 20;
		}else
		//if (uiGetCurrentState() == UI_PLAYBACK_STATE)		//All of other state use this setting
		{
			uwDispScale = OSD_PREVIEW_SCALE_HDMI;
			uwHDMITempWidth = POS_HDMI_PLAY_OSD_W;
			uwHDMITempHeight = POS_HDMI_PLAY_OSD_H;
			uwHDMIOffX = uwHDMIWidth- uwHDMITempWidth - 40;
			uwHDMIOffY = 25;
		}
	}
	else if(MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_480P)
	{
		if ((uiGetCurrentState() == UI_CAMERA_STATE)||
			(uiGetCurrentState() == UI_VIDEO_STATE)||
			(uiGetCurrentState() == UI_CLOCK_SETTING_STATE)||
			(uiGetCurrentState() == UI_USBSELECT_MENU_STATE))
		{
			uwDispScale = OSD_DISPLAY_SCALE_TV;
			uwHDMITempWidth = uwHDMIWidth / uwDispScale;
			uwHDMITempHeight = uwHDMIHeight / uwDispScale;
			uwHDMIOffX = 0;
			uwHDMIOffY = 0;
		}else
		if (uiGetCurrentState() == UI_BROWSER_STATE)
		{
			uwDispScale = OSD_PREVIEW_SCALE_HDMI;
			uwHDMITempWidth = POS_HDMI_480P_BROWSER_OSD_W;
			uwHDMITempHeight = POS_HDMI_480P_BROWSER_OSD_H;
			uwHDMIOffX = 0;
			uwHDMIOffY = 20;
		}else
		//if (uiGetCurrentState() == UI_PLAYBACK_STATE)		//CAUTION:: All of other state use this setting
		{
			uwDispScale = OSD_PREVIEW_SCALE_HDMI;
			uwHDMITempWidth = POS_HDMI_PLAY_OSD_W;
			uwHDMITempHeight = POS_HDMI_PLAY_OSD_H;
			uwHDMIOffX = uwHDMIWidth- uwHDMITempWidth - 40;
			uwHDMIOffY = 25;
		}
	}
#if OSD_PREVIEW_SCALE_HDMI
    {
        UINT16 uwDisplayID = 0, uwDisplayID2 = 0;
        
        uwDisplayID = HDMIFunc_GetUImodeOsdID();
        uwDisplayID2 = HDMIFunc_GetUImodeOsd2ID();
        //** Set HDMI OSD#1 by uwDisplayID = 16
        AHC_OSDCreateBuffer(uwDisplayID ,uwHDMITempWidth, uwHDMITempHeight, OSD_COLOR_RGB565);
        pos[0] = uwHDMIOffX;
        pos[1] = uwHDMIOffY;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        pos[0] = uwDispScale - 1;
        pos[1] = uwDispScale - 1;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        pos[0] = 1;
        pos[1] = OSD_COLOR_TRANSPARENT;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_TRANSPARENT_ENABLE, pos);

        pos[0] = OSD_ROTATE_DRAW_NONE;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_ROTATE_BY_GUI, pos);

        pos[0] = OSD_FLIP_DRAW_NONE_ENABLE;
        AHC_OSDSetDisplayAttr(uwDisplayID, AHC_OSD_ATTR_FLIP_BY_GUI, pos);

        //** Set HDMI OSD by uwDisplayID = 17
        AHC_OSDCreateBuffer(uwDisplayID2 ,uwHDMITempWidth, uwHDMITempHeight, OSD_COLOR_RGB565);
        pos[0] = uwHDMIOffX;
        pos[1] = uwHDMIOffY;
        AHC_OSDSetDisplayAttr(uwDisplayID2, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        pos[0] = uwDispScale - 1;
        pos[1] = uwDispScale - 1;
        AHC_OSDSetDisplayAttr(uwDisplayID2, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        pos[0] = 1;
        pos[1] = OSD_COLOR_TRANSPARENT;
        AHC_OSDSetDisplayAttr(uwDisplayID2, AHC_OSD_ATTR_TRANSPARENT_ENABLE, pos);

        pos[0] = OSD_ROTATE_DRAW_NONE;
        AHC_OSDSetDisplayAttr(uwDisplayID2, AHC_OSD_ATTR_ROTATE_BY_GUI, pos);

        pos[0] = OSD_FLIP_DRAW_NONE_ENABLE;
        AHC_OSDSetDisplayAttr(uwDisplayID2, AHC_OSD_ATTR_FLIP_BY_GUI, pos);
        //--------------------------------------------------------
    }
#endif

#if (HDMI_MENU_EN)
    {
        UINT16 uwMenuID = 0, uwMenuID2 = 0;

        uwMenuID = HDMIFunc_GetMenuOsdID();
        uwMenuID2 = HDMIFunc_GetMenuOsd2ID();
    	if((MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_1080I)||(MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_1080P))
    	{
    		if (uiGetCurrentState() == UI_BROWSER_STATE)
    		{
    			uwDispScale = OSD_DISPLAY_SCALE_HDMI_FHD;
    			uwHDMITempWidth = uwHDMIWidth / uwDispScale;
    			uwHDMITempHeight = uwHDMIHeight / uwDispScale;
    			uwHDMIOffX = 0;
    			uwHDMIOffY = 0;
    		}
    	}
        else if((MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_720P))
        {
    		if (uiGetCurrentState() == UI_BROWSER_STATE)
    		{
    			uwDispScale = OSD_DISPLAY_SCALE_HDMI_HD;
    			uwHDMITempWidth = uwHDMIWidth / uwDispScale;
    			uwHDMITempHeight = uwHDMIHeight / uwDispScale;
    			uwHDMIOffX = 0;
    			uwHDMIOffY = 0;
    		}
    	}
    	//** Set HDMI OSD#1 by uwDisplayID = 18,19
        AHC_OSDCreateBuffer(uwMenuID, uwHDMITempWidth, uwHDMITempHeight, OSD_COLOR_RGB565);

        pos[0] = uwHDMIOffX;
        pos[1] = uwHDMIOffY;
        AHC_OSDSetDisplayAttr(uwMenuID, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        pos[0] = uwDispScale - 1;
        pos[1] = uwDispScale - 1;
        AHC_OSDSetDisplayAttr(uwMenuID, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        pos[0] = OSD_ROTATE_DRAW_NONE;
        AHC_OSDSetDisplayAttr(uwMenuID,  AHC_OSD_ATTR_ROTATE_BY_GUI, pos);
        AHC_OSDClearBuffer(uwMenuID);

        AHC_OSDCreateBuffer(uwMenuID2, uwHDMITempWidth, uwHDMITempHeight, OSD_COLOR_RGB565);
        pos[0] = uwHDMIOffX;
        pos[1] = uwHDMIOffY;
        AHC_OSDSetDisplayAttr(uwMenuID2, AHC_OSD_ATTR_DISPLAY_OFFSET, pos);
        pos[0] = uwDispScale - 1;
        pos[1] = uwDispScale - 1;
        AHC_OSDSetDisplayAttr(uwMenuID2, AHC_OSD_ATTR_DISPLAY_SCALE, pos);
        pos[0] = OSD_ROTATE_DRAW_NONE;
        AHC_OSDSetDisplayAttr(uwMenuID2,  AHC_OSD_ATTR_ROTATE_BY_GUI, pos);
        AHC_OSDClearBuffer(uwMenuID2);
    }
#endif
}
#endif

void InitOSDCustom_Lcd(U32 colorFormat, AHC_OSD_ROTATE_DRAW_MODE ahcOSDRotateDraw)
{
    UINT32 i;
    UINT32 iVal[10];
    INT32  bAlphaBlendingEn = 0;

    // LAYER 0 - ID 0 ~ 15
    // LAYER 1 - ID 16 ~ 31
    // Default layer is display id 0 & 16    

    if(OSD_COLOR_RGB565 == colorFormat){
        colorFormat = OSD_COLOR_RGB565_CCW; //use RGB565 CCW internally.
    }
    else if(OSD_COLOR_ARGB32 == colorFormat){
        colorFormat = OSD_COLOR_ARGB32_CCW; //use ARGB CCW internally.
    }
    
    for(i=1; i<2; i++){
        AHC_OSDCreateBuffer(i,
                    RTNA_LCD_GetAttr()->usPanelW / OSD_DISPLAY_SCALE_LCD, RTNA_LCD_GetAttr()->usPanelH / OSD_DISPLAY_SCALE_LCD,
                    colorFormat/*OSD_COLOR_RGB565_CCW*//*OSD_COLOR_ARGB32_CCW*/);
#if 0        
        if (OSD_DISPLAY_SCALE_LCD) {
            AHC_OSDCreateBuffer(i,
            RTNA_LCD_GetAttr()->usPanelW / OSD_DISPLAY_SCALE_LCD, RTNA_LCD_GetAttr()->usPanelH / OSD_DISPLAY_SCALE_LCD,
            OSD_COLOR_RGB565_CCW/*OSD_COLOR_ARGB32_CCW*/);
        }
        else {
            AHC_OSDCreateBuffer(i, RTNA_LCD_GetAttr()->usPanelW, RTNA_LCD_GetAttr()->usPanelH, colorFormat);
        }
#endif

        iVal[0] = ahcOSDRotateDraw;        
        AHC_OSDSetDisplayAttr(i,  AHC_OSD_ATTR_ROTATE_BY_GUI, iVal);
        iVal[0] = OSD_FLIP_DRAW_NONE_ENABLE;       
        AHC_OSDSetDisplayAttr(i,  AHC_OSD_ATTR_FLIP_BY_GUI, iVal);        

        if (OSD_DISPLAY_SCALE_LCD) {
            iVal[0] = OSD_DISPLAY_SCALE_LCD - 1;    // remapping to enum MMP_DISPLAY_DUPLICATE
            iVal[1] = OSD_DISPLAY_SCALE_LCD - 1;
        }
        else {
            iVal[0] = MMP_DISPLAY_DUPLICATE_1X;
            iVal[1] = MMP_DISPLAY_DUPLICATE_1X;
        }

        AHC_OSDSetDisplayAttr(i, AHC_OSD_ATTR_DISPLAY_SCALE, iVal);
        AHC_OSDClearBuffer(i);
    }

    for(i=16; i<=20; i++)
    {
        AHC_OSDCreateBuffer(i,
                RTNA_LCD_GetAttr()->usPanelW / OSD_DISPLAY_SCALE_LCD, RTNA_LCD_GetAttr()->usPanelH / OSD_DISPLAY_SCALE_LCD,
                colorFormat);

        iVal[0] = ahcOSDRotateDraw;                
        AHC_OSDSetDisplayAttr(i,  AHC_OSD_ATTR_ROTATE_BY_GUI, iVal);
        iVal[0] = OSD_FLIP_DRAW_NONE_ENABLE;                
        AHC_OSDSetDisplayAttr(i,  AHC_OSD_ATTR_FLIP_BY_GUI, iVal);          

        if (OSD_DISPLAY_SCALE_LCD) {
            iVal[0] = OSD_DISPLAY_SCALE_LCD - 1;    // remapping to enum MMP_DISPLAY_DUPLICATE
            iVal[1] = OSD_DISPLAY_SCALE_LCD - 1;
        }
        else {
            iVal[0] = MMP_DISPLAY_DUPLICATE_1X;
            iVal[1] = MMP_DISPLAY_DUPLICATE_1X;
        }    
        AHC_OSDSetDisplayAttr(i, AHC_OSD_ATTR_DISPLAY_SCALE, iVal);
        AHC_OSDClearBuffer(i);
    }

    // Set transparent color
    iVal[0] = 1;         	// Enable
    iVal[1] = 0x00000000; 	// Black color
    AHC_OSDSetDisplayAttr(0, AHC_OSD_ATTR_TRANSPARENT_ENABLE, iVal);
    AHC_OSDSetDisplayAttr(1, AHC_OSD_ATTR_TRANSPARENT_ENABLE, iVal);

    for(i=0;i<5;i++){
        AHC_OSDSetDisplayAttr(i+16, AHC_OSD_ATTR_TRANSPARENT_ENABLE, iVal);
    }

    if(bAlphaBlendingEn){
        iVal[0] = 1;        // Enable
        iVal[1] = AHC_OSD_ALPHA_ARGB;

        for(i=0; i<8; i++) {
            iVal[2+i] = i * 256 / 8;  // Alpha weighting 
        }

        AHC_OSDSetDisplayAttr(0, AHC_OSD_ATTR_ALPHA_BLENDING_ENABLE, iVal);
        AHC_OSDSetDisplayAttr(1, AHC_OSD_ATTR_ALPHA_BLENDING_ENABLE, iVal);
        AHC_OSDSetDisplayAttr(2, AHC_OSD_ATTR_ALPHA_BLENDING_ENABLE, iVal);

        for(i=0;i<5;i++){
            AHC_OSDSetDisplayAttr(i+16, AHC_OSD_ATTR_ALPHA_BLENDING_ENABLE, iVal);
        }
    }    

    AHC_OSDSetColor(1,  0x80000000); // Alpha value is 0x80
    AHC_OSDSetColor(0,  0x80000000); // Alpha value is 0x80
    AHC_OSDSetColor(20, 0x80000000); // Alpha value is 0x80

    AHC_OSDSetCurrentDisplay(1);    
    AHC_OSDSetCurrentDisplay(0);
    AHC_OSDSetCurrentDisplay(20);

    // Set MAIN Inactive & OVERLAY WINDOW Active
    AHC_OSDSetActive(0, 0);
    AHC_OSDSetActive(20, 1);
    //printc("### %x\r\n\r\n", OSTimeGet());

}

#if (TVOUT_ENABLE)//(TVOUT_PREVIEW_EN)
void InitOSD_TVout(void)
{
    U32 colorFormat;
    AHC_OSD_ROTATE_DRAW_MODE ahcOSDRotateDraw;

    U32 ulOSDWidth, ulOSDHeight, ulThumbWidth, ulThumbHeight; 
    AHC_BOOL bMainBuffFull;

    UINT16 		uwTVWidth, uwTVHeight;

    printc("### %s,%d -\r\n", __func__, __LINE__);

    GUI_RegisterExternalFillRectCB((void *)AHC_OSD_ExternalFillRectCB);

#if 0//TBD
    if(MenuSettingConfig()->uiTVSystem==TV_SYSTEM_PAL)
    AHC_SetDisplayOutputDev(DISP_OUT_TV_PAL, AHC_DISPLAY_DUPLICATE_1X);
    else
    AHC_SetDisplayOutputDev(DISP_OUT_TV_NTSC, AHC_DISPLAY_DUPLICATE_1X);
#endif
#if (TVOUT_PREVIEW_EN)
    ulOSDWidth = 320; //TBD
    ulOSDHeight = 240;
#else
    ulOSDWidth = 0; //TBD
    ulOSDHeight = 0
#endif
    if (uiGetCurrentState() == UI_BROWSER_STATE){
        AHC_DISPLAY_OUTPUTPANEL pCurDevice;

        AHC_GetDisplayOutputDev(&pCurDevice);    
        //TBD
        if(((MenuSettingConfig()->uiTVSystem == TV_SYSTEM_PAL) && (pCurDevice != AHC_DISPLAY_PAL_TV)) ||
        ((MenuSettingConfig()->uiTVSystem == TV_SYSTEM_NTSC) && (pCurDevice != AHC_DISPLAY_NTSC_TV))){   
            switch(MenuSettingConfig()->uiTVSystem) 
            {
                case TV_SYSTEM_PAL:
                    AHC_SetDisplayOutputDev(DISP_OUT_TV_PAL, AHC_DISPLAY_DUPLICATE_1X);
                    AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_PAL_TV);
                    break;	    	    
                    
                case TV_SYSTEM_NTSC:
                default:
                    AHC_SetDisplayOutputDev(DISP_OUT_TV_NTSC, AHC_DISPLAY_DUPLICATE_1X);
                    AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_NTSC_TV);
                    break;
            }    							
        }
        AHC_Display_GetWidthHdight(&uwTVWidth, &uwTVHeight);	   
        switch(MenuSettingConfig()->uiTVSystem) {
            case TV_SYSTEM_NTSC:
                ulThumbWidth 	= uwTVWidth;
                ulThumbHeight	= uwTVHeight - POS_TV_NTSC_BROWSER_OSD_H;	
                break;

            case TV_SYSTEM_PAL:
                ulThumbWidth 	= uwTVWidth;
                ulThumbHeight	= uwTVHeight - POS_TV_PAL_BROWSER_OSD_H;
                break;			

            default:
                printc("TVFunc_ChangeOSDStatus Error!\r\n");
                break;
        }

    }else{
        ulThumbWidth = 0;
        ulThumbHeight = 0;
    }     

    bMainBuffFull = AHC_FALSE;

    colorFormat = WMSG_LAYER_WIN_COLOR_FMT;
    ahcOSDRotateDraw = OSD_ROTATE_DRAW_NONE;
    AHC_OSD_RegisterInitOSDCustomCB((void *)InitOSDCustom_Tv);

#if	defined(MALLOC_OSDBUFF_DOWN_GROWTH) && (MALLOC_OSDBUFF_DOWN_GROWTH)
	AIHC_SetGUIMemStartAddr( AHC_UF_GetTempBaseAddr());
#else
    AIHC_SetGUIMemStartAddr(AHC_GUI_TEMP_BASE_ADDR);
#endif

    AHC_OSDInit(ulOSDWidth, ulOSDHeight, 
                ulThumbWidth, ulThumbHeight,
                PRIMARY_DATESTAMP_WIDTH, PRIMARY_DATESTAMP_HEIGHT, 
                THUMB_DATESTAMP_WIDTH, THUMB_DATESTAMP_HEIGHT, 
                colorFormat, bMainBuffFull, ahcOSDRotateDraw);

}
#endif

#if (HDMI_ENABLE)//(HDMI_PREVIEW_EN)
void InitOSD_HDMI(void)
{
    U32 colorFormat;
    AHC_OSD_ROTATE_DRAW_MODE ahcOSDRotateDraw;

    U32 ulOSDWidth, ulOSDHeight, ulThumbWidth, ulThumbHeight; 
    AHC_BOOL bMainBuffFull;

    UINT16 		uwHDMIWidth, uwHDMIHeight;
    //UINT16		uwHDMIOffX,  uwHDMIOffY;
    AHC_DISPLAY_HDMIOUTPUTMODE 	HdmiMode;

    printc("### %s,%d -\r\n", __func__, __LINE__);
    GUI_RegisterExternalFillRectCB((void *)AHC_OSD_ExternalFillRectCB);
	//bHaveInitOsdHdmi = AHC_TRUE;
	
#if (HDMI_PREVIEW_EN)
    ulOSDWidth = 320;
    ulOSDHeight = 240;
#else
    ulOSDWidth = 0;
    ulOSDHeight = 0;
#endif
    if (uiGetCurrentState() == UI_BROWSER_STATE)
    {
        AHC_GetHdmiDisplayWidthHeight(&uwHDMIWidth, &uwHDMIHeight);
        AHC_GetHDMIOutputMode(&HdmiMode);

        switch(HdmiMode)
        {
            case AHC_DISPLAY_HDMIOUTPUT_1920X1080P:
                ulThumbWidth 	= uwHDMIWidth;
                ulThumbHeight 	= uwHDMIHeight - POS_HDMI_1080I_BROWSER_OSD_H;// - POS_HDMI_1080I_BROWSER_OSD_H;
                break;
                
            case AHC_DISPLAY_HDMIOUTPUT_1920X1080I:
                ulThumbWidth 	= uwHDMIWidth;
                ulThumbHeight 	= (uwHDMIHeight<<1) - POS_HDMI_1080I_BROWSER_OSD_H;
                break;
                
            case AHC_DISPLAY_HDMIOUTPUT_1280X720P:
                ulThumbWidth 	= uwHDMIWidth;
                ulThumbHeight 	= uwHDMIHeight - POS_HDMI_720P_BROWSER_OSD_H;
                break;
                
            case AHC_DISPLAY_HDMIOUTPUT_640X480P:
            case AHC_DISPLAY_HDMIOUTPUT_720X480P:
            case AHC_DISPLAY_HDMIOUTPUT_720X576P:
            case AHC_DISPLAY_HDMIOUTPUT_1280X720P_50FPS:
            case AHC_DISPLAY_HDMIOUTPUT_1920X1080P_30FPS:
            default:
                printc("HDMIFunc_ChangeOSDStatus Error!\r\n");
                break;
        }
    }else{
        ulThumbWidth = 0;
        ulThumbHeight = 0;
    }

    bMainBuffFull = AHC_FALSE;

    //AHC_SetDisplayOutputDev(DISP_OUT_HDMI, AHC_DISPLAY_DUPLICATE_1X);
    colorFormat = WMSG_LAYER_WIN_COLOR_FMT;
    ahcOSDRotateDraw = OSD_ROTATE_DRAW_NONE;        
    AHC_OSD_RegisterInitOSDCustomCB((void *)InitOSDCustom_Hdmi);

#if	defined(MALLOC_OSDBUFF_DOWN_GROWTH)&&(MALLOC_OSDBUFF_DOWN_GROWTH)
	AIHC_SetGUIMemStartAddr( AHC_UF_GetTempBaseAddr());
#else
    AIHC_SetGUIMemStartAddr(AHC_GUI_TEMP_BASE_ADDR_HDMI);
#endif

    AHC_OSDInit(ulOSDWidth, ulOSDHeight, 
                ulThumbWidth, ulThumbHeight,
                PRIMARY_DATESTAMP_WIDTH, PRIMARY_DATESTAMP_HEIGHT, 
                THUMB_DATESTAMP_WIDTH, THUMB_DATESTAMP_HEIGHT, 
                colorFormat, bMainBuffFull, ahcOSDRotateDraw);
}
#endif

void InitOSD_LCD(void)
{
    U32 colorFormat;
    AHC_OSD_ROTATE_DRAW_MODE ahcOSDRotateDraw;

    U32 ulOSDWidth, ulOSDHeight, ulThumbWidth, ulThumbHeight; 
    AHC_BOOL bMainBuffFull;

    printc("### %s,%d -\r\n", __func__, __LINE__);

    GUI_RegisterExternalFillRectCB((void *)AHC_OSD_ExternalFillRectCB);

    ulOSDWidth = RTNA_LCD_GetAttr()->usPanelW / OSD_DISPLAY_SCALE_LCD;
    ulOSDHeight = RTNA_LCD_GetAttr()->usPanelH / OSD_DISPLAY_SCALE_LCD;
    ulThumbWidth = RTNA_LCD_GetAttr()->usPanelW / OSD_DISPLAY_SCALE_LCD;
    ulThumbHeight = RTNA_LCD_GetAttr()->usPanelH / OSD_DISPLAY_SCALE_LCD;
    bMainBuffFull = AHC_TRUE;

    //Force OSD format as RGB565 for saving memory and drawing time.
    colorFormat = OSD_COLOR_RGB565;
    //colorFormat = OSD_COLOR_ARGB32;
#if (VERTICAL_LCD == VERTICAL_LCD_DEGREE_0)
    ahcOSDRotateDraw = OSD_ROTATE_DRAW_NONE;
#elif (VERTICAL_LCD == VERTICAL_LCD_DEGREE_90)
    ahcOSDRotateDraw = OSD_ROTATE_DRAW_RIGHT_90;
#elif (VERTICAL_LCD == VERTICAL_LCD_DEGREE_270)
    ahcOSDRotateDraw = OSD_ROTATE_DRAW_RIGHT_270;
#endif        
    AHC_OSD_RegisterInitOSDCustomCB((void *)InitOSDCustom_Lcd);

#if	defined(MALLOC_OSDBUFF_DOWN_GROWTH)&&(MALLOC_OSDBUFF_DOWN_GROWTH)
	AIHC_SetGUIMemStartAddr( AHC_UF_GetTempBaseAddr());
#else
    AIHC_SetGUIMemStartAddr(AHC_GUI_TEMP_BASE_ADDR);
#endif

    if(OSD_DISPLAY_SCALE_LCD == 0){
        AHC_PRINT_RET_ERROR(0,0);
        return;
    }

    AHC_OSDInit(ulOSDWidth, ulOSDHeight, 
	    ulThumbWidth, ulThumbHeight,
	    PRIMARY_DATESTAMP_WIDTH, PRIMARY_DATESTAMP_HEIGHT, 
	    THUMB_DATESTAMP_WIDTH, THUMB_DATESTAMP_HEIGHT, 
	    colorFormat, bMainBuffFull, ahcOSDRotateDraw);
}

void InitOSD_None(void)
{
    printc("### %s,%d -\r\n", __func__, __LINE__);

    GUI_RegisterExternalFillRectCB((void *)AHC_OSD_ExternalFillRectCB);

    //TBD
}

void InitOSD(void)
{
#if (TVOUT_ENABLE)//(TVOUT_PREVIEW_EN)
    if(AHC_IsTVConnectEx()){
        InitOSD_TVout();
    }
    else
#endif	
#if (HDMI_ENABLE)//(HDMI_PREVIEW_EN)
    if(AHC_IsHdmiConnect()){
        InitOSD_HDMI();
    }
    else
#endif
    {
        InitOSD_LCD();
    }
}

#if (defined(SUPPORT_SPEECH_RECOG)&&(SUPPORT_SPEECH_RECOG))
int speech_handler(void *obj, wakeup_status_t status, char *json, int bytes)
{
    if (status == 0) {
    	printc("not wakeup.\r\n");
    } else {
    	printc(json);
    }
    return 0;
}
void SpeechRecogRunCB(MMP_ULONG ID)
{
	printc("SpeechRecogCB=%X.\r\n",ID);
}
#endif
UINT8 AHC_WaitForSystemReady(void)
{
    UINT16 SemCount = 0;

    AHC_OS_QuerySem(ahc_System_Ready, &SemCount); 

	return (0 == SemCount)?(0):(1);
}

MMP_UBYTE bDCFInitDone = 0; //Andy Liu. Remove it later...
void AHC_DCF_Init(void)
{
    AHC_AUDIO_I2S_CFG   ConfigAudioI2S;
    AHC_BOOL 			bSD_inserted = AHC_FALSE;

	printc("### %s -\r\n", __func__);
	
	AIHC_SetTempDCFMemStartAddr(AHC_DCF_TEMP_BASE_ADDR);
	AIHC_SetTempBrowserMemStartAddr(AHC_THUMB_TEMP_BASE_ADDR);

    #if (AHC_SHAREENC_SUPPORT)
    AHC_UF_RegisterCallback(AHC_UFCB_CREATE_FORMAT_FREE_FILE, (void *)AHC_UF_CreateShareFileCB);
    #endif

    if(MEDIA_SETTING_SD_CARD == MenuSettingConfig()->uiMediaSelect)
    {
        bSD_inserted = AHC_IsSDInserted();

        if(bSD_inserted){
			Enable_SD_Power(1 /* Power On */);
            AHC_SDMMC_SetState(SDMMC_IN);
            
            #if (FS_FORMAT_FREE_ENABLE)
            if( AHC_CheckMedia_FormatFree( AHC_MEDIA_MMC ) == AHC_FALSE )
            {
                SystemSettingConfig()->byNeedToFormatMediaAsFormatFree = 1;
            }
            else            
            #endif
                AHC_RemountDevices(MenuSettingConfig()->uiMediaSelect);
        }
        else{
			Enable_SD_Power(0 /* Power Off */);
            AHC_SDMMC_SetState(SDMMC_OUT);
    	}
    }
    else
    {
    	AHC_RemountDevices(MenuSettingConfig()->uiMediaSelect);
	}

    ConfigAudioI2S.workingMode      = AHC_AUDIO_I2S_MASTER_MODE;
    ConfigAudioI2S.mclkMode         = AHC_AUDIO_I2S_256FS_MODE;
    ConfigAudioI2S.outputBitSize    = AHC_AUDIO_I2S_OUT_16BIT;
    ConfigAudioI2S.lrckPolarity     = AHC_AUDIO_I2S_LRCK_L_LOW;

    AHC_ConfigAudioExtDevice(&ConfigAudioI2S, 44100);
	//MIMF_CUSTOMIZED_SORT_TYPE use for customized sorting type, implement by AHC_MIMF.c
#if(MIMF_CUSTOMIZED_SORT_TYPE)    
    AHC_MIMF_RegisterCusCallback();
#endif  

    bDCFInitDone = 1;
}

#if (TASK_MONITOR_ENABLE)
void AHC_Main_Task_ReSetMonitorTime(UINT32 ulTime)
{
    gsTaskMonitorAhcMain.ulExecTime = ulTime;
}
#endif


void AHC_Main_Task(void *p_arg)
{
    AHC_SWPACK_TaskInit();
    
    ahc_System_Ready = AHC_OS_CreateSem(0);

    AHC_USBDetectHandler();
    
#if (SUPPORT_IR_CONVERTER)
    #if 1
    printc("--I-- IR function is TBD\r\n");
    #else
    IRConverter_Initialize();
    // To Clear IR status of PowerOn Key avoid power off
    IRConverter_WriteCommand(IR_CONVERTER_CLEAR_POWERON, 0x01);
    #endif
#endif
	
    AHC_Initialize();
#if defined(CFG_BOOT_BYPASS_CLOCK_CHECK)
	CheckRtcAndAutoSetRtc();
#endif
	AHC_WaitForBootComplete();
   
#ifdef USB_LABLE_STRING
	AHC_SetUSBLable(USB_LABLE_STRING);
#endif

	/* Update Menu Setting */ 
	uiCheckDefaultMenuExist();
	//start_logo();// lyj
	gAHC_UpdateMenuDone = AHC_TRUE;
	printc("AHC_Main_Task--MenuSettingConfig()->uiSlideShowEffect=%d\n",MenuSettingConfig()->uiSlideShowEffect);
	printc("AHC_Main_Task--MenuSettingConfig()->uiLCDBrightness=%d\n",MenuSettingConfig()->uiLCDBrightness);
	MenuSettingConfig()->uiMediaSelect = MEDIA_SETTING_SD_CARD;

    #if (AHC_DRAM_SIZE == COMMON_DRAM_SIZE_32MB)
    pf_FCWS_EnSet(FCWS_EN_OFF);
    pf_SAG_EnSet(SAG_EN_OFF);
    pf_HDR_EnSet(HDR_EN_OFF);
    MMP_EnableVidHDR(MMP_FALSE);
    #endif

	#if defined(SUPPORT_ONCHIP_IRDA)&&(SUPPORT_ONCHIP_IRDA)
	MMPS_IrDA_Initialize();
	MMPS_IrDA_SetIrEn(MMP_TRUE);
    #endif

    #if SNR_CLK_POWER_SAVING_SETTING
    pf_HDR_EnSet(COMMON_HDR_EN_OFF);
    #endif

	Menu_SetVolume(MenuSettingConfig()->uiVolume);

	AHC_SetMenuFlashLEDStatus(MenuSettingConfig()->uiLEDFlash);	

#if (SUPPORT_GPS)
	AHC_OS_SetFlags(UartCtrlFlag, GPS_FLAG_INITIAL, AHC_OS_FLAG_SET);
#endif
#if (TOUCH_UART_FUNC_EN)
	AHC_OS_SetFlags(UartCtrlFlag, Touch_Uart_FLAG_INITIAL, AHC_OS_FLAG_SET);
#endif
	/*let power on logo off*/
	//AHC_SetDisplayMode(DISPLAY_MODE_DISABLE); //move to other loc
    // Remove this and initial this with jobdispatch task.//CZ patch...20160204
    //AHC_DCF_Init();

    #ifdef CFG_TURN_OFF_POWER_LED_WHEN_BOOT_FINISH
    LedCtrl_PowerLed(AHC_FALSE);
    #else
    LedCtrl_PowerLed(AHC_TRUE);
    #endif

    AHC_PowerOn_Welcome();
	MMPF_PIO_PadConfig(MMP_GPIO28, PAD_OUT_DRIVING(0), MMP_TRUE);//leo20180702
	MMPF_PIO_PadConfig(MMP_GPIO29, PAD_OUT_DRIVING(0), MMP_TRUE);
	MMPF_PIO_EnableOutputMode(MMP_GPIO28, MMP_TRUE, MMP_FALSE);
	MMPF_PIO_SetData(MMP_GPIO28, USB_PATH_SELECT_SET, MMP_TRUE);
	MMPF_PIO_EnableOutputMode(MMP_GPIO29, MMP_TRUE, MMP_FALSE);
	MMPF_PIO_SetData(MMP_GPIO29, USB_PATH_SELECT_SET, MMP_TRUE);

    MMPF_PIO_EnableOutputMode(USB_PATH_SELECT_GPIO, MMP_TRUE, MMP_FALSE);
    MMPF_PIO_SetData(USB_PATH_SELECT_GPIO, USB_PATH_SELECT_SET, MMP_TRUE);
	#ifndef	NO_PANEL
	// InitOSD will make LCD off, Shutdown LCD backlight must before,
	// Otherwise, Some kind of LCD will show ugly white lines
	//LedCtrl_LcdBackLight(AHC_FALSE);
	//LedCtrl_LcdBackLightLock(AHC_TRUE);
	#endif	// NO_PANEL

    //InitOSD(); //Move to CE_JOB_DISPATCH_Task for fast boot.

    AHC_OS_ReleaseSem(ahc_System_Ready);

    //InitKeyPad(); //Move to CE_JOB_DISPATCH_Task for fast boot.
	
    AHC_UartRegisterUartCmdList(sAhcUartCommand);

#ifdef CFG_BOOT_FLASH_MODE //may be defined in config_xxx.h, could be FLASH_OFF or FLASH_ON
	// Always set Flash ON or OFF when system start up.
	#if CFG_BOOT_FLASH_MODE
	    MenuSettingConfig()->uiFlashMode = FLASH_ON;
	#else
	    MenuSettingConfig()->uiFlashMode = FLASH_OFF;
	#endif
#endif
#ifdef CFG_BOOT_MOV_CLIP_TIME //may be defined in config_xxx.h, could be MOVIE_CLIP_TIME_OFF or MOVIE_CLIP_TIME_xMIN
	MenuSettingConfig()->uiMOVClipTime 	 = CFG_BOOT_MOV_CLIP_TIME;
#endif
#ifdef CFG_BOOT_SLIDESHOW_FILE //may be defined in config_xxx.h, could be SLIDESHOW_FILE_STILL
	// Force slideshow photo only
	MenuSettingConfig()->uiSlideShowFile = SLIDESHOW_FILE_STILL; 
#endif
#ifdef CFG_BOOT_CHECK_USB_KEY_FILE //may be defined in config_xxx.h, a file name such as "SD:\\aituvc.txt"
	// Check USB Key file to run PCCAM
	AHC_CheckUSB_KeyFile(CFG_BOOT_CHECK_USB_KEY_FILE, 1 /* 1 for PCCAM */);
#endif
#ifdef CFG_BOOT_LCD_ROTATE //may be defined in config_xxx.h, could be LCD_ROTATE_OFF
    // Init LCD direction, always is normal
	MenuSettingConfig()->uiLCDRotate = CFG_BOOT_LCD_ROTATE;
#endif
#ifdef CFG_BOOT_FORCE_TURN_OFF_WIFI //	defined in config_xxx.h, for project of key control Wi-Fi On/Off
	Setpf_WiFi(WIFI_MODE_OFF);
#endif
#ifdef CFG_BOOT_FORCE_TURN_ON_WIFI	//	defined in config_xxx.h, for project of key control Wi-Fi On/Off
	Setpf_WiFi(WIFI_MODE_ON);
#endif
#ifdef CFG_VIDEO_FORMAT_SELECT_BY_SWITCH
	CFG_VIDEO_FORMAT_SELECT_BY_SWITCH();
#endif

#if(MENU_REARCAM_TYPE_EN)
	if(SCD_CAM_NONE != MMP_GetScdCamType()){
		pf_RearCamType_EnSet(USB_CAM_SONIX_MJPEG2H264 + MMP_GetScdCamType());
	}
	else {
		pf_RearCamType_EnSet(MMP_GetUSBCamType());
	}
#endif
    
    //Setpf_WiFi(WIFI_MODE_ON);
    
    uiStateSystemStart();

	AHC_PreSetLCDDirection();

    #if 0//#ifdef CFG_BOOT_CHECK_USB_UPDATE_FILE //may be defined in config_xxx.h, a file name such as "SD:\\FLYTECAM.bin"
    if(AHC_IsUsbConnect()) {
	    if(IsAdapter_Connect()) {
		   SDUpdateCheckFileExisted(CFG_BOOT_CHECK_USB_UPDATE_FILE);
	    } 
	    else {
		    NotifyUSB_HwStatus(1);
	    }
    }	
    #endif

#if (VR_VIDEO_TYPE==COMMON_VR_VIDEO_TYPE_3GP)
    AHC_VIDEO_SetMovieConfig(0, AHC_CLIP_CONTAINER_TYPE     , AHC_MOVIE_CONTAINER_3GP);
#elif (VR_VIDEO_TYPE==COMMON_VR_VIDEO_TYPE_AVI)
    AHC_VIDEO_SetMovieConfig(0, AHC_CLIP_CONTAINER_TYPE     , AHC_MOVIE_CONTAINER_AVI);
#endif

#if (TASK_MONITOR_ENABLE)
    memcpy(&gsTaskMonitorAhcMain.TaskName[0], __func__, TASK_MONITOR_MAXTASKNAMELEN);
    gsTaskMonitorAhcMain.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_WAITING;
    gsTaskMonitorAhcMain.ulExecTime = 0;
    memset((void *)gsTaskMonitorAhcMain.ParaArray, 0x0, sizeof(gsTaskMonitorAhcMain.ParaArray));
    gsTaskMonitorAhcMain.ulParaLength = 0;    
    gsTaskMonitorAhcMain.pTimeoutCB = (TASK_MONITOR_TimeoutCallback *)NULL;
    MMPF_TaskMonitor_RegisterTask(&gsTaskMonitorAhcMain);
    //MMPF_TaskMonitor_RegisterGblCB((void *)&AHC_PrintUITask);
    MMPF_TaskMonitor_RegisterGblCB((void *) NULL);
#endif

#if defined(AIT_HW_WATCHDOG_ENABLE) && (AIT_HW_WATCHDOG_ENABLE)
        AHC_WD_Enable(AHC_TRUE);
#endif

#ifndef	NO_PANEL
		// InitOSD will make LCD off, Shutdown LCD backlight must before,
		// Otherwise, Some kind of LCD will show ugly white lines
		//LedCtrl_LcdBackLight(AHC_FALSE);
		//LedCtrl_LcdBackLightLock(AHC_TRUE);
#endif	// NO_PANEL

#if defined(SUPPORT_SPEECH_RECOG)&&(SUPPORT_SPEECH_RECOG)
	MMPS_Sensor_StartSpeechRecog(0,MMP_TRUE);
	MMPS_AUDIO_InitLiveRecord(MMP_AUDIO_PCM_ENCODE, 0, 16000, (MMP_LivePCMCB)SpeechPCMTransferCB);
	MMPS_LiveAudio_StartRecord();
	MMPS_StartSpeechRecog((MMP_SpeechRecogCbFunc)SpeechRecogRunCB);
#endif

    while(1)
    {
        UINT32 uiMsgId, uiParam1, uiParam2;
		UINT16 usCount;

        while (1) {
    		AHC_GetAHLHPMessageCount(&usCount);

    		if(usCount < AHC_HP_MSG_QUEUE_SIZE) {
    			if(AHC_GetAHLMessage_HP( &uiMsgId, &uiParam1, &uiParam2) == AHC_FALSE) {
    			    if (AHC_GetAHLMessage( &uiMsgId, &uiParam1, &uiParam2) == AHC_FALSE) {
        			    continue;
    			    }

			        break;
    			}

			    break;
    		}
    		else if (AHC_GetAHLMessage( &uiMsgId, &uiParam1, &uiParam2) == AHC_FALSE) {
		        continue;
    		}

		    break;
        }

#if (TASK_MONITOR_ENABLE)
        MMPF_OS_GetTime(&gsTaskMonitorAhcMain.ulExecTime);
        gsTaskMonitorAhcMain.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_RUNNING;    

        *(MMP_ULONG *)(gsTaskMonitorAhcMain.ParaArray) = (MMP_ULONG)uiMsgId;
        gsTaskMonitorAhcMain.ulParaLength = sizeof(MMP_ULONG);  

        *(MMP_ULONG *)(gsTaskMonitorAhcMain.ParaArray + gsTaskMonitorAhcMain.ulParaLength) = (MMP_ULONG)uiParam1;
        gsTaskMonitorAhcMain.ulParaLength += sizeof(MMP_ULONG);  
        
        *(MMP_ULONG *)(gsTaskMonitorAhcMain.ParaArray + gsTaskMonitorAhcMain.ulParaLength) = (MMP_ULONG)uiParam2;
        gsTaskMonitorAhcMain.ulParaLength += sizeof(MMP_ULONG);          
#endif

        AHC_OS_AcquireSem(ahc_System_Ready, 0);

		uiStateMachine(uiMsgId, uiParam1, uiParam2);

        AHC_OS_ReleaseSem(ahc_System_Ready);

#if (TASK_MONITOR_ENABLE)
        gsTaskMonitorAhcMain.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_WAITING;
#endif
    }
}

void AHC_WorkStack_Task(void *p_arg)
{
	#define WORK_STACK_WAIT_UINT       50   // ms
	#define RTC_UPDATE_PERIOD          1    // ms, RTC_UPDATE_PERIOD * WORK_STACK_WAIT_UINT
	#define CHARGING_CHECK_PERIOD      3    // ms, CHARGING_CHECK_PERIOD * WORK_STACK_WAIT_UINT

    UINT32 uiTaskCounter = 0;

	AHC_WaitForBootComplete();

#if (TASK_MONITOR_ENABLE)
    memcpy(&gsTaskMonitorAHCWorkStack.TaskName[0], __func__, TASK_MONITOR_MAXTASKNAMELEN);
    gsTaskMonitorAHCWorkStack.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_WAITING;
    gsTaskMonitorAHCWorkStack.ulExecTime = 0;
    memset((void *)gsTaskMonitorAHCWorkStack.ParaArray, 0x0, sizeof(gsTaskMonitorAHCWorkStack.ParaArray)); 
    gsTaskMonitorAHCWorkStack.ulParaLength = 0;    
    gsTaskMonitorAHCWorkStack.pTimeoutCB = (TASK_MONITOR_TimeoutCallback *)NULL;       
    MMPF_TaskMonitor_RegisterTask(&gsTaskMonitorAHCWorkStack);
#endif
    
    while(1)
    {
    	// TODO: This task only for draw Battery Icon!?

        uiTaskCounter++;
#if (TASK_MONITOR_ENABLE)
        MMPF_OS_GetTime(&gsTaskMonitorAHCWorkStack.ulExecTime);
        gsTaskMonitorAHCWorkStack.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_RUNNING;    
        *(MMP_ULONG *)(gsTaskMonitorAHCWorkStack.ParaArray) = (MMP_ULONG)uiTaskCounter;
        gsTaskMonitorAHCWorkStack.ulParaLength = sizeof(MMP_ULONG); 
#endif            
        
        ///////////////////////////////////////////////////////////////////////////////
        // RTC Update Counter
        ///////////////////////////////////////////////////////////////////////////////
        if((uiTaskCounter % RTC_UPDATE_PERIOD) == 0) {
            AHC_SetParam(PARAM_ID_USE_RTC, 1);
            AHC_RTC_UpdateTime();
        }

        #if (OSD_SHOW_BATTERY_STATUS)
        if ((uiTaskCounter % CHARGING_CHECK_PERIOD) == 0 && gAHC_InitialDone) {
    		if ((AHC_TRUE == AHC_Charger_GetStatus()))
    		{
    			AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_UPDATE_CHARGE_ICON, 0);
    		}
        }
        #endif

        #if (TASK_MONITOR_ENABLE)
        gsTaskMonitorAHCWorkStack.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_WAITING;        
        #endif
        
        AHC_OS_SleepMs(WORK_STACK_WAIT_UINT);
    }
}
extern UINT32				Motion_Parking_Rec;

void AHC_WorkJob_Task(void *p_arg)
{
    static MMP_ULONG Count = 0;
    static MMP_ULONG Count_Det = 0,LowPower_Count=0,NoPower_count=0;
    static int 	acc_det=0,first_det1=0,acc_det2=0,acc_det3=0,B_det=0,old_acc_det=0;
    static int	acc_det_count=5;
	static int	acc_count1=0,acc_count2=0;
    INT32		level=0;
	INT32		timetemp=0;
	INT32		ledflog,ledflog1=0;
#if (GPS_CONNECT_ENABLE)    
    static MMP_ULONG ulGPSInfoRefreshCnt;
#endif
    MMPF_PIO_EnableOutputMode(MMP_GPIO40, MMP_FALSE, MMP_TRUE);
	MMPF_PIO_EnableOutputMode(MMP_GPIO60, MMP_FALSE, MMP_TRUE);

    MMPF_PIO_EnableOutputMode(MMP_GPIO25, MMP_FALSE, MMP_TRUE);//旧版B+是25，acc是40
#if (TASK_MONITOR_ENABLE)
    memcpy(&gsTaskMonitorAHCWorkStack.TaskName[0], __func__, TASK_MONITOR_MAXTASKNAMELEN);
    gsTaskMonitorAHCWorkStack.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_WAITING;
    gsTaskMonitorAHCWorkStack.ulExecTime = 0;
    memset((void *)gsTaskMonitorAHCWorkStack.ParaArray, 0x0, sizeof(gsTaskMonitorAHCWorkStack.ParaArray)); 
    gsTaskMonitorAHCWorkStack.ulParaLength = 0;    
    gsTaskMonitorAHCWorkStack.pTimeoutCB = (TASK_MONITOR_TimeoutCallback *)NULL;       
    MMPF_TaskMonitor_RegisterTask(&gsTaskMonitorAHCWorkStack);
#endif

    while(1)
    {    
#if (TASK_MONITOR_ENABLE)
        MMPF_OS_GetTime(&gsTaskMonitorAHCWorkStack.ulExecTime);
        gsTaskMonitorAHCWorkStack.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_RUNNING;    
#endif            
        Count_Det++;
	 if(Count_Det>100)
 	{
 		Count_Det=1;
		SDFORMATE++;
		//printc("LcdBrightCount=%d\n",SDFORMATE);
 	}
 	// if(SDFORMATE>60480)
 	#if 0 // lyj 20180712
 	if(SDFORMATE>7*24*360)//for test
	{	
		timetemp=MenuSettingConfig()->uiWDR;
		printc("LcdBrightCount=%d\n",SDFORMATE);
		printc("MenuSettingConfig()->uiWDR;=%d\n",MenuSettingConfig()->uiWDR);	
		if(timetemp==0)
		{
			SDFORMATE=0 ;
			//QuickMediaOperation(MEDIA_CMD_FORMAT);
			AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, EVENT_TV_FORMAT_MEDIA, 0);

		}
		else if(SDFORMATE>=timetemp*15*24*360)
		{
			//QuickMediaOperation(MEDIA_CMD_FORMAT);
			SDFORMATE=0;
			AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, EVENT_TV_FORMAT_MEDIA, 0);

		}
		else if(SDFORMATE>=timetemp*30*24*360)
		{
			//QuickMediaOperation(MEDIA_CMD_FORMAT);
			SDFORMATE=0;
			AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, EVENT_TV_FORMAT_MEDIA, 0);

		}

	 }
	#endif
	 
        if(AHC_GetWaitIconState()) 
        {
    	    AHC_DrawWaitIcon();  
        }
        
        if(AHC_WMSG_States()) 
        {
            Count++;
            if(AHC_WMSG_IsTimeOut(Count*100)) {
                Count = 0;
            }    
        }

#if (GPS_CONNECT_ENABLE) 
		if( uiGetCurrentState() >= UI_CAMERA_MENU_STATE    &&
			uiGetCurrentState() <= UI_CLOCK_SETTING_STATE)
		{
			ulGPSInfoRefreshCnt++;
			if(ulGPSInfoRefreshCnt == 10)
			{
				ulGPSInfoRefreshCnt = 0;
				AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_UPDATE_MESSAGE, 0);
			}
		}
#endif

    //Print below to check ADC key voltage, ex: ADC_KEY_VOLT_REC

/********rec led **********/	
	if(Count_Det%5==0)//500ms检测一次
	{
		if(VideoFunc_RecordStatus()&&!(uiGetParkingModeEnable()||VideoFunc_LockFileEnabled()))
		{
		
			if(ledflog)
			{
				ledflog=0;
				MMPF_PIO_SetData(MMP_GPIO28, POWER_EN_GPIO_LEVEL, MMP_TRUE);
			}
			else
			{
				ledflog=1;
				MMPF_PIO_SetData(MMP_GPIO28, !POWER_EN_GPIO_LEVEL, MMP_TRUE);
			}
		}
		
		if(VideoFunc_RecordStatus()&&(uiGetParkingModeEnable()||VideoFunc_LockFileEnabled()))
		{
			
		}
		else
		{
		
			if(ledflog1)
			{
				ledflog1=0;
				MMPF_PIO_SetData(MMP_GPIO29, POWER_EN_GPIO_LEVEL, MMP_TRUE);
			}
			else
			{
				ledflog1=1;
				MMPF_PIO_SetData(MMP_GPIO29, !POWER_EN_GPIO_LEVEL, MMP_TRUE);

			}
		}
	}
	
	if(Count_Det%2==0)//200ms检测一次
	{
		if(VideoFunc_RecordStatus()&&(uiGetParkingModeEnable()||VideoFunc_LockFileEnabled()))
		{
		
			if(ledflog)
			{
				ledflog=0;
				MMPF_PIO_SetData(MMP_GPIO28, POWER_EN_GPIO_LEVEL, MMP_TRUE);
				MMPF_PIO_SetData(MMP_GPIO29, POWER_EN_GPIO_LEVEL, MMP_TRUE);

			}
			else
			{
					ledflog=1;
					MMPF_PIO_SetData(MMP_GPIO28, !POWER_EN_GPIO_LEVEL, MMP_TRUE);
					MMPF_PIO_SetData(MMP_GPIO29, !POWER_EN_GPIO_LEVEL, MMP_TRUE);
			}
		}
	}
#if 1
	if(0){
					MMP_USHORT saradc_data;
					MMP_USHORT TEMP_data;
					
				   AHC_SARADC_ReadData(ADC_KEY_SARADC_CHANNEL, &saradc_data);//
				  // printc("ADC voltage = %d\r\n", saradc_data);
				 
				 if(saradc_data>=2000)
				 	saradc_data=2000;
						TEMP_data=95-saradc_data*47/1000;
				//printc("ADC voltage = %dTEMP_data=%d\r\n", saradc_data,TEMP_data);
				
				 if(TEMP_data>85&&TEMP_flog==0)
			 	{
					 if(VideoFunc_RecordStatus()){
					 				 printc("@@@ TEMP_data==%d\r\n",TEMP_data);//温度检测及其不准
					 				// printc("@@@ BUTTON_REC_REL22 \r\n");
									 AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_REC_REL, 0);
						// AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_COVER_CLOSED, 0);
						if (LedCtrl_GetBacklightStatus())
								LedCtrl_LcdBackLight(AHC_FALSE);
					 	}

				TEMP_flog=1;


			 	}
				 else if(TEMP_data<80)
			 	{
			 		if(TEMP_flog)
					{
				 		if(!VideoFunc_RecordStatus())
				 			{ 	printc("@@@ BUTTON_REC_REL 11\r\n");
								AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_REC_REL, 0);

							}
						if (!LedCtrl_GetBacklightStatus())
								LedCtrl_LcdBackLight(AHC_TRUE);
						TEMP_flog=0;
				 	}
					TEMP_flog=0;
			 	}
		}

#endif
/********acc det and B+ det**********/		//acc det

	/////B+ DET
	/*	MMPF_PIO_GetData(MMP_GPIO25, &B_det);
			if(B_det&&NoPower_count<=2)
			{
					NoPower_count++;
				if(NoPower_count>=2)
					AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_LPRESS, 0);
			}
			if(B_det==0)
				NoPower_count=0;
			*/
	if(Count_Det%10==0)//没一秒检测一次
	{
		
	/********auto close backlight**********/
		switch(MenuSettingConfig()->uiLCDBrightness)//auto close backlight
		{
			case 0:
				DeskClock=0;
				if(LcdBrightCount<=60)
					LcdBrightCount++;
				//if(LcdBrightCount%20==0)
				//	printc("LcdBrightCount=%d\n",LcdBrightCount);
				if((LcdBrightCount>10*6)&&(LedCtrl_GetBacklightStatus()))
				{
					//AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_COVER_CLOSED, 0);
					DeskClock=1;
				}
				break;
			case 1:
				DeskClock=0;
				if(LcdBrightCount<=60*3)
					LcdBrightCount++;
				if((LcdBrightCount>10*6*3)&&(LedCtrl_GetBacklightStatus()))
				{
					//AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_COVER_CLOSED, 0);
					DeskClock=1;
				}
				break;
			//case 2:
			//	DeskClock=0;
			//	LcdBrightCount=0;
			//	break;
			case 2:
				DeskClock=0;
				LcdBrightCount=0;
				break;
			default:
				break;
				
		}
		//printc("SettingConfig()->uiWNR=%d\n\r",MenuSettingConfig()->uiWNR);
		#if 1
		MMPF_PIO_GetData(MMP_GPIO25, &acc_det);//gpio25 acc det, B+  ADC det
		#else
		MMPF_PIO_GetData(MMP_GPIO40, &acc_det);
		#endif
		//MMPF_PIO_GetData(MMP_GPIO40, &acc_det);
		//MMPF_PIO_GetData(MMP_GPIO25, &acc_det2);
		//MMPF_PIO_GetData(MMP_GPIO60, &acc_det3);
		
		//printc("MMPF_PIO_GetData(MMP_GPIO25)==%d\n",acc_det);
		//printc("MMPF_PIO_GetData(MMP_GPIO40)==%d\n",acc_det2);
		//printc("MMPF_PIO_GetData(MMP_GPIO60)==%d\n",acc_det3);
		
		if(first_det1<5)
		{
			first_det1++;
		}
		else if(acc_det!=old_acc_det)
		{
			if(acc_count2<10)
				acc_count2++;
			if((acc_det==1)&&(acc_count2>2))//acc 断开
			{	
				old_acc_det=acc_det;
				acc_count2=0;
				pf_General_EnGet(COMMON_KEY_B_VOL, &level);
				if(level<600){


				}else{
				printc("VideoFunc_RecordStatus()=%d,acc_det=%d\n",VideoFunc_RecordStatus(),acc_det);
				
				if(VideoFunc_RecordStatus())
						AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_REC_REL, 0);
				Motion_Parking_Rec=0;
				AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_MENU_LPRESS, 0);
				//AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_COVER_CLOSED, 0);
				if (LedCtrl_GetBacklightStatus())
								LedCtrl_LcdBackLight(AHC_FALSE);
				printc("AHC_WorkJob_Task--send BUTTON_MENU_LPRESS message\n");
				old_acc_det=acc_det;
			}
			}
			else if((acc_det==0)&&(acc_count2>2))//acc 连接
			{
				old_acc_det=acc_det;
				acc_count2=0;
				if(VideoFunc_RecordStatus()){
					 printc("@@@ BUTTON_REC_REL \r\n");
					AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_REC_REL, 0);

						}
					AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_MENU_LPRESS, 0);
				if (!LedCtrl_GetBacklightStatus())
	             			LedCtrl_LcdBackLight(AHC_TRUE);
				printc("AHC_WorkJob_Task--send BUTTON_REC_REL message\n");
				old_acc_det=acc_det;
			}
		}
		else if(acc_det==old_acc_det)
			acc_count2=0;
		
		/*************防止正常录像模式，不开屏***************/
		if(acc_det==0&&acc_count1<10)
			acc_count1++;
		else 
			acc_count1=0;
		if(acc_count1>5)
			{
			if(uiGetParkingModeEnable()==1)
				{

					if(VideoFunc_RecordStatus())
						{ printc("@@@ BUTTON_REC_REL 333 \r\n");
						AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_REC_REL, 0);
						}
						AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_MENU_LPRESS, 0);

				}


			}
		/****************************/
		if(acc_det)
		{
			//if(acc_det_count>0)
			//	acc_det_count--;
			//if(acc_det_count==0)
			//	AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_LPRESS, 0);
			//low power det
			pf_General_EnGet(COMMON_KEY_B_VOL, &level);
			//printc("AHC_WorkJob_Task--level==%d\n",level);
			if(level<305)
			{		
				AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
			}

			switch(MenuSettingConfig()->uiSlideShowEffect)
			{
				case 0:
					if(level<705)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							{
								
							//printc("AHC_WorkJob_Task--send BUTTON_POWER_LPRESS message\n");
							//printc("AHC_WorkJob_Task--send BUTTON_POWER_LPRESS message\n");
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
							}
					}
					else
						LowPower_Count=0;
					break;
				case 1:
					if(level<720)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
				case 2:
					if(level<732)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
				case 3:
					if(level<745)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
					
				case 5:
					if(level<905)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
				case 6:
					if(level<917)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
				case 7:
					if(level<935)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
				case 8:
					if(level<955)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
					
				case 10:
					if(level<1661)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					
					else
						LowPower_Count=0;
					break;
				case 11:
					if(level<1673)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					break;
				case 12:
					if(level<1685)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
				case 13:
					if(level<1697)
					{
						if(LowPower_Count<=60)
							LowPower_Count++;
						if(LowPower_Count>60)
							AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_POWER_REL, 0);
					}
					else
						LowPower_Count=0;
					break;
					
				default:
					LowPower_Count=0;
					break;
			}
		}
		//else 
		//	acc_det_count=5;

		
		//printc("AHC_WorkJob_Task acc_det=%d;acc_det_count=%d;B_det=%d;\n",acc_det,acc_det_count,B_det);

		///桌上面时钟
		if((DeskClock)||(DeskClock==0&&OldDeskClock==1))
		{
				AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, EVENT_DESK_CLOCK_UPDATE, 0);
		}
		OldDeskClock=DeskClock;
		if(Icon_Clear_Count<10)
			Icon_Clear_Count++;
		if((Icon_Clear_Count==10)&&(Icon_Clear_Flag==0))
		{
			       AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, EVENT_DESK_CLOCK_UPDATE, 0);
		}
		else if((Icon_Clear_Count<10)&&(Icon_Clear_Flag==1))
		{
				AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, EVENT_DESK_CLOCK_UPDATE, 0);
		}
		
	}

#if defined(AIT_HW_WATCHDOG_ENABLE) && (AIT_HW_WATCHDOG_ENABLE)
        AHC_WD_Kick();
#endif

#if (TASK_MONITOR_ENABLE)
        gsTaskMonitorAHCWorkStack.sTaskMonitorStates = MMPF_TASK_MONITOR_STATES_WAITING;        
#endif

		#if (defined(CUS_ADAS_OUTPUT_LOG) && ADAS_OUTPUT_LOG == 1)
		{
            extern MMP_BOOL FlagSetWrAdasLog;
            extern void ADAS_write_log(void);
            if (FlagSetWrAdasLog)
            {
                FlagSetWrAdasLog = MMP_FALSE;
                ADAS_write_log();
            }
		}
		#endif

        AHC_OS_SleepMs(100);
    }
}

#if 0
void ________Uart_Function_________(){ruturn;} //dummy
#endif

void UartCmdSetMode( char* szParam )
{
    int iMode;

    sscanfl( szParam, "%x",&iMode);

    if( iMode == 0xFF )
    {
        printc("AHC_MODE_IDLE = 0x00,              \n");
        printc("AHC_MODE_CAPTURE_PREVIEW = 0x10,   \n");
        printc("AHC_MODE_DRAFT_CAPTURE = 0x11,     \n");
        printc("AHC_MODE_STILL_CAPTURE = 0x20,     \n");
        printc("AHC_MODE_C2C_CAPTURE = 0x21,       \n");
        printc("AHC_MODE_SEQUENTIAL_CAPTURE = 0x22,\n");
        printc("AHC_MODE_USB_MASS_STORAGE = 0x30,  \n");
        printc("AHC_MODE_USB_WEBCAM = 0x31,        \n");
        printc("AHC_MODE_PLAYBACK  = 0x40,         \n");
        printc("AHC_MODE_THUMB_BROWSER = 0x50,     \n");
        printc("AHC_MODE_VIDEO_RECORD = 0x60,      \n");
        printc("AHC_MODE_RECORD_PREVIEW = 0x62,    \n");
        printc("AHC_MODE_CALIBRATION = 0xF0,       \n");
        printc("AHC_MODE_MAX = 0xFF                \n");
    }
    else
    {
        AHC_SetMode( iMode );
    }  
}

void UartCmdSetUiMode( char* szParam )
{
    int uiMode;

    sscanfl( szParam, "%x",&uiMode);

    if( uiMode == 0xFF )
    {
        printc("UI_CAMERA_STATE    		= 0x00, \n");
        printc("UI_VIDEO_STATE     		= 0x01, \n");
        printc("UI_BROWSER_STATE   		= 0x02, \n");
        printc("UI_PLAYBACK_STATE  		= 0x03, \n");
        printc("UI_SLIDESHOW_STATE 		= 0x04, \n");
        printc("UI_MSDC_STATE      		= 0x05, \n");
        //printc("UI_HDMI_STATE      		= 0x06, \n");
        //printc("UI_TVOUT_STATE     		= 0x07, \n");
        printc("UI_CAMERA_MENU_STATE   	= 0x08, \n");
        printc("UI_VIDEO_MENU_STATE    	= 0x09, \n");
        printc("UI_PLAYBACK_MENU_STATE 	= 0x0A, \n");
    }
    else
    {
        StateSwitchMode( uiMode );
    }   
}

void UartCmd_Main(char* szParam)
{
    UINT16 uiItem   = 0;
    UINT16 uiParam1 = 0;
    UINT16 uiParam2 = 0;
    UINT16 uiParam3 = 0;
    UINT16 uiParam4 = 0;
    UINT16 uiParam5 = 0;
    UINT16 uiParam6 = 0;
	
	sscanfl( szParam, "%d %d %d %d %d %d %d",&uiItem, &uiParam1, &uiParam2, &uiParam3, &uiParam4, &uiParam5, &uiParam6 );

    switch( uiItem ) 
    {
        case 0:     
            break;
            
        case 1: 
            break;
                
        case 2: 
            break;
    }
}

UARTCOMMAND sAhcUartCommand[] =
{
    { "mode",          " Mode ",   		   "AHC_SetMode",      			UartCmdSetMode }, 
    { "uimode",        " uiMode ",   	   "UI_SetMode",      			UartCmdSetUiMode },
    { "ucm",           "",                 "Test Uart command",         UartCmd_Main},
    {0,0,0,0}
};

AHC_BOOL MenuFunc_CheckAvailableSpace( UINT8 uiCameraState )
{
    AHC_BOOL bIsAvailableSpace = AHC_TRUE;

    printc("### %s -\r\n", __func__);
        
    switch( uiCameraState )
    {
    case UI_VIDEO_STATE:
        {
            UBYTE bHour = 0, bMin = 0, bSec = 0;

            AHC_VIDEO_AvailableRecTime(&bHour, &bMin, &bSec);
            
            if ((bHour + bMin + bSec) == 0)
            {
                bIsAvailableSpace = AHC_FALSE;
            }
        }
        break;

    case UI_CAMERA_BURST_STATE:
        /*
        // TBD
        if( CameraSettingConfig()->uiRemainingShots < CaptureBurstFunc_GetShotNumber( MenuSettingConfig()->uiBurstShot ) )
        {
            bIsAvailableSpace = AHC_FALSE;
        }
        */
        break;

    default:
        {
            UINT64 freeBytes = 0;
            UINT32 remainCaptureNum = 0;
            
            CaptureFunc_CheckMemSizeAvailable(&freeBytes, &remainCaptureNum);

            if (remainCaptureNum)
            {
                bIsAvailableSpace = AHC_FALSE;
            }
        }
        break;
    }

    return bIsAvailableSpace;
}

