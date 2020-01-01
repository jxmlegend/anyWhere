/**
 @file AHC_Display.c
 @brief AHC display control Function
 @author
 @version 1.0
*/

/*===========================================================================
 * Include files
 *===========================================================================*/

#include "Customer_Config.h"
#include "AIHC_Browser.h"
#include "AHC_Display.h"
#include "AHC_General.h"
#include "AHC_Capture.h"
#include "AHC_DCF.h"
#include "AHC_GUI.h"
#include "AHC_OS.h"
#include "AHC_Parameter.h"

#include "ait_config.h"
#include "mmps_vidplay.h"
#include "mmps_3gprecd.h"
#include "mmps_uvcrecd.h"
#include "mmps_display.h"
#include "mmpf_display.h"
#include "mmpf_pio.h"
#include "mmpf_pll.h"
#include "ColorDefine.h"
#include "IconDefine.h"
#include "MenuSetting.h"
#include "snr_cfg.h"

/** @addtogroup AHC_DISPLAY
@{
*/

/*===========================================================================
 * Global varible
 *===========================================================================*/
#if defined(CCIR656_OUTPUT_ENABLE)&&(CCIR656_OUTPUT_ENABLE)
static MMP_DISPLAY_OUTPUT_SEL   m_ulOutputDevice = MMP_DISPLAY_SEL_CCIR;
#else
static MMP_DISPLAY_OUTPUT_SEL   m_ulOutputDevice = MMP_DISPLAY_SEL_MAIN_LCD;
#endif
static UINT16                   m_usAHCDispStartX;
static UINT16                   m_usAHCDispStartY;
static UINT16                   m_usAHCDispWidth;
static UINT16                   m_usAHCDispHeight;
static MMP_DISPLAY_DUPLICATE    m_dupWinDupMode = MMP_DISPLAY_DUPLICATE_1X;

static UINT8                    m_WaitID = 0;
static AHC_BOOL                 m_WaitIcon = AHC_FALSE;
static AHC_BOOL                 m_EnableSendHDMIEvent = AHC_FALSE;

/* Add for recording battery icon states */
static UINT8                    rcIconID = BATTERY_VOLTAGE_NUM;

MMP_DISPLAY_OUTPUT_SEL          gsAhcCurrentDisplay     = MMP_DISPLAY_SEL_MAIN_LCD;

/*===========================================================================
 * Extern varible
 *===========================================================================*/ 

extern MMP_USHORT               gsAhcPrmSensor;
extern MMP_USHORT               gsAhcScdSensor;
extern AHC_BOOL                 gbAhcDbgBrk;

/*===========================================================================
 * Extern function
 *===========================================================================*/

extern void OSDDraw_GetLastOvlDrawBuffer( UINT8* ulID);
extern MMP_BOOL MMPF_HDMI_IsConnect(void);
void InitOSD_TVout(void);
void InitOSD_LCD(void);
void InitOSD_HDMI(void);
#ifdef BATTERY_ICON_MAPPING
extern UINT8 Mapping_BatteryIndex(BATTERY_VOLTAGE BatteryVol) ;
#endif
#if (BATTERY_ICON_PROC_DIFF_STATE_WITH_FUNC)
extern void AHC_ProcessBatteryIconWithDiffState(void);
#endif

/*===========================================================================
 * Main body
 *===========================================================================*/

#if 0
void _____Internal_Functions_____(){}
#endif

//------------------------------------------------------------------------------
//  Function    : AIHC_GetWinDupMode
//  Description : 
//------------------------------------------------------------------------------ 
UINT16 AIHC_GetWinDupMode(void)
{
    return m_dupWinDupMode;
}

//------------------------------------------------------------------------------
//  Function    : AIHC_Display_EnterHdmi
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AIHC_Display_EnterHdmi(void)
{
	UINT16 						PBHeight;
	MMPS_VIDPLAY_SCALERCONFIG	sScalerCfg;
	MMPS_AUDIO_I2S_CFG   		m_I2Scfg;
	MMP_UBYTE                   ubI2Sch = MMP_I2S_CH0;
	UINT16                      DispWidth, DispHeight;

	MMPS_Display_SetOutputPanel(MMP_DISPLAY_PRM_CTL, MMP_DISPLAY_SEL_NONE);

    MMPS_VIDPLAY_SetDisplayType(MMP_DISPLAY_SEL_HDMI);

    MMPS_Display_SetOutputPanel(MMP_DISPLAY_PRM_CTL, MMP_DISPLAY_SEL_HDMI);

    m_I2Scfg.workingMode      = MMP_I2S_MASTER_MODE;
    m_I2Scfg.mclkMode         = MMP_I2S_FIX_256FS_MODE;
    m_I2Scfg.outputBitSize    = MMP_I2S_OUT_16BIT;
    m_I2Scfg.lrckPolarity     = MMP_I2S_LRCK_L_LOW;
    m_I2Scfg.outDataDelayMode = MMP_I2S_OUT_DELAY_STD;
    m_I2Scfg.bOutputBySDI     = MMP_FALSE;
    m_I2Scfg.bInputFromSDO    = MMP_FALSE;
    m_I2Scfg.ubInBitAlign     = 0;
    m_I2Scfg.ubOutBitAlign    = 16;

    MMPS_AUDIO_InitializeI2SConfig(ubI2Sch, &m_I2Scfg);
    MMPS_AUDIO_EnableI2SMclk(ubI2Sch,MMP_TRUE, 44100, 0);
    MMPS_AUDIO_SetOutPath(MMPS_AUDIO_OUT_I2S0);

    AIHC_GetCurrentPBHeight(&PBHeight);

    AHC_Display_GetWidthHdight(&DispWidth, &DispHeight);//#if defined(CCIR656_OUTPUT_ENABLE)&&(CCIR656_OUTPUT_ENABLE)

	MMPS_VIDPLAY_SetScalerCfg(&sScalerCfg, sScalerCfg.bUseScaler, DispWidth, DispHeight, 0, 0);
    if ((PBHeight == 1088) && (1080 == sScalerCfg.ulOutHeight)) {
        // 1080P@60 VR v.s. 1080P output => no scaling
        sScalerCfg.ulOutWidth  = 0;
        sScalerCfg.ulOutHeight = 0;
        sScalerCfg.bUseScaler  = MMP_FALSE;
    }
    else if (sScalerCfg.ulOutHeight < PBHeight) {
        sScalerCfg.bUseScaler = MMP_TRUE;

        if (sScalerCfg.ulOutHeight > AHC_HD_VIDPLAY_MAX_HEIGHT) {
            sScalerCfg.ulOutWidth = AHC_HD_VIDPLAY_MAX_WIDTH;
            sScalerCfg.ulOutHeight = AHC_HD_VIDPLAY_MAX_HEIGHT;
        }
    }
    else {
        sScalerCfg.ulOutWidth 	= 0;
	    sScalerCfg.ulOutHeight 	= 0;
	    sScalerCfg.bUseScaler 	= MMP_FALSE;
    }

    MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_HDMI, MMP_DISPLAY_ROTATE_MAX, MMP_FALSE, NULL, &sScalerCfg);

    MMPS_AUDIO_SetPlayVolumeDb(95);

    MMPS_Display_SetWinTransparent(WMSG_LAYER_WINDOW_ID, MMP_TRUE, 0x0);

    MMPS_Display_SetDisplayRefresh(MMP_DISPLAY_PRM_CTL);

	return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AIHC_Display_EnterCCIR656
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AIHC_Display_EnterCCIR656(void)//#if defined(CCIR656_OUTPUT_ENABLE)&&(CCIR656_OUTPUT_ENABLE)
{
    MMPS_VIDPLAY_SCALERCONFIG sScalerCfg;

    MMPS_VIDPLAY_SetDisplayType(MMP_DISPLAY_SEL_CCIR);

    MMPS_Display_SetOutputPanel(MMP_DISPLAY_PRM_CTL, MMP_DISPLAY_SEL_CCIR);

#if 1//(VERTICAL_LCD == 0)
    MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_CCIR, MMP_DISPLAY_ROTATE_MAX, MMP_FALSE, NULL, &sScalerCfg);
#else
    MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_NTSC_TV, MMP_DISPLAY_ROTATE_NO_ROTATE, MMP_FALSE, NULL, &sScalerCfg);
