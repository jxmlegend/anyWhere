//==============================================================================
//
//  File        : lcd_ILI8961.c
//  Description : 320x240 LCD Panel Control Function
//  Author      :
//  Revision    : 1.0
//
//==============================================================================

//==============================================================================
//
//                              INCLUDE FILE
//
//==============================================================================

#include "Customer_config.h"
#include "lib_retina.h"
#include "lcd_common.h"
#include "mmpf_pio.h"
#include "mmp_gpio_inc.h"
//==============================================================================
//
//                              MACRO DEFINE
//
//==============================================================================

#define Delayms(t) 					LCD_WAIT(t)

#define	DELTA_RGB 					(0)
#define	STRIP_RGB 					(1) // TBD??
#define	LCD_RGB_MODE 				(DELTA_RGB)

#define SECONDARY_DISPLAY 			(0)

#define LCD_ILI8961_WIDTH           (320)
#define LCD_ILI8961_HEIGHT          (240)

static MMPF_PANEL_ATTR m_PanelAttr = 
{
	// Panel basic setting
	LCD_ILI8961_WIDTH, LCD_ILI8961_HEIGHT,
	LCD_TYPE_RGBLCD,
	LCD_PRM_CONTROLER,
	0,

#if ((16 == LCD_MODEL_RATIO_X) && (9 == LCD_MODEL_RATIO_Y))
	LCD_RATIO_16_9,
#else
	LCD_RATIO_4_3,
#endif

	// Panel initial sequence
	NULL,

	// Index/Cmd sequence
	NULL,

	// MCU interface
	LCD_BUS_WIDTH_8,
	LCD_PHASE0,
	LCD_POLARITY0,
	LCD_MCU_80SYS,
	0,
	0,
	0,
	LCD_RGB_ORDER_RGB,

	// RGB interface
	MMP_FALSE,
    LCD_SIG_POLARITY_H,     // DCLK Polarity
	LCD_SIG_POLARITY_L,     // H-Sync Polarity
	LCD_SIG_POLARITY_L,     // V-Sync Polarity
	RGB_D24BIT_RGB888,

	{0}
};

#define ENABLE_LCD_ILI8961_LOG 		(0)
#define ENABLE_LCD_ILI8961_SW_3W_SERIAL_CMD 1

#if (ENABLE_LCD_ILI8961_SW_3W_SERIAL_CMD == 1)
#define SPENB_GPIO_PIN  MMP_GPIO124
#define SPDA_GPIO_PIN   MMP_GPIO123
#define SPCK_GPIO_PIN   MMP_GPIO122

extern void Set_Serial_SPENB(unsigned long index); //Serial enable
extern void Set_Serial_SPDA(unsigned long index);
extern void Set_Serial_SCK(unsigned long index);
extern void Serial_SET_16BIT(MMP_USHORT cmd);
extern MMP_BYTE Serial_GET_16BIT(MMP_USHORT cmd);

#endif

#if defined(FAT_BOOT)
#define ENABLE_LCD_TEST_PATTERN 	(1)
#else
#define ENABLE_LCD_TEST_PATTERN 	(0)
#endif

#define LCD_BRIGETNESS_DEFAULT 		(1)

#define LCD_CONTRAST_DEFAULT 		(1)

#define LCD_VCOM_DEFAULT 			(0)

#define LCD_GAMMA_AUTO 				(1) // 0: Manual set gamma by R17h~R1Ah, 1: Auto set to gamma2.2


//==============================================================================
//
//                              FUNCTIONS
//
//==============================================================================

