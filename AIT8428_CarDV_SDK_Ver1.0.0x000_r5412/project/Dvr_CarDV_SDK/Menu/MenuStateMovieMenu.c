/*===========================================================================
 * Include file 
 *===========================================================================*/ 

#include "AHC_Display.h"
#include "AHC_MACRO.h"
#include "AHC_Menu.h"
#include "AHC_Message.h"
#include "AHC_Common.h"
#include "AHC_GUI.h"
#include "AHC_Utility.h"
#include "AHC_Video.h"
#include "AHC_WarningMSG.h"
#include "Ahc_Parameter.h"
#include "AHC_General_CarDV.h"
#include "AHC_General.h"
#include "MenuCommon.h"
#include "MenuStateMovieMenu.h"
#include "MenuStateModeSelectMenu.h"
#include "MenuSetting.h"
#include "MenuStateMenu.h"
#include "MenuDrawingFunc.h"
#include "MenuDrawCommon.h" //For OVL_DISPLAY_BUFFER
#include "AHC_USB.h"
#include "AHC_Fs.h"
#if WIFI_PORT
#include "netapp.h"
#endif
#include "IconPosition.h"
#include "ColorDefine.h"
#include "OsdStrings.h"
#include "IconDefine.h"
#include "LedControl.h"
#if (SD_UPDATE_FW_EN)
#include "SD_Updater.h"
#endif
#include "snr_cfg.h"
#include "ait_utility.h"
#include "ParkingModeCtrl.h"

/*===========================================================================
 * External function
 *===========================================================================*/ 
AHC_BOOL IsAdapter_Connect(void);
AHC_BOOL VideoFunc_RecordStatus(void);
AHC_BOOL SwitchUIDrawSetVideoRecordMode(void);
AHC_BOOL StateVideoRecModeInitHDMI(void* pData);
UINT32 KeyParser_ClockSettingEvent(UINT32 ulMsgId, UINT32 ulEvent, UINT32 ulParam );
void InitOSD(void);

/*===========================================================================
 * Local function
 *===========================================================================*/ 

