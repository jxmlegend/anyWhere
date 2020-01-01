//==============================================================================
//
//  File        : AHC_Callback.h
//  Description : INCLUDE File for the AHC Callback function porting.
//  Author      : 
//  Revision    : 1.0
//
//==============================================================================

#ifndef _AHC_CALLBCAK_H_
#define _AHC_CALLBCAK_H_

/*===========================================================================
 * Include files
 *===========================================================================*/
#include "AHC_Common.h" 


/*===========================================================================
 * Macro define
 *===========================================================================*/



/*===========================================================================
 * Enum define
 *===========================================================================*/



/*===========================================================================
 * Structure define
 *===========================================================================*/



/*===========================================================================
 * Function prototype
 *===========================================================================*/

__weak void AHC_PostInitISP( void );
__weak void AHC_PostEmergencyDone(void);
__weak void AHC_FormatFreeConfigInfo( void );
__weak void AHC_FormatFreeFileNameCB( MMP_ULONG ulFolderIndex, MMP_ULONG ulFileIndex, MMP_BYTE byFileName[] );

__weak void AHC_PostDualRecdDone(void);
__weak void AHC_UF_CreateShareFileCB(UINT8 *NeedFileNum);
#endif //_AHC_CALLBCAK_H_
