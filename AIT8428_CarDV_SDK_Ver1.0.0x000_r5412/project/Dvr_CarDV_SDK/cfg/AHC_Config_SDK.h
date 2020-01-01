/**
 @file AHC_Config_SDK.h
 @brief Header File for the AHC configuration.
 @author 
 @version 1.0
*/

#ifndef _AHC_CONFIG_SDK_H_
#define _AHC_CONFIG_SDK_H_

/*===========================================================================
 * Include files
 *===========================================================================*/
#include "config_fw.h"
#include "Customer_config.h"
#include "AHC_OS.h"

/*===========================================================================
 * Marco define
 *===========================================================================*/
//UI DRAM address++
#if defined (MCR_V2_32MB)      // defined in MCP
    #undef AHC_DRAM_SIZE
    #define AHC_DRAM_SIZE                           (COMMON_DRAM_SIZE_32MB)
#elif defined (MCR_V2_Q_128MB) // defined in MCP
    #undef AHC_DRAM_SIZE
    #define AHC_DRAM_SIZE                           (COMMON_DRAM_SIZE_128MB)
    //if use 8x28Q 8x28G, change to COMMON_DRAM_SIZE_128MB
#else
    #undef AHC_DRAM_SIZE   
    #define AHC_DRAM_SIZE                           (COMMON_DRAM_SIZE_64MB) //Default DRAM setting    
#endif

//MIMF define 
#define MIMF_CUSTOMIZED_SORT_NONE               (0)
#define MIMF_CUSTOMIZED_SORT_DECREASE           (1)
#define MIMF_CUSTOMIZED_SORT_FILETYPE           (2)
#define MIMF_CUSTOMIZED_SORT_MULTI_DB_DECREASE  (3)
#define MIMF_CUSTOMIZED_SORT_TYPE               (MIMF_CUSTOMIZED_SORT_NONE)

#if(MIMF_CUSTOMIZED_SORT_TYPE == MIMF_CUSTOMIZED_SORT_MULTI_DB_DECREASE)
#define MIMF_MULTI_DB_DECREASE_SELECT_DB        (0x01 << DCF_DB_TYPE_1ST_DB | 0x01 << DCF_DB_TYPE_3RD_DB)
#endif

#define MIMF_CUSTOMIZED_SORT_MOV_ALL       (1)
#define MIMF_CUSTOMIZED_SORT_JPG_ALL       (1)
//memory config
#define MALLOC_OSDBUFF_DOWN_GROWTH			(1)

#if (AHC_DRAM_SIZE == COMMON_DRAM_SIZE_64MB && MIMF_CUSTOMIZED_SORT_TYPE != MIMF_CUSTOMIZED_SORT_MULTI_DB_DECREASE)

    //64MB version
    #define AHC_THUMB_TEMP_BASE_ADDR                (0x2700000)
    #define AHC_THUMB_BUF_NUM                       (2) //1 or 2
    #define AHC_DCF_TEMP_BASE_ADDR                  (0x4D80000)
    #define AHC_MIMF_TEMP_BASE_ADDR                 (0x4E00000)

    // VIDEO_DIGIT_ZOOM_EN and HDR, change it to 0x04600000
    #if ((AHC_EMERGENTRECD_SUPPORT == 1) || VIDEO_DIGIT_ZOOM_EN)
    #define AHC_GUI_TEMP_BASE_ADDR                  (0x4A80000)	//(==MIMF_CUSTOMIZED_SORT_NONE)tvout mode,	Buffer will overlap (OSD and DCF)
    #else
    #define AHC_GUI_TEMP_BASE_ADDR                  (0x4A80000) // 
    #endif

    #define AHC_GUI_TEMP_BASE_ADDR_HDMI             (0x4000000) // Andy Liu.  It causes system lock up if it is in 0x3C00000.
    #define AHC_GUI_TEMP_BASE_ADDR_HDMI_PHOTO_PB    (0x4000000)
    #define AHC_GUI_STRUCT_MEMORY_SIZE              (0x80000)
    #define AHC_SOUNDFILE_DRAM_ADDR                 (0x0000000) //0: SOUNDFILE doesn't at DRAM

    #define AHC_MENU_SETTING_TEMP_BUFFER_SIZE       (8192)      // front of MMPS_System_GetPreviewFrameStart