void RTNA_LCD_InitMainLCD(void)
{
#if (SECONDARY_DISPLAY == 0)
	RTNA_DBG_Str(0, "### RTNA_LCD_InitMainLCD -\r\n");

	m_PanelAttr.usDotClkRatio 	= 6;

	#if (LCD_RGB_MODE == STRIP_RGB)
	m_PanelAttr.usHBPorch 		= 0x3C;
	m_PanelAttr.usHBlanking 	= 0x47;
	#else
	m_PanelAttr.usHBPorch 		= 309-2/*24*/;
	m_PanelAttr.usHBlanking 	= 255/*0x46*/;
	#endif
	m_PanelAttr.usHSyncW 		= 2/*0*/;
	m_PanelAttr.usVBPorch 		= 21;
	m_PanelAttr.usVBlanking 	= 22;
	m_PanelAttr.usVSyncW 		= 2/*0*/;
	m_PanelAttr.usPRT2HdotClk 	= 2;

	#if (LCD_RGB_MODE == STRIP_RGB)
	m_PanelAttr.bDeltaRBG 		= MMP_FALSE; // TBD??
	m_PanelAttr.bDummyRBG 		= MMP_TRUE; // TBD??
	#else
	m_PanelAttr.bDeltaRBG 		= MMP_TRUE;
	m_PanelAttr.bDummyRBG 		= MMP_FALSE;
	#endif

	if ((0 == m_PanelAttr.ubEvenLnOrder) || (0 == m_PanelAttr.ubOddLnOrder))
	{
		// The order maybe changed by RTNA_LCD_Direction
		#if (LCD_RGB_MODE == STRIP_RGB)
		m_PanelAttr.ubEvenLnOrder 	= LCD_SPI_PIX_ORDER_RGB;
		m_PanelAttr.ubOddLnOrder 	= LCD_SPI_PIX_ORDER_RGB;
		#else
		m_PanelAttr.ubEvenLnOrder 	= LCD_SPI_PIX_ORDER_BGR;
		m_PanelAttr.ubOddLnOrder 	= LCD_SPI_PIX_ORDER_RBG;
		#endif

		RTNA_DBG_Str(0, "Change PIX order to default\r\n");
	}

	m_PanelAttr.usSpi2MciRatio 	= 0xff;//0x80;
	m_PanelAttr.usCsSetupCyc 	= 0x0F;
	m_PanelAttr.usCsHoldCyc 	= 0x0F;
	m_PanelAttr.usCsHighWidth 	= 0xFF;
	m_PanelAttr.usSPIRegBitCnt  = SPI_PANEL_16BITS;

	m_PanelAttr.ubDispWinId 	= LCD_DISP_WIN_PIP;
	m_PanelAttr.usWinStX 		= 0;
	m_PanelAttr.usWinStY 		= 0;
	m_PanelAttr.usWinW 			= LCD_ILI8961_WIDTH;
	m_PanelAttr.usWinH 			= LCD_ILI8961_HEIGHT;
	m_PanelAttr.usWinBPP 		= 2;
	m_PanelAttr.usWinFmt 		= LCD_WIN_FMT_16BPP;
	m_PanelAttr.ulWinAddr 		= 0x03000000;

	#if ((16 == LCD_MODEL_RATIO_X) && (9 == LCD_MODEL_RATIO_Y))
	m_PanelAttr.ubRatio 		= LCD_RATIO_16_9;
	#else
	m_PanelAttr.ubRatio 		= LCD_RATIO_4_3;
	#endif

	MMPF_LCD_InitPanel(&m_PanelAttr);
#endif
}

void RTNA_LCD_Init2ndLCD(void)
{
#if (SECONDARY_DISPLAY == 1)
	m_PanelAttr.usPanelW 		= LCD_ILI8961_WIDTH;
	m_PanelAttr.usPanelH 		= LCD_ILI8961_HEIGHT;
	m_PanelAttr.ubDevType 		= LCD_TYPE_RGBLCD;
	m_PanelAttr.ubController 	= LCD_SCD_CONTROLER;

	m_PanelAttr.pInitSeq 		= NULL;
	m_PanelAttr.pIdxCmdSeq 		= NULL;

	m_PanelAttr.bPartialDisp 	= MMP_FALSE;
	m_PanelAttr.ubHsyncPor 		= LCD_SIG_POLARITY_L;
	m_PanelAttr.ubVsyncPor 		= LCD_SIG_POLARITY_L;
	m_PanelAttr.ubRgbFmt 		= RGB_D24BIT_RGB888;

	m_PanelAttr.usDotClkRatio 	= 7;
	m_PanelAttr.usHBPorch 		= 0x3F;
	m_PanelAttr.usHBlanking 	= 0x46;
	m_PanelAttr.usHSyncW 		= 0;
	m_PanelAttr.usVBPorch 		= 13;
	m_PanelAttr.usVBlanking 	= 22;
	m_PanelAttr.usVSyncW 		= 0;
	m_PanelAttr.usPRT2HdotClk 	= 2;

	m_PanelAttr.bDeltaRBG 		= MMP_TRUE;
	m_PanelAttr.bDummyRBG 		= MMP_TRUE;
	m_PanelAttr.ubEvenLnOrder 	= LCD_SPI_PIX_ORDER_RGB;
	m_PanelAttr.ubOddLnOrder 	= LCD_SPI_PIX_ORDER_RGB;

	m_PanelAttr.usSpi2MciRatio 	= 0x80;
	m_PanelAttr.usCsSetupCyc 	= 0x0F;
	m_PanelAttr.usCsHoldCyc 	= 0x0F;
	m_PanelAttr.usCsHighWidth 	= 0xFF;

	m_PanelAttr.ubDispWinId 	= LCD_DISP_WIN_OSD;
	m_PanelAttr.usWinStX 		= 0;
	m_PanelAttr.usWinStY 		= 0;
	m_PanelAttr.usWinW 			= LCD_ILI8961_WIDTH;
	m_PanelAttr.usWinH 			= LCD_ILI8961_HEIGHT;
	m_PanelAttr.usWinBPP 		= 2;
	m_PanelAttr.usWinFmt 		= LCD_WIN_FMT_16BPP;
	m_PanelAttr.ulWinAddr 		= 0x02500000;

	MMPF_LCD_InitPanel(&m_PanelAttr);
#endif
}

