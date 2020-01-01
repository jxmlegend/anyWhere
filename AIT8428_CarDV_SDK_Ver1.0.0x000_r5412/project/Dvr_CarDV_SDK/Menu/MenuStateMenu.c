/*===========================================================================
 * Include file 
 *===========================================================================*/ 

#include "AHC_Utility.h"
#include "AHC_MACRO.h"
#include "AHC_Menu.h"
#include "AHC_Message.h"
#include "AHC_GUI.h"
#include "AHC_General.h"
#include "AHC_Parameter.h"
#include "AHC_Display.h"
#include "AHC_Browser.h"
#include "AHC_Warningmsg.h"
#include "AHC_General_CarDV.h"
#include "MenuCommon.h"
#include "MenuTouchButton.h"
#include "MenuStateMenu.h"
#include "MenuDrawingFunc.h"
#include "MenuDrawCommon.h"
#include "MenuStateMenu.h"
#include "MenuSetting.h"
#include "IconPosition.h"
#include "ColorDefine.h"
#include "OsdStrings.h"
#include "IconDefine.h"
#include "StateBrowserFunc.h"
#ifdef _OEM_MENU_
#include "Oem_Menu.h"
#endif
#include "StateTVFunc.h"
/*===========================================================================
 * Global variable
 *===========================================================================*/ 
#define  ONE_TOUCH_MENU


PSMENUSTRUCT MenuPageList[] = 
{
#if (MENU_MOVIE_PAGE_EN)
	&sMainMenuVideo,
#endif
#if (MENU_STILL_PAGE_EN)	
	&sMainMenuStill,
#endif
#if (MENU_PLAYBACK_PAGE_EN)	
	&sMainMenuPlayback,
#endif
#if (MENU_MEDIA_PAGE_EN)		
	&sMainMenuMedia,
#endif
#if (MENU_GENERAL_PAGE_EN)	
	&sMainMenuGeneral,
#endif
#if (MENU_WIFI_PAGE_EN)
	&sMainMenuWifi,
#endif
#if (EXIT_MENU_PAGE_EN)	
	&sMainMenuExit,
#endif	
};

AHC_BOOL	m_ubAtMenuTab = AHC_FALSE;

/*===========================================================================
 * Extern variable
 *===========================================================================*/ 
  extern  UINT32 firstin;
extern  UINT32		Selectfile ;//leo20180702

extern AHC_BOOL 	Deleting_InBrowser;
extern AHC_BOOL 	Protecting_InBrowser;
extern AHC_BOOL   	bShowHdmiWMSG;
extern AHC_BOOL   	bShowTvWMSG;
//extern AHC_BOOL 	bForceSwitchBrowser;

extern AHC_BOOL 	Protecting_InBrowser;//leo20180630
extern AHC_BOOL 	bForceSwitchBrowser;//leo20180630
extern SMENUSTRUCT sSubMovieSettingvolt;
extern SMENUSTRUCT sSubMovieSettingvolt1;
extern SMENUSTRUCT sSubMovieSettingvolt2;

#if (VIRTUAL_KEY_BOARD)
extern UINT32	SubMenuEventHandler_Keyboard(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam );
#endif

/*===========================================================================
 * Main body
 *===========================================================================*/

#if 0
void ________Touch_Function_______(){ruturn;} //dummy
#endif

#if 0
void ________MainSubMenu_Function_______(){ruturn;} //dummy
#endif

#define MENU_PAGE_NUM 	sizeof(MenuPageList)/sizeof(PSMENUSTRUCT)

PSMENUSTRUCT GetPrevCatagoryMenu(PSMENUSTRUCT pMenu)
{
	int i, Prev;
	
	for(i=0; i<MENU_PAGE_NUM; i++)
	{
		if(pMenu->iMenuId==MenuPageList[i]->iMenuId)
		{
			Prev = (i==0)?(MENU_PAGE_NUM-1):(i-1);
			return (MenuPageList[Prev]);
		}
	}

	return &sMainMenuVideo;
}

PSMENUSTRUCT GetNextCatagoryMenu(PSMENUSTRUCT pMenu)
{
	int i, Next;
	
	for(i=0; i<MENU_PAGE_NUM; i++)
	{
		if(pMenu->iMenuId==MenuPageList[i]->iMenuId)
		{
			Next = ((i+1)==MENU_PAGE_NUM)?(0):(i+1);
			return (MenuPageList[Next]);
		}
	}

	return &sMainMenuVideo;
}

int GetCatagoryMenuID(PSMENUSTRUCT pMenu)
{
    if     ( pMenu == &sMainMenuManual   ) { return 0;  }
    else if( pMenu == &sMainMenuVideo    ) { return 1;  }
    else if( pMenu == &sMainMenuStill    ) { return 2;  }
    else if( pMenu == &sMainMenuPlayback ) { return 3;  }
    else if( pMenu == &sMainMenuEdit     ) { return 4;  }
    else if( pMenu == &sMainMenuMedia    ) { return 5;  }
    else if( pMenu == &sMainMenuGeneral  ) { return 6;  }
    else if( pMenu == &sMainMenuExit	 ) { return 7;  }
#if (MENU_WIFI_PAGE_EN)
	else if( pMenu == &sMainMenuWifi	 ) { return 8;	}
#endif
    else                                   { return 0;  }
}

AHC_BOOL CommonMenuOK(PSMENUSTRUCT pMenu, AHC_BOOL bHover)
{
    INT32		 	i,level;
    PSMENUITEM 		pCurItem;
    PSMENUSTRUCT 	pSubMenu;

    if( pMenu == NULL )
    {
    	return AHC_FALSE;
    }

    if( bHover == AHC_FALSE )
	    i = pMenu->iSelected;
    else
	    i = pMenu->iCurrentPos;

    pCurItem = pMenu->pItemsList[i];
    pSubMenu = pCurItem->pSubMenu;

    if(pSubMenu->iMenuId==MENUID_SUB_MENU_SETTINGVOLT
		||pSubMenu->iMenuId==MENUID_SUB_MENU_SETTINGVOLT1
		||pSubMenu->iMenuId==MENUID_SUB_MENU_SETTINGVOLT2)
    {
	pf_General_EnGet(COMMON_KEY_B_VOL, &level);
	printc("CommonMenuOK--level=%d\n",level);
	if(level>820&&level<=1300)
		pSubMenu=&sSubMovieSettingvolt1;
	else if(level>1300)
		pSubMenu=&sSubMovieSettingvolt2;
	else 
		pSubMenu=&sSubMovieSettingvolt;
    }
    if( pSubMenu != NULL )
    {
        pSubMenu->pParentMenu = pMenu;
        SetCurrentMenu(pSubMenu);

        pSubMenu->pfEventHandler( pSubMenu, MENU_INIT, 0 );
		Deleting_InBrowser = 0;
		Protecting_InBrowser = 0;
  
        return AHC_FALSE;
    }
    else if( pCurItem->pfItemSelect != NULL )
    {
        pCurItem->pfItemSelect( pCurItem, bHover );
        
        return AHC_TRUE;
    }
    else
    {
        return AHC_FALSE;
    }
}

UINT32 MenuGetDefault(PSMENUSTRUCT pMenu)
{
	return 0;
}

AHC_BOOL MenuModePreCheck(UINT32 ulEvent)
{
#if (TVOUT_PREVIEW_EN==0 && HDMI_PREVIEW_EN==0)

    if(BUTTON_HDMI_DETECTED == ulEvent)
    {
        if(AHC_IsHdmiConnect() && bShowHdmiWMSG)
        {
            bShowHdmiWMSG = AHC_FALSE;
            AHC_WMSG_Draw(AHC_TRUE, WMSG_HDMI_TV, 3);
        }         
    }
    
    if( BUTTON_TV_DETECTED == ulEvent)
    {
        if(AHC_IsTVConnectEx() && bShowTvWMSG) 
        {
            bShowTvWMSG = AHC_FALSE;
            AHC_WMSG_Draw(AHC_TRUE, WMSG_HDMI_TV, 3);
        }    
    }

    if(BUTTON_HDMI_REMOVED == ulEvent)
    {
        bShowHdmiWMSG = AHC_TRUE;
    }
    
    if(BUTTON_TV_REMOVED == ulEvent)
    {
        bShowTvWMSG = AHC_TRUE;
    }
#endif

    if(BUTTON_CLEAR_WMSG == ulEvent)
    {
        AHC_WMSG_Draw(AHC_FALSE, WMSG_NONE, 0);
    }
    
    return AHC_TRUE;
}

UINT32 MenuEditConfirmEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam)
{
    INT32 	iPrevPos;
    
#if(SUPPORT_TOUCH_PANEL)    
    POINT	TouchPoint;

    if( MENU_TOUCH == ulEvent || MENU_TOUCH_MOVE == ulEvent ){
        TouchPoint.PointX = ulParam & 0xFFFF;
        TouchPoint.PointY = (ulParam >> 16) & 0xFFFF;
    }
#endif

    switch(ulEvent)
    {
        case MENU_EXIT:
            return MENU_ERR_EXIT;
            break;

        case MENU_INIT:
            pMenu->uiStatus = MENU_STATUS_NONE;
            pMenu->bSetOne  = 0;
            pMenu->iCurrentPos = pMenu->iSelected;

#if (SUPPORT_TOUCH_PANEL)
            KeyParser_TouchItemRegister(&MainMenu_TouchButton[0], ITEMNUM(MainMenu_TouchButton));
#endif

#if (EXIT_MENU_PAGE_EN)  
            if(pMenu == (&sMainMenuExit))
                MenuDrawExitMainPage( pMenu );
            else
#endif
                MenuDrawMainPage( pMenu );
            break;

        case MENU_UP            :
            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber( pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, -1, AHC_TRUE );

#if (EXIT_MENU_PAGE_EN)
            if(pMenu == (&sMainMenuExit))
                MenuDrawChangeExitItem( pMenu, pMenu->iCurrentPos, iPrevPos);
            else
#endif
                MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
            break;

        case MENU_DOWN          :
            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber( pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, 1, AHC_TRUE);

#if (EXIT_MENU_PAGE_EN)
            if(pMenu == (&sMainMenuExit)) 
                MenuDrawChangeExitItem( pMenu, pMenu->iCurrentPos, iPrevPos);
            else
#endif
                MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected ); 
            break;

        case MENU_LEFT          :
            {
#ifdef CFG_MENU_TOP_PAGE_SD //may be defined in config_xxx.h
                break;
#else
                PSMENUSTRUCT pNewMenu;

                pNewMenu = GetPrevCatagoryMenu( pMenu );
                SetCurrentMenu( pNewMenu );
                pNewMenu->pfEventHandler ( pNewMenu, MENU_INIT, 0 );
#endif
            }
            break;

        case MENU_RIGHT         :
            {	
#ifdef CFG_MENU_TOP_PAGE_SD //may be defined in config_xxx.h
                    break;
#else
                PSMENUSTRUCT pNewMenu;

                pNewMenu = GetNextCatagoryMenu( pMenu );


                SetCurrentMenu( pNewMenu );
                pNewMenu->pfEventHandler ( pNewMenu, MENU_INIT, 0 );
#endif
            }
            break;

        case MENU_OK            :
        case MENU_SET_ONE		:

            if(ulEvent==MENU_SET_ONE){	
                pMenu->iCurrentPos = ulParam;
                pMenu->iSelected   = ulParam;
                pMenu->bSetOne   = 1;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
                pMenu->uiStatus |= MENU_STATUS_ITEM_TOUCHED;
            }
            else{
                pMenu->bSetOne   = 1;
                pMenu->iSelected = pMenu->iCurrentPos;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
            }

            if( CommonMenuOK( pMenu, AHC_TRUE ) == AHC_TRUE ){

				#if (TOP_MENU_PAGE_EN)
                PSMENUSTRUCT pCurrMenu = NULL;

                SetCurrentMenu(&sTopMenu);
                pCurrMenu = GetCurrentMenu();
                if( pCurrMenu == NULL ){
                    return MENU_ERR_EXIT;
                }

                pCurrMenu->pfEventHandler(pCurrMenu, MENU_INIT, 0);
				#endif
				
                return MENU_ERR_EXIT;
            }
            break;

        case MENU_MENU          :
            {
                PSMENUSTRUCT pParentMenu;

                pMenu->bSetOne     = 0;
                pParentMenu        = pMenu->pParentMenu;
                pMenu->pParentMenu = NULL;

                BrowserFunc_ThumbnailEditFilelComplete();
                if( pParentMenu == NULL ){
                    return MENU_ERR_EXIT;
                }

                SetCurrentMenu(pParentMenu);
                pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);
            }
            break;

        default:
            return MENU_ERR_OK;//MENU_ERR_NOT_MENU_EVENT;
            break;
    }

    return MENU_ERR_OK;
}

