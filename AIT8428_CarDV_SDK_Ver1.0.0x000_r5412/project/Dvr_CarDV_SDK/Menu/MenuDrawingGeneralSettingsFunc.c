/*===========================================================================
 * Include file
 *===========================================================================*/

#include "AHC_Common.h"
#include "AHC_MACRO.h"
#include "AHC_Utility.h"
#include "AHC_Gui.h"
#include "AHC_Version.h"
#include "AHC_Display.h"
#include "AHC_General_CarDV.h"
#include "MenuCommon.h"
#include "IconDefine.h"
#include "IconPosition.h"
#include "ColorDefine.h"
#include "OsdStrings.h"
#include "MenuDrawingFunc.h"
#include "MenuDrawCommon.h"
#include "StateTVFunc.h"
#include "StateHDMIFunc.h"
#include "ShowOSDFunc.h"
#include "DrawStateMenuSetting.h"
#include "MenuDrawingFunc.h"
#include "stdio.h"
#if (GPS_CONNECT_ENABLE)
#include "gps_nmea0183.h"
#include "GPS_Ctl.h"
#endif

/*===========================================================================
 * Global varible
 *===========================================================================*/

int Datetime[6] = {2011,01,01,12,0,0};//year,month,day,hour,minute

/*===========================================================================
 * Extern varible
 *===========================================================================*/

extern GUI_POINT*	m_PointDir[];
extern AHC_OSD_INSTANCE *m_OSD[];

/*===========================================================================
 * Main body
 *===========================================================================*/
#if (MENU_GENERAL_DRIVER_ID_SETTING_EN) && !defined(_OEM_MENU_)
#define _HCARDC_MENU_DRAWING_DRIVE_ID_FUNC_C_
#include "HCarDV_MenuDrawingDriveIdFunc.c"
#endif

#if 0
void ________Clock_Function_________(){ruturn;} //dummy
#endif

