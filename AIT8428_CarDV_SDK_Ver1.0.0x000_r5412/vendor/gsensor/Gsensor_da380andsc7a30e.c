/*===========================================================================
 * Include file 
 *===========================================================================*/ 

#include "Customer_Config.h"
#include "config_fw.h"
#include "mmpf_i2cm.h"
#include "os_wrap.h"
#include "mmpf_pio.h"
#include "mmpf_timer.h"
#include "lib_retina.h"
#include "AHC_utility.h"
#include "AHC_Fs.h"
#include "dsc_Key.h"

#if (GSENSOR_CONNECT_ENABLE && GSENSOR_MODULE == GSENSOR_MODULE_DA380ANDSC7A30E)
#include "Gsensor_da380andsc7a30e.h"
#include "mmpf_gsensor.h"
#include "Gsensor_Ctrl.h"
#include "AIT_Init.h"

typedef unsigned short u16;
typedef short i16;
typedef unsigned char u8;

/*===========================================================================
 * Macro define
 *===========================================================================*/

#define	TO_NEGATIVE_VALUE(x)	( (~x + 0x01) | 0x200 )
#define	TO_ABSOLUTE_VALUE(x)	( (x > 0x200)? (0x400 - x):(x) )

#define   TO_XXXX_VALUE(x)   (((x)> 0x20)?((~((x)&(~0x20)))^0x1f):((x)))

#define	LOW_BOUND_1G(s)			((signed char)s->ub1G - (signed char)s->ub1G_Err)
#define ABS(x) 					((x) > 0 ? (x) : -(x))

#define	MAP2HW_G(g, s)			((g * s->ub1G) / SCALE_G)

#define	STABLE_CNT				(0)

/*===========================================================================
 * Global varible
 *===========================================================================*/

static GSENSOR_DATA			m_GSensorOffset 		= {0, 0, 0};
#if (GSENSOR_DETECT_MODE == GSNESOR_DETECT_DIFF)
static MMP_BOOL	 			bGsensorCalibrated   	= MMP_FALSE;
#endif
MMP_USHORT	ApicalDeviceID = 0x00;
static int					kfirstread 				= 0;

static int					kUpCnt 					= 0;
static int					kDwCnt 					= 0;
static int					kLastPosition 			= GSNR_POSITION_UNKNOWN;
// I2cm Attribute
static MMP_I2CM_ATTR m_I2cmAttr_gsensor_sc7a30e = {
	MMP_I2CM1,			//i2cmID
	GSENSOR_SLAVE_ADDR_SC7A30E >> 1,	//ubSlaveAddr
	8,//8, 					//ubRegLen
	8, 					//ubDataLen
	0x2, 				//ubDelayTime
	MMP_FALSE, 			//bDelayWaitEn
	MMP_FALSE, 			//bInputFilterEn
	MMP_FALSE, 			//b10BitModeEn
	MMP_FALSE, 			//bClkStretchEn
	0, 					//ubSlaveAddr1
	0, 					//ubDelayCycle
	1, 					//ubPadNum
	200000 /*200KHZ*/, 	//ulI2cmSpeed
	MMP_TRUE, 			//bOsProtectEn
	GSENSOR_SCL, 		//sw_clk_pin
	GSENSOR_SDA, 		//sw_data_pin
	MMP_FALSE,			//bRfclModeEn 
	MMP_FALSE,			//bWfclModeEn
	MMP_FALSE,			//bRepeatModeEn
	0                   //ubVifPioMdlId
};
static MMP_I2CM_ATTR m_I2cmAttr_gsensor = {
	MMP_I2CM1,			//i2cmID
	GSENSOR_SLAVE_ADDR >> 1,	//ubSlaveAddr
	8,//8, 					//ubRegLen
	8, 					//ubDataLen
	0x2, 				//ubDelayTime
	MMP_FALSE, 			//bDelayWaitEn
	MMP_FALSE, 			//bInputFilterEn
	MMP_FALSE, 			//b10BitModeEn
	MMP_FALSE, 			//bClkStretchEn
	0, 					//ubSlaveAddr1
	0, 					//ubDelayCycle
	1, 					//ubPadNum
	200000 /*200KHZ*/, 	//ulI2cmSpeed
	MMP_TRUE, 			//bOsProtectEn
	GSENSOR_SCL, 		//sw_clk_pin
	GSENSOR_SDA, 		//sw_data_pin
	MMP_FALSE,			//bRfclModeEn 
	MMP_FALSE,			//bWfclModeEn
	MMP_FALSE,			//bRepeatModeEn
	0                   //ubVifPioMdlId
};


/*===========================================================================
 * Extern function
 *===========================================================================*/

#if (GSENSOR_DBG_MSG && defined(ALL_FW))

extern void	printc( char* szFormat, ... );
#define	DPRINT( ... ) printc( __VA_ARGS__ )

#else

//#define	DPRINT( ... ) {}
#define	DPRINT( ... ) printc( __VA_ARGS__ )
#endif

MMP_ERR Gsensor_EnableIntXYZ(int ex, int ey, int ez);

/*===========================================================================
 * Main body
 *===========================================================================*/\
#ifdef ALL_FW

 MMP_BOOL SC7A30E_Gsensor_Probe(void)
{
	MMP_ERR 		status = -1;
	MMP_USHORT 	usdeviceID 	= 0;

	MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor_sc7a30e, GSensor_Sc7a30e_Reg_ChipID, (MMP_USHORT*)&usdeviceID);
    	if(usdeviceID == GSENSOR_ID_SC7A30E)
        	return MMP_TRUE;

	return MMP_FALSE;
}
 MMP_BOOL DA312_Gsensor_Probe(void)
{
	MMP_ERR 		status = -1;
	MMP_USHORT 	usdeviceID 	= 0;

	MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor, GSensor_Reg_ChipID, (MMP_USHORT*)&usdeviceID);
    	if(usdeviceID == GSENSOR_ID_DA312)
        	return MMP_TRUE;

	return MMP_FALSE;
}

MMP_ERR Gsensor_Config_I2CM(struct _3RD_PARTY_GSENSOR *pthis, GSNR_I2CM_ATTRIBUTE *psI2cmattr)
{
	m_I2cmAttr_gsensor.i2cmID 		= psI2cmattr->i2cmID;
	m_I2cmAttr_gsensor.ubPadNum 	= psI2cmattr->ubPadNum;
	m_I2cmAttr_gsensor.ulI2cmSpeed  = psI2cmattr->ulI2cmSpeed;

    return MMP_ERR_NONE;
}