UINT32 MainMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam)//leo20180630
{
    INT32 	iPrevPos;
    
#if(SUPPORT_TOUCH_PANEL)    
    POINT	TouchPoint;

    if( MENU_TOUCH == ulEvent || MENU_TOUCH_MOVE == ulEvent ){
        TouchPoint.PointX = ulParam & 0xFFFF;
        TouchPoint.PointY = (ulParam >> 16) & 0xFFFF;
    }
#endif

    switch(ulEvent)
    {
        case MENU_EXIT:
            return MENU_ERR_EXIT;
            break;
		case EVENT_USB_DETECT:
			return MENU_EXIT_DO_PARENT_EVENT;
			break;
			
        case MENU_INIT:
            pMenu->uiStatus = MENU_STATUS_NONE;
            pMenu->bSetOne  = 0;
            pMenu->iCurrentPos = pMenu->iSelected;

#if (SUPPORT_TOUCH_PANEL)
            KeyParser_TouchItemRegister(&MainMenu_TouchButton[0], ITEMNUM(MainMenu_TouchButton));
#endif

#if (EXIT_MENU_PAGE_EN)  
            if(pMenu == (&sMainMenuExit))
                MenuDrawExitMainPage( pMenu );
            else
#endif
                MenuDrawMainPage( pMenu );
            break;

        case MENU_UP            :
            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber( pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, -1, AHC_TRUE );

#if (EXIT_MENU_PAGE_EN)
            if(pMenu == (&sMainMenuExit))
                MenuDrawChangeExitItem( pMenu, pMenu->iCurrentPos, iPrevPos);
            else
#endif
                MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
            break;

        case MENU_DOWN          :
            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber( pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, 1, AHC_TRUE);

#if (EXIT_MENU_PAGE_EN)
            if(pMenu == (&sMainMenuExit)) 
                MenuDrawChangeExitItem( pMenu, pMenu->iCurrentPos, iPrevPos);
            else
#endif
                MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected ); 
            break;

        case MENU_LEFT          :
            {
#ifdef CFG_MENU_TOP_PAGE_SD //may be defined in config_xxx.h
                break;
#else
                PSMENUSTRUCT pNewMenu;

                pNewMenu = GetPrevCatagoryMenu( pMenu );
                SetCurrentMenu( pNewMenu );
                pNewMenu->pfEventHandler ( pNewMenu, MENU_INIT, 0 );
#endif
            }
            break;

        case MENU_RIGHT         :
            {	
#ifdef CFG_MENU_TOP_PAGE_SD //may be defined in config_xxx.h
                break;
#else
                PSMENUSTRUCT pNewMenu;

                pNewMenu = GetNextCatagoryMenu( pMenu );


                SetCurrentMenu( pNewMenu );
                pNewMenu->pfEventHandler ( pNewMenu, MENU_INIT, 0 );
#endif
            }
            break;

        case MENU_OK            :
        case MENU_SET_ONE		:

            if(ulEvent==MENU_SET_ONE){	
                pMenu->iCurrentPos = ulParam;
                pMenu->iSelected   = ulParam;
                pMenu->bSetOne   = 1;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
                pMenu->uiStatus |= MENU_STATUS_ITEM_TOUCHED;
            }
            else{
                pMenu->bSetOne   = 1;
                pMenu->iSelected = pMenu->iCurrentPos;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
            }

            if( CommonMenuOK( pMenu, AHC_TRUE ) == AHC_TRUE ){
#if (TOP_MENU_PAGE_EN)
                PSMENUSTRUCT pCurrMenu = NULL;

                SetCurrentMenu(&sTopMenu);
                pCurrMenu = GetCurrentMenu();
                if( pCurrMenu == NULL ){
                    return MENU_ERR_EXIT;
                }

                pCurrMenu->pfEventHandler(pCurrMenu, MENU_INIT, 0);
#endif
                return MENU_ERR_OK;
            }
            break;

        case MENU_MENU          :
			printc("~~~~~~~~sssssssAAAAAAAAAA~~~~~~~\r\n");
        
        	if(Deleting_InBrowser || Protecting_InBrowser){
            	printc("Delete/Protect/UnProtect Change to Browser Mode2\r\n");
            	// To check is there file in card, it no any file
                // to reset Delete/(Un)Protect file in broswer flags.
                // Otherwise those flag will make key/device events to
                // be NULL!! UI will be stuck
                BrowserFunc_ThumbnailEditCheckFileObj();
                return MENU_ERR_FORCE_BROWSER_EXIT;
            }
            else
            {
                PSMENUSTRUCT pParentMenu;

                pMenu->bSetOne     = 0;
                pParentMenu        = pMenu->pParentMenu;
                pMenu->pParentMenu = NULL;

                if( pParentMenu == NULL ){
                    return MENU_ERR_EXIT;
                }

                SetCurrentMenu(pParentMenu);
                pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);
            }
            break;

        default:
            return MENU_ERR_OK;//MENU_ERR_NOT_MENU_EVENT;
            break;
    }

    return MENU_ERR_OK;
}
/*
UINT32 MainMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam)
{
    INT32 	iPrevPos;
    
#if(SUPPORT_TOUCH_PANEL)    
    POINT	TouchPoint;

    if( MENU_TOUCH == ulEvent || MENU_TOUCH_MOVE == ulEvent ){
        TouchPoint.PointX = ulParam & 0xFFFF;
        TouchPoint.PointY = (ulParam >> 16) & 0xFFFF;
    }
#endif

    switch(ulEvent)
    {
        case MENU_EXIT:
            return MENU_ERR_EXIT;
            break;
		case EVENT_USB_DETECT:
			return MENU_EXIT_DO_PARENT_EVENT;
			break;
			
        case MENU_INIT:
            pMenu->uiStatus = MENU_STATUS_NONE;
            pMenu->bSetOne  = 0;
            pMenu->iCurrentPos = pMenu->iSelected;

#if (SUPPORT_TOUCH_PANEL)
            KeyParser_TouchItemRegister(&MainMenu_TouchButton[0], ITEMNUM(MainMenu_TouchButton));
#endif

#if (EXIT_MENU_PAGE_EN)  
            if(pMenu == (&sMainMenuExit))
                MenuDrawExitMainPage( pMenu );
            else
#endif
                MenuDrawMainPage( pMenu );
            break;

        case MENU_UP            :
            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber( pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, -1, AHC_TRUE );

#if (EXIT_MENU_PAGE_EN)
            if(pMenu == (&sMainMenuExit))
                MenuDrawChangeExitItem( pMenu, pMenu->iCurrentPos, iPrevPos);
            else
#endif
                MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
            break;

        case MENU_DOWN          :
            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber( pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, 1, AHC_TRUE);

#if (EXIT_MENU_PAGE_EN)
            if(pMenu == (&sMainMenuExit)) 
                MenuDrawChangeExitItem( pMenu, pMenu->iCurrentPos, iPrevPos);
            else
#endif
                MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected ); 
            break;

        case MENU_LEFT          :
            {
#ifdef CFG_MENU_TOP_PAGE_SD //may be defined in config_xxx.h
                break;
#else
                PSMENUSTRUCT pNewMenu;

                pNewMenu = GetPrevCatagoryMenu( pMenu );
                SetCurrentMenu( pNewMenu );
                pNewMenu->pfEventHandler ( pNewMenu, MENU_INIT, 0 );
#endif
            }
            break;

        case MENU_RIGHT         :
            {	
#ifdef CFG_MENU_TOP_PAGE_SD //may be defined in config_xxx.h
                break;
#else
                PSMENUSTRUCT pNewMenu;

                pNewMenu = GetNextCatagoryMenu( pMenu );


                SetCurrentMenu( pNewMenu );
                pNewMenu->pfEventHandler ( pNewMenu, MENU_INIT, 0 );
#endif
            }
            break;

        case MENU_OK            :
        case MENU_SET_ONE		:

            if(ulEvent==MENU_SET_ONE){	
                pMenu->iCurrentPos = ulParam;
                pMenu->iSelected   = ulParam;
                pMenu->bSetOne   = 1;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
                pMenu->uiStatus |= MENU_STATUS_ITEM_TOUCHED;
            }
            else{
                pMenu->bSetOne   = 1;
                pMenu->iSelected = pMenu->iCurrentPos;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
            }

            if( CommonMenuOK( pMenu, AHC_TRUE ) == AHC_TRUE ){
#if (TOP_MENU_PAGE_EN)
                PSMENUSTRUCT pCurrMenu = NULL;

                SetCurrentMenu(&sTopMenu);
                pCurrMenu = GetCurrentMenu();
                if( pCurrMenu == NULL ){
                    return MENU_ERR_EXIT;
                }

                pCurrMenu->pfEventHandler(pCurrMenu, MENU_INIT, 0);
#endif
                return MENU_ERR_OK;
            }
            break;

        case MENU_MENU          :
        
        	if(Deleting_InBrowser || Protecting_InBrowser){
            	printc("Delete/Protect/UnProtect Change to Browser Mode2\r\n");
            	// To check is there file in card, it no any file
                // to reset Delete/(Un)Protect file in broswer flags.
                // Otherwise those flag will make key/device events to
                // be NULL!! UI will be stuck
                BrowserFunc_ThumbnailEditCheckFileObj();
                return MENU_ERR_FORCE_BROWSER_EXIT;
            }
            else
            {
                PSMENUSTRUCT pParentMenu;

                pMenu->bSetOne     = 0;
                pParentMenu        = pMenu->pParentMenu;
                pMenu->pParentMenu = NULL;

                if( pParentMenu == NULL ){
                    return MENU_ERR_EXIT;
                }

                SetCurrentMenu(pParentMenu);
                pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);
            }
            break;

        default:
            return MENU_ERR_OK;//MENU_ERR_NOT_MENU_EVENT;
            break;
    }

    return MENU_ERR_OK;
}
*/
AHC_BOOL flag_p = 1;
extern void MenuDrawChangeItem2(AHC_BOOL changeP);
#if (SUPPORT_TOUCH_PANEL)
UINT32 MainMenuItem_Touch(UINT16 pt_x,UINT16 pt_y)
{
	UINT32  uiNextEvent = MENU_NONE;
	UINT32  TouchEvent;
	UINT16  i = 0;
	PSMENUSTRUCT pMenu;
	int iPrevPos;
	
	pMenu=GetCurrentMenu();

  	//i = (pt_y - OSD_MENU_ITEM_Y )/( OSD_MENU_ITEM_H + OSD_MENU_ITEM_INTERVAL );
   // i = (pMenu->iCurrentPos/MAIN_MENU_PAGE_ITEM)*MAIN_MENU_PAGE_ITEM + i;
 {
   	if(pt_x<175)
   	{
		if(pt_y>55&&pt_y<165)
			i=0;
		if(pt_y>175&&pt_y<285)
			i=1;
	}
	if(pt_x>175&&pt_x<325)
	{
		i=2;
	}
	if(pt_x>345)
	{
		i=3;
	} 
	printc("MainMenuItem_Touch %d,ofItem %d\r\n",i,pMenu->iNumOfItems);
	if(i < pMenu->iNumOfItems)
	{
		#ifdef ONE_TOUCH_MENU
			//else
			if(i == 3)
			{
				printc("~~~~3333333~~~~~~~~~\r\n");
				if(flag_p == 1)
					flag_p = 0;
				else
					flag_p = 1;
			 	 MenuDrawChangeItem2(flag_p);
			}// lyj 20180711
			else
			{
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			}
			AHC_OS_SleepMs(20);
			if(i ==  pMenu->iCurrentPos)
				MainMenuEventHandler(pMenu,MENU_SET_ONE,i);
			}
			
		#else
			if(i ==  pMenu->iCurrentPos)
				MainMenuEventHandler(pMenu,MENU_SET_ONE,i);
			else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			}
		#endif
	}
	return MENU_ERR_OK;
}
}
#endif
UINT32 SubMenuEventHandler_settingvolt(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam)
{
	INT32 	iPrevPos;
	INT32	level;
	//printc("SubMenuEventHandler_settingvoltpMenu->iMenuId=%dUINT32 ulEvent%d\n",pMenu->iMenuId, ulEvent);
	switch(ulEvent)
	{
		case MENU_LCD_NORMAL:
		case MENU_LCD_ROTATE:
			AHC_DrawMenuRotateEvent(ulEvent ,pMenu);
		break;

		case MENU_SDMMC_IN:
		case MENU_SDMMC_OUT:
			MenuDraw_SD_Status();
		break;

		case MENU_BATTERY_DETECTION:
		case MENU_UPDATE_MESSAGE:
		break;

		case MENU_EXIT:
			return MENU_ERR_EXIT;
			break;

		case MENU_INIT:
			CHARGE_ICON_ENABLE(AHC_FALSE);

				pMenu->uiStatus = MENU_STATUS_NONE;
					
					if( pMenu->pfMenuGetDefaultVal )
					{
						pMenu->iSelected = pMenu->pfMenuGetDefaultVal(pMenu);
					}
					else
					{
						pMenu->iSelected = 0 ;
					}
						printc("pMenu->iSelected=%d\n",pMenu->iSelected);
					pMenu->iCurrentPos = pMenu->iSelected;
					pMenu->bSetOne	   = 0;
			
			
		#if (SUPPORT_TOUCH_PANEL)
					printc("SubMenuEventHandler---pMenu->iNumOfItems=%d,ulParam=%d\n",pMenu->iNumOfItems,ulParam);
					printc("11pMenu->iMenuId=%d\n",pMenu->iMenuId); 
						KeyParser_TouchItemRegister(&SubMenusetvolt_TouchButton[0], ITEMNUM(SubMenusetvolt_TouchButton));
						MenuDrawSubPage(pMenu);
				
		#endif			
						
			break;		

		

		case MENU_MENU:
			{
				PSMENUSTRUCT	pParentMenu;
				pParentMenu= &sSubMovieSetting;

				//spParentMenu = pMenu->pParentMenu;
				pMenu->pParentMenu = NULL;
				pMenu->bSetOne = 0;

				if (pParentMenu == NULL)
				{
					return MENU_ERR_EXIT;
				}

				SetCurrentMenu(pParentMenu);
				pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);

			}
			break;
			case MENU_OK			:
				case MENU_SET_ONE		:
				
					pMenu->bSetOne	 = 1;
					iPrevPos		 = pMenu->iCurrentPos;
					
					if(ulEvent==MENU_SET_ONE)
					{		  
						pMenu->iCurrentPos = ulParam;
						pMenu->iSelected   = ulParam;
						pMenu->uiStatus |= MENU_STATUS_PRESSED;
						pMenu->uiStatus |= MENU_STATUS_ITEM_TOUCHED;	 
					}
					else
					{
						pMenu->iSelected = pMenu->iCurrentPos;
						pMenu->uiStatus |= MENU_STATUS_PRESSED;
					}
					
					if( CommonMenuOK( pMenu, AHC_TRUE ) == AHC_TRUE )
					{			
						if(Deleting_InBrowser || Protecting_InBrowser)
						{
							printc("Delete/Protect/UnProtect Change to Browser Mode2\r\n");
							// To check is there file in card, it no any file
							// to reset Delete/(Un)Protect file in broswer flags.
							// Otherwise those flag will make key/device events to
							// be NULL!! UI will be stuck
							BrowserFunc_ThumbnailEditCheckFileObj();
			
							
							
							return MENU_ERR_EXIT;
						}
						else
						{
			
							PSMENUSTRUCT pParentMenu;
							
							pParentMenu 	   = pMenu->pParentMenu;
							pMenu->pParentMenu = NULL;
			
							if( pParentMenu == NULL )
							{
								return MENU_ERR_EXIT;
							}
							printc("SetCurrentMenu pMenu->iMenuId=%d\n",pMenu->iMenuId);
							SetCurrentMenu(pParentMenu);
							pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0); 
							
						}
			
						return MENU_ERR_OK;
					}
				break;

		default:
			break;
	}

	return MENU_ERR_OK;
}
UINT32 SubMenuEventHandler_settingVOICE(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam)
{
	INT32 	iPrevPos;
	//printc("SubMenuEventHandler_settingvoltpMenu->iMenuId=%dUINT32 ulEvent%d\n",pMenu->iMenuId, ulEvent);
	switch(ulEvent)
	{
		case MENU_LCD_NORMAL:
		case MENU_LCD_ROTATE:
			AHC_DrawMenuRotateEvent(ulEvent ,pMenu);
		break;

		case MENU_SDMMC_IN:
		case MENU_SDMMC_OUT:
			MenuDraw_SD_Status();
		break;

		case MENU_BATTERY_DETECTION:
		case MENU_UPDATE_MESSAGE:
		break;

		case MENU_EXIT:
			return MENU_ERR_EXIT;
			break;

		case MENU_INIT:
			CHARGE_ICON_ENABLE(AHC_FALSE);


				pMenu->uiStatus = MENU_STATUS_NONE;
					
					if( pMenu->pfMenuGetDefaultVal )
					{
						pMenu->iSelected = pMenu->pfMenuGetDefaultVal(pMenu);
					}
					else
					{
						pMenu->iSelected = 0 ;
					}
					
				printc("pMenu->iSelected=%d\n",pMenu->iSelected);
					pMenu->iCurrentPos = pMenu->iSelected;
					pMenu->bSetOne	   = 0;
			
			
		#if (SUPPORT_TOUCH_PANEL)
					printc("SubMenuEventHandler---pMenu->iNumOfItems=%d,ulParam=%d\n",pMenu->iNumOfItems,ulParam);
					 
					KeyParser_TouchItemRegister(&SubMenusetvoice_TouchButton[0], ITEMNUM(SubMenusetvoice_TouchButton));
						MenuDrawSubPage(pMenu);
				
				
		#endif			
						
			break;		

		

		case MENU_MENU:
			{
				PSMENUSTRUCT	pParentMenu;

				pParentMenu = pMenu->pParentMenu;
				pMenu->pParentMenu = NULL;
				pMenu->bSetOne = 0;

				if (pParentMenu == NULL)
				{
					return MENU_ERR_EXIT;
				}

				SetCurrentMenu(pParentMenu);
				pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);

			}
			break;
			case MENU_OK			:
				case MENU_SET_ONE		:
				
					pMenu->bSetOne	 = 1;
					iPrevPos		 = pMenu->iCurrentPos;
					
					if(ulEvent==MENU_SET_ONE)
					{		  
						pMenu->iCurrentPos = ulParam;
						pMenu->iSelected   = ulParam;
						pMenu->uiStatus |= MENU_STATUS_PRESSED;
						pMenu->uiStatus |= MENU_STATUS_ITEM_TOUCHED;	 
					}
					else
					{
						pMenu->iSelected = pMenu->iCurrentPos;
						pMenu->uiStatus |= MENU_STATUS_PRESSED;
					}
					
					if( CommonMenuOK( pMenu, AHC_TRUE ) == AHC_TRUE )
					{			
						if(Deleting_InBrowser || Protecting_InBrowser)
						{
							printc("Delete/Protect/UnProtect Change to Browser Mode2\r\n");
							// To check is there file in card, it no any file
							// to reset Delete/(Un)Protect file in broswer flags.
							// Otherwise those flag will make key/device events to
							// be NULL!! UI will be stuck
							BrowserFunc_ThumbnailEditCheckFileObj();
			
							
							
							return MENU_ERR_EXIT;
						}
						else
						{
							#ifndef ONE_TOUCH_MENU
								PSMENUSTRUCT pParentMenu;
								
								pParentMenu 	   = pMenu->pParentMenu;
								pMenu->pParentMenu = NULL;
				
								if( pParentMenu == NULL )
								{
									return MENU_ERR_EXIT;
								}
								printc("SetCurrentMenu pMenu->iMenuId=%d\n",pMenu->iMenuId);
								SetCurrentMenu(pParentMenu);
								pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0); 
							#endif 
						}
			
						return MENU_ERR_OK;
					}
				break;

		default:
			break;
	}

	return MENU_ERR_OK;
}
INT32 SubMenuEventHandler_playback(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam)
{
	INT32 	iPrevPos;
	//printc("SubMenuEventHandler_settingvoltpMenu->iMenuId=%dUINT32 ulEvent%d\n",pMenu->iMenuId, ulEvent);
	switch(ulEvent)
	{
		case MENU_LCD_NORMAL:
		case MENU_LCD_ROTATE:
			AHC_DrawMenuRotateEvent(ulEvent ,pMenu);
		break;

		case MENU_SDMMC_IN:
		case MENU_SDMMC_OUT:
			MenuDraw_SD_Status();
		break;

		case MENU_BATTERY_DETECTION:
		case MENU_UPDATE_MESSAGE:
		break;

		case MENU_EXIT:
			return MENU_ERR_EXIT;
			break;

		case MENU_INIT:
			CHARGE_ICON_ENABLE(AHC_FALSE);

				pMenu->uiStatus = MENU_STATUS_NONE;
					
					if( pMenu->pfMenuGetDefaultVal )
					{
						pMenu->iSelected = pMenu->pfMenuGetDefaultVal(pMenu);
					}
					else
					{
						pMenu->iSelected = 0 ;
					}
					pMenu->iCurrentPos = pMenu->iSelected;
					pMenu->bSetOne	   = 0;
			
			
		#if (SUPPORT_TOUCH_PANEL)
					printc("SubMenuEventHandler---pMenu->iNumOfItems=%d,ulParam=%d\n",pMenu->iNumOfItems,ulParam);
					 if(pMenu->iMenuId==MENUID_SUB_MENU_PLAYBACK)
					{

						ResetCurrentMenu();

					Menu_WriteSetting();
					bForceSwitchBrowser = AHC_TRUE;
		        	StateSwitchMode(UI_BROWSER_STATE);
					}
					else
						MenuDrawSubPage(pMenu);
				
				
		#endif			
						
			break;		

		

		case MENU_MENU:
			{
				PSMENUSTRUCT	pParentMenu;

				pParentMenu = pMenu->pParentMenu;
				pMenu->pParentMenu = NULL;
				pMenu->bSetOne = 0;

				if (pParentMenu == NULL)
				{
					return MENU_ERR_EXIT;
				}

				SetCurrentMenu(pParentMenu);
				pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);

			}
			break;
			case MENU_OK			:
				case MENU_SET_ONE		:
				
					pMenu->bSetOne	 = 1;
					iPrevPos		 = pMenu->iCurrentPos;
					
					if(ulEvent==MENU_SET_ONE)
					{		  
						pMenu->iCurrentPos = ulParam;
						pMenu->iSelected   = ulParam;
						pMenu->uiStatus |= MENU_STATUS_PRESSED;
						pMenu->uiStatus |= MENU_STATUS_ITEM_TOUCHED;	 
					}
					else
					{
						pMenu->iSelected = pMenu->iCurrentPos;
						pMenu->uiStatus |= MENU_STATUS_PRESSED;
					}
					
					if( CommonMenuOK( pMenu, AHC_TRUE ) == AHC_TRUE )
					{			
						if(Deleting_InBrowser || Protecting_InBrowser)
						{
							printc("Delete/Protect/UnProtect Change to Browser Mode2\r\n");
							// To check is there file in card, it no any file
							// to reset Delete/(Un)Protect file in broswer flags.
							// Otherwise those flag will make key/device events to
							// be NULL!! UI will be stuck
							BrowserFunc_ThumbnailEditCheckFileObj();
			
							
							
							return MENU_ERR_EXIT;
						}
						else
						{
							#ifndef ONE_TOUCH_MENU
								PSMENUSTRUCT pParentMenu;
								
								pParentMenu 	   = pMenu->pParentMenu;
								pMenu->pParentMenu = NULL;
				
								if( pParentMenu == NULL )
								{
									return MENU_ERR_EXIT;
								}
								printc("SetCurrentMenu pMenu->iMenuId=%d\n",pMenu->iMenuId);
								SetCurrentMenu(pParentMenu);
								pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0); 
							#endif
							
						}
			
						return MENU_ERR_OK;
					}
				break;

		default:
			break;
	}

	return MENU_ERR_OK;
}