void GetSubItemRect_ClockSetting( int i,  RECT* pRc )
{
	#if defined(FONT_LARGE)
	switch(i)
	{
		case 0: {static RECT rc = {25,  110, 24,  30};  	*pRc = rc;break;}
		case 1: {static RECT rc = {75,  110, 24,  30};  	*pRc = rc;break;}
		case 2: {static RECT rc = {125, 110, 24,  30};  	*pRc = rc;break;}
		case 3: {static RECT rc = {175, 110, 24,  30};  	*pRc = rc;break;}
	 	case 4: {static RECT rc = {225, 110, 102, 30};  	*pRc = rc;break;}
	 	case 5: {static RECT rc = {275, 110, 16,  14};  	*pRc = rc;break;}
	 	case 6: {static RECT rc = {2,   206, 102, 30};  	*pRc = rc;break;}
	 	default:{static RECT rc = {20,  110, 24,  30};  	*pRc = rc;break;}
	}
	#else	// FONT_LARGE		
	if(AHC_IsHdmiConnect())
	{
		if(MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_1080P)
    	{
			switch(i)
			{
				case 0: {static RECT rc = {95 , 110, 16,  14};		*pRc = rc;break;}
				case 1: {static RECT rc = {135, 110, 16,  14};		*pRc = rc;break;}
				case 2: {static RECT rc = {175, 110, 16,  14};		*pRc = rc;break;}
				case 3: {static RECT rc = {235, 110, 16,  14};		*pRc = rc;break;}
				case 4: {static RECT rc = {275, 110, 16,  14};		*pRc = rc;break;}
				case 5: {static RECT rc = {315, 110, 16,  14};		*pRc = rc;break;}
				case 6: {static RECT rc = {155, 236, 102, 30};		*pRc = rc;break;}
				default:{static RECT rc = {45,	110, 16,  14};		*pRc = rc;break;}
			}
		}
		else
		{
			switch(i)
			{				
				case 0: {static RECT rc = {45 , 110, 16,  14};		*pRc = rc;break;}
				case 1: {static RECT rc = {85 , 110, 16,  14};		*pRc = rc;break;}
				case 2: {static RECT rc = {125, 110, 16,  14};		*pRc = rc;break;}
				case 3: {static RECT rc = {185, 110, 16,  14};		*pRc = rc;break;}
				case 4: {static RECT rc = {225, 110, 16,  14};		*pRc = rc;break;}
				case 5: {static RECT rc = {265, 110, 16,  14};		*pRc = rc;break;}
				case 6: {static RECT rc = {105, 206, 102, 30};		*pRc = rc;break;}
				default:{static RECT rc = {45,	110, 16,  14};		*pRc = rc;break;}
			}
		}	
	}	
	else if(AHC_IsTVConnectEx())
	{
		switch(i)
		{			
			case 0: {static RECT rc = {45 , 110, 16,  14};		*pRc = rc;break;}
			case 1: {static RECT rc = {85 , 110, 16,  14};		*pRc = rc;break;}
			case 2: {static RECT rc = {125, 110, 16,  14};		*pRc = rc;break;}
			case 3: {static RECT rc = {185, 110, 16,  14};		*pRc = rc;break;}
			case 4: {static RECT rc = {225, 110, 16,  14};		*pRc = rc;break;}
			case 5: {static RECT rc = {265, 110, 16,  14};		*pRc = rc;break;}
			case 6: {static RECT rc = {105, 206, 102, 30};		*pRc = rc;break;}
			default:{static RECT rc = {45,	110, 16,  14};		*pRc = rc;break;}
		}
	}
	else		
	{	
		switch(i)
		{				
			case 0: {static RECT rc = {45 , 110, 16,  14};  	*pRc = rc;break;}
			case 1: {static RECT rc = {85 , 110, 16,  14};  	*pRc = rc;break;}
			case 2: {static RECT rc = {125, 110, 16,  14};  	*pRc = rc;break;}
			case 3: {static RECT rc = {185, 110, 16,  14};  	*pRc = rc;break;}
		 	case 4: {static RECT rc = {225, 110, 16,  14};  	*pRc = rc;break;}
		 	case 5: {static RECT rc = {265, 110, 16,  14};  	*pRc = rc;break;}
		 	case 6: {static RECT rc = {105, 206, 102, 30};  	*pRc = rc;break;}
		 	default:{static RECT rc = {45,  110, 16,  14};  	*pRc = rc;break;}
		}
	}	
	#endif	// FONT_LARGE
}

int GetSubItemClockSetting_DateTime(int iItems)
{
    int DateTime;

    switch(iItems)
    {
        case 0: {DateTime = Datetime[0]; break;}
        case 1: {DateTime = Datetime[1]; break;}
        case 2: {DateTime = Datetime[2]; break;}
        case 3: {DateTime = Datetime[3]; break;}
        case 4: {DateTime = Datetime[4]; break;}
        default:{DateTime = Datetime[5]; break;}
    }
	return DateTime;
}