void RTNA_LCD_Init(void)
{
	RTNA_DBG_Str(0, "### RTNA_LCD_Init for ILI8961 ...\r\n");

#if (SECONDARY_DISPLAY == 1)
	RTNA_LCD_Init2ndLCD();
#else
	RTNA_LCD_InitMainLCD();
#endif

	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x055F);
	Delayms(50);
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x051F);
	Delayms(100);
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x055F);
	Delayms(150);
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x2B01);
	Delayms(150);
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0B83);
	
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x000E);  //0E

	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x01A6); //b3
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0340);//48
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0799);//(&m_PanelAttr, 0x0748);//HBLK
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0D40);   //30


	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x040F);

	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0E40);//35
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0F40);//4//40
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x1040);
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x1140);//40
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x2F69);
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x1604); //0x1604
}

//============================================================================//
#if (ENABLE_LCD_ILI8961_LOG)
#include "Customer_config.h"
#include "mmpf_pio.h"
#endif

void RTNA_LCD_BLACK_LIGHT(MMP_BOOL enable)
{
  	if(enable)
    MMPF_LCD_Write16BitCmd(&m_PanelAttr,0x0340);
  else
    MMPF_LCD_Write16BitCmd(&m_PanelAttr,0x0300);
}

void RTNA_LCD_RGB_Test(void)
{
#if (ENABLE_LCD_TEST_PATTERN)
	#if defined(LCD_GPIO_BACK_LIGHT)
		/* Force turn-on LCD backlight */
		if (LCD_GPIO_BACK_LIGHT != MMP_GPIO_MAX)
		{
			MMPF_PIO_EnableOutputMode(LCD_GPIO_BACK_LIGHT, MMP_TRUE, MMP_TRUE);
			RTNA_DBG_Str(0, "dbg-set LCD_BL to output mode\r\n");

			MMPF_PIO_SetData(LCD_GPIO_BACK_LIGHT, !LCD_GPIO_BACK_LIGHT_ACT_LEVEL, MMP_TRUE);
			Delayms(20);
			RTNA_DBG_Str(0, "dbg-set LCD_BL to low\r\n");

			MMPF_PIO_SetData(LCD_GPIO_BACK_LIGHT, LCD_GPIO_BACK_LIGHT_ACT_LEVEL, MMP_TRUE);
			Delayms(20);
			RTNA_DBG_Str(0, "dbg-set LCD_BL to high\r\n");
		}
	#endif

	MMPF_LCD_DrawTestPattern(&m_PanelAttr);
#endif
}

