//==============================================================================
//
//  File        : mmpd_uart.h
//  Description : INCLUDE File for the uart
//  Author      : Ben Lu
//  Revision    : 1.0
//
//==============================================================================
/**
 *  @file mmpd_uart.h
 *  @brief Header File for the uart API.
 *  @author Ben Lu
 *  @version 1.0
 */

#ifndef _MMPD_UART_H_
#define _MMPD_UART_H_

//==============================================================================
//
//                              INCLUDE FILE
//
//==============================================================================

#include "mmp_uart_inc.h"

//==============================================================================
//
//                              FUNCTION PROTOTYPES
//
//==============================================================================

MMP_ERR	MMPD_Uart_Open(MMP_UART_ID uart_id, MMP_UART_ATTR *uartattribute);
MMP_ERR	MMPD_Uart_Write(MMP_UART_ID uart_id, char *pWrite_Str, MMP_ULONG ulLength);
MMP_ERR	MMPD_Uart_Close(MMP_UART_ID uart_id);
MMP_ERR MMPD_UART_UseTxDMA(MMP_UART_ID uart_id, MMP_ULONG ulTxStartAddr, 
						   MMP_USHORT usTxTotalByte,  MMP_UART_DMA_INT_MODE uart_int_mode, 
						   MMP_BOOL bEnableInt, void* callBackFunc);
MMP_ERR MMPD_UART_UseRxDMA(MMP_UART_ID uart_id, MMP_BOOL bRingModeEn, MMP_ULONG ulRxStartAddr, MMP_ULONG ulRxEndAddr, MMP_ULONG ulRxLowBoundAddr,  MMP_UART_DMA_INT_MODE uart_int_mode, 
						   MMP_BOOL bEnableInt, void* callBackFunc, MMP_USHORT usRxThreshold, MMP_UBYTE ubRxTimeOut);

#endif // _MMPD_UART_H_