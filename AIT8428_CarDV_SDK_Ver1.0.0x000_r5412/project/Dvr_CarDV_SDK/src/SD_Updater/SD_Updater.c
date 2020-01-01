/*===========================================================================
 * Include file
 *===========================================================================*/

#if 0
void __HEADER__(){}
#endif

#include "AHC_common.h"
#include "AHC_utility.h"
#include "AHC_GUI.h"
#include "AHC_menu.h"
#include "AHC_Fs.h"
#include "AHC_Display.h"
#include "AHC_Media.h"
#include "LedControl.h"
#include "ColorDefine.h"
#include "IconDefine.h"
#include "ait_utility.h"
#include "mmpf_storage_api.h"
#include "OSDStrings.h"
#include "ShowOSDFunc.h"
#include "mmpf_sf.h"
#include "lcd_common.h"

#include "cdvmem.h"
#include "OSDStrings.h"


#include "MD5_in.h"
#include "stdio.h"
#include "MenuDrawCommon.h"
#include "SD_Updater.h"
#include "StateTVFunc.h"
#include "StateHDMIFunc.h"
#include "Fs_cfg.h"
#include "Fs_cfg.h"

#include "UpdaterX.h"
#if(WIFI_PORT==1)
#include "amn_config.h"
#endif

#if (EDOG_ENABLE == 1)
#include "EDOG_ctl.h"
#endif
/*===========================================================================
 * Macro define
 *===========================================================================*/

#ifdef _DEBUG_
#define DPRINT( ... ) 	printc(  __VA_ARGS__ )
#else
#define DPRINT( ... )
#endif

#define SD_UPDATER_DEBUG_LEVEL0 (0)
#define SD_UPDATER_DEBUG_LEVEL1 (1)
#define SD_UPDATER_DEBUG_LEVEL2 (2)

#define SD_UPDATER_DEBUG        SD_UPDATER_DEBUG_LEVEL0

#define	SIF_MBR_ADDRESS         (0x8000)
#define SIF_MBR_SIZE            (0x200)

#define	BLOCK_SIZE			    128

#ifndef CHECKSDFWHEADER
#define CHECKSDFWHEADER         (0)
#endif

#ifndef SDUPDATEDRAWPROGRESS
#define SDUPDATEDRAWPROGRESS    (1)
#endif

#define STORAGE_TEMP_BUFFER 0x106000

/*===========================================================================
 * Stucture define
 *===========================================================================*/



/*===========================================================================
 * Global varible
 *===========================================================================*/

static SD_UPDATER_ERR m_eSDUpdater;
// for WIFI Remote status
enum {
	FWU_NONE,
	FWU_PREPARE,
	FWU_BURN,
	FWU_VERIFY,
	FWU_SUCCEED,
	FWU_FAIL,
	FWU_ERROR
	};
static int _fwuState = FWU_NONE;
static int _uPercent = 0;
#if (SD_FW_UPDATER)
static MMP_ULONG m_uiIndexTable[SIF_MBR_SIZE>>2];
#endif
static char gubWpFileName[64] = "SD:\\SD_WpData";
static char gubWpExtName[8] = ".txt";
MMP_UBYTE   gubWpFileNum = 1;
MMP_ULONG   m_BackupFileID = 0;
MMP_ULONG m_DumpSpiFileID = 0;

extern int KeySize;
extern AHC_OSD_INSTANCE     *m_OSD[];
extern AHC_BOOL             gbAhcDbgBrk;
#if (DEBUG_UART_TO_FILE)
extern AHC_BOOL             gbBlockDebugWr;
#endif
/*===========================================================================
 * Extern function
 *===========================================================================*/

extern MMP_ERR MMPF_SF_WriteData(MMP_ULONG ulSFAddr, MMP_ULONG ulDmaAddr, MMP_ULONG ulByteCount);
extern MMP_ERR MMPF_SF_EraseChip(void);
extern UINT8 AHC_OS_Sleep(UINT16 usTickCount);
extern MMP_ERR MMPF_SF_SetProtWpArea(void);
extern MMP_ERR MMPF_SF_ClrProtArea(void);
extern MMP_ERR MMPF_FS_FRead(MMP_ULONG ulFileID, void *pData, MMP_ULONG NumBytes, MMP_ULONG *write_count);
extern MMP_ERR MMPF_FS_FWrite(MMP_ULONG ulFileID, void *pData, MMP_ULONG NumBytes, MMP_ULONG *write_count);
extern MMP_ERR MMPF_FS_FOpen(const MMP_BYTE *pFileName, const MMP_BYTE *pMode, MMP_ULONG *ulFileID);
extern MMP_ERR MMPF_FS_FClose(MMP_ULONG ulFileID);
extern void MMPF_MMU_InvalidateDCacheMVA(MMP_ULONG ulRegion, MMP_ULONG ulSize);
extern SD_UPDATER_ERR SDUpdateCfgWpFile(char ubFileName[], char ubFileExt[], MMP_BYTE ubWpFileNum);
extern MMP_ERR MMPD_System_GetFWEndAddr(MMP_ULONG *ulAddress);
extern MMP_BOOL MMPF_SF_IsProtect(void);
#if (TASK_MONITOR_ENABLE) //Prolong timeout due to writing flash.
extern void AHC_Main_Task_ReSetMonitorTime(UINT32 ulTime);
#endif
/*===========================================================================
 * Main body
 *===========================================================================*/

#if 0
void __FW_UPATER__(){}
#endif

//------------------------------------------------------------------------------
//  Function    : SDUpdateDrawProgress
//  Description :
//------------------------------------------------------------------------------
/**
 * @brief                 	Draw progress bar when it writes to serial flash.
 *
 * @param[in ] current		Byte number had read
 * @param[in ] total		Total bytes
 *
 * @retval void
 * @retval others
 */
void SDUpdateDrawProgress(int current, int total, UINT8* Text, int tclr, int fclr, int bclr)
{
#if (SDUPDATEDRAWPROGRESS == 1)
	UINT8			bID0;
    UINT8			bID1;
    UINT16			MaxBarLine = 135;
    UINT16			MaxBarLineHeigt = 10;
    UINT16			BarPos;
    UINT16			BarLineStartX = 90;
    UINT16			BarLineStartY = 110;
    UINT16			TxtOffsetX = 9;
    UINT16			TxtOffsetY = 30;
    const GUI_FONT  *pGuiFont = &GUI_Font20B_ASCII;
	AHC_DISPLAY_OUTPUTPANEL  OutputDevice;
#if TVOUT_PREVIEW
    MMPS_3GPRECD_PRESET_CFG *pVideoConfig = MMPS_3GPRECD_GetConfig();
#endif

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return; }

    OSDDraw_EnterDrawing(&bID0, &bID1);
    BEGIN_LAYER(bID0);

    AHC_GetDisplayOutputDev(&OutputDevice);

	switch (OutputDevice) {
#if TVOUT_PREVIEW
    case MMP_DISPLAY_SEL_NTSC_TV:
        TxtOffsetX = 9;
        TxtOffsetY = 40;
        MaxBarLine = 135 << 1;
        MaxBarLineHeigt = 30;

        BarLineStartX = ( pVideoConfig->previewdata[0].usVidDisplayW[VIDRECD_NTSC_PREVIEW_MODE] - MaxBarLine) >> 1;
        BarLineStartY = ( pVideoConfig->previewdata[0].usVidDisplayH[VIDRECD_NTSC_PREVIEW_MODE] - 60) >> 1;
        pGuiFont = &GUI_Font32B_ASCII;
        break;

    case MMP_DISPLAY_SEL_PAL_TV:
        TxtOffsetX = 9;
        TxtOffsetY = 40;
        MaxBarLine = 135 << 1;
        MaxBarLineHeigt = 30;
        BarLineStartX = ( pVideoConfig->previewdata[0].usVidDisplayW[VIDRECD_PAL_PREVIEW_MODE] - MaxBarLine) >> 1;
        BarLineStartY = ( pVideoConfig->previewdata[0].usVidDisplayH[VIDRECD_PAL_PREVIEW_MODE] - 60) >> 1;
        pGuiFont = &GUI_Font32B_ASCII;
        break;
#endif

#if HDMI_PREVIEW
    case MMP_DISPLAY_SEL_HDMI:
        // TBD
        break;
#endif
    //case MMP_DISPLAY_SEL_MAIN_LCD:
    default:
        MaxBarLine = 135;
        MaxBarLineHeigt = 10;
        BarLineStartX = (AHC_GET_ATTR_OSD_W(bID0) - MaxBarLine) >> 1;
        BarLineStartY = (AHC_GET_ATTR_OSD_H(bID0) - 20) >> 1;
        pGuiFont = &GUI_Font20B_ASCII;
        break;
	}

	AHC_OSDSetBkColor(bID0, OSD_COLOR_BLACK);

    //Draw Text
    AHC_OSDSetColor(bID0, tclr);
	AHC_OSDSetFont(bID0, pGuiFont);
    AHC_OSDDrawText(bID0, (UINT8*)Text, BarLineStartX-TxtOffsetX, BarLineStartY-TxtOffsetY, 1);
	//Draw Rectangle
    AHC_OSDSetColor(bID0, fclr);
    AHC_OSDSetPenSize(bID0, 1);
    AHC_OSDDrawLine(bID0, BarLineStartX,            BarLineStartX+MaxBarLine,   BarLineStartY,                  BarLineStartY);
    AHC_OSDDrawLine(bID0, BarLineStartX,            BarLineStartX+MaxBarLine,   BarLineStartY+MaxBarLineHeigt,  BarLineStartY+MaxBarLineHeigt);
    AHC_OSDDrawLine(bID0, BarLineStartX,            BarLineStartX,              BarLineStartY,                  BarLineStartY+MaxBarLineHeigt);
    AHC_OSDDrawLine(bID0, BarLineStartX+MaxBarLine, BarLineStartX+MaxBarLine,   BarLineStartY,                  BarLineStartY+MaxBarLineHeigt);

    //Draw Progress Bar
    AHC_OSDSetColor(bID0, bclr);
    BarPos = (((UINT64) MaxBarLine-2) * current) / total;
    AHC_OSDDrawFillRect(bID0, BarLineStartX+1, BarLineStartY+1, (BarLineStartX+1)+BarPos, BarLineStartY+MaxBarLineHeigt-1);

    OSDDraw_ExitDrawing(&bID0, &bID1);

	END_LAYER(bID0);
#endif  // SDUPDATEDRAWPROGRESS
}

static void SDUpdateDrawProgressEX(int current, int total, UINT8* Text, int tclr, int fclr, int bclr, UINT8* Text2, UINT8* Text3)
{

    UINT8			bID0 = 0;
    UINT8			bID1 = 0;
    UINT16			MaxBarLine = 280;
    UINT16          MaxBarHeight  = 30;
    UINT16			BarPos;
    UINT16			BarLineStartX = 20;
    UINT16			BarLineStartY = 110;
    RECT			Text_1 = {0, 60, 320,30};
    RECT			Text_2 = {0, 160, 320,30};
    RECT			Text_3 = {0, 190, 320,30};

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return; }

    OSDDraw_EnterDrawing(&bID0, &bID1);
    BEGIN_LAYER(bID0);

    //Draw Text
    AHC_OSDSetColor(bID0, tclr);
    AHC_OSDSetFont(bID0, &GUI_Font20B_ASCII);

    AHC_OSDDrawTextInRectEx(    bID0,
			                    (UINT8*)Text,
                                Text_1.uiLeft,
                                Text_1.uiTop,
                                Text_1.uiLeft + Text_1.uiWidth,
                                Text_1.uiTop + Text_1.uiHeight,
                                GUI_TA_HCENTER | GUI_TA_VCENTER,
                                strlen((char*)Text),
                                AHC_OSD_ROTATE_NONE  );

    AHC_OSDDrawTextInRectEx(    bID0,
			                    (UINT8*)Text2,
                                Text_2.uiLeft,
                                Text_2.uiTop,
                                Text_2.uiLeft + Text_2.uiWidth,
                                Text_2.uiTop + Text_2.uiHeight,
                                GUI_TA_HCENTER | GUI_TA_VCENTER,
                                strlen((char*)Text2),
                                AHC_OSD_ROTATE_NONE  );

    AHC_OSDDrawTextInRectEx(    bID0,
			                    (UINT8*)Text3,
                                Text_3.uiLeft,
                                Text_3.uiTop,
                                Text_3.uiLeft + Text_3.uiWidth,
                                Text_3.uiTop + Text_3.uiHeight,
                                GUI_TA_HCENTER | GUI_TA_VCENTER,
                                strlen((char*)Text3),
                                AHC_OSD_ROTATE_NONE  );


	//Draw Rectangle
    AHC_OSDSetColor(bID0, fclr);
    AHC_OSDSetPenSize(bID0, 1);

    AHC_OSDDrawLine(bID0, BarLineStartX, BarLineStartX+MaxBarLine, BarLineStartY, BarLineStartY);
    AHC_OSDDrawLine(bID0, BarLineStartX, BarLineStartX+MaxBarLine, BarLineStartY+MaxBarHeight, BarLineStartY+MaxBarHeight);
    AHC_OSDDrawLine(bID0, BarLineStartX, BarLineStartX, BarLineStartY, BarLineStartY+MaxBarHeight);
    AHC_OSDDrawLine(bID0, BarLineStartX+MaxBarLine, BarLineStartX+MaxBarLine, BarLineStartY, BarLineStartY+MaxBarHeight);

    //Draw Progress Bar
    AHC_OSDSetColor(bID0, bclr);

    BarPos = ((MaxBarLine-2) * current) / total;
    if(BarPos > (MaxBarLine-2))
    BarPos = (MaxBarLine-2);

    AHC_OSDDrawFillRect(bID0, BarLineStartX+1, BarLineStartY+1, (BarLineStartX+1)+BarPos, BarLineStartY+MaxBarHeight -1);
    OSDDraw_ExitDrawing(&bID0, &bID1);
	END_LAYER(bID0);
}

#if SD_FW_UPDATER
static void SDUpdateShowFWInfo(UINT32 uiAddr)
{
	    AIT_STORAGE_INDEX_TABLE* pIndexTable = (AIT_STORAGE_INDEX_TABLE *)uiAddr;

    UINT32 i;

    for(i = 0; i < MAX_STORAGE_NUM; i++){

        if(pIndexTable->it[i].ulStartSec != 0xFFFFFFFF){
    		RTNA_DBG_Str(0,"PART : ");
    		RTNA_DBG_Long(0, i);
    		RTNA_DBG_Str(0,", \t");
    		RTNA_DBG_Str(0,"Start Address : ");
    		RTNA_DBG_Long(0,(pIndexTable->it[i].ulStartSec<<9));
    		RTNA_DBG_Str(0,", \t");
    		RTNA_DBG_Str(0,"Size : ");
    		RTNA_DBG_Long(0,(pIndexTable->it[i].ulSecSize<<9));
    		RTNA_DBG_Str(0,"\r\n");
		}
    }
}
#endif

#if SD_FW_UPDATER
static void SDUpdateOpenBackupFile(void)
{
    AHC_FS_FileOpen("SD:\\sif.bin", STRLEN("SD:\\sif.bin"),"wb", sizeof("wb"),&m_BackupFileID);

}

static void SDUpdateWriteBackupFile(MMP_ULONG uiSeekPos, MMP_ULONG uiAddr, MMP_ULONG uiSize)
{
    MMP_ULONG uiWriteSize;

    AHC_FS_FileWrite(m_BackupFileID, (UINT8*)uiAddr, uiSize, &uiWriteSize);

}

static void SDUpdateCloseBackupFile(void)
{
    AHC_FS_FileClose(m_BackupFileID);

}
#endif

#if defined(SD_FW_DUMP_BIN_NORFLASH)&&(SD_FW_DUMP_BIN_NORFLASH)	
static AHC_ERR SDUpdateOpenDumpFile(void)
{
    return AHC_FS_FileOpen(SD_FW_DUMP_BIN_NAME, STRLEN(SD_FW_DUMP_BIN_NAME),"wb", sizeof("wb"),&m_DumpSpiFileID);
}

static void SDUpdateWriteDumpFile(MMP_ULONG uiSeekPos, MMP_ULONG uiAddr, MMP_ULONG uiSize)
{
    MMP_ULONG uiWriteSize;

    AHC_FS_FileWrite(m_DumpSpiFileID, (UINT8*)uiAddr, uiSize, &uiWriteSize);
}

static void SDUpdateCloseDumpFile(void)
{
    AHC_FS_FileClose(m_DumpSpiFileID);
}
#endif