void RTNA_LCD_Direction(LCD_DIR dir)
{
	DSPY_DECL;
	MMP_UBYTE value;

#if (ENABLE_LCD_ILI8961_LOG)
	RTNA_DBG_Str(0, "### RTNA_LCD_Direction, dir:");
	RTNA_DBG_Short(0, dir);
	RTNA_DBG_Str(0, "\r\n");

	RTNA_DBG_Str(0, "pDSPY->DSPY_RGB_DELTA_MODE at ");
	RTNA_DBG_Long(0, (MMP_ULONG)&(pDSPY->DSPY_RGB_DELTA_MODE));
	RTNA_DBG_Str(0, "\r\n");
	RTNA_DBG_Str(0, "pDSPY->DSPY_RGB_DELTA_MODE = ");
	RTNA_DBG_Long(0, *(AIT_REG_B *)((MMP_ULONG)&(pDSPY->DSPY_RGB_DELTA_MODE)));
	RTNA_DBG_Str(0, "\r\n");
#endif

	switch(dir)
	{
		case LCD_DIR_DOWNLEFT_UPRIGHT:
			// not support;
			break;

		case LCD_DIR_UPRIGHT_DOWNLEFT:
			// not support;
			break;

		case LCD_DIR_DOWNRIGHT_UPLEFT:
		#if (ENABLE_LCD_ILI8961_LOG)
			RTNA_DBG_Str(0, "LCD_DIR_DOWNRIGHT_UPLEFT -\r\n");
		#endif

			value = 0x0C;
		#if (LCD_RGB_MODE == STRIP_RGB)
			DSPY_WR_B(DSPY_RGB_DELTA_MODE, RGB_DUMMY_MODE_ENABLE |
										   RGB_DELTA_MODE_ENABLE |
										   SPI_ODD_LINE_RGB |
										   SPI_EVEN_LINE_RGB);
		#else
			DSPY_WR_B(DSPY_RGB_DELTA_MODE, RGB_DELTA_MODE_ENABLE |
										   SPI_ODD_LINE_RBG |
										   SPI_EVEN_LINE_BGR);
			m_PanelAttr.ubOddLnOrder 	= LCD_SPI_PIX_ORDER_RBG;
			m_PanelAttr.ubEvenLnOrder 	= LCD_SPI_PIX_ORDER_BGR;
		#endif

		#if (ENABLE_LCD_ILI8961_LOG)
			RTNA_DBG_Str(0, "pDSPY->DSPY_RGB_DELTA_MODE at ");
			RTNA_DBG_Long(0, (MMP_ULONG) &(pDSPY->DSPY_RGB_DELTA_MODE));
			RTNA_DBG_Str(0, "\r\n");
			RTNA_DBG_Str(0, "pDSPY->DSPY_RGB_DELTA_MODE = ");
			RTNA_DBG_Long(0, *(AIT_REG_B *)((MMP_ULONG)&(pDSPY->DSPY_RGB_DELTA_MODE)));
			RTNA_DBG_Str(0, "\r\n");
		#endif
			break;

		case LCD_DIR_UPLEFT_DOWNRIGHT:
		#if (ENABLE_LCD_ILI8961_LOG)
			RTNA_DBG_Str(0, "LCD_DIR_UPLEFT_DOWNRIGHT -\r\n");
		#endif

			value = 0x0C | 0x03;
		#if (LCD_RGB_MODE == STRIP_RGB)
			DSPY_WR_B(DSPY_RGB_DELTA_MODE, RGB_DUMMY_MODE_ENABLE |
										   RGB_DELTA_MODE_ENABLE |
										   SPI_ODD_LINE_RGB |
										   SPI_EVEN_LINE_RGB);
		#else
			DSPY_WR_B(DSPY_RGB_DELTA_MODE, RGB_DELTA_MODE_ENABLE |
										   SPI_ODD_LINE_RGB |
										   SPI_EVEN_LINE_GBR);
			m_PanelAttr.ubOddLnOrder 	= LCD_SPI_PIX_ORDER_RGB;
			m_PanelAttr.ubEvenLnOrder 	= LCD_SPI_PIX_ORDER_GBR;
		#endif
			break;

		default:
			break;
	}

#if (ENABLE_LCD_ILI8961_LOG)
	RTNA_DBG_Str(0, "New pDSPY->DSPY_RGB_DELTA_MODE = ");
	RTNA_DBG_Long(0, *(AIT_REG_B *)((MMP_ULONG)&(pDSPY->DSPY_RGB_DELTA_MODE)));
	RTNA_DBG_Str(0, "\r\n");
#endif

	MMPF_LCD_Write16BitCmd(&m_PanelAttr, (0x04 << 8) | value);
}
#if (ENABLE_LCD_ILI8961_SW_3W_SERIAL_CMD == 1)