MMP_ERR Gsensor_ReadRegister(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE reg , MMP_UBYTE* pval)
{
	MMP_USHORT data;

	if (reg >= GSensor_Reg_INVALID)
	{
		DPRINT("GsnrRead-->Wrong Register Address\r\n");
		return MMP_ERR_NONE;
	}
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor, (MMP_USHORT)reg, (MMP_USHORT*)&data);
	}
	else
	{
		MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor_sc7a30e, (MMP_USHORT)reg, (MMP_USHORT*)&data);
	}
	*pval = (MMP_UBYTE)(data & 0x00FF);
	
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_WriteRegister(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE reg , MMP_UBYTE val)
{
	MMP_ERR st= MMP_ERR_NONE;
	if (reg >= GSensor_Reg_INVALID)
	{
		DPRINT("GsnrWrite-->Wrong Register Address\r\n");
		return MMP_ERR_NONE;
	}
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		st = MMPF_I2cm_WriteReg(&m_I2cmAttr_gsensor, (MMP_USHORT)reg, (MMP_USHORT)val);
	}
	else
	{
		st = MMPF_I2cm_WriteReg(&m_I2cmAttr_gsensor_sc7a30e, (MMP_USHORT)reg, (MMP_USHORT)val);
	}
	//printc("Gsensor_WriteRegister:%d   \r\n",st);
	return MMP_ERR_NONE;
}

void Gsensor_DumpRegister(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr)
{
#if (GSENSOR_DBG_MSG)

	MMP_LONG	i;
	MMP_UBYTE	rv;
	MMP_UBYTE	regs[] = {		0x0F, 0x20, 0x21, 0x22, 0x23, 
								0x24, 0x25, 0x28, 0x29, 0x2A, 
								0x2B, 0x2C, 0x2D, 0x30, 0x31,
								0x32, 0x33
							};

	DPRINT("====LIS3DH Registers====\r\n");
							        
	for (i = 0; i < sizeof(regs); i++) 
	{
		pthis->MMPF_Gsensor_GetReg(pthis, regs[i], &rv);
		DPRINT("Reg: 0x%02x, 0x%02x\r\n", regs[i], rv);
	}
	
	DPRINT("PROPERTY id = 0x%x, Range = %d, Sens = %d, GroundAxis = %d, 1G = 0x%x , ErrG = 0x%x, MaxG = %d\r\n",
	       pgsnr_attr->ubGsensorID,
	       pgsnr_attr->ubRange,
	       pgsnr_attr->ubSensitive,
	       pgsnr_attr->ubGroundAxis,
	       pgsnr_attr->ub1G,
	       pgsnr_attr->ub1G_Err,
	       pgsnr_attr->nMaxG);
#endif
}

MMP_ERR Gsensor_ReadOffsetFromFile(struct _3RD_PARTY_GSENSOR *pthis)
{
#if 0
	AHC_ERR err;
	UINT32 	ulFileId, ulFileSize, ulActualSize;
	
	err |= AHC_FS_FileOpen(GSNR_OFFSET_FILENAME,AHC_StrLen(GSNR_OFFSET_FILENAME),"rb",AHC_StrLen("rb"),&ulFileId);
		
	err |= AHC_FS_FileGetSize(ulFileId, &ulFileSize);
	
	err |= AHC_FS_FileRead(ulFileId, (UINT8*)&m_GSensorOffset, sizeof(GSENSOR_DATA), &ulActualSize);
	
	err |= AHC_FS_FileClose(ulFileId);

	if(err)
		DPRINT("Gsensor_ReadOffsetFromFile Error\r\n");
	else
		DPRINT("Gsensor_ReadOffsetFromFile Success\r\n");
#endif

	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_WriteOffsetToFile(struct _3RD_PARTY_GSENSOR *pthis)
{
	//DramToFile(GSNR_OFFSET_FILENAME, (INT8*)&m_GSensorOffset,sizeof(GSENSOR_DATA));
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_ResetDevice(struct _3RD_PARTY_GSENSOR *pthis) 
{
	MMP_ERR 	status 	= MMP_ERR_NONE;
	MMP_UBYTE	reg;
#if 0	
	for (reg = 0x1F; reg < 0x28; reg++) 
	status |= pthis->MMPF_Gsensor_SetReg(pthis, reg, 0);
#endif
	return status; 
}
MMP_ERR Gsensor_ReadDeviceID_DA380(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr)
{
	MMP_ERR 	status = MMP_ERR_NONE;
	MMP_USHORT 	usdeviceID 	= 0;	
	
	status = pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Reg_ChipID, (MMP_UBYTE *)&usdeviceID);

	if(usdeviceID != GSENSOR_ID_DA312)
	{
		status = 1;	// ERROR
		printc(FG_RED("DA312 Gsensor Read Device ID 0x%x  Error\r\n"),usdeviceID);
		pgsnr_attr->ubGsensorID = 0xFF;
	}
	else
	{
		status = 0;	// SUCCESS
		printc(FG_GREEN("DA312 Gsensor Read Device ID 0x%x  Success\r\n"),usdeviceID);
		pgsnr_attr->ubGsensorID = (MMP_UBYTE)usdeviceID;
	}
	return status; 
}

MMP_ERR Gsensor_ReadDeviceID_SC7A30E(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr)
{
	MMP_ERR 	status;
	MMP_USHORT	usdeviceID	= 0;
	m_I2cmAttr_gsensor_sc7a30e.ubSlaveAddr = GSENSOR_SLAVE_ADDR_SC7A30E >> 1;

	pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Sc7a30e_Reg_ChipID, (MMP_UBYTE *)&usdeviceID);

	pgsnr_attr->ubGsensorID = usdeviceID;
	if(pgsnr_attr->ubGsensorID == GSENSOR_ID_SC7A30E)
		status = 0;
	return status;
}

MMP_ERR Gsensor_ReadDeviceID(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr)
{
	//MMP_ERR 	status = MMP_ERR_NONE;
	//MMP_USHORT	usdeviceID	= 0;	
	if(ApicalDeviceID == GSENSOR_ID_SC7A30E)
		return Gsensor_ReadDeviceID_SC7A30E(pthis, pgsnr_attr);
	else if(ApicalDeviceID == GSENSOR_ID_DA312)
		return Gsensor_ReadDeviceID_DA380(pthis, pgsnr_attr);

	return MMP_ERR_NONE;
}

MMP_ERR	Gsensor_GetSensorOffset(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE *data)
{
	int i;

	for(i=0;i<GSENSOR_DATA_LEN;i++)
	{
		data[i] = m_GSensorOffset.acc_val[i];
	}

	return MMP_ERR_NONE;
}