#elif (AHC_DRAM_SIZE == COMMON_DRAM_SIZE_32MB)

    //32MB version
    #define AHC_THUMB_TEMP_BASE_ADDR                (0x2370000)
    #define AHC_THUMB_BUF_NUM                       (1) //1 or 2 
    #define AHC_DCF_TEMP_BASE_ADDR                  (0x2E30000)
    #define AHC_MIMF_TEMP_BASE_ADDR                 (AHC_DCF_TEMP_BASE_ADDR + 0x80000) 
    #define AHC_GUI_TEMP_BASE_ADDR                  (0x2B60000)       // for 320x240 Panel
    //#define AHC_GUI_TEMP_BASE_ADDR                  (0x2900000)     // for 480x272 Panel
    #define AHC_GUI_TEMP_BASE_ADDR_HDMI             (0x2800000)
    #define AHC_GUI_TEMP_BASE_ADDR_HDMI_PHOTO_PB    (0x2800000)
    #define AHC_GUI_STRUCT_MEMORY_SIZE              (0x80000)
    #define AHC_SOUNDFILE_DRAM_ADDR                 (0x0000000) //0: SOUNDFILE doesn't at DRAM 

    #define AHC_MENU_SETTING_TEMP_BUFFER_SIZE       (8192)      // front of MMPS_System_GetPreviewFrameStart

#elif (AHC_DRAM_SIZE == COMMON_DRAM_SIZE_128MB)

	#if (defined(SUPPORT_SPEECH_RECOG)&&(SUPPORT_SPEECH_RECOG))
	#define AHC_THUMB_TEMP_BASE_ADDR				(0x2700000)
	#define AHC_THUMB_BUF_NUM						(2) //1 or 2
	#define AHC_DCF_TEMP_BASE_ADDR					(0x7D80000)
	#define AHC_MIMF_TEMP_BASE_ADDR 				(0x8E00000)
	#define AHC_SPEECH_BASE_ADDR				 (0x7D80000+0x100000)//(0x7E80000)~(0x8D80000)use to speech

	// VIDEO_DIGIT_ZOOM_EN and HDR, change it to 0x04600000
	#if ((AHC_EMERGENTRECD_SUPPORT == 1) || VIDEO_DIGIT_ZOOM_EN)
	#define AHC_GUI_TEMP_BASE_ADDR					(0x7000000)
	#else
	#define AHC_GUI_TEMP_BASE_ADDR					(0x7000000)
	#endif

	#define AHC_GUI_TEMP_BASE_ADDR_HDMI 			(0x7000000) // Andy Liu.  It causes system lock up if it is in 0x3C00000.
	#define AHC_GUI_TEMP_BASE_ADDR_HDMI_PHOTO_PB	(0x7000000)
    #define AHC_GUI_STRUCT_MEMORY_SIZE              (0x80000)
    #define AHC_SOUNDFILE_DRAM_ADDR                 (0x0000000) //0: SOUNDFILE doesn't at DRAM
	
	#define AHC_MENU_SETTING_TEMP_BUFFER_SIZE       (8192)      // front of MMPS_System_GetPreviewFrameStart
	#else
    //64MB version
    #define AHC_THUMB_TEMP_BASE_ADDR                (0x2700000)
    #define AHC_THUMB_BUF_NUM                       (2) //1 or 2
    #define AHC_DCF_TEMP_BASE_ADDR                  (0x8D80000)
    #define AHC_MIMF_TEMP_BASE_ADDR                 (0x8E00000)

    // VIDEO_DIGIT_ZOOM_EN and HDR, change it to 0x04600000
    #if ((AHC_EMERGENTRECD_SUPPORT == 1) || VIDEO_DIGIT_ZOOM_EN)
    #define AHC_GUI_TEMP_BASE_ADDR                  (0x8000000)
    #else
    #define AHC_GUI_TEMP_BASE_ADDR                  (0x8000000)
    #endif

    #define AHC_GUI_TEMP_BASE_ADDR_HDMI             (0x8000000) // Andy Liu.  It causes system lock up if it is in 0x3C00000.
    #define AHC_GUI_TEMP_BASE_ADDR_HDMI_PHOTO_PB    (0x8000000)
    #define AHC_GUI_STRUCT_MEMORY_SIZE              (0x80000)
    #define AHC_SOUNDFILE_DRAM_ADDR                 (0x0000000) //0: SOUNDFILE doesn't at DRAM

    #define AHC_MENU_SETTING_TEMP_BUFFER_SIZE       (8192)      // front of MMPS_System_GetPreviewFrameStart

#endif