#endif

	return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AIHC_Display_EnterNtscTv
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AIHC_Display_EnterNtscTv(void)
{
	MMPS_VIDPLAY_SCALERCONFIG sScalerCfg;
    #if defined(AHC_CUS_TV_GAMMA_CURVE)
    MMP_DISPLAY_TV_GAMMA DSPY_TVGamma;
    #endif
	MMPS_VIDPLAY_SetDisplayType(MMP_DISPLAY_SEL_NTSC_TV);

    MMPS_Display_SetOutputPanel(MMP_DISPLAY_PRM_CTL, MMP_DISPLAY_SEL_NTSC_TV);

    #ifdef AHC_CUS_TV_NTSC_BRIGHT
    MMPD_TV_SetBrightLevel(AHC_CUS_TV_NTSC_BRIGHT);
    #endif
    #ifdef AHC_CUS_TV_NTSC_CONTRAST
    MMPD_TV_SetContrast(AHC_CUS_TV_NTSC_CONTRAST);
    #endif
    #if defined(AHC_CUS_TV_NTSC_SATURATION_U) && defined(AHC_CUS_TV_NTSC_SATURATION_V)
    MMPD_TV_SetSaturation(AHC_CUS_TV_NTSC_SATURATION_U, AHC_CUS_TV_NTSC_SATURATION_V);
    #endif
    #if defined(AHC_CUS_TV_GAMMA_CURVE)
    AIHC_Display_SetTVGamma(DSPY_TVGamma);
    #endif      
    MMPS_AUDIO_SetOutPath(MMPS_AUDIO_OUT_AFE_HP | MMPS_AUDIO_OUT_AFE_LINE); // TBD

	MMPS_VIDPLAY_SetScalerCfg(&sScalerCfg, MMP_FALSE, 0, 0, 0, 0);
    #if 1//(VERTICAL_LCD == 0)
    MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_NTSC_TV, MMP_DISPLAY_ROTATE_MAX, MMP_FALSE, NULL, &sScalerCfg);
	#else
	MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_NTSC_TV, MMP_DISPLAY_ROTATE_NO_ROTATE, MMP_FALSE, NULL, &sScalerCfg);
	#endif

	return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AIHC_Display_EnterPalTv
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AIHC_Display_EnterPalTv(void)
{
	MMPS_VIDPLAY_SCALERCONFIG sScalerCfg;
    #if defined(AHC_CUS_TV_GAMMA_CURVE)
    MMP_DISPLAY_TV_GAMMA DSPY_TVGamma;
    #endif
	MMPS_VIDPLAY_SetDisplayType(MMP_DISPLAY_SEL_PAL_TV);

    MMPS_Display_SetOutputPanel(MMP_DISPLAY_PRM_CTL, MMP_DISPLAY_SEL_PAL_TV);

    #ifdef AHC_CUS_TV_PAL_BRIGHT
    MMPD_TV_SetBrightLevel(AHC_CUS_TV_PAL_BRIGHT);
    #endif
    #ifdef AHC_CUS_TV_PAL_CONTRAST
    MMPD_TV_SetContrast(AHC_CUS_TV_PAL_CONTRAST);
    #endif
    #if defined(AHC_CUS_TV_PAL_SATURATION_U) && defined(AHC_CUS_TV_PAL_SATURATION_V)
    MMPD_TV_SetSaturation(AHC_CUS_TV_PAL_SATURATION_U, AHC_CUS_TV_PAL_SATURATION_V);
    #endif
    #if defined(AHC_CUS_TV_GAMMA_CURVE)
    AIHC_Display_SetTVGamma(DSPY_TVGamma);  
    #endif    
    MMPS_AUDIO_SetOutPath(MMPS_AUDIO_OUT_AFE_HP | MMPS_AUDIO_OUT_AFE_LINE); // TBD

	MMPS_VIDPLAY_SetScalerCfg(&sScalerCfg, MMP_FALSE, 0, 0, 0, 0);
    #if 1//(VERTICAL_LCD == 0)
    MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_PAL_TV, MMP_DISPLAY_ROTATE_MAX, MMP_FALSE, NULL, &sScalerCfg);
	#else
	MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_PAL_TV, MMP_DISPLAY_ROTATE_NO_ROTATE, MMP_FALSE, NULL, &sScalerCfg);
	#endif

	return AHC_TRUE;
}

AHC_BOOL AIHC_Display_SetTVGamma(MMP_DISPLAY_TV_GAMMA DSPY_TVGamma)
{
    MMPD_TV_SetGamma0(DSPY_TVGamma.usGamma0);
    MMPD_TV_SetGamma1(DSPY_TVGamma.usGamma1);
    MMPD_TV_SetGamma2(DSPY_TVGamma.usGamma2);
    MMPD_TV_SetGamma3(DSPY_TVGamma.usGamma3);
    MMPD_TV_SetGamma4(DSPY_TVGamma.usGamma4);
    MMPD_TV_SetGamma5(DSPY_TVGamma.usGamma5);
    MMPD_TV_SetGamma6(DSPY_TVGamma.usGamma6);
    MMPD_TV_SetGamma7(DSPY_TVGamma.usGamma7);
    MMPD_TV_SetGamma8(DSPY_TVGamma.usGamma8);
	return AHC_TRUE;
}

AHC_BOOL AIHC_Display_GetTVGamma(MMP_DISPLAY_TV_GAMMA *DSPY_TVGamma)
{
    MMPD_TV_GetGamma(DSPY_TVGamma);
	return AHC_TRUE;
}
//------------------------------------------------------------------------------
//  Function    : AIHC_Display_EnterLcd
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AIHC_Display_EnterLcd(void)
{
    MMPS_VIDPLAY_SCALERCONFIG sScalerCfg;

    MMPS_Display_SetOutputPanel(MMP_DISPLAY_PRM_CTL, MMP_DISPLAY_SEL_NONE);
    
    MMPS_VIDPLAY_SetDisplayType(MMP_DISPLAY_SEL_MAIN_LCD);
    
    MMPS_Display_SetOutputPanel(MMP_DISPLAY_PRM_CTL, MMP_DISPLAY_SEL_MAIN_LCD);
    
    MMPS_AUDIO_SetOutPath(MMPS_AUDIO_OUT_AFE_HP | MMPS_AUDIO_OUT_AFE_LINE);

    MMPS_VIDPLAY_SetScalerCfg(&sScalerCfg, MMP_FALSE, 0, 0, 0, 0);
    MMPS_VIDPLAY_SetDisplayMode(VIDEOPLAY_MODE_STD_LCD, MMP_DISPLAY_ROTATE_NO_ROTATE, MMP_FALSE, NULL, &sScalerCfg);

    return AHC_TRUE;
}

#if 0
void _____Display_Functions_____(){}
#endif

/* Set Display type at video record mode. Default is LCD,
   Change it if no LCD, like as TV out only kit
 */
void AHC_SetCurrentDisplayEx(MMP_DISPLAY_OUTPUT_SEL kind)
{
    gsAhcCurrentDisplay = kind;
}

