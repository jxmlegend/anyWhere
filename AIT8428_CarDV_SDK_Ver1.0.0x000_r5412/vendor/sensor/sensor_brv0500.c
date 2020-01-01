//==============================================================================
//
//  File        : sensor_brv0500.c
//  Description : Firmware Sensor Control File
//  Author      :
//  Revision    : 1.0
//
//==============================================================================

//==============================================================================
//
//                              INCLUDE FILE
//
//==============================================================================

#include "includes_fw.h"
#include "customer_config.h"

#if (SENSOR_EN)
#if (BIND_SENSOR_BRV0500)

#include "mmpf_sensor.h"
#include "Sensor_Mod_Remapping.h"
#include "isp_if.h"
#include "hdr_cfg.h"
#include "snr_cfg.h"

//==============================================================================
//
//                              MACRO DEFINE
//
//==============================================================================
#define MAX_SENSOR_GAIN (16)
#define ISP_DGAIN_BASE  (0x200)

//==============================================================================
//
//                              GLOBAL VARIABLE
//
//==============================================================================

// Resolution Table
MMPF_SENSOR_RESOLUTION m_SensorRes = 
{
    16,     // ubSensorModeNum
    0,      // ubDefPreviewMode
    5,      // ubDefCaptureMode
    2000,   // usPixelSize
//   HDR    FHD@60  FHD/1440@30                                                                     HD@120                  4K@24 
// 	Mode0   Mode1   Mode2	Mode3   Mode4   Mode5   Mode6   Mode7   Mode8   Mode9   Mode10  Mode11  Mode12	Mode13  Mode14  Mode15
    {1,		1,		1,    	1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      3,      1},    	// usVifGrabStX
    {1,     1,		1,    	1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      3,      1},    	// usVifGrabStY
    {2436,	2692,	2592, 	1288,   2592,   1928,   1928,   2032,   1928,   1288,   1288,   1288,   1348,	644,    1516,   2692}, 	// usVifGrabW
    {1370,	1524,	1944,  	964,    1944,   1092,   1092,   1524,   1092,   724,    724,    724,    760,    482,    1516,   1524}, 	// usVifGrabH
#if (CHIP == MCR_V2)
    {1,		1,		1,     	1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1},    	// usBayerInGrabX
    {1,		1,		1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1},    	// usBayerInGrabY
    {4,		4,		16,   	8,      8,      8,      8,      8,    	8,      8,      8,      8,      4,      4,      4,      4},   	// usBayerInDummyX
    {2,		12,		 8,     4,      8,     12,     12,     4,     	12,     4,      4,      4,      4,      2,      4,      12},    // usBayerInDummyY
    {2432,	1920,	2560, 	1280,   1920,   1920,   1920,   2024,   1920,   1280,   1280,   1280,   1344, 	640,    1512,   2688},	// usBayerOutW
    {1368,	1080,	1440,  	960,    1080,   1080,   1080,   1520,   1080,   720,    720,    720,    756,    480,    1512,   1512}, 	// usBayerOutH
#endif
    {2432,	1920,	2560, 	1280,   1920,   1920,   1920,   2024,   1920,   1280,   1280,   1280,   1344,	640,    1512,   2688}, 	// usScalInputW
    {1368,	1080,	1440,  	960,   	1080,   1080,   1080,   1520,   1080,  	720,    720,    720,    756,    480,    1512,   1512}, 	// usScalInputH
    {300,   600,    300,   	300,    300,    500,    600,    300,    150,    300,    600,   	1000,   1200,	300,    300,    240}, 	// usTargetFpsx10
    {1540,	1600,	2002, 	1597,   2002,   1136,   1120,   1558,   1136,   1597,	797,    797,    821,   1589,    1552,   1562},	// usVsyncLine (VTS: Reg0x380E-F)
    {1,		1,		1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1},    	// ubWBinningN
    {1,		1,		1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1},    	// ubWBinningM
    {1,		1,		1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1},    	// ubHBinningN
    {1,		1,		1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1},     // ubHBinningM
    {0xFF,	0xFF,	0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  // ubCustIQmode
    {0xFF,	0xFF,	0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF}   // ubCustAEmode
};

// VIF Setting
static MMPF_SENSOR_VIF_SETTING m_VifSetting_Prm =
{
    // SnrType
    MMPF_VIF_SNR_TYPE_BAYER,

    // OutInterface
    #if (SENSOR_IF == SENSOR_IF_PARALLEL)
    MMPF_VIF_IF_PARALLEL,
    #elif (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    MMPF_VIF_IF_MIPI_SINGLE_0,
    #elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    MMPF_VIF_IF_MIPI_DUAL_01,
    #elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    MMPF_VIF_IF_MIPI_QUAD,
    #endif

    // VifPadId
    #if (PRM_SENSOR_VIF_ID == MMPF_VIF_MDL_ID1)
    MMPF_VIF_MDL_ID1,
    #else
    MMPF_VIF_MDL_ID0,
    #endif

    // powerOnSetting
    {
        MMP_TRUE,                       // bTurnOnExtPower
        SENSOR_GPIO_ENABLE,             // usExtPowerPin
        SENSOR_GPIO_ENABLE_ACT_LEVEL,   // bExtPowerPinHigh
        50,                             // usExtPowerPinDelay
        MMP_FALSE,                      // bFirstEnPinHigh
        10,                             // ubFirstEnPinDelay
        MMP_TRUE,                       // bNextEnPinHigh
        100,                            // ubNextEnPinDelay
        MMP_TRUE,                       // bTurnOnClockBeforeRst
        MMP_FALSE,                      // bFirstRstPinHigh
        100,                            // ubFirstRstPinDelay
        MMP_TRUE,                       // bNextRstPinHigh
        100                             // ubNextRstPinDelay
    },

    // powerOffSetting
    {
        MMP_FALSE,                      // bEnterStandByMode
        0x0100,                         // usStandByModeReg
        0x01,                           // usStandByModeMask
        MMP_TRUE,                       // bEnPinHigh
        20,                             // ubEnPinDelay
        MMP_TRUE,                       // bTurnOffMClock
        MMP_TRUE,                       // bTurnOffExtPower
        SENSOR_GPIO_ENABLE              // usExtPowerPin
    },

    // clockAttr
    {
        MMP_TRUE,                       // bClkOutEn
        0,                              // ubClkFreqDiv
        24000,                          // ulMClkFreq
        24000,			                // ulDesiredFreq
        MMPF_VIF_SNR_PHASE_DELAY_NONE,  // ubClkPhase
        MMPF_VIF_SNR_CLK_POLARITY_POS,  // ubClkPolarity
        MMPF_VIF_SNR_CLK_SRC_PMCLK      // ubClkSrc
    },

    // paralAttr
    {
        MMPF_VIF_SNR_LATCH_POS_EDGE,    // ubLatchTiming
        MMPF_VIF_SNR_CLK_POLARITY_POS,  // ubHsyncPolarity
        MMPF_VIF_SNR_CLK_POLARITY_POS,  // ubVsyncPolarity
        MMPF_VIF_SNR_PARAL_BITMODE_16   // ubBusBitMode
    },

    // mipiAttr
    {
        MMP_FALSE,                      // bClkDelayEn
        MMP_FALSE,                      // bClkLaneSwapEn
        0,                              // usClkDelay
        MMPF_VIF_SNR_LATCH_NEG_EDGE,    // ubBClkLatchTiming
#if (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
        // Lane 0,                      Lane 1,                             Lane 2,                             Lane 3
        {MMP_TRUE,                      MMP_TRUE,                           MMP_TRUE,                           MMP_TRUE},                          // bDataLaneEn
        {MMP_TRUE,                      MMP_TRUE,                           MMP_TRUE,                           MMP_TRUE},                          // bDataDelayEn
        {MMP_FALSE,                     MMP_FALSE,                          MMP_FALSE,                          MMP_FALSE},                         // bDataLaneSwapEn
        {MMPF_VIF_MIPI_DATA_SRC_PHY_0,  MMPF_VIF_MIPI_DATA_SRC_PHY_1,       MMPF_VIF_MIPI_DATA_SRC_PHY_2,       MMPF_VIF_MIPI_DATA_SRC_PHY_3},      // ubDataLaneSrc
        {0,                             0,                                  0,                                  0},                                 // usDataDelay
        {0x1F,                          0x1F,                               0x1F,                               0x1F}                               // ubDataSotCnt
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
        // Lane 0,                      Lane 1,                             Lane 2,                             Lane 3
        {MMP_TRUE,                      MMP_TRUE,                           MMP_FALSE,                          MMP_FALSE},                         // bDataLaneEn
        {MMP_TRUE,                      MMP_TRUE,                           MMP_FALSE,                          MMP_FALSE},                         // bDataDelayEn
        {MMP_FALSE,                     MMP_FALSE,                          MMP_FALSE,                          MMP_FALSE},                         // bDataLaneSwapEn
        #if (PRM_SENSOR_VIF_ID == MMPF_VIF_MDL_ID1)
        {MMPF_VIF_MIPI_DATA_SRC_PHY_1,  MMPF_VIF_MIPI_DATA_SRC_PHY_2,       MMPF_VIF_MIPI_DATA_SRC_PHY_UNDEF,   MMPF_VIF_MIPI_DATA_SRC_PHY_UNDEF},  // ubDataLaneSrc
        #else
        {MMPF_VIF_MIPI_DATA_SRC_PHY_0,  MMPF_VIF_MIPI_DATA_SRC_PHY_1,       MMPF_VIF_MIPI_DATA_SRC_PHY_UNDEF,   MMPF_VIF_MIPI_DATA_SRC_PHY_UNDEF},  // ubDataLaneSrc
        #endif
        {0,                             0,                                  0,                                  0},                                 // usDataDelay
        {0x1F,                          0x1F,                               0x1F,                               0x1F}                               // ubDataSotCnt
#elif (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
        // Lane 0,                      Lane 1,                             Lane 2,                             Lane 3
        {MMP_TRUE,                      MMP_FALSE,                          MMP_FALSE,                          MMP_FALSE},                         // bDataLaneEn
        {MMP_TRUE,                      MMP_FALSE,                          MMP_FALSE,                          MMP_FALSE},                         // bDataDelayEn
        {MMP_FALSE,                     MMP_FALSE,                          MMP_FALSE,                          MMP_FALSE},                         // bDataLaneSwapEn
        {MMPF_VIF_MIPI_DATA_SRC_PHY_0,  MMPF_VIF_MIPI_DATA_SRC_PHY_UNDEF,   MMPF_VIF_MIPI_DATA_SRC_PHY_UNDEF,   MMPF_VIF_MIPI_DATA_SRC_PHY_UNDEF},  // ubDataLaneSrc
        {0,                             0,                                  0,                                  0},                                 // usDataDelay
        {0x1F,                          0x1F,                               0x1F,                               0x1F}                               // ubDataSotCnt
#endif
    },

    // colorId
    {
        MMPF_VIF_COLORID_01,            // VifColorId  // BRV0200
        MMP_FALSE                       // bUseCustomId
    },

    // vcAttr
    {
        MMP_FALSE,                                      	// bEnable
        MMP_FALSE,                                      	// bAllChannel2Isp
        {MMP_FALSE, MMP_FALSE, MMP_FALSE, MMP_FALSE},   	// bVC2Isp
        {MMP_FALSE, MMP_FALSE, MMP_FALSE, MMP_FALSE},  	    // bVC2Raw
        MMP_FALSE                                       	// bSlowFsForStagger
    },

    // yuvAttr
    {
        MMP_FALSE,                                      	// bRawStoreEnable
        // MMP_FALSE,                                      	// bYuv422ToYuv420
        // MMP_FALSE,                                      	// bYuv422ToYuv422
        // MMP_FALSE,                                      	// bYuv422ToBayer
        // MMPF_VIF_YUV422_YUYV,                           	// ubYuv422Order
    }
};

// IQ Table
const ISP_UINT8 Sensor_IQ_CompressedText[] = 
{
#if defined(SUPPORT_UVC_ISP_EZMODE_FUNC) && (SUPPORT_UVC_ISP_EZMODE_FUNC)       
    #include "isp_8428_iq_data_v3_OV4689_ezmode.xls.ciq.txt"
#else // Use old IQ table
    #ifdef CUS_ISP_8428_IQ_DATA     // maybe defined in project MCP or Config_SDK.h
    #include CUS_ISP_8428_IQ_DATA
    #else
    #include "isp_8428_iq_data_v2_OV4689_v1_casio.xls.ciq.txt"
    #endif
#endif
};

#if defined(SUPPORT_UVC_ISP_EZMODE_FUNC) && (SUPPORT_UVC_ISP_EZMODE_FUNC) 
// Replace it by custom IQ table.
const  __align(4) ISP_UINT8 Sensor_EZ_IQ_CompressedText[] = 
{
    #include "isp_8428_iq_data_v3_OV4689_ezmode.iq.custom.txt"
};

ISP_UINT32 eziqsize = sizeof(Sensor_EZ_IQ_CompressedText);
#endif

// I2cm Attribute for primary sensor
static MMP_I2CM_ATTR m_I2cmAttr_Prm = 
{
    MMP_I2CM0,      // i2cmID
    (0x26 >> 1),    // ubSlaveAddr
    16,             // ubRegLen
    16,              // ubDataLen
    0,              // ubDelayTime
    MMP_FALSE,      // bDelayWaitEn
    MMP_TRUE,       // bInputFilterEn
    MMP_FALSE,      // b10BitModeEn
    MMP_FALSE,      // bClkStretchEn
    0,              // ubSlaveAddr1
    0,              // ubDelayCycle
    0,              // ubPadNum
    150000,         // ulI2cmSpeed 150KHZ
    MMP_TRUE,       // bOsProtectEn
    NULL,           // sw_clk_pin
    NULL,           // sw_data_pin
    MMP_FALSE,      // bRfclModeEn
    MMP_FALSE,      // bWfclModeEn
    MMP_FALSE,      // bRepeatModeEn
    0               // ubVifPioMdlId
};

// 3A Timing
MMPF_SENSOR_AWBTIMIMG m_AwbTime = 
{
    3,  // ubPeriod
    1,  // ubDoAWBFrmCnt
    2   // ubDoCaliFrmCnt
};

MMPF_SENSOR_AETIMIMG m_AeTime = 
{
    4,  // ubPeriod
    0,  // ubFrmStSetShutFrmCnt
    0   // ubFrmStSetGainFrmCnt
};

MMPF_SENSOR_AFTIMIMG m_AfTime = 
{
    1,  // ubPeriod
    0   // ubDoAFFrmCnt
};

// IQ Data
#define ISP_IQ_DATA_NAME "isp_8428_iq_data_v2_OV4689_v1_casio.xls.ciq.txt"

static const MMP_UBYTE s_IqCompressData[] =
{
	#include ISP_IQ_DATA_NAME
};

#if (ISP_EN) //For HDR
static ISP_UINT16 ae_gain_l;
static ISP_UINT16 ae_shutter_l;
static ISP_UINT16 ae_gain_s;
static ISP_UINT16 ae_shutter_s;

ISP_UINT16 hdr_th           = 6;        //fix
ISP_UINT16 hdr_gap          = 7;        //fix
ISP_UINT16 hdr_gain         = 6*256;    //dynamic by code
ISP_UINT16 hdr_short_gain   = 13;       //fix
ISP_UINT16 hdr_x_thd        = 1023;     //fix
ISP_UINT16 hdr_on           = 1;        //hdr on
ISP_UINT16 sensor_max_gain  = 16;       //fix
#endif

ISP_UINT32 dgain;
extern MMP_ULONG  m_ulISPFrameCount;

// Casio : when use this, open code in DoAEOperation_ST
#ifdef CUS_AE_BIAS_TBL		// maybe defined in project's MCP or Config_SDK.h

#include CUS_AE_BIAS_TBL

#else

// AE curve	new extent node for18//LV1,		LV2,		LV3,		LV4,		LV5,		LV6,		LV7,		LV8,		LV9,		LV10,	LV11,	LV12,	LV13,	LV14,	LV15,	LV16 	LV17  	LV18
// test,IQ 8
ISP_UINT32 AE_Bias_tbl[54] =
{
                /*Lux*/	        4,			8,			15,			30,			56, 		118, 		224, 		426, 		810,		1630,	3369,	7101,	14720,	28380,	53300,	110000,	220000, 440000,
                /*ENG*/		    0x2FFFFFF,  4841472*2,	2420736,    1562060,	801396,     439704, 	241131, 	132233, 	72400,		39000,	19000,	11000,	5000,	3084,	1716,	966,	370,    200,
                /*Tar*/	        110,		145,	    175,	    200,		225,		240,		253,		256,		256,		256,	256,	256,	256,	226,	226,	256,	256,    256,
};

#define AE_tbl_size  (18)

#endif

//==============================================================================
//
//                              FUNCTIONS
//
//==============================================================================

#if 0
void ____Sensor_Init_OPR_Table____(){ruturn;} //dummy
#endif

ISP_UINT16 SNR_BRV0500_Reg_Init_Customer[] = 
{
    SENSOR_DELAY_REG, 100 // delay
};

#if 0
void ____Sensor_Res_OPR_Table____(){ruturn;} //dummy
#endif

static ISP_UINT16 SNR_BRV0500_Reg_2432x1368_60P_HDR[] = 
{
    //TBD
    SENSOR_DELAY_REG, 100 // delay
};

//*** FHD@60fps (Index 1)
static ISP_UINT16 SNR_BRV0500_Reg_2688x1520_60P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

//*** FHD@30fps (Index 2)
static ISP_UINT16 SNR_BRV0500_Reg_2688x1520_30P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    //======================================
    // Mode number  : mode1_10b
    // Mode name    : 5M-B
    // Features     : 30fps, 10bit, 4lane MIPI
    // External clk : 24MHz
    // Frame rate   : 30fps
    // Data I/F     : 4lane MIPI, 10bit
    // Data rate    : 672MHz
    // Row time     : 16.67us (1600 ana_clk)
    // V total      : 2002Line
    // Output mode  : 10bit
    // Readout pixel: Total = 2592x1944, active area = (0,0,2592,1944)
    //======================================
    0x0400, 0x1d80,
    0x041e, 0x0203,  //Tint take effect = 2 frames;     Gain take effect = 1 frame
    0x0800, 0x010d,
    0x0802, 0x1b0f,
    0x0804, 0x0037,
    0x0806, 0x0100,
    0x0c18, 0x3701,
    0x0c16, 0x83cc,
    0x0c08, 0x07d2,
    0x0c0e, 0x0640,
    0x0c10, 0x0320,
    0x0c0a, 0x0002,
    0x0C00, 0x0208,
    0x0c02, 0x0001,
    0x0c1a, 0x4008,
    0x0c1c, 0x07b4,
    0x0c30, 0xffff,
    0x0c34, 0x1000,
    0x0c32, 0x0000,
    0x0c42, 0x0244,
    0x1000, 0x4004,
    0x1002, 0x406c,
    0x1004, 0x40b2,
    0x1006, 0x4008,
    0x1008, 0x4070,
    0x100a, 0x40b6,
    0x100c, 0x1012,
    0x100e, 0x10be,
    0x1010, 0x4010,
    0x1012, 0x1002,
    0x1014, 0x1006,
    0x1016, 0x100a,
    0x1018, 0x1024,
    0x101a, 0x10b4,
    0x101c, 0x10b8,
    0x101e, 0x4000,
    0x1020, 0x4004,
    0x1022, 0x4008,
    0x1024, 0x1006,
    0x1026, 0x100a,
    0x1028, 0x1066,
    0x102a, 0x106e,
    0x102c, 0x1072,
    0x102e, 0x107a,
    0x1030, 0x4004,
    0x1032, 0x4008,
    0x1034, 0x4064,
    0x1036, 0x0013,
    0x1038, 0x0067,
    0x103a, 0x0065,
    0x103c, 0x00bf,
    0x103e, 0x0415,
    0x1040, 0x00bd,
    0x1046, 0x0032,
    0x1048, 0x0061,
    0x104a, 0x000a,
    0x104c, 0x0011,
    0x104e, 0x0014,
    0x1050, 0x00be,
    0x1056, 0x0000,
    0x1058, 0x0012,
    0x105a, 0x0066,
    0x105c, 0x008e,
    0x106e, 0x2055,
    0x1080, 0x0014,
    0x1082, 0x00be,
    0x1088, 0x0036,
    0x108a, 0x003a,
    0x108c, 0x0000,
    0x108e, 0x0011,
    0x1090, 0x0014,
    0x1092, 0x00be,
    0x1098, 0x0449,
    0x109a, 0x00bd,
    0x109c, 0x0000,
    0x109e, 0x0000,
    0x10a0, 0x0000,
    0x10a2, 0x0000,
    0x10a4, 0x0000,
    0x10a6, 0x0028,
    0x10b2, 0x0067,
    0x10b4, 0x0800,
    0x10b6, 0x0000,
    0x10b8, 0x0000,
    0x10ba, 0x0000,
    0x10bc, 0x0000,
    0x10be, 0x0000,
    0x10c0, 0x0800,
    0x10c2, 0x0000,
    0x10c8, 0x0800,
    0x10ca, 0x0000,
    0x10cc, 0x0437,
    0x10ce, 0x005a,
    0x10dc, 0x0039,
    0x10de, 0x003f,
    0x10f0, 0x0063,
    0x10f2, 0x0069,
    0x1100, 0x0070,
    0x1102, 0x00bc,
    0x1104, 0x0126,
    0x1106, 0x0172,
    0x1108, 0x0000,
    0x110a, 0x0000,
    0x110c, 0x0000,
    0x110e, 0x0000,
    0x1110, 0x00b8,
    0x1112, 0x00c0,
    0x1114, 0x016e,
    0x1116, 0x0176,
    0x1118, 0x0000,
    0x111a, 0x0000,
    0x111c, 0x0000,
    0x111e, 0x0000,
    0x1122, 0x2126,
    0x1124, 0x0000,
    0x1126, 0x0000,
    0x112a, 0x20d5,
    0x112c, 0x2126,
    0x112e, 0x0000,
    0x1130, 0x0000,
    0x1132, 0x0000,
    0x1134, 0x20c2,
    0x1136, 0x0000,
    0x113c, 0x0043,
    0x113e, 0x004b,
    0x1142, 0x00d8,
    0x1144, 0x0000,
    0x1146, 0x0000,
    0x1400, 0x1002,
    0x1402, 0x00a2,
    0x1404, 0x00a6,
    0x1406, 0x7011,
    0x1800, 0x1016,
    0x1c04, 0x0ca8,
    0x1c00, 0x1008,
    0x1164, 0x0000,
    0x1168, 0x0000,
    0x1166, 0x0000,
    0x1168, 0x0000,
    0x0402, 0x0080,
    0x103A, 0x0865,
    0x0020, 0x848A,
    0x0022, 0x8A20,
    0x001C, 0x82A4,
    0x001E, 0xA884,
    0x0C1C, 0x07B4,
    0x0C1E, 0x07A4,
    0x0C38, 0x0001,
    0x0C20, 0x07A8,
    0x0C22, 0x0007,
    0x0C3C, 0x0799,
    0x102A, 0x102A,
    0x1002, 0x4028,
    0x102C, 0x102E,
    0x1008, 0x402C,
    0x1080, 0x04BE,
    0x1082, 0x00C4,
    0x108C, 0x00BF,
    0x108E, 0x00C2,
    0x1090, 0x04BE,
    0x1092, 0x00C4,
    0x105E, 0x0037,
    0x1060, 0x003A,
    0x1088, 0x0034,
    0x108A, 0x003A,
    0x1168, 0x0000,
    0x1140, 0x0A02,
    0x114C, 0x2131,
    0x114E, 0x2212,
    0x1098, 0x0451,
    0x10A4, 0x0041,
    0x10A6, 0x0045,
    0x10CE, 0x0041,
    0x104E, 0x04BE,
    0x1050, 0x00C4,
    0x1056, 0x00BF,
    0x1058, 0x00C2,
    0x1048, 0x0075,
    0x0028, 0x1589,
    0x1162, 0xD200,
    0x0C00, 0x0208,
    0x0001, 0xADBA,
    0x0002, 0xBADA,
    0x000B, 0x92AB,
    0x000E, 0x95AD,
    0x1162, 0xD200,
    0x0022, 0x8A7F,
    0x0024, 0x342A,
    0x0026, 0x257B,
    0x0012, 0xBE3E,
    0x0014, 0x3E3D,
    0x1162, 0xD200,
    0x1162, 0xD000,
    0x1800, 0x0017,
    0x0C0C, 0x0044,
    0x1822, 0x0047,
    0x0402, 0x0010,
    SENSOR_DELAY_REG, 10,
    0x0402, 0x0001,
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_2032x1524_30P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1280x960_30P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1920x1080_30P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    //======================================
    // Mode number  : mode1_10b
    // Mode name    : 5M-B
    // Features     : 30fps, 10bit, 4lane MIPI
    // External clk : 24MHz
    // Frame rate   : 30fps
    // Data I/F     : 4lane MIPI, 10bit
    // Data rate    : 672MHz
    // Row time     : 16.67us (1600 ana_clk)
    // V total      : 2002Line
    // Output mode  : 10bit
    // Readout pixel: Total = 2592x1944, active area = (0,0,2592,1944)
    //======================================
    0x0400, 0x1d80,
    0x041e, 0x0203,  //Tint take effect = 2 frames;     Gain take effect = 1 frame
    0x0800, 0x010d,
    0x0802, 0x1b0f,
    0x0804, 0x0037,
    0x0806, 0x0100,
    0x0c18, 0x3701,
    0x0c16, 0x83cc,
    0x0c08, 0x07d2,
    0x0c0e, 0x0640,
    0x0c10, 0x0320,
    0x0c0a, 0x0002,
    0x0C00, 0x0208,
    0x0c02, 0x0001,
    0x0c1a, 0x4008,
    0x0c1c, 0x07b4,
    0x0c30, 0xffff,
    0x0c34, 0x1000,
    0x0c32, 0x0000,
    0x0c42, 0x0244,
    0x1000, 0x4004,
    0x1002, 0x406c,
    0x1004, 0x40b2,
    0x1006, 0x4008,
    0x1008, 0x4070,
    0x100a, 0x40b6,
    0x100c, 0x1012,
    0x100e, 0x10be,
    0x1010, 0x4010,
    0x1012, 0x1002,
    0x1014, 0x1006,
    0x1016, 0x100a,
    0x1018, 0x1024,
    0x101a, 0x10b4,
    0x101c, 0x10b8,
    0x101e, 0x4000,
    0x1020, 0x4004,
    0x1022, 0x4008,
    0x1024, 0x1006,
    0x1026, 0x100a,
    0x1028, 0x1066,
    0x102a, 0x106e,
    0x102c, 0x1072,
    0x102e, 0x107a,
    0x1030, 0x4004,
    0x1032, 0x4008,
    0x1034, 0x4064,
    0x1036, 0x0013,
    0x1038, 0x0067,
    0x103a, 0x0065,
    0x103c, 0x00bf,
    0x103e, 0x0415,
    0x1040, 0x00bd,
    0x1046, 0x0032,
    0x1048, 0x0061,
    0x104a, 0x000a,
    0x104c, 0x0011,
    0x104e, 0x0014,
    0x1050, 0x00be,
    0x1056, 0x0000,
    0x1058, 0x0012,
    0x105a, 0x0066,
    0x105c, 0x008e,
    0x106e, 0x2055,
    0x1080, 0x0014,
    0x1082, 0x00be,
    0x1088, 0x0036,
    0x108a, 0x003a,
    0x108c, 0x0000,
    0x108e, 0x0011,
    0x1090, 0x0014,
    0x1092, 0x00be,
    0x1098, 0x0449,
    0x109a, 0x00bd,
    0x109c, 0x0000,
    0x109e, 0x0000,
    0x10a0, 0x0000,
    0x10a2, 0x0000,
    0x10a4, 0x0000,
    0x10a6, 0x0028,
    0x10b2, 0x0067,
    0x10b4, 0x0800,
    0x10b6, 0x0000,
    0x10b8, 0x0000,
    0x10ba, 0x0000,
    0x10bc, 0x0000,
    0x10be, 0x0000,
    0x10c0, 0x0800,
    0x10c2, 0x0000,
    0x10c8, 0x0800,
    0x10ca, 0x0000,
    0x10cc, 0x0437,
    0x10ce, 0x005a,
    0x10dc, 0x0039,
    0x10de, 0x003f,
    0x10f0, 0x0063,
    0x10f2, 0x0069,
    0x1100, 0x0070,
    0x1102, 0x00bc,
    0x1104, 0x0126,
    0x1106, 0x0172,
    0x1108, 0x0000,
    0x110a, 0x0000,
    0x110c, 0x0000,
    0x110e, 0x0000,
    0x1110, 0x00b8,
    0x1112, 0x00c0,
    0x1114, 0x016e,
    0x1116, 0x0176,
    0x1118, 0x0000,
    0x111a, 0x0000,
    0x111c, 0x0000,
    0x111e, 0x0000,
    0x1122, 0x2126,
    0x1124, 0x0000,
    0x1126, 0x0000,
    0x112a, 0x20d5,
    0x112c, 0x2126,
    0x112e, 0x0000,
    0x1130, 0x0000,
    0x1132, 0x0000,
    0x1134, 0x20c2,
    0x1136, 0x0000,
    0x113c, 0x0043,
    0x113e, 0x004b,
    0x1142, 0x00d8,
    0x1144, 0x0000,
    0x1146, 0x0000,
    0x1400, 0x1002,
    0x1402, 0x00a2,
    0x1404, 0x00a6,
    0x1406, 0x7011,
    0x1800, 0x1016,
    0x1c04, 0x0ca8,
    0x1c00, 0x1008,
    0x1164, 0x0000,
    0x1168, 0x0000,
    0x1166, 0x0000,
    0x1168, 0x0000,
    0x0402, 0x0080,
    0x103A, 0x0865,
    0x0020, 0x848A,
    0x0022, 0x8A20,
    0x001C, 0x82A4,
    0x001E, 0xA884,
    0x0C1C, 0x07B4,
    0x0C1E, 0x07A4,
    0x0C38, 0x0001,
    0x0C20, 0x07A8,
    0x0C22, 0x0007,
    0x0C3C, 0x0799,
    0x102A, 0x102A,
    0x1002, 0x4028,
    0x102C, 0x102E,
    0x1008, 0x402C,
    0x1080, 0x04BE,
    0x1082, 0x00C4,
    0x108C, 0x00BF,
    0x108E, 0x00C2,
    0x1090, 0x04BE,
    0x1092, 0x00C4,
    0x105E, 0x0037,
    0x1060, 0x003A,
    0x1088, 0x0034,
    0x108A, 0x003A,
    0x1168, 0x0000,
    0x1140, 0x0A02,
    0x114C, 0x2131,
    0x114E, 0x2212,
    0x1098, 0x0451,
    0x10A4, 0x0041,
    0x10A6, 0x0045,
    0x10CE, 0x0041,
    0x104E, 0x04BE,
    0x1050, 0x00C4,
    0x1056, 0x00BF,
    0x1058, 0x00C2,
    0x1048, 0x0075,
    0x0028, 0x1589,
    0x1162, 0xD200,
    0x0C00, 0x0208,
    0x0001, 0xADBA,
    0x0002, 0xBADA,
    0x000B, 0x92AB,
    0x000E, 0x95AD,
    0x1162, 0xD200,
    0x0022, 0x8A7F,
    0x0024, 0x342A,
    0x0026, 0x257B,
    0x0012, 0xBE3E,
    0x0014, 0x3E3D,
    0x1162, 0xD200,
    0x1162, 0xD000,
    0x1800, 0x0017,
    0x0C0C, 0x0044,
    0x1822, 0x0047,
    0x0402, 0x0010,
    SENSOR_DELAY_REG, 10,
    0x0402, 0x0001,
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1920x1080_50P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1920x1080_60P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1920x1080_15P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

ISP_UINT16 SNR_BRV0500_Reg_1280x720_30P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1280x720_60P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1280x720_100P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

ISP_UINT16 SNR_BRV0500_Reg_1280x720_120P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_640x480_30P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

static ISP_UINT16 SNR_BRV0500_Reg_1520x1520_30P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

//*** Scale Up 4K@24fps (Index 15)
//Reference: Index 2
static ISP_UINT16 SNR_BRV0500_Reg_2688x1512_24P[] = 
{
#if (SENSOR_IF == SENSOR_IF_MIPI_1_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_2_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#elif (SENSOR_IF == SENSOR_IF_MIPI_4_LANE)
    // TBD
    SENSOR_DELAY_REG, 100 // delay
#endif
};

// OPR Table Setting
static MMPF_SENSOR_OPR_TABLE m_OprTable = 
{
    // usInitSize
    (sizeof(SNR_BRV0500_Reg_Init_Customer)/sizeof(SNR_BRV0500_Reg_Init_Customer[0]))/2,

    // uspInitTable
    &SNR_BRV0500_Reg_Init_Customer[0],

    MMP_FALSE,  // bBinTableExist
    0,          // usBinTableNum
    {0},        // usBinRegAddr
    {0},        // usBinSize
    {NULL},     // ubBinTable

    MMP_FALSE,  // bInitDoneTableExist
    0,          // usInitDoneSize
    NULL,       // uspInitDoneTable

    // usSize
    {
        (sizeof(SNR_BRV0500_Reg_2432x1368_60P_HDR)/sizeof(SNR_BRV0500_Reg_2432x1368_60P_HDR[0]))/2,
        (sizeof(SNR_BRV0500_Reg_2688x1520_60P)/sizeof(SNR_BRV0500_Reg_2688x1520_60P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_2688x1520_30P)/sizeof(SNR_BRV0500_Reg_2688x1520_30P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1280x960_30P)/sizeof(SNR_BRV0500_Reg_1280x960_30P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1920x1080_30P)/sizeof(SNR_BRV0500_Reg_1920x1080_30P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1920x1080_50P)/sizeof(SNR_BRV0500_Reg_1920x1080_50P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1920x1080_60P)/sizeof(SNR_BRV0500_Reg_1920x1080_60P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_2032x1524_30P)/sizeof(SNR_BRV0500_Reg_2032x1524_30P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1920x1080_15P)/sizeof(SNR_BRV0500_Reg_1920x1080_15P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1280x720_30P)/sizeof(SNR_BRV0500_Reg_1280x720_30P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1280x720_60P)/sizeof(SNR_BRV0500_Reg_1280x720_60P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1280x720_100P)/sizeof(SNR_BRV0500_Reg_1280x720_100P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1280x720_120P)/sizeof(SNR_BRV0500_Reg_1280x720_120P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_640x480_30P)/sizeof(SNR_BRV0500_Reg_640x480_30P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_1520x1520_30P)/sizeof(SNR_BRV0500_Reg_1520x1520_30P[0]))/2,
        (sizeof(SNR_BRV0500_Reg_2688x1512_24P)/sizeof(SNR_BRV0500_Reg_2688x1512_24P[0]))/2,
    },

    // uspTable
    {
        &SNR_BRV0500_Reg_2432x1368_60P_HDR[0],
        &SNR_BRV0500_Reg_2688x1520_60P[0],
        &SNR_BRV0500_Reg_2688x1520_30P[0],
        &SNR_BRV0500_Reg_1280x960_30P[0],
        &SNR_BRV0500_Reg_1920x1080_30P[0],
        &SNR_BRV0500_Reg_1920x1080_50P[0],
        &SNR_BRV0500_Reg_1920x1080_60P[0],
        &SNR_BRV0500_Reg_2032x1524_30P[0],
        &SNR_BRV0500_Reg_1920x1080_15P[0],
        &SNR_BRV0500_Reg_1280x720_30P[0],
        &SNR_BRV0500_Reg_1280x720_60P[0],
        &SNR_BRV0500_Reg_1280x720_100P[0],
        &SNR_BRV0500_Reg_1280x720_120P[0],
        &SNR_BRV0500_Reg_640x480_30P[0],
        &SNR_BRV0500_Reg_1520x1520_30P[0],
        &SNR_BRV0500_Reg_2688x1512_24P[0]
    }
};

#if 0
void ____Sensor_Customer_Func____(){ruturn;} // dummy
#endif

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_InitConfig
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_InitConfig(void)
{
    RTNA_DBG_Str(0, "SNR_Cust_InitConfig BRV0500\r\n");
    
    if ((gsHdrCfg.bVidEnable || gsHdrCfg.bDscEnable) && 
        (gsHdrCfg.ubMode == HDR_MODE_STAGGER))
    {
        m_VifSetting_Prm.vcAttr.bEnable             = MMP_TRUE;
        m_VifSetting_Prm.vcAttr.bAllChannel2Isp     = MMP_TRUE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[0]          = MMP_TRUE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[1]          = MMP_TRUE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[2]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[3]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[0]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[1]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[2]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[3]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bSlowFsForStagger   = MMP_TRUE;
    }
    else {
        m_VifSetting_Prm.vcAttr.bEnable             = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bAllChannel2Isp     = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[0]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[1]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[2]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Isp[3]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[0]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[1]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[2]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bVC2Raw[3]          = MMP_FALSE;
        m_VifSetting_Prm.vcAttr.bSlowFsForStagger   = MMP_FALSE;
    }  
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_DoAE_FrmSt
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_DoAE_FrmSt(MMP_UBYTE ubSnrSel, MMP_ULONG ulFrameCnt)
{
#if 0//(ISP_EN)
    MMP_ULONG   ulVsync = 0;
    MMP_ULONG   ulShutter = 0;
    MMP_ULONG   ulSetShutCurISPFrameCnt;
    MMP_ULONG   ulSetGainDoneCurISPFrameCnt;
    
    if ((gsHdrCfg.bVidEnable || gsHdrCfg.bDscEnable) && 
        (gsHdrCfg.ubMode == HDR_MODE_STAGGER))
    {
        #define SENSOR_GROUP_LAUNCH     (1)
        
        if ((ulFrameCnt % m_AeTime.ubPeriod) == 0)
        {
            ISP_IF_AE_Execute();

            // Get long exposure
            ae_shutter_l = ISP_IF_AE_GetShutter();
            ae_gain_l    = ISP_IF_AE_GetGain();        

            ae_shutter_s = ISP_IF_IQ_GetHDR_ShortShutter();
            ae_gain_s    = ISP_IF_IQ_GetHDR_ShortGain();

            if (ae_gain_l >= ISP_IF_AE_GetGainBase() * MAX_SENSOR_GAIN) {
               dgain      = ISP_DGAIN_BASE * ae_gain_l / ISP_IF_AE_GetGainBase() / MAX_SENSOR_GAIN;
               ae_gain_l  = ISP_IF_AE_GetGainBase() * MAX_SENSOR_GAIN;
            }
            else {
                dgain = ISP_DGAIN_BASE;
            }

            ae_gain_s = ae_gain_s * ISP_DGAIN_BASE / dgain;
            
            // Sensor limit
            if (ae_shutter_l <= ae_shutter_s)   
                ae_shutter_l = ae_shutter_s + 1;
        }
        
        // For AE curve
        if (ulFrameCnt % 100 == 10) {
            #ifdef HB_IQ_VER    // temp, For DVR002
            if ((HB_IQ_VER == 4) || (HB_IQ_VER == 5))
                ISP_IF_CMD_SendCommandtoAE(0x52, AE_Bias_tbl, AE_tbl_size, 1);
            else
                ISP_IF_CMD_SendCommandtoAE(0x51, AE_Bias_tbl, AE_tbl_size, 0);
            #else
            ISP_IF_CMD_SendCommandtoAE(0x51, AE_Bias_tbl, AE_tbl_size, 0); // <<AE table set once at preview start
            #endif
            ISP_IF_NaturalAE_Enable(2);	//0: no , 1:ENERGY 2: Lux 3: test mode
        }
		#if (SENSOR_GROUP_LAUNCH)
		gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3208, 0x0);  //group 0 start
        #endif
        
        // Set shutter
        if ((ulFrameCnt % m_AeTime.ubPeriod) == m_AeTime.ubFrmStSetShutFrmCnt) {
            gsSensorFunction->MMPF_Sensor_SetShutter(ubSnrSel, 0, 0);    
        }
        
        // Set gain
        if ((ulFrameCnt % m_AeTime.ubPeriod) == m_AeTime.ubFrmStSetGainFrmCnt) {
            gsSensorFunction->MMPF_Sensor_SetGain(ubSnrSel, 0);
       	}

       	ISP_IF_IQ_SetAEGain(dgain, ISP_DGAIN_BASE);

        #if (SENSOR_GROUP_LAUNCH)
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3208, 0x10); //group 0 end
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x320B, 0x00); //group 0 manual launch on
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3208, 0xA0); //group 0 delay launch
        #endif
    }
    else
    {
        extern MMP_ULONG  m_ulISPFrameCount;
        static ISP_UINT16 ae_gain;
        static ISP_UINT16 ae_shutter;
        static ISP_UINT16 ae_vsync;
        
        ISP_IF_CMD_SendCommandtoAE(0x52, AE_Bias_tbl, AE_tbl_size, 1);
        
        if ((ulFrameCnt % m_AeTime.ubPeriod) == 0)
        {
            ISP_IF_AE_Execute();
            
            ae_gain     = ISP_IF_AE_GetGain();
            ae_shutter  = ISP_IF_AE_GetShutter();
            ae_vsync    = ISP_IF_AE_GetVsync();
        }

        ulVsync 	= (gSnrLineCntPerSec[ubSnrSel] * ISP_IF_AE_GetVsync()) / ISP_IF_AE_GetVsyncBase();
        ulShutter 	= (gSnrLineCntPerSec[ubSnrSel] * ISP_IF_AE_GetShutter()) / ISP_IF_AE_GetShutterBase();

        if ((ulFrameCnt % m_AeTime.ubPeriod) == m_AeTime.ubFrmStSetGainFrmCnt)
       	{
            if (ae_gain >= ISP_IF_AE_GetGainBase() * MAX_SENSOR_GAIN) {

                dgain  	= ISP_DGAIN_BASE * ae_gain /ISP_IF_AE_GetGainBase() / MAX_SENSOR_GAIN ;
                ae_gain	= ISP_IF_AE_GetGainBase() * MAX_SENSOR_GAIN;
            }
            else {
                dgain 	= ISP_DGAIN_BASE;
            }
        }
        
        if (CAM_CHECK_SCD(SCD_CAM_BAYER_SENSOR)) {
            if ((ulFrameCnt % m_AeTime.ubPeriod) == m_AeTime.ubFrmStSetShutFrmCnt) {

                gsSensorFunction->MMPF_Sensor_SetExposure(PRM_SENSOR, ae_gain, ulShutter, ulVsync);
                gsSensorFunction->MMPF_Sensor_SetExposure(SCD_SENSOR, ae_gain, ulShutter, ulVsync);
            }
        }
        else {
        
            if ((ulFrameCnt % m_AeTime.ubPeriod) == m_AeTime.ubFrmStSetShutFrmCnt) {
                
                ulSetShutCurISPFrameCnt = m_ulISPFrameCount;
                gsSensorFunction->MMPF_Sensor_SetShutter(ubSnrSel, ulShutter, ulVsync);
            }
            
            if ((ulFrameCnt % m_AeTime.ubPeriod) == m_AeTime.ubFrmStSetGainFrmCnt) {
                
                gsSensorFunction->MMPF_Sensor_SetGain(ubSnrSel, ae_gain);
                ulSetGainDoneCurISPFrameCnt = m_ulISPFrameCount;
                
                if (ulSetShutCurISPFrameCnt != ulSetGainDoneCurISPFrameCnt) {
                    RTNA_DBG_Str0("Set AE and Shutter over ");
                    RTNA_DBG_Byte0(ulSetGainDoneCurISPFrameCnt - ulSetShutCurISPFrameCnt);
                    RTNA_DBG_Str0(" frame duration\r\n");
                }
            }
        }

        if ((ulFrameCnt % m_AeTime.ubPeriod) == m_AeTime.ubFrmStSetGainFrmCnt + 1) {
            ISP_IF_IQ_SetAEGain(dgain, ISP_DGAIN_BASE);
        }
    }