MMP_ERR	Gsensor_SetSensorOffset(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE *data)
{
	int i;

	for(i=0;i<GSENSOR_DATA_LEN;i++)
	{
		m_GSensorOffset.acc_val[i] = data[i];
	}

	return MMP_ERR_NONE;
}
MMP_ERR Gsensor_Read_XYZ_DA380(struct _3RD_PARTY_GSENSOR *pthis, GSENSOR_DATA *data) 
{
	UINT32 x_value = 0,y_value = 0,z_value = 0,tmp0,tmp1,tmp2;
	UINT32  tempdata[6] = {0};
	int i,j;
       static UINT32	buffer_old[3]= {0};	// 16 buffer
	static UINT8 first_get_data=0;
       
	//MMPF_I2cm_ReadBurstData(&m_I2cmAttr_gsensor,GSensor_Reg_XOUT_L,&tempdata[0],6);
	pthis->MMPF_Gsensor_GetReg(pthis, 0x02, (MMP_UBYTE*)&tempdata[0]);
	pthis->MMPF_Gsensor_GetReg(pthis, 0x03, (MMP_UBYTE*)&tempdata[1]);
	pthis->MMPF_Gsensor_GetReg(pthis, 0x04, (MMP_UBYTE*)&tempdata[2]);
	pthis->MMPF_Gsensor_GetReg(pthis, 0x05, (MMP_UBYTE*)&tempdata[3]);
	pthis->MMPF_Gsensor_GetReg(pthis, 0x06, (MMP_UBYTE*)&tempdata[4]);
	pthis->MMPF_Gsensor_GetReg(pthis, 0x07, (MMP_UBYTE*)&tempdata[5]);

	x_value = ((tempdata[1])<<8|tempdata[0])>>4;//HIGH 8bit 
	y_value = ((tempdata[3])<<8|tempdata[2])>>4;
	z_value = ((tempdata[5])<<8|tempdata[4])>>4;
	//DPRINT("00  x_value y_value z_value = %d, %d, %d\r\n\n", x_value, y_value, z_value);

	if(x_value>=2048)
	{
		x_value=4096-x_value;
	}
	if(y_value>=2048)
	{
		y_value=4096-y_value;
	}
	if(z_value>=2048)
	{
		z_value=4096-z_value;
	}
	
	if(!first_get_data)
	{
		buffer_old[0] = x_value;
		buffer_old[1] = y_value;
		buffer_old[2] = z_value;
		first_get_data=1;
	}

	if(x_value>buffer_old[0])
		tmp0=(x_value - buffer_old[0]);
	else
		tmp0=(buffer_old[0]-x_value);

	if(y_value>buffer_old[1])
		tmp1=(y_value - buffer_old[1]);
	else
		tmp1=(buffer_old[1]-y_value);

	if(z_value>buffer_old[2])
		tmp2=(z_value - buffer_old[2]);
	else
		tmp2=(buffer_old[2]-z_value);
	
	data->acc_val[0] = tmp0/8;
	data->acc_val[1] = tmp1/8;
	data->acc_val[2] = tmp2/8;
	//DPRINT("tmp0 tmp1 tmp2 = %d, %d, %d\r\n\n", tmp0, tmp1, tmp2);
	//DPRINT("buffer_old[0] buffer_old[1] buffer_old[2] = %d, %d, %d\r\n\n", buffer_old[0], buffer_old[1], buffer_old[2]);
	//DPRINT("11  x_value y_value z_value = %d, %d, %d\r\n\n", x_value, y_value, z_value);

	buffer_old[0] = x_value;
	buffer_old[1] = y_value;
	buffer_old[2] = z_value;


	
	//DPRINT("Gsensor_Read_XYZ___data->acc_val = %d, %d, %d\r\n\n", data->acc_val[0], data->acc_val[1], data->acc_val[2]);

	return MMP_ERR_NONE;
}
MMP_ERR Gsensor_Read_XYZ_SC7A30E(struct _3RD_PARTY_GSENSOR *pthis, GSENSOR_DATA *data) 
{
	u16 x_value = 0, y_value = 0, z_value = 0,SR;
	u16 tempdata[6] = {0};
	i16 temp_value = 0;
	int i = 0;
	for(i=0; i<6; i++) {
		pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Sc7a30e_Reg_XOUT_L+i, (u8*)(tempdata+i));
	}
	
	x_value = (((tempdata[1] << 8) | (tempdata[0] & 0xff)) >> 4);
	y_value = (((tempdata[3] << 8) | (tempdata[2] & 0xff)) >> 4);
	z_value = (((tempdata[5] << 8) | (tempdata[4] & 0xff)) >> 4);

	temp_value = (x_value & 0x800/*为负值*/) ? (x_value | 0xF000) : (x_value);
	x_value = (u16)(temp_value + 2048);
	temp_value = (y_value & 0x800/*为负值*/) ? (y_value | 0xF000) : (y_value);
	y_value = (u16)(temp_value + 2048);
	temp_value = (z_value & 0x800/*为负值*/) ? (z_value | 0xF000) : (z_value);
	z_value = (u16)(temp_value + 2048);

	//printc(FG_GREEN("GXYZ:%d\t%d\t%d\r\n"),x_value,y_value,z_value);
	
	data->acc_val[0] = x_value>>4;
	data->acc_val[1] = y_value>>4;
	data->acc_val[2] = z_value>>4;
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_Read_XYZ(struct _3RD_PARTY_GSENSOR *pthis, GSENSOR_DATA *data) 
{
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		Gsensor_Read_XYZ_DA380(pthis,data);
	}
	else if(ApicalDeviceID == GSENSOR_ID_SC7A30E)
	{   
		Gsensor_Read_XYZ_SC7A30E(pthis,data);
	}

	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_Read_XYZ_WithCompensation_SC7A30E(struct _3RD_PARTY_GSENSOR *pthis, GSENSOR_DATA *data)
{
#define GET_DIFF(x,y) ((x) > (y))?((x) -(y)):((y)-(x))

    static GSENSOR_DATA prev;
    GSENSOR_DATA cur;

    Gsensor_Read_XYZ(pthis,&cur);
	if(kfirstread == 0)
	{
		kfirstread = 1;
		data->acc_val[0] = 0;
		data->acc_val[1] = 0;
		data->acc_val[2] = 0;
	}
	else
	{
	    data->acc_val[0] = GET_DIFF(cur.acc_val[0],prev.acc_val[0]);
	    data->acc_val[1] = GET_DIFF(cur.acc_val[1],prev.acc_val[1]);
	    data->acc_val[2] = GET_DIFF(cur.acc_val[2],prev.acc_val[2]);
	}
    prev=cur;
    return MMP_ERR_NONE;

}