void DrawSubItem_ClockSetting(UINT16 uwDispID, int iItem, UINT32 iStrID, GUI_COLOR clrFont, GUI_BITMAP barID, GUI_COLOR clrBack, GUI_COLOR clrSelect)
{
    RECT 	rc, tmpRECT;
    int 	DateTime;

	AHC_OSDSetFont(uwDispID, &GUI_Font24B_1);

	GetSubItemRect_ClockSetting(iItem, &rc);

	DateTime = GetSubItemClockSetting_DateTime(iItem);

	switch(iItem)
	{
		case 0://Year
		case 1://Month
		case 2://Day
			tmpRECT.uiLeft   = rc.uiLeft + 22;
			tmpRECT.uiTop    = rc.uiTop ;
			tmpRECT.uiWidth  = rc.uiWidth;
			tmpRECT.uiHeight = rc.uiHeight;

	        OSD_ShowStringPool(uwDispID, iStrID, tmpRECT, clrFont, OSD_COLOR_BLACK, GUI_TA_HCENTER|GUI_TA_VCENTER);
	    break;

	    case 3://Hour
	    case 4://Minute
			AHC_OSDSetColor(uwDispID, clrFont);
			AHC_OSDSetBkColor(uwDispID, OSD_COLOR_BLACK);

			AHC_OSDDrawText(uwDispID, (UINT8*)":", rc.uiLeft + 30, rc.uiTop, strlen(":"));
		break;

		case 6://OK
			OSD_Draw_Icon(barID, rc, uwDispID);

			tmpRECT.uiLeft   = rc.uiLeft 	+ STR_RECT_OFFSET_X;
			tmpRECT.uiTop    = rc.uiTop  	+ STR_RECT_OFFSET_Y;
			tmpRECT.uiWidth  = rc.uiWidth 	+ STR_RECT_OFFSET_W;
			tmpRECT.uiHeight = rc.uiHeight  + STR_RECT_OFFSET_H;
			OSD_ShowStringPool(uwDispID, iStrID, tmpRECT, clrFont, clrBack, GUI_TA_HCENTER|GUI_TA_VCENTER);
		break;
	}

	AHC_OSDSetFont(uwDispID, &GUI_Font24B_1);

    //Draw DateTime
	if( iItem < 6 )
	{
		AHC_OSDSetColor(uwDispID, clrFont);
		AHC_OSDSetBkColor(uwDispID, clrBack);

		if( iItem ==0 )//Draw year
			AHC_OSDDrawDec(uwDispID, DateTime, rc.uiLeft - 25, 	rc.uiTop, 4);
		else
			AHC_OSDDrawDec(uwDispID, DateTime, rc.uiLeft, 		rc.uiTop, 2);
	}
}

void DrawSubItem_ClockSettingUP(UINT16 uwDispID, int iItem, int iOffset, GUI_COLOR clrFont, GUI_COLOR clrBack)
{
    RECT 		rc;
    UINT32		iXoffset = 0,   iYoffset = 0;
    GUI_COLOR 	bkColor = OSD_COLOR_DARKGRAY;

	GetSubItemRect_ClockSetting(iItem, &rc);

	AHC_OSDSetFont(uwDispID, &GUI_Font24B_1);

	AHC_OSDSetColor(uwDispID, clrFont);
	AHC_OSDSetBkColor(uwDispID, clrBack);

	switch(iItem)
	{
		case 0:
			Datetime[0] = Datetime[0] + iOffset;

			AHC_Validate_ClockSetting(Datetime, CHECK_YEAR, AHC_TRUE);

            AHC_OSDDrawDec(uwDispID, Datetime[0] , rc.uiLeft+iXoffset- 25, rc.uiTop+iYoffset, 4);

            GetSubItemRect_ClockSetting(2, &rc);

            AHC_OSDSetBkColor(uwDispID, bkColor);
            AHC_OSDDrawDec(uwDispID, Datetime[2] , rc.uiLeft+iXoffset, rc.uiTop+iYoffset, 2);
		break;

		case 1:
			Datetime[1] = Datetime[1] + iOffset;

			AHC_Validate_ClockSetting(Datetime, CHECK_MONTH, AHC_TRUE);

            AHC_OSDDrawDec(uwDispID, Datetime[1] , rc.uiLeft+iXoffset, rc.uiTop+iYoffset, 2);
            GetSubItemRect_ClockSetting(2, &rc);

            AHC_OSDSetBkColor(uwDispID, bkColor);
            AHC_OSDDrawDec(uwDispID, Datetime[2] , rc.uiLeft+iXoffset, rc.uiTop+iYoffset, 2);
		break;

		case 2:
			Datetime[2] = Datetime[2] + iOffset;

			AHC_Validate_ClockSetting(Datetime, CHECK_DAY, AHC_TRUE);

			AHC_OSDDrawDec(uwDispID, Datetime[2], rc.uiLeft+iXoffset, rc.uiTop+iYoffset, 2);
		break;

		case 3:
			Datetime[3] = Datetime[3] + iOffset;

			AHC_Validate_ClockSetting(Datetime, CHECK_HOUR, AHC_TRUE);

			AHC_OSDDrawDec(uwDispID, Datetime[3], rc.uiLeft+iXoffset, rc.uiTop+iYoffset, 2);
		break;

		case 4:
			Datetime[4] = Datetime[4] + iOffset;

			AHC_Validate_ClockSetting(Datetime, CHECK_MIN, AHC_TRUE);

			AHC_OSDDrawDec(uwDispID, Datetime[4], rc.uiLeft+iXoffset, rc.uiTop+iYoffset, 2);
		break;

		case 5:
			Datetime[5] = Datetime[5] + iOffset;

			AHC_Validate_ClockSetting(Datetime, CHECK_SEC, AHC_TRUE);

			AHC_OSDDrawDec(uwDispID, Datetime[5], rc.uiLeft+iXoffset, rc.uiTop+iYoffset, 2);
		break;
	}
}

