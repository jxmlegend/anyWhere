//==============================================================================
//
//  File        : AHC_Gsensor.h
//  Description : INCLUDE File for the AHC gsensor control function porting.
//  Author      : 
//  Revision    : 1.0
//
//==============================================================================

#ifndef _AHC_GSENSOR_H_
#define _AHC_GSENSOR_H_

/*===========================================================================
 * Include files
 *===========================================================================*/ 
#include "AHC_Common.h"
#include "AHC_General.h" 

/*===========================================================================
 * 
 *===========================================================================*/ 
#define POLLING_TYPE_NOEN				(0x00)
#define MOVE_BY_ACCELERATION			(0x01)
#define MOVE_BY_INTERUPT				(0x02)
#define Z_AXIS_GRAVITY_CAHNGE			(0x04)	

#define AHC_GSENSOR_RET_FALSE (AHC_FALSE)
#define AHC_GSENSOR_RET_TRUE (AHC_TRUE)
#define AHC_GSENSOR_NO_ERROR (MMP_ERR_NONE)
#define AHC_GSENSOR_CHECK_RETURE_VALUE(RET_VAL, NO_ERROR, YES, NO) return (RET_VAL == NO_ERROR) ? YES : NO;

/*===========================================================================
 * Enum define
 *===========================================================================*/ 
typedef enum{

    GSENSOR_XYZ = 0,
    GSENSOR_X,
    GSENSOR_Y,
    GSENSOR_Z
}AHC_GSENSOR_COORDINATES;

typedef enum _AHC_GSENSOR_I2CM_ID
{
    AHC_GSENSOR_I2CM_ID_0 = 0,   	//HW I2CM, reserved for Sensor HW I2C
    /* Re-order due to the I2CM register map */
    AHC_GSENSOR_I2CM_ID_2,
    AHC_GSENSOR_I2CM_ID_1,
    AHC_GSENSOR_I2CM_ID_3,
    AHC_GSENSOR_I2CM_ID_SW,			//SW I2CM, using GPIO pins
    AHC_GSENSOR_I2CM_ID_SW_SENSOR,	//SW I2CM, sensor uses VIF pins
    AHC_GSENSOR_I2CM_MAX_COUNT
} AHC_GSENSOR_I2CM_ID;
/*===========================================================================
 * Structure define
 *===========================================================================*/
typedef struct
{
	UINT8 	acc_val[3];	//X,Y,Z axis acceleration value	
}AHC_GSENSOR_DATA;



typedef struct _AHC_GSENSOR_I2CM_ATTRIBUTE
{
   AHC_GSENSOR_I2CM_ID i2cmID;      //MMP_I2CM0 ~ MMP_I2CM2 stand for HW I2CM
   UINT8 	 	ubPadNum;      		//HW pad map, the relate pad definition, please refer global register spec.
   UINT32	 	ulI2cmSpeed;        //SW and HW I2CM speed control, the unit is HZ.
  
} AHC_GSENSOR_I2CM_ATTRIBUTE;

/*===========================================================================
 * Function prototype
 *===========================================================================*/
AHC_BOOL AHC_Gsensor_Module_Attached(void);
AHC_BOOL AHC_Gsensor_Initial(void);
AHC_BOOL AHC_Gsensor_ReadRegister(UINT16 reg , UINT16* pval);
AHC_BOOL AHC_Gsensor_WriteRegister(UINT16 reg , UINT16 pval);
AHC_BOOL AHC_Gsensor_ReadOffsetFromFile(void);
AHC_BOOL AHC_Gsensor_WriteOffsetToFile(void);
AHC_BOOL AHC_Gsensor_ResetDevice(void);
AHC_BOOL AHC_Gsensor_ReadDeviceID(void);
AHC_BOOL AHC_Gsensor_GetSensorOffset(UINT8 *data);
AHC_BOOL AHC_Gsensor_SetSensorOffset(UINT8 *data);
AHC_BOOL AHC_Gsensor_Gsensor_Read_XYZ(AHC_GSENSOR_DATA *data);
AHC_BOOL AHC_Gsensor_Read_XYZ_WithCompensation(AHC_GSENSOR_DATA *data);
AHC_BOOL AHC_Gsensor_Read_Accel_Avg(AHC_GSENSOR_DATA *data);
AHC_BOOL AHC_Gsensor_Calibration(UINT8 side);
AHC_BOOL AHC_Gsensor_SetIntDuration(UINT8 arg);
AHC_BOOL AHC_Gsensor_GetIntStatus(UINT8* pstatus);
AHC_BOOL AHC_Gsensor_SetPowerSaveMode(UINT8 mode);
AHC_BOOL AHC_Gsensor_SetNormalModeDataRate(UINT8 rate);
AHC_BOOL AHC_Gsensor_SetInterruptSource(UINT8 src);
AHC_BOOL AHC_Gsensor_GetDataRate(UINT16 *wDataRate);
AHC_BOOL AHC_Gsensor_SetDynamicRange(UINT8 ubDrange);
AHC_BOOL AHC_Gsensor_GetDynamicRange(UINT8 *ubDrange);
AHC_BOOL AHC_Gsensor_SetGroundAxis(UINT8 arg);
AHC_BOOL AHC_Gsensor_IOControl(UINT8 cmd, void* arg);

AHC_BOOL AHC_Gsensor_SetCurInfo(void);
AHC_BOOL AHC_Gsensor_SetIntThreshold(void);
AHC_BOOL AHC_Gsensor_Polling(UINT8 ubType);
AHC_BOOL AHC_Gsensor_Module_Attached(void);

#endif // _AHC_GSENSOR_H_
