//==============================================================================
//
//  File        : mmps_uart.c
//  Description : UART Functions.
//  Author      : Ben Lu
//  Revision    : 1.0
//
//==============================================================================
/**
 *  @file mmps_uart.c
 *  @brief The uart control functions
 *  @author Ben Lu
 *  @version 1.0
 */
//==============================================================================
//
//                              INCLUDE FILE
//
//============================================================================== 
 
#include "mmp_lib.h"
#include "mmps_uart.h"
#include "ait_utility.h"
#include "mmpd_uart.h"

//==============================================================================
//
//                              FUNCTIONS
//
//==============================================================================
//------------------------------------------------------------------------------
//  Function    : MMPS_Uart_Open
//  Description :
//  Note        :
//------------------------------------------------------------------------------
MMP_ERR MMPS_Uart_Open(MMP_UART_ID uart_id, MMP_UART_ATTR *uartAttr)
{
	return MMPD_Uart_Open(uart_id, uartAttr);
}

//------------------------------------------------------------------------------
//  Function    : MMPS_Uart_Write
//  Description : Debug output a string
//  Note        :
//------------------------------------------------------------------------------
MMP_ERR MMPS_Uart_Write(MMP_UART_ID uart_id, char *pWrite_Str, MMP_ULONG ulLength)
{
	return MMPD_Uart_Write(uart_id, pWrite_Str, ulLength);
}

//------------------------------------------------------------------------------
//  Function    : MMPS_Uart_Close
//  Description :
//  Note        :
//------------------------------------------------------------------------------
MMP_ERR MMPS_Uart_Close(MMP_UART_ID uart_id)
{
	return MMPD_Uart_Close(uart_id);
}

//------------------------------------------------------------------------------
//  Function    : MMPS_UART_UseTxDMA
//  Description :
//  Note        : UART tx_dma mode API, the only one API for upper layer.
//------------------------------------------------------------------------------
/** @brief UART tx_dma mode API, the only one API for upper layer.

UART tx_dma mode API, the only one API for upper layer.
@param[in] uart_id indicate which UART device, please refer the data structure of MMP_UART_ID
@param[in] ulTxStartAddr indicate the Tx DMA start address.
@param[in] usTxTotalByte indicate number of bytes would be sent (start from start address).
@param[in] uart_int_mode indicate which DMA interrupt mode to be used, please refer the data structure MMP_UART_DMA_INT_MODE.
@param[in] bEnableInt indicate enable interrup mode or not
@param[in] callBackFunc is the callback function when interrupt occur.
@return It reports the status of the operation.
*/
MMP_ERR MMPS_UART_UseTxDMA(MMP_UART_ID uart_id, MMP_ULONG ulTxStartAddr, MMP_USHORT usTxTotalByte,  MMP_UART_DMA_INT_MODE uart_int_mode, MMP_BOOL bEnableInt, void* callBackFunc)
{
	return MMPD_UART_UseTxDMA(uart_id, ulTxStartAddr, usTxTotalByte, uart_int_mode, bEnableInt, callBackFunc);
}

//------------------------------------------------------------------------------
//  Function    : MMPS_UART_UseRxDMA
//  Description :
//  Note        : UART rx_dma mode API, the only one RX API for upper layer.
//------------------------------------------------------------------------------
/** @brief UART rx_dma mode API, the only one RX API for upper layer.

UART rx_dma mode API, the only one RX API for upper layer.
@param[in] uart_id indicates which UART device, please refer the data structure of MMP_UART_ID
@param[in] bRingModeEn indicates to use ring mode or not.
@param[in] ulRxStartAddr indicate the RX DMA start address.
@param[in] ulRxEndAddr indicate the RX DMA End address.
@param[in] ulRxLowBoundAddr indicate the RX lower bound address (Using by RX DMA Ring Mode). 
@param[in] uart_int_mode indicate which DMA interrupt mode to be used, please refer the data structure MMP_UART_DMA_INT_MODE.
@param[in] bEnableInt indicate enable interrup mode or not
@param[in] callBackFunc is the callback function when interrupt occur.
@param[in] usRxThreshold which is used in ring mode.
@param[in] ubRxTimeOut is used in RX interrupt mode (MMPS_UART_RXDMA_WRMEM_EN & MMPS_UART_RXDMA_DROPDATA_EN)
@return It reports the status of the operation.
*/
MMP_ERR MMPS_UART_UseRxDMA(MMP_UART_ID uart_id, MMP_BOOL bRingModeEn, MMP_ULONG ulRxStartAddr, MMP_ULONG ulRxEndAddr, MMP_ULONG ulRxLowBoundAddr,  MMP_UART_DMA_INT_MODE uart_int_mode, 
						   MMP_BOOL bEnableInt, void* callBackFunc, MMP_USHORT usRxThreshold, MMP_UBYTE ubRxTimeOut)
{
	return MMPD_UART_UseRxDMA(uart_id, bRingModeEn, ulRxStartAddr, ulRxEndAddr, ulRxLowBoundAddr, uart_int_mode, bEnableInt, callBackFunc, usRxThreshold, ubRxTimeOut);
}