#if SD_FW_UPDATER
static SD_UPDATER_ERR SDUpdaterLoadIdxTableFromSD(MMP_ULONG uiFileID, MMP_ULONG uiAddr, IDX_TABLE_TYPE eType)
{

    /**
    The information of bin file.
    */
    MMP_ERR eError;
    MMP_ULONG  uiReadCount, uiFilePos;

    uiFilePos = sizeof(SDBIN)+MBR_OFFSET;

	AHC_FS_FileSeek(uiFileID, uiFilePos, 0);

    #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
	RTNA_DBG_Str(0,"SD File Pos : ");
	RTNA_DBG_Long(0, uiFilePos);
	RTNA_DBG_Str(0,"\r\n");
    #endif

	eError = MMPS_FS_FileRead(uiFileID, (MMP_UBYTE*)uiAddr, SIF_MBR_SIZE, &uiReadCount);


    return SD_UPDATER_ERR_NONE;
}
#endif

#if SD_FW_UPDATER
static SD_UPDATER_ERR SDUpdaterLoadIdxTableFromSIF(MMP_ULONG uiAddr, IDX_TABLE_TYPE eType)
{
    switch(eType){

    case IDX_TABLE_TYPE_FIRST:
        {
        MMPF_MMU_FlushDCache();

        MMPF_SF_ReadData(SIF_MBR_ADDRESS, (MMP_UBYTE*)uiAddr, SIF_MBR_SIZE);
        }
    break;
    default:
    break;

    }

    return SD_UPDATER_ERR_NONE;
}
#endif

#if SD_FW_UPDATER
static SD_UPDATER_ERR SDUpdaterBurnSIF(MMP_ULONG uiSifAddr, MMP_ULONG uiDataAddr, MMP_ULONG uiSize, MMP_ULONG uiSectorSize)
{
    MMP_UBYTE* pbyNonCacheableBuf;
    MMP_ULONG uiTime1, uiTime2;
    MMP_ULONG uiSifSize, uiWriteSize;

    if(uiSifAddr % uiSectorSize == 0){
        MMPF_SF_EraseSector(uiSifAddr);
    }

    //MMPF_MMU_FlushDCacheMVA(uiDataAddr, uiSize);
    MMPF_MMU_FlushDCache();

    //pbyNonCacheableBuf = TO_NONCACHE( uiDataAddr );
    //MEMCPY(pbyNonCacheableBuf, (MMP_UBYTE*)uiDataAddr, uiSize);

    MMPF_OS_GetTime(&uiTime1);
    #if 0
    MMPF_SF_WriteData((MMP_ULONG)uiSifAddr, (MMP_ULONG)uiDataAddr, (MMP_ULONG)uiSize);
    #else

    uiWriteSize = 0;

    while(uiSize > 0){

        if(uiSize / 0x100 > 0){
            uiSifSize = 0x100;
        }else{
            uiSifSize = uiSize % 0x100;
        }
        //MMPF_SF_WriteData((MMP_ULONG)uiSifAddr, (MMP_ULONG)pbyNonCacheableBuf, (MMP_ULONG)uiSize);
        MMPF_SF_WriteData((MMP_ULONG)uiSifAddr + uiWriteSize, (MMP_ULONG)uiDataAddr + uiWriteSize, (MMP_ULONG)uiSifSize);

        uiWriteSize += uiSifSize;

        uiSize -= uiSifSize;

    }
    #endif
    MMPF_OS_GetTime(&uiTime2);

    //printc("uiSifAddr : 0x%x, Time : %d \n", uiSifAddr, (uiTime2 - uiTime1));

    if((uiTime2 - uiTime1) > 100){

        printc("uiSifAddr : 0x%x, Time : %d \n", uiSifAddr, (uiTime2 - uiTime1));
        return 0;
    }
}
#endif
#if SD_FW_UPDATER
static void SDUpdaterBurnSIFFromSD(MMP_ULONG uiFileID,
                            MMP_ULONG uiSeekPos,
                            MMP_ULONG uiFileSize,
                            MMP_ULONG uiSIFAddr,
                            MMP_ULONG uiSectorAlign,
                            MMP_ULONG uiDataAddr,
                            MMP_ULONG uiDataBufSize )
{
	MMP_ERR     eError;
    MMP_ULONG   uiLoadSize;
	MMP_ULONG   uiPrevSector;
	MMP_ULONG   uiCurSector;
	MMP_ULONG   uiReadSize;
    MMP_ULONG   uiTotalFileSize;
	MMP_ULONG   uiDrawLoop = 0;

	MMP_BOOL    bNeedEraseSector = MMP_FALSE;
    #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
	printc("uiFileID        : %x \n", uiFileID);
	printc("uiSeekPos       : %x \n", uiSeekPos);
	printc("uiFileSize      : %x \n", uiFileSize);
	printc("uiSIFAddr       : %x \n", uiSIFAddr);
	printc("uiSectorAlign   : %x \n", uiSectorAlign);
	printc("uiDataAddr      : %x \n", uiDataAddr);
	printc("uiDataBufSize   : %x \n", uiDataBufSize);
    #endif
	AHC_FS_FileGetSize(uiFileID, &uiTotalFileSize);

	uiPrevSector = uiSIFAddr/uiSectorAlign;

	if((uiSectorAlign - (uiSIFAddr % uiSectorAlign)) % uiDataBufSize != 0){
	    bNeedEraseSector = MMP_TRUE;
	    #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
	    printc("Erase Sector manually \n");
	    #endif
	}

    MMPS_FS_FileSeek(uiFileID, uiSeekPos, 0);

    while(uiFileSize > 0){

#if (TASK_MONITOR_ENABLE) //Prolong timeout due to writing flash.
        MMP_ULONG ulOSCurTime;
        MMPF_OS_GetTime(&ulOSCurTime);
        AHC_Main_Task_ReSetMonitorTime(ulOSCurTime);
#endif

        if((uiFileSize / uiDataBufSize) != 0){
            uiLoadSize = uiDataBufSize;

        }else{
            uiLoadSize = uiFileSize;
        }

        if(bNeedEraseSector == MMP_TRUE){

            uiCurSector = (uiSIFAddr + uiLoadSize)/uiSectorAlign;

            if( uiCurSector != uiPrevSector){
                MMP_ULONG i;

                for(i = uiPrevSector + 1; i < uiCurSector + 1;i++)
                    MMPF_SF_EraseSector(i*uiSectorAlign);

                uiPrevSector = uiCurSector;
            }
        }

        eError = AHC_FS_FileRead(uiFileID, (MMP_UBYTE*)uiDataAddr, uiLoadSize, &uiReadSize);

        if (eError == AHC_ERR_NONE || eError == AHC_FS_ERR_EOF)
		{

    		SDUpdaterBurnSIF(uiSIFAddr, (MMP_ULONG)uiDataAddr, uiReadSize, uiSectorAlign);

    		MMPF_SF_ReadData(uiSIFAddr, uiDataAddr, uiReadSize);
    		#if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
    		SDUpdateWriteBackupFile(uiSeekPos, uiDataAddr, uiReadSize);
    		#endif
		}else{
		    printc("Write Failed \n");

		}


		uiSIFAddr += uiLoadSize;
		uiFileSize -= uiLoadSize;
        #if 0
        printc("uiFileSize : %x \n", uiFileSize);
        #else

        uiSeekPos += uiLoadSize;

        if(((uiSeekPos >> 16) != uiDrawLoop) && ((uiSeekPos >> 16) != (uiTotalFileSize>>16))){

            uiDrawLoop = uiSeekPos>>16;

            #if (MENU_STYLE == 1)
			SDUpdateDrawProgressEX2( uiDrawLoop,
									 uiTotalFileSize>>16,
									 IDS_DS_Data_Updating,
									 OSD_COLOR_WHITE,
									 OSD_COLOR_WHITE,
									 OSD_COLOR_TRANSPARENT,
									 IDS_DS_Data_Updating_2);

            #else
            SDUpdateDrawProgressEX( uiDrawLoop,
                                    uiTotalFileSize>>16,
                                (UINT8 *) "Data Updating",
                                OSD_COLOR_WHITE,
                                OSD_COLOR_WHITE,
                                OSD_COLOR_ORANGE,
                                (UINT8 *) "Don't Remove SD Card and",
                                (UINT8 *)"Charger");
            #endif
        }

        uiDrawLoop++;

		#endif
    }
}
#endif

#if SD_FW_UPDATER
static MMP_ULONG SDUpdaterGetIdxTablePos(IDX_TABLE_TYPE eType, AIT_STORAGE_INDEX_TABLE* pIndexTable)
{

    switch(eType){

    case IDX_TABLE_TYPE_FIRST:
        return SIF_MBR_ADDRESS;
    break;

    case IDX_TABLE_TYPE_SECOND:

        if(pIndexTable != NULL)
        {

            MMP_ULONG ulSecondTablePos;

            ulSecondTablePos = (pIndexTable->it[0].ulStartSec<<9) + (pIndexTable->it[0].ulSecSize<<9);

            #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
            printc("ulSecondTablePos : %x before \n", ulSecondTablePos);
            #endif
            ulSecondTablePos = MMPF_SF_FindSecondIdxTable(  (pIndexTable->it[0].ulStartSec<<9),
                                                            (pIndexTable->it[0].ulSecSize<<9));

            #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
            printc("ulSecondTablePos : %x after \n", ulSecondTablePos);
            #endif

            return ulSecondTablePos;
        }
    break;

    default:
        return SIF_MBR_ADDRESS;
    break;

    }

}
#endif


#if SD_FW_UPDATER

//------------------------------------------------------------------------------
//  Function    : SDUpdateReadBinFile
//  Description :
//------------------------------------------------------------------------------
/**
 * @brief                 	sRead bin file and write to serial flash.
 *
 * @param[in ] bFileName	The file id.
 *
 * @retval 0				Succeed.
 * @retval others			Fail.
 */

