#ifndef _CARDV_MENUSTATE_MISC_H_
#define _CARDV_MENUSTATE_MISC_H_

typedef enum {

    ITEMID_EXIT_SELECT = 1,
    ITEMID_EXIT_OK,
    ITEMID_EXIT_CANCEL
}EXIT_ITEMID;

typedef enum {

    ITEMID_EXIT_SETTING_YES = 1,
    ITEMID_EXIT_SETTING_NO
} POWERONSETTINGITEMID;

typedef enum {

    ITEMID_TOP_VIDEO = 1,
    ITEMID_TOP_CAMERA,
    ITEMID_TOP_PLAYBACK,
    ITEMID_TOP_MEDIA,
    ITEMID_TOP_GENERAL,
    ITEMID_TOP_LANGUAGE,
    ITEMID_TOP_ALLSETTING
} TOPMENUITEMID;

AHC_BOOL MenuItemExitMenu(PSMENUITEM pItem, AHC_BOOL bIsHover);

void 	 MenuStatePowerOnSettingMode( UINT32 ulEvent, UINT32 ulPosition );
AHC_BOOL MenuItemExitPowerOnSetting(PSMENUITEM pItem, AHC_BOOL bIsHover);
UINT32 	 SubMenuEventHandler_ExitPowerOnSet(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);

#if (POWER_ON_MENU_SET_EN)
UINT32 MenuStatePowerOnSettingModeHandler(UINT32 ulMsgId, UINT32 ulEvent, UINT32 ulParam );
AHC_BOOL MenuStatePowerOnSettingModeInit(void* pData);
AHC_BOOL MenuStatePowerOnSettingModeShutDown(void* pData);
#endif

#endif
