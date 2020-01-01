/**
 @file mmpf_player.h
 @brief Header function of component template
 @author Truman Yang
 @version 1.0 Original Version
*/

#ifndef _MMPF_PLAYER_H_
#define _MMPF_PLAYER_H_
#include "includes_fw.h"
#include "mmpf_media_struct.h"
#include "mmpf_timer.h"
#include "mmpf_media_core.h"
#include "mmpf_3gpparser.h"
#include "mmpf_vpp.h"

/// Video renderer private data
typedef struct {
    MMP_VID_AUDIOTYPE           sSupportedAudioFmt; 	///< The audio format supported in current firmware
    MMP_VID_VIDEOTYPE           sSupportedVideoFmt; 	///< The video format supported in current firmware
    MMP_VIDEO_CONTAINER_INFO    *info;                  ///< Information of the player
    MMP_ULONG                   ulAudioTimeBase;        ///< Audio time base in 3GP, used for audio time
    MMP_ULONG                   ulLastRefreshTime;      ///< Record the latest refresh time for AV sync
    MMP_M_STATE                 state;                  ///< The actual state in video player
    //MMP_USHORT                  sTVoutEn;               ///< If the video player is using TV
    MMPF_3GPPLAY_OUTPUTPANEL    PanelType;              ///< Panel type
    MMP_ULONG                   LastKeyFrameNum;
}MMPF_PLAYER_PRIVATE_DATA;
//==============================================================================
//                                IPC SECTION
//==============================================================================
/** @addtogroup MMPF_Player
@{
*/

//==============================================================================
//
//                              FUNCTION PROTOTYPES
//
//==============================================================================
// CONSIDER TO BECOME LOCAL FUNCTIONS
typedef void VideoPlayerCallBackFunc(void *VidContext, MMP_ULONG flag1, MMP_ULONG flag2);

void MMPF_Player_SetAudioDecodeOp(const MMP_USHORT sAudioCommand, const MMP_BOOL bBlock);


#if (VIDEO_P_EN)
void    MMPF_3GPP_RewindAudioBuffer (void);
void    MMPF_Player_SetClockTime(const MMP_ULONG ulCurTime);
#else
#define MMPF_3GPP_RewindAudioBuffer() do{} while(0);
#endif
const   MMP_M_STATE MMPF_Player_GetState(void);
void    MMPF_Player_SetRefreshTime(const MMP_ULONG ulCurTime);
void    MMPF_Player_GetLastRefreshTime(MMP_ULONG *ulCurTime);
void    MMPF_Player_SetLastAudioTime(MMP_ULONG ulTime);
MMP_ERR MMPF_Player_Stop(void);
MMP_ERR MMPF_Player_Flush(const MMP_BOOL bFlushVideo, const MMP_BOOL bFlushAudio, const MMP_BOOL bOnlyAudio);
MMP_ERR MMPF_Player_GetSupportedFormat(MMP_VID_AUDIOTYPE *supported_audio_format,
                                       MMP_VID_VIDEOTYPE *supported_video_format);
void    MMPF_Player_ParseDone(const MMP_BOOL bFileValid);
void    MMPF_Player_SetState(const MMP_M_STATE state);
MMP_ERR MMPF_Player_Start(void);
MMP_ERR MMPF_Player_Pause(void);
MMP_ERR MMPF_Player_Resume(void);
MMP_ERR MMPF_Player_Predecode(MMP_BOOL bDecodeAudio, MMP_BOOL bDecodeVideo);
void    MMPF_Player_Tick(void);
#if (HDMI_OUTPUT_EN==1)
MMP_ERR MMPF_Player_SetHDMIAudioOutput(MMP_ULONG ulSampleRate, MMP_UBYTE *pubShift);
MMP_ERR MMPF_Player_HDMIAudioDis(void);
#endif
void    MMPF_Player_Debug(const MMPF_3GPP_DEBUG debugcase);
MMP_ERR MMPF_Player_SendStopCmd(void);
MMP_ERR MMPF_Player_SetPlaySpeed(MMPF_3GPPLAY_PLAYSPEED_MODE mode,MMP_SHORT speed);

MMP_UBYTE MMPF_VIDPLAY_GetColorFmt(void);

/** @} */ // end of group MMPF_M
#endif