//this memory config is use only for specific project
#elif (AHC_DRAM_SIZE == COMMON_DRAM_SIZE_64MB && MIMF_CUSTOMIZED_SORT_TYPE == MIMF_CUSTOMIZED_SORT_MULTI_DB_DECREASE)
    //64MB version specific project for MIMF_CUSTOMIZED_SORT_MULTI_DB_DECREASE
    #define AHC_THUMB_TEMP_BASE_ADDR                (0x2700000)
    #define AHC_THUMB_BUF_NUM                       (2) //1 or 2
    #define AHC_DCF_TEMP_BASE_ADDR                  (0x4D80000)
    #define AHC_MIMF_TEMP_BASE_ADDR                 (0x4E00000)
    // VIDEO_DIGIT_ZOOM_EN and HDR, change it to 0x04600000
    #if ((AHC_EMERGENTRECD_SUPPORT == 1) || VIDEO_DIGIT_ZOOM_EN)
    #define AHC_GUI_TEMP_BASE_ADDR                  (0x4A80000)
    #else
    #define AHC_GUI_TEMP_BASE_ADDR                  (0x4A80000) // HDR - 0x4600000, else 0x4000000
    #endif

    #define AHC_GUI_TEMP_BASE_ADDR_HDMI             (0x4000000) // Andy Liu.  It causes system lock up if it is in 0x3C00000.
    #define AHC_GUI_TEMP_BASE_ADDR_HDMI_PHOTO_PB    (0x4000000)
    #define AHC_GUI_STRUCT_MEMORY_SIZE              (0x80000)
    #define AHC_SOUNDFILE_DRAM_ADDR                 (0x0000000) //0: SOUNDFILE doesn't at DRAM

	#define AHC_MENU_SETTING_TEMP_BUFFER_SIZE       (8192)      // front of MMPS_System_GetPreviewFrameStart
#else

#error "AHC_DRAM_SIZE NG!!!"

#endif

//UI DRAM address--
//Dynamic Download OSD
#define DDOSD                           (0)

// Use ARGB format ( So far alpha is skipped )
#define DATESTAMP_COLOR_BLACK           (0x00000000)
#define DATESTAMP_COLOR_YELLOW          (0xFFFFFF00)
#define DATESTAMP_COLOR_ORANGE          (0xFFFF8000)
#define DATESTAMP_COLOR_WHITE           (0xFFFFFFFF)

#define PROTECT_FILE_NONE               (0)
#define PROTECT_FILE_RENAME				(1)
#define PROTECT_FILE_MOVE_SUBDB         (2)
#define PROTECT_FILE_MOVE_DB            (3)

#define AHC_BROWSER_THUMBNAIL_STYLE_0   (0)
#define AHC_BROWSER_THUMBNAIL_STYLE_1   (1)

#define AHC_VIDEO_BITRATE_SHD30FPS_SUPPERFINE  	(VR_BITRATE_SHD30P_SUPER)   //24000000
#define AHC_VIDEO_BITRATE_SHD30FPS_FINE        	(VR_BITRATE_SHD30P_FINE)    // 8000000
#define AHC_VIDEO_BITRATE_FHD60FPS_SUPPERFINE  	(VR_BITRATE_FHD60P_SUPER)   //32000000
#define AHC_VIDEO_BITRATE_FHD60FPS_FINE        	(VR_BITRATE_FHD60P_FINE)    //16000000
#define AHC_VIDEO_BITRATE_FHD30FPS_SUPPERFINE  	(VR_BITRATE_FHD30P_SUPER)   //16000000
#define AHC_VIDEO_BITRATE_FHD30FPS_FINE        	(VR_BITRATE_FHD30P_FINE)    // 8000000
#define AHC_VIDEO_BITRATE_HD30FPS_SUPPERFINE    (VR_BITRATE_HD30P_SUPER)    // 7200000
#define AHC_VIDEO_BITRATE_HD30FPS_FINE          (VR_BITRATE_HD30P_FINE)     // 4000000
#define AHC_VIDEO_BITRATE_HD60FPS_SUPPERFINE    (VR_BITRATE_HD60P_SUPER)    //12000000
#define AHC_VIDEO_BITRATE_HD60FPS_FINE          (VR_BITRATE_HD60P_FINE)     // 7200000
#define AHC_VIDEO_BITRATE_VGA30FPS_SUPPERFINE   (VR_BITRATE_VGA30P_SUPER)   //12000000
#define AHC_VIDEO_BITRATE_VGA30FPS_FINE         (VR_BITRATE_VGA30P_FINE)    //10000000
#define AHC_VIDEO_BITRATE_900P30FPS_SUPPERFINE  (VR_BITRATE_900P30FPS_SUPER)
#define AHC_VIDEO_BITRATE_900P30FPS_FINE        (VR_BITRATE_900P30FPS_FINE)
#define AHC_VIDEO_BITRATE_960P30FPS_SUPPERFINE  (VR_BITRATE_960P30FPS_SUPER)
#define AHC_VIDEO_BITRATE_960P30FPS_FINE        (VR_BITRATE_960P30FPS_FINE)

#define AHC_VIDEO_MAXBITRATE_PRERECORD			(8000000)

//General
#define AHC_GENERAL_DBG_LEVEL	    	0x0

#define AHC_MSG_QUEUE_SIZE		    	0x80
#define AHC_MSG_QUEUE_VIP_SIZE		    0x10
#define AHC_HP_MSG_QUEUE_SIZE		    0x08

#define MAX_FILE_NAME_SIZE          	256
#define SHOW_ALL_MENU_FUNCTIONS     	0
#define AHC_DATA_DIR					"DATA"
#define AHC_MESSAGE_QUEUE_OVF			(0)
#define AHC_AHL_MSGQ_TIMEOUT            (MS_TO_TICK(100))

