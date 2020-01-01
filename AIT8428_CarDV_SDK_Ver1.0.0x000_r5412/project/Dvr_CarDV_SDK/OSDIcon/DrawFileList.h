//==============================================================================
//
//  File        : DrawFileList.h
//  Description : INCLUDE File for the Draw File.
//                Remove unused icon files could reduce the file hundreds of KB
//                and could reduce link memory requirement and link time.
//                Include your icon only when you need it.
//  Author      :
//  Revision    : 1.0
//
//==============================================================================

#ifndef _DRAW_FILE_LIST_H_
#define _DRAW_FILE_LIST_H_

#include "Customer_Config.h"

#if (MENU_MOVIE_SIZE_EN==1 || OSD_SHOW_MOVIE_SIZE==1)
	#if defined(FONT_LARGE)

    #if MENU_MOVIE_SIZE_1440_30P_EN
	#include "Icon_MovieSize_SHD_30PL.c"
	#endif
	#if MENU_MOVIE_SIZE_SHD_30P_EN
	#include "Icon_MovieSize_SHD_30PL.c"
	#endif
	#if MENU_MOVIE_SIZE_SHD_25P_EN
	#include "Icon_MovieSize_SHD_25P.c"
	#endif
	#if MENU_MOVIE_SIZE_SHD_24P_EN
	#include "Icon_MovieSize_SHD_24P.c"
	#endif
	#if MENU_MOVIE_SIZE_1080_60P_EN
	#include "Icon_MovieSize_1080P_60Large.c"
	#endif
    #if MENU_MOVIE_SIZE_1080_24P_EN
    #include "Icon_MovieSize_1080P_24Large.c"
    #endif
	#if MENU_MOVIE_SIZE_1080P_EN
	#include "Icon_MovieSize_1080PLarge.c"
	#endif
	#if MENU_MOVIE_SIZE_720P_EN
	#include "Icon_MovieSize_720PLarge.c"
	#endif
    #if MENU_MOVIE_SIZE_720_24P_EN
    #include "Icon_MovieSize_720_24PLarge.c"
    #endif
    #if MENU_MOVIE_SIZE_720_60P_EN
	#include "Icon_MovieSize_720_60PLarge.c"
	#endif
	#if MENU_MOVIE_SIZE_VGA30P_EN
	#include "Icon_MovieSize_VGA_30PLarge.c"
	#endif
	#if MENU_MOVIE_SIZE_VGA120P_EN
	#include "Icon_MovieSize_VGA_120P.c"//TBD
	#endif

	#else

    #if MENU_MOVIE_SIZE_1440_30P_EN
	#include "Icon_MovieSize_SHD_30P.c"
	#endif
	#if MENU_MOVIE_SIZE_SHD_30P_EN
	#include "Icon_MovieSize_SHD_30P.c"
	#endif
	#if MENU_MOVIE_SIZE_SHD_25P_EN
	#include "Icon_MovieSize_SHD_25P.c"
	#endif
	#if MENU_MOVIE_SIZE_SHD_24P_EN
	#include "Icon_MovieSize_SHD_24P.c"
	#endif
	#if MENU_MOVIE_SIZE_1080_60P_EN
	#include "Icon_MovieSize_1080P_60.c"
	#endif
    #if MENU_MOVIE_SIZE_1080_24P_EN
    #include "Icon_MovieSize_1080_24P.c"
    #endif
	#if MENU_MOVIE_SIZE_1080P_EN
	#include "Icon_MovieSize_1080P.c"
	#endif
	#if MENU_MOVIE_SIZE_720P_EN
	#include "Icon_MovieSize_720P.c"
	#endif
    #if MENU_MOVIE_SIZE_720_24P_EN
    #include "Icon_MovieSize_720_24P.c"
    #endif
	#if MENU_MOVIE_SIZE_720_60P_EN
	#include "Icon_MovieSize_720_60P.c"
	#endif
	#if MENU_MOVIE_SIZE_VGA30P_EN
	#include "Icon_MovieSize_VGA_30P.c"
	#endif
	#if MENU_MOVIE_SIZE_VGA120P_EN
	#include "Icon_MovieSize_VGA_120P.c"
	#endif

	#endif

#endif