void MenuDrawSubPage_ClockSetting(PSMENUSTRUCT pMenu)
{
    UINT8  bID0 = 0, bID1 = 0;
    UINT32 i;
    UINT32 iBegin, iEnd;

#ifdef SLIDE_STRING
	StopSlideString();
#endif

    OSDDraw_EnterMenuDrawing(&bID0, &bID1);

    iBegin = ALIGN_DOWN( pMenu->iCurrentPos, 7 );
    iEnd   = MIN( iBegin+ 7, pMenu->iNumOfItems);

	OSDDraw_ClearOvlDrawBuffer(bID0);
	MenuDrawSubBackCurtain(bID0, OSD_COLOR_DARKGRAY2);

	if( pMenu->uiStringId != -1 )
	{
		MenuDrawSubTitle(bID0, pMenu->uiStringId);
	}

    for( i=iBegin; i< iEnd; i++ )
    {
        GUI_COLOR clrBk = OSD_COLOR_DARKGRAY;
        GUI_BITMAP barID = BMICON_SMALLBAR_WHITE;

        if( i > 5 )
        {
		    clrBk = OSD_COLOR_TRANSPARENT;
        }

        if(( i == pMenu->iCurrentPos ) && ( pMenu->iCurrentPos <= 5 ))
        {
		    clrBk = OSD_COLOR_DARKYELLOW;
        }
        DrawSubItem_ClockSetting(bID0, i, pMenu->pItemsList[i]->uiStringId, MENU_TEXT_COLOR, barID, clrBk, 0x0);
    }

    if(pMenu->uiStringDescription != NULL)
    	MenuDrawSubMenuDescription(bID0, pMenu->uiStringDescription);

	#if (SUPPORT_TOUCH_PANEL)
	{
		RECT RECTUp 	= RECT_TOUCH_BUTTON_CLOCK_UP;
		RECT RECTDown 	= RECT_TOUCH_BUTTON_CLOCK_DOWN;

		OSD_Draw_Icon(bmIcon_D_Up, 		RECTUp,		bID0);
		OSD_Draw_Icon(bmIcon_D_Down, 	RECTDown,	bID0);
	}
	#endif

	MenuDraw_BatteryStatus(bID0);

	OSDDraw_ExitMenuDrawing(&bID0, &bID1);

	AHC_OSDSetActive(bID0, AHC_TRUE);
}

void MenuDrawChangeSubItem_ClockSetting(PSMENUSTRUCT pMenu, UINT32 uiCurrItem, UINT32 uiPrevItem, UINT32 uiPreSelected )
{
    UINT8     	bID0 = 0, bID1 = 0;
    GUI_COLOR 	colorFill 		= OSD_COLOR_DARKGRAY;
    GUI_COLOR 	colorSelected   = 0x0;
    GUI_BITMAP 	barID 			= BMICON_SUBBAR_WHITE;

    OSDDraw_EnterMenuDrawing(&bID0, &bID1);

    if( pMenu->iSelected == uiPrevItem )
	    colorSelected = OSD_COLOR_SELECTED;
    else
	    colorSelected = 0;

    if( uiPrevItem > 5 )
    {
	    barID 		= BMICON_SUBBAR_WHITE;
		colorFill 	= OSD_COLOR_TRANSPARENT;
    }

    //Draw Previous
    DrawSubItem_ClockSetting(bID0, uiPrevItem,  pMenu->pItemsList[uiPrevItem]->uiStringId, MENU_TEXT_COLOR, barID, colorFill, colorSelected);

    if( pMenu->iSelected == uiCurrItem )
	    colorSelected = OSD_COLOR_SELECTED;
    else
	    colorSelected = 0;

    if ( uiCurrItem > 5 )
    {
	    barID 	  = BMICON_SMALLBAR_YELLOW;
        colorFill = OSD_COLOR_TRANSPARENT;
    }
    else if(uiCurrItem <= 5 )
    {
	    colorFill = OSD_COLOR_DARKYELLOW;
    }

    //Draw current
    DrawSubItem_ClockSetting(bID0, uiCurrItem,  pMenu->pItemsList[uiCurrItem]->uiStringId, MENU_TEXT_COLOR, barID, colorFill, colorSelected);

    OSDDraw_ExitMenuDrawing(&bID0, &bID1);
}

