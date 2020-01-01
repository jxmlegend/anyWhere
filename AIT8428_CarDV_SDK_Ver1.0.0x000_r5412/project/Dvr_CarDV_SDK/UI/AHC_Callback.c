//==============================================================================
//
//  File        : AHC_Callback.c
//  Description : Callback functions from MMP core
//  Author      : Sunny Chang
//  Revision    : 1.0
//
//==============================================================================

/*===========================================================================
 * Include files
 *===========================================================================*/ 

#include "AHC_Config_SDK.h"
#include "AHC_General.h"
#include "MenuSetting.h" 
#include "GPS_ctl.h"
#include "GSensor_ctrl.h" 
#include "Mmps_fs.h" 
#include "AHC_DCFDT.h"

#include "AHC_Fs.h"
#include "AHC_UF.h"
#include "mmpf_fs_api.h"
#include "ait_utility.h"
#include "Uartshell.h"

/*===========================================================================
 * Function prototype
 *===========================================================================*/ 

INT32 AHC_DCFDT_GetFormatFreeDBFileNum(UINT DBIdx);

/*===========================================================================
 * Main body
 *===========================================================================*/ 

//==============================================================================
//  Function    : AHC_CheckCallbackExisted
//  Description : This function will be called in AHC_Initialize() to make sure 
//                functions in AHC_Callback.c can work with __weak declare.
//                (It seems a compilation link bug of CodeWarrior.)
//==============================================================================
void AHC_CheckCallbackExisted( void )
{
	//TBD
}

//==============================================================================
//  Function    : AHC_PostInitISP
//  Description : This function will be called in MMP_SensorDrv_PostInitISP() 
//                which will be run after reset ISP in MMPS_Sensor_Initialize().
//                It's used to re-set IPS settings after sensor init.
//==============================================================================
void AHC_PostInitISP( void )
{
    // Note: Below code is just for reference, please DO NOT enable it!!!
        // They will be run in MMPS_Sensor_Initialize()
        #if 0
        // initialize AE mode, min FPS, AWB mode, matering mode, EV bias, face detection status,
        //            AF mode, contrast, sharpness, saturation, image effect, filcker mode, WDR
        ISP_IF_3A_Control(ISP_3A_DISABLE);
        ISP_IF_IQ_SetRunMode(0);
        ISP_IF_AE_SetMinFPS(8);
        ISP_IF_AE_SetISO(0);
        ISP_IF_AE_SetMetering(ISP_AE_METERING_CENTER);
        ISP_IF_AE_SetEVBias(0, 250);
        ISP_IF_AWB_SetMode(ISP_AWB_MODE_AUTO);
        ISP_IF_F_SetSaturation(0);
        ISP_IF_F_SetSharpness(0);
        ISP_IF_F_SetContrast(0);
        VR_AF_SetMode(ISP_AF_MODE_AUTO, 0);
        ISP_IF_F_SetImageEffect(ISP_IMAGE_EFFECT_NORMAL);
        ISP_IF_AE_SetFlicker(ISP_AE_FLICKER_50HZ);
        ISP_IF_AE_SetFPS(0);
        ISP_IF_AE_SetEV(0);
        ISP_IF_F_SetHue(0);
        ISP_IF_F_SetGamma(0);
        ISP_IF_F_SetBrightness(0);
        ISP_IF_F_SetPosterization(255);
        ISP_IF_F_SetZoomRatio(100);
        ISP_IF_F_AdjustColorTemp(0);
        ISP_IF_F_SetColorEffect(ISP_COLOR_EFFECT_NORMAL, 0, 0, 0, 0, 0, 0, 0);
        ISP_IF_F_SetTiltShift(0, 0, 0, 0, 0);
        ISP_IF_F_SetBeautyShot(0, 0, 0, 0, 0);
        ISP_IF_F_SetDOFEffect(0, 0, 0, 0, 0);
        ISP_IF_F_SetWDREn(0);
        ISP_IF_F_SetDark(0);
        VR_Apical_SetWDRCtl(0);
        VR_Apical_SetCCMCtl(0);
        ISP_IF_F_SetAL(ISP_AL_SET, 0x00, 0xFF, 0x20, 0xE0, 1, 1, 10, 15);
        ISP_IF_F_SetAL(ISP_AL_DISABLE, 0, 0, 0, 0, 0, 0, 0, 0);
        #endif

    INT32 s32Setting = 0;
    UINT8  u8Setting = 0;

    if (MenuSettingConfig()->uiEffect != EFFECT_NORMAL)
    {
        Menu_SetEffect(MenuSettingConfig()->uiEffect);
    }
    else
    {
        if (AHC_Menu_SettingGetCB( (char *) COMMON_KEY_CONTRAST, &s32Setting ) == AHC_TRUE) {
            AHC_SetColorContrastLevel(s32Setting);
        }

        if (MenuSettingConfig()->uiColor != COLOR_NATURAL)
        {
            Menu_SetColor(MenuSettingConfig()->uiColor);
        }
        else
        {
            if (AHC_Menu_SettingGetCB( (char *) COMMON_KEY_SATURATION, &s32Setting ) == AHC_TRUE) {
                AHC_SetColorSaturateLevel(s32Setting);
            }
        }

        if (AHC_Menu_SettingGetCB( (char *) COMMON_KEY_SHARPNESS, &s32Setting ) == AHC_TRUE) {
            AHC_SetColorSharpnessLevel(s32Setting);
        }

        if (AHC_Menu_SettingGetCB( (char *) COMMON_KEY_GAMMA, &s32Setting ) == AHC_TRUE) {
            AHC_SetColorGammaLevel(s32Setting);
        }

        if (AHC_Menu_SettingGetCB( (char *) COMMON_KEY_WB, &u8Setting ) == AHC_TRUE) {
            Menu_SetAWB(u8Setting);
        }
    }
    if (AHC_Menu_SettingGetCB( (char *) COMMON_KEY_EV, &u8Setting ) == AHC_TRUE) {
        Menu_SetEV(u8Setting);
    }

    if (AHC_Menu_SettingGetCB( (char *) COMMON_KEY_ISO, &u8Setting ) == AHC_TRUE) {
        Menu_SetISO(u8Setting);
    }
    //MMPS_Sensor_SetIQFunction(MMP_ISP_IQ_FUNC_SET_WDR,MenuSettingConfig()->uiWDR);
    //MMPS_Sensor_SetIQFunction(MMP_ISP_IQ_FUNC_SET_HUE,MenuSettingConfig()->);
}