void Set_Serial_SPENB(unsigned long index)
{
	if(index) MMPF_PIO_SetData(SPENB_GPIO_PIN, GPIO_HIGH, MMP_TRUE);//rGPEDAT |= 0x8000;
	else       MMPF_PIO_SetData(SPENB_GPIO_PIN, GPIO_LOW, MMP_TRUE);//rGPEDAT &= ~0x8000;	
}

void Set_Serial_SPDA(unsigned long index)
{
	if(index) MMPF_PIO_SetData(SPDA_GPIO_PIN, GPIO_HIGH, MMP_TRUE);//rGPEDAT |= 0x2000;
	else       MMPF_PIO_SetData(SPDA_GPIO_PIN, GPIO_LOW, MMP_TRUE);//rGPEDAT &= ~0x2000;	
}

void Set_Serial_SCK(unsigned long index)
{
	if(index) MMPF_PIO_SetData(SPCK_GPIO_PIN, GPIO_HIGH, MMP_TRUE);//rGPEDAT |= 0x1000;
	else      MMPF_PIO_SetData(SPCK_GPIO_PIN, GPIO_LOW, MMP_TRUE);//rGPEDAT &= ~0x1000;	
}

void Serial_SET_16BIT(MMP_USHORT cmd)
{
	unsigned long kk;
	
	Set_Serial_SPENB(0);			//Set DC=0, for writing to Command register
	Set_Serial_SPDA(0);
	Set_Serial_SCK(0);
	if((cmd&0x4000)==0x4000) 
       Set_Serial_SPDA(1);
	else         
       Set_Serial_SPDA(0);
	Set_Serial_SCK(0);
	Set_Serial_SCK(1);	
	Set_Serial_SCK(0);
	Set_Serial_SPDA(0); // write command
	Set_Serial_SCK(1);
	Set_Serial_SCK(0);
	cmd = cmd<<1;
	for(kk=0;kk<14;kk++)
	{
		if((cmd&0x4000)==0x4000) 
            Set_Serial_SPDA(1);
		else         
            Set_Serial_SPDA(0);
		Set_Serial_SCK(1);
		Set_Serial_SCK(0);
		cmd = cmd<<1;	
	}
	Set_Serial_SPENB(1);
}
MMP_BYTE Serial_GET_16BIT(MMP_USHORT cmd)
{
	long kk;
	MMP_UBYTE data=0;
	MMP_BYTE out=0;
	Set_Serial_SPENB(0);			//Set DC=0, for writing to Command register
	Set_Serial_SPDA(0);
	Set_Serial_SCK(0);
	if((cmd&0x4000)==0x4000) 
       Set_Serial_SPDA(1);
	else         
       Set_Serial_SPDA(0);
    cmd = cmd<<1;
	Set_Serial_SCK(0);
	Set_Serial_SCK(1);	
	Set_Serial_SCK(0);
	Set_Serial_SPDA(1); // Read command
	Set_Serial_SCK(1);
	Set_Serial_SCK(0);
	
	for(kk=0;kk<6;kk++)
	{
		if((cmd&0x4000)==0x4000) 
            Set_Serial_SPDA(1);
		else         
            Set_Serial_SPDA(0);
		Set_Serial_SCK(1);
		Set_Serial_SCK(0);
		cmd = cmd<<1;	
	}
	Set_Serial_SCK(0);
	MMPF_PIO_EnableOutputMode(SPDA_GPIO_PIN, MMP_FALSE, MMP_TRUE);
	for(kk=7;kk>=0;kk--)
	{	    
		Set_Serial_SCK(1);
		MMPF_PIO_GetData(SPDA_GPIO_PIN, &data);
		out |= data<<kk;
		Set_Serial_SCK(0);
		cmd = cmd<<1;	
	}
	MMPF_PIO_EnableOutputMode(SPDA_GPIO_PIN, MMP_TRUE, MMP_TRUE);
	Set_Serial_SPENB(1);
	return out;
}
#endif
void RTNA_LCD_SetReg(MMP_ULONG reg, MMP_UBYTE value)
{
    #if (ENABLE_LCD_ILI8961_SW_3W_SERIAL_CMD == 0)
	MMP_SHORT Tempvalue;

	Tempvalue = (((reg & 0xFF) << 8) + value);
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, Tempvalue);
    #else
    MMP_SHORT Tempvalue;
    OS_CRITICAL_INIT();
	Tempvalue = (((reg & 0xFF) << 8) + value);
	OS_ENTER_CRITICAL();
    MMPF_PIO_EnableOutputMode(SPENB_GPIO_PIN, MMP_TRUE, MMP_TRUE);
	MMPF_PIO_EnableOutputMode(SPDA_GPIO_PIN, MMP_TRUE, MMP_TRUE);
	MMPF_PIO_EnableOutputMode(SPCK_GPIO_PIN, MMP_TRUE, MMP_TRUE); 
	Serial_SET_16BIT(Tempvalue); 
	MMPF_PIO_Enable(SPENB_GPIO_PIN,MMP_FALSE);
	MMPF_PIO_Enable(SPDA_GPIO_PIN,MMP_FALSE);
	MMPF_PIO_Enable(SPCK_GPIO_PIN,MMP_FALSE);
	OS_EXIT_CRITICAL();
    #endif
}