#if (MENU_STILL_SIZE_EN==1 || OSD_SHOW_IMAGE_SIZE==1)

	#if defined(FONT_LARGE)
	#if MENU_STILL_SIZE_14M_EN
	#include "Icon_ImageSize_14MLarge.c"
	#endif
	#if MENU_STILL_SIZE_12M_EN
	#include "Icon_ImageSize_12MLarge.c"
    #endif
    #if MENU_STILL_SIZE_8M_EN
	#include "Icon_ImageSize_8MLarge.c"
    #endif
    #if MENU_STILL_SIZE_5M_EN
	#include "Icon_ImageSize_5MLarge.c"
    #endif
    #if MENU_STILL_SIZE_3M_EN
	#include "Icon_ImageSize_3MLarge.c"
    #endif
    #if MENU_STILL_SIZE_2M_WIDE_EN
	#include "Icon_ImageSize_2MLarge.c"
    #endif
    #if MENU_STILL_SIZE_1d2M_EN
	#include "Icon_ImageSize_1_2MLarge.c"
    #endif
    #if MENU_STILL_SIZE_VGA_EN
	#include "Icon_ImageSize_VGALarge.c"
	#endif
	#else
    #if MENU_STILL_SIZE_30M_EN
	#include "Icon_ImageSize_30M.c"
    #endif
    #if MENU_STILL_SIZE_14M_EN
	#include "Icon_ImageSize_14M.c"
    #endif
    #if MENU_STILL_SIZE_12M_EN
	#include "Icon_ImageSize_12M.c"
    #endif
    #if MENU_STILL_SIZE_8M_EN
	#include "Icon_ImageSize_8M.c"
    #endif
    #if MENU_STILL_SIZE_5M_EN
	#include "Icon_ImageSize_5M.c"
    #endif
    #if MENU_STILL_SIZE_3M_EN
	#include "Icon_ImageSize_3M.c"
    #endif
    #if MENU_STILL_SIZE_2M_WIDE_EN
	#include "Icon_ImageSize_2M.c"
    #endif
    #if MENU_STILL_SIZE_1d2M_EN
	#include "Icon_ImageSize_1_2M.c"
    #endif
    #if MENU_STILL_SIZE_VGA_EN
	#include "Icon_ImageSize_VGA.c"
	#endif
	#endif

#endif

#if (MENU_STILL_FLASH_EN==1 || OSD_SHOW_FLASH_STATUS==1)
#if MENU_STILL_FLASH_ON_EN
#include "Icon_Flash_ON.c"
#endif
#if MENU_STILL_FLASH_OFF_EN
#include "Icon_Flash_OFF.c"
#endif
#if MENU_STILL_FLASH_AUTO_EN || OSD_SHOW_FLASH_STATUS
#include "Icon_Flash_Auto.c"
#endif
#endif

#if (MENU_STILL_SELF_TIMER_EN==1 || OSD_SHOW_SELF_TIMER==1)
#include "Icon_Selftimer_2S.c"
#include "Icon_Selftimer_10S.c"
//#include "Icon_Selftimer.c"
#endif

#include "Icon_Num_0.c"
#include "Icon_Num_1.c"
#include "Icon_Num_2.c"
#include "Icon_Num_3.c"
#include "Icon_Num_4.c"
#include "Icon_Num_5.c"
#include "Icon_Num_6.c"
#include "Icon_Num_7.c"
#include "Icon_Num_8.c"
#include "Icon_Num_9.c"

#if (MENU_STILL_BURST_SHOT_EN==1 || OSD_SHOW_BURST_SHOT==1)
#if MENU_STILL_BURST_SHOT_HI_EN
#include "Icon_Burst_High.c"
#endif
#if MENU_STILL_BURST_SHOT_LO_EN
#include "Icon_Burst_Low.c"
#endif
#if MENU_STILL_BURST_SHOT_MID_EN
#include "Icon_Burst_Middle.c"
#endif
#endif

#if (MENU_STILL_STABILIZE_EN==1 || OSD_SHOW_IMAGE_STABLIZATION==1)
#if MENU_STILL_STABILIZE_OFF_EN
#include "Icon_ImageStable_OFF.c"
#endif
#if MENU_STILL_STABILIZE_ON_EN
#include "Icon_ImageStable_ON.c"
#endif
#endif