__weak void AHC_PostEmergencyDone(void)
{
	
    if (AHC_VIDEO_GetRecCardSlowStop())
    {
        AHC_VIDEO_SetRecordModeDeleteFile(AHC_TRUE);    
        AHC_VIDEO_RestartRecMode();
        AHC_VIDEO_SetRecCardSlowStop(AHC_FALSE);
    }
    else if(AHC_VIDEO_GetAPStopRecord())
    {
#if (GPS_RAW_FILE_ENABLE == 1)
        if (MMPS_3GPRECD_GetEmergActionType() == MMP_3GPRECD_EMERG_SWITCH_FILE)
        {
            //if(GPSCtrl_GPSRawWriteFirst_Normal())
            GPSCtrl_GPSRawResetBuffer();
        }
#endif
        AHC_VIDEO_SetRecordModeDeleteFile(AHC_TRUE);    
        AHC_VIDEO_RestartRecMode();
        AHC_VIDEO_SetAPStopRecord(AHC_FALSE);
    }
    else
    {
        // Add below code to sync GPS / GSensor data with video of next Normal record
#if (GPS_RAW_FILE_ENABLE == 0 && GSENSOR_RAW_FILE_ENABLE == 0)
        UINT32 ulPreRecordedTime = 0;

        if (MMPS_3GPRECD_GetEmergActionType() == MMP_3GPRECD_EMERG_SWITCH_FILE)
        {
            AHC_VIDEO_RecordStartWriteInfo();
            AHC_VIDEO_GetCurRecordingTime(&ulPreRecordedTime); //Note: AHC_VIDEO_GetCurRecordingTime() can NOT be run in VR Callback functions
#if (GPS_CONNECT_ENABLE)
            AHC_GPS_TriggerRestorePreRecordInfo( AHC_TRUE, ulPreRecordedTime, AHC_TRUE );
#endif
#if (GSENSOR_CONNECT_ENABLE)
            AHC_Gsensor_TriggerRestorePreRecordInfo( AHC_TRUE, ulPreRecordedTime, AHC_TRUE );
#endif
//#endif
        }
#endif        
        if( SystemSettingConfig()->byStartNormalRecordAfterEmergencyRecord != 0 )
        {
            AHC_VIDEO_SetRecordModeDeleteFile(AHC_TRUE);    
            AHC_VIDEO_RestartRecMode();
        }
    }
	
}

