/// @ait_only
/** @file mmpd_rmparser.c
@brief RMFF Parser Control driver and host side parser
@author Truman Yang
@author Champ Yen,
@version 1.0 Original version
*/

/** @addtogroup MMPD_3GPPSR
 *  @{
 */

/** @defgroup MMPD_3GPPLAY_memory MMPD_3GPPLAY (Memory Mode)
 *  @brief This module is for memory mode video play only.
 */

/** @addtogroup MMPD_3GPPLAY_memory
 *  @{
 */

#include "mmp_lib.h"
#include "mmpd_rmparser.h"
#include "mmpd_3gpparser.h"
#include "mmph_hif.h"
#include "ait_utility.h"// for custom_memcpy
#include "mmpd_system.h"
#include "mmpd_vidplayer.h"
#include "mmp_3gpparser.h"
#include "mmpd_h264dec.h"
#include "mmps_vidplay.h"

#if MMP_RMPARSER_SUPPORT

typedef struct mp4AudioSpecificConfig
{
    /* Audio Specific Info */
    MMP_UBYTE   objectTypeIndex;
    MMP_UBYTE   samplingFrequencyIndex;
    MMP_ULONG   samplingFrequency;
    MMP_UBYTE   channelsConfiguration;

    /* GA Specific Info */
    MMP_UBYTE   frameLengthFlag;
    MMP_USHORT  sample256_time;

} mp4AudioSpecificConfig;


extern MMP_VIDEO_CONTAINER_INFO    		gVidFileInfo;
extern mp4AudioSpecificConfig          	mp4ASC; 
extern MMP_3GP_MEDIA_INFO       		gAudioMedia;
extern MMP_UBYTE                       	gDemuxOption;

//extern function
extern MMP_VID_FILEVALIDATION MMPF_RMPSR_Parser(void);
extern MMP_ERR MMPF_RMPSR_Demux(void);

/** @brief Init the module used data

@param[in] ulDefaultBSAddress Default bitstream address to load the data for parsing.
           Set 0 if the memory is not enough.
@return Always return success
*/
MMP_ERR MMPD_RMPSR_Init(const MMP_ULONG ulDefaultBSAddress)
{
    MMP_ERR error = MMP_ERR_NONE;

    return error;
}