void MenuDrawChangeSubItem_ClockSettingUD(PSMENUSTRUCT pMenu, int iOffset)
{
    UINT8  bID0 = 0, bID1 = 0;

    OSDDraw_EnterMenuDrawing(&bID0, &bID1);

    DrawSubItem_ClockSettingUP(bID0, pMenu->iCurrentPos, iOffset, MENU_TEXT_COLOR, OSD_COLOR_DARKYELLOW);

    OSDDraw_ExitMenuDrawing(&bID0, &bID1);
}

#if 0
void ________FW_Version_Function_________(){ruturn;} //dummy
#endif

void DrawSubItem_FwVersionInfo(int iMenuId, UINT16 uwDispID, GUI_COLOR clrFont, GUI_COLOR clrBack)
{
	#define	VER_LS		45 // Line Space

	#if defined(FONT_LARGE)
    RECT	verRECT = RECT_MENU_LARGE_VERSION;
    RECT 	tmpRECT = RECT_MENU_LARGE_VERSION;
	#else
    RECT	verRECT = RECT_MENU_VERSION;
    RECT 	tmpRECT = RECT_MENU_VERSION;
    #endif
    UINT16 	ulMajorVer, ulMediumVer, ulMinorVer;
    UINT16 	ulBranchVer, ulTestVer;
    char*  	szReleaseName = 0;
    char	szver[20];
	char	szBranch[32];

    AHC_GetFwVersion(&ulMajorVer, &ulMediumVer, &ulMinorVer, &ulBranchVer, &ulTestVer, &szReleaseName);

    AHC_OSDSetColor(uwDispID, MENU_TEXT_COLOR);
    AHC_OSDSetBkColor(uwDispID, OSD_COLOR_DARKGRAY2);

    AHC_OSDSetFont(uwDispID, &GUI_Font16B_ASCII);
    AHC_OSDSetPenSize(uwDispID, 1);

	// Version
	OSD_ShowStringPool(uwDispID, IDS_DS_FW_VERSION, tmpRECT, clrFont, clrBack, GUI_TA_LEFT|GUI_TA_VCENTER);
	sprintf(szver, ": %04d.%04d.%04d", ulMajorVer, ulMediumVer, ulMinorVer);
	tmpRECT.uiLeft += tmpRECT.uiWidth;
	tmpRECT.uiWidth = AHC_GET_ATTR_OSD_W(uwDispID) - tmpRECT.uiLeft;
	OSD_ShowString(uwDispID, szver, NULL, tmpRECT, clrFont, clrBack, GUI_TA_LEFT|GUI_TA_VCENTER);

	// Branch
	tmpRECT.uiLeft   = verRECT.uiLeft;
    tmpRECT.uiTop   += VER_LS;
	tmpRECT.uiWidth  = verRECT.uiWidth;
	OSD_ShowStringPool(uwDispID, IDS_DS_FW_BRANCH, tmpRECT, clrFont, clrBack, GUI_TA_LEFT|GUI_TA_VCENTER);

  	snprintf(szBranch, sizeof(szBranch) - 1, ": %04d %s", ulBranchVer, szReleaseName);

	tmpRECT.uiLeft += tmpRECT.uiWidth;
	tmpRECT.uiWidth = AHC_GET_ATTR_OSD_W(uwDispID) - tmpRECT.uiLeft;

	OSD_ShowString(uwDispID, szBranch, NULL, tmpRECT, clrFont, clrBack, GUI_TA_LEFT|GUI_TA_VCENTER);

	// Test
	tmpRECT.uiLeft   = verRECT.uiLeft;
    tmpRECT.uiTop   += VER_LS;
	tmpRECT.uiWidth  = verRECT.uiWidth;
	OSD_ShowStringPool(uwDispID, IDS_DS_FW_TEST, tmpRECT, clrFont, clrBack, GUI_TA_LEFT|GUI_TA_VCENTER);

	sprintf(szver, ": %04d", ulTestVer);

	tmpRECT.uiLeft += tmpRECT.uiWidth;
	tmpRECT.uiWidth = AHC_GET_ATTR_OSD_W(uwDispID) - tmpRECT.uiLeft;
	OSD_ShowString(uwDispID, szver, NULL, tmpRECT, clrFont, clrBack, GUI_TA_LEFT|GUI_TA_VCENTER);

    AHC_OSDSetColor(uwDispID, MENU_TEXT_COLOR);
    AHC_OSDSetBkColor(uwDispID, clrBack);
    AHC_OSDSetFont(uwDispID, &GUI_Font16B_1);
}