//------------------------------------------------------------------------------
//  Function    : AHC_SetHDMIOutputMode
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_SetHDMIOutputMode(AHC_DISPLAY_HDMIOUTPUTMODE mode)
{
    switch(mode) {
    case AHC_DISPLAY_HDMIOUTPUT_640X480P:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_640X480P;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  =  640;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight =  480;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_720X480P:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_720X480P;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  =  720;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight =  480;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_720X576P:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_720X576P;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  =  720;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight =  576;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_1280X720P:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1280X720P;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1280;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight =  720;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_1280X720P_50FPS:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1280X720P_50FPS;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1280;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight =  720;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_1280X720P_100FPS:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1280X720P_100FPS;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1280;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight =  720;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_1280X720P_120FPS:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1280X720P_120FPS;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1280;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight =  720;
    break;    
    case AHC_DISPLAY_HDMIOUTPUT_1920X1080P:
  		MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1920X1080P;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1920;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight = 1080;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_1920X1080P_30FPS:
  		MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1920X1080P_30FPS;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1920;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight = 1080;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_1920X1080P_50FPS:
  		MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1920X1080P_50FPS;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1920;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight = 1080;
    break;
    case AHC_DISPLAY_HDMIOUTPUT_1920X1080I:
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1920X1080I;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1920;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight = 1080/2;
    break;
    default:
        printc("--E-- %s: Wrong or unsupported HDMI mode !!!\r\n", __func__);
        MMPS_Display_GetConfig()->hdmi.ubOutputMode = MMP_HDMI_OUTPUT_1920X1080P;
        MMPS_Display_GetConfig()->hdmi.usDisplayWidth  = 1920;
        MMPS_Display_GetConfig()->hdmi.usDisplayHeight = 1080;
    break;
    }

    MMPS_VIDPLAY_GetConfig()->config[VIDEOPLAY_MODE_HDMI].display.usWidth = MMPS_Display_GetConfig()->hdmi.usDisplayWidth;
    MMPS_VIDPLAY_GetConfig()->config[VIDEOPLAY_MODE_HDMI].display.usHeight= MMPS_Display_GetConfig()->hdmi.usDisplayHeight;

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetHDMIOutputMode
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_GetHDMIOutputMode(AHC_DISPLAY_HDMIOUTPUTMODE *pMode)
{
    *pMode = (AHC_DISPLAY_HDMIOUTPUTMODE)(MMPS_Display_GetConfig()->hdmi.ubOutputMode);

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetHdmiDisplayWidthHeight
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_GetHdmiDisplayWidthHeight(UINT16 *uwDisplayW, UINT16 *uwDisplayH)
{
    *uwDisplayW = MMPS_Display_GetConfig()->hdmi.usDisplayWidth;
    *uwDisplayH = MMPS_Display_GetConfig()->hdmi.usDisplayHeight;

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetNtscTvDisplayWidthHeight
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_GetNtscTvDisplayWidthHeight(UINT16 *uwDisplayW, UINT16 *uwDisplayH)
{
    *uwDisplayW = MMPS_Display_GetConfig()->ntsctv.usDisplayWidth;
	*uwDisplayH = MMPS_Display_GetConfig()->ntsctv.usDisplayHeight;

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetPalTvDisplayWidthHeight
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_GetPalTvDisplayWidthHeight(UINT16 *uwDisplayW, UINT16 *uwDisplayH)
{
    *uwDisplayW = MMPS_Display_GetConfig()->paltv.usDisplayWidth;
    *uwDisplayH = MMPS_Display_GetConfig()->paltv.usDisplayHeight;

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetCCIR656DisplayWidthHeight
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_GetCCIR656DisplayWidthHeight(UINT16 *uwDisplayW, UINT16 *uwDisplayH)
{
    *uwDisplayW = MMPS_Display_GetConfig()->ccir.usDisplayWidth;
    *uwDisplayH = MMPS_Display_GetConfig()->ccir.usDisplayHeight;

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AIHC_Display_GetWidthHdight
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_Display_GetWidthHdight(UINT16 *uwDisplayW, UINT16 *uwDisplayH)
{
    AHC_DISPLAY_OUTPUTPANEL  	OutputDevice;

    AHC_GetDisplayOutputDev(&OutputDevice);

    switch(OutputDevice) {
        case MMP_DISPLAY_SEL_HDMI:
            AHC_GetHdmiDisplayWidthHeight(uwDisplayW, uwDisplayH);
            break;

        case MMP_DISPLAY_SEL_NTSC_TV:
            AHC_GetNtscTvDisplayWidthHeight(uwDisplayW, uwDisplayH);
            break;

        case MMP_DISPLAY_SEL_PAL_TV:
            AHC_GetPalTvDisplayWidthHeight(uwDisplayW, uwDisplayH);
            break;

        case AHC_DISPLAY_MAIN_LCD:
            *uwDisplayW = RTNA_LCD_GetAttr()->usPanelW;
            *uwDisplayH = RTNA_LCD_GetAttr()->usPanelH;
            break;
        case AHC_DISPLAY_CCIR:
            AHC_GetCCIR656DisplayWidthHeight(uwDisplayW, uwDisplayH);                
            break;
        default:
            *uwDisplayW = 320;
            *uwDisplayH = 240;
            AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); 
            return AHC_FALSE; 
            break;
    }

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_SetDisplayMode
//  Description : 
//------------------------------------------------------------------------------ 
/**
 @brief Set display mode

 Sets the new display mode and switching to it.
 Parameters:
 @param[in] byMode Select a mode of display and switching to it: AHC_DISPLAY_MODE.
 @retval AHC_TRUE Success.
*/
AHC_BOOL AHC_SetDisplayMode(UINT8 byMode)
{
#if defined(CCIR656_OUTPUT_ENABLE) && (CCIR656_OUTPUT_ENABLE)
	return AHC_TRUE;
#endif

    switch(byMode) {
		case DISPLAY_MODE_DISABLE:
			MMPS_Display_SetWinActive(UPPER_IMAGE_WINDOW_ID     , MMP_FALSE);
			MMPS_Display_SetWinActive(OSD_LAYER_WINDOW_ID       , MMP_FALSE);
			MMPS_Display_SetWinActive(LOWER_IMAGE_WINDOW_ID     , MMP_FALSE);
			MMPS_Display_SetWinActive(WMSG_LAYER_WINDOW_ID      , MMP_FALSE);
			break;
		case DISPLAY_MODE_PIP_DISABLE:
		    MMPS_Display_SetWinActive(LOWER_IMAGE_WINDOW_ID     , MMP_FALSE);
			break;
		case DISPLAY_MODE_PIP_ENABLE:
		    MMPS_Display_SetWinActive(LOWER_IMAGE_WINDOW_ID     , MMP_TRUE);
			break;
		case DISPLAY_MODE_OSD_DISABLE:
		    MMPS_Display_SetWinActive(OSD_LAYER_WINDOW_ID       , MMP_FALSE);
			break;
		case DISPLAY_MODE_OSD_ENABLE:
		    MMPS_Display_SetWinActive(OSD_LAYER_WINDOW_ID       , MMP_TRUE);
			break;
		case DISPLAY_MODE_ENABLE: // CHECK
		    MMPS_Display_SetWinActive(LOWER_IMAGE_WINDOW_ID     , MMP_TRUE);
			MMPS_Display_SetWinActive(OSD_LAYER_WINDOW_ID       , MMP_TRUE);
			MMPS_Display_SetWinActive(UPPER_IMAGE_WINDOW_ID     , MMP_FALSE);
			MMPS_Display_SetWinActive(WMSG_LAYER_WINDOW_ID      , MMP_FALSE);
			break;
        default:
            return AHC_FALSE;
            break;
    }

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_SetDisplayOutputDev
//  Description :
//------------------------------------------------------------------------------
/**
 @brief Set video output format

 Sets the video output format. After configuration, the display output will switch
 to the selected format immediately. This operation will make the specific display
 driver be involved.
 Parameters:
 @param[in] byMode Select the output format: AHC_DISPLAY_OUT_FORMAT.
 @retval AHC_TRUE Success.
*/
AHC_BOOL AHC_SetDisplayOutputDev(AHC_DISPLAY_OUT_FORMAT byMode, AHC_DISPLAY_DUPLICATE byOSDDuplicate)
{
    UINT32           	CurMode;
    MMPC_3GPPLAY_MODE	VideoPMode = VIDEOPLAY_MODE_STD_LCD;

    AHC_GetSystemStatus(&CurMode);

    CurMode >>= 16;

    if (CurMode != AHC_MODE_IDLE)
    {
        printc("--E--  %s - mode is not idle mode\r\n", __func__);
        return AHC_FALSE;
    }

    m_dupWinDupMode = byOSDDuplicate;

    switch(byMode) {
    case DISP_OUT_TV_NTSC:
        m_ulOutputDevice    = MMP_DISPLAY_SEL_NTSC_TV;
        VideoPMode          = VIDEOPLAY_MODE_NTSC_TV;
        AIHC_Display_EnterNtscTv();
        break;
    case DISP_OUT_TV_PAL:
        m_ulOutputDevice    = MMP_DISPLAY_SEL_PAL_TV;
        VideoPMode          = VIDEOPLAY_MODE_PAL_TV;
        AIHC_Display_EnterPalTv();
        break;
    case DISP_OUT_HDMI :
        m_ulOutputDevice    = MMP_DISPLAY_SEL_HDMI;
        VideoPMode          = VIDEOPLAY_MODE_HDMI;
        AIHC_Display_EnterHdmi();
        break;
    case DISP_OUT_LCD:
        m_ulOutputDevice    = MMP_DISPLAY_SEL_MAIN_LCD;
        VideoPMode          = VIDEOPLAY_MODE_STD_LCD;
        AIHC_Display_EnterLcd();
        break;
    case DISP_OUT_CCIR656:
        m_ulOutputDevice    = MMP_DISPLAY_SEL_CCIR;
        VideoPMode          = VIDEOPLAY_MODE_CCIR;
        AIHC_Display_EnterCCIR656();
        break;        
    default:
        return AHC_FALSE;
        break;
    }
	return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetDisplayOutputDev
//  Description : 
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_GetDisplayOutputDev(AHC_DISPLAY_OUTPUTPANEL *pDevice)
{
	*pDevice = (AHC_DISPLAY_OUTPUTPANEL)m_ulOutputDevice;

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_SetDisplayWindow
//  Description :
//------------------------------------------------------------------------------
/**
 @brief Set display widnow information

 Configures the display window of the specific mode.
 Parameters:
 @param[in] bySysMode Indicates the mode to be set.
 @param[in] uwStartX The top-left corner's X of the display window.
 @param[in] The top-left corner' Y of the display window.
 @param[in] uwWidth The width of display window.
 @param[in] uwHeight The height of display window.
 @param[in] wDispFlag Supports the different kind of display ratio. This lets UI layout become more flexibility.
 @retval AHC_TRUE Success.
*/
AHC_BOOL AHC_SetDisplayWindow(UINT8 bySysMode, AHC_BOOL bUse, UINT8 byRotate, UINT16 uwStartX, UINT16 uwStartY, UINT16 uwWidth, UINT16 uwHeight, UINT16 wDispFlag)
{
    UINT32                  ret = AHC_TRUE;
    UINT16 	                bufWidth;
    UINT16 	                bufHeight;
    AHC_DISPLAY_OUTPUTPANEL OutputDevice;
    MMP_DISPLAY_ROTATE_TYPE PrevRotateDir;

    #if (UVC_HOST_VIDEO_ENABLE)
    if (DISPLAY_SYSMODE_VIDEOCAPTURE_UVC == bySysMode){
        //NOP
    }
    else
    #endif
    {
        m_usAHCDispStartX = uwStartX;
        m_usAHCDispStartY = uwStartY;
        m_usAHCDispWidth  = uwWidth;
        m_usAHCDispHeight = uwHeight;
    }

    AHC_GetDisplayOutputDev(&OutputDevice);

#if (HDMI_PREVIEW_EN)
    if (AHC_IsHdmiConnect())
    {
        PrevRotateDir = MMP_DISPLAY_ROTATE_NO_ROTATE;
        bufWidth        = uwWidth;
        bufHeight       = uwHeight;
    }
    else
#endif
#if (TVOUT_PREVIEW_EN)
    if (AHC_IsTVConnect())
    {
        PrevRotateDir = MMP_DISPLAY_ROTATE_NO_ROTATE;
        bufWidth        = uwWidth;
        bufHeight       = uwHeight;
    }
    else
#endif
    {
        #if (VERTICAL_LCD == VERTICAL_LCD_DEGREE_0)
        PrevRotateDir = MMP_DISPLAY_ROTATE_NO_ROTATE;
        bufWidth        = uwWidth;
        bufHeight       = uwHeight;
        #elif (VERTICAL_LCD == VERTICAL_LCD_DEGREE_90)
        PrevRotateDir   = MMP_DISPLAY_ROTATE_RIGHT_90;
        bufWidth        = uwHeight;
        bufHeight       = uwWidth;
        #elif (VERTICAL_LCD == VERTICAL_LCD_DEGREE_270)
        PrevRotateDir   = MMP_DISPLAY_ROTATE_RIGHT_270;
        bufWidth        = uwHeight;
        bufHeight       = uwWidth;
        #endif
    }
    
    if (OutputDevice == MMP_DISPLAY_SEL_MAIN_LCD || 
        OutputDevice == MMP_DISPLAY_SEL_SUB_LCD) {
        
        switch(bySysMode) {
            case DISPLAY_SYSMODE_STILLCAPTURE:
                if (CAM_CHECK_PRM(PRM_CAM_TV_DECODER)) {
                    ret = MMPS_DSC_CustomedPreviewAttr( bUse, byRotate, PrevRotateDir, 
                                                        MMP_SCAL_FITMODE_OPTIMAL, 
                                                        bufWidth, bufHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                }
                else {
                    ret = MMPS_DSC_CustomedPreviewAttr( bUse, byRotate, PrevRotateDir, 
                                                        MMP_SCAL_FITMODE_OUT, 
                                                        bufWidth, bufHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                }
                break;
            case DISPLAY_SYSMODE_STILLPLAYBACK:
                ret = MMPS_DSC_CustomedPlaybackAttr(bUse, byRotate, PrevRotateDir, 
                                                    MMP_SCAL_FITMODE_OPTIMAL, 
                                                    bufWidth, bufHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
            case DISPLAY_SYSMODE_VIDEOCAPTURE_PRM:
                // Fix Video mode support zoom by preview with 4:3 LCD will lock up issue.
                #if (VIDEO_DIGIT_ZOOM_EN)
                ret = MMPS_3GPRECD_CustomedPreviewAttr( gsAhcPrmSensor, bUse, byRotate, PrevRotateDir, 
                                                        MMP_SCAL_FITMODE_OUT, 
                                                        bufWidth, bufHeight, uwStartX, uwStartY, uwWidth, uwHeight);		
                #else
                ret = MMPS_3GPRECD_CustomedPreviewAttr( gsAhcPrmSensor, bUse, byRotate, PrevRotateDir, 
                                                        MMP_SCAL_FITMODE_OPTIMAL, 
                                                        bufWidth, bufHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                #endif
                break;
            case DISPLAY_SYSMODE_VIDEOCAPTURE_UVC:
                ret = MMPS_UVCRECD_SetCustomedPrevwAttr(bUse, byRotate, PrevRotateDir, 
                                                        MMP_SCAL_FITMODE_OPTIMAL, 
                                                        bufWidth, bufHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
            case DISPLAY_SYSMODE_VIDEOCAPTURE_SCD:
                ret = MMPS_3GPRECD_CustomedPreviewAttr( gsAhcScdSensor, bUse, byRotate, PrevRotateDir,
                                                        MMP_SCAL_FITMODE_OPTIMAL,
                                                        bufWidth, bufHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
            case DISPLAY_SYSMODE_VIDEOPLAYBACK:
                ret = MMPS_VIDPLAY_SetDisplayWindow(bUse, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
        }
    }
    else {
        switch(bySysMode) {
            case DISPLAY_SYSMODE_STILLCAPTURE:
                if (CAM_CHECK_PRM(PRM_CAM_TV_DECODER)) {
                    ret = MMPS_DSC_CustomedPreviewAttr( bUse, byRotate, MMP_DISPLAY_ROTATE_NO_ROTATE, 
                                                        MMP_SCAL_FITMODE_OPTIMAL, 
                                                        uwWidth, uwHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                }
                else {
                    ret = MMPS_DSC_CustomedPreviewAttr( bUse, byRotate, MMP_DISPLAY_ROTATE_NO_ROTATE, 
                                                        MMP_SCAL_FITMODE_OUT, 
                                                        uwWidth, uwHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                }
                break;
            case DISPLAY_SYSMODE_STILLPLAYBACK:
                ret = MMPS_DSC_CustomedPlaybackAttr(bUse, byRotate, MMP_DISPLAY_ROTATE_NO_ROTATE, 
                                                    MMP_SCAL_FITMODE_OPTIMAL, 
                                                    uwWidth, uwHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
            case DISPLAY_SYSMODE_VIDEOCAPTURE_PRM:
                ret = MMPS_3GPRECD_CustomedPreviewAttr( gsAhcPrmSensor, bUse, byRotate, MMP_DISPLAY_ROTATE_NO_ROTATE, 
                                                        MMP_SCAL_FITMODE_OPTIMAL, 
                                                        uwWidth, uwHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
            case DISPLAY_SYSMODE_VIDEOCAPTURE_UVC:
                ret = MMPS_UVCRECD_SetCustomedPrevwAttr( bUse, byRotate, MMP_DISPLAY_ROTATE_NO_ROTATE, 
                                                         MMP_SCAL_FITMODE_OPTIMAL, 
                                                         uwWidth, uwHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
            case DISPLAY_SYSMODE_VIDEOCAPTURE_SCD:
                ret = MMPS_3GPRECD_CustomedPreviewAttr( gsAhcScdSensor, bUse, byRotate, MMP_DISPLAY_ROTATE_NO_ROTATE, 
                                                        MMP_SCAL_FITMODE_OPTIMAL, 
                                                        uwWidth, uwHeight, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
            case DISPLAY_SYSMODE_VIDEOPLAYBACK:
                ret = MMPS_VIDPLAY_SetDisplayWindow(bUse, uwStartX, uwStartY, uwWidth, uwHeight);
                break;
        }
    }

    if (ret) {
        printc("bySysMode:%d, bUse:%d, byRotate:%d,wDispFlag:%d\r\n", bySysMode, bUse, byRotate, wDispFlag);            
        printc("uwStartX:%d, uwStartY:%d, uwWidth:%d, uwHeight:%d\r\n", uwStartX, uwStartY, uwWidth, uwHeight, wDispFlag);
        AHC_PRINT_RET_ERROR(gbAhcDbgBrk, ret);
        return AHC_FALSE;
    }
    
    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetDisplayWindowAttr
//  Description :
//------------------------------------------------------------------------------ 
AHC_BOOL AHC_GetDisplayWindowAttr(UINT16 *uwStartX, UINT16 *uwStartY, UINT16 *uwWidth, UINT16 *uwHeight)
{
    *uwStartX = m_usAHCDispStartX;
    *uwStartY = m_usAHCDispStartY;
    *uwWidth  = m_usAHCDispWidth;
    *uwHeight = m_usAHCDispHeight;

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_HDMISendEventEnable
//  Description :
//------------------------------------------------------------------------------
void AHC_HDMISendEventEnable(AHC_BOOL Enable)
{
    m_EnableSendHDMIEvent = Enable;
}

//------------------------------------------------------------------------------
//  Function    : AHC_CanSendHDMIEvent
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_CanSendHDMIEvent(void)
{
    return m_EnableSendHDMIEvent;
}

//------------------------------------------------------------------------------
//  Function    : AHC_IsHdmiConnect
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_IsHdmiConnect(void)
{
#if (HDMI_ENABLE)
    //Andy Liu. Note: detection function should not include system flow.
    return MMPF_HDMI_IsConnect();

#else  // HDMI_ENABLE
    return AHC_FALSE;
#endif
}

//------------------------------------------------------------------------------
//  Function    : AHC_IsTVConnect
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_IsTVConnect(void)
{
#ifdef TV_ONLY
    return AHC_TRUE;

#elif (TVOUT_ENABLE)
    //Andy Liu. Note: detection function should not include system flow.
    MMP_UBYTE tempValue;

#ifdef DEVICE_GPIO_TV
    if (DEVICE_GPIO_TV == MMP_GPIO_MAX) {
        return AHC_FALSE;
    }
    MMPF_PIO_EnableOutputMode(DEVICE_GPIO_TV, MMP_FALSE, MMP_TRUE);
    MMPF_PIO_GetData(DEVICE_GPIO_TV, &tempValue);
#else
    tempValue = !TV_CONNECTED_STATE;
#endif

    return (tempValue == TV_CONNECTED_STATE) ? (AHC_TRUE):(AHC_FALSE);

#else
    return AHC_FALSE;
#endif
}

//------------------------------------------------------------------------------
//  Function    : AHC_IsTVConnectEx
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_IsTVConnectEx(void)
{
#if (SWITCH_TVOUT_BY_KEY)
    extern AHC_BOOL	m_ubGoToTVmode;

    return (AHC_IsTVConnect() && m_ubGoToTVmode);
#elif (defined(SWITCH_TVOUT_BY_MENU) && SWITCH_TVOUT_BY_MENU == 1) //No TV Detect PIN
    extern AHC_BOOL	m_ubGoToTVmode;
    return m_ubGoToTVmode;
#else
    return AHC_IsTVConnect();
#endif
}

//------------------------------------------------------------------------------
//  Function    : AHC_SwitchDisplayOutputDevNone
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_SwitchDisplayOutputDevNone(void)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    AHC_DISPLAY_OUTPUTPANEL sCurDevice;

    //Get current output device.	
    AHC_GetDisplayOutputDev(&sCurDevice);    

    AHC_SetDisplayOutputDev(DISP_OUT_LCD, AHC_DISPLAY_DUPLICATE_1X); //TBD
    AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_NONE);
        
    //Output device has changed.        
    if((sCurDevice != AHC_DISPLAY_NONE) || (AIHC_GetOSDInitStatus() == AHC_FALSE)){
        if(AIHC_GetOSDInitStatus() == AHC_TRUE){
            AHC_OSDUninit();
        }
        
        //InitOSD(); //Should be InitOSDNone();
    }
    
    return ahcRet;
}

//------------------------------------------------------------------------------
//  Function    : AHC_SwitchDisplayOutputDevLCD
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_SwitchDisplayOutputDevLCD(void)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    AHC_DISPLAY_OUTPUTPANEL sCurDevice;

    //Get current output device.	
    AHC_GetDisplayOutputDev(&sCurDevice);    

    #if defined(CCIR656_OUTPUT_ENABLE)&&(CCIR656_OUTPUT_ENABLE)
    AHC_SetDisplayOutputDev(DISP_OUT_CCIR656, AHC_DISPLAY_DUPLICATE_1X);
    AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_CCIR);
    #else
    if ((sCurDevice != AHC_DISPLAY_MAIN_LCD) && (sCurDevice != AHC_DISPLAY_SUB_LCD)){
    	AHC_SetDisplayOutputDev(DISP_OUT_LCD, AHC_DISPLAY_DUPLICATE_1X);
    }

    if(sCurDevice == AHC_DISPLAY_MAIN_LCD){
        AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_MAIN_LCD);
    }
    else if(sCurDevice == AHC_DISPLAY_SUB_LCD){
        AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_SUB_LCD);
    }
    #endif
        
    //Output device has changed.        
    if(((sCurDevice != AHC_DISPLAY_MAIN_LCD) && (sCurDevice != AHC_DISPLAY_SUB_LCD)) || (AIHC_GetOSDInitStatus() == AHC_FALSE)){
        if(AIHC_GetOSDInitStatus() == AHC_TRUE){
            AHC_OSDUninit();
        }
        
        InitOSD_LCD();//InitOSD();
    }

    return ahcRet;
}

#if (HDMI_ENABLE)

AHC_BOOL AHC_CheckHDMIOutputChanged(void)
{
    AHC_DISPLAY_HDMIOUTPUTMODE 	HdmiMode;
	
    AHC_GetHDMIOutputMode(&HdmiMode);
    
#if MENU_GENERAL_HDMI_OUTPUT_1080P_EN                
    if((MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_1080P) && (HdmiMode != MMP_HDMI_OUTPUT_1920X1080P)){
        return AHC_TRUE;
    }    
#endif   
#if MENU_GENERAL_HDMI_OUTPUT_1080I_EN        
    if((MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_1080I) && (HdmiMode != MMP_HDMI_OUTPUT_1920X1080I)){
        return AHC_TRUE;
    }
#endif
#if MENU_GENERAL_HDMI_OUTPUT_720P_EN
    if((MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_720P) && (HdmiMode != MMP_HDMI_OUTPUT_1280X720P)){
        return AHC_TRUE;
    }
#endif
#if MENU_GENERAL_HDMI_OUTPUT_480P_EN
    if((MenuSettingConfig()->uiHDMIOutput == HDMI_OUTPUT_480P) && (HdmiMode != MMP_HDMI_OUTPUT_720X480P)){
        return AHC_TRUE;
    }
#endif

    return AHC_FALSE;    
}

//------------------------------------------------------------------------------
//  Function    : AHC_SwitchDisplayOutputDevHDMI
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_SwitchDisplayOutputDevHDMI(void)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    AHC_DISPLAY_OUTPUTPANEL sCurDevice;
    
    //Get current output device.	
    AHC_GetDisplayOutputDev(&sCurDevice);       

    //Output device has not changed.
    if((sCurDevice == AHC_DISPLAY_HDMI) && (AHC_CheckHDMIOutputChanged() == AHC_FALSE)){        
        printc("%s,%d HDMI output is not changed:%d\r\n", __func__, __LINE__, MenuSettingConfig()->uiHDMIOutput);
        //return ahcRet;
    }  
    else {
        switch (MenuSettingConfig()->uiHDMIOutput){
#if MENU_GENERAL_HDMI_OUTPUT_1080I_EN        
            case HDMI_OUTPUT_1080I:
            printc("@@@ HDMI_OUTPUT_1080I\r\n");
            AHC_SetHDMIOutputMode(AHC_DISPLAY_HDMIOUTPUT_1920X1080I);
            break;
#endif        
#if MENU_GENERAL_HDMI_OUTPUT_720P_EN
            case HDMI_OUTPUT_720P:
            printc("@@@ HDMI_OUTPUT_720P\r\n");
            AHC_SetHDMIOutputMode(AHC_DISPLAY_HDMIOUTPUT_1280X720P);
            break;
#endif
#if MENU_GENERAL_HDMI_OUTPUT_480P_EN
            case HDMI_OUTPUT_480P:
            printc("@@@ HDMI_OUTPUT_480P\r\n");
            AHC_SetHDMIOutputMode(AHC_DISPLAY_HDMIOUTPUT_720X480P);
            break;
#endif        
            default:
#if MENU_GENERAL_HDMI_OUTPUT_1080P_EN            
            case HDMI_OUTPUT_1080P:
#endif            
            printc("@@@ HDMI_OUTPUT_1080P\r\n");
            AHC_SetHDMIOutputMode(AHC_DISPLAY_HDMIOUTPUT_1920X1080P);
            break;
        }

        AHC_SetDisplayOutputDev(DISP_OUT_HDMI, AHC_DISPLAY_DUPLICATE_1X);
        AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_HDMI);
    }

	if(AHC_IsHdmiConnect()){ 
        AHC_OSDUninit(); 
    }
    if(AIHC_GetOSDInitStatus() == AHC_TRUE){
    }
    else {
        InitOSD_HDMI();
    }
    
    return ahcRet;
}
#endif

#if (TVOUT_ENABLE)
//------------------------------------------------------------------------------
//  Function    : AHC_SwitchDisplayOutputDevTVout
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_SwitchDisplayOutputDevTVout(void)
{
    AHC_BOOL ahcRet = AHC_TRUE;
    AHC_DISPLAY_OUTPUTPANEL sCurDevice;
    UINT8 ubTVSystem = MenuSettingConfig()->uiTVSystem;
    extern AHC_BOOL        gAHC_UpdateMenuDone;

    //Get current output device.	
    AHC_GetDisplayOutputDev(&sCurDevice);    

    if(AHC_FALSE == gAHC_UpdateMenuDone){
        AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0);
    }

    if(ubTVSystem == TV_SYSTEM_PAL){
        if(sCurDevice != AHC_DISPLAY_PAL_TV){
            AHC_SetDisplayOutputDev(DISP_OUT_TV_PAL, AHC_DISPLAY_DUPLICATE_1X);
        }
        AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_PAL_TV);
    }
    else{
        if(sCurDevice != AHC_DISPLAY_NTSC_TV){
            AHC_SetDisplayOutputDev(DISP_OUT_TV_NTSC, AHC_DISPLAY_DUPLICATE_1X);
        }
        AHC_SetCurrentDisplayEx(MMP_DISPLAY_SEL_NTSC_TV);
    }

    //Output device has changed.
    if(((sCurDevice != AHC_DISPLAY_NTSC_TV) && (sCurDevice != AHC_DISPLAY_PAL_TV)) || (AIHC_GetOSDInitStatus() == AHC_FALSE)){
        if(AIHC_GetOSDInitStatus() == AHC_TRUE){
            AHC_OSDUninit();
        }
        
        InitOSD_TVout();
    }

    return ahcRet;
}
#endif

#if 0
void _____WaitIcon_Functions_____(){}
#endif

//------------------------------------------------------------------------------
//  Function    : AHC_EnableWaitIcon
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_EnableWaitIcon(AHC_BOOL EnWaitIcon)
{
    if (EnWaitIcon) {
        m_WaitID = 0;
        m_WaitIcon = AHC_TRUE;
    }
    else {
        m_WaitIcon = AHC_FALSE;
    }

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetWaitIconState
//  Description : 
//------------------------------------------------------------------------------
AHC_BOOL AHC_GetWaitIconState(void)
{
    return m_WaitIcon;
}

//------------------------------------------------------------------------------
//  Function    : AHC_DrawWaitIcon
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_DrawWaitIcon(void)
{
    UINT8 ubID;
    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return AHC_TRUE; }

    OSDDraw_GetLastOvlDrawBuffer(&ubID);

	BEGIN_LAYER(ubID);

    AHC_OSDSetColor(ubID, OSD_COLOR_TRANSPARENT);

    AHC_OSDDrawFillRect(ubID, (320>>1)-40, (240>>1)-30, (320>>1)+30, (240>>1)+35);

    AHC_OSDSetColor(ubID, 0xAA000000);

    if (m_WaitIcon == AHC_TRUE) {
        switch(m_WaitID) {
        case 0:
            AHC_OSDDrawBitmap(ubID, &bmIcon_Wait0, (320>>1)-25, (240>>1)-30);
        break;
        case 1:
            AHC_OSDDrawBitmap(ubID, &bmIcon_Wait1, (320>>1)-25, (240>>1)-30);
        break;
        case 2:
            AHC_OSDDrawBitmap(ubID, &bmIcon_Wait2, (320>>1)-25, (240>>1)-30);
        break;
        case 3:
            AHC_OSDDrawBitmap(ubID, &bmIcon_Wait3, (320>>1)-25, (240>>1)-30);
        break;
        case 4:
            AHC_OSDDrawBitmap(ubID, &bmIcon_Wait4, (320>>1)-25, (240>>1)-30);
        break;
        }
        m_WaitID++;
        m_WaitID = (m_WaitID)%5;
    }

	END_LAYER(ubID);

    return AHC_TRUE;
}

#if 0
void _____ChargingIcon_Functions_____(){}
#endif

#if (OSD_SHOW_BATTERY_STATUS == 1)||(OSD_SHOW_BATLOW_WARNING == 1)

#include "IconPosition.h"
#include "AHC_Menu.h"
#include "MenuSetting.h"
#include "StateHDMIFunc.h"
#include "StateTVFunc.h"
#include "dsc_charger.h"
#include "MenuDrawCommon.h"
#include "MediaPlaybackCtrl.h"

static UINT8 	m_ChargingID 	= 0;
static AHC_BOOL m_ChargingIconEn = AHC_FALSE;

#if (CHARGE_FULL_NOTIFY)
extern AHC_BOOL	bChargeFull;
#endif

//------------------------------------------------------------------------------
//  Function    : AHC_SetChargingID
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_SetChargingID(UINT8 ubID)
{
	#if (BATTERY_ICON_WITH_DIFF_STATE)
    UINT8 bID0 = 0;
	if (m_ChargingID != ubID)
	{
		m_ChargingID = ubID;
		#if (BATTERY_ICON_PROC_DIFF_STATE_WITH_FUNC)
		AHC_ProcessBatteryIconWithDiffState();
		#else
		AHC_DrawBatteryIcon(bID0, ubID, AHC_TRUE);
		#endif
	}
	else 
	#endif
	{
		m_ChargingID = ubID;
	}
	return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_ResetChargingID
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_ResetChargingID(void)
{
	AHC_SetChargingID(0);
	return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetChargingID
//  Description :
//------------------------------------------------------------------------------
UINT8 AHC_GetChargingID(void)
{
	return m_ChargingID;
}

//------------------------------------------------------------------------------
//  Function    : AHC_EnableChargingIcon
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_EnableChargingIcon(AHC_BOOL EnChargingIcon)
{
    if (EnChargingIcon) {
        m_ChargingIconEn = AHC_TRUE;
    }
    else {
        m_ChargingIconEn = AHC_FALSE;
    }

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_GetChargingIconState
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_GetChargingIconState(void)
{
    return m_ChargingIconEn;
}

#if 0
//------------------------------------------------------------------------------
//  Function    : AHC_DrawBatteryIcon_HDMI
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_DrawBatteryIcon_HDMI(UINT8 ubID, UINT8 ubIconID, AHC_BOOL bFlag)
{
    GUI_BITMAP 	IconID 		= BMICON_BATTERY_FULL;
    GUI_COLOR	bkClr		= OSD_COLOR_TRANSPARENT;
    UINT8    	ubCurState 	= uiGetCurrentState();
    UINT16  	x, y;
    GUI_COLOR	oldclr, oldbk;

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return AHC_TRUE; }

#ifdef BATTERY_ICON_HDMI_SPEC_CONDITION
	if ((ubCurState != UI_HDMI_STATE) && (!MMPF_HDMI_IsConnect()))
#else
	if (ubCurState != UI_HDMI_STATE)
#endif
		return AHC_TRUE;

	if ((ubIconID >= BATTERY_VOLTAGE_NUM) && (ubIconID != 0xFF) && (ubIconID != 0xFE))
		return AHC_FALSE;
	
    if (HDMIFunc_IsInMenu())
	{
	    switch(MenuSettingConfig()->uiHDMIOutput)
	    {
	    case HDMI_OUTPUT_1080I:
	        x = POS_ICON_HDMI_1080P_BATT_X;
	        y = POS_ICON_HDMI_1080P_BATT_Y;
	    break;
	    case HDMI_OUTPUT_720P:
	        x = POS_ICON_HDMI_720P_BATT_X;
	        y = POS_ICON_HDMI_720P_BATT_X;
	    break;
	    case HDMI_OUTPUT_480P:      // TBD
	        x = POS_HDMI_480P_BRO_BATT_X;
	        y = POS_HDMI_480P_BRO_BATT_Y;
	    break;
	    default:
	    case HDMI_OUTPUT_1080P:     // TBD
	        x = POS_ICON_HDMI_1080P_BATT_X;
	        y = POS_ICON_HDMI_1080P_BATT_Y;
	    break;
	    }
		bkClr 	= OSD_COLOR_DARKGRAY2;
		goto L_Draw;
	}

    if (HDMIFunc_GetStatus()==AHC_HDMI_BROWSER_STATUS)
	{
	    switch(MenuSettingConfig()->uiHDMIOutput)
	    {
	    case HDMI_OUTPUT_1080I:
	        x = POS_HDMI_1080I_BRO_BATT_X;
	        y = POS_HDMI_1080I_BRO_BATT_Y;
	    break;
	    case HDMI_OUTPUT_720P:
	        x = POS_HDMI_720P_BRO_BATT_X;
	        y = POS_HDMI_720P_BRO_BATT_Y;
	    break;
	    case HDMI_OUTPUT_480P:      // TBD
	        x = POS_HDMI_480P_BRO_BATT_X;
	        y = POS_HDMI_480P_BRO_BATT_Y;
	    break;
	    default:
	    case HDMI_OUTPUT_1080P:     // TBD
	        x = POS_HDMI_1080P_BRO_BATT_X;
	        y = POS_HDMI_1080P_BRO_BATT_Y;
	    break;
	    }

	    bkClr 	= OSD_COLOR_TITLE;
    }
    else if (HDMIFunc_GetStatus()==AHC_HDMI_VIDEO_PREVIEW_STATUS ||
    		 HDMIFunc_GetStatus()==AHC_HDMI_DSC_PREVIEW_STATUS)
    {
	    switch(MenuSettingConfig()->uiHDMIOutput)
	    {
	    case HDMI_OUTPUT_1080I:
	        x = POS_ICON_HDMI_1080P_BATT_X;
	        y = POS_ICON_HDMI_1080P_BATT_Y;
	    break;
	    case HDMI_OUTPUT_720P:
	        x = POS_ICON_HDMI_720P_BATT_X;
	        y = POS_ICON_HDMI_720P_BATT_X;
	    break;
	    case HDMI_OUTPUT_480P:      // TBD
	        x = POS_HDMI_480P_BRO_BATT_X;
	        y = POS_HDMI_480P_BRO_BATT_Y;
	    break;
	    default:
	    case HDMI_OUTPUT_1080P:     // TBD
	        x = POS_ICON_HDMI_1080P_BATT_X;
	        y = POS_ICON_HDMI_1080P_BATT_Y;
	    break;
	    }
    	bkClr 	= OSD_COLOR_TRANSPARENT;
    }

L_Draw:

	BEGIN_LAYER(ubID);

	/* the procedure may interrupt some function that is drawing */
	/* backup color to protect interrupted drawing routine */
	oldclr = AHC_OSDGetColor(ubID);
	oldbk  = AHC_OSDGetBkColor(ubID);

    if (AHC_FALSE == bFlag)
    {
        AHC_OSDSetColor(ubID, bkClr);
		AHC_OSDDrawFillRect(ubID, x, y, x+IconID.XSize, y+IconID.YSize);
    }
    else
    {
#ifdef BATTERY_ICON_MAPPING
        switch (Mapping_BatteryIndex(AHC_GetChargingID()))
#else
        switch (ubIconID)
#endif
        {
            case 0:
				 IconID = BMICON_BATTERY_EMPTY;
            break;
            case 1:
                 IconID = BMICON_BATTERY_NORMAL;
            break;
            case 2:
                 IconID = BMICON_BATTERY_MID;
            break;
            case 3:
                 IconID = BMICON_BATTERY_FULL;
            break;
#ifdef BATTERY_ICON_CASE_ADDING
            case 4:
                 IconID = BMICON_BATTERY_EXHAUST;
            break;
            case 5:
                 IconID = BMICON_BATTERY_CHARGING;
            break;
            case 6:
                 IconID = BMICON_BATTERY_CHARGE_DONE;
            break;
#endif
        }

#ifdef BATTERY_ICON_CLEAR_BEFORE_DRAWING
		if ((ubIconID != 0xFF)) 
		{
			// Disable transparent mode
			AHC_OSDSetColor(ubID, OSD_COLOR_TRANSPARENT);
			
			/* Clear Icon before drawing */
	        if (HDMIFunc_IsInMenu())
			{
				AHC_OSDDrawFillRect(ubID, x, y, x+IconID.XSize-1, y+IconID.YSize-1);
			}
			else
			{
	        	switch(MenuSettingConfig()->uiHDMIOutput)
	        	{
	        	case HDMI_OUTPUT_1080I:
					AHC_OSDDrawFillRect(ubID, x, y, x+IconID.XSize*4, y+IconID.YSize*4);
	            	//AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 4, 4);
	        	break;
	        	case HDMI_OUTPUT_720P:
					AHC_OSDDrawFillRect(ubID, x, y, x+IconID.XSize*3, y+IconID.YSize*3);
	            	//AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 3, 3);
	        	break;
	        	case HDMI_OUTPUT_480P:      // TBD
					AHC_OSDDrawFillRect(ubID, x, y, x+IconID.XSize*2, y+IconID.YSize*2);
	            	//AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 2, 2);
	        	break;
	    	    default:
	    	    case HDMI_OUTPUT_1080P:     // TBD
					AHC_OSDDrawFillRect(ubID, x, y, x+IconID.XSize*4, y+IconID.YSize*4);
	            	//AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 4, 4);
	    	    break;
	        	}
	        }
	        
			if (ubIconID != 0xFF)
				rcIconID = ubIconID;
			else
				rcIconID = BATTERY_VOLTAGE_DEFAULT;
		}
		//BMICON_BATTERY_CHARGE_DONE
		if (ubIconID != 0xFF){
			AHC_OSDSetColor(ubID, OSD_COLOR_BLACK);
			AHC_OSDSetBkColor(ubID, OSD_COLOR_BLACK);
#endif
        if (HDMIFunc_IsInMenu())
		{
			AHC_OSDDrawBitmap(ubID, &IconID, x, y);
		}
		else
		{
        	switch(MenuSettingConfig()->uiHDMIOutput)
        	{
        	case HDMI_OUTPUT_1080I:
    			#if (OSD_PREVIEW_SCALE_HDMI)
    	        if (HDMIFunc_GetStatus() == AHC_HDMI_BROWSER_STATUS) {
                	AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 4, 4);
    	        }
    			else {
    				AHC_OSDDrawBitmap(ubID, &IconID, x, y);
    			}
			    #else
            	AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 4, 4);
			    #endif
        	break;
        	case HDMI_OUTPUT_720P:
    			#if (OSD_PREVIEW_SCALE_HDMI)
    	        if (HDMIFunc_GetStatus() == AHC_HDMI_BROWSER_STATUS) {
                	AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 3, 3);
    	        }
    			else {
    				AHC_OSDDrawBitmap(ubID, &IconID, x, y);
    			}
    			#else
            	AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 3, 3);
			    #endif
        	break;
        	case HDMI_OUTPUT_480P:      // TBD
            	AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 2, 2);
        	break;
    	    default:
    	    case HDMI_OUTPUT_1080P:     // TBD
    			#if OSD_PREVIEW_SCALE_HDMI
    	        if (HDMIFunc_GetStatus() == AHC_HDMI_BROWSER_STATUS) {
                	AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 4, 4);
    	        }
    			else {
    				AHC_OSDDrawBitmap(ubID, &IconID, x, y);
    			}
    			#else
            	AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 4, 4);
			    #endif
    	    break;
        	}
        }
		
#ifdef BATTERY_ICON_CLEAR_BEFORE_DRAWING
		}
#endif
    }

    /* restore colors to make interrupted routine keep right color */
    AHC_OSDSetColor(ubID, oldclr);
    AHC_OSDSetBkColor(ubID, oldbk);

	END_LAYER(ubID);

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_DrawBatteryIcon_TV
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_DrawBatteryIcon_TV(UINT8 ubID, UINT8 ubIconID, AHC_BOOL bFlag)
{
    GUI_BITMAP 	IconID 		= BMICON_BATTERY_FULL;
    GUI_COLOR	bkClr		= OSD_COLOR_TRANSPARENT;
    UINT8    	ubCurState 	= uiGetCurrentState();
    UINT16  	x, y;
    GUI_COLOR	oldclr, oldbk;

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return AHC_TRUE; }

    if (ubCurState!=UI_TVOUT_STATE)
        return AHC_TRUE;

	if (TVFunc_IsInMenu())
	{
		x 		= POS_ICON_BATT_X;
		y 		= POS_ICON_BATT_Y;
		bkClr 	= OSD_COLOR_DARKGRAY2;
		goto L_Draw;
	}

    if (TVFunc_Status()==AHC_TV_BROWSER_STATUS)
    {
        UINT8 ctv_sys = 0;
        
        if ((AHC_Menu_SettingGetCB((char*)COMMON_KEY_TV_SYSTEM, &ctv_sys) == AHC_FALSE)) {
            ctv_sys = COMMON_TV_SYSTEM_NTSC; //default value
        }

        switch(ctv_sys)
        {
            case COMMON_TV_SYSTEM_NTSC:
                x = POS_TV_NTSC_BRO_BATT_X;
                y = POS_TV_NTSC_BRO_BATT_Y;
                break;
            case COMMON_TV_SYSTEM_PAL:
                x = POS_TV_PAL_BRO_BATT_X;
                y = POS_TV_PAL_BRO_BATT_Y;
                break;
        }
        bkClr 	= OSD_COLOR_TITLE;
    }
    else if (TVFunc_Status()==AHC_TV_VIDEO_PREVIEW_STATUS ||
    		 TVFunc_Status()==AHC_TV_DSC_PREVIEW_STATUS)
    {
    	x 		= POS_ICON_TV_BATT_X;
    	y 		= POS_ICON_TV_BATT_Y;
    	bkClr 	= OSD_COLOR_TRANSPARENT;
    }

L_Draw:

	BEGIN_LAYER(ubID);

	/* the procedure may interrupt some function that is drawing */
	/* backup color to protect interrupted drawing routine */
	oldclr = AHC_OSDGetColor(ubID);
	oldbk  = AHC_OSDGetBkColor(ubID);

    if (AHC_FALSE == bFlag)
    {
        AHC_OSDSetColor(ubID, bkClr);
		AHC_OSDDrawFillRect(ubID, x, y, x+IconID.XSize, y+IconID.YSize);
    }
    else
    {
#ifdef BATTERY_ICON_MAPPING
        switch (Mapping_BatteryIndex(AHC_GetChargingID()))
#else
        switch (ubIconID)
#endif
        {
            case 0:
				 IconID = BMICON_BATTERY_EMPTY;
            break;
            case 1:
                 IconID = BMICON_BATTERY_NORMAL;
            break;
            case 2:
                 IconID = BMICON_BATTERY_MID;
            break;
            case 3:
                 IconID = BMICON_BATTERY_FULL;
            break;
#ifdef BATTERY_ICON_CASE_ADDING
            case 4:
                 IconID = BMICON_BATTERY_EXHAUST;
            break;
            case 5:
                 IconID = BMICON_BATTERY_CHARGING;
            break;
            case 6:
                 IconID = BMICON_BATTERY_CHARGE_DONE;
            break;
#endif
        }

		if (TVFunc_IsInMenu())
		{
			AHC_OSDDrawBitmap(ubID, &IconID, x, y);
		}
		else
		{
			AHC_OSDDrawBitmapMag(ubID, &IconID, x, y, 2, 2);
		}
    }

    /* restore colors to make interrupted routine keep right color */
    AHC_OSDSetColor(ubID, oldclr);
    AHC_OSDSetBkColor(ubID, oldbk);

	END_LAYER(ubID);

    return AHC_TRUE;
}
#endif
//------------------------------------------------------------------------------
//  Function    : AHC_DrawBatteryIcon
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_DrawBatteryIcon(UINT8 ubID, UINT8 ubIconID, AHC_BOOL bFlag)
{
    GUI_BITMAP 	IconID 		= BMICON_BATTERY_FULL;
    RECT 		btyRECT 	= POS_STATE_BATTERY;
//    UINT8    	ubCurState 	= uiGetCurrentState();
    GUI_COLOR	oldclr, oldbk;
	UINT 		bLarge = NORMAL_SIZE;

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return AHC_TRUE; }

    if (((rcIconID == 0xFF) && (ubIconID == rcIconID) && (uiGetCurrentState() == UI_MSDC_STATE))
        || ((rcIconID == 0xFF) && (ubIconID == 0xFE)  && (uiGetCurrentState() == UI_MSDC_STATE)))
        return AHC_TRUE;

	if (AHC_IsHdmiConnect())
		return AHC_TRUE;

	if(AHC_IsTVConnectEx())
	{
		btyRECT.uiLeft	= 600;
		btyRECT.uiTop	= 22;
		bLarge = LARGE_SIZE;
	}

	BEGIN_LAYER(ubID);

	/* the procedure may interrupt some function that is drawing */
	/* backup color to protect interrupted drawing routine */
	oldclr = AHC_OSDGetColor(ubID);
	oldbk  = AHC_OSDGetBkColor(ubID);

    if (AHC_FALSE == bFlag)
    {
        AHC_OSDSetColor(ubID, 0xFF000000);
    }
    else
    {
		#ifdef BATTERY_ICON_MAPPING
        switch (Mapping_BatteryIndex(AHC_GetChargingID()))
		#else
        switch (ubIconID)
		#endif
        {
            case 0:
				 IconID = BMICON_BATTERY_EMPTY;
            break;
            case 1:
                 IconID = BMICON_BATTERY_NORMAL;
            break;
            case 2:
                 IconID = BMICON_BATTERY_MID;
            break;
            case 3:
                 IconID = BMICON_BATTERY_FULL;
            break;
			#ifdef BATTERY_ICON_CASE_ADDING
            case 4:
                 IconID = BMICON_BATTERY_EXHAUST;
            break;
            case 5:
                 IconID = BMICON_BATTERY_CHARGING;
            break;
            case 6:
                 IconID = BMICON_BATTERY_CHARGE_DONE;
            break;
			#endif
        }

	#ifdef BATTERY_ICON_CLEAR_BEFORE_DRAWING
        // Disable transparent mode
        AHC_OSDSetColor(ubID, OSD_COLOR_TRANSPARENT);

		if (uiGetCurrentState() == UI_BROWSER_STATE)
		{
            AHC_OSDSetColor(ubID, OSD_COLOR_BLACK);
        }
        
        /* Clear Icon before drawing */
        AHC_OSDDrawFillRect( ubID, btyRECT.uiLeft, btyRECT.uiTop, btyRECT.uiLeft+IconID.XSize, btyRECT.uiTop+IconID.YSize-4);

		rcIconID = 0xFF;

        if (ubIconID != 0xFF){  
			rcIconID = ubIconID;
            AHC_OSDSetColor(ubID, OSD_COLOR_BLACK);
            AHC_OSDDrawBitmap(ubID, &IconID, btyRECT.uiLeft, btyRECT.uiTop);
        }
	#else
        // Disable transparent mode
        AHC_OSDSetColor(ubID, OSD_COLOR_BLACK);
        AHC_OSDSetBkColor(ubID, OSD_COLOR_TRANSPARENT);
		if(bLarge==LARGE_SIZE)
			AHC_OSDDrawBitmapMag(ubID, &IconID, btyRECT.uiLeft, btyRECT.uiTop, MAG_2X, MAG_2X);
		else  
        	AHC_OSDDrawBitmap(ubID, &IconID, btyRECT.uiLeft, btyRECT.uiTop);
	#endif 

    }
    /* restore colors to make interrupted routine keep right color */
	AHC_OSDSetColor(ubID, oldclr);
    AHC_OSDSetBkColor(ubID, oldbk);

	END_LAYER(ubID);

    return AHC_TRUE;
}

AHC_BOOL AHC_DrawBatteryLowIcon_UsingTimer(UINT8 bID0)
{
	if (m_ChargingID >= BATTERY_VOLTAGE_NUM)
		return AHC_TRUE;
	
	if (uiGetCurrentState() == UI_MSDC_STATE)
    	AHC_DrawBatteryIcon(bID0, 0xFF, AHC_TRUE);
	else
    	AHC_DrawBatteryIcon(bID0, m_ChargingID, AHC_TRUE);

	return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_DrawChargingIcon
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_DrawChargingIcon(void)
{
    UINT8 		bID0 = 0 ,bID1 = 0;
    UINT8		ubCurState;
    AHC_BOOL	bUseDoubleOSDBuf 	= AHC_FALSE;
    AHC_BOOL	bMenuMode 			= AHC_FALSE;

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return AHC_TRUE; }

#if (CHARGE_FULL_NOTIFY)
    if (bChargeFull == AHC_TRUE)
        return AHC_TRUE;
#endif

    ubCurState = uiGetCurrentState();

    switch(ubCurState)
    {
        case UI_CAMERA_STATE:
            // using AHC_DrawChargingIcon_UsingTimer
            return AHC_FALSE;
    	break;

    	case UI_VIDEO_STATE:
            // using AHC_DrawChargingIcon_UsingTimer
            return AHC_FALSE;
    	break;

    	case UI_BROWSER_STATE:
    		bUseDoubleOSDBuf = AHC_TRUE;
    	break;

    	case UI_CAMERA_MENU_STATE:
    	case UI_VIDEO_MENU_STATE:
    	case UI_PLAYBACK_MENU_STATE:
    	case UI_CLOCK_SETTING_STATE:
    		bMenuMode = AHC_TRUE;
    		bUseDoubleOSDBuf = AHC_TRUE;
    	break;

    	case UI_PLAYBACK_STATE:
    		if (GetCurrentOpMode()==JPGPB_MODE)
    			bUseDoubleOSDBuf = AHC_TRUE;
    		else if (GetCurrentOpMode()==MOVPB_MODE)
    			return AHC_FALSE;//bUseDoubleOSDBuf = AHC_FALSE;
    		else if (GetCurrentOpMode()==AUDPB_MODE)
    			bUseDoubleOSDBuf = AHC_TRUE;
			else if (GetCurrentOpMode()==JPGPB_MOVPB_MODE)
    			bUseDoubleOSDBuf = AHC_TRUE;
    	break;
    	case UI_MSDC_STATE:
    	case UI_PCCAM_STATE:
			bUseDoubleOSDBuf = AHC_TRUE;
    	break;

    	default:
    		return AHC_TRUE;
    	break;
    }

    if (bUseDoubleOSDBuf == AHC_TRUE)
    {
    	if(bMenuMode)
    		OSDDraw_EnterMenuDrawing(&bID0, &bID1);
    	else
    		OSDDraw_EnterDrawing(&bID0, &bID1);
    }
    else
    {
    	OSDDraw_GetLastOvlDrawBuffer(&bID0);
    }

    AHC_DrawChargingIcon_UsingTimer(bID0);

	if (bUseDoubleOSDBuf == AHC_TRUE)
	{
    	BEGIN_LAYER(bID0);
    	BEGIN_LAYER(bID1);

    	if (bMenuMode)
    		OSDDraw_ExitMenuDrawing(&bID0, &bID1);
    	else
    		OSDDraw_ExitDrawing(&bID0, &bID1);

    	END_LAYER(bID0);
    	END_LAYER(bID1);
    }

    return AHC_TRUE;
}

AHC_BOOL AHC_DrawPowerLowIcon(void)
{
    UINT8 		bID0 = 0 ,bID1 = 0;
    UINT8		ubCurState;
    AHC_BOOL	bUseDoubleOSDBuf 	= AHC_FALSE;
    AHC_BOOL	bMenuMode 			= AHC_FALSE;

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return AHC_TRUE; }

    ubCurState = uiGetCurrentState();

    switch(ubCurState)
    {
        case UI_CAMERA_STATE:
            // using AHC_DrawChargingIcon_UsingTimer
            return AHC_FALSE;
    	break;

    	case UI_VIDEO_STATE:
            // using AHC_DrawChargingIcon_UsingTimer
            return AHC_FALSE;
    	break;

    	case UI_BROWSER_STATE:
    		bUseDoubleOSDBuf = AHC_TRUE;
    	break;

    	case UI_CAMERA_MENU_STATE:
    	case UI_VIDEO_MENU_STATE:
    	case UI_PLAYBACK_MENU_STATE:
    	case UI_CLOCK_SETTING_STATE:
    		bMenuMode = AHC_TRUE;
    		bUseDoubleOSDBuf = AHC_TRUE;
    	break;

    	case UI_PLAYBACK_STATE:
    		if (GetCurrentOpMode()==JPGPB_MODE)
    			bUseDoubleOSDBuf = AHC_TRUE;
    		else if (GetCurrentOpMode()==MOVPB_MODE)
    			return AHC_FALSE;//bUseDoubleOSDBuf = AHC_FALSE;
    		else if (GetCurrentOpMode()==AUDPB_MODE)
    			bUseDoubleOSDBuf = AHC_TRUE;
			else if (GetCurrentOpMode()==JPGPB_MOVPB_MODE)
    			bUseDoubleOSDBuf = AHC_TRUE;
    	break;
    	case UI_MSDC_STATE:
    	case UI_PCCAM_STATE:
			bUseDoubleOSDBuf = AHC_TRUE;
    	break;

    	default:
    		return AHC_TRUE;
    	break;
    }

    if (bUseDoubleOSDBuf == AHC_TRUE)
    {
    	if(bMenuMode)
    		OSDDraw_EnterMenuDrawing(&bID0, &bID1);
    	else
    		OSDDraw_EnterDrawing(&bID0, &bID1);
    }
    else
    {
    	OSDDraw_GetLastOvlDrawBuffer(&bID0);
    }

    AHC_DrawBatteryLowIcon_UsingTimer(bID0);

	if (bUseDoubleOSDBuf == AHC_TRUE)
	{
    	BEGIN_LAYER(bID0);
    	BEGIN_LAYER(bID1);

    	if (bMenuMode)
    		OSDDraw_ExitMenuDrawing(&bID0, &bID1);
    	else
    		OSDDraw_ExitDrawing(&bID0, &bID1);

    	END_LAYER(bID0);
    	END_LAYER(bID1);
    }

    return AHC_TRUE;
}

#define DRAW_CHARGING_ICON_PERIOD       (300)       // ms

//------------------------------------------------------------------------------
//  Function    : AHC_DrawChargingIcon_UsingTimer
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_DrawChargingIcon_UsingTimer(UINT8 bID0)
{
    static UINT32 preTime = 0;
    UINT32      curTime;

#ifdef BATTERY_ICON_CHARGING_WITHOUT_TIMER
	if (m_ChargingID >= BATTERY_VOLTAGE_NUM)
		return AHC_TRUE;
	
	if (uiGetCurrentState() == UI_MSDC_STATE)
    	AHC_DrawBatteryIcon(bID0, 0xFF, AHC_TRUE);
	else
    	AHC_DrawBatteryIcon(bID0, m_ChargingID, AHC_TRUE);

	return AHC_TRUE;
#endif

	if (AHC_TRUE != AHC_Charger_GetStatus()) {
		preTime = 0;
		return AHC_FALSE;
	}

	#if (CHARGE_FULL_NOTIFY)
	if(bChargeFull == AHC_TRUE)
		return AHC_FALSE;
	#endif

	if (AHC_FALSE == AHC_GetChargingIconState())
    	return AHC_FALSE;

    AHC_OS_GetTime(&curTime);
    
    if (curTime >= preTime) {
        curTime -= preTime;
    }
    else {
        curTime += (UINT32) -1 - preTime + 1;
    }

    curTime = TICK_TO_MS(curTime);

    if (curTime < DRAW_CHARGING_ICON_PERIOD) {
        return AHC_FALSE;
    }

    AHC_OS_GetTime(&preTime);
    //AHC_DrawBatteryIcon(bID0, m_ChargingID, AHC_FALSE);
    AHC_DrawBatteryIcon(bID0, m_ChargingID, AHC_TRUE);

    m_ChargingID++;
    m_ChargingID = (m_ChargingID) % 4;

    return AHC_TRUE;
}

#if 0
//------------------------------------------------------------------------------
//  Function    : AHC_ClearAllChargingIcon_HDMI
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_ClearAllChargingIcon_HDMI(void)
{
	UINT8 bID0, bID1;

    OSDDraw_EnterDrawing(&bID0, &bID1);
    AHC_DrawBatteryIcon_HDMI(bID0, 0, AHC_FALSE);
    OSDDraw_ExitDrawing(&bID0, &bID1);

    return AHC_TRUE;
}

//------------------------------------------------------------------------------
//  Function    : AHC_ClearAllChargingIcon_TV
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_ClearAllChargingIcon_TV(void)
{
	UINT8 bID0, bID1;

    OSDDraw_EnterDrawing(&bID0, &bID1);
    AHC_DrawBatteryIcon_TV(bID0, 0, AHC_FALSE);
    OSDDraw_ExitDrawing(&bID0, &bID1);

    return AHC_TRUE;
}
#endif

//------------------------------------------------------------------------------
//  Function    : AHC_ClearAllChargingIcon
//  Description :
//------------------------------------------------------------------------------
AHC_BOOL AHC_ClearAllChargingIcon(void)
{
    GUI_BITMAP 	IconID 		= BMICON_BATTERY_FULL;
    RECT 		btyRECT 	= POS_STATE_BATTERY;
    UINT8    	ubCurState 	= uiGetCurrentState();

    if (AHC_FALSE == AIHC_GetOSDInitStatus()) { AHC_PRINT_RET_ERROR(gbAhcDbgBrk,0); return AHC_TRUE; }
#if 0
    if (ubCurState==UI_HDMI_STATE){
        AHC_ClearAllChargingIcon_HDMI();
        return AHC_TRUE;
    }
    else if (ubCurState==UI_TVOUT_STATE){
        AHC_ClearAllChargingIcon_TV();
        return AHC_TRUE;
    }
#endif
	BEGIN_LAYER(OVL_DISPLAY_BUFFER);

    if (ubCurState == UI_CAMERA_MENU_STATE 	||
        ubCurState == UI_VIDEO_MENU_STATE  	||
        ubCurState == UI_PLAYBACK_MENU_STATE ||
        ubCurState == UI_CLOCK_SETTING_STATE ||
        ubCurState == UI_BROWSER_STATE		||
        ubCurState == UI_MSDC_STATE ||
        ubCurState == UI_PCCAM_STATE )
    {
    	AHC_OSDSetColor(OVL_DISPLAY_BUFFER, OSD_COLOR_TITLE);
    }
    else {
        AHC_OSDSetColor(OVL_DISPLAY_BUFFER, OSD_COLOR_TRANSPARENT);
	}

    AHC_OSDDrawFillRect( OVL_DISPLAY_BUFFER, btyRECT.uiLeft, btyRECT.uiTop, btyRECT.uiLeft+IconID.XSize, btyRECT.uiTop+IconID.YSize);

    AHC_OSDDuplicateBuffer( OVL_DISPLAY_BUFFER, OVL_BUFFER_INDEX );
    AHC_OSDDuplicateBuffer( OVL_DISPLAY_BUFFER, OVL_BUFFER_INDEX1 );
	END_LAYER(OVL_DISPLAY_BUFFER);

    return AHC_TRUE;
}
#endif	

void AHC_LCD_SetBrightness(UINT8 brightness)
{
    RTNA_LCD_AdjustBrightness(brightness);
}

void AHC_LCD_SetContrast(UINT8 Contrast)
{
    RTNA_LCD_AdjustContrast(Contrast);
}

void AHC_LCD_SetBrightness_R(UINT8 brightness)
{
    RTNA_LCD_AdjustBrightness_R(brightness);
}

void AHC_LCD_SetContrast_R(UINT8 Contrast)
{
    RTNA_LCD_AdjustContrast_R(Contrast);
}

void AHC_LCD_SetBrightness_B(UINT8 brightness)
{
    RTNA_LCD_AdjustBrightness_B(brightness);
}

void AHC_LCD_SetContrast_B(UINT8 Contrast)
{
    RTNA_LCD_AdjustContrast_B(Contrast);
}

void AHC_LCD_Direction(AHC_LCD_DIRECTION dir)
{
    RTNA_LCD_Direction(dir);
}

void AHC_LCD_Write_Reg(UINT32 reg, UINT8 value)
{
    RTNA_LCD_SetReg(reg,value);
}

AHC_BOOL AHC_LCD_GetRotateConfig(AHC_BOOL *bWithHallSnr, AHC_BOOL *bSnrFlipEn, AHC_BOOL *bOSDFlipEn)
{
    *bWithHallSnr   = (LCD_ROTATE_WITH_HALL_SENSOR);
    *bSnrFlipEn     = SENSOR_FLIP_ENABLE;
    *bOSDFlipEn     = PANEL_OSD_FILP_ENABLE;
    return AHC_TRUE;
}

void AHC_LCD_GetStatus(UINT8 *status)
{
    UINT32 LCDstatus;

    AHC_GetParam(PARAM_ID_LCD_STATUS, &LCDstatus);
    *status = LCDstatus;
}



/// @}