MMP_ERR Gsensor_Read_XYZ_WithCompensation_DA380(struct _3RD_PARTY_GSENSOR *pthis, GSENSOR_DATA *data) 
{	
	MMP_USHORT x_value = 0,y_value = 0,z_value = 0;
	MMP_USHORT x_value_lsb = 0, x_value_msb = 0, y_value_lsb = 0, y_value_msb = 0, z_value_lsb = 0, z_value_msb = 0;
	MMP_UBYTE  i;
	MMP_UBYTE  ubTempX, ubTempY, ubTempZ;
	MMP_USHORT  tempdata[6] = {0};
	//int i = 0;
	//printc("Gsensor_Read_XYZ_WithCompensation_DA380\n");
	MMPF_I2cm_ReadBurstData(&m_I2cmAttr_gsensor,GSensor_Reg_XOUT_L,&tempdata[0],6);
	// printc("Gsensor_Read_XYZ_WithCompensation_DA380,tempdata:%d,%d,%d %d,%d,%d\n",tempdata[0],tempdata[1],tempdata[2],tempdata[3],tempdata[4],tempdata[5]);

	x_value = (tempdata[1])>>2;//HIGH 8bit 
	y_value = (tempdata[3])>>2;
	z_value = (tempdata[5])>>2;
	
	data->acc_val[0] = x_value;
	data->acc_val[1] = y_value;  
	data->acc_val[2] = z_value;
		
	ubTempX = data->acc_val[0];
	ubTempY = data->acc_val[1];
	ubTempZ = data->acc_val[2];
	

        //printc("Gsensor_Read_XYZ_WithCompensation_DA380,ubTemp:%d,%d,%d\n",ubTempX,ubTempY,ubTempZ);
	data->acc_val[0] = TO_XXXX_VALUE(x_value);
	data->acc_val[1] = TO_XXXX_VALUE(y_value);  
	data->acc_val[2] = TO_XXXX_VALUE(z_value)-0xf;
        //printc("Gsensor_Read_XYZ_WithCompensation_DA380,1111111data->acc_val:%d,%d,%d\n",data->acc_val[0],data->acc_val[1],data->acc_val[2]);

	
	for(i=0;i<GSENSOR_DATA_LEN;i++)
	{
		if(m_GSensorOffset.acc_val[i] >= 0x80)
		{
			if(data->acc_val[i] >= 0x80)
			{
				data->acc_val[i] = data->acc_val[i] + (0x100 - m_GSensorOffset.acc_val[i]);
				if(data->acc_val[i] >= 0xFF)
				data->acc_val[i] = 0xFF;
			}else
			{
				data->acc_val[i] = data->acc_val[i] + (0x100 - m_GSensorOffset.acc_val[i]);
				//if(data->acc_val[i] <= 0x00)
				//data->acc_val[i] = 0x00;
			}
		}
		else
		{
			if(data->acc_val[i] >= 0x80)
			{
				data->acc_val[i] = data->acc_val[i] + m_GSensorOffset.acc_val[i];
				if(data->acc_val[i] >= 0xFF)
				data->acc_val[i] = 0xFF;
			}
			else
			{
				if(data->acc_val[i] >= m_GSensorOffset.acc_val[i])
				data->acc_val[i] = data->acc_val[i] - m_GSensorOffset.acc_val[i];
				else
				data->acc_val[i] = 0;
			}
				
		}

	}
        printc("Gsensor_Read_XYZ_WithCompensation_DA380,22222222222data->acc_val:%d,%d,%d\n",data->acc_val[0],data->acc_val[1],data->acc_val[2]);

	#if 0
	DPRINT("\n");
	#endif
	return MMP_ERR_NONE;
}
MMP_ERR Gsensor_Read_XYZ_WithCompensation(struct _3RD_PARTY_GSENSOR *pthis, GSENSOR_DATA *data) 
{
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		Gsensor_Read_XYZ_WithCompensation_DA380(pthis,data);
	}
	else
	{
		Gsensor_Read_XYZ_WithCompensation_SC7A30E(pthis,data);
	}
	
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_Read_Accel_Avg(struct _3RD_PARTY_GSENSOR *pthis, GSENSOR_DATA *data)
{
#if 1
   	MMP_LONG 		xyz_acc_pos[GSENSOR_DATA_LEN];   
   	MMP_LONG 		xyz_acc_neg[GSENSOR_DATA_LEN];
   	MMP_ULONG 		xyz_acc_sum[GSENSOR_DATA_LEN];
  	GSENSOR_DATA 	temp;
	MMP_UBYTE		i, j;

	memset(&xyz_acc_pos[0], 0, GSENSOR_DATA_LEN * sizeof(MMP_LONG));
	memset(&xyz_acc_neg[0], 0, GSENSOR_DATA_LEN * sizeof(MMP_LONG));
	memset(&xyz_acc_sum[0], 0, GSENSOR_DATA_LEN * sizeof(MMP_ULONG));	
	
	for(i = 0; i < CALIBRATE_AVG_NUM; i++) 
	{      
		pthis->MMPF_Gsensor_Read_XYZ(pthis, (GSENSOR_DATA *)&temp);
		
		for(j = 0; j < GSENSOR_DATA_LEN; j++)
		{ 	 
			if(temp.acc_val[j] & 0x80)
				xyz_acc_neg[j] += (0x100 - temp.acc_val[j]);
			else
				xyz_acc_pos[j] += (temp.acc_val[j] & 0x7F);		
		}
			
		RTNA_WAIT_MS(10);	   
  	}	
	DPRINT("=====Read Average======\r\n");  	
  	for(i = 0; i < GSENSOR_DATA_LEN; i++) 
	{
			if( xyz_acc_pos[i] >= xyz_acc_neg[i] )
			{
				xyz_acc_sum[i] = xyz_acc_pos[i] - xyz_acc_neg[i];
				data->acc_val[i] = (xyz_acc_sum[i] / CALIBRATE_AVG_NUM);
			}
			else
			{
				xyz_acc_sum[i] = xyz_acc_neg[i] - xyz_acc_pos[i];	
				data->acc_val[i] = (0x100 - ((xyz_acc_sum[i] / CALIBRATE_AVG_NUM)));
			}
					
			DPRINT("xyz_acc_sum[i] : %d\r\n",xyz_acc_sum[i]); 	
			//data->acc_val[i] = (xyz_acc_sum[i] / CALIBRATE_AVG_NUM);	
	}
#endif
	return MMP_ERR_NONE;		
}

MMP_ERR Gsensor_CalculateOffset(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE gAxis, GSENSOR_DATA avg)
{
#if 0//TBD
	switch(gAxis)
	{
		case GSNR_CALIBRATE_G_ON_Z_NEGATIVE:  
			m_GSensorOffset.acc_val[0] =  avg.acc_val[0];    
			m_GSensorOffset.acc_val[1] =  avg.acc_val[1] ;
			m_GSensorOffset.acc_val[2] =  avg.acc_val[2] + DEFAULT_SENSITIVITY;
		break;
		case GSNR_CALIBRATE_G_ON_X_POSITIVE:  
			m_GSensorOffset.acc_val[0] =  avg.acc_val[0] + DEFAULT_SENSITIVITY;//Check
			m_GSensorOffset.acc_val[1] =  avg.acc_val[1] ;
			m_GSensorOffset.acc_val[2] =  avg.acc_val[2] ;
		break;
		case GSNR_CALIBRATE_G_ON_Z_POSITIVE:  
			m_GSensorOffset.acc_val[0] =  avg.acc_val[0] ;    
			m_GSensorOffset.acc_val[1] =  avg.acc_val[1] ;
			m_GSensorOffset.acc_val[2] =  avg.acc_val[2] - DEFAULT_SENSITIVITY;
		break;
		case GSNR_CALIBRATE_G_ON_X_NEGATIVE:  
			m_GSensorOffset.acc_val[0] =  avg.acc_val[0] - DEFAULT_SENSITIVITY;//Check    
			m_GSensorOffset.acc_val[1] =  avg.acc_val[1] ;
			m_GSensorOffset.acc_val[2] =  avg.acc_val[2] ;
		break;
		case GSNR_CALIBRATE_G_ON_Y_NEGATIVE:
			m_GSensorOffset.acc_val[0] =  avg.acc_val[0] ;    
			m_GSensorOffset.acc_val[1] =  avg.acc_val[1] + DEFAULT_SENSITIVITY;
			m_GSensorOffset.acc_val[2] =  avg.acc_val[2] ;
		break;
		case GSNR_CALIBRATE_G_ON_Y_POSITIVE: 
			m_GSensorOffset.acc_val[0] =  avg.acc_val[0] ;    
			m_GSensorOffset.acc_val[1] =  avg.acc_val[1] - DEFAULT_SENSITIVITY;
			m_GSensorOffset.acc_val[2] =  avg.acc_val[2] ;
		break;
		default:  
			return MMP_HIF_ERR_PARAMETER;
		break;	
	}

#else

	m_GSensorOffset.acc_val[0] =  avg.acc_val[0] ;    
	m_GSensorOffset.acc_val[1] =  avg.acc_val[1] ;
	m_GSensorOffset.acc_val[2] =  avg.acc_val[2] ;

	DPRINT("OFFSET Acceleration X = 0x%x, Y = 0x%x, Z = 0x%x\r\n",
	       m_GSensorOffset.acc_val[0],
	       m_GSensorOffset.acc_val[1],
	       m_GSensorOffset.acc_val[2]);

#endif
	return MMP_ERR_NONE;
}