#define USE_INFO_FILE           	    (0)

#define AHC_INFO_LOG					"SD:\\DATA\\Info.txt"

//DCF
#define MAX_DCF_SUPPORT_FILE_TYPE   	15
/**
@brief	the total memory space is about 512 kB
*/
#define MAX_DCF_MEM             	    ( 512 * 1024 )
#define AHC_DCF_BACKGROUND				(0)
#define DCF_DB_COUNT                    (3)//4/
#define DCF_SUB_DB                      (0)
#define DCF_DCFX                        (1)

/**
@brief	DCF file name
*/
#define DCF_FILE_NAME_TYPE_NORMAL       (0)  	//Use AHC_DCF   (FILE0001.MOV)
#define DCF_FILE_NAME_TYPE_DATE_TIME    (1)   	//Use AHC_DCFDT (FILE131005-092011.MOV)

#if (VIDRECD_MULTI_TRACK==0)
//only for TYPE_DATE_TIME, TYPE_NORMAL not finished yet
#define DCF_FILE_NAME_TYPE              (DCF_FILE_NAME_TYPE_DATE_TIME)
#define DCF_DUAL_CAM_ENABLE             (1)
#else
#define DCF_DUAL_CAM_ENABLE             (0)
#define DCF_FILE_NAME_TYPE              (DCF_FILE_NAME_TYPE_DATE_TIME)
#endif

#define ALL_TYPE_FILE_BROWSER_PLAYBACK 	((DCF_DB_COUNT == 1) & 0)

#define DSC_DEFAULT_FLIE_FREECHAR		"IMAG"
#define SNAP_DEFAULT_FILE_FREECHAR		"SNAP"
#define VIDEO_DEFAULT_FLIE_FREECHAR		"MOV_"
/*
DB0
*/
#define DCF_ROOT_DIR_NAME				"DCIM"
#define DCF_DEFAULT_DIR_FREECHAR    	"_NOML"
#define DCF_DEFAULT_FILE_FREECHAR   	(VIDEO_DEFAULT_FLIE_FREECHAR)
/*
DB1
*/
#define DCF_ROOT_DIR_NAME_2				"PARK"
#define DCF_DEFAULT_DIR_FREECHAR_2    	"_PARK"
#define DCF_DEFAULT_FILE_FREECHAR_2   	"PARK"
/*
DB2
*/
#define DCF_ROOT_DIR_NAME_3				"EMER"
#define DCF_DEFAULT_DIR_FREECHAR_3    	"_EMER"
#define DCF_DEFAULT_FILE_FREECHAR_3   	"EMER"
/*
DB3
*/
#define DCF_ROOT_DIR_NAME_4				"IMAG"
#define DCF_DEFAULT_DIR_FREECHAR_4      "_IMAG"
#define DCF_DEFAULT_FILE_FREECHAR_4     DSC_DEFAULT_FLIE_FREECHAR
/*
DCFDT Config
*/
//define SD card num
#define DCFDT_SD_NUM        (2)
//define camera num

#define DCFDT_CAM_NUM       (2)

//Important, this definition defines the customized mount DB flow and node operation
//if the DCFDT_CUSTOM_MODULE_MODE == DCFDT_MODULEMODE_DATETIME_INDEX, 
//all FILENAME_NAMING_TYPE shall be DCFDT_FILENAME_NAMING_BYINDEX
#define DCFDT_MODULEMODE_STANDARD_DATETIME (0)
#define DCFDT_MODULEMODE_DATETIME_INDEX    (1)
#define DCFDT_CUSTOM_MODULE_MODE           (DCFDT_MODULEMODE_STANDARD_DATETIME)
//only use for Index Mode of DCFDT, when new file meets duplicate name in DB,
//what system will do to the situation.
#define DCFDT_INDEXMODE_DUPLICATE_REPLACE  (0)
#define DCFDT_INDEXMODE_DUPLICATE_SKIP	   (1)
#define DCFDT_INDEXMODE_DUPLICATE_PROCESSTYPE (DCFDT_INDEXMODE_DUPLICATE_SKIP)

#define DCFDT_FILENAME_NAMING_STANDARD     (0)//FREECHAR + TIMEFORMAT + FIXEDCHAR  
#define DCFDT_FILENAME_NAMING_SINGLE_CAM   (1)
#define DCFDT_FILENAME_NAMING_BYINDEX      (2)