#if (MENU_MANUAL_FOCUS_RANGE_EN==1 || OSD_SHOW_FOCAL_RANGE==1)
#include "Icon_Focus_Infinity.c"
#include "Icon_Focus_Marco.c"
#include "Icon_Focus_Window.c"
#endif

#if (MENU_MANUAL_FOCUS_EN==1)
#include "Icon_FocusWindow.c"
#include "Icon_FocusWindow_CentrSpot.c"
#include "Icon_FocusWindow_Multi.c"
#endif

#if (MENU_MANUAL_METERING_EN==1)
#include "Icon_Metering_CenterSpot.c"
#include "Icon_Metering_Multi.c"
#include "Icon_Metering_Weighted.c"
#endif

#if (MENU_MANUAL_SCENE_EN==1 || OSD_SHOW_SCENE_MODE==1)
#if MENU_MANUAL_SCENE_AUTO_EN
#include "Icon_Scene_Auto.c"
#endif
#if MENU_MANUAL_SCENE_BEACH_EN
#include "Icon_Scene_Beach.c"
#endif
#if MENU_MANUAL_SCENE_FIREWORKS_EN
#include "Icon_Scene_Fireworks.c"
#endif
#if MENU_MANUAL_SCENE_LANDSCAPE_EN
#include "Icon_Scene_Landscape.c"
#endif
#if MENU_MANUAL_SCENE_PORTRAIT_EN
#include "Icon_Scene_Portrait.c"
#endif
#if MENU_MANUAL_SCENE_SNOW_EN
#include "Icon_Scene_Snow.c"
#endif
#if MENU_MANUAL_SCENE_SPORT_EN
#include "Icon_Scene_Sport.c"
#endif
#ifdef CFG_MENU_ICON_DEV_TWLIGHT //may be defined in config_xxx.h. It is recommanded to put right after MENU_MANUAL_SCENE_TWILIGHT_EN
#if MENU_MANUAL_SCENE_TWILIGHT_EN
#include "Icon_Scene_Twilight_DEV.c"
#endif
#else
#if MENU_MANUAL_SCENE_TWILIGHT_EN
#include "Icon_Scene_Twilight.c"
#endif
#endif
#if MENU_MANUAL_SCENE_TWILIGHT_PORTRAIT_EN
#include "Icon_Scene_Twilight_Portrait.c"
#endif
#endif

#if (MENU_MANUAL_WB_EN==1 || OSD_SHOW_WB_MODE==1)
#if MENU_MANUAL_WB_CLOUDY_EN
#include "Icon_WB_Cloudy.c"
#endif
#if MENU_MANUAL_WB_DAYLIGHT_EN
#include "Icon_WB_Daylight.c"
#endif
#if MENU_MANUAL_WB_FLASH_EN
#include "Icon_WB_Flash.c"
#endif
#if MENU_MANUAL_WB_FLUORESCENT1_EN
#include "Icon_WB_Fluorescent1.c"
#endif
#if MENU_MANUAL_WB_FLUORESCENT2_EN
#include "Icon_WB_Fluorescent2.c"
#endif
#if MENU_MANUAL_WB_FLUORESCENT3_EN
#include "Icon_WB_Fluorescent3.c"
#endif
#if MENU_MANUAL_WB_INCANDESCENT_EN
#include "Icon_WB_Incandescent.c"
#endif
#if MENU_MANUAL_WB_ONEPUSH_EN
#include "Icon_WB_One_Push.c"
#endif
#if MENU_MANUAL_WB_ONE_PUSH_SET_EN
#include "Icon_WB_One_Push_Set.c"
#endif
#endif

#if (MENU_PLAYBACK_ROTATE_EN==1)
#include "Icon_Rotate_Left.c"
#include "Icon_Rotate_Right.c"
#endif