//Common SubMenuEventHandler.
UINT32 SubMenuEventHandler(PSMENUSTRUCT pMenu, UINT32 ulEvent, UINT32 ulParam)
{
    INT32 	iPrevPos;
#if(SUPPORT_TOUCH_PANEL)       
    POINT	TouchPoint;
#endif

#if(SUPPORT_TOUCH_PANEL) 
    if( MENU_TOUCH == ulEvent || MENU_TOUCH_MOVE == ulEvent )
    {
        TouchPoint.PointX = ulParam & 0xFFFF;
        TouchPoint.PointY = (ulParam >> 16) & 0xFFFF;
    }
#endif
   switch(pMenu->iMenuId)
   {
   	case MENUID_SUB_MENU_SETTINGVOLT:
	case MENUID_SUB_MENU_SETTINGVOLT2:
	case MENUID_SUB_MENU_SETTINGVOLT1:
			return SubMenuEventHandler_settingvolt(pMenu, ulEvent, ulParam);
		break;
	case MENUID_SUB_MENU_SETTINGVOICE:
			return SubMenuEventHandler_settingVOICE(pMenu, ulEvent, ulParam);
		break;
	case MENUID_SUB_MENU_SETTINGTIME:
			return SubMenuEventHandler_ClockSetting1(pMenu, ulEvent, ulParam);
		break;
	default:	
		break;
   }
    switch(ulEvent)
    {
        case MENU_EXIT:
            return MENU_ERR_EXIT;
            break;	

        case MENU_INIT          :
            pMenu->uiStatus = MENU_STATUS_NONE;

            if( pMenu->pfMenuGetDefaultVal ){
                pMenu->iSelected = pMenu->pfMenuGetDefaultVal(pMenu);
            }
            else{
                pMenu->iSelected = 0 ;
            }
            pMenu->iCurrentPos = pMenu->iSelected;
            pMenu->bSetOne     = 0;

		printc("SubMenuEventHandler--pMenu->iCurrentPos=%d\n",pMenu->iCurrentPos);
		#if (SUPPORT_TOUCH_PANEL)
		printc("SubMenuEventHandler---pMenu->iNumOfItems=%d\n",pMenu->iNumOfItems);
		if( pMenu->iNumOfItems <=2 )
		{
			if(pMenu->iMenuId==MENUID_SUB_MENU_MIC||pMenu->iMenuId==MENUID_SUB_MENU_MOVIE_MODE || pMenu->iMenuId==MENUID_SUB_MENU_SETTINGSYSTEM)
			{		
					if(pMenu->iMenuId==MENUID_SUB_MENU_MIC)
         			KeyParser_TouchItemRegister(&SubMenu2_TouchButton1[0], ITEMNUM(SubMenu2_TouchButton1));
					if(pMenu->iMenuId==MENUID_SUB_MENU_MOVIE_MODE )
					{
						printc("~~~~w~~~~~~^^^^^^^adas666666~~~~~~~~\r\n");
						KeyParser_TouchItemRegister(&SubMenu2e_TouchButton1[0], ITEMNUM(SubMenu2e_TouchButton1));
					}
					if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGSYSTEM)
					{
						printc("~~~~222222~~~~~~^^^^^^^adas666666~~~~~~~~\r\n");
						KeyParser_TouchItemRegister(&SubMenu2c_TouchButton1[0], ITEMNUM(SubMenu2c_TouchButton1));
					}
			}
			else
				KeyParser_TouchItemRegister(&SubMenu2_TouchButton[0], ITEMNUM(SubMenu2_TouchButton));
		}
		else if( pMenu->iNumOfItems <=3)
		{
			if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGPARKING)
			{
					printc("~~~JJJJJJJJJ~~~enter~~~~~\r\n");
					KeyParser_TouchItemRegister(&SubMenu3e_TouchButton[0], ITEMNUM(SubMenu3e_TouchButton));
			}
			if(pMenu->iMenuId==MENUID_SUB_MENU_PLAYBACK)
         			KeyParser_TouchItemRegister(&SubMenuPLAYBACK_TouchButton[0], ITEMNUM(SubMenuPLAYBACK_TouchButton));
         	 if(pMenu->iMenuId!=MENUID_SUB_MENU_SETTINGPARKING && pMenu->iMenuId!=MENUID_SUB_MENU_PLAYBACK)
				KeyParser_TouchItemRegister(&SubMenu3_TouchButton[0], ITEMNUM(SubMenu3_TouchButton));
		}
       	else  if( pMenu->iNumOfItems <=4 )
        	{
         		KeyParser_TouchItemRegister(&SubMenu4_TouchButton[0], ITEMNUM(SubMenu4_TouchButton));
		}
		else if(pMenu->iNumOfItems <=5)
		{
			printc("~~~KKKKKKKK~~~enter~~~~~\r\n");
			if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGBATTERYH)
			{
				printc("~~~KKKKKKKK111~~~enter~~~~~\r\n");
				KeyParser_TouchItemRegister(&SubMenu5e_TouchButton[0], ITEMNUM(SubMenu5e_TouchButton));
			}
		}
		else if( pMenu->iNumOfItems <= 6)
		{
			KeyParser_TouchItemRegister(&SubMenu6_TouchButton[0], ITEMNUM(SubMenu6_TouchButton));
		}
		else if( pMenu->iNumOfItems <= 7)
		{
          	KeyParser_TouchItemRegister(&SubMenu7_TouchButton[0], ITEMNUM(SubMenu7_TouchButton));
		}
		else if( pMenu->iNumOfItems <= 8)
		{
			if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGTIME)
			{
				printc("~~~~~~~~999999999999999999~~cc~~\r\n");
          	KeyParser_TouchItemRegister(&SubMenu_settingtime[0], ITEMNUM(SubMenu_settingtime));
			}
			else
			{
				printc("~~~~~~~4444444444444444~~~~~dd~~~~~~~\r\n");
				KeyParser_TouchItemRegister(&SubMenu8_TouchButton[0], ITEMNUM(SubMenu8_TouchButton));
			}
		}
		#endif	
		//printc("pMenu->pItemsList->iItemId=%d\n",pMenu->pItemsList[0]->iItemId);
	
		if(pMenu->iMenuId==MENUID_SUB_MENU_PLAYBACK0)
		{
			//ResetCurrentMenu();
			//Menu_WriteSetting();
			firstin=1;
			Selectfile=1;
		   	bForceSwitchBrowser = AHC_TRUE;
		   	StateSwitchMode(UI_BROWSER_STATE);
		 }
		else if(pMenu->iMenuId==MENUID_SUB_MENU_PLAYBACK1)
		{
			firstin=1;
			Selectfile=2;

			bForceSwitchBrowser = AHC_TRUE;
		   	StateSwitchMode(UI_BROWSER_STATE);
		}
		else if(pMenu->iMenuId==MENUID_SUB_MENU_PLAYBACK2)
		{
			firstin=1;
			Selectfile=0;

			bForceSwitchBrowser = AHC_TRUE;
		   	StateSwitchMode(UI_BROWSER_STATE);
		}
		else
	    		MenuDrawSubPage(pMenu);
	break;

	case MENU_UP            :

		#ifdef SLIDE_MENU
		if(IsSlidingMenu())
			break;
		#endif
		
        iPrevPos = pMenu->iCurrentPos;

		#if (DIR_KEY_TYPE==KEY_TYPE_2KEY)
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, -1, AHC_TRUE);
		#elif defined(FONT_LARGE)
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, -1, AHC_FALSE);
		#else
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, -2, AHC_FALSE);
		#endif
            MenuDrawChangeSubItem(pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected);
            break;

	case MENU_DOWN          :

		#ifdef SLIDE_MENU
		if(IsSlidingMenu())
			break;
		#endif

            iPrevPos = pMenu->iCurrentPos;

		#if (DIR_KEY_TYPE==KEY_TYPE_2KEY)
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, 1, AHC_TRUE);
		#elif defined(FONT_LARGE)
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, 1, AHC_FALSE);
		#else
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, 2, AHC_FALSE);
		#endif
            MenuDrawChangeSubItem(pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected);
            break;

	#if (DIR_KEY_TYPE==KEY_TYPE_4KEY)
        case MENU_LEFT          :

		#ifdef SLIDE_MENU
            if(IsSlidingMenu())
                break;
		#endif

            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, -1, AHC_FALSE);

            MenuDrawChangeSubItem(pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected);
            break;

        case MENU_RIGHT          :

		#ifdef SLIDE_MENU
            if(IsSlidingMenu())
                break;
		#endif

            iPrevPos = pMenu->iCurrentPos;
            pMenu->iCurrentPos = OffsetItemNumber(pMenu->iCurrentPos, 0, pMenu->iNumOfItems-1, 1, AHC_FALSE);

            MenuDrawChangeSubItem(pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected);
            break;
	#endif

        case MENU_OK            :
        case MENU_SET_ONE		:

            pMenu->bSetOne   = 1;
            iPrevPos      	 = pMenu->iCurrentPos;

            if(ulEvent==MENU_SET_ONE){         
                pMenu->iCurrentPos = ulParam;
                pMenu->iSelected   = ulParam;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
                pMenu->uiStatus |= MENU_STATUS_ITEM_TOUCHED;     
            }
            else{
                pMenu->iSelected = pMenu->iCurrentPos;
                pMenu->uiStatus |= MENU_STATUS_PRESSED;
            }

            if( CommonMenuOK( pMenu, AHC_TRUE ) == AHC_TRUE )
            {			
                if(Deleting_InBrowser || Protecting_InBrowser){
                    printc("Delete/Protect/UnProtect Change to Browser Mode2\r\n");
                    // To check is there file in card, it no any file
                    // to reset Delete/(Un)Protect file in broswer flags.
                    // Otherwise those flag will make key/device events to
                    // be NULL!! UI will be stuck
                    BrowserFunc_ThumbnailEditCheckFileObj();
					if(AHC_CheckCurSysMode(AHC_MODE_PLAYBACK))
					{
						Deleting_InBrowser = 0;
						Protecting_InBrowser = 0;
					}
                    return MENU_ERR_FORCE_BROWSER_EXIT;
                }
                else
			{
				#ifndef ONE_TOUCH_MENU
					PSMENUSTRUCT pParentMenu;
					
		        		pParentMenu        = pMenu->pParentMenu;
		        		pMenu->pParentMenu = NULL;

		        		if( pParentMenu == NULL )
		        		{
			        		return MENU_ERR_EXIT;
		        		}
		        		SetCurrentMenu(pParentMenu);
		        		pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);	
				#endif

				 if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGFORMATE1)
				 {
					PSMENUSTRUCT pParentMenu;
					
		        		pParentMenu        = pMenu->pParentMenu;
		        		pMenu->pParentMenu = NULL;

		        		if( pParentMenu == NULL )
		        		{
			        		return MENU_ERR_EXIT;
		        		}
		        		SetCurrentMenu(pParentMenu);
		        		pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);	
				 }
                }

                return MENU_ERR_OK;
            }
            break;

        case MENU_MENU          :
            {     
                {
                    PSMENUSTRUCT pParentMenu;

                    pMenu->bSetOne     = 0;
                    pParentMenu        = pMenu->pParentMenu;
                    pMenu->pParentMenu = NULL;

                    if( pParentMenu == NULL ){
                        return MENU_ERR_EXIT;
                    }

                    SetCurrentMenu(pParentMenu);
                    pParentMenu->pfEventHandler(pParentMenu, MENU_INIT, 0);
                }
            }
        break;
	#if 0
        case MENU_POWER_OFF     :
            AHC_PowerOff_NormalPath();
            break; 
	#endif

        default:
            return MENU_ERR_OK;//MENU_ERR_NOT_MENU_EVENT;        
            break;
    }

    return MENU_ERR_OK;
}

