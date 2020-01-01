//==============================================================================
//
//  File        : mmps_pwm.c
//  Description : PWM driver.
//  Author      : Ben Lu
//  Revision    : 1.0
//
//==============================================================================
/**
 *  @file mmps_pwm.c
 *  @brief The PWM driver
 *  @author Ben Lu
 *  @version 1.0
 */
//==============================================================================
//
//                              INCLUDE FILE
//
//============================================================================== 
 
#include "mmp_lib.h"
#include "mmps_pwm.h"
#include "ait_utility.h"

//==============================================================================
//
//                              FUNCTIONS
//
//==============================================================================

//------------------------------------------------------------------------------
//  Function    : MMPS_PWM_Initialize
//  Description : 
//------------------------------------------------------------------------------
/** @brief Driver init

Driver init
@return It reports the status of the operation.
*/
MMP_ERR MMPS_PWM_Initialize(void)
{
	return MMPD_PWM_Initialize();
}

//------------------------------------------------------------------------------
//  Function    : MMPS_PWM_OutputPulse
//  Description : 
//------------------------------------------------------------------------------
/** @brief Simplely output some pulses (According to the parameters)

Simplely output some pulses (According to the parameters)
@param[in] pwm_pin : PWM I/O pin selection, please refer MMPS_PWMPIN
@param[in] bEnableIoPin : enable/disable the specific PWM pin
@param[in] ulFreq : the pulse frequency in unit of Hz.
@param[in] bHigh2Low : MMP_TRUE: High to Low pulse, MMP_FALSE: Low to High pulse
@param[in] bEnableInterrupt : enable interrupt or not
@param[in] pwm_callBack : call back function when interrupt occurs
@param[in] ulNumOfPulse : number of pulse, 0 stand for using PWM auto mode to generate infinite pulse.
@return It reports the status of the operation.
*/
MMP_ERR MMPS_PWM_OutputPulse(MMP_PWM_PIN pwm_pin, MMP_BOOL bEnableIoPin,
                             MMP_ULONG ulFreq, MMP_ULONG ulDuty,
                             MMP_BOOL bHigh2Low, MMP_BOOL bEnableInterrupt,
                             PwmCallBackFunc *cb, MMP_ULONG ulNumOfPulse)
{
	return MMPD_PWM_OutputPulse(pwm_pin, bEnableIoPin, ulFreq, ulDuty,
	                            bHigh2Low, bEnableInterrupt, cb, ulNumOfPulse);
}

