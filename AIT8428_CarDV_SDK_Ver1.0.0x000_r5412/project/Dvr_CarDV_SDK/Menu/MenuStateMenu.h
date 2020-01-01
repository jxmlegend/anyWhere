
#ifndef _MENU_STATE_MENU_H_
#define _MENU_STATE_MENU_H_

/*===========================================================================
 * Include file 
 *===========================================================================*/ 
#include "Customer_Config.h"

#include "AHC_Common.h"
#include "AHC_Gui.h"
#include "MenuCommon.h"

/*===========================================================================
 * Enum define 
 *===========================================================================*/ 

typedef enum {
    ITEMID_MENU= 1,
    ITEMID_SLIDESHOW
} ITEMID;

/*===========================================================================
 * Function prototype
 *===========================================================================*/ 

int 	 GetCatagoryMenuID(PSMENUSTRUCT pMenu);
UINT32   QuickMenuMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam );
UINT32   QuickMenuMenuEventHandler_Playback(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam );
UINT32   MainMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam );
UINT32 MenuEditConfirmEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32   SubMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam );
UINT32   ThirdMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam );
UINT32   QuickMenuSubMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam );
UINT32   MenuGetDefault(PSMENUSTRUCT pMenu );
AHC_BOOL CommonMenuOK( PSMENUSTRUCT pMenu, AHC_BOOL bHover );
AHC_BOOL MenuModePreCheck(UINT32 ulEvent);

UINT32  TopMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);

UINT32 	SubMenuEventHandler_EditAllFile(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_ClockSetting(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_ResetSetup(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_FwVersionInfo(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_GPSInfoChart(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_EV(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_FormatSDCard(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_StorageInfo(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 	SubMenuEventHandler_Volume(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);

#if MENU_GENERAL_LDWS_CALIBRATION_EN
UINT32  SubMenuEventHandler_LdwsCalibration(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32  SubMenuEventHandler_UserLdwsCalibration(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
#endif

UINT32 SubMenuEventHandler_Contrast(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 SubMenuEventHandler_Saturation(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 SubMenuEventHandler_Sharpness(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
UINT32 SubMenuEventHandler_Gamma(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam);
#if(SUPPORT_TOUCH_PANEL)//leo20180630

UINT32 MainMenuItem_Touch(UINT16 pt_x,UINT16 pt_y);
UINT32 SubMenuItem_Touch(UINT16 pt_x,UINT16 pt_y);
UINT32 SubMenuItem_TouchThird(UINT16 pt_x,UINT16 pt_y);
UINT32 ThirdMenuItem_Touch(UINT16 pt_x,UINT16 pt_y);
UINT32 SubMenuItem_TouchPLAYBACK(UINT16 pt_x,UINT16 pt_y);
UINT32 SubMenu_Touchadjust(UINT16 pt_x,UINT16 pt_y);
UINT32  SubMenuComfirm_Touch(UINT16 pt_x,UINT16 pt_y);
//leo20180702
void DrawSubItemsettingvolt(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemResolution(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemLcd(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemLogo(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemGsensor(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemYesNo1(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemYesNo(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemFormat(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemsettingvoice(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemPLAYBACK(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);
void DrawSubItemsettingvolt(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect);

#endif

#endif //_MENU_STATE_MENU_H_