#if (SUPPORT_TOUCH_PANEL)
UINT32 SubMenuItem_Touch(UINT16 pt_x,UINT16 pt_y)
{
	UINT16  i = 0;
	PSMENUSTRUCT pMenu;
	int iPrevPos;
	UINT32 ret = MENU_ERR_OK;
	
	pMenu=GetCurrentMenu();
	
    if( pMenu->iNumOfItems >= 7 )
    {
    		if(pt_y<=215)
			i =  (pt_x- 35 )/( OSD_SUBMENU60_W + OSD_MENU_ITEM_TOUCH_INTERVAL );
		else
			i =  (pt_x- 35 )/( OSD_SUBMENU60_W + OSD_MENU_ITEM_TOUCH_INTERVAL )+4;
		if(i>7)
			i=7;
    }
    else if( pMenu->iNumOfItems <= 6)
    {
		if(pt_y<=215)
			i =  (pt_x- OSD_SUBMENU60_X )/( OSD_SUBMENU60_W + OSD_MENU_ITEM_TOUCH_INTERVAL );
		else
			i =  (pt_x- OSD_SUBMENU60_X )/( OSD_SUBMENU60_W + OSD_MENU_ITEM_TOUCH_INTERVAL )+3;
    }
    else
    {
		
    }
	printc("SubMenuItem_Touch %d,ofItem %d\r\n",i,pMenu->iNumOfItems);
	printc("SubMenuItem_Touch %d,iCurrentPos %d\r\n",i,pMenu->iCurrentPos);

	if(i < pMenu->iNumOfItems)
	{
		#ifdef ONE_TOUCH_MENU
			//else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			}
			AHC_OS_SleepMs(20);
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			
			#else
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItem( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			}
		#endif
	}
	printc(FG_RED("SubMenuItem_Touch ret %d\r\n"),ret);
	if(ret == MENU_ERR_EXIT)
	{
		#ifdef SLIDE_MENU
		if(IsSlidingMenu())
			StopSlideMenu();
		#endif    
    
        ResetCurrentMenu();

		Menu_WriteSetting();
        
        if(Deleting_InBrowser || Protecting_InBrowser)
        {
        	//bForceSwitchBrowser = AHC_TRUE;
        	StateSwitchMode(UI_BROWSER_STATE);
        }
       
    }	
	return MENU_ERR_OK;	
}