#if (AHC_SHAREENC_SUPPORT)
__weak void AHC_PostDualRecdDone(void)
{
    AHC_VIDEO_SetRecordModeDeleteFile(AHC_TRUE);    

    if (AHC_VIDEO_GetRecCardSlowStop())
    {
        AHC_VIDEO_RestartRecMode();
        AHC_VIDEO_SetRecCardSlowStop(AHC_FALSE);
    }
    else if (AHC_VIDEO_GetAPStopRecord())
    {
        AHC_VIDEO_RestartRecMode();
        AHC_VIDEO_SetAPStopRecord(AHC_FALSE);
    }
    else
    {    
        AHC_VIDEO_RestartRecMode();        
    }	
}
#endif

//==============================================================================
//  Function    : AHC_UF_CreateShareFileCB
//  Description : This function will create/pre-allocate spaces for multiples share files
//==============================================================================
__weak void AHC_UF_CreateShareFileCB(UINT8 *NeedFileNum)
{
    int          i, j;
    //INT8         szTemplate[DCF_MAX_FILE_NAME_SIZE];
    MMP_BYTE     filename[MAX_FILE_NAME_SIZE];
    MMP_BYTE     fullname[MAX_FILE_NAME_SIZE];    
    //DCF_DB_TYPE  sType;    
    MMP_ERR      err = MMP_ERR_NONE;
    MMP_ULONG    ulFileId, count;
    UINT8     bCreateNewDir;
    #if (DCF_FILE_NAME_TYPE == DCF_FILE_NAME_TYPE_DATE_TIME)
    AHC_RTC_TIME DefaultRtcTime = {RTC_MAX_YEAR, RTC_MAX_MONTH, RTC_MAX_DAY_31, 0,
                                            RTC_MAX_HOUR, RTC_MAX_MIN, RTC_DEFAULT_SEC};    
    INT32       sharefilenum = AHC_DCFDT_GetFormatFreeDBFileNum(DCF_DB_FORMAT_FREE_DB);
    #else
	INT32       sharefilenum = 0;
	AHC_RTC_TIME DefaultRtcTime = {RTC_DEFAULT_YEAR, RTC_DEFAULT_MONTH, RTC_DEFAULT_DAY, 
                                            RTC_DEFAULT_HOUR, RTC_DEFAULT_MIN, RTC_DEFAULT_SEC};
    #endif
    
    //If SD_CarDV.bin existed, no need to create share folder & file, this could lead to ulOSDInitTimeout
    if( !(MMPS_FS_FileOpen(SD_FW_UPDATER_BIN_NAME,STRLEN(SD_FW_UPDATER_BIN_NAME),"rb",STRLEN("rb"),&ulFileId)) ) {
    	MMPS_FS_FileClose(ulFileId);	
    	return;
    }
        	
    printc(FG_GREEN("\n<AHC_DCFDT_CreateShareFileCB>\r\n"));
    //sType = AHC_UF_GetDB();
    AHC_UF_SelectDB(DCF_DB_FORMAT_FREE_DB);        
    for (i = (sharefilenum - *NeedFileNum); i < sharefilenum; i++)
    {                
        MEMSET(fullname, 0, sizeof(fullname));
        MEMSET(filename, 0, sizeof(filename));        
        AHC_UF_GetName2(&DefaultRtcTime, (INT8*)fullname, (INT8*)filename, &bCreateNewDir);  
        STRCAT(fullname, EXT_DOT);        
        STRCAT((INT8*)fullname, MOVIE_3GP_EXT);  
        STRCAT(filename, EXT_DOT);        
        STRCAT((INT8*)filename, MOVIE_3GP_EXT); 

        printc("\r\nShare file=%s\r\n", fullname);

        //err = MMPS_FS_FileOpen(fullname, strlen(fullname), "w+b", strlen("w+b"), &ulFileId);
        err = MMPS_FS_FileOpen(fullname, strlen(fullname), "r", strlen("r"), &ulFileId);

        j = 0;
        while (err == MMP_ERR_NONE) {  // try another new filename
            MMPS_FS_FileClose(ulFileId);
            #if 0
            sprintc((INT8*)filename, szTemplate
                                   , DefaultRtcTime.uwYear - DCFDT_FILENAME_YEAR_OFFSET
                                   , DefaultRtcTime.uwMonth
                                   , DefaultRtcTime.uwDay
                                   , DefaultRtcTime.uwHour
                                   , DefaultRtcTime.uwMinute
                                   , DefaultRtcTime.uwSecond + i + sharefilenum + j);
            #endif
            DefaultRtcTime.uwSecond += (i + sharefilenum + j);
            AHC_UF_GetName2(&DefaultRtcTime, (INT8*)fullname, (INT8*)filename, &bCreateNewDir);                             
            STRCAT(fullname, EXT_DOT);        
            STRCAT((INT8*)fullname, MOVIE_3GP_EXT);  
            STRCAT(filename, EXT_DOT);        
            STRCAT((INT8*)filename, MOVIE_3GP_EXT); 

            printc("\r\nShare file2 =%s\r\n", fullname);
            err = MMPS_FS_FileOpen(fullname, strlen(fullname), "r", strlen("r"), &ulFileId);
            if (err == MMP_ERR_NONE) {
                MMPS_FS_FileClose(ulFileId);
                j++;
            }
        }

        err = MMPS_FS_FileOpen(fullname, strlen(fullname), "w+b", strlen("w+b"), &ulFileId);
        if (err != MMP_ERR_NONE) {RTNA_DBG_Str0("Open share file error! -1\r\n");}
        err = MMPS_FS_FileSeek(ulFileId, SHARERECD_FILE_LENGTH, MMP_FS_FILE_BEGIN);
        if (err != MMP_ERR_NONE) {RTNA_DBG_Str0("Seek share file error! -1\r\n");}
        err = MMPS_FS_FileWrite(ulFileId, (void*)0x00105500, 1, &count);
        if (err != MMP_ERR_NONE) {RTNA_DBG_Str0("Write share file error! -1\r\n");}
        err = MMPS_FS_FileClose(ulFileId);
        if (err != MMP_ERR_NONE) {RTNA_DBG_Str0("Close share file error! -1\r\n");}
        AHC_FS_FileDirSetAttribute((INT8*)fullname, STRLEN(fullname), AHC_FS_ATTR_HIDDEN);
        DefaultRtcTime.uwSecond += 1;
    }
        
    //AHC_UF_SelectDB(sType);
}