//define format free DB for one clip share function, only use for one clip share
#define DCF_DB_FORMAT_FREE_DB                 (DCF_DB_TYPE_5TH_DB)
#define DCF_SHARE_FOLDER_FILE_NUM             (10)     
#define DCF_SHARE_FOLDER_STORAGE_BY_SPACE     (0)
#define DCF_SHARE_FOLDER_STORAGE_BY_NUMERATOR (1)
#define DCF_SHARE_FOLDER_STORAGE_TYPE         (DCF_SHARE_FOLDER_STORAGE_BY_SPACE)
#define DCF_SHARE_FOLDER_SPACE                (200)  //unit : MB
#define DCF_SHARE_FOLDER_PER_FILE_SIZE        (SHARERECD_FILE_LENGTH+0x00105500) //unit : Byte   
//=====defined chars of file path================
//e.g. SD:\\Normal\F\FILE010101-010101F.MOV
//DCFDT_ROOTNAME_SD \\ DCFDT_DB0_COMMON_FOLDER_PATH \ DCFDT_DB0_CAM0_FOLDER_PATH \ DCFDT_FILENAME_NAMING_TYPE . ext 
#define DCFDT_FREECHAR_DB0                "FILE"
#define DCFDT_FREECHAR_DB1                "PARK"
#define DCFDT_FREECHAR_DB2                "EMER"
#define DCFDT_FREECHAR_DB3                "IMG"
#define DCFDT_FREECHAR_DB4                "SHARE"
#define DCFDT_FREECHAR_LOCK               "LOCK"
#define DCFDT_FIXEDCHAR_CAM0              "F"
#define DCFDT_FIXEDCHAR_CAM1              "R"

#define DCFDT_TIMEFORMAT_TYPE                     (DCFDT_TIMEFORMAT_TYPE0)

//define of TIMEFORMAT_TYPE0
#define DCFDT_TIMEFORMAT_TYPE0_TEMPLATE           "%02d%02d%02d-%02d%02d%02d"
#define DCFDT_TIMEFORMAT_TYPE0_LEN                (13)

//if add SD card, define ROOTNAME_SD and add enum of DCFDT_SD_CARD_ID
#define DCFDT_ROOTNAME_SD                   "SD:"
#define DCFDT_ROOTNAME_SD1                  "SD1:"
//if add new DB, add COMMON_FOLDER_PATH and fill sDBInit_Config
//if add new Camera, add cam num and define specific CAM_FOLDER_PATH and fill sDBInit_Config

#if (VIDRECD_MULTI_TRACK == 0)

#define DCFDF_DB0_FOLDER_LEVEL              (2)

#define DCFDT_DB0_COMMON_FOLDER_PATH        "Normal"
#define DCFDT_DB0_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB0_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB0_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_STANDARD)

#define DCFDF_DB1_FOLDER_LEVEL              (2)

#define DCFDT_DB1_COMMON_FOLDER_PATH        "Parking"
#define DCFDT_DB1_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB1_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB1_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_STANDARD)

#define DCFDF_DB2_FOLDER_LEVEL              (2)

#define DCFDT_DB2_COMMON_FOLDER_PATH        "Event"
#define DCFDT_DB2_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB2_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB2_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_STANDARD)

#define DCFDF_DB3_FOLDER_LEVEL              (2)

#define DCFDT_DB3_COMMON_FOLDER_PATH        "Photo"
#define DCFDT_DB3_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB3_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB3_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_STANDARD)

#define DCFDF_DB4_FOLDER_LEVEL              (2)

#define DCFDT_DB4_COMMON_FOLDER_PATH        "Share"
#define DCFDT_DB4_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB4_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB4_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_STANDARD)
#if(MIMF_CUSTOMIZED_SORT_TYPE == MIMF_CUSTOMIZED_SORT_MULTI_DB_DECREASE)
#define DCFDT_DBM_MULTI_FOLDER_PATH         "Multi"
#endif

#else

#define DCFDF_DB0_FOLDER_LEVEL              (1)

#define DCFDT_DB0_COMMON_FOLDER_PATH        "Normal"
#define DCFDT_DB0_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB0_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB0_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_SINGLE_CAM)

#define DCFDF_DB1_FOLDER_LEVEL              (1)

#define DCFDT_DB1_COMMON_FOLDER_PATH        "Parking"
#define DCFDT_DB1_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB1_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB1_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_SINGLE_CAM)

#define DCFDF_DB2_FOLDER_LEVEL              (1)

#define DCFDT_DB2_COMMON_FOLDER_PATH        "Event"
#define DCFDT_DB2_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB2_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB2_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_SINGLE_CAM)

#define DCFDF_DB3_FOLDER_LEVEL              (1)

#define DCFDT_DB3_COMMON_FOLDER_PATH        "Photo"
#define DCFDT_DB3_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB3_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB3_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_SINGLE_CAM) //(DCFDT_FILENAME_NAMING_STANDARD) for Dual Cam capture

#define DCFDF_DB4_FOLDER_LEVEL              (1)