SD_UPDATER_ERR SDUpdateReadBinFile(MMP_ULONG SDFileId)
{
	SDBIN       sdbin;
	MMP_ULONG   uiFWSize;
	MMP_ULONG   SDFileSize, i;
	MMP_ULONG   uiFilePos;
	MMP_ULONG   p;
	MMP_ULONG   Status=0;
	MMP_ULONG   rBackLen;

	MMP_ERR     MMP_Status;
	MMP_ULONG   uiFWUpdatePos;
	MD5_CTX     mdContext;

	MMP_ULONG   uiBurnAddr;
	MMP_ULONG   uiLoadAddr;

    MMP_ULONG   uiLoadSize;

    MMP_ULONG   uiBaseAddr, uiBaseSize;

    MMP_BOOL    bRenewBootRegion = MMP_FALSE;//MMP_TRUE;//MMP_FALSE;
    MMP_ULONG   uiSize;
    MMP_ULONG   uiDataAddr, uiDataBufSize;

    MMP_ULONG   uiTime1, uiTime2;

    AIT_STORAGE_INDEX_TABLE* pIndexTable = (AIT_STORAGE_INDEX_TABLE *)m_uiIndexTable;

    MMPF_OS_GetTime(&uiTime1);

    #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
    SDUpdateOpenBackupFile();
    #endif

    AHC_SendAHLMessageEnable(AHC_FALSE);
	Status = AHC_FS_FileGetSize(SDFileId, &SDFileSize);

	uiFWSize   = SDFileSize - sizeof(SDBIN);

    #if 1

    #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
    RTNA_DBG_Str(0, "Dram Allocation \r\n");
    #endif

    MMPD_System_GetFWEndAddress(&uiDataAddr);
    uiDataAddr = ALIGN4K(uiDataAddr);
    #else

    #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
    RTNA_DBG_Str(0, "Compiled Allocation \r\n");
    #endif

    uiDataAddr = (MMP_ULONG)Read_Back;
    #endif

    uiDataBufSize = SD_UPDATER_TEMP_BUF_LENGTH;

	if(Status != AHC_ERR_NONE)
    	return SD_UPDATER_ERR_FILE;

	DPRINT("File Size=%x\n", SDFileSize);

	LedCtrl_LcdBackLight(AHC_TRUE);

	AHC_OSDSetActive(OVL_DISPLAY_BUFFER, 1);

	//Read file and accumulate MD5
	AHC_FS_FileRead(SDFileId, (UINT8*)&sdbin, sizeof(SDBIN), &rBackLen);

	printc("SD BIN Ver %04x\r\n", sdbin.sbVer);
	printc("SD BIN TAG %c%c%c%c\r\n", sdbin.sbTag[0], sdbin.sbTag[1], sdbin.sbTag[2], sdbin.sbTag[3]);

	if (sdbin.sbTag[0] != 'A' ||
		sdbin.sbTag[1] != 'I' ||
		sdbin.sbTag[2] != 'T' ||
		sdbin.sbTag[3] != 'S')
		return SD_UPDATER_ERR_FILE;

	AHC_OSDSetFont( &GUI_Font16B_1);

	#if(WIFI_PORT)
	{
		SDUpdateBackupNetUserConf();
		
		#if(SD_MAC_UPDATER_BURNIN)
		//Check SD_MAC_FILENAME first
		if(!SDUpdateBackupMAC(SD_MAC_FILENAME)){
			//if SD_MAC_FILENAME doesn't exist ,Check SF_MAC_FILENAME in SIF
			SDUpdateBackupMAC(SF_MAC_FILENAME);
		}
		#endif
	}
	#endif
    /**
    Start Updating FW.
    */
	p = 0xffff;

	#if (MENU_STYLE == 1)
	SDUpdateDrawProgressEX2( 1,
							 1,
							IDS_DS_Data_Updating,
							OSD_COLOR_WHITE,
							OSD_COLOR_WHITE,
							OSD_COLOR_TRANSPARENT,
							IDS_DS_Data_Updating_2);
	//AHC_OSDDrawText_Transparent(0, (UINT8*)"-2", 200, 61, strlen("-2"));

	#else

	SDUpdateDrawProgressEX( 1,
	                        1,
	                        (UINT8 *) "Prepare to Update Data",
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_BLACK,
                            (UINT8 *) "Please Wait...",
                            NULL);

	SDUpdateDrawProgressEX( 1,
	                        1,
	                        (UINT8 *) "Data Updating...",
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_BLACK,
                            (UINT8 *) "Don't Remove SD Card and ",
                            (UINT8 *)"Charger");
    #endif


	/*************************************

	            Write FW

	**************************************/
    printc("\nWrite FW...\n");

	#if ( MENU_STYLE == 1)
	SDUpdateDrawProgressEX2( 0,
							 1,
							 IDS_DS_Data_Updating,
							 OSD_COLOR_WHITE,
							 OSD_COLOR_WHITE,
							 OSD_COLOR_TRANSPARENT,
							 IDS_DS_Data_Updating_2);
	//AHC_OSDDrawText_Transparent(0, (UINT8*)"-2", 200, 61, strlen("-2"));

	#else
    SDUpdateDrawProgressEX( 0 ,
                            1,
                            (UINT8 *) "Data Updating",
                            OSD_COLOR_WHITE,
                            OSD_COLOR_WHITE,
                            OSD_COLOR_ORANGE,
                            (UINT8 *) "Don't Remove SD Card and",
                            (UINT8 *)"Charger");
    #endif

    /**
    Read the first index table and get the position of second index table

    */
    SDUpdaterLoadIdxTableFromSIF((MMP_ULONG)pIndexTable, IDX_TABLE_TYPE_FIRST);

    uiBaseAddr = pIndexTable->it[0].ulStartSec;
    uiBaseSize = pIndexTable->it[0].ulSecSize;

    uiFWUpdatePos = SDUpdaterGetIdxTablePos(IDX_TABLE_TYPE_SECOND, pIndexTable);

    /**
    The information of bin file.
    */
    SDUpdaterLoadIdxTableFromSD(SDFileId, (MMP_ULONG)pIndexTable, 0);

    #if 1//(SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
    RTNA_DBG_Str(0,"SD Index \r\n");
    SDUpdateShowFWInfo((MMP_ULONG)pIndexTable);
    #endif


    /**
    seek the position the second fw
    */
    uiFilePos = sizeof(SDBIN) + MBR_OFFSET + SIF_MBR_SIZE + (pIndexTable->it[0].ulSecSize<<9);

    MMPS_FS_FileSeek(SDFileId, uiFilePos, 0);

    if(bRenewBootRegion == MMP_TRUE){
        /**
        renew bootcode, first index table and bootloader.
        */

        /*
        renew the position of base addr.
        */
        uiBaseAddr = pIndexTable->it[0].ulStartSec;
        uiBaseSize = pIndexTable->it[0].ulSecSize;
        /**
        renew the position of second index table.
        */

        uiFWUpdatePos = SDUpdaterGetIdxTablePos(IDX_TABLE_TYPE_SECOND, pIndexTable);

        /**
        burn bootcode
        */
        uiSize = SIF_MBR_ADDRESS;
        uiBurnAddr = 0x0;
        MMPS_FS_FileSeek(SDFileId, sizeof(SDBIN), 0);

        RTNA_DBG_Str(0, "Burn Bootcode \r\n");

        SDUpdaterBurnSIFFromSD(SDFileId,
                            sizeof(SDBIN),
                            uiSize,
                            uiBurnAddr,
                            0x1000,
                            uiDataAddr,
                            uiDataBufSize );

        uiBurnAddr += uiSize;

        /**
        clear the index table
        */
        RTNA_DBG_Str(0, "Clear 1st Table \r\n");
        MMPF_SF_EraseSector(SIF_MBR_ADDRESS);


        /**
        burn bootloader
        */
        uiSize      = (pIndexTable->it[0].ulSecSize<<9);
        uiBurnAddr  = (pIndexTable->it[0].ulStartSec<<9);

        MMPS_FS_FileSeek(SDFileId, sizeof(SDBIN) + uiBurnAddr, 0);

        #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
        RTNA_DBG_Str(0, "Burn BootLoader \t");
        RTNA_DBG_Str(0, "Burn Addr :");
        RTNA_DBG_Long(0, uiBurnAddr);
        RTNA_DBG_Str(0, "\r\n");
        #endif

        SDUpdaterBurnSIFFromSD(SDFileId,
                            sizeof(SDBIN) + uiBurnAddr,
                            uiSize,
                            uiBurnAddr,
                            0x1000,
                            uiDataAddr,
                            uiDataBufSize );

        uiBurnAddr += uiSize;

    }

    /**
    Clear Second Index Table
    */
    MMPF_SF_EraseSector(uiFWUpdatePos);

    pIndexTable->it[0].ulStartSec = uiBaseAddr;
    pIndexTable->it[0].ulSecSize = uiBaseSize;


    uiFWUpdatePos += SIF_MBR_SIZE;


    for(i = 1; i < MAX_STORAGE_NUM; i++){

        if(pIndexTable->it[i].ulStartSec != 0xFFFFFFFF){

            if(i == FW_USER_DATA_INDEX){
                uiSize        = (pIndexTable->it[i].ulSecSize<<9);
                uiFWUpdatePos = (pIndexTable->it[i].ulStartSec << 9);

                uiBurnAddr  = uiFWUpdatePos;//(pIndexTable->it[i].ulStartSec<<9);
            }else{
                uiSize      = (pIndexTable->it[i].ulSecSize<<9);
                uiBurnAddr  = uiFWUpdatePos;//(pIndexTable->it[i].ulStartSec<<9);
            }

            //uiFWUpdatePos = ALIGN4K(uiFWUpdatePos);
            #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
            RTNA_DBG_Str(0, "PART :");
            RTNA_DBG_Long(0, i);
            RTNA_DBG_Str(0, ",\t");

            RTNA_DBG_Str(0, "Burn Addr :");
            RTNA_DBG_Long(0, uiBurnAddr);
            RTNA_DBG_Str(0, ",\t");

            RTNA_DBG_Str(0, "Burn Size :");
            RTNA_DBG_Long(0, uiSize);
            RTNA_DBG_Str(0, "\r\n");
            #endif

            SDUpdaterBurnSIFFromSD(SDFileId,
                            sizeof(SDBIN) + (pIndexTable->it[i].ulStartSec<<9),
                            uiSize,
                            uiBurnAddr,
                            0x1000,
                            uiDataAddr,
                            uiDataBufSize );

            pIndexTable->it[i].ulStartSec = (uiFWUpdatePos >> 9);
            uiFWUpdatePos += (pIndexTable->it[i].ulSecSize<<9);

        }

    }

    /**
    Second Index Table
    */
    MMPF_MMU_FlushDCache();

    if(bRenewBootRegion == MMP_TRUE){
        RTNA_DBG_Str(0, "Burn 1st Table \r\n");
        MMPF_SF_WriteData(SIF_MBR_ADDRESS, (MMP_ULONG)pIndexTable, 0x100);
        MMPF_SF_WriteData(SIF_MBR_ADDRESS+0x100, (MMP_ULONG)pIndexTable+0x100, 0x100);

        #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
        if(uiDataBufSize >= SIF_MBR_SIZE){
            AIT_STORAGE_INDEX_TABLE* pCheckIndexTable = (AIT_STORAGE_INDEX_TABLE*)uiDataAddr;

            MMPF_SF_FastReadData(SIF_MBR_ADDRESS, uiDataAddr, SIF_MBR_SIZE);

            RTNA_DBG_Str(0, "Show 1st Burn Idx Table \r\n");
            SDUpdateShowFWInfo((MMP_ULONG)pIndexTable);

        }
        #endif
    }

    uiFWUpdatePos = SDUpdaterGetIdxTablePos(IDX_TABLE_TYPE_SECOND, pIndexTable);

    RTNA_DBG_Str(0, "Burn 2nd Table \r\n");

    MMPF_SF_WriteData((MMP_ULONG)uiFWUpdatePos, (MMP_ULONG)pIndexTable, 0x100);
    MMPF_SF_WriteData((MMP_ULONG)uiFWUpdatePos+0x100, (MMP_ULONG)pIndexTable+0x100, 0x100);

    #if (SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
    if(uiDataBufSize >= SIF_MBR_SIZE){

        AIT_STORAGE_INDEX_TABLE* pCheckIndexTable = (AIT_STORAGE_INDEX_TABLE*)uiDataAddr;

        MMPF_MMU_FlushDCache();

        MMPF_SF_FastReadData(SIF_MBR_ADDRESS, uiDataAddr, SIF_MBR_SIZE);

        RTNA_DBG_Str(0, "Show 2nd Burn Idx Table \r\n");
        SDUpdateShowFWInfo((MMP_ULONG)pIndexTable);

    }
    #endif

	#if(WIFI_PORT)
	{
		int retry = 3;
		
		while((AHC_FALSE == AHC_RemountDevices(MEDIA_SETTING_FACFAT)) && retry--)
			AHC_OS_Sleep(100);

		SDUpdateWriteBackNetUserConf();
		#if(SD_MAC_UPDATER_BURNIN)
		SDUpdateBurnInMAC();
		#endif
	}
	#endif
	
    #if 1//(SD_UPDATER_DEBUG >= SD_UPDATER_DEBUG_LEVEL1)
    RTNA_DBG_Str(0, "Show Idx Table \r\n");
    SDUpdateShowFWInfo((MMP_ULONG)pIndexTable);
    #endif

    RTNA_DBG_Str(0, "Finish Burn \r\n");
    MMPF_OS_GetTime(&uiTime2);

    printc("Time : %d \n", uiTime2 - uiTime1);

	/*
	OSD
	*/

	#if ( MENU_STYLE == 1)
	SDUpdateDrawProgressEX2( uiFWSize,
							 uiFWSize,
							 IDS_DS_Data_Updating,
							 OSD_COLOR_WHITE,
							 OSD_COLOR_WHITE,
							 OSD_COLOR_TRANSPARENT,
							 IDS_DS_Data_Updating_2);
	AHC_OS_Sleep(1000);

	#else

	SDUpdateDrawProgressEX(uiFWSize, uiFWSize, (UINT8 *) "Data Updating...",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, (UINT8 *) "Don't Remove SD Card and", (UINT8 *)"Charger");

	SDUpdateDrawProgressEX( uiFWSize, uiFWSize, (UINT8 *) "Write Done",
				OSD_COLOR_WHITE, OSD_COLOR_WHITE, OSD_COLOR_ORANGE,NULL, NULL);

	AHC_OS_Sleep(1000);
	SDUpdateDrawProgressEX( uiFWSize, uiFWSize, (UINT8 *) "Write Done",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
	#endif

    AHC_SendAHLMessageEnable(AHC_TRUE);
	return SD_UPDATER_ERR_NONE;

}

#else

//__align(4) unsigned char		Read_Back[0x100];
//#define STORAGE_TEMP_BUFFER 0x106000

SD_UPDATER_ERR SDUpdateReadBinFile(UINT32	SDFileId)
{
    SDBIN				sdbin;
    PROJID				sProjID;
    UINT32				fwcb;
    UINT32				SDFileSize, i;
    UINT32				p;
    AHC_ERR				Status=0;
    UINT32				ulBlkSz, rBackLen, ulFwEndAdr;
    MMP_ERR				MMP_Status;
    MD5_CTX				mdContext;
    UINT8               byDisplayID;
    MMPF_SIF_INFO       *info;
    UINT16				uwNeedWrDataCnt, uwDataWrSlot;
    UINT8               ubPaddingParti = 0, ubWpParti = 0;
    UINT8               ubPadPartiSts = PADDING_NONE;
    UINT32              ulPaddingSt = 0, ulPaddingEd = 0;
    UINT32              ulWpAreaSt = 0, ulWpAreaEd = 0;
    UINT8               *pBuf;
    /* for partial update */
    UINT32		t;
    MMP_ULONG	ulSifAddr = 0x0;
    MMP_ULONG	headerentry[2]={0};
    AIT_STORAGE_INDEX_TABLE2 *pIndexsif = (AIT_STORAGE_INDEX_TABLE2 *)STORAGE_TEMP_BUFFER;
    MMP_ULONG ulOSDInitTimeout = 20;

    MMPD_System_GetFWEndAddr(&ulFwEndAdr);
    ulFwEndAdr = ALIGN_X(ulFwEndAdr, 32);
    pBuf = (UINT8 *)ulFwEndAdr;
    
    //Wait until OSD init done.
    while((AHC_FALSE == AIHC_GetOSDInitStatus()) && (--ulOSDInitTimeout > 0)){
        printc("Wait OSDInit done...\r\n");
        MMPF_OS_SleepMs(100);
    }
    
    if(ulOSDInitTimeout == 0){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return SD_UPDATER_ERR_FILE; }
    
    
    MMPF_MMU_FlushDCacheMVA((MMP_ULONG)pIndexsif, 512);
    MMPF_SF_FastReadData(ulSifAddr, STORAGE_TEMP_BUFFER, 512);

	//Get file size
    Status=AHC_FS_FileGetSize(SDFileId, &SDFileSize);

    if(Status != AHC_ERR_NONE)
        return SD_UPDATER_ERR_FILE;

    printc("File Size=0x%x = %d\n", SDFileSize, SDFileSize);
    fwcb   = SDFileSize - sizeof(SDBIN);
    printc("fwcb=0x%x = %d    sizeof(SDBIN)=0x%x = %d\n", fwcb, fwcb, sizeof(SDBIN), sizeof(SDBIN));

#if (HDMI_PREVIEW)
    if (AHC_IsHdmiConnect())
    {
        byDisplayID = HDMIFunc_GetUImodeOsdID();
    }
    else
#endif
#if (TVOUT_PREVIEW)
    if (AHC_IsTVConnectEx())
    {
        byDisplayID = TVFunc_GetUImodeOsdID();
    }
    else
#endif
    {
        LedCtrl_LcdBackLightLock(AHC_FALSE);
        LedCtrl_LcdBackLight(AHC_TRUE);
        LedCtrl_LcdBackLightLock(AHC_TRUE);
        byDisplayID = OVL_DISPLAY_BUFFER;
    }

	//Read file and accumulate MD5
	AHC_FS_FileSeek(SDFileId, 0, 0);
	AHC_FS_FileRead(SDFileId, (UINT8*)&sdbin, sizeof(SDBIN), &rBackLen);
	AHC_FS_FileRead(SDFileId, (UINT8*)&sProjID, sizeof(PROJID), &rBackLen);

	printc("SD BIN Ver %04x\r\n", sdbin.sbVer);
	printc("SD BIN TAG %c%c%c%c\r\n", sdbin.sbTag[0], sdbin.sbTag[1], sdbin.sbTag[2], sdbin.sbTag[3]);

	if (sdbin.sbTag[0] != 'A' ||
		sdbin.sbTag[1] != 'I' ||
		sdbin.sbTag[2] != 'T' ||
		sdbin.sbTag[3] != 'S') {
		return SD_UPDATER_ERR_FILE;
	}

	BEGIN_LAYER(byDisplayID);

	AHC_OSDSetActive(byDisplayID, 1);

	AHC_OSDSetFont(byDisplayID, &GUI_Font16B_1);

	END_LAYER(byDisplayID);

	MD5Init(&mdContext);
	MD5Update(&mdContext, (unsigned char*)PRECODE, STRLEN(PRECODE));
	p = 0xffff;

	printc("Check MD5\r\n");
	#if (DEBUG_UART_TO_FILE)
	gbBlockDebugWr = MMP_TRUE;
	#endif
	AHC_FS_FileSeek(SDFileId, sizeof(SDBIN), 0);

	for (i = 0; i < fwcb;)
	{
#if (TASK_MONITOR_ENABLE) //Prolong timeout due to writing flash.
		extern void AHC_Main_Task_ReSetMonitorTime(UINT32 ulTime);
        MMP_ULONG ulOSCurTime;
        MMPF_OS_GetTime(&ulOSCurTime);
        AHC_Main_Task_ReSetMonitorTime(ulOSCurTime);
#endif
		Status=AHC_FS_FileRead(SDFileId, &pBuf[0], 0x1000, &rBackLen);//Read file

		MD5Update(&mdContext, &pBuf[0], rBackLen);//accumulate MD5

		if(Status != AHC_ERR_NONE && Status != AHC_FS_ERR_EOF)
		{
			printc("File_Read_ERROR\n");

			BEGIN_LAYER(0);
			AHC_OSDSetColor(0, OSD_COLOR_RED);
			AHC_OSDSetBkColor(0, OSD_COLOR_BLACK);
			AHC_OSDDrawText( 0, (UINT8*)"File Read Error", 81, 65, 1);
			AHC_OSDDrawText( 0, (UINT8*)"Please Turn OFF", 81, 80, 1);
			END_LAYER(0);

			return SD_UPDATER_ERR_FILE;	// File Corrupt
		}

		i += rBackLen;

		if (p != i >> 16)
		{
			p = i >> 16;

            SDUpdateDrawProgress(   p,
                                    fwcb >> 16,
                                    (UINT8 *) "Read File",
                                    OSD_COLOR_GREEN,
                                    OSD_COLOR_LIGHTMAGENTA,
                                    OSD_COLOR_WHITE);
		}
	}

    SDUpdateDrawProgress(fwcb, fwcb, (UINT8 *) "Read File", OSD_COLOR_GREEN, OSD_COLOR_LIGHTMAGENTA, OSD_COLOR_WHITE);

	//Get final MD5
	MD5Final(&mdContext);

	//Check MD5
	for (i = 0; i < MD5_LENGTH; i++)
	{
		if(mdContext.digest[i] != sdbin.sbMD5[i])//MD5 Error
		{
            SDUpdateDrawProgress(   1,
                                    1,
                                    (UINT8 *)"FILE Corruption",
                                    OSD_COLOR_RED,
                                    OSD_COLOR_RED,
                                    OSD_COLOR_RED);
            printc("FILE Corruption\n");
            return SD_UPDATER_ERR_FILE;
		}
	}
    /**

    Start Updating FW.

    */
	//non_read_back = TO_NONCACHE( Read_Back );


    /**
    Write Bootcode.
    */
    AHC_FS_FileSeek(SDFileId, sizeof(SDBIN), 0);
	p = 0xffff;

	SDUpdateDrawProgress(   1,
	                        1,
	                        (UINT8 *) "Write Firmware",
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_BLACK);

	/**
	Write FW
	*/
	//fwcb -= (MBR_OFFSET);
	#if (CHECKSDFWHEADER == 1)
		fwcb -= (MBR_OFFSET);
	#else

	#endif

    // Check FW size
    info = MMPF_SF_GetSFInfo();

	printc("Write FW...\r\n");
	printc("fwcb : %d(0x%x)\r\n",fwcb, fwcb);
	printc("Bin SF Cfg sz : %d(0x%x)\r\n",info->ulSFTotalSize, info->ulSFTotalSize);
	printc("\nFW\n");
    
    if (fwcb > info->ulSFTotalSize) {
		printc(FG_RED("FW size exceed to Flash size\r\n"));
		SDUpdateDrawProgressEX(1, 1, (UINT8 *) "Write SF Error...",
		OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
		return SD_UPDATER_ERR_FAIL;
    }

	#if(WIFI_PORT)
	{
		SDUpdateBackupNetUserConf();
		
		#if(SD_MAC_UPDATER_BURNIN)
		//Check SD_MAC_FILENAME first
		if(!SDUpdateBackupMAC(SD_MAC_FILENAME)){
			//if SD_MAC_FILENAME doesn't exist ,Check SF_MAC_FILENAME in SIF
			SDUpdateBackupMAC(SF_MAC_FILENAME);
		}
		#endif
	}
	#endif
    MMPF_MMU_FlushDCacheMVA((MMP_ULONG)pIndexsif, 512);
    MMPF_SF_FastReadData(ulSifAddr, STORAGE_TEMP_BUFFER, 512);    

	if((pIndexsif->ulHeader == INDEX_TABLE_HEADER)&&
			(pIndexsif->ulTail == INDEX_TABLE_HEADER)&&
			(pIndexsif->ulFlag & 0x01))
	{
	    for (i=10; i<MAX_PARTI_NUM; i++) {
	    	if ((pIndexsif->it[i].ulFlag0 & PART_FLAG_PADDING)) {
	            if (!pIndexsif->it[i].ulSecSize) {
	                ubPadPartiSts = PADDING_NONE;
	                printc(FG_YELLOW("No Padding %d %d %d!\r\n"), pIndexsif->it[i].ulFlag0, pIndexsif->it[i].ulStartSec, pIndexsif->it[i].ulRealSize, pIndexsif->it[i].ulSecSize);
	            }                
	            else {
	                ubPaddingParti = i;
	                ubPadPartiSts = PADDING_HIT;
	    			ulBlkSz = info->ulSFBlockSize;
	                ulPaddingSt = ALIGN_X((pIndexsif->it[ubPaddingParti].ulStartSec<<9), ulBlkSz);
	                ulPaddingEd = ALIGN_X(((pIndexsif->it[ubPaddingParti].ulStartSec + pIndexsif->it[ubPaddingParti].ulSecSize)<<9 - ulBlkSz), ulBlkSz);
	                MMPF_SF_SetProtWpArea();
	                printc(FG_YELLOW("Code: 0 ~ 0x%x (Bytes)\r\n"), pIndexsif->bt.ulCodeSize);
	                printc(FG_YELLOW("Sif Padding Parti:0x%x st:0x%x (sz:0x%x)\r\n"), ubPaddingParti, pIndexsif->it[ubPaddingParti].ulStartSec<<9, pIndexsif->it[ubPaddingParti].ulSecSize<<9);
	                printc(FG_YELLOW("Sif pad range:0x%x ~ 0x%x blksz:0x%x\r\n"), ulPaddingSt, ulPaddingEd, ulBlkSz);
	            }
	        }
	        else if ((pIndexsif->it[i].ulFlag0 & PART_FLAG_WP)) {
	            ubWpParti = i;
	            ulWpAreaSt = (pIndexsif->it[ubWpParti].ulStartSec<<9);
	            ulWpAreaEd = (pIndexsif->it[ubWpParti].ulStartSec + pIndexsif->it[ubWpParti].ulSecSize)<<9;
	            printc(FG_YELLOW("Sif Wp Range:0x%x ~ 0x%x\r\n"), ulWpAreaSt, ulWpAreaEd);            
	            break;
	        }
	        else
	        {
	        	RTNA_DBG_Str0(FG_RED("Parti Flag is InValid"));
	            RTNA_DBG_Long0(i);
	            RTNA_DBG_Long0(pIndexsif->it[i].ulFlag0);
	            RTNA_DBG_Str0("\r\n");
	            break;
	        }
	    }
	}
	#if (CHECKSDFWHEADER == 1)
	//for (i = MBR_OFFSET ;i < SDFileSize; i += 0x100)//Write to flash
    i = MBR_OFFSET;
    fwcb = SDFileSize;
	#else
	//for (i = 0 ;i < fwcb; i += rBackLen)//Write to flash
	i = 0;
	#endif
    uwNeedWrDataCnt = 0;
	do
	{

#if (TASK_MONITOR_ENABLE) //Prolong timeout due to writing flash.
        MMP_ULONG ulOSCurTime;
        MMPF_OS_GetTime(&ulOSCurTime);
        AHC_Main_Task_ReSetMonitorTime(ulOSCurTime);
#endif
		//Deal With FW from AITBOOT, AITBOOTX, ALL.
   		MMP_Status = AHC_FS_FileRead(SDFileId, &pBuf[0], 0x1000, &rBackLen);
        uwNeedWrDataCnt = rBackLen;
        if (i == 0)
        {
        	for (t=0; t<2; t++)
	        	headerentry[t] = pBuf[0+4*t]|(pBuf[1+4*t]<<8)|(pBuf[2+4*t]<<16)|(pBuf[3+4*t]<<24);

        	if ( (headerentry[0] == INDEX_TABLE_HEADER) 		&&
        		 (headerentry[1] & 0x01)						&&
				 (pIndexsif->ulHeader == INDEX_TABLE_HEADER) 	&&
				 (pIndexsif->ulTail == INDEX_TABLE_HEADER)		&&
           		 (pIndexsif->ulFlag & 0x01)	)
        	{
        		headerentry[0] = pBuf[0+16*12]|(pBuf[1+16*12]<<8)|(pBuf[2+16*12]<<16)|(pBuf[3+16*12]<<24);
        		if ( (headerentry[0] == 0x0) && (pIndexsif->it[10].ulStartSec!=0x0) )
        		{
	        		printc("SDUpdate Text -- Without Userarea.\r\n");
    	    		t = 10;
        			while ((pIndexsif->it[t].ulStartSec!=0x0) && (16*(t+2)<256) )
        			{
						pBuf[0+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulStartSec&0xFF);
						pBuf[1+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulStartSec>>8)&0xFF);
						pBuf[2+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulStartSec>>16)&0xFF);
						pBuf[3+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulStartSec>>24)&0xFF);

						pBuf[4+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulSecSize&0xFF);
						pBuf[5+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulSecSize>>8)&0xFF);
						pBuf[6+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulSecSize>>16)&0xFF);
						pBuf[7+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulSecSize>>24)&0xFF);

						pBuf[8+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulFlag0&0xFF);
						pBuf[9+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulFlag0>>8)&0xFF);
						pBuf[10+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulFlag0>>16)&0xFF);
						pBuf[11+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulFlag0>>24)&0xFF);

						pBuf[12+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulRealSize&0xFF);
						pBuf[13+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulRealSize>>8)&0xFF);
						pBuf[14+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulRealSize>>16)&0xFF);
						pBuf[15+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulRealSize>>24)&0xFF);

        				t++;
        			}
        		}
        	}

        }

		if (MMP_Status == AHC_ERR_NONE || MMP_Status == AHC_FS_ERR_EOF)
		{
		    #if 1
			if(i % info->ulSFBlockSize == 0){
			    MMPF_SF_EraseBlock(i);
			    printc("erase addr: 0x%X\r", i);
			}
            #else  // Erase Chip
			if(i == 0){
			    MMPF_SF_EraseChip();
			    printc("i3: 0x%X\r", i);
			}
            #endif

			//MEMCPY(non_read_back, Read_Back, rBackLen);
			while (uwNeedWrDataCnt > 0) {
                uwDataWrSlot = 0x100;
                if (uwNeedWrDataCnt < uwDataWrSlot) {
                    uwDataWrSlot = uwNeedWrDataCnt;
                }
                if (ubPadPartiSts == PADDING_HIT && i >= ulPaddingSt) {
                    AHC_FS_FileSeek(SDFileId, (ulPaddingEd + sizeof(SDBIN)), 0);
                    i = ulPaddingEd;
                    ubPadPartiSts = PADDING_SKIP_DONE;
                    uwNeedWrDataCnt = 0;
                    printc(FG_YELLOW("pad %x\r\n"), i);
                    break;
                }
                else {
    			    MMPF_MMU_FlushDCacheMVA((MMP_ULONG)&pBuf[(rBackLen-uwNeedWrDataCnt)], (MMP_ULONG)uwDataWrSlot);
	    		    MMP_Status = MMPF_SF_WriteData((MMP_ULONG)i,(MMP_ULONG)&pBuf[(rBackLen-uwNeedWrDataCnt)],(MMP_ULONG)uwDataWrSlot);
                    
                    i += uwDataWrSlot;
                    uwNeedWrDataCnt -= uwDataWrSlot;                    
                }
            }
		}
        else {
		    printc(FG_RED("Write Failed \r\n"));
		}

		if (MMP_Status != MMP_ERR_NONE) {
			printc(FG_RED("SF_ERROR MMP_STATUS[%x] STATUS[%x]\r\n"),MMP_Status,Status);
			SDUpdateDrawProgressEX(1, 1, (UINT8 *) "Write SF Error...",
			OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
			return SD_UPDATER_ERR_FAIL;
		}
        
        if (MMPF_SF_IsProtect() && ubPadPartiSts == PADDING_SKIP_DONE) {
            i = fwcb;
            RTNA_DBG_Str0(FG_YELLOW("WpSkip!\r\n"));
        }
		if (p != i >> 16) {
			p = i >> 16;
            SDUpdateDrawProgress(   p,
                                    fwcb >> 16,
                                    (UINT8 *) "Write Firmware",
                                    OSD_COLOR_GREEN,
                                    OSD_COLOR_LIGHTMAGENTA,
                                    OSD_COLOR_WHITE);
		}
	} while (i < fwcb);

	#if(WIFI_PORT)
	{
		int retry = 3;
		
		while((AHC_FALSE == AHC_RemountDevices(MEDIA_SETTING_FACFAT)) && retry--)
			AHC_OS_Sleep(100);
		
		SDUpdateWriteBackNetUserConf();
		#if(SD_MAC_UPDATER_BURNIN)
		SDUpdateBurnInMAC();
		#endif
	}
	#endif
	
	/*
	OSD
	*/
	SDUpdateDrawProgress(p, fwcb >> 16, (UINT8 *) "Write Firmware",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);

	SDUpdateDrawProgress( fwcb, fwcb, (UINT8 *) "Write Done",
				OSD_COLOR_GREEN, OSD_COLOR_LIGHTMAGENTA, OSD_COLOR_WHITE);

	AHC_OS_Sleep(1000);
	SDUpdateDrawProgress( fwcb, fwcb, (UINT8 *) "Write Done",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);
	#if (DEBUG_UART_TO_FILE)
	gbBlockDebugWr = MMP_FALSE;
	#endif
	return SD_UPDATER_ERR_NONE;
}