#if (FS_FORMAT_FREE_ENABLE)
//==============================================================================
//  Function    : AHC_FormatFreeConfigInfo
//  Description : This function will be called before check / format media with 
//                AIT Format Free
//                It will design the folder / file layout of media.
//==============================================================================
void AHC_FormatFreeConfigInfo( void )
{
    //Because DCF has not init yet, this func can only get folderpath/filename itself, TBD
    FormatFreeFolderConfig   TempFolderConfig;
    
#if(DCF_DUAL_CAM_ENABLE == 0)
    TempFolderConfig.FileSizeMB = 150; 
    TempFolderConfig.SpacePercentage = 40;
    #if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_SINGLE_CAM)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 0, "SD:\\Normal", "FILE100101-000000.AVI");      
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 0, "SD:\\Normal", "FILE100101-000000.AVI");
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 0, "SD:\\Normal", "FILE100101-000000-100001F.AVI");
	#endif
	
	TempFolderConfig.FileSizeMB = 100;
    TempFolderConfig.SpacePercentage = 30;
	#if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_SINGLE_CAM)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 1, "SD:\\Parking", "PARK100101-000000.AVI");  
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 1, "SD:\\Parking", "PARK100101-000000.AVI");
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 1, "SD:\\Parking", "PARK100101-000000-100001F.AVI"); 
	#endif
    TempFolderConfig.FileSizeMB = 100;
    TempFolderConfig.SpacePercentage = 20;
	#if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_SINGLE_CAM)    
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 2, "SD:\\Event", "EMER100101-000000.AVI");  
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 2, "SD:\\Event", "EMER100101-000000.AVI");
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 2, "SD:\\Event", "EMER100101-000000-100001F.AVI");
	#endif
    TempFolderConfig.FileSizeMB = 10;
    TempFolderConfig.SpacePercentage = 5;
	#if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_SINGLE_CAM)      
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 3, "SD:\\Photo", "IMG100101-000000.JPG");  
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 3, "SD:\\Photo", "IMG100101-000000.JPG");  
	#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 3, "SD:\\Photo", "IMG100101-000000-100001F.JPG");
    #endif