/** @brief Parsing avi file
@param[in] file_size file size
@param[out] pFileValidation File validation from the firmware
@param[in] bCardMode card mode or mem mode
@return Always return success now.
*/
MMP_ERR MMPD_RMPSR_Parse(const MMP_ULONG file_size,MMP_VID_FILEVALIDATION *fileValidation,MMP_BOOL bCardMode)
{
	MMP_ERR	mmpstatus;

    MMPD_VIDPSR_SetSWVidWorkingBuf();

	MMPD_System_EnableClock(MMPD_SYS_CLK_H264, MMP_TRUE);
        	
    if (bCardMode) {
    	// Notify the firmware to parse
    	MMPH_HIF_WaitSem(GRP_IDX_VID, 0);
        mmpstatus = MMPH_HIF_SendCmd(GRP_IDX_VID, HIF_VID_CMD_PARSER | VIDPLAY_PARSE);

		if (mmpstatus == MMP_ERR_NONE) {
        	mmpstatus = MMPH_HIF_GetParameterL(GRP_IDX_VID, 0);
        }
		if (mmpstatus == MMP_ERR_NONE) {
	        // return simple ULONG value from the host
    	    *fileValidation = (MMP_VID_FILEVALIDATION)MMPH_HIF_GetParameterL(GRP_IDX_VID, 4);
        }
        MMPH_HIF_ReleaseSem(GRP_IDX_VID);
        
        MMPD_VIDPSR_GetSWVidWorkingBufSize();

    	MMPD_System_EnableClock(MMPD_SYS_CLK_H264, MMP_FALSE);
    	
        return mmpstatus;
    }
    else {
        gVidFileInfo.file_size = file_size;

    	*fileValidation = MMPF_RMPSR_Parser();
    	
    	MMPD_System_EnableClock(MMPD_SYS_CLK_H264, MMP_FALSE);

    	if (MMP_VID_FILE_VALID == *fileValidation) {
    		PRINTF("Parsed OK\r\n");
    		// set the player state into idle
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_PLAYER_STATE, MMP_M_STATE_IDLE);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_VIDEO_WIDTH, gVidFileInfo.video_info[0].width);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_VIDEO_HEIGHT, gVidFileInfo.video_info[0].height);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_AUDIO_SAMPLING_RATE, mp4ASC.samplingFrequency);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_AUDIO_TIME_BASE, gAudioMedia.ulTimeScale);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_AUDIO_FORMAT, gVidFileInfo.audio_info.format);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_VIDEO_FORMAT, gVidFileInfo.video_info[0].format);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_VIDEO_TOTAL_TIME, gVidFileInfo.total_file_time);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_VIDEO_AVAILABLE, gVidFileInfo.is_video_available[0]);
    		MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_AUDIO_AVAILABLE, gVidFileInfo.is_audio_available);
    		//MMPD_VIDPSR_SetParameter(MMPD_VIDPLAY_PARAMETER_DECODE_AUDIO, gVidFileInfo.bDecodeAudio);
    		//glAudioSizeRemain = gl3gpAudioSize; 
    	}
    	else {
    	
    		PRINTF("Parsed Failed (%d)\r\n", *fileValidation);
    	}
            
    	if (MMP_VID_UNSUPPORTED_FILE == *fileValidation) {
        	
    		return MMP_VIDPSR_ERR_FORMAT_NOT_DETECTED;
    	}
    	else if (MMP_VID_FILE_VALID != *fileValidation) { // parse error
        	
    		return MMP_VIDPSR_ERR_UNSUPPORTED_SETTING;
    	}
    	else {
        	
    		return MMP_ERR_NONE;
    	}
	   	
        return MMP_VIDPSR_ERR_PARAMETER;
    }
}

/** @brief Playback control: Predecode
@param[in]  bCardMode card mode or mem mode
@param[in]  bDecodeAudio decode audio or not
@param[out] sample_rate audio sampling rate
@pre The firmware is loaded and free to response
@return The status from the firmware
*/
MMP_ERR MMPD_RMPSR_Predecode(MMP_BOOL bCardMode, MMP_BOOL bDecodeVideo,MMP_BOOL bDecodeAudio, MMP_ULONG *sample_rate)
{
    MMP_ERR error;
    MMP_ERR	mmpstatus;
    
    if (!bCardMode) {
        gDemuxOption = 0;

    	if ( (gVidFileInfo.is_audio_available) && (bDecodeAudio) ){
    		//gAudioSample.ulOffset = 1;
    		gDemuxOption |= DEMUX_AUDIO;
    	}
    	if (gVidFileInfo.is_video_available) {
    		gDemuxOption |= DEMUX_VIDEO;
    	}

    	error = MMPF_RMPSR_Demux();
        if(error != MMP_ERR_NONE){
        	PRINTF("%s MMPF_RMPSR_Demux error\n" ,__func__);
            return error;
        }
    }
    
    MMPH_HIF_WaitSem(GRP_IDX_VID, 0);
    MMPH_HIF_SetParameterL(GRP_IDX_VID, 4, bDecodeAudio);
    mmpstatus = MMPH_HIF_SendCmd(GRP_IDX_VID, VIDPLAY_PREDECODE | HIF_VID_CMD_PARSER);
	if (mmpstatus == MMP_ERR_NONE) {
		mmpstatus = MMPH_HIF_GetParameterL(GRP_IDX_VID, 0);
	}
	if (mmpstatus == MMP_ERR_NONE) {
	    *sample_rate = MMPH_HIF_GetParameterL(GRP_IDX_VID, 4);
	}
	MMPH_HIF_ReleaseSem(GRP_IDX_VID);   
    
    return mmpstatus;
}