UINT32  SubMenuComfirm_Touch(UINT16 pt_x,UINT16 pt_y)
{
	UINT16  i = 0;
	UINT32 uParam;
	UINT32 ret = MENU_ERR_OK;
	PSMENUSTRUCT pMenu;
	RECT rc = RECT_TOUCH_BUTTON_MENU_YES;
	#define STR_GAP		(30)
	
	pMenu = GetCurrentMenu();
	i = (pt_x -rc.uiLeft)/(rc.uiWidth+ STR_GAP);
	if(i==0)
		uParam = 0;
	else
		uParam = 1;
	ret = pMenu->pfEventHandler(pMenu,MENU_SET_ONE,uParam);
	
	return MENU_ERR_OK;
}

UINT8	touchtime = 0;

UINT32 ThirdMenuItem_Touch(UINT16 pt_x,UINT16 pt_y)//leo20180630
{
	UINT16  i = 0;
	PSMENUSTRUCT pMenu;
	int iPrevPos;
	UINT32 ret = MENU_ERR_OK;
	
	pMenu=GetCurrentMenu();
	
	printc("ThirdMenuItem_iCurrentPos %d,pMenu->iMenuId %d\r\n",pMenu->iCurrentPos,pMenu->iMenuId);
	printc("ThirdMenuItem_ pt_x %d, pt_y %d\r\n",pt_x, pt_y);
	
    if(pMenu->iMenuId == MENUID_SUB_MENU_SETTINGVOLT||
	pMenu->iMenuId == MENUID_SUB_MENU_SETTINGVOLT1||
	pMenu->iMenuId == MENUID_SUB_MENU_SETTINGVOLT2)
    {
		if (pt_y<215)
			{
			if(pt_x<150)i=0;
				else i=1;
			}
		else{
			if(pt_x<150)i=2;
			else if (pt_x<270)i=3;
			else i=4;
		}
    }
    else if( pMenu->iMenuId == MENUID_SUB_MENU_SETTINGVOICE )
    {
		if (pt_y<215)
			{
			i=(pt_x-120)/85;
			}
		else{
			if(pt_x<360)i=4;
			else i=5;
		}
    }
    else if( pMenu->iMenuId == MENUID_SUB_MENU_SETTINGTIME )
    {

		if( pMenu->iNumOfItems <= 8 )
   
			if(pt_x<135)i=0;
		else if(pt_x<230)i=1;
		else if(pt_x<335)i=2;
		else i=8;
			
		if(pt_y>180)i=i+3;
		printc("ThirdMenuItem_Touch 111*****i=%d\r\n",i);
		if(i < pMenu->iNumOfItems)
	{
		#ifdef ONE_TOUCH_MENU
			//else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
					printc("@@@@@@@@@@@@@@@ 111\r\n");
				MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
		#ifdef AUTO_RETURN
				if(pMenu->iMenuId ==MENUID_SUB_MENU_SETTINGTIME);
					else
				ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
		#endif
			}
			AHC_OS_SleepMs(20);
			if(i ==  pMenu->iCurrentPos)
				printc("ThirdMenuItem_Touch 111\r\n");
			
			#else
			if(i ==  pMenu->iCurrentPos)
				printc("ThirdMenuItem_Touch 111\r\n");
			else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			#ifdef AUTO_RETURN	
				ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
			#endif
			}
		#endif
		
	}else
	{
		UINT8	  bID0 = 0;
		UINT8	  bID1 = 0;

	printc("ThirdMenuItem_Touch 111pMenu->iCurrentPos=%di=%d\r\n",pMenu->iCurrentPos,i);
	OSDDraw_EnterMenuDrawing(&bID0, &bID1);
	DrawSubItem_ClockSetting1(bID0, pMenu->iCurrentPos ,i);
	OSDDraw_ExitMenuDrawing(&bID0, &bID1);


	}
    return MENU_ERR_OK;
    }
	else{

	};
	printc("ThirdMenuItem_Touch %d,ofItem %d pMenu->iCurrentPos%d\r\n",i,pMenu->iNumOfItems, pMenu->iCurrentPos);

	if(i < pMenu->iNumOfItems)
	{
		#ifdef ONE_TOUCH_MENU
			//else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				printc("@@@@@@@@@@@@@@@ 222\r\n");
				printc("@@@@@@@@@@@@@@@ pMenu->iMenuId=%d\r\n",pMenu->iMenuId);
				MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			#ifdef AUTO_RETURN
				if((pMenu->iMenuId != MENUID_SUB_MENU_SETTINGVOLT)&&(pMenu->iMenuId != MENUID_SUB_MENU_SETTINGVOLT2) &&(pMenu->iMenuId != MENUID_SUB_MENU_SETTINGVOLT1))

				ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
			#endif
			}
			AHC_OS_SleepMs(20);
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			
		#else
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			#ifdef AUTO_RETURN
				ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
			#endif
			}
		#endif

		
	}
	printc(FG_RED("ThirdMenuItem_Touch ret %d\r\n"),ret);
	if(ret == MENU_ERR_EXIT)
	{
		#ifdef SLIDE_MENU
		if(IsSlidingMenu())
			StopSlideMenu();
		#endif    
    	printc("ThirdMenuItem_Touch ResetCurrentMenu\n");
        ResetCurrentMenu();

		Menu_WriteSetting();
        
        if(Deleting_InBrowser || Protecting_InBrowser)
        {
        	bForceSwitchBrowser = AHC_TRUE;
        	StateSwitchMode(UI_BROWSER_STATE);
        }
       
    }	
	return MENU_ERR_OK;	
}

UINT32 SubMenuItem_TouchThird(UINT16 pt_x,UINT16 pt_y)//leo20180630
{
	UINT16  i = 0;
	PSMENUSTRUCT pMenu;
	int iPrevPos;
	UINT32 ret = MENU_ERR_OK;
	
	pMenu=GetCurrentMenu();
		if(pMenu->iMenuId==MENUID_MAIN_MENU_VIDEO)
			{ return MENU_ERR_OK;}
	if(pMenu->iMenuId==MENUID_SUB_MENU_PLAYBACK)
	  {
		printc("MENUID_SUB_MENU_PLAYBACK %d,pt_y %d\r\n",pt_x,pt_y);
	  	printc("MENUID_SUB_MENU_PLAYBACK %d,ofItem %d\r\n",i,pMenu->iNumOfItems);
			if(pt_x>=335&&pt_y<=70)
			{
				StateSwitchMode(UI_VIDEO_STATE);
				 //uiStateMachine( 30, 81, 18284971);
				//pMenu=(&sMainMenuVideo);
				//MainMenuEventHandler(pMenu,MENU_INIT,0);
				//SubMenuEventHandler(pMenu,MENU_MENU,0);
				 return MENU_ERR_OK;
			}
			
		  	if( pMenu->iNumOfItems <= 3 )
		    	{
				i =  ( pt_x- OSD_SUBMENUPLAYBACK_X )/( OSD_SUBMENUPLAYBACK_W + 30 );
		    	}
		 	  if(i < pMenu->iNumOfItems)
			   {
				   	#ifdef ONE_TOUCH_MENU
						//else
						{
							iPrevPos = pMenu->iCurrentPos;
							pMenu->iCurrentPos = i;
							MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
							
						}
						//AHC_OS_SleepMs(20);
						if(i ==  pMenu->iCurrentPos)
							ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
						#ifdef AUTO_RETURN	
							ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
						#endif
						
					#else
						if(i ==  pMenu->iCurrentPos)
							ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
						else
						{
							iPrevPos = pMenu->iCurrentPos;
							pMenu->iCurrentPos = i;
							MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
						#ifdef AUTO_RETURN
							ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
						#endif
						}
					#endif

			   }
	
	 return MENU_ERR_OK;
	
	 }

    if( pMenu->iNumOfItems <= 2 )
    {
		if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGSYSTEM)
		{
			i =  ( pt_x- 118 )/( 124 + 4 );	
		}
		else
		{
    		if(pMenu->iMenuId==MENUID_SUB_MENU_MIC||pMenu->iMenuId==MENUID_SUB_MENU_MOVIE_MODE)
			i =  ( pt_x- OSD_SUBMENU22_X )/( OSD_SUBMENU22_W + 20 );		
		else
			i =  ( pt_x- OSD_SUBMENU20_X )/( OSD_SUBMENU20_W + 20 );
		}
    }	
    else if( pMenu->iNumOfItems <= 3 )
    {
    	printc("~AAA~~~pt_x = %d~~BBB~pt_y = %d~~~~~~~~\r\n",pt_x,pt_y);
		if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGPARKING)
		{
			printc("~CCC~~~pt_x = %d~~DDD~pt_y = %d~~~~~~~~\r\n",pt_x,pt_y);
			i = ( pt_x)/( 160 );
			if(i > 2)
				i = 2;
		}
		else
		i =  ( pt_x- OSD_SUBMENU30_X )/( OSD_SUBMENU30_W + 15 );
    }
    else if( pMenu->iNumOfItems <= 4 )
    {
		i =  ( pt_x- OSD_SUBMENU40_X )/( OSD_SUBMENU40_W + 20 );
    }
	else if(pMenu->iNumOfItems <= 5)
	{
		if(pMenu->iMenuId == MENUID_SUB_MENU_SETTINGBATTERYH)
		{
			i = ( pt_x )/( 96);
			printc("PPPPPP~~pMenu->iNumOfItems = %d~~~~i = %d~~~~~~\r\n",pMenu->iNumOfItems,i);
		}
    }
	printc("pMenu->iMenuId %d\r\n",pMenu->iMenuId);

	printc("!!SubMenuItem_TouchThird %d,ofItem %d\r\n",i,pMenu->iNumOfItems);
	printc("!!SubMenuItem_TouchThird %d,ofItem %d\r\n",i,pMenu->iCurrentPos);
	if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGTOUCH)
			{
				if(i == 0){
					UINT8	bID0 = 0;
								 UINT8	   bID1 = 0;
							  UINT8 	x = 0;
							 UINT8	   y = 0;
							 
								// AHC_TouchPanel_IOControl(0,Temp);
								// printc("***************** Temp[0]=%x ***********************Temp[1]%x\n", *Temp, *(Temp+1), *(Temp+2), *(Temp+3));
							//	  x= Temp[0]*16+Temp[1]/16;
								//  y = Temp[2]*16+Temp[3]/16;
								//	 printc("touch**********x=%d*************y=%d\n",x,y);
									   OSDDraw_EnterMenuDrawing(&bID0, &bID1);
								           OSDDraw_ClearOvlDrawBuffer(bID0);
									  //BEGIN_LAYER(bID0);
									 AHC_OSDSetBkColor(bID0,0x00050505);
									 AHC_OSDSetColor(bID0,0x00050505);
									 AHC_OSDDrawFillRect(bID0, 0, 0, 480, 320);
									 AHC_OSDSetColor(bID0, OSD_COLOR_RED);
									 AHC_OSDSetPenSize(bID0, 2);
									AHC_OSDDrawLine(bID0, 60,	60, 		50, 70);
									AHC_OSDDrawLine(bID0, 50,	70, 		60,60);
					
									 // END_LAYER(bID0);
									  OSDDraw_ExitMenuDrawing(&bID0, &bID1);

					// KeyParser_TouchItemRegister(&Touchadjust_TouchButton[0], ITEMNUM(Touchadjust_TouchButton));
					SubMenu_Touchadjust(0,0);
					pMenu->iCurrentPos=0;
					// printc("*****************  SubMenu_Touchadjust(0,0);***********************\n");
	
				}
					 else
						 {
						 iPrevPos = pMenu->iCurrentPos;
							pMenu->iCurrentPos = i;
					#ifdef AUTO_RETURN
						 SubMenuEventHandler(pMenu,MENU_MENU,0);
					#endif
						MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
						touchtime=0;
						return MENU_ERR_OK;
	
					 }
	
	
	
			//return MENU_ERR_OK; 
	
			}

	if(i < pMenu->iNumOfItems)
	{
		#ifdef ONE_TOUCH_MENU
			//else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
				
			AHC_OS_SleepMs(20);
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			if((pMenu->iMenuId==MENUID_SUB_MENU_SETTINGTOUCH)||(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGFORMATE)||(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGFORMATE1));
					else{
						printc("*************************************************\n");
						printc("**************pMenu->iMenuId=%d***********************************\n",pMenu->iMenuId);

						printc("*************************************************\n");
					#ifdef AUTO_RETURN
						ret = SubMenuEventHandler(pMenu,MENU_MENU,i);

					#endif
						}
			}
			
		#else
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			#ifdef AUTO_RETURN
				ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
			#endif
			}
		#endif
	}
	printc(FG_RED("SubMenuItem_Touch ret %d\r\n"),ret);
	if(ret == MENU_ERR_EXIT)
	{
		#ifdef SLIDE_MENU
		if(IsSlidingMenu())
			StopSlideMenu();
		#endif    
    
        ResetCurrentMenu();

		Menu_WriteSetting();
        
        if(Deleting_InBrowser || Protecting_InBrowser)
        {
        	bForceSwitchBrowser = AHC_TRUE;
        	StateSwitchMode(UI_BROWSER_STATE);
        }
       
    }	
	return MENU_ERR_OK;	
}