#define DCFDT_DB4_COMMON_FOLDER_PATH        "Share"
#define DCFDT_DB4_CAM0_FOLDER_PATH          "F"
#define DCFDT_DB4_CAM1_FOLDER_PATH          "R"
#define DCFDT_DB4_FILENAME_NAMING_TYPE      (DCFDT_FILENAME_NAMING_SINGLE_CAM)
#if(MIMF_CUSTOMIZED_SORT_TYPE == MIMF_CUSTOMIZED_SORT_MULTI_DB_DECREASE)
#define DCFDT_DBM_MULTI_FOLDER_PATH         "Multi"
#endif

#endif //(VIDRECD_MULTI_TRACK == 0)

// Add a check for correct naming type
#if ((DCFDT_CUSTOM_MODULE_MODE == DCFDT_MODULEMODE_DATETIME_INDEX) && (DCFDT_DB0_FILENAME_NAMING_TYPE != DCFDT_FILENAME_NAMING_BYINDEX))
#error "DCFDT_MODULEMODE_DATETIME_INDEX only supports DCFDT_FILENAME_NAMING_BYINDEX!"
#endif

#define DCFDT_NEMA_FILE_EXTENTION           "NMEA"

/*
if DCFDT_DB_FILENAME_NAMING_TYPE = DCFDT_FILENAME_NAMING_BYINDEX 
save file setting
*/
#if(FS_FORMAT_FREE_ENABLE)
#if(DCF_DUAL_CAM_ENABLE)
#define DCFDT_FILEINDEX_RENAMESRC_FIlENAME     "SD:\\Normal\\F\\FILE100101-000000-100000F.AVI"
#define DCFDT_FILEINDEX_INDEXSAVED_FIlENAME    "SD:\\Normal\\F\\IndexData.txt"
#else
#define DCFDT_FILEINDEX_RENAMESRC_FIlENAME     "SD:\\Normal\\FILE100101-000000-100000F.AVI"
#define DCFDT_FILEINDEX_INDEXSAVED_FIlENAME    "SD:\\Normal\\IndexData.txt"
#endif
#else
#define DCFDT_FILEINDEX_INDEXSAVED_FIlENAME    "SD:\\IndexData.txt"
#endif
/*
DCFX
*/
#define DCFX_DB_NAME					"BASE.dat"

/*
DB Cluster TH
*/
#if (DCF_DB_COUNT > 4)
#define DCF_CLUSTER_NUMERATOR_DB0       (60)// NORMAL
#define DCF_CLUSTER_NUMERATOR_DB1       (20)// PARK
#define DCF_CLUSTER_NUMERATOR_DB2       (20)// EMER
#define DCF_CLUSTER_NUMERATOR_DB3       (0) // PICT
#define DCF_CLUSTER_NUMERATOR_DB4       (0)// SHARE
#else
#define DCF_CLUSTER_NUMERATOR_DB0       (59)// NORMAL
#define DCF_CLUSTER_NUMERATOR_DB1       (20)// PARK
#define DCF_CLUSTER_NUMERATOR_DB2       (20)// EMER
#define DCF_CLUSTER_NUMERATOR_DB3       (1) // PICT
#endif

#define DCF_CLUSTER_NUMERATOR1_DB0       (34)// NORMAL
#define DCF_CLUSTER_NUMERATOR1_DB1       (20)// PARK
#define DCF_CLUSTER_NUMERATOR1_DB2       (45)// EMER
#define DCF_CLUSTER_NUMERATOR1_DB3       (1) // PICT
#define DCF_CLUSTER_NUMERATOR1_DB4       (0)// SHARE

#define DCF_CLUSTER_NUMERATOR2_DB0       (39)// NORMAL
#define DCF_CLUSTER_NUMERATOR2_DB1       (30)// PARK
#define DCF_CLUSTER_NUMERATOR2_DB2       (30)// EMER
#define DCF_CLUSTER_NUMERATOR2_DB3       (1) // PICT
#define DCF_CLUSTER_NUMERATOR2_DB4       (0)// SHARE

#define DCF_CLUSTER_NUMERATOR3_DB0       (69)// NORMAL
#define DCF_CLUSTER_NUMERATOR3_DB1       (0) // PARK
#define DCF_CLUSTER_NUMERATOR3_DB2       (30)// EMER
#define DCF_CLUSTER_NUMERATOR3_DB3       (1) // PICT
#define DCF_CLUSTER_NUMERATOR3_DB4       (0)// SHARE

#define DCF_FILE_NUMERATOR2_DB0       	 (500)// NORMAL
#define DCF_FILE_NUMERATOR2_DB1       	 (500)// PARK
#define DCF_FILE_NUMERATOR2_DB2       	 (500)// EMER
#define DCF_FILE_NUMERATOR2_DB3       	 (500) // PICT

#define DCF_CLUSTER_DENOMINATOR         (100)

#define DELETION_BY_FILE_NUM             (1)   //TBD, add it temporarily till SFN DB update, by Jerry Li
//OSD
#define MAX_OSD_NUM                    	32
#define ENABLE_ALPHA_BLENDING           (0)