MMP_ERR	Gsensor_Calibration(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE side)
{
	GSENSOR_DATA avg;
	
	pthis->MMPF_Gsensor_Read_Accel_Avg(pthis, &avg);
	
	pthis->MMPF_Gsensor_CalculateOffset(pthis, side, avg); 
	
	return MMP_ERR_NONE;
}

MMP_ERR	Gsensor_SetIntDuration(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE arg)
{
	
	return MMP_ERR_NONE;
}
MMP_ERR Gsensor_GetIntStatus_SC7A30E(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE* pstatus)
{
    MMP_UBYTE   ubStatus= 0,ubStatus1 = 0;
    MMP_UBYTE   ubEn    = 0;


    *pstatus = (ubStatus |ubEn | ubStatus1);

    return MMP_ERR_NONE;
}

MMP_ERR Gsensor_GetIntStatus_DA380(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE* pstatus)
{
	MMP_UBYTE   ubStatus= 0,ubStatus1 = 0;
	MMP_UBYTE   ubEn    = 0;

	pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Reg_Motion_INT, &ubEn);
	pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Reg_Data_INT, &ubStatus);
	pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Reg_Tap_Active_Statu, &ubStatus1);
	//printc("Gsensor_GetIntStatus:%x  \r\n",(ubStatus & ubEn ));
	*pstatus = (ubStatus |ubEn | ubStatus1);

	return MMP_ERR_NONE;
}
MMP_ERR Gsensor_GetIntStatus(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE* pstatus)
{
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		Gsensor_GetIntStatus_DA380(pthis,pstatus);
	}
	else if(ApicalDeviceID == GSENSOR_ID_SC7A30E)
	{   
		Gsensor_GetIntStatus_SC7A30E(pthis,pstatus);
	}
}
MMP_ERR Gsensor_SetPowerSaveMode(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE mode)
{	
#if 0
	MMP_UBYTE value;
	pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Reg_CTRL1, &value);
	value &= 0x0F;
	pthis->MMPF_Gsensor_SetReg(pthis, GSensor_Reg_CTRL1, value);
#endif
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_SetNormalModeDataRate(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE rate)
{
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_SetInterruptSource(struct _3RD_PARTY_GSENSOR *pthis, MMP_UBYTE src)
{
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_GetDataRate(struct _3RD_PARTY_GSENSOR *pthis, MMP_USHORT *wDataRate)
{

	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_SetDynamicRange(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE ubDrange)
{
	MMP_UBYTE ubFullSacleRange;
	
	pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Reg_Grange,&ubFullSacleRange);
	
	ubFullSacleRange &= ~DYNAMIC_RANGE_MASK;	
	switch(ubDrange)
	{
		
		case 2:
			ubFullSacleRange |= DYNAMIC_RANGE_2;
		break;
		case 4:
			ubFullSacleRange |= DYNAMIC_RANGE_4;
		break;
		case 8:
			ubFullSacleRange |= DYNAMIC_RANGE_8;
		break;
		case 16:
			ubFullSacleRange |= DYNAMIC_RANGE_16;
		break;
		
		default:
			return 1;	// ERROR!
	}
	pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_Grange, ubFullSacleRange);
	switch (ubDrange) 
	{
		case 2:
		 	pgsnr_attr->nMaxG	    = MAX_2G;
		 	pgsnr_attr->ub1G	 	= 0x80;	/* the value is absolute for threshold, it needs divided by 2 for XYZ */
		 	pgsnr_attr->ub1G_Err    = 0x08;
		break;
		case 4:
		 	pgsnr_attr->nMaxG	    = MAX_4G;
		 	pgsnr_attr->ub1G	 	= 0x40;
		 	pgsnr_attr->ub1G_Err    = 0x04;
		break;
		case 8:
		 	pgsnr_attr->nMaxG	    = MAX_8G;
		 	pgsnr_attr->ub1G	 	= 0x20;
		 	pgsnr_attr->ub1G_Err    = 0x02;
		break;
		case 16:
		 	pgsnr_attr->nMaxG	    = MAX_16G;
		 	pgsnr_attr->ub1G	 	= 0x10;
		 	pgsnr_attr->ub1G_Err    = 0x01;
		break;
	}
	return MMP_ERR_NONE;
}

MMP_ERR Gsensor_GetDynamicRange_DA380(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE *ubDrange)
{
	MMP_UBYTE ubValue;
	MMP_UBYTE ubRange;

	if(pgsnr_attr->ubGsensorID != GSENSOR_ID_DA312)
		return MMP_ERR_NONE;

	ubRange = (ubValue & DYNAMIC_RANGE_MASK);

	switch(ubRange)
	{
		case DYNAMIC_RANGE_2:
			*ubDrange = 2;
		break;
		case DYNAMIC_RANGE_4:
			*ubDrange = 4;
		break;
		case DYNAMIC_RANGE_8:
			*ubDrange = 8;
		break;
		case DYNAMIC_RANGE_16:
			*ubDrange = 16;
		break;
		default:
			*ubDrange = 1;
		break;
	}
	return MMP_ERR_NONE;
}
MMP_ERR Gsensor_GetDynamicRange_SC7A30E(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE *ubDrange)
{
    MMP_UBYTE ubValue;
    MMP_UBYTE ubRange;

    if(pgsnr_attr->ubGsensorID != GSENSOR_ID_SC7A30E)
        return MMP_ERR_NONE;

    //pthis->MMPF_Gsensor_GetReg(pthis, GSensor_Reg_CTRL4,&ubValue);

    ubRange = (ubValue & DYNAMIC_RANGE_MASK);

    switch(ubRange)
    {
    case DYNAMIC_RANGE_2:
        *ubDrange = 2;
        break;
    case DYNAMIC_RANGE_4:
        *ubDrange = 4;
        break;
    case DYNAMIC_RANGE_8:
        *ubDrange = 8;
        break;
    case DYNAMIC_RANGE_16:
        *ubDrange = 16;
        break;
    default:
        *ubDrange = 1;
        break;
    }
    return MMP_ERR_NONE;
}

MMP_ERR Gsensor_GetDynamicRange(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE *ubDrange)
{
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		Gsensor_GetDynamicRange_DA380(pthis,pgsnr_attr,ubDrange);
	}
	else if(ApicalDeviceID == GSENSOR_ID_SC7A30E)
	{   
		Gsensor_GetDynamicRange_SC7A30E(pthis,pgsnr_attr,ubDrange);
	}
    return MMP_ERR_NONE;
}
MMP_ERR Gsensor_SetGroundAxis(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE arg)
{
	MMP_ERR status = MMP_ERR_NONE;

	if ((arg != X_AXIS) &&
	    (arg != Y_AXIS) &&
	    (arg != Z_AXIS))
		status = 1;  // ERROR!
	else
		pgsnr_attr->ubGroundAxis = (MMP_UBYTE)arg;

	return status;
}
static void G_SC7A30E_open_interrupt(struct _3RD_PARTY_GSENSOR *pthis, int sensitivity)
{
	MMP_UBYTE uTmp = 0;
	MMP_UBYTE uIntSens[4] = {0x0c/*192mg*/, 0x18/*384mg*/, 0x30/*768mg*/,0x6c/*960mg*/};

#if 0
	m_I2cmAttr_gsensor_sc7a30e.ubSlaveAddr = 0x32 >> 1;
	pthis->MMPF_Gsensor_SetReg(pthis,0x24, 0x80);
#endif

	m_I2cmAttr_gsensor_sc7a30e.ubSlaveAddr = GSENSOR_SLAVE_ADDR_SC7A30E >> 1;

	//pthis->MMPF_Gsensor_SetReg(pthis,0x24, 0, 0x80);
	pthis->MMPF_Gsensor_SetReg(pthis,0x25,  0x02);//0x02==>LOW Valid￡?0x00==>High Valid
	pthis->MMPF_Gsensor_SetReg(pthis,0x20,0x47);//DR 0x37:25hz,0x47:50hz,
	pthis->MMPF_Gsensor_GetReg(pthis, 0x26,&uTmp);
	pthis->MMPF_Gsensor_SetReg(pthis,0x21, 0x0D/*0x0C*/);//高通使能
	pthis->MMPF_Gsensor_SetReg(pthis,0x23, 0x80);//量程+-2g
	//pthis->MMPF_Gsensor_SetReg(pthis,0x23, 0x90);//量程+-4g
	switch(sensitivity)
	{
		case 1:
			pthis->MMPF_Gsensor_SetReg(pthis,0x3A,  uIntSens[0]);//触发阈值,越高越不灵敏
			break;
		case 2:
			pthis->MMPF_Gsensor_SetReg(pthis,0x3A,  uIntSens[1]);//触发阈值
			break;
		case 3:
			pthis->MMPF_Gsensor_SetReg(pthis,0x3A,  uIntSens[2]);//触发阈值
			break;
		case 4:
			pthis->MMPF_Gsensor_SetReg(pthis,0x3A,  uIntSens[3]);//触发阈值
			break;
		default:
			pthis->MMPF_Gsensor_SetReg(pthis,0x3A,  uIntSens[1]);//触发阈值
			break;
	}
	
	//pthis->MMPF_Gsensor_SetReg(pthis,0x3A, 0, uIntSens[2]);
	pthis->MMPF_Gsensor_SetReg(pthis,0x3B,  0x7F);
	pthis->MMPF_Gsensor_SetReg(pthis,0x3C, 0x64);//
	//pthis->MMPF_Gsensor_SetReg(pthis,0x25, 0, 0x00);//0x02==>LOW Valid￡?0x00==>High Valid
	pthis->MMPF_Gsensor_SetReg(pthis,0x38,  0x15);
	
	pthis->MMPF_Gsensor_SetReg(pthis,0x22,  0x80);//enable interrupt
	//clear latch and set latch mode
	pthis->MMPF_Gsensor_GetReg(pthis, 0x39,&uTmp);
}