#if (OSD_SHOW_BATTERY_STATUS==1)
	#if defined(FONT_LARGE)
	#include "Icon_Battery_ExhaustLarge.c"
	#include "Icon_Battery_EmptyLarge.c"
	#include "Icon_Battery_FullLarge.c"
	#include "Icon_Battery_MidLarge.c"
	#include "Icon_Battery_NormalLarge.c"
	//#include "Icon_Battery_Low.c"
	#else
	#include "Icon_Battery_Exhaust.c"
	#include "Icon_Battery_Empty.c"
	#include "Icon_Battery_Full.c"
	#include "Icon_Battery_Mid.c"
	#include "Icon_Battery_Normal.c"
	//#include "Icon_Battery_Low.c"
	#endif

#endif

#if (OSD_SHOW_MEDIA_TYPE==1)

	#if defined(FONT_LARGE)
	#include "Icon_SD_CardLarge.c"
	#include "Icon_SD_Card_NoLarge.c"
	#include "Icon_SD_CardSlowLarge.c"
	#include "Icon_SDCard_ErrorLarge.c"
	#else
	#include "Icon_SD_Card.c"
	#include "Icon_SD_Card_No.c"
	#include "Icon_SD_CardSlow.c"
	#include "Icon_SDCard_Error.c"
	#endif

#endif

/////////////////////////////////////////////////////////////////
/* Menu Catalog Icon */
//#include "Icon_Menu_Edit.c"
//#include "Icon_Menu_Edit_HL.c"
#include "Icon_Menu_General_Settings.c"
#include "Icon_Menu_General_SettingsHL.c"
//#include "Icon_Menu_Manul_Settings.c"
//#include "Icon_Menu_Manul_Settings_HL.c"
#include "Icon_Menu_Media.c"
#include "Icon_Menu_Media_HL.c"
#include "Icon_Menu_Movie.c"
#include "Icon_Menu_Movie_HL.c"
#include "Icon_Menu_Playback.c"
#include "Icon_Menu_Playback_HL.c"
#if DSC_MODE_ENABLE
#include "Icon_Menu_Still.c"
#include "Icon_Menu_Still_HL.c"
#endif

/* Menu Bar Icon */
#if defined(FONT_LARGE)
#include "Icon_MenuBar_WhiteXLarge.c"
#include "Icon_MenuBar_YellowXLarge.c"
#include "Icon_MenuBar_WhiteLarge.c"
#include "Icon_MenuBar_YellowLarge.c"
#include "Icon_MenuBar_White_DefaultLarge.c"
#include "Icon_MenuBar_Yellow_DefaultLarge.c"
#include "Icon_MenuBar_WhiteSmall.c"
#include "Icon_MenuBar_YellowSmall.c"
#else
#include "Icon_MenuBar_White.c"
#include "Icon_MenuBar_Yellow.c"
#include "Icon_MenuBar_White_Default.c"
#include "Icon_MenuBar_Yellow_Default.c"
#include "Icon_SubMenuBar_White.c"
#include "Icon_SubMenuBar_White_Default.c"
#include "Icon_SubMenuBar_Yellow.c"
#include "Icon_SubMenuBar_Yellow_Default.c"
#endif

#if defined(FONT_LARGE)
#include "Icon_MenuBar_White_Big.c"
#include "Icon_MenuBar_Yellow_Big.c"
#include "Icon_MenuBar_White_Default_Big.c"
#include "Icon_MenuBar_Yellow_Default_Big.c"
#endif

#if (USB_MODE_SELECT_EN)
#include "Icon_MSDC.c"
#include "Icon_PCCAM.c"
#endif

/* Menu Misc Icon */
#include "Icon_OK.c"
#if (POWER_ON_MENU_SET_EN) || (EXIT_MENU_PAGE_EN)
#include "Icon_Exit.c"
#endif

#include "Icon_D_Down.c"
#include "Icon_D_Left.c"
#include "Icon_D_Right.c"
#include "Icon_D_Up.c"

/* HDMI/TVout Icon */
//#include "Icon_HdmiStillPlay.c"
#include "Icon_HdmiVideoPlay.c"
#include "Icon_HdmiVideoPlayPause.c"
#include "Icon_HdmiVideoPlayStop.c"
#include "Icon_HdmiVideoPlay_FF.c"
#include "Icon_HdmiVideoPlay_FB.c"
#include "Icon_HdmiVideoPlay_PFF.c"
#include "Icon_HdmiVideoPlay_PFB.c"
#include "Icon_HdmiVideoPlay_Play.c"
#include "Icon_HdmiVideoPlay_Pause.c"
#include "Icon_HdmiVideoPlay_Stop.c"
//#include "Icon_HdmiSlideShow.c"