UINT32 SubMenuItem_TouchPLAYBACK(UINT16 pt_x,UINT16 pt_y)//leo20180630
{
	UINT16  i = 0;
	PSMENUSTRUCT pMenu;
	int iPrevPos;
	UINT32 ret = MENU_ERR_OK;
	
	pMenu=GetCurrentMenu();
	
	printc("pMenu` %d\r\n",pMenu->iMenuId);
	printc("SubMenuItem_TouchThirdpt_x %d,pt_y %d\r\n",pt_x,pt_y);
	printc("SubMenuItem_TouchThird %d,ofItem %d\r\n",i,pMenu->iMenuId);
	    SubMenuEventHandler(pMenu,MENU_MENU,0);

	printc(FG_RED("SubMenuItem_Touch ret %d\r\n"),ret);
	
	return MENU_ERR_OK;	
}
//UINT8	touchtime = 0;
UINT16 touch_x[6];
UINT16 touch_y[6];
MMP_SHORT touch_depend[4];

//leo20180630
extern AHC_BOOL attach_flag1 ;
extern AHC_BOOL attach_flag2 ;
void DrawSubItemResolution(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc;
		UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		rc.uiLeft 	     =118+iItem%2*(IconID->XSize+4);
		rc.uiTop       =100 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 	

		/*const GUI_BITMAP* 	SubMenuLcdPushIcon[]={
														&bmIcon_hd_button,*/
		
	

		
	    	 printc("DrawSubItemResolution---iItem=%d;barId=%d\n",iItem,barId);
		
		if(barId&0xff)
		{	
			printc("~~~~~~UUUUU~~~~~\r\n");
			if(iItem == 0)
			{
				if(attach_flag1)
				{
					pf_FCWS_EnSet(FCWS_EN_ON);
					OSD_Draw_Icon(bmIcon_hd2_button, rc, uwDispID);
					//AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_FCWS_START, 0); 
				}
				else
				{
					 pf_FCWS_EnSet(FCWS_EN_OFF);
					OSD_Draw_Icon(*IconID, rc, uwDispID);
					//AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_FCWS_STOP, 0);
				}
			}
			if(iItem == 1)
			{
				if(attach_flag2)
				{
					pf_LDWS_EnSet(LDWS_EN_ON);
					OSD_Draw_Icon(bmIcon_hd_button, rc, uwDispID);
					//AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_LDWS_START, 0);
				}
				else
				{
					pf_LDWS_EnSet(LDWS_EN_OFF);
					OSD_Draw_Icon(*IconID, rc, uwDispID);
					//AHC_SendAHLMessage(AHLM_UI_NOTIFICATION, BUTTON_LDWS_STOP, 0);
				}
			}
		
		}
		else
		{
			printc("~~~~~~JJJJJ~~~~~\r\n");
		}
}
extern AHC_BOOL select_time_flag;
void DrawSubItemResolutionEX2(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc;
		UINT32  iMenuId = GetCurrentMenu()->iMenuId;
		rc.uiLeft 	     =118+iItem%2*(IconID->XSize+4);
		rc.uiTop       =100 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
	    	 printc("DrawSubItemResolution---iItem=%d;barId=%d\n",iItem,barId);
		if(barId&0xff)
		{	
			printc("~~~~~~UUUUU~~~~~\r\n");
			rc.uiTop       =295 ;
			OSD_Draw_Icon(bmPowerbatterys, rc, uwDispID);
		}
		else
		{
			//OSD_Draw_Icon(*IconID, rc, uwDispID);
			printc("~~~~~~JJJJJ~~~~~\r\n");
			if(select_time_flag == 0)
				OSD_Draw_Icon(bmbatterysetting,   rc,uwDispID);
			if(select_time_flag == 1)
				OSD_Draw_Icon(bmsfivehour,   rc,uwDispID);
			if(select_time_flag == 2)
				OSD_Draw_Icon(bmstenhour,   rc,uwDispID);
			if(select_time_flag == 3)
				OSD_Draw_Icon(bmstwentyfour,   rc,uwDispID);
			if(select_time_flag == 4)
				OSD_Draw_Icon(bmsfortyeight,   rc,uwDispID);
		}

}
//leo20180630
void DrawSubItemLcd(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuLcdPushIcon[]={&bmIcon_1min_button_push,
													&bmIcon_3min_button_push,
													&bmIcon_full_time_button_push,
													//&bmIcon_clock_button_push
													};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemLcd---iItem=%d;barId=%d\n",iItem,barId);

		rc.uiLeft 	     =70+iItem%3*(IconID->XSize+35);
		rc.uiTop       =iItem/4*(IconID->YSize)+215 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuLcdPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	

}

//leo20180630
extern AHC_BOOL display_flag1;
extern AHC_BOOL display_flag2;
extern AHC_BOOL display_flag3;
void DrawSubItemLogo(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
													&bmIcon_1_bt_sel,
													&bmIcon_2_bt_sel,
													&bmIcon_3_bt_sel,
													};
		const GUI_BITMAP* 	SubMenuGsensorPushIcon_1080[]={
													&bmIcon_1_bt_sel_1080,
													&bmIcon_2_bt_sel_1080,
													&bmIcon_3_bt_sel_1080,
													};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemLogo---iItem=%d\n",iItem);

		rc.uiLeft 	     =10+iItem%3*(IconID->XSize+5);
		rc.uiTop       =215 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
	#if (MENU_MOVIE_SIZE_1080P_EN||MENU_MOVIE_SIZE_SHD_30P_EN)
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuGsensorPushIcon_1080[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	
       #else
		if(barId&0xff)
		{
			rc.uiTop       =310 ;
			display_flag1 = iItem;
			OSD_Draw_Icon(bmselectBar, rc, uwDispID);
		}
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	
	#endif
}
void DrawSubItemYesNo3(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
														&bmIcon_yes_bt_sel,
														&bmIcon_no_bt_sel,
														};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;
	    	 printc("DrawSubItemYesNo---iItem=%d\n",iItem);
		rc.uiLeft 	     =10+iItem%3*(IconID->XSize+5);
		rc.uiTop       = 193 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
		{
			rc.uiTop       =310 ;
			if(MENUID_SUB_MENU_RESET == iMenuId)
			{
				display_flag2 = iItem;
				AHC_VIDEO_SetMovieConfig(0, AHC_VIDEO_COMPRESSION_RATIO, (UINT32)display_flag2);
			}
			else
			{
				display_flag3 = iItem;
			}
			OSD_Draw_Icon(bmselectBar, rc, uwDispID);
		}
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	
}
void DrawSubItemYesNo4(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
														&bmIcon_yes_bt_sel,
														&bmIcon_no_bt_sel,
														};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;
	    	 printc("DrawSubItemYesNo4---iItem=%d\n",iItem);
			rc.uiLeft 	     =(iItem)%3*(IconID->XSize);
			rc.uiTop       = 320-118 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
		{
				rc.uiTop       = 320-3 ;
			OSD_Draw_Icon(bmlongselect, rc, uwDispID);
		}
		else
		{
			OSD_Draw_Icon(*IconID, rc, uwDispID);	
		}
}
void DrawSubItemYesNo5(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;
	    	 printc("DrawSubItemYesNo---iItem=%d\n",iItem);
		rc.uiLeft 	     =iItem%5*(IconID->XSize+0);
		rc.uiTop       =135 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
		{
				rc.uiTop       = 281 ;
			select_time_flag = iItem;
			OSD_Draw_Icon(bmPowerbatterys2, rc, uwDispID);
			pf_General_EnSet(COMMON_KEY_VR_TIMELAPSE, (INT32)iItem);
		}
		else
		{
			OSD_Draw_Icon(*IconID, rc, uwDispID);	
		}
}

//leo20180630
void DrawSubItemGsensor(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
													&bmIcon_sensor_level1_button_push,
													&bmIcon_sensor_level2_button_push,
													&bmIcon_sensor_level3_button_push,
													};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemGsensor---iItem=%d\n",iItem);

		rc.uiLeft 	     =70+iItem%3*(IconID->XSize+35);
		rc.uiTop       =215 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuGsensorPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	

}

//leo20180630
void DrawSubItemYesNo1(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
														&bmIcon_mic_reco_on,
														&bmIcon_mic_info_off,
														};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemYesNo---iItem=%d\n",iItem);

		rc.uiLeft 	     =20+iItem%2*(IconID->XSize+20);
		rc.uiTop       = 150 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuGsensorPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	

}

//leo20180630
void DrawSubItemYesNo(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
														&bmIcon_yes_bt_sel,
														&bmIcon_no_bt_sel,
														};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemYesNo---iItem=%d\n",iItem);

		rc.uiLeft 	     =250+iItem%2*(IconID->XSize+20);
		rc.uiTop       = 215 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuGsensorPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	

}