MMP_ERR Gsensor_Initial_DA380(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr)
{
    MMP_UBYTE u8temp = 0;
    int i;

    pthis->MMPF_Gsensor_ReadDeviceID(pthis, pgsnr_attr); 
    if(pgsnr_attr->ubGsensorID == GSENSOR_ID_DA312)
    {	
    		printc("Gsensor_Initial_DA380\n");
		pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_Config, 0x24);
		MMPF_OS_Sleep(10);
		pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_Grange, 0x02);//leo20180704
		pthis->MMPF_Gsensor_SetReg(pthis, GSensor_Reg_mode_BW, 0x1e);
		pthis->MMPF_Gsensor_SetReg(pthis, GSensor_Reg_ODR_Axis_DIS, 0x07);
		pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_INT_pin_Cof, 0x03);

		//pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_INT_SET1, 0x00);//leo20180704
		//pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_Active_DURA, 0x03);
		//pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_Active_TH, 0x1b);
		//pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_INT_MAP1, 0x04);
		//pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_INT_LTACH, 0x06);
		pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_engineer_mode, 0x83);
		pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_engineer_mode, 0x69);
		pthis->MMPF_Gsensor_SetReg(pthis,GSensor_Reg_engineer_mode, 0xBD);
		
        pgsnr_attr->nInited = 1;
   		printc(FG_GREEN("Gsensor_Initial  Succes  ID %x \r\n"),pgsnr_attr->ubGsensorID );
    } 
    else 
    {
        printc(FG_RED("Wrong G-Sensor ID for DA312\r\n"));

        pgsnr_attr->ubRange     = 2;
        pgsnr_attr->nMaxG	    = MAX_2G;
        pgsnr_attr->ub1G		= 0x40;	
        pgsnr_attr->ub1G_Err    = 0x08;
    }
    return MMP_ERR_NONE;
}

MMP_ERR Gsensor_Initial_SC7A30E(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr)
{
	MMP_UBYTE u8temp = 0;
	int i;


#if 0
	m_I2cmAttr_gsensor_sc7a30e.ubSlaveAddr=0x32 >> 1;
	pthis->MMPF_Gsensor_SetReg(pthis,0x24, 0x80);
#endif
	m_I2cmAttr_gsensor_sc7a30e.ubSlaveAddr=GSENSOR_SLAVE_ADDR_SC7A30E >> 1;

	pthis->MMPF_Gsensor_ReadDeviceID(pthis, pgsnr_attr);

    if(pgsnr_attr->ubGsensorID == GSENSOR_ID_SC7A30E)
    {
		printc("Gsensor_Initial SC7A30E\r\n");

		pthis->MMPF_Gsensor_SetReg(pthis,0x22, 0x00);//Close Interrupt
		pthis->MMPF_Gsensor_SetReg(pthis,0x38, 0x00);
		pthis->MMPF_Gsensor_SetReg(pthis,0x24, 0x80);//reboot ic
		pthis->MMPF_Gsensor_SetReg(pthis,0x20, 0x47);//DR 50hz
		pthis->MMPF_Gsensor_GetReg(pthis, 0x20, &u8temp);
		//printc("reg 0x20 %x\r\n",u8temp);
		//pthis->MMPF_Gsensor_SetReg(pthis,0x23, 0x80);//á?3ì+-2g
		//pthis->MMPF_Gsensor_SetReg(pthis,0x23, 0x90);//á?3ì+-4g
		pthis->MMPF_Gsensor_SetReg(pthis,0x23, 0x80);//á?3ì+-8g
		pthis->MMPF_Gsensor_SetReg(pthis,0x25,  0x02);//0x02==>LOW Valid￡?0x00==>High Valid
		//SC7A30E_open_interrupt(pthis,2);//gaoc
		pgsnr_attr->nInited = 1;
	}
	else
	{
		//printc(FG_RED("Wrong G-Sensor ID for GSENSOR_ID_SC7A30E\r\n"));

		pgsnr_attr->ubRange     = 2;
		pgsnr_attr->nMaxG	    = MAX_4G;
		pgsnr_attr->ub1G	    = 0x40;
		pgsnr_attr->ub1G_Err    = 0x08;
	}
	
	return MMP_ERR_NONE;
}
MMP_ERR Gsensor_Initial(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr)
{
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		Gsensor_Initial_DA380(pthis,pgsnr_attr);
	}
	else if(ApicalDeviceID == GSENSOR_ID_SC7A30E)
	{   
		Gsensor_Initial_SC7A30E(pthis,pgsnr_attr);
	}
    return MMP_ERR_NONE;
}
void Gsensor_Initial_PowerOff_DA380(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE ubPowerGsenOnValue)
{
    MMP_UBYTE status;

    printc(FG_RED("### %s - %d\r\n"), __func__, ubPowerGsenOnValue);
	
	pthis->MMPF_Gsensor_ReadDeviceID(pthis, pgsnr_attr); 
    
    if(pgsnr_attr->ubGsensorID == GSENSOR_ID_DA312)
	{	

    }
	else
	{
		pgsnr_attr->ubRange     = 2;
        pgsnr_attr->nMaxG       = MAX_2G;
        pgsnr_attr->ub1G        = 0x40;	
        pgsnr_attr->ub1G_Err    = 0x08;
	}
		
}