#if (SD_FW_RAW_UPDATE == 1)
SD_UPDATER_ERR SDUpdateRawReadBinFile(UINT32	SDFileId)
{

	UINT32				SDFileSize, i;
	UINT32				p;
	AHC_ERR				Status=0;
	UINT32				rBackLen;
	unsigned char		Read_Back[256];
	unsigned char *		non_read_back;
	MMP_ERR				MMP_Status;

	//Get file size

	Status=AHC_FS_FileGetSize(SDFileId, &SDFileSize);

	if(Status != AHC_ERR_NONE)
    	return SD_UPDATER_ERR_FILE;

	DPRINT("File Size=%x\n", SDFileSize);


	LedCtrl_LcdBackLight(AHC_TRUE);

	AHC_OSDSetActive(OVL_DISPLAY_BUFFER, 1);

	AHC_OSDSetFont( &GUI_Font16B_1);

    /**

    Start Updating FW.

    */
	non_read_back = TO_NONCACHE( Read_Back );

	/**
	Write FW

	*/
	printc("\nFW\n");
	for (i = 0 ;i < SDFileSize; i += 0x100)//Write to flash
	//for (i = 0 ;i < fwcb; i += 0x100)//Write to flash
	{
        MMPF_SIF_INFO *info;

        info = MMPF_SF_GetSFInfo();

		//Deal With FW from AITBOOT, AITBOOTX, ALL.
		MMP_Status = AHC_FS_FileRead(SDFileId, Read_Back, sizeof(Read_Back), &rBackLen);

		if (MMP_Status == AHC_ERR_NONE || MMP_Status == AHC_FS_ERR_EOF)
		{
			if(i % info->ulSFSectorSize == 0){
			    //printc("MMPF_SF_EraseSector : %d \n", i);
			    MMPF_SF_EraseSector(i);
			}

			MEMCPY(non_read_back, Read_Back, rBackLen);

			MMP_Status = MMPF_SF_WriteData((MMP_ULONG)i,(MMP_ULONG)non_read_back,(MMP_ULONG)rBackLen);

		}else{
		    printc("Write Failed \n");

		}

		if (MMP_Status != MMP_ERR_NONE)
		{
			printc("SF_ERROR MMP_STATUS[%x] STATUS[%x]\r\n",MMP_Status,Status);
			SDUpdateDrawProgressEX(1, 1, (UINT8 *) "Write SF Error...",
			OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
			return SD_UPDATER_ERR_FAIL;
		}

		if (p != i >> 16)
		{
			p = i >> 16;


			SDUpdateDrawProgressEX2( p,
									 SDFileSize >> 16,
									 IDS_DS_Data_Updating,
									 OSD_COLOR_WHITE,
									 OSD_COLOR_WHITE,
									 OSD_COLOR_TRANSPARENT,
									 IDS_DS_Data_Updating_2);

		}
	}

	AHC_OS_Sleep(1000);
	return SD_UPDATER_ERR_NONE;

}
#endif
#endif

#if (EN_WP_AREA_CONFIG == 1)
SD_UPDATER_ERR SDUpdateWPBinFS(UINT32 SDFileId, MMP_BYTE *bSDFileName)
{
	AHC_ERR		MMP_Status;
	MMP_ULONG	SfFileId;
    MMP_ULONG	SrcFileSize, ulBackLen, ulFwEndAdr, i, p;
	MMP_ULONG	ulWriteSize = 0;
    UINT16		uwNeedWrDataCnt, uwDataWrSlot;
    MMP_BYTE    bSfFileName[64] = FS_4TH_DEV;
    MMP_BYTE    *pSDFullPath;
    UINT8       *pBuf, *pRdBack;
            
	//Get file size
    MMP_Status = AHC_FS_FileGetSize(SDFileId, &SrcFileSize);
    if(MMP_Status != AHC_ERR_NONE) {
        RTNA_DBG_Str0(FG_RED("[ERR] FS_FileGetSize "));
        RTNA_DBG_Str0(bSDFileName);
        RTNA_DBG_Str0(":");
        RTNA_DBG_Long0(MMP_Status);
        RTNA_DBG_Str0("\r\n");   
        return SD_UPDATER_ERR_FILE;
    }
    
    MMPD_System_GetFWEndAddr(&ulFwEndAdr);
    ulFwEndAdr = ALIGN_X(ulFwEndAdr, 32);
    pBuf = (UINT8 *)ulFwEndAdr;
    pRdBack = (UINT8 *)(ulFwEndAdr + ALIGN_X(SrcFileSize, 32));
    
    uwNeedWrDataCnt = 0;
    AHC_FS_FileSeek(SDFileId, 0, 0);

    pSDFullPath = STRTOK(bSDFileName, "\\");
    pSDFullPath = STRTOK(NULL, "");
    STRCAT(bSfFileName, pSDFullPath);

    RTNA_DBG_Str0("[Update]Dst ");
    RTNA_DBG_Str0(bSfFileName);
    RTNA_DBG_Str0("\r\n");
    
	MMP_Status = MMPF_FS_FOpen(bSfFileName, "wb", &SfFileId);
	if (MMP_Status != MMP_ERR_NONE)	{
        RTNA_DBG_Str0(FG_RED("[ERR] FS_FOpen "));
        RTNA_DBG_Str0(bSfFileName);
        RTNA_DBG_Str0(":");
        RTNA_DBG_Long0(MMP_Status);        
        RTNA_DBG_Str0("\r\n");
        return SD_UPDATER_ERR_FILE;
    }
	do
	{
		//Deal With Wp_Data.
        MMPF_MMU_InvalidateDCacheMVA((MMP_ULONG)pBuf, SrcFileSize);
   		MMP_Status = AHC_FS_FileRead(SDFileId, &pBuf[0], SrcFileSize, &ulBackLen);
        uwNeedWrDataCnt = ulBackLen;

		if (MMP_Status == AHC_ERR_NONE || MMP_Status == AHC_FS_ERR_EOF)
		{
			while (uwNeedWrDataCnt > 0) {
                uwDataWrSlot = 0x100;
                if (uwNeedWrDataCnt < uwDataWrSlot) {
                    uwDataWrSlot = uwNeedWrDataCnt;
                } 
    			MMPF_MMU_FlushDCacheMVA((MMP_ULONG)&pBuf[(ulBackLen-uwNeedWrDataCnt)], (MMP_ULONG)uwDataWrSlot);
	    		MMP_Status = MMPF_FS_FWrite(SfFileId, &pBuf[(ulBackLen-uwNeedWrDataCnt)], (MMP_ULONG)uwDataWrSlot, &ulWriteSize);
            	if (MMP_Status != MMP_ERR_NONE)	{
                    RTNA_DBG_Str0(FG_RED("[ERR] FS_FWrite "));
                    RTNA_DBG_Str0(bSfFileName);
                    RTNA_DBG_Str0(":");
                    RTNA_DBG_Long0(MMP_Status);
                    RTNA_DBG_Str0("\r\n");
                }
                else {
                    printc(FG_YELLOW("i:%x(%x)\r"), i, uwDataWrSlot);
                }
                i += uwDataWrSlot;
                uwNeedWrDataCnt -= uwDataWrSlot;
            }
		}
        else {
            RTNA_DBG_Str0(FG_RED("[ERR] AHC_FS_FileRead "));
            RTNA_DBG_Str0(bSDFileName);
            RTNA_DBG_Str0(":");
            RTNA_DBG_Long0(MMP_Status);
            RTNA_DBG_Str0("\r\n");
		}

		if (MMP_Status != MMP_ERR_NONE) {
			printc(FG_RED("SF_ERROR MMP_STATUS[%x] \r\n"),MMP_Status);
			SDUpdateDrawProgressEX(1, 1, (UINT8 *) "Write SF Error...",
			OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
			return SD_UPDATER_ERR_FAIL;
		}

		if (p != i >> 16) {
			p = i >> 16;
            SDUpdateDrawProgress(   p,
                                    SrcFileSize >> 16,
                                    (UINT8 *) "Write WP_Data",
                                    OSD_COLOR_GREEN,
                                    OSD_COLOR_LIGHTMAGENTA,
                                    OSD_COLOR_WHITE);
		}
	} while (i < SrcFileSize);
    MMPF_FS_FClose(SfFileId);

    //+++Read back
    MMP_Status = MMPF_FS_FOpen(bSfFileName, "rb", &SfFileId);
    if (MMP_Status == MMP_ERR_NONE) {
        MMP_ULONG j, ulSfBackLen;
    	for (j = 0 ; j<SrcFileSize; j++) {
            if(!(j%0x100)) {
                MMPF_MMU_InvalidateDCacheMVA((MMP_ULONG)pRdBack, 0x100);                    
                MMP_Status = MMPF_FS_FRead(SfFileId, &pRdBack[0], 0x100, &ulSfBackLen);             
            }
    	    if (pBuf[j] != pRdBack[j%0x100] ) {
    	        printc(FG_RED("No match SD:[%x]%x SF:[%x]%x\r\n"), j, pBuf[j], j%0x100, pRdBack[j%0x100]);
                MMPF_FS_FClose(SfFileId);
                return SD_UPDATER_ERR_FAIL;
    	    }
        }
        MMPF_FS_FClose(SfFileId);
        RTNA_DBG_Str0(FG_GREEN("Verify Pass!\r\n"));
    }
    //---Read back
    
	//OSD
	SDUpdateDrawProgress(p, SrcFileSize >> 16, (UINT8 *) "Write FS WP_Data Data",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);

	SDUpdateDrawProgress( SrcFileSize, SrcFileSize, (UINT8 *) "Write FS WP_Data Done",
				OSD_COLOR_GREEN, OSD_COLOR_LIGHTMAGENTA, OSD_COLOR_WHITE);

	AHC_OS_Sleep(1000);
	SDUpdateDrawProgress( SrcFileSize, SrcFileSize, (UINT8 *) "Write FS WP_Data Done",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);
	#if (DEBUG_UART_TO_FILE)
	gbBlockDebugWr = MMP_FALSE;
	#endif
	return SD_UPDATER_ERR_NONE;
}

SD_UPDATER_ERR SDUpdateWPBin(UINT32	SDFileId)
{
    UINT32				fwcb;
    UINT32				SDFileSize, i;
    UINT32				p;
    AHC_ERR				Status=0;
    UINT32				rBackLen, ulFwEndAdr;
    MMP_ERR				MMP_Status;
    UINT8               byDisplayID;
    MMPF_SIF_INFO       *info;
    UINT16				uwNeedWrDataCnt, uwDataWrSlot;
    UINT8               *pBuf;    
    /* for partial update */
    //UINT32		t;
    MMP_ULONG	ulSifAddr = 0x0;
    AIT_STORAGE_INDEX_TABLE2 *pIndexsif = (AIT_STORAGE_INDEX_TABLE2 *)STORAGE_TEMP_BUFFER;
    MMP_ULONG ulOSDInitTimeout = 20;
	MMP_ULONG ult1, ult2;
	
	
    MMPD_System_GetFWEndAddr(&ulFwEndAdr);
    ulFwEndAdr = ALIGN_X(ulFwEndAdr, 32);
    pBuf = (UINT8 *)ulFwEndAdr;
    
    //Wait until OSD init done.
    while((AHC_FALSE == AIHC_GetOSDInitStatus()) && (--ulOSDInitTimeout > 0)){
        printc("Wait OSDInit done...\r\n");
        MMPF_OS_SleepMs(100);
    }
    
    if(ulOSDInitTimeout == 0){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return SD_UPDATER_ERR_FILE; }

    MMPF_MMU_FlushDCacheMVA((MMP_ULONG)pIndexsif, 512);
    MMPF_SF_FastReadData(ulSifAddr, STORAGE_TEMP_BUFFER, 512);
    
	//Get file size
    Status=AHC_FS_FileGetSize(SDFileId, &SDFileSize);

    if(Status != AHC_ERR_NONE)
        return SD_UPDATER_ERR_FILE;

    printc(FG_YELLOW("Protect File Size=0x%x = %d\n"), SDFileSize, SDFileSize);
    fwcb   = SDFileSize;
    //printc(FG_YELLOW("fwcb=0x%x = %d    sizeof(FAT_HEAD)=0x%x = %d\n"), fwcb, fwcb, uwFATHeader, uwFATHeader);

#if (HDMI_PREVIEW)
    if (AHC_IsHdmiConnect())
    {
        byDisplayID = HDMIFunc_GetUImodeOsdID();
    }
    else
#endif
#if (TVOUT_PREVIEW)
    if (AHC_IsTVConnectEx())
    {
        byDisplayID = TVFunc_GetUImodeOsdID();
    }
    else
#endif
    {
        LedCtrl_LcdBackLightLock(AHC_FALSE);
        LedCtrl_LcdBackLight(AHC_TRUE);
        LedCtrl_LcdBackLightLock(AHC_TRUE);
        byDisplayID = OVL_DISPLAY_BUFFER;
    }

	BEGIN_LAYER(byDisplayID);

	AHC_OSDSetActive(byDisplayID, 1);

	AHC_OSDSetFont(byDisplayID, &GUI_Font16B_1);

	END_LAYER(byDisplayID);

    //Write Bootcode.
    //AHC_FS_FileSeek(SDFileId, uwFATHeader, 0);
	p = 0xffff;

	SDUpdateDrawProgress(   1,
	                        1,
	                        (UINT8 *) "Write WP_Data",
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_BLACK);

	printc(FG_YELLOW("Write WP_Data...\r\n"));
	printc(FG_YELLOW("SDFileSize : %d\r\n"),SDFileSize);
	printc(FG_YELLOW("\nWP\n"));

    // Check FW size
    info = MMPF_SF_GetSFInfo();
    for (i= 10; i<MAX_PARTI_NUM; i++) {
        if (pIndexsif->it[i].ulFlag0 & PART_FLAG_WP) {
            printc(FG_YELLOW("WpSizes=0x%x:%d(SEC)\r\n"), pIndexsif->it[i].ulSecSize, pIndexsif->it[i].ulSecSize);
            break;
        }
    }  
    if (SDFileSize > (pIndexsif->it[i].ulSecSize<<9)) {
		printc(FG_RED("WP_Data size exceed to Flash sizes\r\n"));
		SDUpdateDrawProgressEX(1, 1, (UINT8 *) "Write SF Error...",
		OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
		return SD_UPDATER_ERR_FAIL;
    }

	//#if(WIFI_PORT)
	//{
	//	SDUpdateBackupNetUserConf();
	//	
	//	#if(SD_MAC_UPDATER_BURNIN)
	//	//Check SD_MAC_FILENAME first
	//	if(!SDUpdateBackupMAC(SD_MAC_FILENAME)){
	//		//if SD_MAC_FILENAME doesn't exist ,Check SF_MAC_FILENAME in SIF
	//		SDUpdateBackupMAC(SF_MAC_FILENAME);
	//	}
	//	#endif
	//}
	//#endif

    i = info->ulSFTotalSize - (pIndexsif->it[i].ulSecSize<<9);
    uwNeedWrDataCnt = 0;
    AHC_FS_FileSeek(SDFileId, 0, 0);
    MMPF_OS_GetTime(&ult1);    
	do
	{
#if (TASK_MONITOR_ENABLE) //Prolong timeout due to writing flash.
        MMP_ULONG ulOSCurTime;
        MMPF_OS_GetTime(&ulOSCurTime);
        AHC_Main_Task_ReSetMonitorTime(ulOSCurTime);
#endif
		//Deal With Wp_Data.
        MMPF_MMU_InvalidateDCacheMVA((MMP_ULONG)pBuf, 0x100);
   		MMP_Status = AHC_FS_FileRead(SDFileId, &pBuf[0], 0x100, &rBackLen);
        uwNeedWrDataCnt = rBackLen;

		if (MMP_Status == AHC_ERR_NONE || MMP_Status == AHC_FS_ERR_EOF)
		{
		    #if 1
                #if 1 // EraseBlock
    			if(i % info->ulSFBlockSize == 0){
    			    MMPF_SF_EraseBlock(i);
    			    printc("i2: 0x%X\r\n", i);
    			}
                #else  // Erase Chip
    			if(i == 0){
    			    MMPF_SF_EraseChip();
    			    printc("i3: 0x%X\r", i);
    			}
                #endif
            #else
			if(i % info->ulSFSectorSize == 0){
			    //printc("MMPF_SF_EraseSector : %d \n", i);
			    MMPF_SF_EraseSector(i);
			    printc("i: 0x%X\r", i);
			}
            #endif

			//MEMCPY(non_read_back, Read_Back, rBackLen);
			while (uwNeedWrDataCnt > 0) {
                uwDataWrSlot = 0x100;
                if (uwNeedWrDataCnt < uwDataWrSlot) {
                    uwDataWrSlot = uwNeedWrDataCnt;
                } 
    			MMPF_MMU_FlushDCacheMVA((MMP_ULONG)&pBuf[(rBackLen-uwNeedWrDataCnt)], (MMP_ULONG)uwDataWrSlot);
	    		MMP_Status = MMPF_SF_WriteData((MMP_ULONG)i,(MMP_ULONG)&pBuf[(rBackLen-uwNeedWrDataCnt)],(MMP_ULONG)uwDataWrSlot);
                printc(FG_YELLOW("i:%x(%x)\r"), i, uwDataWrSlot);
                i += uwDataWrSlot;
                uwNeedWrDataCnt -= uwDataWrSlot;
            }            
		}
        else {
		    printc(FG_RED("Write WpData Failed \r\n"));
		}

		if (MMP_Status != MMP_ERR_NONE)
		{
			printc(FG_RED("SF_ERROR MMP_STATUS[%x] STATUS[%x]\r\n"),MMP_Status,Status);
			SDUpdateDrawProgressEX(1, 1, (UINT8 *) "Write SF Error...",
			OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
			return SD_UPDATER_ERR_FAIL;
		}

		if (p != i >> 16)
		{
			p = i >> 16;

            SDUpdateDrawProgress(   p,
                                    fwcb >> 16,
                                    (UINT8 *) "Write WP_Data",
                                    OSD_COLOR_GREEN,
                                    OSD_COLOR_LIGHTMAGENTA,
                                    OSD_COLOR_WHITE);
		}
	} while (i < info->ulSFTotalSize);
    RTNA_DBG_Str0("\r\n");    
    MMPF_OS_GetTime(&ult2);
    printc(FG_YELLOW("UpdateBinCost %d\r\n"), (ult2-ult1));
    
	//#if(WIFI_PORT)
	//{
	//	int retry = 3;
	//	
	//	while((AHC_FALSE == AHC_RemountDevices(MEDIA_SETTING_FACFAT)) && retry--)
	//		AHC_OS_Sleep(100);
	//	
	//	SDUpdateWriteBackNetUserConf();
	//	#if(SD_MAC_UPDATER_BURNIN)
	//	SDUpdateBurnInMAC();
	//	#endif
	//}
	//#endif

	//OSD
	SDUpdateDrawProgress(p, fwcb >> 16, (UINT8 *) "Write WP_Data Data",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);

	SDUpdateDrawProgress( fwcb, fwcb, (UINT8 *) "Write WP_Data Done",
				OSD_COLOR_GREEN, OSD_COLOR_LIGHTMAGENTA, OSD_COLOR_WHITE);

	AHC_OS_Sleep(1000);
	SDUpdateDrawProgress( fwcb, fwcb, (UINT8 *) "Write WP_Data Done",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);
	#if (DEBUG_UART_TO_FILE)
	gbBlockDebugWr = MMP_FALSE;
	#endif
	return SD_UPDATER_ERR_NONE;
}

//------------------------------------------------------------------------------
//  Function    : SDDumpWpAreaBak
//  Description :
//------------------------------------------------------------------------------
/**
 * @brief                 	Check WP that is exist or not.
 *
 * @param[in ] bFileName	The file name which you want to check.
 * @param[out] ulFileId		The file id.
 *
 * @retval MMP_ERR_NONE		Succeed.
 * @retval others			Fail
 */
SD_UPDATER_ERR SDDumpWpAreaBak(MMP_BYTE *bFileName)
{
	AHC_ERR		  Status;
	MMP_ULONG	  SDFileId;
	MMP_ULONG	  i,n;
    MMPF_SIF_INFO *info;
	AIT_STORAGE_INDEX_TABLE2 *pIndexsif = (AIT_STORAGE_INDEX_TABLE2 *)STORAGE_TEMP_BUFFER;
    MMP_ULONG     ulWriteCount, ulReadCount, ulFwEndAdr;
    UINT8         *pBuf;
    
    MMPD_System_GetFWEndAddr(&ulFwEndAdr);
    ulFwEndAdr = ALIGN_X(ulFwEndAdr, 32);
    pBuf = (UINT8 *)ulFwEndAdr;
    
	Status = AHC_FS_FileOpen(bFileName, STRLEN(bFileName),"wb", sizeof("wb"),&SDFileId);
    AHC_FS_FileSeek(SDFileId, 0, 0);
    
    if(Status == AHC_ERR_NONE)
    {
        info = MMPF_SF_GetSFInfo();
        ulReadCount = 0x100;
        
        MMPF_MMU_FlushDCacheMVA((MMP_ULONG)pIndexsif, 512);
        MMPF_SF_FastReadData(0, STORAGE_TEMP_BUFFER, 512);        
        for (n= 10; n<MAX_PARTI_NUM; n++) {
            if (pIndexsif->it[n].ulFlag0 & PART_FLAG_WP) {
                printc(FG_YELLOW("WpSizes=0x%x %d (SEC)\r\n"), pIndexsif->it[n].ulSecSize, pIndexsif->it[n].ulSecSize);
                break;
            }
        }
        i = info->ulSFTotalSize - (pIndexsif->it[n].ulSecSize<<9);
    	do
    	{
#if (TASK_MONITOR_ENABLE) //Prolong timeout due to writing flash.
            MMP_ULONG ulOSCurTime;
            MMPF_OS_GetTime(&ulOSCurTime);
            AHC_Main_Task_ReSetMonitorTime(ulOSCurTime);
#endif
            /* Invalid cache to make sure the data coherence */
            MMPF_MMU_InvalidateDCacheMVA((MMP_ULONG)pBuf, ulReadCount);
    	    MMPF_SF_ReadData(i, (MMP_ULONG)pBuf, ulReadCount);

            MMPF_MMU_FlushDCacheMVA((MMP_ULONG)pBuf, ulReadCount);
            AHC_FS_FileWrite(SDFileId, (UINT8 *)pBuf, ulReadCount, &ulWriteCount);
            printc(FG_YELLOW("i:%x(%x)\r\n"), i, ulWriteCount);            
            i += ulReadCount;
            //memset(&gubRead_Back[0], 0x00, ulReadCount);            
    	} while (i < info->ulSFTotalSize);
        RTNA_DBG_Str0(FG_YELLOW("\r\n Dump WpData Success!\r\n"));
	}
    else {
    	printc(FG_RED("Protect Data opened with ERROR\n"));     
    }
    AHC_FS_FileClose(SDFileId);
    
    return Status;
}

//------------------------------------------------------------------------------
//  Function    : SDWrWpAreaBak
//  Description :
//------------------------------------------------------------------------------
/**
 * @brief                 	Check WP that is exist or not.
 *
 * @param[in ] bFileName	The file name which you want to check.
 * @param[out] ulFileId		The file id.
 *
 * @retval MMP_ERR_NONE		Succeed.
 * @retval others			Fail
 */
SD_UPDATER_ERR SDWrWpArea(void)
{
	AHC_ERR		  Status;
	//MMP_ULONG	  SDFileId;
	MMP_ULONG	  i;    
    MMPF_SIF_INFO *info;
    MMP_ULONG     ulWriteCount, ulFwEndAdr;
    UINT8         *pBuf;
    
    MMPD_System_GetFWEndAddr(&ulFwEndAdr);
    ulFwEndAdr = ALIGN_X(ulFwEndAdr, 32);
    pBuf = (UINT8 *)ulFwEndAdr;    
    
    info = MMPF_SF_GetSFInfo();
    ulWriteCount = 0x100;
    i = info->ulSFTotalSize - info->ulSFBlockSize;
    //printc(FG_YELLOW("range:%x\r\n"), i);
    
    Status = MMPF_SF_EraseBlock(i);
    if (Status == AHC_ERR_NONE || Status == AHC_FS_ERR_EOF) {
        printc(FG_YELLOW("Erase WP Pass!\r\n"));
    }
    
    memset(pBuf, 0xCC, ulWriteCount);
    MMPF_MMU_FlushDCacheMVA((MMP_ULONG)&pBuf[0], (MMP_ULONG)ulWriteCount);
	Status = MMPF_SF_WriteData((MMP_ULONG)i,(MMP_ULONG)&pBuf[0],(MMP_ULONG)ulWriteCount);
    
    if (Status == AHC_ERR_NONE || Status == AHC_FS_ERR_EOF) {
        printc(FG_YELLOW("Wr WP Pass!\r\n"));
    }
    
    return Status;
}
#endif //#if (EN_WP_AREA_CONFIG == 1)

#if defined(SD_FW_DUMP_BIN_NORFLASH)&&(SD_FW_DUMP_BIN_NORFLASH)	
SD_UPDATER_ERR SDUpdateReadNor(UINT32	SDFileId)
{
    SDBIN				sdbin;
    PROJID				sProjID;
    UINT32				fwcb;
    UINT32				i;
    UINT32				p;
    AHC_ERR				Status=0;
    UINT32				rBackLen;
    unsigned char		Read_Back[256];
    MMP_ERR				MMP_Status;
    MD5_CTX				mdContext;
    UINT8               byDisplayID;
    MMPF_SIF_INFO       *info;
    /* for partial update */
    MMP_ULONG	ulSifAddr = 0x0;
    //MMP_ULONG	headerentry[2]={0};
    AIT_STORAGE_INDEX_TABLE2 *pIndexsif = (AIT_STORAGE_INDEX_TABLE2 *)STORAGE_TEMP_BUFFER;
    MMP_ULONG ulOSDInitTimeout = 20;

    //Wait until OSD init done.
    while((AHC_FALSE == AIHC_GetOSDInitStatus()) && (--ulOSDInitTimeout > 0)){
        printc("Wait OSDInit done...\r\n");
        MMPF_OS_SleepMs(100);
    }
    if(ulOSDInitTimeout == 0){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return SD_UPDATER_ERR_FILE; }
    
    MMPF_MMU_FlushDCacheMVA((MMP_ULONG)pIndexsif, 512);
    MMPF_SF_FastReadData(ulSifAddr, STORAGE_TEMP_BUFFER, 512);

    LedCtrl_LcdBackLightLock(AHC_FALSE);
    LedCtrl_LcdBackLight(AHC_TRUE);
    LedCtrl_LcdBackLightLock(AHC_TRUE);
    byDisplayID = OVL_DISPLAY_BUFFER;

	if(0)
	{
		//Read file and accumulate MD5
		AHC_FS_FileSeek(SDFileId, 0, 0);
		AHC_FS_FileRead(SDFileId, (UINT8*)&sdbin, sizeof(SDBIN), &rBackLen);
		AHC_FS_FileRead(SDFileId, (UINT8*)&sProjID, sizeof(PROJID), &rBackLen);

		printc("SD BIN Ver %04x\r\n", sdbin.sbVer);
		printc("SD BIN TAG %c%c%c%c\r\n", sdbin.sbTag[0], sdbin.sbTag[1], sdbin.sbTag[2], sdbin.sbTag[3]);

		if (sdbin.sbTag[0] != 'A' ||
			sdbin.sbTag[1] != 'I' ||
			sdbin.sbTag[2] != 'T' ||
			sdbin.sbTag[3] != 'S') {
			return SD_UPDATER_ERR_FILE;
		}
	}
	
	{
		BEGIN_LAYER(byDisplayID);
		AHC_OSDSetActive(byDisplayID, 1);

		AHC_OSDSetFont(byDisplayID, &GUI_Font16B_1);

		END_LAYER(byDisplayID);
	}

	SDUpdateDrawProgress(   1,
	                        1,
	                        (UINT8 *) "Read File",
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_TRANSPARENT,
                            OSD_COLOR_BLACK);

	/**
	Write FW
	*/
	//fwcb -= (MBR_OFFSET);
	#if (CHECKSDFWHEADER == 1)
		fwcb -= (MBR_OFFSET);
	#else

	#endif

    // Check FW size
    info = MMPF_SF_GetSFInfo();
	printc(FG_RED("NorFlash size = 0x%08x\r\n"),info->ulSFTotalSize);

	fwcb = info->ulSFTotalSize;
	
	for (i = 0 ;i < fwcb; i += 0x100)//read from flash
	{

		#if (TASK_MONITOR_ENABLE) //Prolong timeout due to writing flash.
		MMP_ULONG ulOSCurTime;
		MMPF_OS_GetTime(&ulOSCurTime);
		AHC_Main_Task_ReSetMonitorTime(ulOSCurTime);
		#endif
		
		MMPF_MMU_FlushDCache();
		if(MMP_ERR_NONE == MMPF_SF_FastReadData(i, STORAGE_TEMP_BUFFER, 0x100))
		{
			SDUpdateWriteDumpFile(i, STORAGE_TEMP_BUFFER, 0x100);
		}
		else
		{
			printc("SF_ERROR MMP_STATUS[%x] STATUS[%x]\r\n",MMP_Status,Status);
			SDUpdateDrawProgressEX(1, 1, (UINT8 *) "read SF Error...",
			OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);
			return SD_UPDATER_ERR_FAIL;
		}

	#if 0
		//Deal With FW from AITBOOT, AITBOOTX, ALL.
		MMP_Status = AHC_FS_FileRead(SDFileId, Read_Back, sizeof(Read_Back), &rBackLen);

        if (i == 0)
        {
        	for (t=0; t<2; t++)
	        	headerentry[t] = Read_Back[0+4*t]|(Read_Back[1+4*t]<<8)|(Read_Back[2+4*t]<<16)|(Read_Back[3+4*t]<<24);

        	if ( (headerentry[0] == INDEX_TABLE_HEADER) 		&&
        		 (headerentry[1] & 0x01)						&&
				 (pIndexsif->ulHeader == INDEX_TABLE_HEADER) 	&&
				 (pIndexsif->ulTail == INDEX_TABLE_HEADER)		&&
           		 (pIndexsif->ulFlag & 0x01)	)
        	{
        		headerentry[0] = Read_Back[0+16*12]|(Read_Back[1+16*12]<<8)|(Read_Back[2+16*12]<<16)|(Read_Back[3+16*12]<<24);
        		if ( (headerentry[0] == 0x0) && (pIndexsif->it[10].ulStartSec!=0x0) )
        		{
	        		printc("SDUpdate Text -- Without Userarea.\r\n");
    	    		t = 10;
        			while ((pIndexsif->it[t].ulStartSec!=0x0) && (16*(t+2)<256) )
        			{
						Read_Back[0+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulStartSec&0xFF);
						Read_Back[1+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulStartSec>>8)&0xFF);
						Read_Back[2+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulStartSec>>16)&0xFF);
						Read_Back[3+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulStartSec>>24)&0xFF);

						Read_Back[4+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulSecSize&0xFF);
						Read_Back[5+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulSecSize>>8)&0xFF);
						Read_Back[6+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulSecSize>>16)&0xFF);
						Read_Back[7+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulSecSize>>24)&0xFF);

						Read_Back[8+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulFlag0&0xFF);
						Read_Back[9+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulFlag0>>8)&0xFF);
						Read_Back[10+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulFlag0>>16)&0xFF);
						Read_Back[11+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulFlag0>>24)&0xFF);

						Read_Back[12+16*(t+2)]=(UINT8)(pIndexsif->it[t].ulRealSize&0xFF);
						Read_Back[13+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulRealSize>>8)&0xFF);
						Read_Back[14+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulRealSize>>16)&0xFF);
						Read_Back[15+16*(t+2)]=(UINT8)((pIndexsif->it[t].ulRealSize>>24)&0xFF);

        				t++;
        			}
        		}
        	}

        }
	
		if (MMP_Status == AHC_ERR_NONE || MMP_Status == AHC_FS_ERR_EOF)
		{
			if(i % info->ulSFBlockSize == 0){
			    MMPF_SF_EraseBlock(i);
			    printc("i2: 0x%X\r", i);
			}
         
			MMPF_MMU_FlushDCacheMVA((MMP_ULONG)Read_Back, (MMP_ULONG)rBackLen);
			MMP_Status = MMPF_SF_WriteData((MMP_ULONG)i,(MMP_ULONG)Read_Back,(MMP_ULONG)rBackLen);
			
		}else{
		    printc(FG_RED("Write Failed \r\n"));
		}
	#endif

		if (p != i >> 16)
		{
			p = i >> 16;

            SDUpdateDrawProgress(   p,
                                    fwcb >> 16,
                                    (UINT8 *) "Read File",
                                    OSD_COLOR_GREEN,
                                    OSD_COLOR_LIGHTMAGENTA,
                                    OSD_COLOR_WHITE);
		}
	}
	
	{
		SDUpdateDrawProgress(p, fwcb >> 16, (UINT8 *) "Write Firmware",
					OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);

		SDUpdateDrawProgress( fwcb, fwcb, (UINT8 *) "Write Done",
					OSD_COLOR_GREEN, OSD_COLOR_LIGHTMAGENTA, OSD_COLOR_WHITE);

		AHC_OS_Sleep(1000);
		SDUpdateDrawProgress( fwcb, fwcb, (UINT8 *) "Write Done",
					OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK);
	}
	
	return SD_UPDATER_ERR_NONE;
}
#endif