void MenuDrawSubPage_FwVersionInfo(PSMENUSTRUCT pMenu)
{
    UINT8  bID0 = 0, bID1 = 0;

	#ifdef SLIDE_STRING
	StopSlideString();
	#endif

    OSDDraw_EnterMenuDrawing(&bID0, &bID1);

    OSDDraw_ClearOvlDrawBuffer(bID0);
	MenuDrawSubBackCurtain(bID0, OSD_COLOR_DARKGRAY2);

	if( pMenu->uiStringId != -1 )
	{
		MenuDrawSubTitle(bID0, pMenu->uiStringId);
	}

	if(AHC_IsTVConnectEx() || AHC_IsHdmiConnect())
    	DrawSubItem_FwVersionInfo(pMenu->iMenuId, bID0, MENU_TEXT_COLOR, OSD_COLOR_DARKGRAY2);
    else
    	DrawSubItem_FwVersionInfo(pMenu->iMenuId, bID0, MENU_TEXT_COLOR, OSD_COLOR_DARKGRAY2);

    MenuDraw_BatteryStatus(bID0);

	OSDDraw_ExitMenuDrawing(&bID0, &bID1);
}

#if 0
void ________GPS_Info_Function_________(){ruturn;} //dummy
#endif

#if (GPS_CONNECT_ENABLE)
void DrawSubItem_GPSInformationPage(int iMenuId, UINT16 uwDispID, GUI_COLOR clrFont, GUI_COLOR clrBack)
{
	QuickDrawItem_GPSInfo(uwDispID, OSD_COLOR_DARKGRAY2);
}

void MenuDrawSubPage_GPSInfo(PSMENUSTRUCT pMenu)
{
    UINT8  bID0 = 0;
    UINT8  bID1 = 0;

    if(pMenu->iMenuId == MENUID_SUB_MENU_GPS_INFO)
    {
		#ifdef SLIDE_STRING
		StopSlideString();
		#endif

    	OSDDraw_EnterMenuDrawing(&bID0, &bID1);

    	OSDDraw_ClearOvlDrawBuffer(bID0);

		MenuDrawSubBackCurtain(bID0, OSD_COLOR_DARKGRAY2);

		AHC_OSDSetColor(bID0, OSD_COLOR_BLACK);
		AHC_OSDDrawFillRect(bID0, 0, 200, 320, 204);

		if( pMenu->uiStringId != -1 )
		{
			MenuDrawSubTitle(bID0, pMenu->uiStringId);
		}

	    DrawSubItem_GPSInformationPage(pMenu->iMenuId, bID0, MENU_TEXT_COLOR, OSD_COLOR_DARKGRAY2);

    	MenuDraw_BatteryStatus(bID0);

		OSDDraw_ExitMenuDrawing(&bID0, &bID1);
	}
}