/* Warning/Wait Message Icon */
#include "Icon_WMSG.c"
#include "Icon_Wait0.c"
#include "Icon_Wait1.c"
#include "Icon_Wait2.c"
#include "Icon_Wait3.c"
#include "Icon_Wait4.c"
//#include "Icon_Info.c"

/* Video Record Icon */
#include "Icon_VRPause.c"

#include "Icon_ProtectKey.c"
#include "Icon_ProtectKey_90.c"

#ifdef CFG_MENU_ICON_RED_PROTECT //may be defined in config_xxx.h.
#include "Icon_ProtectKeyRed.c"
#endif

//#include "Icon_mutemic.c"
#include "Icon_mutemicW.c"
#if (MOTION_DETECTION_EN)
#include "Icon_Motion_Detection.c"
#endif

/* UI Mode Icon */
#if defined(FONT_LARGE)
#include "Icon_Button_Movie.c"
#include "Icon_Button_Still.c"
#include "Icon_MovieLarge.c"
#include "Icon_Movie_Playback.c"
#include "Icon_StillLarge.c"
#include "Icon_Still_Playback.c"
#else
#include "Icon_Button_Movie.c"
#include "Icon_Button_Still.c"
#include "Icon_Button_Movie_Playback.c"
//#include "Icon_Button_Still_Playback.c"
#include "Icon_Movie.c"
#include "Icon_Movie_Playback.c"
#include "Icon_Still.c"
#include "Icon_Still_Playback.c"
#endif

/* Other Icon */
#include "Icon_Volume_Decrease.c"
#include "Icon_Volume_Increase.c"

#include "Icon_USB_Connect.c"
#ifdef CFG_MENU_ICON_USB_CONNECT_2 //may be defined in config_xxx.h.
#include "Icon_USB_Connect_2.c"
#endif
#if (SUPPORT_GPS)
#include "Icon_GPSSigs.c"
#endif
#include "Icon_Music.c"
#ifdef CFG_MENU_ICON_REC //may be defined in config_xxx.h.
#include "Icon_REC.c"
#endif

#if (ENABLE_ADAS_LDWS)
#include "Icon_LDWS_Right.c"
#include "Icon_LDWS_Left.c"
#endif

#if (SUPPORT_TOUCH_PANEL)//Un-Used
#include "Icon_Button_Zoom_In.c"
//#include "Icon_Button_Zoom_In_Touch.c"
//#include "Icon_Button_Zoom_Out.c"
//#include "Icon_Button_Zoom_Out_Touch.c"
//#include "Icon_Zoom_In.c"
//#include "Icon_Zoom_Out.c"
//#include "Icon_ZoomBar.c"

#if (MENU_MEDIA_SELECT_MDEIA_EN)
#include "Icon_InternalMem.c"
#endif
//#include "Icon_InternalMem_Error.c"

//#include "Icon_ProtectKeyD.c"


//#include "Icon_VR_Volume_On.c"
//#include "Icon_VR_Volume_Off.c"

//#include "Icon_Button_Record.c"
//#include "Icon_Button_Record_Touch.c"
//#include "Icon_Button_Capture.c"
//#include "Icon_Button_Capture_Touch.c"

//#include "Icon_Slide_Show.c"
//#include "Icon_SlideShow_Next.c"
//#include "Icon_Slideshow_Previous.c"

//#include "Icon_Right.c"
#include "Icon_Menu.c"

#include "Icon_Previous.c"
#include "Icon_Next.c"
//#include "Icon_Pause.c"
//#include "Icon_Play.c"
#include "Icon_Play_Pause.c"
#include "Icon_Play_Play.c"
#include "Icon_Return.c"

//#include "Icon_Protect.c"
//#include "Icon_Delete.c"
//#include "Icon_Cut.c"
//#include "Icon_Button_Cut.c"

#include "Icon_Movie_Fast_Forward.c"
#include "Icon_Movie_Fast_Reverse.c"

#endif

#include "Icon_Dummy.c"