void Gsensor_Initial_PowerOff_SC7A30E(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE ubPowerGsenOnValue)
{
	MMP_UBYTE status;

	printc("### %s - %d\r\n", __func__, ubPowerGsenOnValue);

	G_SC7A30E_open_interrupt(pthis,3);

}

void Gsensor_Initial_PowerOff(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE ubPowerGsenOnValue)
{
	if(ApicalDeviceID == GSENSOR_ID_DA312)
	{
		Gsensor_Initial_PowerOff_DA380(pthis,pgsnr_attr,ubPowerGsenOnValue);
	}
	else if(ApicalDeviceID == GSENSOR_ID_SC7A30E)
	{   
		Gsensor_Initial_PowerOff_SC7A30E(pthis,pgsnr_attr,ubPowerGsenOnValue);
	}
}
MMP_ERR Gsensor_IOControl(struct _3RD_PARTY_GSENSOR *pthis, GSNR_ATTRIBUTE *pgsnr_attr, MMP_UBYTE cmd, MMP_UBYTE* arg)
{
	MMP_ERR status = MMP_ERR_NONE;
	
	switch(cmd)
	{
		case GSNR_CMD_RESET: 
			status |= pthis->MMPF_Gsensor_Reset(pthis);
			status |= pthis->MMPF_Gsensor_ReadDeviceID(pthis, pgsnr_attr); 
			if (status == MMP_ERR_NONE)
			{
				// Init default setting
				pgsnr_attr->ubGroundAxis = AXIS_UNKNOWN;
				pthis->MMPF_Gsensor_Initialize(pthis, pgsnr_attr);
				
				#if (GSENSOR_DETECT_MODE == GSNESOR_DETECT_DIFF)
				if(bGsensorCalibrated == AHC_FALSE)
				{
					pthis->MMPF_Gsensor_Calibration(pthis, 0);
					bGsensorCalibrated = AHC_TRUE;
				}
				#endif
				
				//pgsnr_attr->nInited = 1;
			}
			else
			{
				pgsnr_attr->nInited = 0;
			}
		break;

		case GSNR_CMD_CALIBRATION: 
			status |= pthis->MMPF_Gsensor_Calibration(pthis, *arg);
		break;
		
		case GSNR_CMD_GET_OFFSET: 
			status |= pthis->MMPF_Gsensor_ReadOffsetFromFile(pthis);
			//status |= pthis->MMPF_Gsensor_GetSensorOffset(arg);
		break;
		
		case GSNR_CMD_SET_OFFSET: 
			status |= pthis->MMPF_Gsensor_SetSensorOffset(pthis, arg);
			status |= pthis->MMPF_Gsensor_WriteOffsetToFile(pthis);
		break;
		
		case GSNR_CMD_READ_ACCEL_XYZ:
			if(pgsnr_attr->nInited == 1)
    			status |= pthis->MMPF_Gsensor_Read_XYZ(pthis, (GSENSOR_DATA*)arg);
		break;

		case GSNR_CMD_READ_ACCEL_WITH_COMPENSATION:
			if(pgsnr_attr->nInited == 1)
    			status |= pthis->MMPF_Gsensor_Read_XYZ_WithCompensation(pthis, (GSENSOR_DATA*)arg);
		//	DPRINT("io\r\n");
		break;

		case GSNR_CMD_SET_PWR_ON_INT_THD: 
			Gsensor_Initial_PowerOff(pthis, pgsnr_attr, *arg);
		break;

		case GSNR_CMD_SET_INT_THD: 
		{
			int		thg;
			
			thg = *(int *) arg;

			if ((unsigned int)thg > (unsigned int)pgsnr_attr->nMaxG)
				thg = pgsnr_attr->nMaxG;
			
			thg 	= MAP2HW_G(thg, pgsnr_attr);
			//status 	= pthis->MMPF_Gsensor_SetReg(pthis, GSensor_Reg_INT1_THD, (MMP_UBYTE)thg);
			/* duration is how long to keep the HG */
			//status 	= pthis->MMPF_Gsensor_SetReg(pthis, GSensor_Reg_INT1_DURATION, (MMP_UBYTE)0x28);
			
			DPRINT("Set Threshold %d 0x%x\r\n", *(int *) arg, thg);
		}
		break;
		
		case GSNR_CMD_SET_INT_DURATION:
			status |= pthis->MMPF_Gsensor_SetIntDuration(pthis, *arg);
		break;
		
		case GSNR_CMD_GET_INT_STATUS:
			if (pgsnr_attr->nInited) {
			    status |= pthis->MMPF_Gsensor_GetIntStatus(pthis, arg);
			} else {
				status = 1;	// ERROR!
				*arg   = 0;
			}
		break;
		
		case GSNR_CMD_SET_POWER_SAVE:
			status |= pthis->MMPF_Gsensor_SetPowerSaveMode(pthis, *arg);
		break;
		case GSNR_CMD_SET_NORMAL_MODE:
			status |= pthis->MMPF_Gsensor_SetNormalModeDataRate(pthis, *arg);
		break;	
		case GSNR_CMD_SET_DATA_RATE:
			status |= pthis->MMPF_Gsensor_SetNormalModeDataRate(pthis, *arg);
		break;

		case GSNR_CMD_SET_INT_SOURCE:
			status |= pthis->MMPF_Gsensor_SetInterruptSource(pthis, *arg);
		break;
		
		case GSNR_CMD_WRITE_REG:
			status |= pthis->MMPF_Gsensor_SetReg(pthis, *arg , *(arg+1));
		break;
		
		case GSNR_CMD_READ_REG:
			status |= pthis->MMPF_Gsensor_GetReg(pthis, *arg , arg+1);
		break;
		
		case GSNR_CMD_GET_DATA_RATE:
			status |= pthis->MMPF_Gsensor_GetDataRate(pthis, (MMP_USHORT*)arg);
		break;
		
		case GSNR_CMD_SET_DYNA_RANGE:
			status |= pthis->MMPF_Gsensor_SetDynamicRange(pthis, pgsnr_attr, *arg);
		break;
		
		case GSNR_CMD_GET_DYNA_RANGE:
			status |= pthis->MMPF_Gsensor_GetDynamicRange(pthis, pgsnr_attr, arg);
		break;
		
		case GSNR_CMD_SET_GROUND_AXIS:
			status |= pthis->MMPF_Gsensor_SetGroundAxis(pthis, pgsnr_attr, (int)arg);
		break;		
		
		case GSNR_CMD_GROUND_AXIS_STATUS: 
		{
			GSENSOR_DATA	sd;
			GSNR_SBYTE		g;

			if (pgsnr_attr->ubGroundAxis == AXIS_UNKNOWN) {
				*arg = GSNR_POSITION_NOCHANGE;
				return MMP_ERR_NONE;
            }

			pthis->MMPF_Gsensor_Read_XYZ(pthis, &sd);
			
			g = (signed char)sd.acc_val[pgsnr_attr->ubGroundAxis];
			
			if (ABS(g) > LOW_BOUND_1G(pgsnr_attr)) 
			{
				if (g > 0) 
				{
					if ((kLastPosition != GSNR_POSITION_UP ||
						 kLastPosition == GSNR_POSITION_UNKNOWN) &&
						kUpCnt >= STABLE_CNT) 
					{
						kLastPosition = GSNR_POSITION_UP;
						*arg = kLastPosition;
					//	DPRINT("GSNR_POSITION_UP\r\n");
						return MMP_ERR_NONE;
					}

					if (kUpCnt < STABLE_CNT) 
					{
						kDwCnt = 0;
						kUpCnt++;
					}
				} 
				else 
				{
					if ((kLastPosition != GSNR_POSITION_DOWN ||
						 kLastPosition == GSNR_POSITION_UNKNOWN) &&
						 kDwCnt >= STABLE_CNT) 
					{
						kLastPosition = GSNR_POSITION_DOWN;
						*arg = kLastPosition;
					///	DPRINT("GSNR_POSITION_DOWN\r\n");
						return MMP_ERR_NONE;
					}

					if (kDwCnt <= STABLE_CNT) 
					{
						kUpCnt = 0;
						kDwCnt++;
					}
				}
			}

			*arg = GSNR_POSITION_NOCHANGE;
			return MMP_ERR_NONE;
		}
		break;

		default:
		break;
	}

	return status;
}