void OSD_Draw_Iconnumber(int num, RECT Draw, UINT16 ulID)//leo20180630
{
	const GUI_BITMAP*number[]={
									&bmtime_0,
									&bmtime_1,
									&bmtime_2,
									&bmtime_3,

									&bmtime_4,
									&bmtime_5,
									&bmtime_6,
									&bmtime_7,							
									&bmtime_8,
									&bmtime_9,
									&bmtime_0r,
									&bmtime_1r,
									&bmtime_2r,
									&bmtime_3r,

									&bmtime_4r,
									&bmtime_5r,
									&bmtime_6r,
									&bmtime_7r,								
									&bmtime_8r,
									&bmtime_9r,



									};
	if(num>19||num<0)
	{
		printc("number is too big !!!\r\n");//�����
		return ;

	}

	OSD_Draw_Icon( *number[num], Draw, ulID);



}
void OSD_Draw_Iconparkingnum(int num, RECT Draw, UINT16 ulID)//leo20180630
{
	const GUI_BITMAP*number[]={
									&bmnum_0,
									&bmnum_1,
									&bmnum_2,
									&bmnum_3,

									&bmnum_4,
									&bmnum_5,
									&bmnum_6,
									&bmnum_7,							
									&bmnum_8,
									&bmnum_9,
								
							};
	if(num>9||num<0)
	{
		printc("number is too big !!!\r\n");//�����
		return ;

	}

	OSD_Draw_Icon( *number[num], Draw, ulID);



}
//leo20180630
void DrawSubItem_ClockSetting1(UINT16 uwDispID, int iItem,     GUI_COLOR clrSelect)
{
    RECT 	rc, tmpRECT;
    int 	DateTime;
	int    i,iOffset;
	int    DateTemp;
	if(clrSelect==8)
	{
		iOffset=1;
	}
	else if(clrSelect==11){
		
		iOffset=-1;

	}
	else iOffset=0;
 	switch(iItem)
 	{
		 case 0:
			 Datetime[0] = Datetime[0] + iOffset;
 
			 AHC_Validate_ClockSetting(Datetime, CHECK_YEAR, AHC_TRUE);
 
			
		 break;
 
		 case 1:
		 	
			 Datetime[1] = Datetime[1] + iOffset;
  			AHC_Validate_ClockSetting(Datetime, CHECK_MONTH, AHC_TRUE);
			
		 break;
 
		 case 2:
			 Datetime[2] = Datetime[2] + iOffset;
 
			 AHC_Validate_ClockSetting(Datetime, CHECK_DAY, AHC_TRUE);
 
			
		 break;
 
		 case 3:
			 Datetime[3] = Datetime[3] + iOffset;
 
			 AHC_Validate_ClockSetting(Datetime, CHECK_HOUR, AHC_TRUE);
 
			
		 break;
 
		 case 4:
			 Datetime[4] = Datetime[4] + iOffset;
 
			 AHC_Validate_ClockSetting(Datetime, CHECK_MIN, AHC_TRUE);
 
			 
		 break;
 
		 case 5:
			 Datetime[5] = Datetime[5] + iOffset;
 
			 AHC_Validate_ClockSetting(Datetime, CHECK_SEC, AHC_TRUE);
 
			
		 break;
		 }
	//printc("DrawSubItem_ClockSetting1 DateTime%iOffset=%d\n ", Datetime[iItem],iOffset);

	GetSubItemRect_ClockSetting(iItem, &rc);
	

	DateTime = GetSubItemClockSetting_DateTime(iItem);
	



	
	
	//printc("DrawSubItem_ClockSetting1DateTime%duwDispID=%d\n ", DateTime,uwDispID);
	tmpRECT.uiWidth  = 20;
	tmpRECT.uiHeight = 30;
	switch(iItem)
	{
		case 0://Year
			tmpRECT.uiTop    = 135 ;
			tmpRECT.uiLeft   = 128;
			for(i=0;i<4;i++)
			{
			tmpRECT.uiLeft   = tmpRECT.uiLeft -25;
			
			DateTemp=DateTime%10;	
			//printc("DrawSubItem_ClockSetting1DateTemp%d\n ", DateTemp);
			if(clrSelect&&(i==0))DateTemp=DateTemp+10;
			OSD_Draw_Iconnumber(DateTemp,tmpRECT,uwDispID);


			DateTime=DateTime/10;
			}
		 break;	
		case 1://Month
			tmpRECT.uiTop	 = 135 ;
			tmpRECT.uiLeft	 = 234;
			for(i=0;i<2;i++)
			{
			tmpRECT.uiLeft	 =tmpRECT.uiLeft-30;
						
			DateTemp=DateTime%10;	
			//printc("DrawSubItem_ClockSetting1DateTemp%d\n ", DateTemp);
			if(clrSelect&&(i==0))DateTemp=DateTemp+10;
			OSD_Draw_Iconnumber(DateTemp,tmpRECT,uwDispID);
			
			
			DateTime=DateTime/10;
			}
			 break;			

		
		case 2://Day
			tmpRECT.uiTop	 = 135 ;
			tmpRECT.uiLeft	 = 331;
			for(i=0;i<2;i++)
			{
			tmpRECT.uiLeft	 = tmpRECT.uiLeft -30;
						
			DateTemp=DateTime%10;	
			//printc("DrawSubItem_ClockSetting1DateTemp%d\n ", DateTemp);
			if(clrSelect&&(i==0))DateTemp=DateTemp+10;
			OSD_Draw_Iconnumber(DateTemp,tmpRECT,uwDispID);
			
			
			DateTime=DateTime/10;
			}
					
	    break;

	    case 3://Hour

			tmpRECT.uiTop	 = 225 ;
			tmpRECT.uiLeft	 = 132;
			for(i=0;i<2;i++)
			{
			tmpRECT.uiLeft	 = tmpRECT.uiLeft -30;
						
			DateTemp=DateTime%10;	
			//printc("DrawSubItem_ClockSetting1DateTemp%d\n ", DateTemp);
			if(clrSelect&&(i==0))DateTemp=DateTemp+10;
			OSD_Draw_Iconnumber(DateTemp,tmpRECT,uwDispID);
			
			
			DateTime=DateTime/10;
			}
			 break;		
	    case 4://Minute
			tmpRECT.uiTop	 = 225  ;
			tmpRECT.uiLeft	 = 233;
			for(i=0;i<2;i++)
			{
				tmpRECT.uiLeft	 = tmpRECT.uiLeft-30;
							
				DateTemp=DateTime%10;	
				//printc("DrawSubItem_ClockSetting1DateTemp%d\n ", DateTemp);
				if(clrSelect&&(i==0))DateTemp=DateTemp+10;
				OSD_Draw_Iconnumber(DateTemp,tmpRECT,uwDispID);
				
				
				DateTime=DateTime/10;
			} 
		break;
		case 5://Minute
			tmpRECT.uiTop	 = 225  ;
			tmpRECT.uiLeft	 = 331;
			for(i=0;i<2;i++)
			{
				tmpRECT.uiLeft	 = tmpRECT.uiLeft -30;
							
				DateTemp=DateTime%10;	
				//printc("DrawSubItem_ClockSetting1DateTemp%d\n ", DateTemp);
				if(clrSelect&&(i==0))DateTemp=DateTemp+10;
				OSD_Draw_Iconnumber(DateTemp,tmpRECT,uwDispID);
				
				
				DateTime=DateTime/10;
			} 
		break;
		default:
			break;
			

			
	}

	AHC_SetClock(GetSubItemClockSetting_DateTime(0),  GetSubItemClockSetting_DateTime(1),  GetSubItemClockSetting_DateTime(2), 0,
					     GetSubItemClockSetting_DateTime(3),  GetSubItemClockSetting_DateTime(4),  GetSubItemClockSetting_DateTime(5), 0, 0);
}
#endif