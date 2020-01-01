/*===========================================================================
 * Include file 
 *===========================================================================*/  
#include "AHC_Common.h"
#include "AHC_General.h"
#include "AHC_Message.h"
#include "AHC_Os.h"
#include "AHC_Display.h"
#include "AHC_Audio.h"
#include "AHC_Capture.h"
#include "AHC_Dcf.h"
#include "AHC_Media.h"
#include "AHC_Video.h"
#include "AHC_GUI.h"
#include "AHC_Motor.h"
#include "AHC_Menu.h"
#include "AHC_USB.h"
#include "AHC_Parameter.h"
#include "AHC_General_CarDV.h"
#include "MenuCommon.h"
#include "MenuTouchButton.h"
#include "MenuStateMenu.h"
#include "MenuDrawingFunc.h"
#include "MenuStateGeneralSettingsMenu.h"
//#include "MediaPlaybackCtrl.h"e
#include "KeyParser.h"
#include "LedControl.h"
#include "IconPosition.h"
#include "dsc_charger.h"

#include "StateCameraFunc.h"
#include "StateVideoFunc.h"
#include "StateBrowserFunc.h"
#include "StateMoviePBFunc.h"
#include "StatePhotoPBFunc.h"
#include "StateSlideShowFunc.h"
#include "StateMSDCFunc.h"
#include "StateHDMIFunc.h"
#include "StateTVFunc.h"

extern UINT32 return_goFront(UINT16 pt_x,UINT16 pt_y);


#if (SUPPORT_TOUCH_PANEL)
//leo20180630

SMENUTOUCHBUTTON VideoMainPage_TouchButton[] =
{
    {RECT_TOUCH_BUTTON_MENU ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_ID_MENU) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_ID_MENU) ,NULL    		 	 },
    //{RECT_TOUCH_BUTTON_FRONT_BACK_CAM ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_SWAP_PIP) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_SWAP_PIP)  ,NULL    		 	},
    //{RECT_TOUCH_BUTTON_OK ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_ID_WIDE) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_ID_WIDE)  ,NULL    		 	},
#if (MENU_MOVIE_SIZE_1080P_EN||MENU_MOVIE_SIZE_SHD_30P_EN)
   // {RECT_TOUCH_BUTTON_FRONT_CAM ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_SWAP_FRONT_CAM) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_SWAP_FRONT_CAM) ,NULL    		 	 },
   // {RECT_TOUCH_BUTTON_BACK_CAM ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_SWAP_BACK_CAM) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_SWAP_BACK_CAM)  ,NULL    		 	},
   // {RECT_TOUCH_BUTTON_OK1 ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_ID_REC) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_ID_REC)  ,NULL    		 	},
#endif
  // {RECT_TOUCH_BUTTON_MUTE ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_MUTE) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_MUTE)  ,NULL    		 	},

};

SMENUTOUCHBUTTON VideoCtrlPage_TouchButton[] =
{
    {      POS_BUTTON_MENU ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_ID_MENU) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_ID_MENU)  ,NULL    		 	},
    {RECT_TOUCH_BUTTON_FRONT_BACK_CAM ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_SWAP_PIP) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_SWAP_PIP)  ,NULL    		 	},
    {RECT_TOUCH_BUTTON_OK ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_ID_REC) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_ID_REC)  ,NULL    		 	},
    //{RECT_TOUCH_BUTTON_FRONT_CAM ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_SWAP_FRONT_CAM) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_SWAP_FRONT_CAM) ,NULL    		 	 },
    //{RECT_TOUCH_BUTTON_BACK_CAM ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_SWAP_BACK_CAM) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_SWAP_BACK_CAM)  ,NULL    		 	},
    //{RECT_TOUCH_BUTTON_FRONT_BACK_CAM ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_SWAP_FRONT_BACK_CAM) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_SWAP_FRONT_BACK_CAM)  ,NULL    		 	},
    {RECT_TOUCH_BUTTON_MUTE ,  5,      TOUCH_PRESS_ID(VIDEO_TOUCH_MUTE) ,       TOUCH_MOVE_ID(VIDEO_TOUCH_MUTE)  ,NULL    		 	},

};