#if defined(WIFI_PORT) && (WIFI_PORT == 1)
// WiFi Icons
#include "Icon_WiFi.c"		// AP Connected
//#include "Icon_WiFi00.c"	// AP Disconnected
#include "Icon_WiFi01.c"	// rtsp running
#include "Icon_WiFi02.c"	// rtsp running
#include "Icon_WiFi03.c"	// rtsp running
#include "Icon_WiFi_ERR.c"	// rtsp ERROR
#include "Icon_WiFi_FAIL.c"	// init WIFI failed

#include "Icon_WiFi_On.c"		// AP Connected
#include "Icon_WiFi_IDLE.c"		// AP Disconnected
//#include "Icon_WiFi_Setting.c"	// rtsp running
#include "Icon_WiFi_Streaming_On.c"	// rtsp running
#include "Icon_WiFi_Streaming_Off.c"	// rtsp running
#include "Icon_WiFi_Disconnect.c"
#include "Icon_WiFiNoIP.c"	// No IP or Crypto type(key) error
#endif

#if (defined(VIDEO_REC_TIMELAPSE_EN) && VIDEO_REC_TIMELAPSE_EN)
#include "TV_TimeLapseB.c"
#endif

#if (MENU_MOVIE_HDR_MODE_EN)
#include "Icon_HDR.c"
//#include "Icon_HDR_OFF.c"
#endif

#include "Icon_event_bt_normal.c"  
#include "Icon_event_bt_push.c"  
#include "Icon_format_button_normal.c"  
#include "Icon_format_button_push.c"  

#include "Icon_sound_button_normal.c"  
#include "Icon_sound_button_push.c"  
#include "Icon_system_button_normal.c"  
#include "Icon_system_button_push.c" 

#include "Icon_time_button_normal.c"  
#include "Icon_time_button_push.c"  

#include "Icon_touch_button_normal.c"  
#include "Icon_touch_button_push.c"  
#include "Icon_volt_bt_normal.c"  
#include "Icon_volt_bt_push.c" 

//dxp 0413 имик?ио volt ик?б└и║
#include "12volt01_bt_nor.c"  
#include "12volt01_bt_sel.c"  
#include "12volt02_bt_nor.c"  
#include "12volt02_bt_sel.c"  

#include "12volt03_bt_nor.c"  
#include "12volt03_bt_sel.c"  
#include "12volt04_bt_nor.c"  
#include "12volt04_bt_sel.c" 

#include "12volt05_bt_nor.c"  
#include "12volt05_bt_sel.c"  

#include "24volt01_bt_nor.c"  
#include "24volt01_bt_sel.c"  
#include "24volt02_bt_nor.c"  
#include "24volt02_bt_sel.c" 
#include "24volt03_bt_nor.c"  
#include "24volt03_bt_sel.c"  

#include "24volt04_bt_nor.c"  
#include "24volt04_bt_sel.c"  
#include "24volt05_bt_nor.c"  
#include "24volt05_bt_sel.c" 
#include "9volt01_bt_nor.c"  
#include "9volt01_bt_sel.c"  

#include "9volt02_bt_nor.c"  
#include "9volt02_bt_sel.c"  
#include "9volt03_bt_nor.c"  
#include "9volt03_bt_sel.c" 

#include "9volt05_bt_nor.c"  
#include "9volt05_bt_sel.c" 
#include "9volt04_bt_nor.c"  
#include "9volt04_bt_sel.c"  

#include "volt_title02.c" 

#include "no1_bt_nor.c"  
#include "no1_bt_sel.c"  

#include "touch_sound.c"  
#include "vol0_bt_nor.c"
#include "vol0_bt_sel.c" 

#include "vol1_bt_nor.c"
#include "vol1_bt_sel.c" 
#include "vol2_bt_nor.c"
#include "vol2_bt_sel.c" 

#include "vol3_bt_nor.c"
#include "vol3_bt_sel.c" 

#include "yes1_bt_nor.c"  
#include "yes1_bt_sel.c" 


//********************************

#include "black_back.c"

#include "DamageVideoFile.c"
#include "reverse_line.c"
#include "Icon_menu_button.c"
#include "Icon_pip_button_normal.c"
#include "Icon_take_video.c"  