//leo20180630
void DrawSubItemFormat(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuLcdPushIcon[]={&bmIcon_7day_button_push,
													&bmIcon_15day_button_push,
													&bmIcon_30day_button_push,
													&bmIcon_format_sdcard_push};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemLcd---iItem=%d;barId=%d\n",iItem,barId);

		rc.uiLeft 	     =30+iItem%4*(IconID->XSize+20);
		rc.uiTop       =iItem/4*(IconID->YSize)+155 ;
		rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuLcdPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);	

}
//leo20180630
void DrawSubItemsettingvoice(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
														
														&bmvol0_bt_sel,
														&bmvol1_bt_sel,
														&bmvol2_bt_sel,
														&bmvol3_bt_sel,
														&bmyes1_bt_sel,
														&bmno1_bt_sel,
														};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemsettingvoice---iItem=%d\n",iItem);
		
			if(iItem<4)
			{
				rc.uiLeft 	     =120+iItem*(80+5);
				rc.uiTop       = 115 ;
		
			}else
			{
				rc.uiLeft 	     =240+(iItem-4)*(109+11);
				rc.uiTop       = 215 ;
			}
			
			rc.uiWidth    =IconID->XSize; 
		rc.uiHeight    =IconID->YSize; 
		printc("DrawSubItemsettingvoice---rc.uiLeft=%d\n",rc.uiLeft);
		//printc("DrawSubItemsettingvoice---rc.uiTop=%d\n",rc.uiLeft);
		
		
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuGsensorPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);
		
		rc.uiLeft		=15 ;//x
				rc.uiTop	   =215 ;//y
				rc.uiWidth	  =109; 
				rc.uiHeight    =70; 
				OSD_Draw_Icon(bmtouch_sound, rc, uwDispID);	

}

//leo20180630
void DrawSubItemPLAYBACK(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
														
														&bmparking_rec_button_push,
														&bmevent_rec_button_push,
														&bmrec_button_push,
														
														};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		
	    	 printc("DrawSubItemPLAYBACK---iItem=%d\n",iItem);
		if(iItem<3)
			{
			rc.uiLeft 	     =60+iItem*(100+30);
			rc.uiTop       = 150 ;
			rc.uiWidth    =IconID->XSize; 
			rc.uiHeight    =IconID->YSize; 
			}
			
		printc("DrawSubItemsettingvoice---rc.uiLeft=%d\n",rc.uiLeft);
		//printc("DrawSubItemsettingvoice---rc.uiTop=%d\n",rc.uiLeft);
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuGsensorPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);
		
	

}

//leo20180630
void DrawSubItemsettingvolt(UINT16 uwDispID, int iItem, int iTotalItems, UINT8 uLang, UINT32 iStrID, const GUI_BITMAP* IconID, UINT32 barId, GUI_COLOR clrFont, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
	    	RECT    rc, tmpRECT;
		INT32  level;
		const GUI_BITMAP* 	SubMenuGsensorPushIcon[]={
														
														&bm9volt01_bt_sel,
														&bm9volt02_bt_sel,
														&bm9volt03_bt_sel,
														&bm9volt04_bt_sel,
														&bm9volt05_bt_sel,
														&bm12volt01_bt_sel,
														&bm12volt02_bt_sel,
														&bm12volt03_bt_sel,
														&bm12volt04_bt_sel,
														&bm12volt05_bt_sel,
														&bm24volt01_bt_sel,
														&bm24volt02_bt_sel,
														&bm24volt03_bt_sel,
														&bm24volt04_bt_sel,
														&bm24volt05_bt_sel,
														
														};
	    	UINT32  iMenuId = GetCurrentMenu()->iMenuId;

		  
	    	 printc("DrawSubItemsettingvolt---iItem=%d;iMenuId=%dbarId=%d\n",iItem,iMenuId,barId);
			 switch(iItem)
		{
			case 0:
				rc.uiLeft       =40 ;//x
				rc.uiTop       =115 ;//y
				
				break;
			
			case 1:
				rc.uiLeft       =150 ;//x
				rc.uiTop       =115 ;//y
			
				break;

			case 2:
				rc.uiLeft       =40 ;//x
				rc.uiTop       =215 ;//y
				
				break;
			case 3:
				rc.uiLeft       =150 ;//x
				rc.uiTop       =215 ;//y
					
				break;
			case 4:
				rc.uiLeft       =270 ;//x
				rc.uiTop       =215 ;//y
				
				break;
			
				
			default:
				break;
			
		
			}
			rc.uiWidth    =IconID->XSize; 
			rc.uiHeight    =IconID->YSize; 
		if(iMenuId== MENUID_SUB_MENU_SETTINGVOLT1)
			iItem=iItem+5;
		else if(iMenuId== MENUID_SUB_MENU_SETTINGVOLT2)
			iItem=iItem+10;	
			
		
				
		if(barId&0xff)
			OSD_Draw_Icon(*SubMenuGsensorPushIcon[iItem], rc, uwDispID);
		else
			OSD_Draw_Icon(*IconID, rc, uwDispID);

		
		rc.uiTop       =115 ;//y
		rc.uiWidth    =115; 
		rc.uiHeight    =70; 
		rc.uiLeft       =270 ;//x

		OSD_Draw_Icon(bmvolt_title02, rc, uwDispID);	
		
		pf_General_EnGet(COMMON_KEY_B_VOL, &level);
      		printc("DrawSubItemsettingvolt level=%d\n",level);  
		AHC_OSDSetFont(uwDispID, &GUI_Font24B_1);
    		AHC_OSDSetColor(uwDispID, OSD_COLOR_RED);
		AHC_OSDSetBkColor(uwDispID, 0x00202020);
		if(level<820)
		{
			AHC_OSDDrawDec(uwDispID, level*10/791,     300,    150, 1);
	    		AHC_OSDDrawText(uwDispID, (UINT8*)".",  300+15, 150, strlen("."));	
			AHC_OSDDrawDec(uwDispID, (level*100/791)%10,     300+20,    150, 1);	
			AHC_OSDDrawText(uwDispID, (UINT8*)"V",  300+30, 150, strlen("V"));
		}
		else if(level<1300)
		{
			AHC_OSDDrawDec(uwDispID, level*10/758,     300,    150, 2);
	    		AHC_OSDDrawText(uwDispID, (UINT8*)".",  300+25, 150, strlen("."));	
			AHC_OSDDrawDec(uwDispID, (level*100/758)%10,     300+30,    150, 1);	
			AHC_OSDDrawText(uwDispID, (UINT8*)"V",  300+40, 150, strlen("V"));
		}
		else
		{
			AHC_OSDDrawDec(uwDispID, level*10/694,     300,    150, 2);
	    		AHC_OSDDrawText(uwDispID, (UINT8*)".",  300+25, 150, strlen("."));	
			AHC_OSDDrawDec(uwDispID, (level*100/694)%10,     300+30,    150, 1);	
			AHC_OSDDrawText(uwDispID, (UINT8*)"V",  300+40, 150, strlen("V"));
		}
}