#define DATESTAMP_COLOR                 DATESTAMP_COLOR_WHITE

// video
#define AHC_VIDEO_STICKER_POS               AHC_ACC_TIMESTAMP_BOTTOM_RIGHT    
#define STICKER_PATTERN 					(1)
#define STICKER_PATTERN_COLOR				(DATESTAMP_COLOR_ORANGE)
#define AHC_VIDEO_STICKER_X_DEFAULT_POS     (160)
#define AHC_VIDEO_STICKER_Y_DEFAULT_POS     (40)
#define AHC_VIDEO_STICKER_Y_DELTA           (30)	// 60

//capture
#define AHC_CAPTURE_STICKER_POS             AHC_ACC_TIMESTAMP_BOTTOM_RIGHT    

#define AHC_CAPTURE_STICKER_X_DEFAULT_POS   (160)
#define AHC_CAPTURE_STICKER_Y_DEFAULT_POS   (40)
#define AHC_CAPTURE_STICKER_Y_DELTA         (40)

//gps
#define AHC_GPS_STICKER_ENABLE              (0)

#define THUMBNAIL_OSD_COLOR_FORMAT     		OSD_COLOR_RGB565
#define THUMBNAIL_BACKGROUND_COLOR          RGB565_COLOR_GRAY
#define THUMBNAIL_FOCUS_COLOR               RGB565_COLOR_BLUE2

//Browser/Thumbnail
#define MAX_DCF_IN_THUMB            	256
#define VIDEO_BS_SIZE_FOR_THUMB     	(512 * 1024)

#define AHC_BROWSER_THUMBNAIL_STYLE     AHC_BROWSER_THUMBNAIL_STYLE_1
#define AHC_BROWSER_DRAW_GRAY_BORDER		(0)
#define AHC_BROWSER_DRAW_FOCUS_THICK_BORDER	(0)
#define AHC_BROWSER_DRAW_PROTECT_KEY    	(1)
#define AHC_BROWSER_DRAW_DUAL_FILES     	(0) 	

//Video Record
#define MIN_VIDEO_ENC_WIDTH         	64
#define MIN_VIDEO_ENC_HEIGHT        	64

#if (AHC_DRAM_SIZE == COMMON_DRAM_SIZE_32MB)

#define MAX_VIDEO_ENC_WIDTH         	(2304)
#define MAX_VIDEO_ENC_HEIGHT        	(1296)

#else

#define MAX_VIDEO_ENC_WIDTH         	(4096)
#define MAX_VIDEO_ENC_HEIGHT        	(2160)

#endif

#define ENABLE_DVS						0
#define ENABLE_VIDEO_ERR_LOG			0

#if (ENABLE_DVS == 1)
#define AHC_ENABLE_VIDEO_STICKER		0
#else
#define AHC_ENABLE_VIDEO_STICKER	    1
#endif

#define VIDEO_STICKER_INDEX1_EN         0

#define AHC_ENABLE_STILL_STICKER		1

#if (DCF_DB_COUNT == 1)
#define PROTECT_FILE_TYPE               (PROTECT_FILE_RENAME)
#else
#define PROTECT_FILE_TYPE               (PROTECT_FILE_MOVE_DB)
#endif

#ifdef FILE_RENAME_STRING
#define DCF_CHARLOCK_PATTERN            FILE_RENAME_STRING
#else
#define DCF_CHARLOCK_PATTERN            "SOS_"
#endif

#define VR_SLOW_CARD_DETECT             (1)

#define VRCB_TOTAL_SKIP_FRAME           (100)

#define VRCB_CONTINUOUS_SKIP_FRAME      (5)

//Audio Record
#define MAX_AUDIO_ENC_FORMAT        	3

//DSC DBG
#define MAX_DBG_ARRAY_SIZE          	(64*1024)

//EXIF
#define MAX_EXIF_ASCII_LENGTH       	64
#define MAX_EXIF_CONFIG_ARRAY_SIZE  	(16 * 1024)

//STROBE
#define ENABLE_STROBE					0

//IRIS
#define ENABLE_IRIS					    0

//M SHUTTER
#define ENABLE_MECHANICAL_SHUTTER   	0

// USB
#define MTP_FUNC						0///< SUPPORT_MTP_FUNC is connected with this definition
//#define CONFIG_USB_PRODUCT_STR          {'M','i','V','u','e',' ','D','V','R',' ',' ',' ',' ',' '}

#define UVC_HOST_VIDEO_ENABLE           (SUPPORT_USB_HOST_FUNC & (AHC_DRAM_SIZE != COMMON_DRAM_SIZE_32MB) & 1)

// SD Card
#define ENABLE_SD_HOT_PLUGGING      	1
#define SUPPORT_LONGSYS_SMART_CARD      0