#endif
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_DoAE_FrmEnd
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_DoAE_FrmEnd(MMP_UBYTE ubSnrSel, MMP_ULONG ulFrameCnt)
{
    // TBD
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_DoAWB_FrmSt
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_DoAWB_FrmSt(MMP_UBYTE ubSnrSel, MMP_ULONG ulFrameCnt)
{
    // TBD
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_DoAWB_FrmEnd
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_DoAWB_FrmEnd(MMP_UBYTE ubSnrSel, MMP_ULONG ulFrameCnt)
{
    // TBD
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_DoIQ
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_DoIQ(MMP_UBYTE ubSnrSel, MMP_ULONG ulFrameCnt)
{
	// TBD
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_SetGain
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_SetGain(MMP_UBYTE ubSnrSel, MMP_ULONG ulGain)
{
#if 0//(ISP_EN)

     static MMP_UBYTE temp_sensor_H = 0;
     static MMP_UBYTE temp_sensor_L = 0;
     
    if ((gsHdrCfg.bVidEnable || gsHdrCfg.bDscEnable) && 
        (gsHdrCfg.ubMode == HDR_MODE_STAGGER))
    {
        ISP_UINT16  sensor_H;
        ISP_UINT16  sensor_L;
        MMP_ULONG   ulTempGain;

        ulTempGain = ae_gain_l;
        
        /* Sensor Gain Mapping */
        if ((ulTempGain >> 8) < 2)
        {
            sensor_H = 0;
            sensor_L = (ulTempGain * 128) >> 8;                 // 1X ~ 2X
        }
        else if ((ulTempGain >> 8) < 4)
        {
            sensor_H = 1;   
            sensor_L = ((ulTempGain * 64) - (256 * 8)) >> 8;    // 2X ~ 4X
        }       
        else if ((ulTempGain >> 8) < 8)
        {
            sensor_H = 3;   
            sensor_L = ((ulTempGain * 32) - (256 * 12)) >> 8;   // 4X ~ 8X
        }   
        else
        {
            sensor_H = 7;
            sensor_L = ((ulTempGain * 16) - (256 * 8)) >> 8;    // 8X ~16X
        }

        // Long Gain
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3508, sensor_H);
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3509, sensor_L);

        ulTempGain = ae_gain_s;

        /* Sensor Gain Mapping */
        if ((ulTempGain >> 8) < 2)
        {
            sensor_H = 0;
            sensor_L = (ulTempGain * 128) >> 8;                 // 1X ~ 2X
        }
        else if ((ulTempGain >> 8) < 4)
        {
            sensor_H = 1;   
            sensor_L = ((ulTempGain * 64) - (256 * 8)) >> 8;    // 2X ~ 4X
        }       
        else if ((ulTempGain >> 8) < 8)
        {
            sensor_H = 3;   
            sensor_L = ((ulTempGain * 32) - (256 * 12)) >> 8;   // 4X ~ 8X
        }   
        else
        {
            sensor_H = 7;
            sensor_L = ((ulTempGain * 16) - (256 * 8)) >> 8;    // 8X ~16X
        }
        
        // Middle Gain
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x350E, sensor_H);
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x350F, sensor_L);
    }
    else
    {
        ISP_UINT16 sensor_H;
        ISP_UINT16 sensor_L;
        ISP_UINT16 array[4];
        
        /* Sensor Gain Mapping */
        if ((ulGain>>8) < 2)
        {
            sensor_H = 0;
            sensor_L = (ulGain * 128) >> 8;                 // 1X ~ 2X
        }
        else if ((ulGain >> 8) < 4)
        {
            sensor_H = 1;   
            sensor_L = ((ulGain * 64) - (256 * 8)) >> 8;    // 2X ~ 4X
        }       
        else if ((ulGain >> 8) < 8)
        {
            sensor_H = 3;   
            sensor_L = ((ulGain * 32) - (256 * 12)) >> 8;   // 4X ~ 8X
        }   
        else
        {
            sensor_H = 7;
            sensor_L = ((ulGain * 16) - (256 * 8)) >> 8;    // 8X ~16X
        }

        array[0] = 0x3508;
        array[1] = sensor_H;
        array[2] = 0x3509;
        array[3] = sensor_L;

        if(temp_sensor_H == sensor_H && temp_sensor_L == sensor_L) return;
        
        if (CAM_CHECK_SCD(SCD_CAM_BAYER_SENSOR)) {
            gsSensorFunction->MMPF_Sensor_SetRegArray(ubSnrSel, array, ARRAY_SIZE(array)/2);
        }
        else {
            gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3508, sensor_H);
            gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3509, sensor_L);
        }

        temp_sensor_H = sensor_H;
        temp_sensor_L = sensor_L;
    }