//leo20180630
void MenuDrawChangeSubItemThird(PSMENUSTRUCT pMenu, UINT32 uiCurrItem, UINT32 uiPrevItem, UINT32 uiPreSelected)
{
    UINT8     bID0 = 0;
    UINT8     bID1 = 0;
    UINT8     SubPageItem = SUB_MENU_PAGE_ITEM;
    UINT8     uLang;


    uLang = MenuDraw_GetDefaultLanguage(pMenu);

    {
        GUI_COLOR   clrFill     = OSD_COLOR_TRANSPARENT;
        GUI_COLOR   clrSelect   = 0x0;
        GUI_COLOR   clrText     = MENU_TEXT_COLOR;
        UINT32      idx;
        UINT32      barState;
        UINT32      barType     = BAR_STATA_NORMAL;

        OSDDraw_EnterMenuDrawing(&bID0, &bID1);

        printc("MenuDrawChangeSubItemThird ---pMenu->iMenuId=%d\n",pMenu->iMenuId);
	 printc("MenuDrawChangeSubItemThird ---pMenu->pMenu->iSelected=%d\n",pMenu->iSelected);
	 printc("MenuDrawChangeSubItemThird ---pMenu->uiPrevItem=%d\n",uiPrevItem);
	 printc("MenuDrawChangeSubItemThird ---pMenu->uiCurrItem=%d\n",uiCurrItem);
       if(uiCurrItem!=uiPrevItem)
       {
       	barState = BAR_STATA_NORMAL;
		 printc("MenuDrawChangeSubItemThird ---uiPreSelected\n");
		idx=uiPrevItem;	
		switch(pMenu->iMenuId)
		{
			case MENUID_SUB_MENU_LCD:
				 DrawSubItemLcd(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			case MENUID_SUB_MENU_LOGO:
				 DrawSubItemLogo(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			
			case MENUID_SUB_MENU_GSENSOR:
				 DrawSubItemGsensor(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;

			case MENUID_SUB_MENU_MIC:
				DrawSubItemYesNo1(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			case MENUID_SUB_MENU_RESET:
			case MENUID_SUB_MENU_GPS:
				DrawSubItemYesNo3(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			case MENUID_SUB_MENU_SETTINGPARKING:
					DrawSubItemYesNo4(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;// lyj 20180703
			case MENUID_SUB_MENU_SETTINGBATTERYH:
					DrawSubItemYesNo5(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;// lyj 20180704
			case MENUID_SUB_MENU_SETTINGEVENT:
			case MENUID_SUB_MENU_SETTINGFORMATE1:
			case MENUID_SUB_MENU_SETTINGTOUCH:		
				 DrawSubItemYesNo(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;	
			case MENUID_SUB_MENU_SETTINGVOLT:
			case MENUID_SUB_MENU_SETTINGVOLT1:
			case MENUID_SUB_MENU_SETTINGVOLT2:
				
			 DrawSubItemsettingvolt(bID0, idx, pMenu->iNumOfItems,
	                     uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
	                     clrText, clrFill, 0x0);
			break;
			case MENUID_SUB_MENU_SETTINGFORMATE:
			DrawSubItemFormat(bID0, idx, pMenu->iNumOfItems,
	                     uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
	                     clrText, clrFill, 0x0);
			break;
			case MENUID_SUB_MENU_SETTINGVOICE:
				if(uiCurrItem==4)
				{
					printc("MENUID_SUB_MENU_SETTINGVOICE\n");
					 DrawSubItemsettingvoice(bID0, 5, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[5]->uiStringId,
			                    pMenu->pItemsList[5]->bmpIcon,
			                    MAKE_BAR_ID(barType, barState),
		                   clrText, clrFill, 0x0);
				}
				else if(uiCurrItem==5)
				{
					printc("MENUID_SUB_MENU_SETTINGVOICE\n");
					 DrawSubItemsettingvoice(bID0, 4, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[4]->uiStringId,
			                    pMenu->pItemsList[4]->bmpIcon,
			                    MAKE_BAR_ID(barType, barState),
		                   clrText, clrFill, 0x0);
				}
				else if(uiCurrItem<4)
				{
					printc("MENUID_SUB_MENU_SETTINGVOICE\n");
					 DrawSubItemsettingvoice(bID0, 0, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[0]->uiStringId,
			                    pMenu->pItemsList[0]->bmpIcon,
			                    MAKE_BAR_ID(barType, barState),
		                   clrText, clrFill, 0x0);
					  DrawSubItemsettingvoice(bID0, 1, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[1]->uiStringId,
			                    pMenu->pItemsList[1]->bmpIcon,
			                    MAKE_BAR_ID(barType, barState),
		                   clrText, clrFill, 0x0);
					   DrawSubItemsettingvoice(bID0, 2, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[2]->uiStringId,
			                    pMenu->pItemsList[2]->bmpIcon,
			                    MAKE_BAR_ID(barType, barState),
		                   clrText, clrFill, 0x0);
					    DrawSubItemsettingvoice(bID0, 3, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[3]->uiStringId,
			                    pMenu->pItemsList[3]->bmpIcon,
			                    MAKE_BAR_ID(barType, barState),
		                   clrText, clrFill, 0x0);
				}
			break;
			case MENUID_SUB_MENU_SETTINGSYSTEM:
						
			 printc("aaMENUID_SUB_MENU_SETTINGSYSTEM\n");	
			DrawSubItemResolutionEX2(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			 
	                break;
			case MENUID_SUB_MENU_SETTINGTIME:
			 printc("MENUID_SUB_MENU_SETTINGTIME\n");
					 DrawSubItem_ClockSetting1(bID0, idx ,0);
	                break;
			case MENUID_SUB_MENU_MOVIE_MODE:
			DrawSubItemResolution(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			break;	
				case MENUID_SUB_MENU_PLAYBACK:
			DrawSubItemPLAYBACK(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			break;	
			default:
				//printc("DrawSubItem defaultpMenu->iMenuId=d\n\n",pMenu->iMenuId);
				DrawSubItem(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
		}

		// printc("MenuDrawChangeSubItemThird ---uiCurrItem\n");

		barState = BAR_STATA_PUSH;
		idx=uiCurrItem;
		switch(pMenu->iMenuId)
		{
			case MENUID_SUB_MENU_LCD:
				 DrawSubItemLcd(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			case MENUID_SUB_MENU_LOGO:
				 DrawSubItemLogo(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			
			case MENUID_SUB_MENU_GSENSOR:
				 DrawSubItemGsensor(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;

			case MENUID_SUB_MENU_MIC:
				DrawSubItemYesNo1(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			case MENUID_SUB_MENU_RESET:	
			case MENUID_SUB_MENU_GPS:
					DrawSubItemYesNo3(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
			case MENUID_SUB_MENU_SETTINGPARKING:
					DrawSubItemYesNo4(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;//lyj 20180703
			case MENUID_SUB_MENU_SETTINGBATTERYH:
					DrawSubItemYesNo5(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;// lyj 20180704
			case MENUID_SUB_MENU_SETTINGEVENT:
			case MENUID_SUB_MENU_SETTINGFORMATE1:
			case MENUID_SUB_MENU_SETTINGTOUCH:		
				 DrawSubItemYesNo(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;	
			case MENUID_SUB_MENU_SETTINGVOLT:
			case MENUID_SUB_MENU_SETTINGVOLT2:
			case MENUID_SUB_MENU_SETTINGVOLT1:	
			 DrawSubItemsettingvolt(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			break;
			case MENUID_SUB_MENU_SETTINGFORMATE:
			DrawSubItemFormat(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			break;
			case MENUID_SUB_MENU_SETTINGVOICE:
			 //printc("MENUID_SUB_MENU_SETTINGVOICE222\n");
			 DrawSubItemsettingvoice(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
	                break;
			case MENUID_SUB_MENU_SETTINGSYSTEM:
			 	printc("bbbMENUID_SUB_MENU_SETTINGSYSTEM\n");
				DrawSubItemResolutionEX2(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiPrevItem]->uiStringId,
		                    pMenu->pItemsList[uiPrevItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			 
	                	break;
			case MENUID_SUB_MENU_SETTINGTIME:
			 	printc("MENUID_SUB_MENU_SETTINGTIME\n");
			 			DrawSubItem_ClockSetting1(bID0, idx ,1);
	                	break;
			case MENUID_SUB_MENU_MOVIE_MODE:
				DrawSubItemResolution(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			break;	
			case MENUID_SUB_MENU_PLAYBACK:
			DrawSubItemPLAYBACK(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
			break;	
			default:
				//printc("MenuDrawChangeSubItemThird 111---uiCurrItem\n");
				DrawSubItem(bID0, idx, pMenu->iNumOfItems,
		                    uLang, pMenu->pItemsList[uiCurrItem]->uiStringId,
		                    pMenu->pItemsList[uiCurrItem]->bmpIcon,
		                    MAKE_BAR_ID(barType, barState),
		                    clrText, clrFill, 0x0);
				break;
		}
       }

     
        OSDDraw_ExitMenuDrawing(&bID0, &bID1);
    }
}
UINT32 SubMenu_Touchadjust(UINT16 pt_x,UINT16 pt_y)//leo20180630
{
	PSMENUSTRUCT pMenu;
	MMP_SHORT Temp[4];	
	int K1;
	int K2;
	pMenu=GetCurrentMenu();
	printc("SubMenu_Touchadjust**********x=%d*************y=%d@@@@@=%d\n",pt_x,pt_y,touchtime );
	KeyParser_TouchItemRegister(&Touchadjust_TouchButton[0], ITEMNUM(Touchadjust_TouchButton));
	if(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGTOUCH)
	{
		UINT8	 bID0 = 0;
		UINT8	  bID1 = 0;
	
	
		AHC_TouchPanel_IOControl(0,Temp);
		printc("***************** Temp[0]=%x ***********************Temp[1]%x\n", *Temp, *(Temp+1), *(Temp+2), *(Temp+3));
		 touch_x[touchtime]= Temp[0]*16+Temp[1]/16;
		 touch_y[touchtime] = Temp[2]*16+Temp[3]/16;
			printc("touch**********x=%d*************y=%d\n",touch_x[touchtime],touch_y[touchtime]);
			  OSDDraw_EnterMenuDrawing(&bID0, &bID1);
			  OSDDraw_ClearOvlDrawBuffer(bID0);
			 //BEGIN_LAYER(bID0);
			AHC_OSDSetBkColor(bID0,0x00050505);
			AHC_OSDSetColor(bID0,0x00050505);
			AHC_OSDDrawFillRect(bID0, 0, 0, 480, 320);
			AHC_OSDSetColor(bID0, OSD_COLOR_RED);
			 AHC_OSDSetPenSize(bID0, 2);
			switch (touchtime)
				{

					case 0: 

							AHC_OSDDrawLine(bID0, 60,	60, 		50, 70);
							AHC_OSDDrawLine(bID0, 50,	70, 		60,60);
					break;
					case 1: 
							AHC_OSDDrawLine(bID0, 420,	420, 		50, 70);
							AHC_OSDDrawLine(bID0, 410,	430, 		60,60);
					break;
					case 2:
							AHC_OSDDrawLine(bID0, 420,	420, 		250, 270);
							AHC_OSDDrawLine(bID0, 410,	430, 		260,260);
					break;
					case 3: 
							AHC_OSDDrawLine(bID0, 60,	60, 		250, 270);
							AHC_OSDDrawLine(bID0, 50,	70, 		260,260);
							
					break;
					case 4: 
							AHC_OSDDrawLine(bID0, 240,	240, 		150, 170);
							AHC_OSDDrawLine(bID0, 230,	250, 		160,160);
					break;
						case 5:
								break;
					default:
						break;

				
				}
			
	   
			
			
			 //END_LAYER(bID0);
			 OSDDraw_ExitMenuDrawing(&bID0, &bID1);

			touchtime++;	
			if(touchtime==6)
			{  UINT16 	x1,x2 = 0;
				UINT16	 y1,y2 = 0;
				UINT16	i= 0;

			for (i=0;i<6;i++)

			{

			printc("touch%d**********x=%d*************y=%d\n",i,touch_x[i],touch_y[i]);

			}
			
				
				printc("=%d**********=%d*************=%d*************=%d\n",abs(touch_x[3]-touch_x[2]),abs(touch_x[4]-touch_x[1]),abs(touch_y[2]-touch_y[1]),abs(touch_y[3]-touch_y[4]));
				if(abs(touch_x[3]-touch_x[2])>150
				|abs(touch_x[4]-touch_x[1])>150
				|abs(touch_y[2]-touch_y[1])>150
				|abs(touch_y[3]-touch_y[4])>150)

				{
					

				touchtime=0;
				SubMenu_Touchadjust(0,0);


				}



		else
		{	
		K1=((100*(touch_x[2]+touch_x[3]-touch_x[1]-touch_x[4])/720));
		K2=((100*(touch_y[3]+touch_y[4]-touch_y[1]-touch_y[2])/400));
		printc("K1=%d**********k2=%d*************\n",K1,K2);
		printc("**********x=%d*************y=%d\n",60+100*(touch_x[5]-touch_x[1])/K1,60+100*(touch_y[5]-touch_y[1])/K2);
		//MenuSettingConfig()->uiWDR=250;

		MenuSettingConfig()->touch_k1=K1;
			
			
		printc("K1=%d**********k2=%d*************\n",MenuSettingConfig()->touch_k1,MenuSettingConfig()->touch_k2);	
		MenuSettingConfig()->touch_x=touch_x[1];
		MenuSettingConfig()->touch_y=touch_y[1];
		touch_depend[0]=MenuSettingConfig()->touch_x;
		touch_depend[1]=MenuSettingConfig()->touch_y;
		touch_depend[2]=K1;
		touch_depend[3]=K2;
		MenuSettingConfig()->touch_k2=K2;
		printc("K2=%d**********MenuSettingConfig()->touch_k3=%d*************\n",MenuSettingConfig()->touch_k2,MenuSettingConfig()->touch_k3);	
		AHC_TouchPanel_IOControl(1,touch_depend);
		SubMenuEventHandler(pMenu,MENU_MENU,0);
			touchtime=0;
			return MENU_ERR_OK;
			}
			}
			

	
		MenuDrawChangeSubItemThird( pMenu, pMenu->iCurrentPos, pMenu->iCurrentPos, pMenu->iSelected );
	
	//AHC_OS_SleepMs(20);
	
	//	SubMenuEventHandler(pMenu,MENU_SET_ONE,0);
	

			return MENU_ERR_OK; 

	}



}
extern void MenuDrawChangeSubItemfoure(PSMENUSTRUCT pMenu, UINT32 uiCurrItem, UINT32 uiPrevItem, UINT32 uiPreSelected);
UINT32 return_goFront(UINT16 pt_x,UINT16 pt_y)
{
	PSMENUSTRUCT pMenu = GetCurrentMenu();
	UINT16  i;
	int iPrevPos;
	UINT32 ret = MENU_ERR_OK;
	if( pMenu->iNumOfItems <= 2 )
    {
    		if(pMenu->iMenuId==MENUID_SUB_MENU_MIC||pMenu->iMenuId==MENUID_SUB_MENU_MOVIE_MODE)
			i =  ( pt_x- OSD_SUBMENU22_X )/( OSD_SUBMENU22_W + 20 );		
		else
			i =  ( pt_x- OSD_SUBMENU20_X )/( OSD_SUBMENU20_W + 20 );
    }	
	if(i < pMenu->iNumOfItems)
	{
		#ifdef ONE_TOUCH_MENU
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItemfoure( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			AHC_OS_SleepMs(20);
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			if((pMenu->iMenuId==MENUID_SUB_MENU_SETTINGTOUCH)||(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGFORMATE)||(pMenu->iMenuId==MENUID_SUB_MENU_SETTINGFORMATE1));
					else{
						printc("*************************************************\n");
						printc("**************pMenu->iMenuId=%d***********************************\n",pMenu->iMenuId);
						printc("*************************************************\n");
					#ifdef AUTO_RETURN
						ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
					#endif
						}
			}
		#else
			if(i ==  pMenu->iCurrentPos)
				ret = SubMenuEventHandler(pMenu,MENU_SET_ONE,i);
			else
			{
				iPrevPos = pMenu->iCurrentPos;
				pMenu->iCurrentPos = i;
				MenuDrawChangeSubItemfoure( pMenu, pMenu->iCurrentPos, iPrevPos, pMenu->iSelected );
			#ifdef AUTO_RETURN
				ret = SubMenuEventHandler(pMenu,MENU_MENU,i);
			#endif
			}
		#endif
	}
	printc(FG_RED("SubMenuItem_Touch ret %d\r\n"),ret);
	if(ret == MENU_ERR_EXIT)
	{
		#ifdef SLIDE_MENU
		if(IsSlidingMenu())
			StopSlideMenu();
		#endif    
        ResetCurrentMenu();
		Menu_WriteSetting();
        if(Deleting_InBrowser || Protecting_InBrowser)
        {
        	bForceSwitchBrowser = AHC_TRUE;
        	StateSwitchMode(UI_BROWSER_STATE);
        }
    }
	printc("~~~~return_goFront~~~~~~\r\n");
	return 0;
}
#endif