MMP_BOOL Gsensor_Attached(struct _3RD_PARTY_GSENSOR *pthis)
{
#if (GSENSOR_MODULE_ATT_COMPILER)
    #if(GSENSOR_CONNECT_ENABLE)
    return MMP_TRUE;
    #else
    return MMP_FALSE;
    #endif	
#elif (GSENSOR_MODULE_ATT_GPIO)
	MMP_UBYTE ubValue = MMP_FALSE;

	if(GSENSOR_MODULE_GPIO < MMP_GPIO_MAX)
	{
		MMPF_PIO_GetData(GSENSOR_MODULE_GPIO, &ubValue);
		return ubValue;
	}
	else
	{
		return AHC_FALSE;
	}
#endif

	return AHC_FALSE;
}

struct _3RD_PARTY_GSENSOR Gsensor_Obj = 
{
    //0
    Gsensor_ResetDevice,
    Gsensor_ReadDeviceID,
    Gsensor_Initial,
    Gsensor_WriteRegister,
    Gsensor_ReadRegister,
    
    //5
    Gsensor_ReadOffsetFromFile,
    Gsensor_WriteOffsetToFile,
    Gsensor_GetSensorOffset,
    Gsensor_SetSensorOffset,
    Gsensor_Read_XYZ,
    
    //10
    Gsensor_Read_XYZ_WithCompensation,
    Gsensor_Read_Accel_Avg,
    Gsensor_CalculateOffset,
    Gsensor_Calibration,
    Gsensor_SetIntDuration,
    
    //15
    Gsensor_GetIntStatus,
    Gsensor_SetPowerSaveMode,
    Gsensor_SetNormalModeDataRate,
    Gsensor_SetInterruptSource,
    Gsensor_GetDataRate,
    
    //20
    Gsensor_SetDynamicRange,
    Gsensor_GetDynamicRange,
    Gsensor_SetGroundAxis,
    Gsensor_IOControl,
    Gsensor_Attached,
    
    //25
    Gsensor_Config_I2CM,
   
};

int Gsensor_Module_Init(void)
{
    printc("[DA380 and SC7A30E - Gsensor_Module_Init]\r\n");
	if(SC7A30E_Gsensor_Probe())
		ApicalDeviceID = GSENSOR_ID_SC7A30E;
	else if(DA312_Gsensor_Probe())
		ApicalDeviceID = GSENSOR_ID_DA312;

	printc("[APK]Gsensor_Module_Init....ApicalDeviceID=0x%08X\r\n", ApicalDeviceID);

	if (ApicalDeviceID == 0)
	{
		ApicalDeviceID = GSENSOR_ID_SC7A30E;
	}
	
    MMPF_3RDParty_Gsensor_Register(&Gsensor_Obj);
    return 0;    
}

#pragma arm section code = "initcall6", rodata = "initcall6", rwdata = "initcall6",  zidata = "initcall6" 
#pragma O0
ait_module_init(Gsensor_Module_Init);
#pragma
#pragma arm section rodata, rwdata ,  zidata 

#endif		// ALL_FW

#ifdef MBOOT_FW
MMP_BOOL Gsensor_GetIntStatusForBoot(void)
{
    MMP_USHORT	 value	   = 0;
    MMP_UBYTE   ubStatus   = 0;
    MMP_UBYTE   ubStatus1 = 0;
    MMP_UBYTE   ubEn          = 0;

    //MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor_Sc7a30e, SC7A30E_GSensor_Reg_Motion_INT, (MMP_USHORT*)&ubEn);
    //MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor_Sc7a30e, SC7A30E_GSensor_Reg_Data_INT, (MMP_USHORT*)&ubStatus);
    //MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor_Sc7a30e, SC7A30E_GSensor_Reg_Tap_Active_Statu, (MMP_USHORT*)&ubStatus1);
	MMPF_I2cm_ReadReg(&m_I2cmAttr_gsensor_sc7a30e, 0x39, (MMP_USHORT*)&ubStatus1);
      value = ubStatus1 & 0x40;
	RTNA_DBG_Str(0, "SC7A30E_Gsensor_GetIntStatusForBoot = \r\n");
	RTNA_DBG_Byte(0, value);
	RTNA_DBG_Str(0, " \r\n");
    if (value)
    {
       // RTNA_DBG_PrintLong(0, value);
        return MMP_TRUE;
    }

    return MMP_FALSE;
}
#endif

#endif //GSENSOR_CONNECT_ENABLE && GSENSOR_MODULE == GSENSOR_MODULE_MC3230)