#if defined(SD_FW_DUMP_BIN_NORFLASH)&&(SD_FW_DUMP_BIN_NORFLASH)	
SD_UPDATER_ERR SDUpdateDumpSpiNor(void)
{
	SD_UPDATER_ERR ret = SD_UPDATER_ERR_FILE;
	MMP_ERR ErrResult = MMP_ERR_NONE;
	char FileName[200]={0};
	MMP_ULONG	ulFileID = 0;

	#if defined(SD_FW_MAKE_MP_BIN)&&(SD_FW_MAKE_MP_BIN)
	STRCPY(FileName, "SF:1:\\DDRIIIDelayRange.txt");
	ErrResult = MMPF_FS_FOpen(FileName, "rb", &ulFileID);
	
	if(MMP_ERR_NONE == ErrResult){
		MMPF_FS_FClose(ulFileID);
		ErrResult = MMPF_FS_Remove(FileName);
		if (ErrResult)
			printc("Remove %s fail\r\n",FileName);
		else
			printc("Remove %s success\r\n",FileName);
	}

	STRCPY(FileName, "SF:3:\\DDRIIIDelayRange.txt");
	ErrResult = MMPF_FS_FOpen(FileName, "rb", &ulFileID);
	if(MMP_ERR_NONE == ErrResult){
		MMPF_FS_FClose(ulFileID);
		
		MMPF_SF_ClrProtArea();
		ErrResult = MMPF_FS_Remove(FileName);
		if (ErrResult)
			printc("Remove %s fail\r\n",FileName);
		else
			printc("Remove %s success\r\n",FileName);
	}
	#endif

	if(AHC_ERR_NONE == SDUpdateOpenDumpFile())
	{
		printc(FG_GREEN("[INFO]%s dump spi nor ++!\r\n"),__func__);
		
		SDUpdateReadNor(m_DumpSpiFileID);
		SDUpdateCloseDumpFile();
		
		printc(FG_GREEN("[INFO]%s dump spi nor success!\r\n"),__func__);
		ret = SD_UPDATER_ERR_NONE;
		while(1);
	}

	return ret;
}
#endif