SMENUTOUCHBUTTON CaptureMainPage_TouchButton[] =
{
    {POS_BUTTON_MENU,       5, TOUCH_PRESS_ID(CAPT_TOUCH_ID_MENU),     TOUCH_MOVE_ID(CAPT_TOUCH_ID_MENU) ,NULL    		 	     },
    {POS_BUTTON_MODESWITCH, 5, TOUCH_PRESS_ID(CAPT_TOUCH_ID_MODE),     TOUCH_MOVE_ID(CAPT_TOUCH_ID_MODE)  ,NULL    		 	    },
    {RECT_TOUCH_BUTTON_OK, 5, TOUCH_PRESS_ID(CAPT_TOUCH_ID_CAPT),     TOUCH_MOVE_ID(CAPT_TOUCH_ID_CAPT)  ,NULL    		 	    },
};

SMENUTOUCHBUTTON CaptureCtrlPage_TouchButton[] =
{
    {POS_BUTTON_MENU,       5, TOUCH_PRESS_ID(CAPT_TOUCH_ID_MENU),     TOUCH_MOVE_ID(CAPT_TOUCH_ID_MENU)  ,NULL    		 	    },
    {POS_BUTTON_MODESWITCH, 5, TOUCH_PRESS_ID(CAPT_TOUCH_ID_MODE),     TOUCH_MOVE_ID(CAPT_TOUCH_ID_MODE)  ,NULL    		 	    },
    {POS_BUTTON_CAPTURE, 5, TOUCH_PRESS_ID(CAPT_TOUCH_ID_CAPT),     TOUCH_MOVE_ID(CAPT_TOUCH_ID_CAPT) ,NULL    		 	     },
};

SMENUTOUCHBUTTON Browser_TouchButton[] =
{
	{POS_MENU_EXIT_THIRD,   5,  TOUCH_PRESS_ID(THUMB_TOUCH_ID_MODE)     ,   TOUCH_MOVE_ID(THUMB_TOUCH_ID_MODE)	 ,NULL/*SubMenuItem_TouchPLAYBACK*/	},
	{POS_THUMB_CHANEL1,         	5, TOUCH_PRESS_ID(THUMB_TOUCH_ID_CHANEL1)   ,   TOUCH_MOVE_ID(THUMB_TOUCH_ID_CHANEL1)	 ,NULL  		 	},
    	{POS_THUMB_CHANEL2,       	5, TOUCH_PRESS_ID(THUMB_TOUCH_ID_CHANEL2) ,   TOUCH_MOVE_ID(THUMB_TOUCH_ID_CHANEL2),NULL 				},

	{RECT_TOUCH_BUTTON_BRO_MENU,5, TOUCH_PRESS_ID(THUMB_TOUCH_ID_MENU)     ,   TOUCH_MOVE_ID(THUMB_TOUCH_ID_MENU)	 ,NULL    		 	},
    //{POS_BUTTON_MENU_2,  		5, TOUCH_PRESS_ID(THUMB_TOUCH_ID_MODE)     ,   TOUCH_MOVE_ID(THUMB_TOUCH_ID_MODE)	 ,NULL    		 	},
    {POS_THUMB_PGUP,         	5, TOUCH_PRESS_ID(THUMB_TOUCH_ID_PAGEUP)   ,   TOUCH_MOVE_ID(THUMB_TOUCH_ID_PAGEUP)	 ,NULL  		 	},
    {POS_THUMB_PGDOWN,       	5, TOUCH_PRESS_ID(THUMB_TOUCH_ID_PAGEDOWN) ,   TOUCH_MOVE_ID(THUMB_TOUCH_ID_PAGEDOWN),NULL 				},
    {POS_THUMB_FOCUS,        	0, 	NULL									,   NULL								 ,BrowserFunc_Touch },
    
};

/* Photo Playback Mode */
SMENUTOUCHBUTTON JpgPBMainPage_TouchButton[] =
{
//    {RECT_SHOW_CTRL_PAGE,  		5,	TOUCH_PRESS_ID(JPGPB_TOUCH_ID_PAGE2),      	TOUCH_MOVE_ID(JPGPB_TOUCH_ID_PAGE2) }
    {RECT_TOUCH_BUTTON_OK,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_SLIDE),    	TOUCH_MOVE_ID(JPGPB_TOUCH_ID_SLIDE),NULL    		 	 },
    {RECT_TOUCH_BUTTON_MENU,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_MENU),   		TOUCH_MOVE_ID(JPGPB_TOUCH_ID_MENU) ,NULL    		 	 },
    {RECT_TOUCH_BUTTON_MODE,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_EXIT),   		TOUCH_MOVE_ID(JPGPB_TOUCH_ID_EXIT) ,NULL    		 	 }

}; 