/** @brief Seek the video by time in millisecond.

@param[in] ulTargetTime      Time in millisecond
@param[in] bCardMode         card mode or mem mode
@param[in] bSeekToPrecisePos Seek to precise position or not (I frame only)
@return The status of the function call
@retval MMP_M_ERR_BAD_PARAMETER When the percentage is out of range.
*/
MMP_ERR MMPD_RMPSR_SeekByTime(const MMP_ULONG ulTime,MMP_BOOL bCardMode, MMP_BOOL bSeekToPrecisePos)
{
	MMP_ERR	mmpstatus;

    if (bCardMode) {
        MMPH_HIF_WaitSem(GRP_IDX_VID, 0);
        MMPH_HIF_SetParameterL(GRP_IDX_VID, 4, MMPD_VIDPLAY_PARAMETER_SEEK_BY_TIME);
        MMPH_HIF_SetParameterL(GRP_IDX_VID, 8, ulTime);
        MMPH_HIF_SetParameterL(GRP_IDX_VID, 12, bSeekToPrecisePos);
        
        mmpstatus = MMPH_HIF_SendCmd(GRP_IDX_VID, HIF_VID_CMD_VIDPLAY_OTHER | VIDPLAY_SET_PARAMETER);

		if (mmpstatus == MMP_ERR_NONE) {
			mmpstatus = MMPH_HIF_GetParameterL(GRP_IDX_VID, 0);
		}
	    MMPH_HIF_ReleaseSem(GRP_IDX_VID);

        return mmpstatus;
    }
    else {
        return MMP_ERR_NONE;
    }
}

/** @brief Serve the data in the memory mode

This function serves data to the device.
@param[out] pbFinished If the data request from firmware finished or not. Set
                      The data service is finished. It's requested from the firmware.
                      E.g. End of parsing. End of playing.
@return The status of the function call.
*/
MMP_ERR MMPD_RMPSR_FeedData(MMP_BOOL *pbFinished)
{
	MMP_ERR error = MMP_ERR_NONE;
	MMP_M_STATE state;

    if(gVidFileInfo.is_audio_available)
	    gDemuxOption |= DEMUX_AUDIO;
	
	if(gVidFileInfo.is_video_available)
	    gDemuxOption |= DEMUX_VIDEO;
	
	error = MMPF_RMPSR_Demux();
	// unchecked error

	// update previous write header because it might be modified during demux
	//@warning assume that the firmware and the host side header size are the same by compiler
	//MMPH_HIF_MemCopyHostToDev(((MMP_UBYTE*)m_3GPPdata.pPreWriteHeader) - m_pBsBufInfo->buf + m_vidpmem.ulBSAddress,
    //                              (MMP_UBYTE*)m_3GPPdata.pPreWriteHeader, sizeof (MMPF_M_FRAME_INFO));

	
	error = MMPD_VIDPLAYER_GetState(&state);
	*pbFinished = (state == MMP_M_STATE_IDLE);
		
    return error;
}
/// @end_ait_only
/** @} */ // end of MMPD_3GPPLAY

#else

MMP_ERR MMPD_RMPSR_Init(const MMP_ULONG ulDefaultBSAddress)
{
	return MMP_ERR_NONE;
}

MMP_ERR MMPD_RMPSR_Parse(const MMP_ULONG file_size,MMP_VID_FILEVALIDATION *fileValidation,MMP_BOOL bCardMode)
{
	return MMP_VIDPSR_ERR_FORMAT_NOT_DETECTED;
}

MMP_ERR MMPD_RMPSR_Predecode(MMP_BOOL bCardMode, MMP_BOOL bDecodeVideo,MMP_BOOL bDecodeAudio, MMP_ULONG *sample_rate)
{
	return MMP_ERR_NONE;
}

MMP_ERR MMPD_RMPSR_SeekByTime(const MMP_ULONG ulTime,MMP_BOOL bCardMode, MMP_BOOL bSeekToPrecisePos)
{
	return MMP_ERR_NONE;
}

MMP_ERR MMPD_RMPSR_FeedData(MMP_BOOL *pbFinished)
{
	return MMP_ERR_NONE;
}

#endif