//------------------------------------------------------------------------------
//  Function    : SDUpdateCheckFileExisted
//  Description :
//------------------------------------------------------------------------------
/**
 * @brief                 	Check file that is exist or not.
 *
 * @param[in ] bFileName	The file name which you want to check.
 * @param[out] ulFileId		The file id.
 *
 * @retval MMP_ERR_NONE		Succeed.
 * @retval others			Fail
 */
SD_UPDATER_ERR SDUpdateCheckFileExisted(MMP_BYTE *bFileName)
{
    AHC_ERR		Status;
    MMP_ULONG	SDFileId;

#if (SD_FW_UPDATER)
    UPDATERX_ERR eUpdaterError;
#endif

    MMP_ULONG ulOSDInitTimeout = 20;
    extern MMP_UBYTE bDCFInitDone;

    //Andy Liu. Workaround solution. Remove it later...
    printc(FG_RED("\r\n%s,%d. Warning!!! Here is a workaround solution...\r\n"), __func__, __LINE__);

    ulOSDInitTimeout = 70;
    while((MMPS_FS_IsVolumeMounted("SD:\\",4) != MMP_ERR_NONE) && (--ulOSDInitTimeout > 0)){
        printc("Wait SD mount done...\r\n");
        MMPF_OS_SleepMs(100);
    }    
    if(ulOSDInitTimeout == 0){ AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return SD_UPDATER_ERR_FILE; }

	Status = AHC_FS_FileOpen(bFileName, STRLEN(bFileName),"rb", sizeof("rb"),&SDFileId);

    #if (SD_FW_UPDATER)
    eUpdaterError = UpdaterX_CheckFlow(FW_TYPE_ALLFW);

    if(eUpdaterError != UPDATERX_ERR_UPDATE){

        if (eUpdaterError == UPDATERX_ERR_SHUT_DOWN) {
            m_eSDUpdater = SD_UPDATER_ERR_LOG;
            return m_eSDUpdater;
        }

        if (eUpdaterError == UPDATERX_ERR_DONE) {
            m_eSDUpdater = SD_UPDATER_ERR_FILE;
            return m_eSDUpdater;
        }
    }
    #endif

    if (Status == AHC_ERR_NONE)
    {
	//Andy Liu TBD.
#if 0	
#if (HDMI_PREVIEW)
		if(AHC_IsHdmiConnect())
		{
			HDMIFunc_InitPreviewOSD();
			AHC_SetDisplayMode(DISPLAY_MODE_DISABLE);
			AHC_SetDisplayMode(DISPLAY_MODE_OSD_ENABLE);
			SetHDMIState(AHC_HDMI_VIDEO_PREVIEW_STATUS);
		}
#endif
#if (TVOUT_PREVIEW)
		if(AHC_IsTVConnectEx())
		{
			TVFunc_InitPreviewOSD();
			AHC_SetDisplayMode(DISPLAY_MODE_DISABLE);
			AHC_SetDisplayMode(DISPLAY_MODE_OSD_ENABLE);
			//SetTVState(AHC_TV_VIDEO_PREVIEW_STATUS);
		}
#endif
#endif

    	#if (SD_FW_RAW_UPDATE == 1)
    	m_eSDUpdater = SDUpdateRawReadBinFile(SDFileId);
	    #else
	    m_eSDUpdater = SDUpdateReadBinFile(SDFileId);
	    #endif

	    AHC_FS_FileClose(SDFileId);

		#if SD_FW_REMOVE_SD_UPDATE_FILE			// maybe defined in config_sdk.h
		printc("Remove %s ...\r\n", SD_FW_UPDATER_BIN_NAME);
	    AHC_FS_FileRemove(bFileName,  STRLEN(bFileName));
	    #endif

	    #if (SD_FW_UPDATER)
	    UpdaterX_DeleteLog();
	    #endif

	    printc("SD Update Success %d \n", m_eSDUpdater);
    }
    else
    {
    	printc("File opened with ERROR\n");
    	m_eSDUpdater = SD_UPDATER_ERR_FILE;
    }

    return m_eSDUpdater;
}