#include "Icon_cam_normal.c"
#include "Icon_play_normal.c"
//#include "Icon_hd2_button.c"
#include "Icon_setup_normal.c"  
#include "Icon_system_normal.c"

#include "Icon_cam_push.c"
#include "Icon_play_push.c"
#include "Icon_setup_push.c"  
#include "Icon_system_push.c"

#include "Icon_lcd_button_normal.c"
#include "Icon_lcd_button_push.c"
#include "Icon_start_screen_button_normal.c"  
#include "Icon_start_screen_button_push.c"

#include "Icon_sensor_button_normal.c"
#include "Icon_sensor_button_push.c"
#include "Icon_mic_button_normal.c"  
#include "Icon_mic_button_push.c"

#include "Icon_reset_button_normal.c"
#include "Icon_reset_button_push.c"
#include "Icon_gps_button_normal.c"  
#include "Icon_gps_button_push.c"

#include "Icon_home_button3_normal.c"
#include "Icon_home_button4_normal.c"

#include "Icon_1min_button_normal.c"
#include "Icon_3min_button_normal.c"
#include "Icon_clock_button_normal.c"
#include "Icon_full_time_button_normal.c"

#include "Icon_1min_button_push.c"
#include "Icon_3min_button_push.c"
#include "Icon_clock_button_push.c"
#include "Icon_full_time_button_push.c"

#include "Icon_1_bt_normal.c"
#include "Icon_1_bt_sel.c"
#include "Icon_2_bt_normal.c"
#include "Icon_2_bt_sel.c"
#include "Icon_3_bt_normal.c"
#include "Icon_3_bt_sel.c"

#include "Icon_1_bt_normal_1080.c"
#include "Icon_1_bt_sel_1080.c"
#include "Icon_2_bt_normal_1080.c"
#include "Icon_2_bt_sel_1080.c"
#include "Icon_3_bt_normal_1080.c"
#include "Icon_3_bt_sel_1080.c"

#include "Icon_no_bt_nor.c"
#include "Icon_no_bt_sel.c"
#include "Icon_yes_bt_nor.c"
#include "Icon_yes_bt_sel.c"

#include "Icon_sensor_level1_button_normal.c"
#include "Icon_sensor_level1_button_push.c"
#include "Icon_sensor_level2_button_normal.c"
#include "Icon_sensor_level2_button_push.c"
#include "Icon_sensor_level3_button_normal.c"
#include "Icon_sensor_level3_button_push.c"

#include "Icon_setup_bg_bar.c"
#include "Icon_home_button_normal.c"

#include "Icon_lcd_title.c"
#include "Icon_sensor_title.c"
#include "Icon_logo_title.c"
#include "Icon_mic_title.c"
#include "Icon_reset_title.c"
#include "Icon_gps_title.c"

#include "Icon_mic_reco_off.c"
#include "Icon_mic_reco_on.c"
#include "Icon_mic_info_off.c"
#include "Icon_mic_info_on.c"

#include "event_bg.c"
#include "format02_bg.c"
#include "system_bg.c"
#include "time_bg.c"
#include "format_bg.c"
#include "touch_bg.c"
#include "volt_bg2.c"
#include "time_0.c"
#include "time_0r.c"
#include "time_1.c"
#include "time_1r.c"
#include "time_2.c"
#include "time_2r.c"
#include "time_3.c"
#include "time_3r.c"

#include "time_4.c"
#include "time_4r.c"
#include "time_5.c"
#include "time_5r.c"
#include "time_6.c"
#include "time_6r.c"
#include "time_7.c"
#include "time_7r.c"
#include "time_8.c"
#include "time_8r.c"
#include "time_9.c"
#include "time_9r.c"
#include "parking_rec_button_normal.c"
#include "rec_button_normal.c"
#include "parking_rec_button_push.c"
#include "rec_button_push.c"
#include "event_rec_button_normal.c"
#include "event_rec_button_push.c"
#include "Icon_hd_button.c"
#include "Icon_hd2_button.c"

#include "Icon_front_cam_normal.c"
#include "Icon_back_cam_normal.c"
#include "Icon_front_back_cam_normal.c"

#include "Icon_no_mute.c"
#include "Icon_mute.c"
#include "Icon_stop.c"

