#ifndef _UPDATERX_H_
#define _UPDATERX_H_


#include "config_fw.h"
#include "mmp_err.h"
#include "mmpf_typedef.h"
#define UPDATERX_ENABLE (SD_FW_UPDATER)

#if (UPDATERX_ENABLE)

typedef enum _UPDATERX_ERR{

    UPDATERX_ERR_NONE,
  
    UPDATERX_ERR_NO_FW,
    UPDATERX_ERR_EXISTED_FW,
    UPDATERX_ERR_LOAD_FW_FAIL,
        
    UPDATERX_ERR_NO_LOG,
    UPDATERX_ERR_EXISTED_LOG,
    UPDATERX_ERR_LOAD_LOG_FAIL,
    UPDATERX_ERR_SAVE_LOG_FAIL,
    UPDATERX_ERR_DEL_LOG_FAIL,
    
    UPDATERX_ERR_UPDATE,
    UPDATERX_ERR_ORIGINAL_BOOT,
    UPDATERX_ERR_EXTRA_BOOT,
    UPDATERX_ERR_SHUT_DOWN,
    
    UPDATERX_ERR_DONE
}UPDATERX_ERR;

typedef enum _FW_TYPE{
    FW_TYPE_BOOTLOADER,
    FW_TYPE_BOOTLOADERX,
    FW_TYPE_ALLFW
}FW_TYPE;


typedef enum _FW_UPDATER_STAGE{

    FW_UPDATER_STAGE_NONE, 
    FW_UPDATER_STAGE_LOAD,
    FW_UPDATER_STAGE_FAIL, 
    FW_UPDATER_STAGE_SUCCESS
    
}FW_UPDATER_STAGE;

typedef enum _FW_UPDATE_PROCESS{

    FW_UPDATE_PROCESS_NONE,
    FW_UPDATE_PROCESS_VERIFYING,
    FW_UPDATE_PROCESS_VERIFIED,
    FW_UPDATE_PROCESS_FINAL
    
}FW_UPDATE_PROCESS;

typedef struct _FW_UPDATER_LOG{

    FW_UPDATE_PROCESS eUpdateProcess;
    FW_UPDATER_STAGE eExtraBootloaderX;
    FW_UPDATER_STAGE eExtraALLFW;
    FW_UPDATER_STAGE eBootloaderX;
    FW_UPDATER_STAGE eALLFW;

}FW_UPDATER_LOG;

UPDATERX_ERR UpdaterX_DeleteLog(void);
UPDATERX_ERR UpdaterX_CheckFlow(FW_TYPE eCurType);
UPDATERX_ERR UpdaterX_GetExtraFWInfo(FW_TYPE eType, MMP_ULONG *puiPos, MMP_ULONG *puiSize);
UPDATERX_ERR UpdaterX_LoadExtraFW(MMP_ULONG uiAddr, MMP_ULONG uiPos, MMP_ULONG uiSize);
#endif///< #if (UPDATERX_ENABLE)
#endif