SMENUTOUCHBUTTON JpgPBCtrlPage_TouchButton[] =
{
    {RECT_SHOW_CTRL_PAGE,  		5,	TOUCH_PRESS_ID(JPGPB_TOUCH_ID_PAGE1),      	TOUCH_MOVE_ID(JPGPB_TOUCH_ID_PAGE1),NULL    		 	 },
    {RECT_TOUCH_BUTTON_UP,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_PREV),    	TOUCH_MOVE_ID(JPGPB_TOUCH_ID_PREV) ,NULL    		 	 },
    {RECT_TOUCH_BUTTON_DOWN,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_NEXT),    	TOUCH_MOVE_ID(JPGPB_TOUCH_ID_NEXT) ,NULL    		 	 },
    {RECT_TOUCH_BUTTON_OK,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_SLIDE),    	TOUCH_MOVE_ID(JPGPB_TOUCH_ID_SLIDE),NULL    		 	 },
    {RECT_TOUCH_BUTTON_MENU,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_MENU),   		TOUCH_MOVE_ID(JPGPB_TOUCH_ID_MENU) ,NULL    		 	 },
    {RECT_TOUCH_BUTTON_MODE,  	5,  TOUCH_PRESS_ID(JPGPB_TOUCH_ID_EXIT),   		TOUCH_MOVE_ID(JPGPB_TOUCH_ID_EXIT) ,NULL    		 	 }
}; 

SMENUTOUCHBUTTON MovPBMainPage_TouchButton[] =
{
//    {RECT_SHOW_CTRL_PAGE,  		5,	TOUCH_PRESS_ID(MOVPB_TOUCH_ID_PAGE2),      	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_PAGE2) }
    {RECT_TOUCH_BUTTON_MOIVEPB_EXIT,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_EXIT),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_EXIT) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_PREV,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_PREV),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_PREV) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_NEXT,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_NEXT),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_NEXT) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_DEL,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_DEL),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_DEL) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_OK,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_PLAY),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_PLAY) ,NULL },

}; 

SMENUTOUCHBUTTON MovPBCtrlPage_TouchButton[] =
{
//    {RECT_SHOW_CTRL_PAGE,  		5,	TOUCH_PRESS_ID(MOVPB_TOUCH_ID_PAGE2),      	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_PAGE2) }
    {RECT_TOUCH_BUTTON_MOIVEPB_EXIT,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_EXIT),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_EXIT) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_PREV,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_PREV),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_PREV) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_NEXT,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_NEXT),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_NEXT) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_DEL,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_DEL),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_DEL) ,NULL },
    {RECT_TOUCH_BUTTON_MOIVEPB_OK,  	5,  TOUCH_PRESS_ID(MOVPB_TOUCH_ID_PLAY),    	TOUCH_MOVE_ID(MOVPB_TOUCH_ID_PLAY) ,NULL },

}; 