#include "1ch_button_off.c"
#include "1ch_button_on.c"
#include "2ch_button_off.c"
#include "2ch_button_on.c"

#include "down2_button_normal.c"
#include "down2_button_push.c"
#include "enter_button_normal.c"
#include "enter_button_push.c"
#include "up2_button_normal.c"
#include "up2_button_push.c"

#include "back1_button_normal.c"
#include "back1_button_push.c"
#include "del_button_normal.c"
#include "del_button_push.c"

#include "home_button2_normal.c"
#include "home_button2_push.c"
#include "next1_button_normal.c"
#include "next1_button_push.c"
#include "play_button2_normal.c"
#include "play_button2_push.c"
#include "play_button_normal.c"
#include "play_button_push.c"

#include "Icon_play_title.c"
#include "Icon_1ch_button_off.c"
#include "Icon_1ch_button_on.c"
#include "Icon_2ch_button_off.c"
#include "Icon_2ch_button_on.c"

#include "Icon_voice_title.c"
#include "Icon_vol_title02.c"
#include "Icon_volt_title01.c"

#include "Icon_7day_button_normal.c"
#include "Icon_15day_button_normal.c"
#include "Icon_30day_button_normal.c"
#include "Icon_7day_button_push.c"
#include "Icon_15day_button_push.c"
#include "Icon_30day_button_push.c"
#include "Icon_format_sdcard_push.c"
#include "Icon_format_sdcard_normal.c"
#include "Icon_Format_title1.c"
#include "Icon_Format_title2.c"
#include "Icon_format03_title.c"
#include "SDCARD.c"
#include "warning_sd.c"
#include "rec_resolution.c"
#include "gsensor_rec.c"
#include "parking_rec.c"
#include "sdcarderr.c"
#include "readsdcard.c"
#include "warning_power_off.c"

#include "num_1.c"
#include "num_2.c"
#include "num_3.c"
#include "num_4.c"
#include "num_5.c"
#include "num_6.c"
#include "num_7.c"
#include "num_8.c"
#include "num_9.c"
#include "num_0.c"

#include "Icon_1.c"
#include "Icon_2.c"
#include "Icon_3.c"
#include "Icon_4.c"
#include "Icon_5.c"
#include "Icon_6.c"
#include "Icon_7.c"
#include "Icon_8.c"
#include "Icon_9.c"
#include "Icon_h.c"
#include "Icon_pm.c"
#include "Icon_0.c"
#include "Icon_Reset_Ok.c"
#include "Icon_am.c"

#include "Icon_hd2_bt_nor.c"
#include "Icon_hd__bt_nor.c"
#include "Icon_hd_bg_title.c"
#include "Icon_hd_button_normal.c"
#include "Icon_hd_button_push.c"
#include "Icon_format_ok_2.c"
#include "del_button_null.c"
#include "Icon_rec_button_push.c"
#include "Icon_front_cam_push.c"
#include "Icon_back_cam_push.c"
#include "welogo.c"
#include "welogo1.c"
#include "welogo2.c"
#include "welogo_1080.c"
#include "welogo1_1080.c"
#include "welogo2_1080.c"
#include "motion_parking_rec.c"
#include "bestlight.c"
#include "comman.c"
#include "morelihgt.c"
#include "selectBar.c"
#include "m1.c"
#include "m2.c"
#include "m3.c"
#include "sm1.c"
#include "sm2.c"
#include "sm3.c"
#include "night1_on.c"
#include "night2_off.c"
#include "night3_parking.c"
#include "night1_son.c"
#include "night2_soff.c"
#include "night3_sparking.c"
#include "Parkingmode.c"
#include "parkingpoweroff.c"
#include "parkingselect1.c"
#include "parkingselect2.c"
#include "parkingselect3.c"
#include "longselect.c"
#include "batterysetting.c"
#include "voltsetting.c"
#include "Powerbatterys.c"
#include "Powerbatterys2.c"
#include "batterysetting2.c"
#include "tenhour.c"
#include "twentyfour.c"
#include "fivehour.c"
#include "fortyeight.c"
#include "stenhour.c"
#include "stwentyfour.c"
#include "sfivehour.c"
#include "sfortyeight.c"
#endif //_DRAW_FILE_LIST_H_