#else

    //preserve a 0 for F/R char so FILE100101-0000000.AVI have one more "0"
	TempFolderConfig.FileSizeMB = 0; 
    TempFolderConfig.SpacePercentage = 0;    
    MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 1, 0, "SD:\\Normal", NULL);      
    
    TempFolderConfig.FileSizeMB = 0;
    TempFolderConfig.SpacePercentage = 0;    
    MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 1, 1, "SD:\\Parking", NULL);  
    
    TempFolderConfig.FileSizeMB = 0;
    TempFolderConfig.SpacePercentage = 0;    
    MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 1, 2, "SD:\\Event", NULL);  

    TempFolderConfig.FileSizeMB = 0;
    TempFolderConfig.SpacePercentage = 0;    
    MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 1, 3, "SD:\\Photo", NULL);  

	TempFolderConfig.FileSizeMB = 150; 
    TempFolderConfig.SpacePercentage = 28;   
	if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 4, "SD:\\Normal\\F", "FILE100101-000000F.AVI");      
    else if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 4, "SD:\\Normal\\F", "FILE100101-000000-100001F.AVI");

	TempFolderConfig.FileSizeMB = 120;
    TempFolderConfig.SpacePercentage = 10;   
	#if(DCFDT_DB1_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 5, "SD:\\Parking\\F", "PARK100101-000000F.AVI");  
    #elif(DCFDT_DB1_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 5, "SD:\\Parking\\F", "PARK100101-000000-100001F.AVI");
	#endif
	TempFolderConfig.FileSizeMB = 100;
    TempFolderConfig.SpacePercentage = 6;    
	#if(DCFDT_DB2_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 6, "SD:\\Event\\F", "EMER100101-000000F.AVI");  
	#elif(DCFDT_DB2_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 6, "SD:\\Event\\F", "EMER100101-000000-100001F.AVI");
	#endif
	TempFolderConfig.FileSizeMB = 6;
    TempFolderConfig.SpacePercentage = 5;    
	#if(DCFDT_DB3_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 7, "SD:\\Photo\\F", "IMG100101-000000F.JPG");  
	#elif(DCFDT_DB3_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 7, "SD:\\Photo\\F", "IMG100101-000000-100001F.JPG");
	#endif
	TempFolderConfig.FileSizeMB = 100; //45MB/min~=50MB/min ,100MB=2min
    TempFolderConfig.SpacePercentage = 24;
	#if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 8, "SD:\\Normal\\R", "FILE100101-000000R.AVI");      
    #elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 8, "SD:\\Normal\\R", "FILE100101-000000-100001R.AVI");
	#endif
    TempFolderConfig.FileSizeMB = 130;
    TempFolderConfig.SpacePercentage = 12;   
	#if(DCFDT_DB1_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 9, "SD:\\Parking\\R", "PARK100101-000000R.AVI");  
    #elif(DCFDT_DB1_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 9, "SD:\\Parking\\R", "PARK100101-000000-100001R.AVI");
	#endif
    TempFolderConfig.FileSizeMB = 120;
    TempFolderConfig.SpacePercentage = 7;    
	#if(DCFDT_DB2_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 10, "SD:\\Event\\R", "EMER100101-000000R.AVI");  
	#elif(DCFDT_DB2_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 10, "SD:\\Event\\R", "EMER100101-000000-100001R.AVI");
	#endif
    TempFolderConfig.FileSizeMB = 5;
    TempFolderConfig.SpacePercentage = 5;
	#if(DCFDT_DB3_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_STANDARD)
    	MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 11, "SD:\\Photo\\R", "IMG100101-000000R.JPG");
	#elif(DCFDT_DB3_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_BYINDEX)
		MMPS_FORMATFREE_ConfigInfo( &TempFolderConfig, 0, 11, "SD:\\Photo\\R", "IMG100101-000000-100001R.JPG");
	#endif
#endif
}