#endif
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_SetShutter
//  Description :
//------------------------------------------------------------------------------
void SNR_Cust_SetShutter(MMP_UBYTE ubSnrSel, MMP_ULONG shutter, MMP_ULONG vsync)
{
#if 0//(ISP_EN)

       static ISP_UINT32 temp_vysnc = 0;
       static ISP_UINT32 temp_shutter = 0;
       
       static ISP_UINT32 hdr_temp_shutter_Long = 0;
       static ISP_UINT32 hdr_temp_shutter_Short = 0;
     
    if ((gsHdrCfg.bVidEnable || gsHdrCfg.bDscEnable) && 
        (gsHdrCfg.ubMode == HDR_MODE_STAGGER))
    {
        // Long Expo
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3500, (ISP_UINT8)((ae_shutter_l >> 12) & 0xFF));
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3501, (ISP_UINT8)((ae_shutter_l >> 4) & 0xFF));
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3502, (ISP_UINT8)((ae_shutter_l << 4) & 0xFF));
        
        // Middle Expo
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x350A, (ISP_UINT8)((ae_shutter_s >> 12) & 0xFF));
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x350B, (ISP_UINT8)((ae_shutter_s >> 4) & 0xFF));
        gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x350C, (ISP_UINT8)((ae_shutter_s << 4) & 0xFF));
        
        hdr_temp_shutter_Long = ae_shutter_l;
        hdr_temp_shutter_Short = ae_shutter_s;
    }
    else
    {
        ISP_UINT32 new_vsync;
        ISP_UINT32 new_shutter;
        ISP_UINT16 array[10];
        
        if ((shutter == 0 ) || (vsync == 0))
        {
            new_vsync    = gSnrLineCntPerSec[ubSnrSel] * ISP_IF_AE_GetVsync() / ISP_IF_AE_GetVsyncBase();
            new_shutter  = gSnrLineCntPerSec[ubSnrSel] * ISP_IF_AE_GetShutter() / ISP_IF_AE_GetShutterBase();
        }
        else
        {
            new_vsync   = vsync;
            new_shutter = shutter;
        }
        new_vsync   = ISP_MIN(ISP_MAX((new_shutter), new_vsync), 0xFFFF);
        new_shutter = ISP_MIN(ISP_MAX(new_shutter, 1), (new_vsync - 4));
        
        if(temp_vysnc == new_vsync && temp_shutter == new_shutter) return;

        array[0] = 0x380E;
        array[1] = (new_vsync >> 8);
        array[2] = 0x380F;
        array[3] = new_vsync;
        
        array[4] = 0x3500;
        array[5] = (ISP_UINT8)((new_shutter >> 12) & 0xFF);
        array[6] = 0x3501;
        array[7] = (ISP_UINT8)((new_shutter >> 4) & 0xFF);
        array[8] = 0x3502;
        array[9] = (ISP_UINT8)((new_shutter << 4) & 0xFF);
        
        if (CAM_CHECK_SCD(SCD_CAM_BAYER_SENSOR)) {
            gsSensorFunction->MMPF_Sensor_SetRegArray(ubSnrSel, array, ARRAY_SIZE(array)/2);    
        }
        else {
            gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x380E, (new_vsync >> 8));
            gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x380F, new_vsync);

            gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3500, (ISP_UINT8)((new_shutter >> 12) & 0xFF));
            gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3501, (ISP_UINT8)((new_shutter >> 4) & 0xFF));
            gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3502, (ISP_UINT8)((new_shutter << 4) & 0xFF));
        }

        temp_vysnc = new_vsync;
        temp_shutter = new_shutter;
    }