#if (EN_WP_AREA_CONFIG == 1)
//------------------------------------------------------------------------------
//  Function    : SDUpdateCfgWpFile
//  Description :
//------------------------------------------------------------------------------
SD_UPDATER_ERR SDUpdateCfgWpFile(char ubFileName[], char ubFileExt[], MMP_BYTE ubWpFileNum)
{
    MMP_BYTE *pWpFileName;
    MMP_BYTE *pWpFileExt;    

    gubWpFileNum = ubWpFileNum;
    memset(gubWpFileName, 0x00, sizeof(gubWpFileName));
    memset(gubWpExtName, 0x00, sizeof(gubWpExtName));
    STRCPY(gubWpFileName, (INT8*)ubFileName);
    STRCPY(gubWpExtName, (INT8*)ubFileExt);
	pWpFileName = &gubWpFileName[0];
	pWpFileExt = &gubWpExtName[0];
	
    RTNA_DBG_Str0(FG_YELLOW("Cfg WriteProt FileName "));
    RTNA_DBG_Str0(pWpFileName);
    RTNA_DBG_Str0("0~");    
    DBG_D0((gubWpFileNum -1), 1);
    RTNA_DBG_Str0(pWpFileExt);
    RTNA_DBG_Str0("\r\n");

	return MMP_TRUE;
}
//------------------------------------------------------------------------------
//  Function    : SDUpdateCheckWpFsExisted
//  Description :
//------------------------------------------------------------------------------
/**
 * @brief                 	Check WP that is exist or not.
 *
 * @param[in ] bFileName	The file name which you want to check.
 * @param[out] ulFileId		The file id.
 *
 * @retval MMP_ERR_NONE		Succeed.
 * @retval others			Fail
 */
SD_UPDATER_ERR SDUpdateCheckWpFsExisted(MMP_BYTE *bFileName)
{
	AHC_ERR		Status;
	MMP_ULONG	SDFileId;

	Status = AHC_FS_FileOpen(bFileName, STRLEN(bFileName),"rb", sizeof("rb"),&SDFileId);
    
    if(Status == AHC_ERR_NONE) {
        MMPF_SF_ClrProtArea();
        RTNA_DBG_Str0("[Update]Src ");
        RTNA_DBG_Str0(bFileName);
        RTNA_DBG_Str0("\r\n");
    	m_eSDUpdater = SDUpdateWPBinFS(SDFileId, bFileName);
	    AHC_FS_FileClose(SDFileId);
	    //AHC_FS_FileRemove(bFileName,  STRLEN(bFileName));
	    //printc(FG_YELLOW("Protect Data Update Success %d \n"), m_eSDUpdater);
        MMPF_SF_SetProtWpArea();
    }
    else {          
    	m_eSDUpdater = SD_UPDATER_ERR_FILE;
    }
    
    return m_eSDUpdater;
}

//------------------------------------------------------------------------------
//  Function    : SDUpdateCheckWPExisted
//  Description :
//------------------------------------------------------------------------------
/**
 * @brief                 	Check WP that is exist or not.
 *
 * @param[in ] bFileName	The file name which you want to check.
 * @param[out] ulFileId		The file id.
 *
 * @retval MMP_ERR_NONE		Succeed.
 * @retval others			Fail
 */
SD_UPDATER_ERR SDUpdateCheckWPExisted(MMP_BYTE *bFileName)
{
	AHC_ERR		Status;
	MMP_ULONG	SDFileId;

	Status = AHC_FS_FileOpen(bFileName, STRLEN(bFileName),"rb", sizeof("rb"),&SDFileId);
    
    if(Status == AHC_ERR_NONE) {
        MMPF_SF_ClrProtArea();
    	m_eSDUpdater = SDUpdateWPBin(SDFileId);
	    AHC_FS_FileClose(SDFileId);
	    //AHC_FS_FileRemove(bFileName,  STRLEN(bFileName));
	    printc(FG_YELLOW("Protect Data Update Success %d \n"), m_eSDUpdater);
        MMPF_SF_SetProtWpArea();
    }
    else {          
    	m_eSDUpdater = SD_UPDATER_ERR_FILE;
    }
    
    return m_eSDUpdater;
}

MMP_BOOL SDUpdateIsWpExisted(void)
{
	AHC_ERR		uiError = AHC_ERR_NONE;
	MMP_ULONG	uiFileId;
    MMP_ULONG   i;
    MMP_BYTE    Index[2];
    MMP_BYTE    ubSDFName[64] = {0};
    
    printc("### %s -\r\n", __func__);
		      
    for (i = 0; i<gubWpFileNum; i++) {
    	MEMSET0(ubSDFName);        
		STRCPY(ubSDFName, gubWpFileName);
	    sprintf(Index, "%d", i);
		STRCAT(ubSDFName, Index);
		STRCAT(ubSDFName, gubWpExtName);
	    uiError = AHC_FS_FileOpen(ubSDFName, STRLEN(ubSDFName),"rb", sizeof("rb"),&uiFileId);
     
        if(uiError == AHC_ERR_NONE) {
    	    AHC_FS_FileClose(uiFileId);
            RTNA_DBG_Str0(FG_YELLOW("WP Update Files Existed\r\n"));
            return AHC_TRUE;
        }
    }

    RTNA_DBG_Str0("WpFileMiss!\r\n");
    return uiError;
}
#endif //#if (EN_WP_AREA_CONFIG == 1)

MMP_BOOL SDUpdateIsFWExisted(void)
{
	AHC_ERR		uiError;
	MMP_ULONG	uiFileId;

    printc("### %s -\r\n", __func__);

	uiError = AHC_FS_FileOpen(SD_FW_UPDATER_BIN_NAME, STRLEN(SD_FW_UPDATER_BIN_NAME),"rb", sizeof("rb"),&uiFileId);

    if(uiError==AHC_ERR_NONE)
    {
	    AHC_FS_FileClose(uiFileId);
	    printc("FW Update Bin Existed \n");
        return AHC_TRUE;
    }
    else
    {
    	printc("FW Update Bin N/A \n");

		#if defined(SD_FW_DUMP_BIN_NORFLASH)&&(SD_FW_DUMP_BIN_NORFLASH)		
		SDUpdateDumpSpiNor();
		#endif
		
        return AHC_FALSE;
    }
}

/*
 * return 0: Success
 *		  1: File Corruption
 *		  2: Failed
 */
SD_UPDATER_ERR SDUpdateGetStatus(void)
{
	return m_eSDUpdater;
}


#if 0
void __DB_UPATER__(){}
#endif

#define TEMP_BUF_LENGTH (0x200)