AHC_BOOL MenuItemMovieMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieQuality(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieStable(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemAutoSlowShutter(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMICSensitivity(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMoviePreRecord(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieClipTime(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieNightMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMoviePowerOffTime(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSoundRecord(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieVMDRecTime(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieAutoRec(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieTimelapse(PSMENUITEM pItem, AHC_BOOL bIsHover);	// VIDEO_REC_TIMELAPSE_EN
AHC_BOOL MenuItemMovieParkingMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieLDWSMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieFCWSMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSAGMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieHDRMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSlowMotionMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieWNRMode(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemRearCamType(PSMENUITEM pItem,AHC_BOOL bIsHover);

UINT32   MenuGetDefault_Movie(PSMENUSTRUCT pMenu);

AHC_BOOL MenuItemMovieVOLT(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieVOLT1(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieVOLT2(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSettingVol(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSettingTouchVol(PSMENUITEM pItem, AHC_BOOL bIsHover);

AHC_BOOL MenuItemMovieVOLT(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupLcd(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupLogo(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupGsensor(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupYesNo(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupMic(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupReset(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupGps(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupClock(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSettingFormat(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSettingFormat1(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSettingevent(PSMENUITEM pItem, AHC_BOOL bIsHover);
AHC_BOOL MenuItemMovieSetupYesNoEvent(PSMENUITEM pItem, AHC_BOOL bIsHover);




/*===========================================================================
 * Extern varible
 *===========================================================================*/  

extern SMENUITEM 	sItemScence;
extern SMENUITEM 	sItemEV;
extern SMENUITEM 	sItemWB;
extern SMENUITEM 	sItemISO;
extern SMENUITEM 	sItemColor;
extern SMENUITEM 	sItemEffect;
#if defined (CFG_LIST_STILL_IMAGE_SIZE) /*may be defined in config_xxx.h*/ && (MENU_STILL_SIZE_EN)
extern SMENUITEM 	sItemStillImageSize;
#endif
extern SMENUSTRUCT 	sSubClockSettings;

extern AHC_BOOL 	Deleting_InBrowser;
extern AHC_BOOL 	Protecting_InBrowser;
extern AHC_BOOL 	bForceSwitchBrowser;
#ifdef CFG_MENU_MOVIE_EXIT_TO_PREV
extern AHC_BOOL		bForceSwitchCameraMode;
#endif
extern AHC_BOOL 	m_ubAtMenuTab;
extern SMENUITEM 	sItemContrast;
extern SMENUITEM 	sItemSaturation;
extern SMENUITEM 	sItemSharpness;
extern SMENUITEM 	sItemGamma;

extern MMP_UBYTE    gubWpFileNum;
extern AHC_BOOL     gbAhcDbgBrk;
#if (HDMI_ENABLE)
//extern AHC_BOOL		bHaveInitOsdHdmi;
#endif
#define IDS_DS_PARKING_MODE IDS_DS_POWERON_GSENSOR_SENSITIVETY

/*===========================================================================
 * Global varible : Menu
 *===========================================================================*/

SMENUSTRUCT sMainMenuVideo;

SMENUSTRUCT sSubMovieMode;
SMENUSTRUCT sSubMovieQuality;
SMENUSTRUCT sSubMovieStable;
SMENUSTRUCT sSubAutoSlowShutter;
SMENUSTRUCT sSubMICSensitivity;
SMENUSTRUCT sSubMoviePreRecord;
SMENUSTRUCT sSubMovieClipTime;
SMENUSTRUCT sSubMovieNightMode;
SMENUSTRUCT sSubMoviePowerOffTime;
SMENUSTRUCT sSubMovieSoundRecord;
SMENUSTRUCT sSubMovieVMDRecTime;
SMENUSTRUCT sSubMovieAutoRec;
SMENUSTRUCT sSubMovieTimelapse;		// VIDEO_REC_TIMELAPSE_EN
SMENUSTRUCT sSubMovieParkingMode;
SMENUSTRUCT sSubMovieLDWSMode;
SMENUSTRUCT sSubMovieFCWSMode;
SMENUSTRUCT sSubMovieSAGMode;
SMENUSTRUCT sSubMovieHDRMode;
SMENUSTRUCT sSubMovieSlowMotionMode;
SMENUSTRUCT sSubMovieWNRMode;

SMENUSTRUCT sMainMenuVideo;
SMENUSTRUCT sSubMovieSetup;
SMENUSTRUCT sSubMovieSetting;
SMENUSTRUCT sSubMovieSettingvolt;
SMENUSTRUCT sSubMovieSettingvolt1;
SMENUSTRUCT sSubMovieSettingvolt2;
SMENUSTRUCT sSubMovieSettingvol;

SMENUSTRUCT sSubMovieSettingformat;
SMENUSTRUCT sSubMovieSettingevent;
SMENUSTRUCT sSubMovieSettingtouch;
SMENUSTRUCT sSubMovieSettingtime;
SMENUSTRUCT sSubMovieSettingsystem;
SMENUSTRUCT sSubMovieSettingbatteryh;
SMENUSTRUCT sSubRearCamType;
SMENUSTRUCT sSubMovieplayback ;
SMENUSTRUCT sSubMovieplayback0 ;
SMENUSTRUCT sSubMovieplayback1 ;
SMENUSTRUCT sSubMovieplayback2 ;

SMENUSTRUCT sSubMovieSetupLcd;
SMENUSTRUCT sSubMovieSetupLogo;
SMENUSTRUCT sSubMovieSetupLogo_1080;
SMENUSTRUCT sSubMovieSetupGsensor;
SMENUSTRUCT sSubMovieSetupMic;
SMENUSTRUCT sSubMovieSetupReset;
SMENUSTRUCT sSubMovieSetupGps;
SMENUSTRUCT sSubMovieSettingformat1;
SMENUSTRUCT sSubMovieSettingParking;

/*===========================================================================
 * Global varible : Item Structure
 *===========================================================================*/ 
//leo20180630

SMENUITEM sItemMovieSetting_parking_rec  = { ITEMID_MOVIE_PARKING_REC,		&bmparking_rec_button_normal, IDS_TIME_2MIN,		&sSubMovieplayback0, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetting_event_rec	= { ITEMID_MOVIE_EVENT_REC,	&bmevent_rec_button_normal, IDS_TIME_2MIN,        	&sSubMovieplayback1, 0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetting_rec_button	= { ITEMID_MOVIE_REC_BOTTON,	&bmrec_button_normal, IDS_TIME_2MIN,       	&sSubMovieplayback2, 0, 0,AHC_TRUE, NULL };



SMENUITEM sItemMovieMode          		= { ITEMID_MOVIE_MODE, 			&BMICON_MENU_REC_MODE,    	IDS_DS_MOVIE_MODE,			/*&sSubMovieMode*/NULL, 0, 		 0, AHC_TRUE, NULL };
SMENUITEM sItemPlayBackMode          		= { ITEMID_MOVIE_MODE, 			&BMICON_MENU_REC_MODE,    	IDS_DS_MOVIE_MODE,			&sSubMovieplayback, 0, 		 0, AHC_TRUE, NULL };
SMENUITEM sItemSettingMode          		= { ITEMID_MOVIE_MODE, 			&BMICON_MENU_REC_MODE,    	IDS_DS_MOVIE_MODE,			&sSubMovieSetting, 0, 		 0, AHC_TRUE, NULL };
SMENUITEM sItemSetupMode          		= { ITEMID_MOVIE_MODE, 			&BMICON_MENU_REC_MODE,    	IDS_DS_MOVIE_MODE,			&sSubMovieSetup, 0, 		 0, AHC_TRUE, NULL };


SMENUITEM sItemMovieSetupLcd		= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmIcon_lcd_button_normal, IDS_TIME_2MIN,		&sSubMovieSetupLcd, 0, 0, AHC_TRUE, NULL };
#if (MENU_MOVIE_SIZE_1080P_EN||MENU_MOVIE_SIZE_SHD_30P_EN)
SMENUITEM sItemMovieSetupLogo		= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmIcon_start_screen_button_normal, IDS_TIME_2MIN,        	&sSubMovieSetupLogo_1080,    	0, 0,AHC_TRUE, NULL };
#else
SMENUITEM sItemMovieSetupLogo		= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmIcon_start_screen_button_normal, IDS_TIME_2MIN,        	&sSubMovieMode/*sSubMovieSetupLogo*/,    	0, 0,AHC_TRUE, NULL };
#endif
SMENUITEM sItemMovieSetupGsensor	= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmIcon_sensor_button_normal, IDS_TIME_2MIN,       	&sSubMovieSetupGsensor,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupMic		= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmIcon_mic_button_normal, IDS_TIME_2MIN,       	&sSubMovieSetupMic,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupReset		= { ITEMID_MOVIE_POWEROFF_TIME_1MIN,	&bmIcon_reset_button_normal, IDS_TIME_2MIN,       	&sSubMovieSetupReset,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupGps		= { ITEMID_MOVIE_POWEROFF_TIME_1MIN,	&bmIcon_gps_button_normal, IDS_TIME_2MIN,       	&sSubMovieSetupGps,    	0, 0,AHC_TRUE, NULL };

SMENUITEM sItemMovieSetupLcd1    	= { 0, 		&bmIcon_1min_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupLcd, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupLcd3    	= { 1, 		&bmIcon_3min_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupLcd, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupLcdFull    	= { 2, 		&bmIcon_full_time_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupLcd, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupLcdClock    	= { 3, 	&bmIcon_clock_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupLcd, 0, AHC_TRUE, NULL };


SMENUITEM sItemMovieSetupLogo1    	= { 0, 		&bmIcon_1_bt_normal,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupLogo2    	= { 1, 		&bmIcon_2_bt_normal,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupLogo3    	= { 2, 		&bmIcon_3_bt_normal,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetupLogo1_1080    	= { 0, 		&bmIcon_1_bt_normal_1080,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupLogo, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupLogo2_1080    	= { 1, 		&bmIcon_2_bt_normal_1080,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupLogo, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupLogo3_1080    	= { 2, 		&bmIcon_3_bt_normal_1080,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupLogo, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetting_event			= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,		&bmevent_bt_normal, IDS_TIME_2MIN,		&sSubMovieSettingevent, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetting_format		= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmformat_button_normal, IDS_TIME_2MIN,        	&sSubMovieSettingformat,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetting_system	= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmparkingpoweroff, IDS_TIME_2MIN,       	&sSubMovieSettingbatteryh/*sSubMovieSettingsystem*/,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetting_time	= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,	&bmtime_button_normal, IDS_TIME_2MIN,       	&sSubMovieSettingtime,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetting_sound		= { ITEMID_MOVIE_POWEROFF_TIME_1MIN,	&bmsound_button_normal, IDS_TIME_2MIN,       	&sSubMovieSettingvol,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSetting_touch		= { ITEMID_MOVIE_POWEROFF_TIME_1MIN,	&bmtouch_button_normal, IDS_TIME_2MIN,       	&sSubMovieSettingtouch,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieMode_volt		= { ITEMID_MOVIE_POWEROFF_TIME_1MIN,	&bmvolt_bt_normal, IDS_TIME_2MIN,       	&sSubMovieSettingvolt,    	0, 0,AHC_TRUE, NULL };
SMENUITEM sItemMoiveSetting_parking = { ITEMID_MOVIE_POWEROFF_TIME_5SEC,		&bmParkingmode, IDS_TIME_2MIN,		&sSubMovieSettingParking, 0, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetup9volt1    	= { 0, 		&bm9volt01_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup9volt2    	= { 1, 		&bm9volt02_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup9volt3    	= { 2, 		&bm9volt03_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup9volt4    	= { 3, 		&bm9volt04_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup9volt5    	= { 4, 		&bm24volt05_bt_nor,  IDS_DS_QUALITY_FINE,				&sSubMovieSettingvolt1,    	0, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetup12volt1    	= { 0, 		&bm12volt01_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT1, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup12volt2    	= { 1, 		&bm12volt02_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT1, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup12volt3    	= { 2, 		&bm12volt03_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT1, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup12volt4    	= { 3, 	&bm12volt04_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT1, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup12volt5    	= { 4, 	&bm9volt05_bt_nor,  IDS_DS_QUALITY_FINE,			&sSubMovieSettingvolt2,    	0, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetup24volt1    	= {0, 		&bm24volt01_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT2, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup24volt2    	= { 1, 		&bm24volt02_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT2, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup24volt3    	= { 2, 		&bm24volt03_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT2, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup24volt4    	= { 3, 	&bm24volt04_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieVOLT2, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetup24volt5    	= { 4, 	&bm12volt05_bt_nor,  IDS_DS_QUALITY_FINE,			&sSubMovieSettingvolt,    	0, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSettingvol0    	= { 0, 		&bmvol0_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingVol, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingvol1    	= { 1, 		&bmvol1_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingVol, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingvol2    	= { 2, 		&bmvol2_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingVol, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingvol3    	= { 3, 		&bmvol3_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingVol, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSettingparkingvolt0    	= { 0, 	&bmvoltsetting,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingparkingvolt1    	= { 1, 	&bmbatterysetting,  IDS_DS_QUALITY_FINE,	&sSubMovieSettingbatteryh, NULL, 0, AHC_TRUE, NULL };// lyj 20180704
SMENUITEM sItemMovieSettingbatteryh0    	= { 0, 	&bmbatterysetting2,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingbatteryh1    	= { 0, 	&bmfivehour,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingbatteryh2    	= { 0, 	&bmtenhour,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingbatteryh3    	= { 0, 	&bmtwentyfour,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingbatteryh4    	= { 0, 	&bmfortyeight,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingTouchSoundyes    	= { 0, 	&bmyes1_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingTouchVol, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingTouchSoundno    	= { 1, 	&bmno1_bt_nor,  IDS_DS_QUALITY_FINE,	NULL, MenuItemMovieSettingTouchVol, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSettingFormat0    	= { 0, 		&bmIcon_7day_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingFormat, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingFormat1    	= { 1, 		&bmIcon_15day_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingFormat, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingFormat2    	= { 2, 		&bmIcon_30day_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingFormat, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingFormat3    	= { 3, 		&bmIcon_format_sdcard_normal,  IDS_DS_QUALITY_FINE,	&sSubMovieSettingformat1,	0, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSettingyes    	= { 0, 	&bmIcon_yes_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupYesNo, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingno    	= { 1, 	&bmIcon_no_bt_nor,  IDS_DS_QUALITY_FINE,	NULL, MenuItemMovieSetupYesNo, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingyesno3    	= { 0, 	&bmparkingselect1,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingyes3    	= { 1, 	&bmparkingselect2,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingno3    	= { 2, 	&bmparkingselect3,  IDS_DS_QUALITY_FINE,	NULL, NULL, 0, AHC_TRUE, NULL };//lyj 20180702


SMENUITEM sItemMovieSettingyes1    	= { 0, 	&bmIcon_yes_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupYesNoEvent, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingno1   	= { 1, 	&bmIcon_no_bt_nor,  IDS_DS_QUALITY_FINE,	NULL, MenuItemMovieSetupYesNoEvent, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSettingFormatYes  	= { 0, 	&bmIcon_yes_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingFormat1, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingFormatNO   	= { 1, 	&bmIcon_no_bt_nor,  IDS_DS_QUALITY_FINE,	NULL, MenuItemMovieSettingFormat1, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingeventYes  	= { 0, 	&bmIcon_yes_bt_nor,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSettingevent, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSettingeventNO   	= { 1, 	&bmIcon_no_bt_nor,  IDS_DS_QUALITY_FINE,	NULL, MenuItemMovieSettingevent, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetupGsensor1    	= { 0, 		&bmIcon_sensor_level1_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupGsensor, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupGsensor2    	= { 1, 		&bmIcon_sensor_level2_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupGsensor, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupGsensor3    	= { 2, 		&bmIcon_sensor_level3_button_normal,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupGsensor, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetupReset0 	= { 0, 		&bmsm1,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupReset1    	= { 1, 		&bmsm2,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupReset2    	= { 2, 		&bmsm3,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetupGps0 	= { 0, 		&bmnight1_son,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupGps1   	= { 1, 		&bmnight2_soff,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupGps2   	= { 2, 		&bmnight3_sparking,  IDS_DS_QUALITY_FINE,		NULL, NULL, 0, AHC_TRUE, NULL };

SMENUITEM sItemMovieSetupYes1 	= { 0, 		&bmIcon_mic_info_on,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupMic, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSetupNo1    	= { 1, 		&bmIcon_mic_reco_off,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieSetupMic, 0, AHC_TRUE, NULL };

PSMENUITEM   sMenuListMainVideo[] =
{
    &sItemPlayBackMode,
    &sItemSettingMode,
    &sItemSetupMode,
    &sItemMovieMode,
};


SMENUSTRUCT   sMainMenuVideo =
{
    MENUID_MAIN_MENU_VIDEO,                               
    NULL,                                                 
    IDS_DS_MOVIE_MODE,                                 
    NULL,                                              
    sizeof(sMenuListMainVideo)/sizeof(PSMENUITEM),   
    sMenuListMainVideo,                               
    MainMenuEventHandler,                             
    MenuGetDefault,                                   
    0,                                                
    0,                                                
    0,                                                
    0                                               
};

PSMENUITEM   sMenuMovieSetup[] =
{
    &sItemMovieSetupLcd,
    &sItemMovieSetupLogo,        
    &sItemMovieSetupGsensor,
    &sItemMovieSetupMic,
    &sItemMovieSetupReset,
    &sItemMovieSetupGps,    
};

PSMENUITEM   sMenuMovieSetting[] =
{
	&sItemMovieMode_volt,
	&sItemMovieSetting_sound, 
	&sItemMovieSetting_format, 
    &sItemMovieSetting_time,
    &sItemMovieSetting_event,
    &sItemMovieSetting_touch,
     &sItemMovieSetting_system,
      &sItemMoiveSetting_parking,
};
PSMENUITEM   sMenuplayback[] =
{
	&sItemMovieSetting_parking_rec,
	&sItemMovieSetting_event_rec, 
	&sItemMovieSetting_rec_button, 
  
};


SMENUSTRUCT sSubMovieSetup =
{
    MENUID_SUB_MENU_SETUP,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetup)/sizeof(PSMENUITEM),
    sMenuMovieSetup,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieplayback =
{
    MENUID_SUB_MENU_PLAYBACK,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuplayback)/sizeof(PSMENUITEM),
    sMenuplayback,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieplayback0 =
{
    MENUID_SUB_MENU_PLAYBACK0,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuplayback)/sizeof(PSMENUITEM),
    sMenuplayback,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieplayback1 =
{
    MENUID_SUB_MENU_PLAYBACK1,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuplayback)/sizeof(PSMENUITEM),
    sMenuplayback,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieplayback2 =
{
    MENUID_SUB_MENU_PLAYBACK2,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuplayback)/sizeof(PSMENUITEM),
    NULL,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

SMENUSTRUCT sSubMovieSetting =
{
    MENUID_SUB_MENU_SETTING,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetting)/sizeof(PSMENUITEM),
    sMenuMovieSetting,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
PSMENUITEM   sMenuMovieSettingvol[] =
{
    &sItemMovieSettingvol0,
    &sItemMovieSettingvol1,        
    &sItemMovieSettingvol2,
    &sItemMovieSettingvol3,
    &sItemMovieSettingTouchSoundyes,
    &sItemMovieSettingTouchSoundno,
};
SMENUSTRUCT sSubMovieSettingvol=
{
    MENUID_SUB_MENU_SETTINGVOICE,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingvol)/sizeof(PSMENUITEM),
    sMenuMovieSettingvol,
     SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};



PSMENUITEM   sMenuMovieSettingvolt[] =
{
    &sItemMovieSetup9volt1,
    &sItemMovieSetup9volt2,        
    &sItemMovieSetup9volt3,
    &sItemMovieSetup9volt4,
    &sItemMovieSetup9volt5,
};

PSMENUITEM   sMenuMovieSettingvolt1[] =
{
    &sItemMovieSetup12volt1,
    &sItemMovieSetup12volt2,        
    &sItemMovieSetup12volt3,
    &sItemMovieSetup12volt4,
    &sItemMovieSetup12volt5,
};
PSMENUITEM   sMenuMovieSettingvolt2[] =
{
    &sItemMovieSetup24volt1,
    &sItemMovieSetup24volt2,        
    &sItemMovieSetup24volt3,
    &sItemMovieSetup24volt4,
    &sItemMovieSetup24volt5,
};

PSMENUITEM   sMenuMovieSettingparkingvolt[] =
{
    &sItemMovieSettingparkingvolt0,
    &sItemMovieSettingparkingvolt1,        
};
PSMENUITEM   sMenuMovieSettingbatteryh[] =
{
    &sItemMovieSettingbatteryh0,
    &sItemMovieSettingbatteryh1,
    &sItemMovieSettingbatteryh2,
    &sItemMovieSettingbatteryh3,
    &sItemMovieSettingbatteryh4,
};

SMENUSTRUCT sSubMovieSettingvolt=
{
    MENUID_SUB_MENU_SETTINGVOLT,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingvolt)/sizeof(PSMENUITEM),
    sMenuMovieSettingvolt,
     SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieSettingsystem=
{
    MENUID_SUB_MENU_SETTINGSYSTEM,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingparkingvolt)/sizeof(PSMENUITEM),
    sMenuMovieSettingparkingvolt,
     SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieSettingbatteryh=
{
    MENUID_SUB_MENU_SETTINGBATTERYH,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingbatteryh)/sizeof(PSMENUITEM),
    sMenuMovieSettingbatteryh,
     SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieSettingtime=
{
    MENUID_SUB_MENU_SETTINGTIME,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    8,
   NULL,
     SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

SMENUSTRUCT sSubMovieSettingvolt1=
{
    MENUID_SUB_MENU_SETTINGVOLT1,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingvolt1)/sizeof(PSMENUITEM),
    sMenuMovieSettingvolt1,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieSettingvolt2=
{
    MENUID_SUB_MENU_SETTINGVOLT2,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingvolt2)/sizeof(PSMENUITEM),
    sMenuMovieSettingvolt2,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};



PSMENUITEM   sMenuMovieSetupLcd[] =
{
    &sItemMovieSetupLcd1,
    &sItemMovieSetupLcd3,        
    &sItemMovieSetupLcdFull,
    //&sItemMovieSetupLcdClock,
};

SMENUSTRUCT sSubMovieSetupLcd =
{
    MENUID_SUB_MENU_LCD,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupLcd)/sizeof(PSMENUITEM),
    sMenuMovieSetupLcd,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

PSMENUITEM   sMenuMovieSetupLogo[] =
{
    &sItemMovieSetupLogo1,
    &sItemMovieSetupLogo2,        
    &sItemMovieSetupLogo3,
};

PSMENUITEM   sMenuMovieSetupLogo_1080[] =
{
    &sItemMovieSetupLogo1_1080,
    &sItemMovieSetupLogo2_1080,        
    &sItemMovieSetupLogo3_1080,
};

SMENUSTRUCT sSubMovieSetupLogo =
{
    MENUID_SUB_MENU_LOGO,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupLogo)/sizeof(PSMENUITEM),
    sMenuMovieSetupLogo,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

SMENUSTRUCT sSubMovieSetupLogo_1080 =
{
    MENUID_SUB_MENU_LOGO,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupLogo_1080)/sizeof(PSMENUITEM),
    sMenuMovieSetupLogo_1080,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

PSMENUITEM   sMenuMovieSetupGsensor[] =
{
    &sItemMovieSetupGsensor1,
    &sItemMovieSetupGsensor2,        
    &sItemMovieSetupGsensor3,
};

SMENUSTRUCT sSubMovieSetupGsensor=
{
    MENUID_SUB_MENU_GSENSOR,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupGsensor)/sizeof(PSMENUITEM),
    sMenuMovieSetupGsensor,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

PSMENUITEM   sMenuMovieSetupReset[] =
{
    &sItemMovieSetupReset0,
    &sItemMovieSetupReset1,        
    &sItemMovieSetupReset2,
};

PSMENUITEM   sMenuMovieSetupGps[] =
{
    &sItemMovieSetupGps0,
    &sItemMovieSetupGps1,        
    &sItemMovieSetupGps2,
};

PSMENUITEM   sMenuMovieSetupYesNo1[] =
{
    &sItemMovieSetupYes1,
    &sItemMovieSetupNo1,        
};
PSMENUITEM   sMenuMovieSetupYesNo3[] =
{
	 &sItemMovieSettingyesno3,
    &sItemMovieSettingyes3,
    &sItemMovieSettingno3,
};

PSMENUITEM   sMenuMovieSetupYesNo[] =
{
    &sItemMovieSettingyes,
    &sItemMovieSettingno,        
};
PSMENUITEM   sMenuMovieSettingevent[] =
{
    &sItemMovieSettingeventYes,
    &sItemMovieSettingeventNO,        
};

PSMENUITEM   sMenuMovieSettingFormat1[] =
{
    &sItemMovieSettingFormatYes,
    &sItemMovieSettingFormatNO,        
};
PSMENUITEM   sMenuMovieSettingFormat[] =
{
    &sItemMovieSettingFormat0,
    &sItemMovieSettingFormat1,        
    &sItemMovieSettingFormat2,
    &sItemMovieSettingFormat3,
};

SMENUSTRUCT sSubMovieSetupMic=
{
    MENUID_SUB_MENU_MIC,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupYesNo1)/sizeof(PSMENUITEM),
    sMenuMovieSetupYesNo1,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

SMENUSTRUCT sSubMovieSetupReset=
{
    MENUID_SUB_MENU_RESET,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupReset)/sizeof(PSMENUITEM),
    sMenuMovieSetupReset,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

SMENUSTRUCT sSubMovieSetupGps=
{
    MENUID_SUB_MENU_GPS,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupGps)/sizeof(PSMENUITEM),
    sMenuMovieSetupGps,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieSettingformat=
{
    MENUID_SUB_MENU_SETTINGFORMATE,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingFormat)/sizeof(PSMENUITEM),
    sMenuMovieSettingFormat,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

SMENUSTRUCT sSubMovieSettingformat1=
{
    MENUID_SUB_MENU_SETTINGFORMATE1,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSettingFormat1)/sizeof(PSMENUITEM),
    sMenuMovieSettingFormat1,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};

SMENUSTRUCT sSubMovieSettingevent=
{
    MENUID_SUB_MENU_SETTINGEVENT,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupYesNo)/sizeof(PSMENUITEM),
    sMenuMovieSettingevent,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieSettingParking= 
{
	MENUID_SUB_MENU_SETTINGPARKING,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupYesNo3)/sizeof(PSMENUITEM),
    sMenuMovieSetupYesNo3,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
SMENUSTRUCT sSubMovieSettingtouch=
{
    MENUID_SUB_MENU_SETTINGTOUCH,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuMovieSetupYesNo)/sizeof(PSMENUITEM),
    sMenuMovieSetupYesNo,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};


AHC_BOOL MenuItemMovieSetupLcd(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	
	MenuSettingConfig()->uiLCDBrightness= pItem->iItemId;// - ITEMID_MOVIE_MODE -1;
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
	printc("************************\n");
	printc("MenuItemMovieSetupLcd--pItem->iItemId=%d\n",pItem->iItemId);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupLogo(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	MenuSettingConfig()->uiHDMIOutput= pItem->iItemId;
	printc("MenuItemMovieSetupLogo--MenuSettingConfig()->uiTVSystem=%d\n",MenuSettingConfig()->uiHDMIOutput);

	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupGsensor(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSetupGsensor\n");

	MenuSettingConfig()->uiGsensorSensitivity=pItem->iItemId+1 ;
	
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupMic(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSetupMic---pItem=%d,pItem=%d\n");

	if(MenuSettingConfig()->uiMOVSoundRecord!=pItem->iItemId)
		AHC_ToggleMute();
	//MenuSettingConfig()->uiMOVSoundRecord=pItem->iItemId;
	//MenuSettingConfig()->uiMOVSize= pItem->iItemId - ITEMID_MOVIE_MODE -1;
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupReset(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSetupReset---pItem->iItemId=%d\n",pItem->iItemId);
		if(pItem->iItemId == 0)  //Only change settings while press OK.
		{
			ResetDefaultSetting();
			//RestoreFromDefaultSetting();
			//Menu_RenewMenuSetting(AHC_TRUE);
			//AHC_WMSG_DrawBMP (WMSG_RESET_OK, 1);
			AHC_OS_SleepMs(1000);
		}
	//MenuSettingConfig()->uiMOVSize= pItem->iItemId;
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupGps(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSetupGps---pItem->iItemId=%d\n",pItem->iItemId);
	MenuSettingConfig()->uiRecordGPS= pItem->iItemId ;
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupYesNo(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSetupYesNo---pItem=%d,pItem=%d\n");
	//MenuSettingConfig()->uiMOVSize= pItem->iItemId;
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupYesNoEvent(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSetupYesNo---pItem=%d,pItem=%d\n");
	MenuSettingConfig()->uiGPSStamp= pItem->iItemId;
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSetupClock(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	//MenuSettingConfig()->uiMOVSize= pItem->iItemId - ITEMID_MOVIE_MODE -1;
	//Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieVOLT(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieVOLT---pItem->iItemId=%d\n",pItem->iItemId);
	
	MenuSettingConfig()->uiSlideShowEffect=pItem->iItemId;//保存电压选项
	//MenuSettingConfig()->uiGPSStamp=pItem->iItemId;//保存电压选项
	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieVOLT1(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieVOLT1---pItem->iItemId=%d\n",pItem->iItemId);
	//MenuSettingConfig()->uiWNR=pItem->iItemId+5;//保存电压选项
	MenuSettingConfig()->uiSlideShowEffect=pItem->iItemId+5;//保存电压选项
	printc("MenuItemMovieVOLT1--MenuSettingConfig()->uiSlideShowEffect=%d\n",MenuSettingConfig()->uiSlideShowEffect);
	//MenuSettingConfig()->uiGPSStamp=pItem->iItemId+5;//保存电压选项
	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieVOLT2(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieVOLT2---pItem->iItemId=%d\n",pItem->iItemId);
	//MenuSettingConfig()->uiWNR=pItem->iItemId+10;//保存电压选项
	MenuSettingConfig()->uiSlideShowEffect=pItem->iItemId+10;//保存电压选项
//	MenuSettingConfig()->uiGPSStamp=pItem->iItemId+10;//保存电压选项
	return AHC_TRUE;
}
AHC_BOOL MenuItemMovieSettingVol(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSettingVol---pItem->iItemId=%d\n",pItem->iItemId);
	MenuSettingConfig()->uiVolume=4+pItem->iItemId*2;
	Menu_SetVolume(MenuSettingConfig()->uiVolume);
	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSettingFormat(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("**************MenuItemMovieSettingFormat---pItem->iItemId=%d\n",pItem->iItemId);
	MenuSettingConfig()->uiWDR=pItem->iItemId;
	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSettingFormat1(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSettingFormat1---pItem->iItemId=%d\n",pItem->iItemId);
	if(pItem->iItemId==0)
	{
		QuickMediaOperation(0);
	}
	return AHC_TRUE;
}
AHC_BOOL MenuItemMovieSettingevent(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSettingevent---pItem->iItemId=%d\n",pItem->iItemId);
	//MenuSettingConfig(void)()->uiStatusLight= pItem->iItemId ;
	MenuSettingConfig()->uiGPSStamp= pItem->iItemId ;
	return AHC_TRUE;
}

AHC_BOOL MenuItemMovieSettingTouchVol(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	printc("MenuItemMovieSettingTouchVol---pItem->iItemId=%d\n",pItem->iItemId);
	MenuSettingConfig()->uiBeep = pItem->iItemId;
	return AHC_TRUE;
}

//leo20180630

/*******************************************/

// Movie Mode
#if (MENU_MOVIE_SIZE_EN)
//SMENUITEM sItemMovieMode          		= { ITEMID_MOVIE_MODE, 			&BMICON_MENU_REC_MODE,    	IDS_DS_MOVIE_MODE,			&sSubMovieMode, 0, 		 0, AHC_TRUE, NULL };
#if (MENU_MOVIE_SIZE_4K_24P_EN)
SMENUITEM sItemMovieMode_4K_24P     	= { ITEMID_MOVIE_MODE_4K_24P,  	&BMICON_MOVIE_RES_SHD30P, 	IDS_DS_MOVIE_MODE_SHD,		NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_1440_30P_EN)
SMENUITEM sItemMovieMode_1440_30P     	= { ITEMID_MOVIE_MODE_1440_30P, &BMICON_MOVIE_RES_SHD30P, 	IDS_DS_MOVIE_MODE_SHD,		NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_SHD_30P_EN)
SMENUITEM sItemMovieMode_SHD      		= { ITEMID_MOVIE_MODE_SHD,  	&BMICON_MOVIE_RES_SHD30P, 	IDS_DS_MOVIE_MODE_SHD,		NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_SHD_25P_EN)
SMENUITEM sItemMovieMode_SHD_25P      	= { ITEMID_MOVIE_MODE_SHD_25P,  &BMICON_MOVIE_RES_SHD25P, 	IDS_DS_MOVIE_MODE_SHD_25P,		NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_SHD_24P_EN)
SMENUITEM sItemMovieMode_SHD_24P      	= { ITEMID_MOVIE_MODE_SHD_24P,  &BMICON_MOVIE_RES_SHD24P, 	IDS_DS_MOVIE_MODE_SHD_24P,		NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_1080_60P_EN)
SMENUITEM sItemMovieMode_FHD_60P   		= { ITEMID_MOVIE_MODE_FHD_60P, 	&BMICON_MOVIE_RES_FHD60P, 	IDS_DS_MOVIE_MODE_FHD_60P,	NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (1)
SMENUITEM sItemMovieMode_FHD      		= { ITEMID_MOVIE_MODE_FHD,  	&bmIcon_hd2_bt_nor, 	IDS_DS_MOVIE_MODE_FHD,		NULL, /*MenuItemMovieMode*/NULL, 0, AHC_TRUE, NULL };
#endif

#if (MENU_MOVIE_SIZE_1080P_30_HDR_EN)
SMENUITEM sItemMovieMode_FHD_HDR      	= { ITEMID_MOVIE_MODE_FHD_HDR,  &BMICON_MOVIE_RES_FHD30PHDR, IDS_DS_MOVIE_MODE_FHD_HDR,		NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif   

#if (MENU_MOVIE_SIZE_1080_24P_EN)
SMENUITEM sItemMovieMode_FHD_24P        = { ITEMID_MOVIE_MODE_FHD_24P,  &BMICON_MOVIE_RES_FHD24P,   IDS_DS_MOVIE_MODE_FHD_24P,  NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_720P_EN)
SMENUITEM sItemMovieMode_HD     		= { ITEMID_MOVIE_MODE_HD,  		&bmIcon_hd__bt_nor, 	IDS_DS_MOVIE_MODE_HD,		NULL, /*MenuItemMovieMode*/NULL, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_720_60P_EN)
SMENUITEM sItemMovieMode_HD_60P     	= { ITEMID_MOVIE_MODE_HD_60P,   &BMICON_MOVIE_RES_HD60P,  	IDS_DS_MOVIE_MODE_HD_60P,   NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_720_24P_EN)
SMENUITEM sItemMovieMode_HD_24P         = { ITEMID_MOVIE_MODE_HD_24P,   &BMICON_MOVIE_RES_HD24P,    IDS_DS_MOVIE_MODE_HD_24P,   NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_VGA30P_EN)
SMENUITEM sItemMovieMode_VGA_30P  		= { ITEMID_MOVIE_MODE_VGA_30P,  &BMICON_MOVIE_RES_VGA30P, 	IDS_DS_MOVIE_MODE_VGA_30P,	NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SIZE_VGA120P_EN)
SMENUITEM sItemMovieMode_VGA_120P  		= { ITEMID_MOVIE_MODE_VGA_120P, &BMICON_MOVIE_RES_VGA30P, 	IDS_DS_MOVIE_MODE_VGA_30P,	NULL, MenuItemMovieMode, 0, AHC_TRUE, NULL };
#endif

#endif

// Movie Quality
#if (MENU_MOVIE_QUALITY_EN)
SMENUITEM sItemMovieQuality         	= { ITEMID_MOVIE_QUALITY, 		&BMICON_MENU_REC_QUALITY, IDS_DS_MOVIE_QUALITY,	&sSubMovieQuality, 0, 		0, AHC_TRUE, NULL };
#if (MENU_MOVIE_QUALITY_SUPER_FINE_EN)
SMENUITEM sItemMovieQuality_Super_Fine  = { ITEMID_MQUALITY_SUPERFINE, 	&BMICON_QUALITY_SHQ, IDS_DS_QUALITY_SUPERFINE,	NULL, MenuItemMovieQuality, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_QUALITY_FINE_EN)
SMENUITEM sItemMovieQuality_Fine    	= { ITEMID_MQUALITY_FINE, 		&BMICON_QUALITY_HQ,  IDS_DS_QUALITY_FINE,		NULL, MenuItemMovieQuality, 0, AHC_TRUE, NULL };
#endif
#endif

// Movie Stabilization
#if (MENU_MOVIE_STABILIZE_EN)
SMENUITEM sItemMovieStabilization       = { ITEMID_MOVIE_STABLEILIZATION, NULL, IDS_DS_MOVIE_STABLEILIZATION,	&sSubMovieStable, 0, 0, AHC_TRUE, NULL };
#if (MENU_MOVIE_STABILIZE_ON_EN)
SMENUITEM sItemMovieStabilization_On    = { ITEMID_MOVIE_STABLE_ON,  &bmIcon_ImageStable_ON,  IDS_DS_ON,	NULL, MenuItemMovieStable, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_STABILIZE_OFF_EN)
SMENUITEM sItemMovieStabilization_Off   = { ITEMID_MOVIE_STABLE_OFF, &bmIcon_ImageStable_OFF, IDS_DS_OFF,	NULL, MenuItemMovieStable, 0, AHC_TRUE, NULL };
#endif
#endif

// Auto Slow Shutter
#if (MENU_MOVIE_SLOW_SHUTTER_EN)
SMENUITEM sItemAutoSlowShutter          = { ITEMID_AUTO_SLOW_SHUTTER,  NULL, IDS_DS_AUTO_SLOW_SHUTTER,	&sSubAutoSlowShutter, 0, 0, AHC_TRUE, NULL };
#if (MENU_MOVIE_SLOW_SHUTTER_ON_EN)
SMENUITEM sItemAutoSlowShutter_On       = { ITEMID_AUTO_SLOW_SHUT_ON,  NULL, IDS_DS_ON,	NULL, MenuItemAutoSlowShutter, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SLOW_SHUTTER_OFF_EN)
SMENUITEM sItemAutoSlowShutter_Off      = { ITEMID_AUTO_SLOW_SHUT_OFF, NULL, IDS_DS_OFF,NULL, MenuItemAutoSlowShutter, 0, AHC_TRUE, NULL };
#endif
#endif

// MIC Sensitivity
#if (MENU_MOVIE_MIC_SEN_EN)
SMENUITEM sItemMICSensitivity           = { ITEMID_MIC_SENSITIVITY, 	 &BMICON_MENU_MIC,   IDS_DS_MIC_SENSITIVITY, &sSubMICSensitivity, 0,  0, AHC_TRUE, NULL };
#if (MENU_MOVIE_MIC_SEN_STANDARD_EN)
SMENUITEM sItemMICSensitivity_Standard  = { ITEMID_MIC_SENSITI_STANDARD, &BMICON_MIC_SENS_H, IDS_DS_STANDARD,	NULL, MenuItemMICSensitivity, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_MIC_SEN_LOW_EN)
SMENUITEM sItemMICSensitivity_Low       = { ITEMID_MIC_SENSITI_LOW, 	 &BMICON_MIC_SENS_L, IDS_DS_LOW,		NULL, MenuItemMICSensitivity, 0, AHC_TRUE, NULL };
#endif
#endif

// Movie PreRecord
#if (MENU_MOVIE_PRE_RECORD_EN)
SMENUITEM sItemMoviePreRecord          = { ITEMID_MOVIE_PRE_RECORD, NULL, IDS_DS_MOVIE_PRE_RECORD,	&sSubMoviePreRecord, 0, 0, AHC_TRUE, NULL };
#if (MENU_MOVIE_PRE_RECORD_ON_EN)
SMENUITEM sItemMoviePreRecord_On       = { ITEMID_MOVIE_PRE_RECORD_ON,  NULL, IDS_DS_ON, NULL, MenuItemMoviePreRecord, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_PRE_RECORD_OFF_EN)
SMENUITEM sItemMoviePreRecord_Off      = { ITEMID_MOVIE_PRE_RECORD_OFF, NULL, IDS_DS_OFF,NULL, MenuItemMoviePreRecord, 0, AHC_TRUE, NULL };
#endif
#endif

// Movie Clip Time
#if (MENU_MOVIE_CLIP_TIME_EN)
SMENUITEM sItemMovieClipTime      		= { ITEMID_MOVIE_CLIPTIME, 			&BMICON_MENU_CLIPTIME,	IDS_MOVIE_CLIPTIME, 	&sSubMovieClipTime, 	0, 	 0, AHC_TRUE, NULL };
#if (MENU_MOVIE_CLIP_TIME_OFF_EN)
SMENUITEM sItemMovieClipTime_off  		= { ITEMID_MOVIE_CLIPTIME_OFF,	 	&BMICON_OPTION_CLOSE, 	IDS_DS_OFF,				NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_6SEC_EN)
SMENUITEM sItemMovieClipTime_6sec      	= { ITEMID_MOVIE_CLIPTIME_6SEC, 	&BMICON_CLIPTIME_1MIN, 	IDS_TIME_1MIN,			NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_1MIN_EN)
SMENUITEM sItemMovieClipTime_1min      	= { ITEMID_MOVIE_CLIPTIME_1MIN, 	&BMICON_CLIPTIME_1MIN, 	IDS_TIME_1MIN,			NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_2MIN_EN)
SMENUITEM sItemMovieClipTime_2min      	= { ITEMID_MOVIE_CLIPTIME_2MIN, 	&BMICON_CLIPTIME_1MIN, 	IDS_TIME_2MIN,			NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_3MIN_EN)
SMENUITEM sItemMovieClipTime_3min  		= { ITEMID_MOVIE_CLIPTIME_3MIN, 	&BMICON_CLIPTIME_3MIN, 	IDS_TIME_3MIN,			NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_5MIN_EN)
SMENUITEM sItemMovieClipTime_5min       = { ITEMID_MOVIE_CLIPTIME_5MIN, 	&BMICON_CLIPTIME_5MIN, 	IDS_TIME_5MIN,			NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_10MIN_EN)
SMENUITEM sItemMovieClipTime_10min      = { ITEMID_MOVIE_CLIPTIME_10MIN, 	&BMICON_CLIPTIME_10MIN, IDS_TIME_10MIN,			NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_25MIN_EN)
SMENUITEM sItemMovieClipTime_25min      = { ITEMID_MOVIE_CLIPTIME_25MIN, 	NULL,                   IDS_TIME_25MIN, 		NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_CLIP_TIME_30MIN_EN)
SMENUITEM sItemMovieClipTime_30min      = { ITEMID_MOVIE_CLIPTIME_30MIN, 	&BMICON_CLIPTIME_1MIN, 	IDS_TIME_1MIN,			NULL, MenuItemMovieClipTime, 0, AHC_TRUE, NULL };
#endif
#endif

// Movie Night mode
#if (MENU_MOVIE_NIGHT_MODE_EN)
SMENUITEM sItemMovieNightMode          = { ITEMID_MOVIE_NIGHT_MODE, NULL, IDS_DS_NIGHT_MODE,	&sSubMovieNightMode, 0, 0, AHC_TRUE, NULL };
#if (MENU_MOVIE_NIGHT_MODE_ON_EN)
SMENUITEM sItemMovieNightMode_On       = { ITEMID_MOVIE_NIGHT_MODE_ON,  NULL, IDS_DS_ON, NULL, MenuItemMovieNightMode, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_NIGHT_MODE_OFF_EN)
SMENUITEM sItemMovieNightMode_Off      = { ITEMID_MOVIE_NIGHT_MODE_OFF, NULL, IDS_DS_OFF,NULL, MenuItemMovieNightMode, 0, AHC_TRUE, NULL };
#endif
#endif

// Movie PowerOff Time
#ifndef CFG_POWER_OFF_STR_ID //may be defined in config_xxx.h
    #define CFG_POWER_OFF_STR_ID IDS_MOVIE_POWEROFF_TIME
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_EN)
SMENUITEM sItemMoviePowerOffTime      	= { ITEMID_MOVIE_POWEROFF_TIME, 	  &BMICON_MENU_OFFTIME, 	CFG_POWER_OFF_STR_ID, 	&sSubMoviePowerOffTime, 0, 	0, AHC_TRUE, NULL };
#if (MENU_MOVIE_POWER_OFF_DELAY_0SEC_EN)
SMENUITEM sItemMoviePowerOffTime_0min 	= { ITEMID_MOVIE_POWEROFF_TIME_0MIN,  &BMICON_OFFTIME_0MIN, 	IDS_TIME_0MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_5SEC_EN)
SMENUITEM sItemMoviePowerOffTime_5sec 	= { ITEMID_MOVIE_POWEROFF_TIME_5SEC,  &BMICON_OFFTIME_5SEC, 	IDS_TIME_5SEC,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_10SEC_EN)
SMENUITEM sItemMoviePowerOffTime_10sec 	= { ITEMID_MOVIE_POWEROFF_TIME_10SEC, &BMICON_OFFTIME_10SEC, 	IDS_TIME_10SEC,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_15SEC_EN)
SMENUITEM sItemMoviePowerOffTime_15sec 	= { ITEMID_MOVIE_POWEROFF_TIME_15SEC, NULL, 					IDS_TIME_15SEC,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_30SEC_EN)
SMENUITEM sItemMoviePowerOffTime_30sec 	= { ITEMID_MOVIE_POWEROFF_TIME_30SEC, &BMICON_OFFTIME_30SEC, 	IDS_TIME_30SEC,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_1MIN_EN)
SMENUITEM sItemMoviePowerOffTime_1min  	= { ITEMID_MOVIE_POWEROFF_TIME_1MIN,  &BMICON_OFFTIME_1MIN,		IDS_TIME_1MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_2MIN_EN)
SMENUITEM sItemMoviePowerOffTime_2min   = { ITEMID_MOVIE_POWEROFF_TIME_2MIN,  &BMICON_OFFTIME_2MIN, 	IDS_TIME_2MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_3MIN_EN)
SMENUITEM sItemMoviePowerOffTime_3min   = { ITEMID_MOVIE_POWEROFF_TIME_3MIN,  &BMICON_OFFTIME_3MIN, 	IDS_TIME_3MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_5MIN_EN)
SMENUITEM sItemMoviePowerOffTime_5min   = { ITEMID_MOVIE_POWEROFF_TIME_5MIN,  NULL, 					IDS_TIME_5MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_10MIN_EN)
SMENUITEM sItemMoviePowerOffTime_10min  = { ITEMID_MOVIE_POWEROFF_TIME_10MIN, NULL,						IDS_TIME_10MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_15MIN_EN)
SMENUITEM sItemMoviePowerOffTime_15min  = { ITEMID_MOVIE_POWEROFF_TIME_15MIN, NULL,						IDS_TIME_15MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_30MIN_EN)
SMENUITEM sItemMoviePowerOffTime_30min  = { ITEMID_MOVIE_POWEROFF_TIME_30MIN, NULL,						IDS_TIME_30MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_60MIN_EN)
SMENUITEM sItemMoviePowerOffTime_60min  = { ITEMID_MOVIE_POWEROFF_TIME_60MIN, NULL, 					IDS_TIME_60MIN,	NULL, MenuItemMoviePowerOffTime, 0, AHC_TRUE, NULL };
#endif
#endif

// Movie Sound Record
#if (MENU_MOVIE_SOUND_RECORD_EN)
SMENUITEM sItemMovieSoundRecord       	= { ITEMID_MOVIE_SOUND_RECORD, 		&BMICON_MENU_RECORD_SOUND, 	IDS_MOVIE_SOUND_RECORD,	&sSubMovieSoundRecord, 	0, 	0, AHC_TRUE, NULL };
#if (MENU_MOVIE_SOUND_RECORD_ON_EN)
SMENUITEM sItemMovieSoundRecord_On    	= { ITEMID_MOVIE_SOUND_RECORD_ON,  	&BMICON_OPTION_OPEN,  		IDS_DS_ON,		NULL, MenuItemMovieSoundRecord, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_SOUND_RECORD_OFF_EN)
SMENUITEM sItemMovieSoundRecord_Off   	= { ITEMID_MOVIE_SOUND_RECORD_OFF, 	&BMICON_OPTION_CLOSE, 		IDS_DS_OFF,		NULL, MenuItemMovieSoundRecord, 0, AHC_TRUE, NULL };
#endif
#endif

// Motion Record Time
#if (MENU_MOVIE_VMD_REC_TIME_EN)
SMENUITEM sItemMovieVMDRecTime      	= { ITEMID_MOVIE_VMD_REC_TIME, 	  	 &BMICON_MENU_MTD, 	IDS_VMD_REC_TIME, 	&sSubMovieVMDRecTime, 			0, 		   0, AHC_TRUE, NULL };
#if (MENU_MOVIE_VMD_REC_TIME_5SEC_EN)
SMENUITEM sItemMovieVMDRecTime_5sec 	= { ITEMID_MOVIE_VMD_REC_TIME_5SEC,  &BMICON_OFFTIME_5SEC, 	IDS_TIME_5SEC,	NULL, MenuItemMovieVMDRecTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_VMD_REC_TIME_10SEC_EN)
SMENUITEM sItemMovieVMDRecTime_10sec 	= { ITEMID_MOVIE_VMD_REC_TIME_10SEC, &BMICON_OFFTIME_10SEC, IDS_TIME_10SEC,	NULL, MenuItemMovieVMDRecTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_VMD_REC_TIME_30SEC_EN)
SMENUITEM sItemMovieVMDRecTime_30sec 	= { ITEMID_MOVIE_VMD_REC_TIME_30SEC, &BMICON_OFFTIME_30SEC, IDS_TIME_30SEC,	NULL, MenuItemMovieVMDRecTime, 0, AHC_TRUE, NULL };
#endif
#if (MENU_MOVIE_VMD_REC_TIME_1MIN_EN)
SMENUITEM sItemMovieVMDRecTime_1min  	= { ITEMID_MOVIE_VMD_REC_TIME_1MIN,  &BMICON_OFFTIME_60SEC,	IDS_TIME_1MIN,	NULL, MenuItemMovieVMDRecTime, 0, AHC_TRUE, NULL };
#endif
#endif

// Movie Auto Rec
#if (MENU_MOVIE_AUTO_REC_EN)
SMENUITEM sItemMovieAutoRec				= { ITEMID_MOVIE_AUTO_REC,			NULL, IDS_AUTO_REC,		&sSubMovieAutoRec, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieAutoRec_On			= { ITEMID_MOVIE_AUTO_REC_ON,		NULL, IDS_DS_ON,        NULL,    MenuItemMovieAutoRec, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieAutoRec_Off			= { ITEMID_MOVIE_AUTO_REC_OFF,		NULL, IDS_DS_OFF,       NULL,    MenuItemMovieAutoRec, 0,AHC_TRUE, NULL };
#endif

#if (MENU_VIDEO_TIMELAPSE_MODE_EN)	// VIDEO_REC_TIMELAPSE_EN
SMENUITEM sItemMovieTimelapse			= { ITEMID_MOVIE_TIMELAPSE,		NULL, IDS_DS_VIDEO_TIMELAPSE,		&sSubMovieTimelapse, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieTimelapse_Off		= { ITEMID_MOVIE_TIMELAPSE_OFF,	NULL, IDS_DS_VIDEO_TIMELAPSE_OFF,        	NULL,    	MenuItemMovieTimelapse, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieTimelapse_1Sec		= { ITEMID_MOVIE_TIMELAPSE_1SEC,	NULL, IDS_DS_VIDEO_TIMELAPSE_1SEC,       	NULL,    	MenuItemMovieTimelapse, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieTimelapse_5Sec		= { ITEMID_MOVIE_TIMELAPSE_5SEC,	NULL, IDS_DS_VIDEO_TIMELAPSE_5SEC,       	NULL,    	MenuItemMovieTimelapse, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieTimelapse_10Sec		= { ITEMID_MOVIE_TIMELAPSE_10SEC,	NULL, IDS_DS_VIDEO_TIMELAPSE_10SEC,       	NULL,    	MenuItemMovieTimelapse, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieTimelapse_30Sec		= { ITEMID_MOVIE_TIMELAPSE_30SEC,	NULL, IDS_DS_VIDEO_TIMELAPSE_30SEC,       	NULL,    	MenuItemMovieTimelapse, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieTimelapse_60Sec		= { ITEMID_MOVIE_TIMELAPSE_60SEC,	NULL, IDS_DS_VIDEO_TIMELAPSE_60SEC,       	NULL,    	MenuItemMovieTimelapse, 0,AHC_TRUE, NULL };
#endif

#if (MENU_MOVIE_PARKING_MODE_EN)
SMENUITEM sItemMovieParkingMode			= { ITEMID_MOVIE_PARKING_MODE,		NULL, IDS_DS_PARKING_MODE,  &sSubMovieParkingMode, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieParkingMode_On		= { ITEMID_MOVIE_PARKING_MODE_ON,	NULL, IDS_DS_ON,        	NULL,    	MenuItemMovieParkingMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieParkingMode_Off		= { ITEMID_MOVIE_PARKING_MODE_OFF,	NULL, IDS_DS_OFF,       	NULL,    	MenuItemMovieParkingMode, 0,AHC_TRUE, NULL };
#endif

// LDWS On/Off
#if (MENU_MOVIE_LDWS_MODE_EN)
SMENUITEM sItemMovieLDWSMode            = { ITEMID_MOVIE_LDWS_MODE,         NULL, IDS_DS_LDWS,		    &sSubMovieLDWSMode, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieLDWSMode_On         = { ITEMID_MOVIE_LDWS_MODE_ON,      NULL, IDS_DS_ON,        	NULL,    	MenuItemMovieLDWSMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieLDWSMode_Off        = { ITEMID_MOVIE_LDWS_MODE_OFF,     NULL, IDS_DS_OFF,       	NULL,    	MenuItemMovieLDWSMode, 0,AHC_TRUE, NULL };
#endif

// FCWS On/Off
#if (MENU_MOVIE_FCWS_MODE_EN)
SMENUITEM sItemMovieFCWSMode            = { ITEMID_MOVIE_FCWS_MODE,         NULL, IDS_DS_FCWS,		    &sSubMovieFCWSMode, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieFCWSMode_On         = { ITEMID_MOVIE_FCWS_MODE_ON,      NULL, IDS_DS_ON,        	NULL,    	MenuItemMovieFCWSMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieFCWSMode_Off        = { ITEMID_MOVIE_FCWS_MODE_OFF,     NULL, IDS_DS_OFF,       	NULL,    	MenuItemMovieFCWSMode, 0,AHC_TRUE, NULL };
#endif

// SAG On/Off
#if (MENU_MOVIE_SAG_MODE_EN)
SMENUITEM sItemMovieSAGMode            = { ITEMID_MOVIE_SAG_MODE,         NULL, IDS_DS_SAG,		        &sSubMovieSAGMode, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSAGMode_On         = { ITEMID_MOVIE_SAG_MODE_ON,      NULL, IDS_DS_ON,           	NULL,    	MenuItemMovieSAGMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSAGMode_Off        = { ITEMID_MOVIE_SAG_MODE_OFF,     NULL, IDS_DS_OFF,          	NULL,    	MenuItemMovieSAGMode, 0,AHC_TRUE, NULL };
#endif

// HDR On/Off
#if (MENU_MOVIE_HDR_MODE_EN)
SMENUITEM sItemMovieHDRMode            = { ITEMID_MOVIE_HDR_MODE,           NULL, IDS_DS_HDR,		    &sSubMovieHDRMode, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieHDRMode_On         = { ITEMID_MOVIE_HDR_MODE_ON,        NULL, IDS_DS_ON,        	NULL,    	MenuItemMovieHDRMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieHDRMode_Off        = { ITEMID_MOVIE_HDR_MODE_OFF,       NULL, IDS_DS_OFF,       	NULL,    	MenuItemMovieHDRMode, 0,AHC_TRUE, NULL };
#endif

// SLOWMOTION
#if (MENU_MOVIE_SLOWMOTION_MODE_EN)
SMENUITEM sItemMovieSlowMotionMode           = { ITEMID_MOVIE_SLOWMOTION_MODE,           NULL, IDS_DS_SLOWMOTION,		    &sSubMovieSlowMotionMode, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieSlowMotionMode_X1        = { ITEMID_MOVIE_SLOWMOTION_MODE_X1,        NULL, IDS_DS_SLOWMOTION_X1,        NULL,    	MenuItemMovieSlowMotionMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSlowMotionMode_X2        = { ITEMID_MOVIE_SLOWMOTION_MODE_X2,        NULL, IDS_DS_SLOWMOTION_X2,       	NULL,    	MenuItemMovieSlowMotionMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSlowMotionMode_X4        = { ITEMID_MOVIE_SLOWMOTION_MODE_X4,        NULL, IDS_DS_SLOWMOTION_X4,        NULL,    	MenuItemMovieSlowMotionMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieSlowMotionMode_X8        = { ITEMID_MOVIE_SLOWMOTION_MODE_X8,        NULL, IDS_DS_SLOWMOTION_X8,       	NULL,    	MenuItemMovieSlowMotionMode, 0,AHC_TRUE, NULL };
#endif

// WNR On/Off
#if (MENU_MOVIE_WNR_MODE_EN)
SMENUITEM sItemMovieWNRMode            = { ITEMID_MOVIE_WNR_MODE,           NULL, IDS_DS_WNR,		    &sSubMovieWNRMode, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemMovieWNRMode_On         = { ITEMID_MOVIE_WNR_MODE_ON,        NULL, IDS_DS_ON,        	NULL,    	MenuItemMovieWNRMode, 0,AHC_TRUE, NULL };
SMENUITEM sItemMovieWNRMode_Off        = { ITEMID_MOVIE_WNR_MODE_OFF,       NULL, IDS_DS_OFF,       	NULL,    	MenuItemMovieWNRMode, 0,AHC_TRUE, NULL };
#endif


//RearCam type
#if(MENU_REARCAM_TYPE_EN)
SMENUITEM sItemRearCamType 							={ITEMID_REARCAM_TYPE,           		NULL, IDS_DS_REARCAMTYPE,		    &sSubRearCamType, 0, 0, AHC_TRUE, NULL };
SMENUITEM sItemRearCamType_None 					={ITEMID_REARCAM_TYPE_NONE,           		NULL, IDS_DS_NONE,								NULL, 				MenuItemRearCamType,0, AHC_TRUE, NULL };
SMENUITEM sItemRearCamType_AIT 						={ITEMID_REARCAM_TYPE_AIT,           	NULL, IDS_DS_AIT,		   			NULL, 				MenuItemRearCamType,0, AHC_TRUE, NULL };
SMENUITEM sItemRearCamType_SONIX_MJPEG2H264 		={ITEMID_REARCAM_TYPE_SONIX_MJPEG2H264, NULL, IDS_DS_SONIXMJPEG2H264,		NULL, 				MenuItemRearCamType,0, AHC_TRUE, NULL };
SMENUITEM sItemRearCamType_SONIX_MJPEG 				={ITEMID_REARCAM_TYPE_SONIX_MJPEG,      NULL, IDS_DS_SONIXMJPEG,		    NULL, 				MenuItemRearCamType,0, AHC_TRUE, NULL };
SMENUITEM sItemRearCamType_TV_DECODER 				={ITEMID_REARCAM_TYPE_TV_DECODER,       NULL, IDS_DS_TVDECODER,		    NULL, 				MenuItemRearCamType,0, AHC_TRUE, NULL };
SMENUITEM sItemRearCamType_BAYER_SENSOR 			={ITEMID_REARCAM_TYPE_BAYER_SENSOR,     	NULL, IDS_DS_BAYERSENSOR,		    NULL, 				MenuItemRearCamType,0, AHC_TRUE, NULL };
SMENUITEM sItemRearCamType_YUV_SENSOR				={ITEMID_REARCAM_TYPE_YUV_SENSOR,       NULL, IDS_DS_YUVSENSOR,		    NULL, 				MenuItemRearCamType,0, AHC_TRUE, NULL };
#endif
/*===========================================================================
 * Global varible : Item List
 *===========================================================================*/ 

#if (MENU_MOVIE_SIZE_EN)
PSMENUITEM   sMenuListMovieMode[] =
{
#if (1/*MENU_MOVIE_SIZE_1080P_EN*/)


    &sItemMovieMode_FHD,
#endif
#if (MENU_MOVIE_SIZE_720P_EN)
    &sItemMovieMode_HD,
#endif
};
#endif

#if (MENU_MOVIE_QUALITY_EN)
PSMENUITEM   sMenuListMovieQuality[] =
{
#if (MENU_MOVIE_QUALITY_SUPER_FINE_EN)
    &sItemMovieQuality_Super_Fine,
#endif
#if (MENU_MOVIE_QUALITY_FINE_EN)    
    &sItemMovieQuality_Fine
#endif    
};
#endif

#if (MENU_MOVIE_STABILIZE_EN)
PSMENUITEM   sMenuListMovieStable[] =
{
#if (MENU_MOVIE_STABILIZE_ON_EN)
    &sItemMovieStabilization_On,
#endif
#if (MENU_MOVIE_STABILIZE_OFF_EN)    
    &sItemMovieStabilization_Off
#endif    
};
#endif

#if (MENU_MOVIE_SLOW_SHUTTER_EN)
PSMENUITEM   sMenuListAutoSlowShutter[] =
{
#if (MENU_MOVIE_SLOW_SHUTTER_ON_EN)
    &sItemAutoSlowShutter_On,
#endif
#if (MENU_MOVIE_SLOW_SHUTTER_OFF_EN)    
    &sItemAutoSlowShutter_Off
#endif    
};
#endif

#if (MENU_MOVIE_MIC_SEN_EN)
PSMENUITEM   sMenuListMICSensitivity[] =
{
#if (MENU_MOVIE_MIC_SEN_STANDARD_EN)
    &sItemMICSensitivity_Standard,
#endif
#if (MENU_MOVIE_MIC_SEN_LOW_EN)
    &sItemMICSensitivity_Low
#endif
};
#endif

#if (MENU_MOVIE_PRE_RECORD_EN)
PSMENUITEM   sMenuListMoviePreRecord[] =
{
#if (MENU_MOVIE_PRE_RECORD_ON_EN)
    &sItemMoviePreRecord_On,
#endif
#if (MENU_MOVIE_PRE_RECORD_OFF_EN)    
    &sItemMoviePreRecord_Off
#endif    
};
#endif

#if (MENU_MOVIE_CLIP_TIME_EN)
PSMENUITEM   sMenuListMovieClipTime[] =
{
#if (MENU_MOVIE_CLIP_TIME_OFF_EN)
	&sItemMovieClipTime_off,
#endif
#if (MENU_MOVIE_CLIP_TIME_6SEC_EN)
    &sItemMovieClipTime_6sec,
#endif
#if (MENU_MOVIE_CLIP_TIME_1MIN_EN)	
    &sItemMovieClipTime_1min,
#endif
#if (MENU_MOVIE_CLIP_TIME_2MIN_EN)	
    &sItemMovieClipTime_2min,
#endif
#if (MENU_MOVIE_CLIP_TIME_3MIN_EN)    
    &sItemMovieClipTime_3min,
#endif
#if (MENU_MOVIE_CLIP_TIME_5MIN_EN)    
    &sItemMovieClipTime_5min,
#endif
#if (MENU_MOVIE_CLIP_TIME_10MIN_EN)    
    &sItemMovieClipTime_10min,
#endif    
#if (MENU_MOVIE_CLIP_TIME_25MIN_EN)    
    &sItemMovieClipTime_25min,
#endif
#if (MENU_MOVIE_CLIP_TIME_30MIN_EN)
    &sItemMovieClipTime_30min,
#endif    
};
#endif

#if (MENU_MOVIE_NIGHT_MODE_EN)
PSMENUITEM   sMenuListMovieNightMode[] =
{
#if (MENU_MOVIE_NIGHT_MODE_ON_EN)
    &sItemMovieNightMode_On,
#endif
#if (MENU_MOVIE_NIGHT_MODE_OFF_EN)    
    &sItemMovieNightMode_Off
#endif    
};
#endif

#if (MENU_MOVIE_POWER_OFF_DELAY_EN)
PSMENUITEM   sMenuListMoviePowerOffTime[] =
{
#if (MENU_MOVIE_POWER_OFF_DELAY_0SEC_EN)
	&sItemMoviePowerOffTime_0min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_5SEC_EN)
	&sItemMoviePowerOffTime_5sec,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_10SEC_EN)
	&sItemMoviePowerOffTime_10sec,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_15SEC_EN)
	&sItemMoviePowerOffTime_15sec,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_30SEC_EN)
	&sItemMoviePowerOffTime_30sec,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_1MIN_EN)
    &sItemMoviePowerOffTime_1min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_2MIN_EN)
    &sItemMoviePowerOffTime_2min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_3MIN_EN)
    &sItemMoviePowerOffTime_3min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_5MIN_EN)
    &sItemMoviePowerOffTime_5min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_10MIN_EN)
    &sItemMoviePowerOffTime_10min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_15MIN_EN)
    &sItemMoviePowerOffTime_15min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_30MIN_EN)
    &sItemMoviePowerOffTime_30min,
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_60MIN_EN)
    &sItemMoviePowerOffTime_60min
#endif
};
#endif

#if (MENU_MOVIE_SOUND_RECORD_EN)
PSMENUITEM	sMenuListMovieSoundRecord[] =
{
#if (MENU_MOVIE_SOUND_RECORD_ON_EN)
	&sItemMovieSoundRecord_On,
#endif
#if (MENU_MOVIE_SOUND_RECORD_OFF_EN)	
	&sItemMovieSoundRecord_Off,
#endif	
};
#endif

#if (MENU_MOVIE_VMD_REC_TIME_EN)
PSMENUITEM   sMenuListMovieVMDRecTime[] =
{
#if (MENU_MOVIE_VMD_REC_TIME_5SEC_EN)	
	&sItemMovieVMDRecTime_5sec,
#endif
#if (MENU_MOVIE_VMD_REC_TIME_10SEC_EN)	
	&sItemMovieVMDRecTime_10sec,
#endif
#if (MENU_MOVIE_VMD_REC_TIME_30SEC_EN)	
	&sItemMovieVMDRecTime_30sec,
#endif
#if (MENU_MOVIE_VMD_REC_TIME_1MIN_EN)	
    &sItemMovieVMDRecTime_1min,
#endif   
};
#endif

#if (MENU_MOVIE_AUTO_REC_EN)
PSMENUITEM   sMenuMovieAutoRec[] =
{
	&sItemMovieAutoRec_On,
	&sItemMovieAutoRec_Off
};
#endif

#if (MENU_VIDEO_TIMELAPSE_MODE_EN)	// VIDEO_REC_TIMELAPSE_EN
PSMENUITEM   sMenuMovieTimelapse[] =
{
    &sItemMovieTimelapse_Off,
    &sItemMovieTimelapse_1Sec,        
    &sItemMovieTimelapse_5Sec,
    &sItemMovieTimelapse_10Sec,
    &sItemMovieTimelapse_30Sec,
    &sItemMovieTimelapse_60Sec,    
};
#endif

#if (MENU_MOVIE_PARKING_MODE_EN)
PSMENUITEM   sMenuMovieParkingMode[] =
{
	&sItemMovieParkingMode_On,
	&sItemMovieParkingMode_Off,
};
#endif

#if (MENU_MOVIE_LDWS_MODE_EN)
PSMENUITEM   sMenuMovieLDWSMode[] =
{
	&sItemMovieLDWSMode_On,
	&sItemMovieLDWSMode_Off,
};
#endif

#if (MENU_MOVIE_FCWS_MODE_EN)
PSMENUITEM   sMenuMovieFCWSMode[] =
{
	&sItemMovieFCWSMode_On,
	&sItemMovieFCWSMode_Off,
};
#endif

#if (MENU_MOVIE_SAG_MODE_EN)
PSMENUITEM   sMenuMovieSAGMode[] =
{
	&sItemMovieSAGMode_On,
	&sItemMovieSAGMode_Off,
};
#endif

#if (MENU_MOVIE_HDR_MODE_EN)
PSMENUITEM   sMenuMovieHDRMode[] =
{
	&sItemMovieHDRMode_On,
	&sItemMovieHDRMode_Off,
};
#endif

#if (MENU_MOVIE_SLOWMOTION_MODE_EN)
PSMENUITEM   sMenuMovieSlowMotionMode[] =
{
	&sItemMovieSlowMotionMode_X1,
	&sItemMovieSlowMotionMode_X2,
	&sItemMovieSlowMotionMode_X4,
	&sItemMovieSlowMotionMode_X8,
};
#endif

#if (MENU_MOVIE_WNR_MODE_EN)
PSMENUITEM   sMenuMovieWNRMode[] =
{
	&sItemMovieWNRMode_Off,
	&sItemMovieWNRMode_On,
	
};
#endif

#if(MENU_REARCAM_TYPE_EN)
PSMENUITEM   sMenuRearCamType[] =
{
	&sItemRearCamType_None,				
	&sItemRearCamType_AIT,
	&sItemRearCamType_SONIX_MJPEG,
	&sItemRearCamType_SONIX_MJPEG2H264,	
	&sItemRearCamType_TV_DECODER,	
	&sItemRearCamType_BAYER_SENSOR,
	&sItemRearCamType_YUV_SENSOR,
};

#endif

/*PSMENUITEM   sMenuListMainVideo[] =
{
#if (MENU_MOVIE_SIZE_EN)
    &sItemMovieMode,
#endif

#if defined (CFG_LIST_STILL_IMAGE_SIZE)  && (MENU_STILL_SIZE_EN)
    &sItemStillImageSize,
#endif

#if (MENU_MOVIE_QUALITY_EN)    
    &sItemMovieQuality,
#endif
#if (MENU_MOVIE_CLIP_TIME_EN)       
    &sItemMovieClipTime,
#endif
#if (MENU_MOVIE_NIGHT_MODE_EN)
    &sItemMovieNightMode,
#endif    
#if (MENU_MOVIE_POWER_OFF_DELAY_EN)    
    &sItemMoviePowerOffTime, 
#endif
#if (MENU_MOVIE_VMD_REC_TIME_EN)
	&sItemMovieVMDRecTime, 
#endif
#if (MENU_MOVIE_AUTO_REC_EN)
	&sItemMovieAutoRec,
#endif
#if (MENU_MOVIE_SOUND_RECORD_EN)    
    &sItemMovieSoundRecord,  
#endif
#if (MENU_MOVIE_PRE_RECORD_EN)
	&sItemMoviePreRecord,
#endif
#if (MENU_MOVIE_MIC_SEN_EN)    
    &sItemMICSensitivity,
#endif    
#if (MENU_MOVIE_SCENE_EN)    
	&sItemScence, 
#endif
#if (MENU_MOVIE_EV_EN)	   
    &sItemEV,
#endif
#if (MENU_MOVIE_WB_EN)
    &sItemWB,
#endif    
#if (MENU_MOVIE_ISO_EN)    
    &sItemISO,
#endif
#if (MENU_MOVIE_COLOR_EN)	
	&sItemColor,
#endif
#if (MENU_MOVIE_EFFECT_EN)	
	&sItemEffect,
#endif
#if (MENU_MOVIE_PARKING_MODE_EN)
	&sItemMovieParkingMode,
#endif
#if (MENU_MOVIE_LDWS_MODE_EN)
	&sItemMovieLDWSMode,
#endif
#if (MENU_MOVIE_FCWS_MODE_EN)
	&sItemMovieFCWSMode,
#endif
#if (MENU_MOVIE_SAG_MODE_EN)
	&sItemMovieSAGMode,
#endif
#if (MENU_MOVIE_HDR_MODE_EN)
	&sItemMovieHDRMode,
#endif
#if (MENU_MOVIE_CONTRAST_EN)
    &sItemContrast,
#endif
#if (MENU_MOVIE_SATURATION_EN)
    &sItemSaturation,
#endif
#if (MENU_MOVIE_SHARPNESS_EN)
    &sItemSharpness,
#endif
#if (MENU_MOVIE_GAMMA_EN)
    &sItemGamma,
#endif
#if (MENU_VIDEO_TIMELAPSE_MODE_EN)	// VIDEO_REC_TIMELAPSE_EN
	&sItemMovieTimelapse,
#endif
#if (MENU_MOVIE_SLOWMOTION_MODE_EN)
	&sItemMovieSlowMotionMode,
#endif
#if (MENU_MOVIE_WNR_MODE_EN)
	&sItemMovieWNRMode,
#endif
#if (MENU_REARCAM_TYPE_EN)
	&sItemRearCamType,
#endif
};*/

/*===========================================================================
 * Global variable : Menu Structure
 *===========================================================================*/

//--------------SUB MENU-------------------

#if (MENU_MOVIE_SIZE_EN)
SMENUSTRUCT sSubMovieMode =
{
    MENUID_SUB_MENU_MOVIE_MODE,
    NULL,
    IDS_DS_MOVIE_MODE,
    NULL,
    sizeof(sMenuListMovieMode)/sizeof(PSMENUITEM),
    sMenuListMovieMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_MODE_DESCRIPTION,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_QUALITY_EN)
SMENUSTRUCT sSubMovieQuality =
{
    MENUID_SUB_MENU_MOVIE_QUALITY,
    NULL,
    IDS_DS_MOVIE_QUALITY,
    NULL,
    sizeof(sMenuListMovieQuality)/sizeof(PSMENUITEM),
    sMenuListMovieQuality,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_QUALITY_DESCRIPTION,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_STABILIZE_EN)
SMENUSTRUCT sSubMovieStable =
{
    MENUID_SUB_MENU_MOVIE_STABLEILIZATION,
    NULL,
    IDS_DS_MOVIE_STABLEILIZATION,
    NULL,
    sizeof(sMenuListMovieStable)/sizeof(PSMENUITEM),
    sMenuListMovieStable,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_STABLE_DESCRIPTION,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_SLOW_SHUTTER_EN)
SMENUSTRUCT sSubAutoSlowShutter =
{
    MENUID_SUB_MENU_AUTO_SLOW_SHUTTER,
    NULL,
    IDS_DS_AUTO_SLOW_SHUTTER,
    NULL,
    sizeof(sMenuListAutoSlowShutter)/sizeof(PSMENUITEM),
    sMenuListAutoSlowShutter,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_ASSHUTTER_DESCRIPTION,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_MIC_SEN_EN)
SMENUSTRUCT sSubMICSensitivity =
{
    MENUID_SUB_MENU_MIC_SENSITIVITY,
    NULL,
    IDS_DS_MIC_SENSITIVITY,
    NULL,
    sizeof(sMenuListMICSensitivity)/sizeof(PSMENUITEM),
    sMenuListMICSensitivity,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MIC_SENSITIVITY_DESCRIPTION,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_PRE_RECORD_EN)
SMENUSTRUCT sSubMoviePreRecord =
{
    MENUID_SUB_MENU_MOVIE_PRERECORD,
    NULL,
    IDS_DS_MOVIE_PRE_RECORD,
    NULL,
    sizeof(sMenuListMoviePreRecord)/sizeof(PSMENUITEM),
    sMenuListMoviePreRecord,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_MOVIE_PRE_RECORD_DESCRIPTION,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_CLIP_TIME_EN)
SMENUSTRUCT sSubMovieClipTime =
{
    MENUID_SUB_MENU_MOVIE_CLIPTIME,
    NULL,
    IDS_MOVIE_CLIPTIME,
    NULL,
    sizeof(sMenuListMovieClipTime)/sizeof(PSMENUITEM),
    sMenuListMovieClipTime,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_MOVIE_CLIPTIME,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_NIGHT_MODE_EN)
SMENUSTRUCT sSubMovieNightMode =
{
    MENUID_SUB_MENU_MOVIE_NIGHT_MODE,
    NULL,
    IDS_DS_NIGHT_MODE,
    NULL,
    sizeof(sMenuListMovieNightMode)/sizeof(PSMENUITEM),
    sMenuListMovieNightMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_DS_NIGHT_MODE,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_POWER_OFF_DELAY_EN)
SMENUSTRUCT sSubMoviePowerOffTime =
{
    MENUID_SUB_MENU_MOVIE_POWER_OFF_TIME,
    NULL,
    CFG_POWER_OFF_STR_ID,
    NULL,
    sizeof(sMenuListMoviePowerOffTime)/sizeof(PSMENUITEM),
    sMenuListMoviePowerOffTime,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    CFG_POWER_OFF_STR_ID,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_SOUND_RECORD_EN)
SMENUSTRUCT sSubMovieSoundRecord =
{
    MENUID_SUB_MENU_MOVIE_SOUND_RECORD,
    NULL,
    IDS_MOVIE_SOUND_RECORD,
    NULL,
    sizeof(sMenuListMovieSoundRecord)/sizeof(PSMENUITEM),
    sMenuListMovieSoundRecord,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0,
    IDS_MOVIE_SOUND_RECORD,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_VMD_REC_TIME_EN)
SMENUSTRUCT sSubMovieVMDRecTime =
{
    MENUID_SUB_MENU_MOVIE_VMD_REC_TIME,
    NULL,
	IDS_VMD_REC_TIME,	
    NULL,
    sizeof(sMenuListMovieVMDRecTime)/sizeof(PSMENUITEM),
    sMenuListMovieVMDRecTime,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_VMD_REC_TIME,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_AUTO_REC_EN)
SMENUSTRUCT sSubMovieAutoRec =
{
    MENUID_SUB_MENU_MOVIE_AUTO_REC,
    NULL,
	IDS_AUTO_REC,	
    NULL,
    sizeof(sMenuMovieAutoRec)/sizeof(PSMENUITEM),
    sMenuMovieAutoRec,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_AUTO_REC,
    AHC_FALSE,
    0
};
#endif

#if (MENU_VIDEO_TIMELAPSE_MODE_EN)	// VIDEO_REC_TIMELAPSE_EN
SMENUSTRUCT sSubMovieTimelapse =
{
    MENUID_SUB_MENU_VIDEO_TIMELAPSE,
    NULL,
	IDS_DS_TIMELAPSE,	
    NULL,
    sizeof(sMenuMovieTimelapse)/sizeof(PSMENUITEM),
    sMenuMovieTimelapse,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_TIMELAPSE,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_PARKING_MODE_EN)
SMENUSTRUCT sSubMovieParkingMode =
{
    MENUID_SUB_MENU_MOVIE_PARKING_MODE,
    NULL,
	IDS_DS_PARKING_MODE,	
    NULL,
    sizeof(sMenuMovieParkingMode)/sizeof(PSMENUITEM),
    sMenuMovieParkingMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_PARKING_MODE,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_LDWS_MODE_EN)
SMENUSTRUCT sSubMovieLDWSMode =
{
    MENUID_SUB_MENU_MOVIE_LDWS_MODE,
    NULL,
	IDS_DS_LDWS,	
    NULL,
    sizeof(sMenuMovieLDWSMode)/sizeof(PSMENUITEM),
    sMenuMovieLDWSMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_LDWS,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_FCWS_MODE_EN)
SMENUSTRUCT sSubMovieFCWSMode =
{
    MENUID_SUB_MENU_MOVIE_FCWS_MODE,
    NULL,
	IDS_DS_FCWS,
    NULL,
    sizeof(sMenuMovieFCWSMode)/sizeof(PSMENUITEM),
    sMenuMovieFCWSMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_FCWS,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_SAG_MODE_EN)
SMENUSTRUCT sSubMovieSAGMode =
{
    MENUID_SUB_MENU_MOVIE_SAG_MODE,
    NULL,
	IDS_DS_SAG,
    NULL,
    sizeof(sMenuMovieSAGMode)/sizeof(PSMENUITEM),
    sMenuMovieSAGMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_SAG,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_HDR_MODE_EN)
SMENUSTRUCT sSubMovieHDRMode =
{
    MENUID_SUB_MENU_MOVIE_HDR_MODE,
    NULL,
	IDS_DS_HDR,	
    NULL,
    sizeof(sMenuMovieHDRMode)/sizeof(PSMENUITEM),
    sMenuMovieHDRMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_HDR,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_SLOWMOTION_MODE_EN)
SMENUSTRUCT sSubMovieSlowMotionMode =
{
    MENUID_SUB_MENU_MOVIE_SLOWMOTION_MODE,
    NULL,
	IDS_DS_SLOWMOTION,	
    NULL,
    sizeof(sMenuMovieSlowMotionMode)/sizeof(PSMENUITEM),
    sMenuMovieSlowMotionMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_SLOWMOTION,
    AHC_FALSE,
    0
};
#endif

#if (MENU_MOVIE_WNR_MODE_EN)
SMENUSTRUCT sSubMovieWNRMode =
{
    MENUID_SUB_MENU_MOVIE_WNR_MODE,
    NULL,
	IDS_DS_WNR,	
    NULL,
    sizeof(sMenuMovieWNRMode)/sizeof(PSMENUITEM),
    sMenuMovieWNRMode,
    SubMenuEventHandler,
    MenuGetDefault_Movie,
    0, 
	IDS_DS_WNR,
    AHC_FALSE,
    0
};
#endif

#if(MENU_REARCAM_TYPE_EN)
SMENUSTRUCT sSubRearCamType =
{
	MENUID_SUB_MENU_REAR_TYPE,
	NULL,
	IDS_DS_NONE, 
	NULL,
	sizeof(sMenuRearCamType)/sizeof(PSMENUITEM),
	sMenuRearCamType,
	SubMenuEventHandler,
	MenuGetDefault_Movie,
	0, 
	IDS_DS_WNR,
	AHC_FALSE,
	0
};
#endif
/*===========================================================================
 * Main body
 *===========================================================================*/

#if (MENU_MOVIE_SIZE_EN) 
AHC_BOOL MenuItemMovieMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	MenuSettingConfig()->uiMOVSize= pItem->iItemId - ITEMID_MOVIE_MODE -1;
	Menu_SetMovieMode(MenuSettingConfig()->uiMOVSize);
    return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_QUALITY_EN)
AHC_BOOL MenuItemMovieQuality(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    MenuSettingConfig()->uiMOVQuality = pItem->iItemId - ITEMID_MOVIE_QUALITY -1;
    Menu_SetMovieQuality(MenuSettingConfig()->uiMOVQuality);    
    return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_STABILIZE_EN)
AHC_BOOL MenuItemMovieStable(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    MenuSettingConfig()->uiMOVStabilize = pItem->iItemId - ITEMID_MOVIE_STABLEILIZATION -1;
    return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_SLOW_SHUTTER_EN)
AHC_BOOL MenuItemAutoSlowShutter(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    MenuSettingConfig()->uiSlowShutter = pItem->iItemId - ITEMID_AUTO_SLOW_SHUTTER -1;
    return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_MIC_SEN_EN)
AHC_BOOL MenuItemMICSensitivity(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    MenuSettingConfig()->uiMicSensitivity = pItem->iItemId - ITEMID_MIC_SENSITIVITY -1;
	Menu_SetMICSensitivity(MenuSettingConfig()->uiMicSensitivity);    
    return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_PRE_RECORD_EN)
AHC_BOOL MenuItemMoviePreRecord(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	#if (VR_PREENCODE_EN)
	extern AHC_BOOL     m_ubPreEncodeEn;
	#endif

    if (ITEMID_MOVIE_PRE_RECORD_ON == pItem->iItemId)
    	MenuSettingConfig()->uiMOVPreRecord = MOVIE_PRE_RECORD_ON;
    else
    	MenuSettingConfig()->uiMOVPreRecord = MOVIE_PRE_RECORD_OFF;
    
	#if (VR_PREENCODE_EN)
	m_ubPreEncodeEn = (MenuSettingConfig()->uiMOVPreRecord == MOVIE_PRE_RECORD_ON)?(AHC_TRUE):(AHC_FALSE);
	#endif
	
    if(MenuSettingConfig()->uiMOVPreRecord == MOVIE_PRE_RECORD_ON)
    	AHC_VIDEO_SetMovieConfig(0, AHC_VIDEO_PRERECORD_STATUS, 1);
    else
    	AHC_VIDEO_SetMovieConfig(0, AHC_VIDEO_PRERECORD_STATUS, 0);    	
    return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_CLIP_TIME_EN)
AHC_BOOL MenuItemMovieClipTime(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	MenuSettingConfig()->uiMOVClipTime = pItem->iItemId - ITEMID_MOVIE_CLIPTIME -1;
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_NIGHT_MODE_EN)
AHC_BOOL MenuItemMovieNightMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	if (ITEMID_MOVIE_NIGHT_MODE_ON == pItem->iItemId) {
	    printc("MenuItemMovieNightMode: NIGHT_MODE ON\r\n");
        pf_NightMode_EnSet(COMMON_NIGHT_MODE_EN_ON);
        //AHC_SetParam(PARAM_ID_NIGHT_MODE_AE, 1);
    }    
    else {
        printc("MenuItemMovieNightMode: NIGHT_MODE OFF\r\n"); 
        pf_NightMode_EnSet(COMMON_NIGHT_MODE_EN_OFF);
        //AHC_SetParam(PARAM_ID_NIGHT_MODE_AE, 0);
    }
    printc("%s = %d\n", __func__,  pf_NightMode_EnGet());    
    	
	return AHC_TRUE;
}
#endif


#if (MENU_MOVIE_POWER_OFF_DELAY_EN)
AHC_BOOL MenuItemMoviePowerOffTime(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	MenuSettingConfig()->uiMOVPowerOffTime = pItem->iItemId - ITEMID_MOVIE_POWEROFF_TIME -1;
	VideoPowerOffCounterReset();
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_SOUND_RECORD_EN)
AHC_BOOL MenuItemMovieSoundRecord(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	extern AHC_BOOL	bMuteRecord;
	
	MenuSettingConfig()->uiMOVSoundRecord = pItem->iItemId - ITEMID_MOVIE_SOUND_RECORD -1;
	bMuteRecord = (MenuSettingConfig()->uiMOVSoundRecord == MOVIE_SOUND_RECORD_ON)?(AHC_FALSE):(AHC_TRUE);
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_VMD_REC_TIME_EN)
AHC_BOOL MenuItemMovieVMDRecTime(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	MenuSettingConfig()->uiVMDRecTime = pItem->iItemId - ITEMID_MOVIE_VMD_REC_TIME -1;
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_AUTO_REC_EN)
AHC_BOOL MenuItemMovieAutoRec(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	MenuSettingConfig()->uiAutoRec = pItem->iItemId - ITEMID_MOVIE_AUTO_REC -1;
	return AHC_TRUE;
}
#endif

#if (MENU_VIDEO_TIMELAPSE_MODE_EN)	// VIDEO_REC_TIMELAPSE_EN
AHC_BOOL MenuItemMovieTimelapse(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    pf_General_EnSet(COMMON_KEY_VR_TIMELAPSE, pItem->iItemId - ITEMID_MOVIE_TIMELAPSE -1);
	
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_PARKING_MODE_EN)
AHC_BOOL MenuItemMovieParkingMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    AHC_BOOL bParkingModeEn;

	bParkingModeEn = (pItem->iItemId - ITEMID_MOVIE_PARKING_MODE -1)? AHC_FALSE : AHC_TRUE;
	printc("MenuItemMovieParkingMode = %d\n", bParkingModeEn);

    if(uiGetParkingCfg()->bParkingModeFuncEn == AHC_TRUE)
    uiSetParkingModeEnable( bParkingModeEn );
 
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_LDWS_MODE_EN)
AHC_BOOL MenuItemMovieLDWSMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    if (ITEMID_MOVIE_LDWS_MODE_ON == pItem->iItemId) {
        pf_LDWS_EnSet(LDWS_EN_ON);
		printc("~aaaaaaa~~~~~MenuItemMovieLDWSMode~~~~~~~~~~~\r\n");
        
        #if (MOTION_DETECTION_EN) // TBD
        if(uiGetParkingCfg()->bParkingModeFuncEn == AHC_FALSE)
        {
	        // Turn-Off VMD
	        MenuSettingConfig()->uiMotionDtcSensitivity = MOTION_DTC_SENSITIVITY_OFF;
			Menu_SetMotionDtcSensitivity(MenuSettingConfig()->uiMotionDtcSensitivity);
			printc("~AAAAAAAAA~~~~~MenuItemMovieLDWSMode~~~~~~~~~~~\r\n");
		}
        #endif
    }
    else {
        pf_LDWS_EnSet(LDWS_EN_OFF);

    }

	printc("%s = %d\n", __func__, pf_LDWS_EnGet());
    
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_FCWS_MODE_EN)
AHC_BOOL MenuItemMovieFCWSMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    if (ITEMID_MOVIE_FCWS_MODE_ON == pItem->iItemId) { 

        pf_FCWS_EnSet(FCWS_EN_ON);
        
        #if (MOTION_DETECTION_EN) // TBD
        if(uiGetParkingCfg()->bParkingModeFuncEn == AHC_FALSE)
        {
	        // Turn-Off VMD
	        MenuSettingConfig()->uiMotionDtcSensitivity = MOTION_DTC_SENSITIVITY_OFF;
			Menu_SetMotionDtcSensitivity(MenuSettingConfig()->uiMotionDtcSensitivity);
        }
        #endif
    }
    else {
        pf_FCWS_EnSet(FCWS_EN_OFF);
    }
	printc("%s = %d\n", __func__, pf_FCWS_EnGet());
    
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_SAG_MODE_EN)
AHC_BOOL MenuItemMovieSAGMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    if (ITEMID_MOVIE_SAG_MODE_ON == pItem->iItemId) { 
        //pf_LDWS_EnSet(LDWS_EN_ON);
        //pf_FCWS_EnSet(FCWS_EN_ON);
        pf_SAG_EnSet(SAG_EN_ON);
        
        #if (MOTION_DETECTION_EN) // TBD
        if(uiGetParkingCfg()->bParkingModeFuncEn == AHC_FALSE)
        {
	        // Turn-Off VMD
	        MenuSettingConfig()->uiMotionDtcSensitivity = MOTION_DTC_SENSITIVITY_OFF;
			Menu_SetMotionDtcSensitivity(MenuSettingConfig()->uiMotionDtcSensitivity);
		}
        #endif
    }
    else {
        pf_SAG_EnSet(SAG_EN_OFF);
    }

	printc("%s = %d\n", __func__, pf_SAG_EnGet());
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_HDR_MODE_EN)
AHC_BOOL MenuItemMovieHDRMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
#if SNR_CLK_POWER_SAVING_SETTING
    pf_HDR_EnSet(COMMON_HDR_EN_OFF);
#else
    if (ITEMID_MOVIE_HDR_MODE_ON == pItem->iItemId)
        pf_HDR_EnSet(COMMON_HDR_EN_ON);
    else
        pf_HDR_EnSet(COMMON_HDR_EN_OFF);

	printc("%s = %d\n", __func__, pf_HDR_EnGet());
#endif
    
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_SLOWMOTION_MODE_EN)
UINT8 setVal;
AHC_BOOL MenuItemMovieSlowMotionMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
	setVal = pItem->iItemId - ITEMID_MOVIE_SLOWMOTION_MODE -1;
	printc(FG_RED("***** setVal = %d \r\n"),setVal);
	pf_SLOWMOTION_EnSet(setVal);
	
	return AHC_TRUE;
}
#endif

#if (MENU_MOVIE_WNR_MODE_EN)
AHC_BOOL MenuItemMovieWNRMode(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
#if SNR_CLK_POWER_SAVING_SETTING
    pf_WNR_EnSet(COMMON_WNR_EN_OFF);
#else
    if (ITEMID_MOVIE_WNR_MODE_ON == pItem->iItemId)
        pf_WNR_EnSet(COMMON_WNR_EN_ON);
    else
        pf_WNR_EnSet(COMMON_WNR_EN_OFF);

	printc("%s = %d\n", __func__, pf_WNR_EnGet());
#endif
    
	return AHC_TRUE;
}
#endif

#if(MENU_REARCAM_TYPE_EN)
AHC_BOOL MenuItemRearCamType(PSMENUITEM pItem, AHC_BOOL bIsHover)
{
    MMP_ERR MMPF_JPEG_SetCtrlByQueueEnable(MMP_BOOL bEnable);

	int id;

	switch(pItem->iItemId)
	{
		case ITEMID_REARCAM_TYPE_NONE:
			id = SCD_CAM_NONE;
			break;
		case ITEMID_REARCAM_TYPE_AIT:
			id = USB_CAM_AIT;
			break;
		case ITEMID_REARCAM_TYPE_SONIX_MJPEG:
			id = USB_CAM_SONIX_MJPEG;
			break;
		case ITEMID_REARCAM_TYPE_SONIX_MJPEG2H264:
			id = USB_CAM_SONIX_MJPEG2H264;
			break;
		case ITEMID_REARCAM_TYPE_TV_DECODER:
			id = USB_CAM_SONIX_MJPEG2H264 + SCD_CAM_TV_DECODER;
			break;
		case ITEMID_REARCAM_TYPE_BAYER_SENSOR:
			id = USB_CAM_SONIX_MJPEG2H264 + SCD_CAM_BAYER_SENSOR;
			break;
		case ITEMID_REARCAM_TYPE_YUV_SENSOR:
			id = USB_CAM_SONIX_MJPEG2H264 + SCD_CAM_YUV_SENSOR;
			break;
		default:
			id = SCD_CAM_NONE;
			break;
	}

	printc("%s : RearCam ID= %d\n", __func__, id);
	
	pf_RearCamType_EnSet(id);
	
	if (pItem->iItemId == ITEMID_REARCAM_TYPE_NONE) {
	    MMP_SetScdCamType(SCD_CAM_NONE);
	    MMP_SetUSBCamType(USB_CAM_NONE);
	} 
	else if (id == USB_CAM_AIT ||
	         id == USB_CAM_SONIX_MJPEG ||
	         id == USB_CAM_SONIX_MJPEG2H264) {
	    MMP_SetScdCamType(SCD_CAM_NONE);         
	    MMP_SetUSBCamType(id);       
	} 
	else if (id == SCD_CAM_TV_DECODER ||
	         id == SCD_CAM_BAYER_SENSOR ||
	         id == SCD_CAM_YUV_SENSOR) {
	    MMP_SetScdCamType(id);         
	    MMP_SetUSBCamType(USB_CAM_NONE);   
	}

    //MMPC_Display_InitConfig();
    //MMPC_DSC_InitConfig();
    MMPC_3gpRecd_InitConfig();
    MMPC_3gpPlay_InitConfig();
    MMPC_Audio_InitConfig();
	
    #if (SUPPORT_USB_HOST_FUNC)
    MMP_InitUSBHConfig(CAM_GET_USB);
    #endif
    
    #if defined(ALL_FW) && (HANDLE_JPEG_EVENT_BY_QUEUE)
    if (CAM_CHECK_SCD(SCD_CAM_TV_DECODER) ||
        CAM_CHECK_SCD(SCD_CAM_YUV_SENSOR)) {
        MMPF_JPEG_SetCtrlByQueueEnable(MMP_FALSE);
    }
    else {
        MMPF_JPEG_SetCtrlByQueueEnable(MMP_TRUE);
    }
    #endif
	
    #if defined(ALL_FW) && (HANDLE_H264E_EVENT_BY_QUEUE)
    if (CAM_CHECK_SCD(SCD_CAM_YUV_SENSOR)) {
        MMPF_H264E_SetCtrlByQueueEnable(MMP_TRUE);
    }
    else {
        MMPF_H264E_SetCtrlByQueueEnable(MMP_FALSE);
    }
    #endif
	printc( "MenuChangeRearCamType(%d)\r\n", id);
	StateSwitchMode(UI_VIDEO_STATE);
	return AHC_TRUE;
}
#endif


UINT32 MenuGetDefault_Movie(PSMENUSTRUCT pMenu)
{
    UINT32	DefaultValue = 0;
	

   //return 0;
    switch( pMenu->iMenuId )
    {
    	case MENUID_SUB_MENU_LCD:
		DefaultValue = MenuSettingConfig()->uiLCDBrightness;	
		break;
	case MENUID_SUB_MENU_LOGO:
		DefaultValue =MenuSettingConfig()->uiHDMIOutput;//logo 用这个标志位
		break;
	case MENUID_SUB_MENU_GSENSOR:
		DefaultValue =MenuSettingConfig()->uiGsensorSensitivity-1;
		break;
	case MENUID_SUB_MENU_MIC:
		DefaultValue =MenuSettingConfig()->uiMOVSoundRecord;
		break;
	case MENUID_SUB_MENU_GPS:
		DefaultValue =MenuSettingConfig()->uiRecordGPS;
		break;
	case MENUID_SUB_MENU_SETTINGVOLT:
	case MENUID_SUB_MENU_SETTINGVOLT1:
	case MENUID_SUB_MENU_SETTINGVOLT2:
		DefaultValue =MenuSettingConfig()->uiSlideShowEffect%5;
		break;
	case MENUID_SUB_MENU_SETTINGVOICE:
		DefaultValue =(MenuSettingConfig()->uiVolume-4)/2;
		break;
	case MENUID_SUB_MENU_SETTINGEVENT:
		DefaultValue =MenuSettingConfig()->uiGPSStamp;
		break;
	case MENUID_SUB_MENU_SETTINGFORMATE:
		 DefaultValue =MenuSettingConfig()->uiWDR;
		break;
	#if(MENU_MOVIE_SIZE_1080P_EN)
	case MENUID_SUB_MENU_MOVIE_MODE:
		DefaultValue =MenuSettingConfig()->uiMOVSize;
		break;
	#endif
	default :
		DefaultValue=0;
		break;
		printc("***** ######################### \r\n");	
		printc("***** DefaultValue = %dpMenu->iMenuId=%d \r\n",DefaultValue,pMenu->iMenuId);	
		printc("***** ######################### \r\n");	
/*#if (MENU_MOVIE_SIZE_EN)
        case MENUID_SUB_MENU_MOVIE_MODE:
            DefaultValue = MenuSettingConfig()->uiMOVSize ;
            break;
#endif
#if (MENU_MOVIE_QUALITY_EN)
        case MENUID_SUB_MENU_MOVIE_QUALITY:
            DefaultValue = MenuSettingConfig()->uiMOVQuality ;
            break;
#endif   
#if (MENU_MOVIE_STABILIZE_EN)
        case MENUID_SUB_MENU_MOVIE_STABLEILIZATION:
            DefaultValue = MenuSettingConfig()->uiMOVStabilize;
            break;
#endif
#if (MENU_MOVIE_SLOW_SHUTTER_EN)
        case MENUID_SUB_MENU_AUTO_SLOW_SHUTTER:
            DefaultValue = MenuSettingConfig()->uiSlowShutter;
            break;
#endif
#if (MENU_MOVIE_MIC_SEN_EN)
        case MENUID_SUB_MENU_MIC_SENSITIVITY:
            DefaultValue = MenuSettingConfig()->uiMicSensitivity;
            break;
#endif            
#if (MENU_MOVIE_PRE_RECORD_EN)
		case MENUID_SUB_MENU_MOVIE_PRERECORD:
        	DefaultValue = MenuSettingConfig()->uiMOVPreRecord;
        	break;
#endif
#if (MENU_MOVIE_CLIP_TIME_EN)
        case MENUID_SUB_MENU_MOVIE_CLIPTIME:
            DefaultValue = MenuSettingConfig()->uiMOVClipTime;
            break;
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_EN)
       	case MENUID_SUB_MENU_MOVIE_POWER_OFF_TIME:
            DefaultValue = MenuSettingConfig()->uiMOVPowerOffTime;
            break;
#endif
#if (MENU_MOVIE_SOUND_RECORD_EN)
       	case MENUID_SUB_MENU_MOVIE_SOUND_RECORD:
       		DefaultValue = MenuSettingConfig()->uiMOVSoundRecord;
       		break;
#endif
#if (MENU_MOVIE_VMD_REC_TIME_EN)
       	case MENUID_SUB_MENU_MOVIE_VMD_REC_TIME:
            DefaultValue = MenuSettingConfig()->uiVMDRecTime;
            break;
#endif
#if (MENU_MOVIE_AUTO_REC_EN)
		case MENUID_SUB_MENU_MOVIE_AUTO_REC:
			DefaultValue = MenuSettingConfig()->uiAutoRec;
			break;
#endif
#if (MENU_VIDEO_TIMELAPSE_MODE_EN)	// VIDEO_REC_TIMELAPSE_EN
		case MENUID_SUB_MENU_VIDEO_TIMELAPSE:
            pf_General_EnGet(COMMON_KEY_VR_TIMELAPSE, (INT32 *) &DefaultValue);
            break;
#endif
#if (MENU_MOVIE_PARKING_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_PARKING_MODE:
            DefaultValue = ( uiGetParkingModeEnable() == AHC_TRUE )? 0 : 1;
            break;
#endif
#if (MENU_MOVIE_LDWS_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_LDWS_MODE:
            DefaultValue = (pf_LDWS_EnGet() == LDWS_EN_ON) ? (ITEMID_MOVIE_LDWS_MODE_ON - ITEMID_MOVIE_LDWS_MODE - 1) : (ITEMID_MOVIE_LDWS_MODE_OFF - ITEMID_MOVIE_LDWS_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_FCWS_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_FCWS_MODE:
            DefaultValue = (pf_FCWS_EnGet() == FCWS_EN_ON) ? (ITEMID_MOVIE_FCWS_MODE_ON - ITEMID_MOVIE_FCWS_MODE - 1) : (ITEMID_MOVIE_FCWS_MODE_OFF - ITEMID_MOVIE_FCWS_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_SAG_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_SAG_MODE:
            DefaultValue = (pf_SAG_EnGet() == SAG_EN_ON) ? (ITEMID_MOVIE_SAG_MODE_ON - ITEMID_MOVIE_SAG_MODE - 1) : (ITEMID_MOVIE_SAG_MODE_OFF - ITEMID_MOVIE_SAG_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_HDR_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_HDR_MODE:
            DefaultValue = (pf_HDR_EnGet() == HDR_EN_ON) ? (ITEMID_MOVIE_HDR_MODE_ON - ITEMID_MOVIE_HDR_MODE - 1) : (ITEMID_MOVIE_HDR_MODE_OFF - ITEMID_MOVIE_HDR_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_SLOWMOTION_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_SLOWMOTION_MODE:
            DefaultValue = pf_SLOWMOTION_EnGet();
            break;
#endif
#if (MENU_MOVIE_WNR_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_WNR_MODE:
            DefaultValue = (pf_WNR_EnGet() == WNR_EN_ON) ? (ITEMID_MOVIE_WNR_MODE_ON - ITEMID_MOVIE_WNR_MODE - 1) : (ITEMID_MOVIE_WNR_MODE_OFF - ITEMID_MOVIE_WNR_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_CONTRAST_EN)
        case MENUID_SUB_MENU_CONTRAST:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_CONTRAST, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
#if (MENU_MOVIE_SATURATION_EN)
        case MENUID_SUB_MENU_SATURATION:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_SATURATION, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
#if (MENU_MOVIE_SHARPNESS_EN)
        case MENUID_SUB_MENU_SHARPNESS:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_SHARPNESS, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
#if (MENU_MOVIE_GAMMA_EN)
        case MENUID_SUB_MENU_GAMMA:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_GAMMA, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
	*/
    }
	printc("MenuGetDefault_Movie--DefaultValue=%d\n",DefaultValue);
	return DefaultValue;
}
/*UINT32 MenuGetDefault_Movie(PSMENUSTRUCT pMenu)
{
    UINT32	DefaultValue = 0;

    switch( pMenu->iMenuId )
    {
#if (MENU_MOVIE_SIZE_EN)
        case MENUID_SUB_MENU_MOVIE_MODE:
            DefaultValue = MenuSettingConfig()->uiMOVSize ;
            break;
#endif
#if (MENU_MOVIE_QUALITY_EN)
        case MENUID_SUB_MENU_MOVIE_QUALITY:
            DefaultValue = MenuSettingConfig()->uiMOVQuality ;
            break;
#endif   
#if (MENU_MOVIE_STABILIZE_EN)
        case MENUID_SUB_MENU_MOVIE_STABLEILIZATION:
            DefaultValue = MenuSettingConfig()->uiMOVStabilize;
            break;
#endif
#if (MENU_MOVIE_SLOW_SHUTTER_EN)
        case MENUID_SUB_MENU_AUTO_SLOW_SHUTTER:
            DefaultValue = MenuSettingConfig()->uiSlowShutter;
            break;
#endif
#if (MENU_MOVIE_MIC_SEN_EN)
        case MENUID_SUB_MENU_MIC_SENSITIVITY:
            DefaultValue = MenuSettingConfig()->uiMicSensitivity;
            break;
#endif            
#if (MENU_MOVIE_PRE_RECORD_EN)
		case MENUID_SUB_MENU_MOVIE_PRERECORD:
        	DefaultValue = MenuSettingConfig()->uiMOVPreRecord;
        	break;
#endif
#if (MENU_MOVIE_CLIP_TIME_EN)
        case MENUID_SUB_MENU_MOVIE_CLIPTIME:
            DefaultValue = MenuSettingConfig()->uiMOVClipTime;
            break;
#endif
#if (MENU_MOVIE_POWER_OFF_DELAY_EN)
       	case MENUID_SUB_MENU_MOVIE_POWER_OFF_TIME:
            DefaultValue = MenuSettingConfig()->uiMOVPowerOffTime;
            break;
#endif
#if (MENU_MOVIE_SOUND_RECORD_EN)
       	case MENUID_SUB_MENU_MOVIE_SOUND_RECORD:
       		DefaultValue = MenuSettingConfig()->uiMOVSoundRecord;
       		break;
#endif
#if (MENU_MOVIE_VMD_REC_TIME_EN)
       	case MENUID_SUB_MENU_MOVIE_VMD_REC_TIME:
            DefaultValue = MenuSettingConfig()->uiVMDRecTime;
            break;
#endif
#if (MENU_MOVIE_AUTO_REC_EN)
		case MENUID_SUB_MENU_MOVIE_AUTO_REC:
			DefaultValue = MenuSettingConfig()->uiAutoRec;
			break;
#endif
#if (MENU_VIDEO_TIMELAPSE_MODE_EN)	// VIDEO_REC_TIMELAPSE_EN
		case MENUID_SUB_MENU_VIDEO_TIMELAPSE:
            pf_General_EnGet(COMMON_KEY_VR_TIMELAPSE, (INT32 *) &DefaultValue);
            break;
#endif
#if (MENU_MOVIE_PARKING_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_PARKING_MODE:
            DefaultValue = ( uiGetParkingModeEnable() == AHC_TRUE )? 0 : 1;
            break;
#endif
#if (MENU_MOVIE_LDWS_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_LDWS_MODE:
            DefaultValue = (pf_LDWS_EnGet() == LDWS_EN_ON) ? (ITEMID_MOVIE_LDWS_MODE_ON - ITEMID_MOVIE_LDWS_MODE - 1) : (ITEMID_MOVIE_LDWS_MODE_OFF - ITEMID_MOVIE_LDWS_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_FCWS_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_FCWS_MODE:
            DefaultValue = (pf_FCWS_EnGet() == FCWS_EN_ON) ? (ITEMID_MOVIE_FCWS_MODE_ON - ITEMID_MOVIE_FCWS_MODE - 1) : (ITEMID_MOVIE_FCWS_MODE_OFF - ITEMID_MOVIE_FCWS_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_SAG_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_SAG_MODE:
            DefaultValue = (pf_SAG_EnGet() == SAG_EN_ON) ? (ITEMID_MOVIE_SAG_MODE_ON - ITEMID_MOVIE_SAG_MODE - 1) : (ITEMID_MOVIE_SAG_MODE_OFF - ITEMID_MOVIE_SAG_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_HDR_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_HDR_MODE:
            DefaultValue = (pf_HDR_EnGet() == HDR_EN_ON) ? (ITEMID_MOVIE_HDR_MODE_ON - ITEMID_MOVIE_HDR_MODE - 1) : (ITEMID_MOVIE_HDR_MODE_OFF - ITEMID_MOVIE_HDR_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_SLOWMOTION_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_SLOWMOTION_MODE:
            DefaultValue = pf_SLOWMOTION_EnGet();
            break;
#endif
#if (MENU_MOVIE_WNR_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_WNR_MODE:
            DefaultValue = (pf_WNR_EnGet() == WNR_EN_ON) ? (ITEMID_MOVIE_WNR_MODE_ON - ITEMID_MOVIE_WNR_MODE - 1) : (ITEMID_MOVIE_WNR_MODE_OFF - ITEMID_MOVIE_WNR_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_NIGHT_MODE_EN)
        case MENUID_SUB_MENU_MOVIE_NIGHT_MODE:
            DefaultValue = (pf_NightMode_EnGet() == COMMON_NIGHT_MODE_EN_ON) ? (ITEMID_MOVIE_NIGHT_MODE_ON - ITEMID_MOVIE_NIGHT_MODE - 1) : (ITEMID_MOVIE_NIGHT_MODE_OFF - ITEMID_MOVIE_NIGHT_MODE - 1);
            break;
#endif
#if (MENU_MOVIE_CONTRAST_EN)
        case MENUID_SUB_MENU_CONTRAST:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_CONTRAST, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
#if (MENU_MOVIE_SATURATION_EN)
        case MENUID_SUB_MENU_SATURATION:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_SATURATION, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
#if (MENU_MOVIE_SHARPNESS_EN)
        case MENUID_SUB_MENU_SHARPNESS:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_SHARPNESS, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
#if (MENU_MOVIE_GAMMA_EN)
        case MENUID_SUB_MENU_GAMMA:
            {
                INT32 s32Setting = 0;
                
                pf_General_EnGet(COMMON_KEY_GAMMA, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
            }
            break;
#endif
#if (MENU_REARCAM_TYPE_EN)
		case MENUID_SUB_MENU_REAR_TYPE:
			{
				INT32 s32Setting = 0;
				pf_General_EnGet(COMMON_KEY_REARCAM_TYPE, &s32Setting);
                DefaultValue = (UINT32) s32Setting;
			}
#endif
    }

	return DefaultValue;
}*/

#if 0
 UINT32 MenuStateVideoMode( UINT32 ulEvent, UINT32 ulPosition )
{
    //UINT32       ulOperation;
    UINT32       ulResult;
    PSMENUSTRUCT pCurrMenu = NULL;

#if 0
    if( BUTTON_VIRTUAL_RESET == ulEvent )
    {  
#if (TOP_MENU_PAGE_EN)
        SetCurrentMenu(&sTopMenu);
#else
        SetCurrentMenu(&sMainMenuVideo);
#endif
    }
#endif

#if 0
    MenuModePreCheck(ulEvent);

    ulOperation = MenuButtonToMenuOp( ulEvent ,ulPosition);

 	if(ulOperation == MENU_NONE)
        return;

#ifdef NET_SYNC_PLAYBACK_MODE
	if(EVENT_NET_ENTER_PLAYBACK == ulOperation)
	{
		ResetCurrentMenu();
		Menu_WriteSetting();
		bForceSwitchBrowser = AHC_FALSE;
		StateSwitchMode(UI_NET_PLAYBACK_STATE);
        CGI_SET_STATUS(ulOperation, 0 /* SUCCESSFULLY */)
        return;
	}
#endif
#endif
    pCurrMenu = GetCurrentMenu();

    if(pCurrMenu == NULL)
    	return;

    //MainMenuEventHandler
    ulResult = pCurrMenu->pfEventHandler( pCurrMenu, ulEvent, ulPosition );

    if( ulResult == MENU_ERR_EXIT ){
#ifdef SLIDE_MENU
        if(IsSlidingMenu())
            StopSlideMenu();
#endif    

        ResetCurrentMenu();
        Menu_WriteSetting();
    }

    return ulResult;
}
#endif

#if 0
void MenuStateClockSettingsMode( UINT32 ulEvent, UINT32 ulPosition )
{
    UINT32       ulOperation;
    UINT32       ulResult;
    PSMENUSTRUCT pCurrMenu = NULL;
	
    if( BUTTON_VIRTUAL_RESET == ulEvent )
    {
    	SetCurrentMenu(&sSubClockSettings);
  	}

	MenuModePreCheck(ulEvent);

    ulOperation = MenuButtonToMenuOp( ulEvent ,ulPosition);

 	if(ulOperation == MENU_NONE)
        return;

    pCurrMenu = GetCurrentMenu();

    if( pCurrMenu == NULL )
    	return;

    //SubMenuEventHandler
    ulResult = pCurrMenu->pfEventHandler( pCurrMenu, ulOperation, ulPosition );

    if( ulResult == MENU_ERR_EXIT )
    {
        ResetCurrentMenu();

        StateSwitchMode(UI_VIDEO_STATE);
    }
}
#endif

#if (SD_UPDATE_FW_EN)
void MenuStateSDUpdateMode( UINT32 ulEvent, UINT32 ulPosition )
{
    SD_UPDATER_ERR eError;
    
    printc("\r\n MenuStateSDUpdateMode \r\n");
    
    AHC_SetMode(AHC_MODE_IDLE);
    
    eError = SDUpdateCheckFileExisted(SD_FW_UPDATER_BIN_NAME);
	if(eError != SD_UPDATER_ERR_FILE) {
		printc(FG_YELLOW("Update SD:\\SD_CarDV.bin success!\r\n"));      
    }
    else {
        eError = SDUpdateCheckWPExisted(SD_WP_FAT_BIN_NAME);
        if (eError != SD_UPDATER_ERR_FILE) {
            if (AHC_FS_FileRemove(SD_WP_FAT_BIN_NAME,  STRLEN(SD_WP_FAT_BIN_NAME)) ) {
                printc(FG_YELLOW("Update SD:\\SD_WpFat.bin success!\r\n"));
            }
        }        
    }
    
    {
        UINT32 i;
		MMP_BYTE Index[2];
        MMP_BYTE SDfilename[64] = {0};
        
        for (i = 0; i<gubWpFileNum; i++) {
			MEMSET0(SDfilename);            
			STRCPY(SDfilename, SD_WP_BASENAME);            
			sprintf(Index,"%d",i);
			STRCAT(SDfilename,Index);
    		STRCAT(SDfilename, SD_WP_EXTNAME);            
            
            if (SDUpdateCheckWpFsExisted(SDfilename) != SD_UPDATER_ERR_FILE) {
    			MEMSET0(SDfilename);            
    			STRCPY(SDfilename, SD_WP_BASENAME);            
    			sprintf(Index,"%d",i);
    			STRCAT(SDfilename,Index);
        		STRCAT(SDfilename, SD_WP_EXTNAME);                 
                if (AHC_FS_FileRemove(SDfilename,  STRLEN(SDfilename))) {
                    printc(FG_YELLOW("Update %s success!\r\n"), SDfilename);                    
                }
            }
        }
    }
    
	#if(EDOG_ENABLE)
    eError = SDUpdateCheckDBExisted(SD_DB_UPDATER_BIN_NAME);
	if(eError != SD_UPDATER_ERR_FILE)
	{
		printc("Update SD:\\SpeedCam_Data.bin success!\r\n");
	}
	#endif

	if(eError != SD_UPDATER_ERR_FILE)
	{
		if (eError == SD_UPDATER_ERR_FAIL)
		    AHC_OS_Sleep(3000);
					    
		// Update Success
		uiPowerOffPicture();
		AHC_PMU_PowerOff();
    }
}
#endif


AHC_BOOL MenuStateVideoModeInit(void* pData)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    UINT32 ulEvent = 0, ulResult = 0;
    
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif

    ulEvent = MenuButtonToMenuOp(BUTTON_VIRTUAL_RESET , 0);
#if (TOP_MENU_PAGE_EN)
    SetCurrentMenu(&sTopMenu);
#else
    SetCurrentMenu(&sMainMenuVideo);
#endif
    ulResult = MenuStateExecutionCommon(ulEvent, 0);//ulResult = MenuStateVideoMode(ulEvent, 0);

#if 0
    ahcRet = AHC_OSDSetActive(OVL_DISPLAY_BUFFER, 1);
    if(ahcRet != AHC_TRUE){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk,ahcRet);} 
#endif

    return ahcRet;
}

AHC_BOOL MenuStateVideoModeShutDown(void* pData)
{
#if (1)
    printc("%s,%d \n", __func__, __LINE__);

	AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_MENU_EXIT, 0);

#endif
	
    return AHC_TRUE;
}

DECL_AHC_EV_HANDLER(STATE_CLOCK_SETTING_MODE_EVENT_ENTER_NET_PLAYBACK)
{
	StateSwitchMode(UI_NET_PLAYBACK_STATE);
    CGI_SET_STATUS(ulEvent, CGI_ERR_NONE /* SUCCESSFULLY */);
}

DECL_AHC_EV_HANDLER(STATE_CLOCK_SETTING_MODE_EVENT_USB_REMOVED_LCD)
{
   /* In Video state, the event should always be USB adapter out,
     * the real USB plug-out event should be received in MSDC state.
     *
     * The function AHC_USB_GetLastStatus() may return wrong status,
     * when USB plug-in/out then adapter plug-in.
     *
     */
    if (AHC_IsDcCableConnect() == AHC_TRUE)
        return;//break;

    //AHC_Charger_SetEnable(AHC_FALSE);
}

DECL_AHC_EV_HANDLER(STATE_CLOCK_SETTING_MODE_EVENT_USB_DETECT_LCD)
{
    if(IsAdapter_Connect())
    {
        if (VideoFunc_RecordStatus()) {
            // Video Recording to ignore USB Charger
            return;//break;
        }
        else {
            //Power Off
        }
    }
    else
    {
        if (MMPS_USB_NORMAL == MMPS_USB_GetStatus()) {
            StateSwitchMode(UI_USBSELECT_MENU_STATE);
        }
        else
        {
            StateSwitchMode(UI_VIDEO_STATE);
        }
    }
}

DECL_AHC_EV_HANDLER(STATE_CLOCK_SETTING_MODE_EVENT_USB_B_DEVICE_DETECT)
{
	{
		/////////////////////////////////////////////////////// Description ////////////////////////////////////////////////////
		// EVENT_USB_B_DEVICE_DETECT: Got when Rear Cam streaming was detected
		// EVENT_USB_B_DEVICE_REMOVED: Got when Rear Cam streaming was disconnected
		// Note: When some UVC error happened (ex: AHC_HostUVCVideoSetEp0TimeoutCB() / AHC_HostUVCVideoSetFrmRxTimeout2CB() was called),
		//		 EVENT_USB_B_DEVICE_REMOVED will be sent. Below sample code will stop current recording and start next recording with only Front Cam.
		//		 Once AHC_USBDetectHandler() detected Rear Cam streaming was recovered, EVENT_USB_B_DEVICE_DETECT will be sent.
		//		 Below sample code will stop current recording and start next recording with Front & Rear Cam.
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AHC_PRINT_RET_ERROR(0, 0);
	
		AHC_USB_PauseDetection(0);
		if((AHC_GetCurKeyEventID() == BUTTON_USB_B_DEVICE_DETECTED) ||
			(AHC_GetCurKeyEventID() == BUTTON_USB_B_DEVICE_REMOVED)){
			AHC_SetCurKeyEventID(EVENT_NONE);
		}
		else{
			AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0);
			printc("KeyEventID: BUTTON_USB_B_DEVICE_DETECTED,REMOVED is interrupted.\r\n");
		}  	
	}
}

#if (TVOUT_ENABLE)
DECL_AHC_EV_HANDLER(STATE_CLOCK_SETTING_MODE_EVENT_TV_DETECT)
{
    StateSwitchMode(UI_CLOCK_SETTING_STATE);
	//StateSwitchMode(UI_VIDEO_STATE);
}
#endif

#if (HDMI_ENABLE)
DECL_AHC_EV_HANDLER(STATE_CLOCK_SETTING_MODE_EVENT_HDMI_DETECT)
{
	StateSwitchMode(UI_VIDEO_STATE);
}
#endif

UINT32 MenuStateClockSettingsModeHandler(UINT32 ulMsgId, UINT32 ulEvent, UINT32 ulParam)
{
    UINT32 ulResult = 0;
    //MenuStateClockSettingsMode(ulEvent, ulParam);

    ulResult = MenuStateHandlerCommon(ulMsgId, ulEvent, ulParam);
    if(MENU_ERR_EXIT == ulResult){
        StateSwitchMode(UI_VIDEO_STATE);
    }
    else{
    	ulEvent = KeyParser_ClockSettingEvent(ulMsgId, ulEvent, ulParam);
    	StateModeDoHandlerEvent(ulMsgId, ulEvent, ulParam);
    }
    return 0;
}

#if 0
AHC_BOOL MenuStateClockSettingsModeInit(void* pData)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif
    MMPC_HDMI_PC_Enable(MMP_TRUE);
    //SwitchUIDrawSet(UI_VIDEO_STATE);
    SwitchUIDrawSetVideoRecordMode();
    
    StateLCDCheckStatus(UI_CLOCK_SETTING_STATE);
    MenuStateClockSettingsMode(BUTTON_VIRTUAL_RESET, 0);
    return ahcRet;
}
#endif

AHC_BOOL MenuStateClockSettingsModeInitCommon(void)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    UINT32 ulEvent = 0, ulResult = 0;
    
    ulEvent = MenuButtonToMenuOp(BUTTON_VIRTUAL_RESET , 0);
    SetCurrentMenu(&sSubClockSettings);

    ulResult = MenuStateExecutionCommon(ulEvent, 0);

    return ahcRet;
}

AHC_BOOL MenuStateClockSettingsModeInitLCD(void* pData)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif

    ahcRet = AHC_SwitchDisplayOutputDevLCD();
    if(ahcRet != AHC_TRUE){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk, ahcRet); return AHC_FALSE;}
    
    ahcRet = MenuStateClockSettingsModeInitCommon();
    if(ahcRet != AHC_TRUE){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk, ahcRet); return AHC_FALSE;}

    return ahcRet;
}

#if (TVOUT_ENABLE)
AHC_BOOL MenuStateClockSettingsModeInitTV(void* pData)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif

    ahcRet = AHC_SwitchDisplayOutputDevTVout();
    if(ahcRet != AHC_TRUE){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk, ahcRet); return AHC_FALSE;}
    
    ahcRet = MenuStateClockSettingsModeInitCommon();
    if(ahcRet != AHC_TRUE){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk, ahcRet); return AHC_FALSE;}

    return ahcRet;
}
#endif

#if (HDMI_ENABLE)
AHC_BOOL MenuStateClockSettingsModeInitHDMI(void* pData)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif

    ahcRet = AHC_SwitchDisplayOutputDevHDMI();
    if(ahcRet != AHC_TRUE){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk, ahcRet); return AHC_FALSE;}
    
    ahcRet = MenuStateClockSettingsModeInitCommon();
    if(ahcRet != AHC_TRUE){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk, ahcRet); return AHC_FALSE;}

    return ahcRet;
}
#endif

AHC_BOOL MenuStateClockSettingsModeShutDown(void* pData)
{
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif

    AHC_SetMode(AHC_MODE_IDLE);
	
    return AHC_TRUE;
}

AHC_BOOL MenuStateEditConfirmModeShutDown(void* pData)
{
    printc("%s,%d \n", __func__, __LINE__);
	
    return AHC_TRUE;
}

#if (SD_UPDATE_FW_EN)
UINT32 MenuStateSDUpdateModeHandler(UINT32 ulMsgId, UINT32 ulEvent, UINT32 ulParam )
{
    AHC_SDUpdateMode();
    return 0;
}

AHC_BOOL MenuStateSDUpdateModeInit(void* pData)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif

    /* At this monunt, The StateLCDCheckStatus will set LCD Direction,
    * and it will cause system crash when booting from TV
    */
    if ((AHC_FALSE == AHC_IsTVConnectEx()) && (AHC_FALSE == AHC_IsHdmiConnect()))
        StateLCDCheckStatus(UI_SD_UPDATE_STATE);

    MenuStateSDUpdateMode(BUTTON_VIRTUAL_RESET, 0);

    return ahcRet;
}

AHC_BOOL MenuStateSDUpdateModeShutDown(void* pData)
{
#if (1)
    printc("%s,%d \n", __func__, __LINE__);
#endif
	
    return AHC_TRUE;
}

AHC_BOOL StateSelectFuncClockSettingMode(void)
{
    AHC_BOOL    ahcRet = AHC_TRUE;

#if defined(WIFI_PORT) && (WIFI_PORT == 1)
    RegisterEventCb(EVENT_NET_ENTER_PLAYBACK,			STATE_CLOCK_SETTING_MODE_EVENT_ENTER_NET_PLAYBACK);
#endif

	RegisterEventCb(EVENT_USB_DETECT,					STATE_CLOCK_SETTING_MODE_EVENT_USB_DETECT_LCD);
	RegisterEventCb(EVENT_USB_REMOVED, 					STATE_CLOCK_SETTING_MODE_EVENT_USB_REMOVED_LCD);

#if (UVC_HOST_VIDEO_ENABLE)
	RegisterEventCb(EVENT_USB_B_DEVICE_DETECT,			STATE_CLOCK_SETTING_MODE_EVENT_USB_B_DEVICE_DETECT);
	RegisterEventCb(EVENT_USB_B_DEVICE_REMOVED,			STATE_CLOCK_SETTING_MODE_EVENT_USB_B_DEVICE_DETECT);
#endif

#if (TVOUT_ENABLE)
	RegisterEventCb(EVENT_TV_DETECT,					STATE_CLOCK_SETTING_MODE_EVENT_TV_DETECT);
	RegisterEventCb(EVENT_TV_REMOVED, 					STATE_CLOCK_SETTING_MODE_EVENT_TV_DETECT);
#endif
#if (HDMI_ENABLE)
	RegisterEventCb(EVENT_HDMI_DETECT,					STATE_CLOCK_SETTING_MODE_EVENT_HDMI_DETECT);
	RegisterEventCb(EVENT_HDMI_REMOVED, 				STATE_CLOCK_SETTING_MODE_EVENT_HDMI_DETECT);
#endif

#if (HDMI_ENABLE)
    if (AHC_IsHdmiConnect()){
        ahcRet = StateModeSetOperation(UI_CLOCK_SETTING_STATE, MenuStateClockSettingsModeInitHDMI, MenuStateClockSettingsModeShutDown, MenuStateClockSettingsModeHandler);
    }else
#endif
#if (TVOUT_ENABLE)
    if (AHC_IsTVConnectEx()){
        ahcRet = StateModeSetOperation(UI_CLOCK_SETTING_STATE, MenuStateClockSettingsModeInitTV, MenuStateClockSettingsModeShutDown, MenuStateClockSettingsModeHandler);
    }else
#endif
    {
        ahcRet = StateModeSetOperation(UI_CLOCK_SETTING_STATE, MenuStateClockSettingsModeInitLCD, MenuStateClockSettingsModeShutDown, MenuStateClockSettingsModeHandler);
    }

    return ahcRet;
}
#endif