//==============================================================================
//  Function    : AHC_FormatFreeFileNameCB
//  Description : This function will be called when format media with AIT Format Free.
//                It allows to modify default filename to meet DCF file name rule.
//==============================================================================
void AHC_FormatFreeFileNameCB( MMP_ULONG ulFolderIndex, MMP_ULONG ulFileIndex, MMP_BYTE byFileName[] )
{
    UINT uiStrlen = 0;
    char sSequence[5]; //Add one more byte for '\0'.
    AHC_BOOL bCreareRearCamFile = AHC_FALSE;
    AHC_BOOL bFrontCam = AHC_TRUE;

    #if (VIDRECD_MULTI_TRACK == 1)
    if ((ulFolderIndex == 3) &&  //"SD:\\Photo"
        (AHC_UF_GetInitDBFlag(ulFolderIndex) & DCFDT_DB_FLAG_HAS_REAR_CAM))
    {
        // Create files as below:
        // SD:\Photo\IMG100101-000000F.JPG
        // SD:\Photo\IMG100101-000000R.JPG
        // SD:\Photo\IMG100101-000001F.JPG
        // SD:\Photo\IMG100101-000001R.JPG
        // ...................
        
        bCreareRearCamFile = AHC_TRUE;

        if( ulFileIndex % 2 )
            bFrontCam = AHC_FALSE;
        ulFileIndex /= 2;
    }
    #endif
    
    // Change ulFileIndex to format of minute+second
    // Ex: "FILE110101-000079.AVI" --> "FILE110101-000119.AVI"
    //printc(" old [%d][%d]: %s\r\n", ulFolderIndex, ulFileIndex, byFileName);
    ulFileIndex = (ulFileIndex/60)*100 + (ulFileIndex%60); 
	#if(DCF_DUAL_CAM_ENABLE == 1 && DCFDT_CUSTOM_MODULE_MODE == DCFDT_MODULEMODE_DATETIME_INDEX)
	//"SD:\\Normal\\R" index++ ,bypass index 0,\\Normal\\F first file has been renamed to Index.txt,
	//and cause asymmetric issue 
	if(ulFolderIndex == 8)
	{
		ulFileIndex++;
	}
	#endif
    sprintf(sSequence, "%04d", ulFileIndex );   
    uiStrlen = strlen( byFileName );
    
#if (DCF_FILE_NAME_TYPE == DCF_FILE_NAME_TYPE_DATE_TIME)
    #if(DCF_DUAL_CAM_ENABLE == 0)	
	if(DCFDT_CUSTOM_MODULE_MODE == DCFDT_MODULEMODE_STANDARD_DATETIME)
	{
		//Because strncpy() will automatically write '\0' in the end, so the '.' will be overwrite. Manually write it back.
		strncpy( byFileName+uiStrlen-8, sSequence, 4);
		#if(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_FILENAME_NAMING_SINGLE_CAM) 
    	*(byFileName+uiStrlen-4) = 0x2E; //ASCII '.' 
		#elif(DCFDT_DB0_FILENAME_NAMING_TYPE == DCFDT_DB0_FILENAME_NAMING_TYPE)
		*(byFileName+uiStrlen-4) = 0x46; //ASCII 0x46 'F'
		strncpy( byFileName+uiStrlen-3, ".JPG", 4);
		#endif
        if( bCreareRearCamFile == AHC_TRUE ) 
        {
            if( bFrontCam == AHC_TRUE )
            {
                *(byFileName+uiStrlen-4) = 0x46; //ASCII 0x46 'F'
            }
            else
            {
                *(byFileName+uiStrlen-4) = 0x52; //ASCII 0x52 'R'
            }
            strncpy( byFileName+uiStrlen-3, ".JPG", 4);
        }
	}
	else if(DCFDT_CUSTOM_MODULE_MODE == DCFDT_MODULEMODE_DATETIME_INDEX)
	{
		strncpy( byFileName+uiStrlen-9, sSequence, 4);
		*(byFileName+uiStrlen-5) = 0x46; //ASCII 0x46 'F'

        if( bCreareRearCamFile == AHC_TRUE ) 
        {
            if( bFrontCam == AHC_FALSE )
            {
                *(byFileName+uiStrlen-5) = 0x52; //ASCII 0x52 'R'
            }
        }
	}
    #else	
    strncpy( byFileName+uiStrlen-9, sSequence, 4);
    if((ulFolderIndex >= 4) && (ulFolderIndex <= 7))
    {       
        *(byFileName+uiStrlen-5) = 0x46; //ASCII 0x46 'F'
    }
    else if((ulFolderIndex >= 8) && (ulFolderIndex <= 11))
    {
        *(byFileName+uiStrlen-5) = 0x52; //ASCII 0x52 'R'
    }
    #endif
#endif
    //printc("[%d][%d]: %s\r\n", ulFolderIndex, ulFileIndex, byFileName);
}
#endif