#if(EDOG_ENABLE)
#if (EDOG_DB_DYNAMIC_SEGMENT == 0)
SD_UPDATER_ERR SDUpdateReadDBBin(UINT32	uiNewFileId)
{
	UINT32  uiNewFileSize = 0, i = 0;
	UINT32  uiReadLen = 0;
	UINT32  uiWriteLen = 0;
	UBYTE   byBuf[TEMP_BUF_LENGTH] = {0};

	#if(POI_UPDATE_DECRYPT)
	UBYTE	ubUUID[BLOCK_SIZE/8] = {0};
	INT8	ubUUIDTemp[33] = {0};
	UBYTE	key[BLOCK_SIZE/8] = {0};
	UBYTE	AITEncryptHeader[] = "AIT__RU8AU4HEAD";
	UBYTE	AITHeader[] = "AIT_RU8AU4TOOLS";
	UBYTE	ubEncryptedCHKSUM[4] = {0};
	UBYTE	ubRawCHKSUM[4] = {0};
	UINT32	uiEncryptedCHKSUM = 0;
	UINT32	uiRbEncryptedCHKSUM = 0;
	UINT32	uiRbRawCHKSUM = 0;
	UINT32	uiRawCHKSUM = 0;
	UINT32	j = 0;
	#endif

	AHC_ERR eStatus = 0;

    UINT32  uiOldFileID = 0;

	//Get file size
	eStatus = AHC_FS_FileGetSize(uiNewFileId, &uiNewFileSize);

	if(eStatus != AHC_ERR_NONE)
    	return SD_UPDATER_ERR_FILE;

	DPRINT("DB Size = %d \n", uiNewFileSize);


	LedCtrl_LcdBackLight(AHC_TRUE);

	AHC_OSDSetActive(OVL_DISPLAY_BUFFER, 1);

	AHC_OSDSetFont(OVL_DISPLAY_BUFFER, &GUI_Font16B_1);

    /**

    Start Updating DB.

    */
    AHC_FS_FileOpen(SPEED_CAM_DB_FILE_NAME, STRLEN(SPEED_CAM_DB_FILE_NAME), "wb", sizeof("wb"), &uiOldFileID);

    #if(POI_UPDATE_DECRYPT)//EU Sku cancels decrypt speed cam data

    AHC_FS_FileRead(uiNewFileId, key, BLOCK_SIZE/8, &uiReadLen);
	if(strcmp((char*) key, (char*) AITHeader) != 0)
	{
		AHC_FS_FileClose(uiOldFileID);

		printc("Wrong Header\r\n");
		printc("Read Header: %s\r\n", key);
		printc("Correct Header: %s\r\n",AITHeader);
		return SD_UPDATER_ERR_FAIL;
	}

    //Check checksum of bin file
    AHC_FS_FileSeek(uiNewFileId, -8, AHC_FS_SEEK_END);
    AHC_FS_FileRead(uiNewFileId, ubRawCHKSUM, 4, &uiReadLen);
    AHC_FS_FileRead(uiNewFileId, ubEncryptedCHKSUM, 4, &uiReadLen);

    memcpy(&uiRawCHKSUM, ubRawCHKSUM, 4);
    memcpy(&uiEncryptedCHKSUM, ubEncryptedCHKSUM, 4);

    AHC_FS_FileSeek(uiNewFileId, 16, AHC_FS_SEEK_SET);

    for(i = 0; i < uiNewFileSize - 16; i += uiReadLen)
    {
        AHC_FS_FileRead(uiNewFileId, byBuf, TEMP_BUF_LENGTH, &uiReadLen);
        for(j = 0; j < uiReadLen; j++)
        {
        	if( (i+j) < uiNewFileSize - 8 -16)
        	{
        		uiRbEncryptedCHKSUM = (uiRbEncryptedCHKSUM + *(byBuf+j)) & 0xFFFFFFFF;
        	}
        	else
        	{
        		//TBD
        	}
        }
    }

    if(uiRbEncryptedCHKSUM != uiEncryptedCHKSUM)
    {
    	AHC_FS_FileClose(uiOldFileID);

    	printc("Correct SD File CHKSUM=%d. Wrnog SD File CHKSUM=%d\r\n", uiEncryptedCHKSUM, uiRbEncryptedCHKSUM);
    	return SD_UPDATER_ERR_FILE;
    }

    //GetUUID
    memset(key, 0, BLOCK_SIZE/8);
    UI_GetUUID(ubUUIDTemp, 33);
	strncpy((char *) ubUUID, (char *)ubUUIDTemp+16, BLOCK_SIZE/8);

	EncryptBlockBuffer(AITEncryptHeader, KeySize, (MMP_UBYTE*) ubUUID, key );

	AHC_FS_FileSeek(uiNewFileId, 16, AHC_FS_SEEK_SET);
    #endif

    #if(POI_UPDATE_DECRYPT)//EU Sku cancels decrypt speed cam data
    for(i = 16; i < uiNewFileSize - 8; i += uiReadLen){
    #else
    for(i = 0; i < uiNewFileSize; i += uiReadLen){
    #endif

        memset(byBuf, 0, TEMP_BUF_LENGTH);

        #if(POI_UPDATE_DECRYPT)///EU Sku cancels decrypt speed cam data
        if( (i + TEMP_BUF_LENGTH) > uiNewFileSize)
        	AHC_FS_FileRead(uiNewFileId, byBuf, (uiNewFileSize - i -8), &uiReadLen);
        else
        	AHC_FS_FileRead(uiNewFileId, byBuf, TEMP_BUF_LENGTH, &uiReadLen);

        Decrypt(uiNewFileId, uiOldFileID, byBuf, uiReadLen, key);

        AHC_FS_FileSeek(uiOldFileID, i-16, 0);
        AHC_FS_FileWrite(uiOldFileID, byBuf, uiReadLen, &uiWriteLen);
        #else
        AHC_FS_FileRead(uiNewFileId, byBuf, TEMP_BUF_LENGTH, &uiReadLen);

        AHC_FS_FileSeek(uiOldFileID, i, 0);
        AHC_FS_FileWrite(uiOldFileID, byBuf, uiReadLen, &uiWriteLen);
        #endif

#if 0
		SDUpdateDrawProgressEX2( i,
                                uiNewFileSize,
                                IDS_DS_Data_Updating,
                                OSD_COLOR_WHITE,
                                OSD_COLOR_WHITE,
                                OSD_COLOR_TRANSPARENT,//OSD_COLOR_ORANGE,
                                IDS_DS_Data_Updating_2);
        if(uiReadLen != uiWriteLen){
			SDUpdateDrawProgressEX2( i,
									uiNewFileSize,
									IDS_DS_Data_Updating,
									OSD_COLOR_WHITE,
									OSD_COLOR_WHITE,
									OSD_COLOR_TRANSPARENT,//OSD_COLOR_ORANGE,
									IDS_DS_Warning);
            return SD_UPDATER_ERR_FAIL;
        }
#endif
    }

#if 0
	SDUpdateDrawProgressEX2(uiNewFileSize,
							uiNewFileSize,
							IDS_DS_Data_Updating,
							OSD_COLOR_WHITE,
							OSD_COLOR_WHITE,
							OSD_COLOR_TRANSPARENT,//OSD_COLOR_ORANGE,
							IDS_DS_Done);
	AHC_OS_Sleep(1000);

	SDUpdateDrawProgressEX2(uiNewFileSize,
							uiNewFileSize,
							IDS_DS_Data_Updating,
							OSD_COLOR_TRANSPARENT,
							OSD_COLOR_TRANSPARENT,
							OSD_COLOR_TRANSPARENT,//OSD_COLOR_ORANGE,
							IDS_DS_Done);
#endif

	#if(POI_UPDATE_DECRYPT)
	AHC_FS_FileClose(uiOldFileID);
	AHC_FS_FileOpen(SPEED_CAM_DB_FILE_NAME, STRLEN(SPEED_CAM_DB_FILE_NAME), "rb", sizeof("rb"), &uiOldFileID);
	AHC_FS_FileSeek(uiOldFileID, 0, AHC_FS_SEEK_SET);

	eStatus = AHC_FS_FileGetSize(uiOldFileID, &uiNewFileSize);
	for(i = 0; i < uiNewFileSize; i += uiReadLen)
    {
        AHC_FS_FileRead(uiOldFileID, byBuf, TEMP_BUF_LENGTH, &uiReadLen);
        for(j = 0; j < uiReadLen; j++)
        {
        	uiRbRawCHKSUM = (uiRbRawCHKSUM + *(byBuf+j)) & 0xFFFFFFFF;
        }
    }

	if(uiRbRawCHKSUM != uiRawCHKSUM)
	{
		AHC_FS_FileClose(uiOldFileID);

		printc("Correct SIF File CHKSUM=%d. Wrnog SIF File CHKSUM=%d\r\n", uiRawCHKSUM, uiRbRawCHKSUM);
    	return SD_UPDATER_ERR_FILE;
	}
	#endif

	AHC_FS_FileClose(uiOldFileID);
	return SD_UPDATER_ERR_NONE;
}
#else
SD_UPDATER_ERR SDUpdateReadDBBin(UINT32	uiNewFileId)
{

	UINT32  uiNewFileSize = 0;
	UINT32  uiReadLen = 0;
	UINT32  uiWriteLen = 0;
	UINT32  ulTotalWrSize = 0;
	UINT32  ulFileSize;
	UBYTE   byBuf[TEMP_BUF_LENGTH] = {0};
	AHC_ERR eStatus = 0;
	UINT32  i, j , k, l = 0;
    UINT32  uiOldFileID = 0;
    UINT32  checksum = 0;
    UINT8   pbyBuffer[TEMP_BUF_LENGTH];
    UINT32  readback;

	//Get file size
	eStatus = AHC_FS_FileGetSize(uiNewFileId, &uiNewFileSize);

	if(eStatus != AHC_ERR_NONE)
    	return SD_UPDATER_ERR_FILE;

	DPRINT("DB Size = %d \n", uiNewFileSize);


	LedCtrl_LcdBackLight(AHC_TRUE);

	AHC_OSDSetActive(OVL_DISPLAY_BUFFER, 1);

	AHC_OSDSetFont( &GUI_Font16B_1);

    //Initial EDOG_Buffer

    EDOGCtrl_Initial();
    /**

    Start Updating DB.

    */
    EDOGCtrl_ParseDBHeader(uiNewFileId);

    AHC_FS_FileSeek(uiNewFileId,512,AHC_FS_SEEK_SET);

    uiNewFileSize -=4096; uiNewFileSize += sizeof(FILELAYOUTTABLE);// Skip the first 4K bytes except sFileLayout

    printc(" sizeof(FILELAYOUTTABLE) : %d\r\n", sizeof(FILELAYOUTTABLE));

    eStatus = AHC_FS_FileOpen(MAPLAYOUT_HEADERNAME, STRLEN(MAPLAYOUT_HEADERNAME), "wb", sizeof("wb"), &uiOldFileID);

    if(eStatus)
    printc("AHC_FS_FileOpen err %d\r\n",eStatus);

    for(i = 0; i < sizeof(FILELAYOUTTABLE); i += uiReadLen){
    	memset(byBuf, 0, TEMP_BUF_LENGTH);
    	AHC_FS_FileRead(uiNewFileId, byBuf, TEMP_BUF_LENGTH, &uiReadLen);
    	//AHC_FS_FileSeek(uiOldFileID, i, 0);
        AHC_FS_FileWrite(uiOldFileID, byBuf, uiReadLen, &uiWriteLen);

		        SDUpdateDrawProgressEX( i,
		                                uiNewFileSize,
		                                (UINT8 *) "SpeedCam Updating...",
		                                OSD_COLOR_WHITE,
		                                OSD_COLOR_WHITE,
		                                OSD_COLOR_ORANGE,
		                                (UINT8 *) "Don't Remove SD Card and",
		                                (UINT8 *)"Charger");
		        if(uiReadLen != uiWriteLen){

					SDUpdateDrawProgressEX(i,
					                    uiNewFileSize,
					                    (UINT8 *) "SpeedCam Updating Fail...",
					                    OSD_COLOR_TRANSPARENT,
					                    OSD_COLOR_TRANSPARENT,
					                    OSD_COLOR_BLACK,
					                    NULL,
					                    NULL);
		            AHC_FS_FileClose(uiOldFileID);
    				uiOldFileID = NULL;
    				printc("SD_UPDATER_ERR_FAIL_1\r\n");
		            return SD_UPDATER_ERR_FAIL;
    		}
    }
    AHC_FS_FileClose(uiOldFileID);
    uiOldFileID = NULL;
    ulTotalWrSize += i;
    printc("sFileLayout->ulTotalPart %d\r\n",EDOGCtrl_HeaderInfo()->ulTotalPart);
    for(i = 0; i < (EDOGCtrl_HeaderInfo()->ulTotalPart); i++)
    {
    	char filename[256] = {0};
		char Index[2];
		MMP_ULONG ulRemainBytes;
		MEMSET0(filename);
		STRCPY(filename, MAPLAYOUT_BASENAME);
		sprintf(Index,"%d",i);
		STRCAT(filename,Index);
		STRCAT(filename,MAPLAYOUT_EXTNAME);
		eStatus = AHC_FS_FileOpen(filename, STRLEN(filename), "wb", sizeof("wb"), &uiOldFileID);
		if(eStatus)
		printc("AHC_FS_FileOpen err %d\r\n",eStatus);

		printc("filename : %s\r\n",filename);
		printc("FileSize %d\r\n",EDOGCtrl_HeaderInfo()->sFileInfo[i].ulFileSize);
    	AHC_FS_FileSeek(uiNewFileId, EDOGCtrl_HeaderInfo()->sFileInfo[i].ulOffset+4096, AHC_FS_SEEK_SET);
    	ulRemainBytes = EDOGCtrl_HeaderInfo()->sFileInfo[i].ulFileSize;
	    for(j = 0; j < EDOGCtrl_HeaderInfo()->sFileInfo[i].ulFileSize; j += uiReadLen){
	    	memset(byBuf, 0, TEMP_BUF_LENGTH);
	    	if(ulRemainBytes >= TEMP_BUF_LENGTH)
	    	{
	    		ulRemainBytes -= TEMP_BUF_LENGTH;
	    		AHC_FS_FileRead(uiNewFileId, byBuf, TEMP_BUF_LENGTH, &uiReadLen);
	    	}
	    	else
	    	{
	    		AHC_FS_FileRead(uiNewFileId, byBuf, ulRemainBytes, &uiReadLen);
	    	}
	    	//AHC_FS_FileSeek(uiOldFileID, j, 0);
	        AHC_FS_FileWrite(uiOldFileID, byBuf, uiReadLen, &uiWriteLen);


			        SDUpdateDrawProgressEX( i,
			                                uiNewFileSize,
			                                (UINT8 *) "SpeedCam Updating...",
			                                OSD_COLOR_WHITE,
			                                OSD_COLOR_WHITE,
			                                OSD_COLOR_ORANGE,
			                                (UINT8 *) "Don't Remove SD Card and",
			                                (UINT8 *)"Charger");
			        if(uiReadLen != uiWriteLen){
						SDUpdateDrawProgressEX(i,
						                    uiNewFileSize,
						                    (UINT8 *) "SpeedCam Updating Fail...",
						                    OSD_COLOR_TRANSPARENT,
						                    OSD_COLOR_TRANSPARENT,
						                    OSD_COLOR_BLACK,
						                    NULL,
						                    NULL);
			            AHC_FS_FileClose(uiOldFileID);
	    				uiOldFileID = NULL;
	    				printc("SD_UPDATER_ERR_FAIL_2\r\n");
			            return SD_UPDATER_ERR_FAIL;
	    		}
	    }
    	ulTotalWrSize += j;
    	//Calculate CheckSum
		AHC_FS_FileClose(uiOldFileID);
		uiOldFileID = NULL;
		AHC_FS_FileOpen(filename, STRLEN(filename), "rb", sizeof("rb"), &uiOldFileID);
		AHC_FS_FileSeek(uiOldFileID, 0, AHC_FS_SEEK_SET);
		ulFileSize = 0;
		eStatus = AHC_FS_FileGetSize(uiOldFileID, &ulFileSize);
		for(;ulFileSize >= TEMP_BUF_LENGTH;ulFileSize-=TEMP_BUF_LENGTH)
		{
			AHC_FS_FileRead(uiOldFileID, pbyBuffer, TEMP_BUF_LENGTH, &readback);
			for(k = 0; k<(TEMP_BUF_LENGTH/4); k++)
			{
				checksum += (UINT32)(pbyBuffer[k*4] << 24 | pbyBuffer[k*4 + 1] << 16 | pbyBuffer[k*4 + 2] << 8 | pbyBuffer[k*4 + 3]);
			}
			//printc("CheckSum[%d] = 0x%x\r\n",l,checksum);
			l++;
		}
		memset(pbyBuffer, 0, TEMP_BUF_LENGTH);
		if(ulFileSize > 0)
		{
			AHC_FS_FileRead(uiOldFileID, pbyBuffer, ulFileSize, &readback);
			for(k = 0; k<(ulFileSize+3)/4; k++)
			{
				checksum += (UINT32)(pbyBuffer[k*4] << 24 | pbyBuffer[k*4 + 1] << 16 | pbyBuffer[k*4 + 2] << 8 | pbyBuffer[k*4 + 3]);
			}
			ulFileSize = 0;
			//printc("Remaining bytes\r\n");
			//printc("CheckSum[%d] = 0x%x\r\n",l,checksum);
		}
		if(checksum != EDOGCtrl_HeaderInfo()->sFileInfo[i].ulCheckSum)
		{
			AHC_FS_FileClose(uiOldFileID);
			uiOldFileID = NULL;
			printc("Header SIF File CHKSUM=%d. Calculate CHKSUM=%d\r\n", EDOGCtrl_HeaderInfo()->sFileInfo[i].ulCheckSum, checksum);
	    	return SD_UPDATER_ERR_FILE;
		}
		checksum = 0;
		AHC_FS_FileClose(uiOldFileID);
		uiOldFileID = NULL;

    }

	SDUpdateDrawProgressEX( uiNewFileSize, uiNewFileSize, (UINT8 *) "SpeedCam Updating Success",
				OSD_COLOR_WHITE, OSD_COLOR_WHITE, OSD_COLOR_ORANGE, NULL, NULL);

	AHC_OS_Sleep(1000);

	SDUpdateDrawProgressEX( uiNewFileSize, uiNewFileSize, (UINT8 *) "SpeedCam Updating Success",
				OSD_COLOR_TRANSPARENT, OSD_COLOR_TRANSPARENT, OSD_COLOR_BLACK, NULL, NULL);

	AHC_FS_FileClose(uiOldFileID);
	AHC_FS_FileClose(uiNewFileId);
    uiOldFileID = NULL;
    uiNewFileId = NULL;
	return SD_UPDATER_ERR_NONE;
}
#endif

SD_UPDATER_ERR SDUpdateCheckDBExisted(MMP_BYTE *bFileName)
{
	AHC_ERR		Status;
	MMP_ULONG	SDFileId;

	Status = AHC_FS_FileOpen(bFileName, STRLEN(bFileName),"rb", sizeof("rb"),&SDFileId);

    if(Status == AHC_ERR_NONE)
    {
    	m_eSDUpdater = SDUpdateReadDBBin(SDFileId);
	    AHC_FS_FileClose(SDFileId);
	    AHC_FS_FileRemove(bFileName,  STRLEN(bFileName));
	    printc("SpeedCam Update Success %d \n", m_eSDUpdater);
    }
    else
    {
    	printc("SpeedCam opened with ERROR\n");
    	m_eSDUpdater = SD_UPDATER_ERR_FILE;
    }

    return m_eSDUpdater;
}


MMP_BOOL SDUpdateIsDBExisted(void)
{
	AHC_ERR		uiError;
	MMP_ULONG	uiFileId;

	uiError=AHC_FS_FileOpen(SD_DB_UPDATER_BIN_NAME, STRLEN(SD_DB_UPDATER_BIN_NAME),"rb", sizeof("rb"),&uiFileId);

    if(uiError==AHC_ERR_NONE)
    {
	    AHC_FS_FileClose(uiFileId);
	    printc("SpeedCam Update Bin Existed \n");
        return AHC_TRUE;
    }
    else
    {
    	printc("SpeedCam Update Bin N/A \n");
        return AHC_FALSE;
    }
}
#endif

const char* FWUpdatePercent(void)
{
	switch (_fwuState) {
	case FWU_NONE:
		return "idle";
	case FWU_PREPARE:
		return "prepare";
	case FWU_BURN: {
		static char _szPer[8];
		sprintf(_szPer, "%d", _uPercent);
		return _szPer;
	}
	case FWU_VERIFY:
		return "verify";
		break;
	case FWU_SUCCEED:
		return "succeed";
		break;
	case FWU_FAIL:
	case FWU_ERROR:
		return "fail";
	}
	return NULL;
}

MMP_BOOL	SDUpdateBackupNetUserConf(void)
{
#if WIFI_PORT == 1
	char*		path1="SD:\\usernetconf.txt";
	AHC_ERR		err, err1;
	UINT32		ulFileId, ulFileId1;
	
	err = AHC_FS_FileOpen(NCFG_USERSET_FILE_NAME, STRLEN(NCFG_USERSET_FILE_NAME),"r", sizeof("r"), &ulFileId);
	if (err == AHC_ERR_NONE)
	{
		AHC_FS_FileClose(ulFileId);
		AHC_FS_FileCopy(NCFG_USERSET_FILE_NAME, STRLEN(NCFG_USERSET_FILE_NAME), path1, STRLEN(path1));
		err1 = AHC_FS_FileOpen(path1, STRLEN(path1),"r", sizeof("r"),&ulFileId1);
		if (err1 != AHC_ERR_NONE)
		{
			printc("Backup %s failed.\r\n",NCFG_USERSET_FILE_NAME);
			return AHC_FALSE;
		}
		else
		{
			printc("Backup %s Sucessed.\r\n",NCFG_USERSET_FILE_NAME);
			AHC_FS_FileClose(ulFileId1);
		}
	}
	else
	{
		return AHC_FALSE;
	}
	
#endif
	return AHC_TRUE;
}

MMP_BOOL	SDUpdateWriteBackNetUserConf(void)
{
#if WIFI_PORT == 1
	char*		path1="SD:\\usernetconf.txt";
	AHC_ERR		err, err1;
	UINT32		ulFileId, ulFileId1;
	
	err1 = AHC_FS_FileOpen(path1, STRLEN(path1),"r", sizeof("r"), &ulFileId1);
	if (err1 == AHC_ERR_NONE)
	{
		AHC_FS_FileClose(ulFileId1);
		AHC_FS_FileRemove(NCFG_USERSET_FILE_NAME, STRLEN(NCFG_USERSET_FILE_NAME));
		AHC_FS_FileCopy(path1, STRLEN(path1), NCFG_USERSET_FILE_NAME, STRLEN(NCFG_USERSET_FILE_NAME));
		err = AHC_FS_FileOpen(NCFG_USERSET_FILE_NAME, STRLEN(NCFG_USERSET_FILE_NAME),"r", sizeof("r"),&ulFileId);
		if (err != AHC_ERR_NONE)
		{
			printc("Write Back %s failed.\r\n",NCFG_USERSET_FILE_NAME);
			return AHC_FALSE;
		}
		else
		{
			AHC_FS_FileClose(ulFileId);
			AHC_FS_FileRemove(path1, STRLEN(path1));
			printc("Write Back %s Sucessed.\r\n",NCFG_USERSET_FILE_NAME);
		}
	}
	else
	{
		printc("SD %s N/A.\r\n",NCFG_USERSET_FILE_NAME);
		return AHC_FALSE;
	}

#endif
	return AHC_TRUE;
}

#if(WIFI_PORT && (SD_MAC_UPDATER_BURNIN || SD_MAC_POWERON_BURNIN))

static UINT8 gaBurnInMAC[] = {0,0,0,0,0,0};

MMP_BOOL	SDUpdateBackupMAC(char *path)
{
	AHC_ERR		err;
	UINT32		ulFileId;
		
	if ((err = AHC_FS_FileOpen(path, STRLEN(path),"rb", sizeof("rb"), &ulFileId)) == AHC_ERR_NONE)
	{
		UINT32 ulReadCount = 0;
		
		err = AHC_FS_FileRead(ulFileId, gaBurnInMAC, 6, &ulReadCount);
		AHC_FS_FileClose(ulFileId);
		
		if (err != AHC_ERR_NONE || ulReadCount != 6)
		{
			printc("Backup MAC Failed.err = %d ,ulReadCount = %d \r\n",err,ulReadCount);
			return AHC_FALSE;
		}
		else
		{
			printc("Backup MAC Succeeded.\r\n");
		}
	}
	else
	{
		printc("%s N/A.\r\n",path);
		return AHC_FALSE;
	}	
	
	return AHC_TRUE;
}

void	SDUpdateBurnInMAC(void)
{
	char		*path = SF_MAC_FILENAME;
	AHC_ERR		err;
	UINT32		ulFileId;
	
	//Check MAC data
	if(gaBurnInMAC[0] == 0 && gaBurnInMAC[1] == 0 && gaBurnInMAC[2] == 0){
		printc("No MAC data to Burn In\r\n");
		return ;
	}
	
	//Check if the file exist,remove it if it exist.
	err = AHC_FS_FileOpen(path, STRLEN(path),"rb", sizeof("rb"), &ulFileId);
	if (err == AHC_ERR_NONE)
	{
		AHC_FS_FileClose(ulFileId);
		AHC_FS_FileRemove(path,STRLEN(path));
	}
	
	//Open and create file for burn in.
	err = AHC_FS_FileOpen(path, STRLEN(path),"wb", sizeof("wb"), &ulFileId);
	
	if (err == AHC_ERR_NONE)
	{
		UINT32 ulWriteCount;
		
		err = AHC_FS_FileWrite(ulFileId, gaBurnInMAC, 6, &ulWriteCount);
		AHC_FS_FileClose(ulFileId);

		if (err != AHC_ERR_NONE || ulWriteCount != 6)
		{
			printc("BurnIn MAC Failed.err = %d ,ulWriteCount = %d \r\n",err,ulWriteCount);
		}
		else
		{
			printc("BurnIn MAC Succeeded.\r\n");
			#if(SD_MAC_REMOVE_SOURCE)
			AHC_FS_FileRemove(SD_MAC_FILENAME,STRLEN(SD_MAC_FILENAME));
			#endif
		}
		
	}
	else
	{
		printc("SDUpdateBurnInMAC : %s open failed.\r\n",path);

	}
	
}
#endif