//Battery detection UART message
#define ENABLE_BATTERY_DET_DEBUG		(BATTERY_DETECTION_EN & 0)
#define ENABLE_BATTERY_TEMP_DEBUG	    (BATTERY_DETECTION_EN & 0)

//Speaker Control
#define NEVER_TURN_OFF_SPEAKER			0

// GPS to RTC time offset
#define GPS_TO_RTC_TIME_OFFSET          0

// RTC
#define RTC_DEFAULT_YEAR				(2011)
#define RTC_MAX_YEAR                	(2099)

//#define PRIMARY_DATESTAMP_WIDTH        	320
//#define PRIMARY_DATESTAMP_HEIGHT       	64
//#define PRIMARY_DATESTAMP_COLOR_FORMAT 	OSD_COLOR_RGB332
//#define THUMB_DATESTAMP_WIDTH          	320
//#define THUMB_DATESTAMP_HEIGHT         	64
//#define THUMB_DATESTAMP_COLOR_FORMAT   	OSD_COLOR_RGB332

#define AHC_CAPTURE_STICKER_HEIGHT      PRIMARY_DATESTAMP_HEIGHT

#if (VERTICAL_LCD == VERTICAL_LCD_DEGREE_0) 
#define AHC_HD_VIDPLAY_MAX_WIDTH        (1280)
#define AHC_HD_VIDPLAY_MAX_HEIGHT       (720)
#elif (VERTICAL_LCD == VERTICAL_LCD_DEGREE_90) || (VERTICAL_LCD == VERTICAL_LCD_DEGREE_270)
#define AHC_HD_VIDPLAY_MAX_WIDTH        (720)
#define AHC_HD_VIDPLAY_MAX_HEIGHT       (1280)
#endif

#define STICKER_FRONT_1                 GUI_Font32B_1
#define STICKER_FRONT_2                 GUI_Font24B_1


#define EMER_RECORD_DUAL_WRITE_ENABLE   (AHC_EMERGENTRECD_SUPPORT & 1)

#define EMER_RECORD_DUAL_WRITE_PRETIME  (5)  // 5s  // Pre-recording time of emergency recording
#define EMER_RECORD_DUAL_WRTIE_DELTA    (5)
#define EMER_RECORD_DUAL_WRITE_POSTTIME (10) //10s  // The increased time if triggered again and remaining emergency recording time <= EMER_RECORD_DUAL_WRTIE_DELTA
#define EMER_RECORD_DUAL_WRITE_INTERVAL (20) //10s  // The emergency recorded time after 1st trigger (including pre-recording time)
#define EMER_RECORD_DUAL_WRTIE_MAX_TIME (120)//120s = 2mins // Max time for an emergency recording

#define DUAL_RECORD_WRITE_PRETIME       (5)     //  5s   // Pre-recording time of dualenc recording
#define DUAL_RECORD_WRITE_PRETIME_LIMIT (10)    //  10s  // Pre-recording time of dualenc recording
#define DUAL_RECORD_WRITE_INTERVAL      (20)    //  10s  // The recorded time after 1st trigger (including pre-recording time)
#define SHARE_FILESIZE_OFFSET           (28)    //  3gp file's "skip" field
#define EMER_RECORD_WRITE_INTERVAL      (30)

#define UUID_XML_FEATURE                (0) ///(1)

//#define SPEEM_CAM_MANUAL_DB				("SF:2:\\SpeedCam_Data_Manual.bin")

#define LOAD_MENU_DEFFAULT_SETTING_FROM_FILE  0
#define MENU_DEFAULT_SETTING_FILENAME         "SF:0:\\MenuSetting.bin"

#define AHC_VIDEO_PLUG_OUT_AC				   (0)///(1)

#define PARK_RECORD_WRTIE_MAX_TIME				(180)
#define PARK_RECORD_WRITE_POSTTIME 				(10)
#define PARK_RECORD_INTERVAL					(10) 

//#define CFG_MVD_MODE_LINK_WITH_MENU_SENSITIVITY
//#define CFG_ENABLE_MIN_VR_TIME                  (1000)      // ms
#define LOCK_KEYPAD_UNDER_WIFI_STREAMING		
//EDOG Setting
#define   AHC_EDOG_POI_DATA_SEGMENTATION         				(1)		//1:for China, 0:for other

#define SD_UPDATE_FW_SIZE (9*1024*1024)  // 9 MB
#include "AHC_Config_Dummy.h"

//set panel yuv attribute
#define YUV_U2U       0   //default 0
#define YUV_V2U       0   //default 0              
#define YUV_U2V       0   //default 0  
#define YUV_V2V       32  //default 32
#define YUV_YGAIN     16  //default 16  range 0~31
#define YUV_YOFFSET   0   //default 0 

#define ENABLE_SET_YUV_ATTRIBUTE   0

#endif	// __AHC_CONFIG_SDK_H__