#endif
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_SetExposure
//  Description :
//------------------------------------------------------------------------------
static void SNR_Cust_SetExposure(MMP_UBYTE ubSnrSel, MMP_ULONG ulGain, MMP_ULONG shutter, MMP_ULONG vsync)
{
#if 0//(ISP_EN)
    if ((gsHdrCfg.bVidEnable || gsHdrCfg.bDscEnable) && 
        (gsHdrCfg.ubMode == HDR_MODE_STAGGER))
    {
        // TBD
    }
    else
    {
        ISP_UINT32 new_vsync;
        ISP_UINT32 new_shutter;
        ISP_UINT16 sensor_H;
        ISP_UINT16 sensor_L;
        ISP_UINT16 array[14];

        // Gain Setting
        if ((ulGain>>8) < 2)
        {
            sensor_H = 0;
            sensor_L = (ulGain * 128) >> 8;                 // 1X ~ 2X
        }
        else if ((ulGain >> 8) < 4)
        {
            sensor_H = 1;
            sensor_L = ((ulGain * 64) - (256 * 8)) >> 8;    // 2X ~ 4X
        }
        else if ((ulGain >> 8) < 8)
        {
            sensor_H = 3;
            sensor_L = ((ulGain * 32) - (256 * 12)) >> 8;   // 4X ~ 8X
        }
        else
        {
            sensor_H = 7;
            sensor_L = ((ulGain * 16) - (256 * 8)) >> 8;    // 8X ~16X
        }

        array[0] = 0x3508;
        array[1] = sensor_H;
        array[2] = 0x3509;
        array[3] = sensor_L;
       
        // Shutter Setting
        new_vsync   = ISP_MIN(ISP_MAX((shutter), vsync), 0xFFFF);
        new_shutter = ISP_MIN(ISP_MAX(shutter, 1), (vsync - 4));
        
        array[4] = 0x380E;
        array[5] = (new_vsync >> 8);
        array[6] = 0x380F;
        array[7] = new_vsync;
        
        array[8] = 0x3500;
        array[9] = (ISP_UINT8)((new_shutter >> 12) & 0xFF);
        array[10] = 0x3501;
        array[11] = (ISP_UINT8)((new_shutter >> 4) & 0xFF);
        array[12] = 0x3502;
        array[13] = (ISP_UINT8)((new_shutter << 4) & 0xFF);
        
        // Set data via I2C
        gsSensorFunction->MMPF_Sensor_SetRegArray(ubSnrSel, array, ARRAY_SIZE(array)/2);
    }
#endif
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_SetFlip
//  Description :
//------------------------------------------------------------------------------
void SNR_Cust_SetFlip(MMP_UBYTE ubSnrSel, MMP_UBYTE ubMode)
{
    #if 0
    MMP_USHORT  usRdVal1, usRdVal2;

    #define SENSOR_FLIP_MASK_BITS (0x06)
    
    gsSensorFunction->MMPF_Sensor_GetReg(ubSnrSel, 0x3820, &usRdVal1);//v
    gsSensorFunction->MMPF_Sensor_GetReg(ubSnrSel, 0x3821, &usRdVal2);//h
    
    if (ubMode == MMPF_SENSOR_NO_FLIP) 
    {
        usRdVal1 &= ~SENSOR_FLIP_MASK_BITS;
        usRdVal2 |= SENSOR_FLIP_MASK_BITS;
        m_VifSetting_Prm.colorId.VifColorId = MMPF_VIF_COLORID_11;
    }
    else if (ubMode == MMPF_SENSOR_COLUMN_FLIP)
    {
        usRdVal1 |= SENSOR_FLIP_MASK_BITS;
        usRdVal2 |= SENSOR_FLIP_MASK_BITS;
        m_VifSetting_Prm.colorId.VifColorId = MMPF_VIF_COLORID_10;
    }
    else if (ubMode == MMPF_SENSOR_ROW_FLIP)
    {
        usRdVal1 &= ~SENSOR_FLIP_MASK_BITS;
        usRdVal2 &= ~SENSOR_FLIP_MASK_BITS;
        m_VifSetting_Prm.colorId.VifColorId = MMPF_VIF_COLORID_01;
    }
    else if (ubMode == MMPF_SENSOR_COLROW_FLIP)
    {
        usRdVal1 |= SENSOR_FLIP_MASK_BITS;
        usRdVal2 &= ~SENSOR_FLIP_MASK_BITS;
        m_VifSetting_Prm.colorId.VifColorId = MMPF_VIF_COLORID_00;
    }
    else {
        return;
    }

    gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3820, usRdVal1);
    gsSensorFunction->MMPF_Sensor_SetReg(ubSnrSel, 0x3821, usRdVal2);
    #endif
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_SetRotate
//  Description :
//------------------------------------------------------------------------------
void SNR_Cust_SetRotate(MMP_UBYTE ubSnrSel, MMP_UBYTE ubMode)
{
    // TBD
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_CheckVersion
//  Description :
//------------------------------------------------------------------------------
void SNR_Cust_CheckVersion(MMP_UBYTE ubSnrSel, MMP_ULONG *pulVersion)
{
    // TBD
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_GetIqCompressData
//  Description :
//------------------------------------------------------------------------------
const MMP_UBYTE* SNR_Cust_GetIqCompressData(MMP_UBYTE ubSnrSel)
{
    return s_IqCompressData;
}

//------------------------------------------------------------------------------
//  Function    : SNR_Cust_StreamEnable
//  Description : Enable/Disable streaming of sensor
//------------------------------------------------------------------------------
static void SNR_Cust_StreamEnable(MMP_UBYTE ubSnrSel, MMP_BOOL bEnable)
{
    // TBD
}

MMPF_SENSOR_CUSTOMER SensorCustFunc_Prm = 
{
    SNR_Cust_InitConfig,
    SNR_Cust_DoAE_FrmSt,
    SNR_Cust_DoAE_FrmEnd,
    SNR_Cust_DoAWB_FrmSt,
    SNR_Cust_DoAWB_FrmEnd,
    SNR_Cust_DoIQ,
    SNR_Cust_SetGain,
    SNR_Cust_SetShutter,
    SNR_Cust_SetExposure,
    SNR_Cust_SetFlip,
    SNR_Cust_SetRotate,
    SNR_Cust_CheckVersion,
    SNR_Cust_GetIqCompressData,
    SNR_Cust_StreamEnable,
    
    &m_SensorRes,
    &m_OprTable,
    &m_VifSetting_Prm,
    &m_I2cmAttr_Prm,
    &m_AwbTime,
    &m_AeTime,
    &m_AfTime,
    MMP_SNR_PRIO_PRM
};

int SNR_Module_Init(void)
{
    if (SensorCustFunc_Prm.sPriority == MMP_SNR_PRIO_PRM)
        MMPF_SensorDrv_Register(PRM_SENSOR, &SensorCustFunc_Prm);
    else
        MMPF_SensorDrv_Register(SCD_SENSOR, &SensorCustFunc_Prm);
    return 0;
}

#pragma arm section code = "initcall6", rodata = "initcall6", rwdata = "initcall6",  zidata = "initcall6" 
#pragma O0
ait_module_init(SNR_Module_Init);
#pragma
#pragma arm section rodata, rwdata, zidata

#endif // (BIND_SENSOR_BRV0500)
#endif // (SENSOR_EN)