SMENUTOUCHBUTTON MainMenu_TouchButton[] =
{
    {RECT_TOUCH_BUTTON_MENU_EXIT,	0,   MENU_MENU,		MENU_EXIT_MOVE  	,NULL    	},
    //{POS_MAINMENU3,       			0,   TOUCH_PRESS_ID(VIDEO_TOUCH_ID_MODE),		TOUCH_PRESS_ID(VIDEO_TOUCH_ID_MODE) 		,NULL    	},
   // {POS_MENU_DOWN,     			0,   MENU_DOWN,		MENU_DOWN_MOVE 		,NULL    	},
   // {RECT_TOUCH_BUTTON_MENU_LEFT,   0,   MENU_LEFT,		MENU_LEFT_MOVE  	,NULL    	},
   // {RECT_TOUCH_BUTTON_MENU_RIGHT,  0,   MENU_RIGHT,	MENU_RIGHT_MOVE 	,NULL    	},
    {POS_MAINMENU0,     			0,   NULL,		MENU_MAIN0_MOVE ,MainMenuItem_Touch },
    {POS_MAINMENU1,     			0,   NULL,		MENU_MAIN1_MOVE ,MainMenuItem_Touch },
    {POS_MAINMENU2,     			0,   NULL,		MENU_MAIN2_MOVE ,MainMenuItem_Touch },
    {POS_MAINMENU3,     			0,   NULL,		MENU_MAIN3_MOVE ,MainMenuItem_Touch },

};
SMENUTOUCHBUTTON SubMenuPLAYBACK_TouchButton[] =
{
    {POS_MENU_EXIT_THIRD,   0,  NULL,   MENU_RETURN_MOVE,SubMenuItem_TouchThird 	},
    {POS_SUBMENUPLAYBACK1,   0,  NULL				,  MENU_SUB20_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENUPLAYBACK2,   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENUPLAYBACK3,   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
};

// 2 Sub Item
SMENUTOUCHBUTTON SubMenu2_TouchButton[] =
{
    {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,   MENU_RETURN_MOVE,NULL 	},
    {POS_SUBMENU20,   0,  NULL				,  MENU_SUB20_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENU21,   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
};
// 2 Sub Item--1
SMENUTOUCHBUTTON SubMenu2_TouchButton1[] =
{
    {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,   MENU_RETURN_MOVE,NULL 	},
    {POS_SUBMENU22,   0,  NULL				,  MENU_SUB20_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENU23,   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
};
SMENUTOUCHBUTTON SubMenu2e_TouchButton1[] =
{
    {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,   MENU_RETURN_MOVE,NULL 	},
    {{118,100,124,200},   0,  NULL				,  MENU_SUB20_MOVE ,return_goFront},
    {{246,100,124,200},   0,  NULL				,  MENU_SUB21_MOVE ,return_goFront},
};
SMENUTOUCHBUTTON SubMenu2c_TouchButton1[] =
{
    {POS_MENU_EXIT,   0,  MENU_MENU,   MENU_RETURN_MOVE,NULL 	},
    {{118,100,124,200},   0,  NULL				,  MENU_SUB20_MOVE ,SubMenuItem_TouchThird},
    {{246,100,124,200},   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
};
// 3 Sub Item
SMENUTOUCHBUTTON SubMenu3_TouchButton[] =
{
    {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,   MENU_RETURN_MOVE,NULL 	},
    {POS_SUBMENU30,   0,  NULL				,  MENU_SUB20_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENU31,   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENU32,   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
};
SMENUTOUCHBUTTON SubMenu3e_TouchButton[] =
{
    {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,   MENU_RETURN_MOVE,NULL 	},
    {{0,202,160,118},   0,  NULL				,  MENU_SUB20_MOVE ,SubMenuItem_TouchThird},
    {{160,202,160,118},   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
    {{320,202,160,118},   0,  NULL				,  MENU_SUB21_MOVE ,SubMenuItem_TouchThird},
};
// 4 Sub Item
SMENUTOUCHBUTTON SubMenu4_TouchButton[] =
{
    {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,      MENU_RETURN_MOVE ,NULL	},
    {POS_SUBMENU40,   0,  NULL		,       MENU_SUB40_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENU41,   0,  NULL		,       MENU_SUB41_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENU42,   0,  NULL		,       MENU_SUB42_MOVE ,SubMenuItem_TouchThird},
    {POS_SUBMENU43,   0,  NULL		,       MENU_SUB43_MOVE ,SubMenuItem_TouchThird},
};
SMENUTOUCHBUTTON SubMenu5e_TouchButton[] =
{
    {POS_MENU_EXIT,   0,  MENU_MENU,      MENU_RETURN_MOVE ,NULL	},
    {{0,135,96,150},   0,  NULL		,       MENU_SUB40_MOVE ,SubMenuItem_TouchThird},
    {{96,135,96,150},   0,  NULL		,       MENU_SUB41_MOVE ,SubMenuItem_TouchThird},
    {{192,135,96,150},   0,  NULL		,       MENU_SUB42_MOVE ,SubMenuItem_TouchThird},
    {{288,135,96,150},   0,  NULL		,       MENU_SUB43_MOVE ,SubMenuItem_TouchThird},
    {{384,135,96,150},   0,  NULL		,       MENU_SUB43_MOVE ,SubMenuItem_TouchThird},
};

// 6 Sub Item
SMENUTOUCHBUTTON SubMenu6_TouchButton[] =
{
    {POS_MENU_EXIT,   0,    MENU_MENU,      MENU_RETURN_MOVE,NULL	},
    //{POS_MENU_UP,     0,    MENU_UP,          MENU_UP_MOVE 	,NULL},
    //{POS_MENU_DOWN,   0,    MENU_DOWN,        MENU_DOWN_MOVE 	,NULL},
    {POS_SUBMENU60,   0,    NULL		,       MENU_SUB60_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU61,   0,    NULL		,       MENU_SUB61_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU62,   0,    NULL		,       MENU_SUB62_MOVE ,SubMenuItem_Touch},
    {{85,215,100,100},   0,    NULL		,       MENU_SUB63_MOVE ,SubMenuItem_Touch},
    {{190,215,100,100},   0,    NULL		,       MENU_SUB64_MOVE ,SubMenuItem_Touch},
    {{295,215,100,100},   0,    NULL		,       MENU_SUB65_MOVE ,SubMenuItem_Touch},
};

// 7 Sub Item
SMENUTOUCHBUTTON SubMenu7_TouchButton[] =
{
      {POS_MENU_EXIT,   0,    MENU_MENU,      MENU_RETURN_MOVE,NULL	},
    //{POS_MENU_UP,     0,    MENU_UP,          MENU_UP_MOVE 	,NULL},
    //{POS_MENU_DOWN,   0,    MENU_DOWN,        MENU_DOWN_MOVE 	,NULL},
    {POS_SUBMENU60,   0,    NULL		,       MENU_SUB60_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU61,   0,    NULL		,       MENU_SUB61_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU62,   0,    NULL		,       MENU_SUB62_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU63,   0,    NULL		,       MENU_SUB63_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU64,   0,    NULL		,       MENU_SUB64_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU65,   0,    NULL		,       MENU_SUB65_MOVE ,SubMenuItem_Touch},
    {POS_SUBMENU66,   0,    NULL		,       MENU_SUB65_MOVE ,SubMenuItem_Touch},
};
SMENUTOUCHBUTTON SubMenu8_TouchButton[] =
{
      {POS_MENU_EXIT,   0,    MENU_MENU,      MENU_RETURN_MOVE,NULL	},
    {{35,110,100,100},   0,    NULL		,       MENU_SUB60_MOVE ,SubMenuItem_Touch},
    {{140,110,100,100},   0,    NULL		,       MENU_SUB61_MOVE ,SubMenuItem_Touch},
    {{245,110,100,100},   0,    NULL		,       MENU_SUB62_MOVE ,SubMenuItem_Touch},
    {{350,110,100,100},   0,    NULL		,       MENU_SUB63_MOVE ,SubMenuItem_Touch},
    {{35,215,100,100},   0,    NULL		,       MENU_SUB64_MOVE ,SubMenuItem_Touch},
    {{140,215,100,100},   0,    NULL		,       MENU_SUB65_MOVE ,SubMenuItem_Touch},
    {{245,215,100,100},   0,    NULL		,       MENU_SUB65_MOVE ,SubMenuItem_Touch},
    {{350,215,100,100},   0,    NULL		,       MENU_SUB65_MOVE ,SubMenuItem_Touch},

};
SMENUTOUCHBUTTON Touchadjust_TouchButton[] =
{
    
    {{10,10,460,300},   0,    NULL		,       NULL ,SubMenu_Touchadjust},
};

SMENUTOUCHBUTTON SubMenu_settingtime[] =
{
    {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,      MENU_RETURN_MOVE ,NULL},
    //{POS_MENU_UP,     0,    MENU_UP,          MENU_UP_MOVE 	,NULL},
    //{POS_MENU_DOWN,   0,    MENU_DOWN,        MENU_DOWN_MOVE 	,NULL},
    {POS_SUBMENUCLOCK,   0,    NULL		,       MENU_SUB60_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUCLOCK1,   0,    NULL		,       MENU_SUB61_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUCLOCK2,   0,    NULL		,       MENU_SUB62_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUCLOCK3,   0,    NULL		,       MENU_SUB63_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUCLOCK4,   0,    NULL		,       MENU_SUB64_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUCLOCK5,   0,    NULL		,       MENU_SUB65_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUCLOCK6,   0,    NULL		,       MENU_SUB65_MOVE ,ThirdMenuItem_Touch},
	{POS_SUBMENUCLOCK7,   0,    NULL		,       MENU_SUB65_MOVE ,ThirdMenuItem_Touch},
};

SMENUTOUCHBUTTON SubMenusetvolt_TouchButton[] =
{
     {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,      MENU_RETURN_MOVE ,NULL},
    //{POS_MENU_UP,     0,    MENU_UP,          MENU_UP_MOVE 	,NULL},
    //{POS_MENU_DOWN,   0,    MENU_DOWN,        MENU_DOWN_MOVE 	,NULL},
    {POS_SUBMENUsetvolt1,   0,    NULL		,       MENU_SUB60_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvolt2,   0,    NULL		,       MENU_SUB61_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvolt3,   0,    NULL		,       MENU_SUB62_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvolt4,   0,    NULL		,       MENU_SUB63_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvolt5,   0,    NULL		,       MENU_SUB64_MOVE ,NULL},
    
};
SMENUTOUCHBUTTON SubMenusetvoice_TouchButton[] =
{
     {POS_MENU_EXIT_THIRD,   0,  MENU_MENU,      MENU_RETURN_MOVE ,NULL},
    //{POS_MENU_UP,     0,    MENU_UP,          MENU_UP_MOVE 	,NULL},
    //{POS_MENU_DOWN,   0,    MENU_DOWN,        MENU_DOWN_MOVE 	,NULL},
    {POS_SUBMENUsetvoice0,   0,    NULL		,       MENU_SUB60_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvoice1,   0,    NULL		,       MENU_SUB61_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvoice2,   0,    NULL		,       MENU_SUB62_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvoice3,   0,    NULL		,       MENU_SUB63_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvoicey,   0,    NULL		,       MENU_SUB64_MOVE ,ThirdMenuItem_Touch},
    {POS_SUBMENUsetvoicen,   0,    NULL		,       MENU_SUB64_MOVE ,ThirdMenuItem_Touch},
    
};

SMENUTOUCHBUTTON MenuConfirm_TouchButton[] =
{
    {RECT_TOUCH_BUTTON_MENU_EXIT,   0,  MENU_MENU,  MENU_RETURN_MOVE,NULL  }, 
    {RECT_TOUCH_BUTTON_MENU_YES,    0,  NULL	,   MENU_YES_MOVE 	,SubMenuComfirm_Touch  },
    {RECT_TOUCH_BUTTON_MENU_NO,    	0,  NULL	,   MENU_NO_MOVE   	,SubMenuComfirm_Touch  },
};

//Info Page
SMENUTOUCHBUTTON MenuInfo_TouchButton[] =
{
    {RECT_TOUCH_BUTTON_MENU_EXIT,   0,  MENU_MENU,      MENU_RETURN_MOVE,NULL  },
};

SMENUTOUCHBUTTON MenuClockSetting_TouchButton[] =
{
    {RECT_TOUCH_BUTTON_MENU_EXIT,   	0,  MENU_MENU,     MENU_RETURN_MOVE,NULL },
    {RECT_TOUCH_BUTTON_CLOCK_UP,       	0,  MENU_UP,         MENU_UP_MOVE ,NULL	},
    {RECT_TOUCH_BUTTON_CLOCK_DOWN,     	0,  MENU_DOWN,       MENU_DOWN_MOVE ,NULL},
    {RECT_TOUCH_BUTTON_CLOCK_OK,       	0,  MENU_OK,         MENU_OK_MOVE 	,NULL},
};

SMENUTOUCHBUTTON Browser_FileEditTouchButton[] =
{
    {RECT_MENU_CONFIRM_YES,	5, 	NULL, TOUCH_MOVE_ID(THUMB_TOUCH_ID_EDIT_YES),Browser_FileEdit_Touch },
    {RECT_MENU_CONFIRM_NO,	5, 	NULL , TOUCH_MOVE_ID(THUMB_TOUCH_ID_EDIT_NO) ,Browser_FileEdit_Touch },
};

SMENUTOUCHBUTTON MenuEV_TouchButton[] =
{
    {POS_MENU_EXIT,           	0,    MENU_MENU,          MENU_EXIT_MOVE 	,NULL	},
    {RECT_TOUCH_BUTTON_MENU_OK, 0,    MENU_OK,      		MENU_OK_MOVE 	,NULL	},
    {POS_MENU_EVDECREASE,     	0,    MENU_DOWN,		MENU_DECREASE_MOVE 	,NULL},
    {POS_MENU_EVINCREASE,     	0,    MENU_UP,		MENU_INCREASE_MOVE 	,NULL},
};

SMENUTOUCHBUTTON MenuVolume_TouchButton[] =
{
    {POS_MENU_EXIT,           	0,    MENU_MENU,          MENU_EXIT_MOVE 	,NULL	}, 
    {RECT_TOUCH_BUTTON_MENU_OK, 0,    MENU_OK,      		MENU_OK_MOVE 	,NULL	}, 
    {POS_MENU_EVDECREASE,     	0,    MENU_DOWN,		MENU_DECREASE_MOVE ,NULL	},
    {POS_MENU_EVINCREASE,     	0,    MENU_UP,		MENU_INCREASE_MOVE ,NULL	},
};

#ifdef CFG_VIRTUAL_TOUCH
SMENUTOUCHBUTTON MenuDriverIdSetting_TouchButton[] =
{
    {RECT_TOUCH_BUTTON_DRIVEID_EXIT,   0,  MENU_RETURN_PRESS,      MENU_RETURN_MOVE 	},
    {RECT_TOUCH_BUTTON_DRIVEID_OK,       	0,  MENU_OK_PRESS,         MENU_OK_MOVE 	},
    {POS_SUBMENU_DRIVEID_00,   0,  MENU_SUB_DRIVEID_00_PRESS,       MENU_SUB_DRIVEID_00_MOVE 	},
    {POS_SUBMENU_DRIVEID_01,   0,  MENU_SUB_DRIVEID_01_PRESS,       MENU_SUB_DRIVEID_01_MOVE 	},
    {POS_SUBMENU_DRIVEID_10,   0,  MENU_SUB_DRIVEID_10_PRESS,       MENU_SUB_DRIVEID_10_MOVE 	},
    {POS_SUBMENU_DRIVEID_11,   0,  MENU_SUB_DRIVEID_11_PRESS,       MENU_SUB_DRIVEID_11_MOVE 	},
    {POS_SUBMENU_DRIVEID_20,   0,  MENU_SUB_DRIVEID_20_PRESS,       MENU_SUB_DRIVEID_20_MOVE 	},
    {POS_SUBMENU_DRIVEID_21,   0,  MENU_SUB_DRIVEID_21_PRESS,       MENU_SUB_DRIVEID_21_MOVE 	},
    {POS_SUBMENU_DRIVEID_30,   0,  MENU_SUB_DRIVEID_30_PRESS,       MENU_SUB_DRIVEID_30_MOVE 	},
    {POS_SUBMENU_DRIVEID_31,   0,  MENU_SUB_DRIVEID_31_PRESS,       MENU_SUB_DRIVEID_31_MOVE 	},
    {POS_SUBMENU_DRIVEID_40,   0,  MENU_SUB_DRIVEID_40_PRESS,       MENU_SUB_DRIVEID_40_MOVE 	},
    {POS_SUBMENU_DRIVEID_41,   0,  MENU_SUB_DRIVEID_41_PRESS,       MENU_SUB_DRIVEID_41_MOVE 	},
    {POS_SUBMENU_DRIVEID_50,   0,  MENU_SUB_DRIVEID_50_PRESS,       MENU_SUB_DRIVEID_50_MOVE 	},
    {POS_SUBMENU_DRIVEID_51,   0,  MENU_SUB_DRIVEID_51_PRESS,       MENU_SUB_DRIVEID_51_MOVE 	},
    {POS_SUBMENU_DRIVEID_60,   0,  MENU_SUB_DRIVEID_60_PRESS,       MENU_SUB_DRIVEID_60_MOVE 	},
    {POS_SUBMENU_DRIVEID_61,   0,  MENU_SUB_DRIVEID_61_PRESS,       MENU_SUB_DRIVEID_61_MOVE 	},
    {POS_SUBMENU_DRIVEID_70,   0,  MENU_SUB_DRIVEID_70_PRESS,       MENU_SUB_DRIVEID_70_MOVE 	},
    {POS_SUBMENU_DRIVEID_71,   0,  MENU_SUB_DRIVEID_71_PRESS,       MENU_SUB_DRIVEID_71_MOVE 	},
    
};
#endif
#endif