void RTNA_LCD_GetReg(MMP_ULONG reg, MMP_ULONG *value)
{
    #if (ENABLE_LCD_ILI8961_SW_3W_SERIAL_CMD == 1)
    MMP_SHORT Tempvalue;
	MMP_BYTE regval;
    OS_CRITICAL_INIT();
	Tempvalue = (((reg & 0xFF) << 8));
	OS_ENTER_CRITICAL();
    MMPF_PIO_EnableOutputMode(SPENB_GPIO_PIN, MMP_TRUE, MMP_TRUE);
	MMPF_PIO_EnableOutputMode(SPDA_GPIO_PIN, MMP_TRUE, MMP_TRUE);
	MMPF_PIO_EnableOutputMode(SPCK_GPIO_PIN, MMP_TRUE, MMP_TRUE);
	regval= Serial_GET_16BIT(Tempvalue);  
	MMPF_PIO_Enable(SPENB_GPIO_PIN,MMP_FALSE);
	MMPF_PIO_Enable(SPDA_GPIO_PIN,MMP_FALSE);
	MMPF_PIO_Enable(SPCK_GPIO_PIN,MMP_FALSE);
	OS_EXIT_CRITICAL();

	RTNA_DBG_Str0("\r\nReg: ");
	RTNA_DBG_Byte0((MMP_BYTE)(reg&0xFF));
	RTNA_DBG_Str0(" RegVal: ");
	RTNA_DBG_Byte0(regval);
	RTNA_DBG_Str0("\r\n");
    #endif
}

void RTNA_LCD_AdjustBrightness(MMP_UBYTE level)
{
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0300 | level); 	// Brightness(R03h[7:0]): RGB brightness level control
}

void RTNA_LCD_AdjustBrightness_R(MMP_UBYTE level)
{
	// TBD
}

void RTNA_LCD_AdjustBrightness_B(MMP_UBYTE level)
{
	// TBD
}

void RTNA_LCD_AdjustContrast(MMP_UBYTE level)
{
	MMPF_LCD_Write16BitCmd(&m_PanelAttr, 0x0D00 | level); 	// CONTRAST(R0Dh[7:0]): RGB contrast level setting, the gain changes (1/64) bit.
}

void RTNA_LCD_AdjustContrast_R(MMP_BYTE level)
{
	// TBD
}

void RTNA_LCD_AdjustContrast_B(MMP_BYTE level)
{
	// TBD
}

/*
 * Set Panel Properties
 * Width, Height, Color Depth, Display Type
 */
MMPF_PANEL_ATTR* RTNA_LCD_GetAttr(void)
{
	return &m_PanelAttr;
}

void ApkIli8961LcdDbg(MMP_USHORT uType, MMP_USHORT uData)
{
	
	if (uType)
	{
		uData &= 0x00FF;
		uData |= 0x1000;
		MMPF_LCD_Write16BitCmd(&m_PanelAttr, uData);
		#if !defined(MBOOT_FW)
		printc("[APK]ApkIli8961LcdDbg Set Blue Contrast %d\r\n", (uData & 0xFF));
		#endif
	}
	else
	{
		uData &= 0x00FF;
		uData |= 0x1100;
		MMPF_LCD_Write16BitCmd(&m_PanelAttr, uData);
		#if !defined(MBOOT_FW)
		printc("[APK]ApkIli8961LcdDbg Set Blue Bright %d\r\n", (uData & 0xFF));
		#endif
	}
}


