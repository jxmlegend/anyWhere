/**
 @file mmpf_3gpmgr.h
 @brief Header function of 3gp processor
 @author Will Tseng, Truman Yang
 @version 1.1 Add constant and struction section to sync with the host
 @version 1.0 Original Version
*/

#ifndef _MMPF_3GPMGR_H_
#define _MMPF_3GPMGR_H_

//==============================================================================
//
//                              INCLUDE FILE
//
//==============================================================================

#include "includes_fw.h"
#include "mmp_media_def.h"
#include "mmp_vidrec_inc.h"

/** @addtogroup MMPF_3GPP
@{
*/

//==============================================================================
//
//                              COMPILER OPTION
//
//==============================================================================

#define MGR_SUPPORT_AVI             (1)
#define MGR_SUPPORT_AVI_IDX1        (1)

#define MGR_AUDIO_DATA_FLAG         (0xFF000000)
#define MGR_AUDIO_DATA_MASK         (0x00FFFFFF)
#if (VIDRECD_MULTI_TRACK == 1)
#define MGR_VIDEO_DATA_MASK         (0x0FFFFFFF)
#define MGR_TRACK_FLAG              (0x30000000)
#else
#define MGR_VIDEO_DATA_MASK         (0x3FFFFFFF)
#define MGR_TRACK_FLAG              (0x30000000)
#endif

#define MGR_FRAME_TYPE_MASK         (0xC0000000)
#define MGR_FRAME_TYPE_I            (0x40000000)
#define MGR_FRAME_TYPE_P            (0x80000000)
#define MGR_FRAME_TYPE_B            (0xC0000000)

#define MGR_AUD_FRM_HDR_SIZE        (2+4) // Size:2 Bytes, Time:4 Bytes

#define CHUNK_VIDEO                 (0)
#define CHUNK_AUDIO                 (1)
#define CHUNK_VIDEO_2               (2)

#define UVC_VIDEO_FRAME             (0)
#define UVC_AUDIO_FRAME             (1)

#define FRAME_TABLE                 (0)
#define TIME_TABLE 	                (1)

#if (EMERGENTRECD_SUPPORT)
#define KEEP_CONTAINER_GAP          (10)
#endif

/* MIO player can't play the recording file for file has plenty of audio frames
 * in beginning file. Limit the audio frames to MAX_AUDIO_FRAMEBUF.
 */
#define MGR_AUDIO_SUPPORT_MIOPLAYER (1)
#define MAX_AUDIO_FRAMEBUF          (3)

#if (SUPPORT_VR_REFIX_TAILINFO)
#define FIXED_TAIL_SAVE_HEAD        (0)
#define FIXED_TAIL_SAVE_CONTAINER   (8)
#define FIXED_TAIL_SAVE_TAILSIZE    (8+sizeof(MMPF_CONTAINER_INFO))
#define FIXED_TAIL_SAVE_TAILDATA    (8+sizeof(MMPF_CONTAINER_INFO)+4)
#define FIXED_TAILINFO_FILESIZE     (4*1024*1024)
#define FIXED_TAIL_SAVED_AVNUM      (10)
#endif

/* During muxing audio frames, break muxing audio while the muxed audio time
 * will be longer than muxed video time
 */
#define MGR_AUD_FIT_VID_MUX_TIME    (0) 

/*
 * Insert SPS and PPS as the dummy frame
 */
#define MGR_SPSPPS_AS_DUMMY_FRM     (0)

/**
 @brief Select AVI muxer version to use
*/
#define AVI_1_0_MUXER               (1)
#define AVI_2_0_MUXER               (2)

#if (VIDRECD_MULTI_TRACK == 1) // Currently only AVI 1.0 supports multi-tracks
#define AVI_VERSION                 AVI_1_0_MUXER   ///< supported AVI muxer version
#else
#define AVI_VERSION                 AVI_2_0_MUXER   ///< supported AVI muxer version
#endif

#if (AVI_VERSION == AVI_2_0_MUXER)
#define AVI_SEG_INDEXES             (1)   ///< several segmented sub-indexes to speed up post processing
#if (VIDENC_SEAMLESS == 1) && (AVI_SEG_INDEXES == 0)
    #error To support seamless video recording, segmented subindex feature must be enabled
#endif
#else
#define AVI_SEG_INDEXES             (0)   ///< only one sub-index for each stream
#endif

/**
 @brief predefined num of entries per sub-index & per super index
*/
#if (AVI_SEG_INDEXES == 1)
#define AVI_STD_INDEX_ENTRY_CNT     (900)   ///< 30fps * 30 sec
///< For 5-hr video, 600 * 30 sec, prefer size of super index entries is 512x for performance
#define AVI_SUPER_INDEX_ENTRY_CNT   (640)   ///< 640 * 16 byte per entry
#else
#define AVI_SUPER_INDEX_ENTRY_CNT   (1)     ///< 1 * 16 bytes per entry
#endif

/* Storage Range Type */
#if (SUPPORT_SHARE_REC)
#define MMPF_3GPMGR_SAVE_AV_PART            (~(MMP_ULONG)0x3ffff)   ///< mask of moving 256K bytes AV data
#else
#define MMPF_3GPMGR_SAVE_AV_PART            (~(MMP_ULONG)0xffff)    ///< mask of moving 64K bytes AV data
#endif
#define MMPF_3GPMGR_SAVE_AV_FULL            ((MMP_ULONG)0xffffffff) ///< mask of moving all AV data
#define MMPF_3GPMGR_SAVE_FT_PART            (0)     ///< flag of moving 512 bytes frame table
#define MMPF_3GPMGR_SAVE_FT_FULL            (1)     ///< flag of moving all frame table

/* Frame Info Queue Size */
#define MMPF_3GP_ENCODE_FRAME_QUEUE_SIZE    (1023)  ///< queue of frame size for data transfer
#define MMPF_UVCRECD_FRAME_QUEUE_SIZE       (1023)  ///< queue of frame size for data transfer
#define MMPF_DUALENC_FRAME_QUEUE_SIZE       (1023)  ///< queue of frame size for data transfer
#define MMPF_WIFI_FRAME_QUEUE_SIZE			(1023)  ///< queue of frame size for data transfer

/* Frame Marker */
#define MMPF_3GPMGR_EOF_MARKER              (0xFFFFFE0F)
#define MMPF_3GPMGR_TIMETBL_HDR_MARKER      (0xFFFFFFFF)

//==============================================================================
//
//                              ENUMERATION
//
//==============================================================================

// Video Merger TX Status
typedef enum _MMPF_VIDMGR_TX_STAT {
    MMPF_VIDMGR_TX_STAT_NONE = 0,
    MMPF_VIDMGR_TX_STAT_OPEN,
    MMPF_VIDMGR_TX_STAT_CLOSE
} MMPF_VIDMGR_TX_STAT;

// Video Callback Type
typedef enum _MMPF_3GPMGR_CB_TYPE {
    MMPF_3GPMGR_CB_NONE             = 0x0000,
    MMPF_3GPMGR_MEDIA_FULL          = 0x0001,
    MMPF_3GPMGR_SIZE_MEET           = 0x0002,
    MMPF_3GPMGR_TIME_MEET           = 0x0004,
    MMPF_3GPMGR_SLOW_MEDIA          = 0x0008,
    MMPF_3GPMGR_SEAMLESS            = 0x0010,
    MMPF_3GPMGR_MEDIA_ERROR         = 0x0020, 
    MMPF_3GPMGR_EMERG_TIMEMEET      = 0x0040, 
    MMPF_3GPMGR_APSTOP_VIDRECD      = 0x0080,
    MMPF_3GPMGR_RECDSTOP_CARDSLOW   = 0x0100,
    MMPF_3GPMGR_UVCRECD_TIMEMEET    = 0x0200,
    MMPF_3GPMGR_DUALENC_TIMEMEET    = 0x0400,
    MMPF_3GPMGR_LONG_TIME_SIZE_MEET = 0x0800,
    MMPF_3GPMGR_CB_NUM
} MMPF_3GPMGR_CB_TYPE;

// Stream Callback Type
typedef enum _MMPF_VIDRECD_STREAMCB_TYPE {
    MMPF_VIDRECD_STREAMCB_VIDMOVE = 0,      // move video stream
    MMPF_VIDRECD_STREAMCB_VIDUPDATE,        // update video stream
    MMPF_VIDRECD_STREAMCB_AUDMOVE,          // move audio stream
    MMPF_VIDRECD_STREAMCB_AUDUPDATE         // update audio stream
} MMPF_VIDRECD_STREAMCB_TYPE;

#if (UVC_VIDRECD_SUPPORT)
// UVC File Store Type
typedef enum _MMPF_UVCRECD_FILE_TYPE {
    MMPF_UVCRECD_MULFILE = 0,
    MMPF_UVCRECD_MULTRACK
} MMPF_UVCRECD_FILE_TYPE;
#endif

#if (SUPPORT_VUI_INFO)
typedef enum _MMPF_SEI_SHUTTER_TYPE {
    MMPF_SEI_SHUTTER_1920x1080_29d97 = 0,
    MMPF_SEI_SHUTTER_1280x720_59d94,
    MMPF_SEI_SHUTTER_1280x720_29d97,
    MMPF_SEI_SHUTTER_848x480_29d97,
    MMPF_SEI_SHUTTER_NONE
} MMPF_SEI_SHUTTER_TYPE;

typedef struct _MMPF_SEI_SHUTTER_TABLE {
    MMP_ULONG  ulShuSpd;
    MMP_USHORT usValue;
} MMPF_SEI_SHUTTER_TABLE;
#endif

#if (SUPPORT_VR_REFIX_TAILINFO)
typedef enum _MMPF_REFIXRECD_UPDATE {
    MMPF_REFIXRECD_UPDATE_VIDFRAME = 0,
    MMPF_REFIXRECD_UPDATE_AUDFRAME,
    MMPF_REFIXRECD_UPDATE_TAILINFOSIZE
} MMPF_REFIXRECD_UPDATE;
#endif

//==============================================================================
//
//                              STRUCTURES
//
//==============================================================================

/// ptr of AV repack buffer, frame tabel buffer and audio compressed buffer.
typedef struct _AV_BUFFER_INFO {
    MMP_USHORT      ttableReadPtr;      // 0
    MMP_USHORT      ttableWritePtr;     // 2
    MMP_USHORT      ttableReadWrap;     // 4
    MMP_USHORT      ttableWriteWrap;    // 6
    MMP_USHORT      ftableReadPtr;      // 8
    MMP_USHORT      ftableWritePtr;     // 10
    MMP_USHORT      ftableReadWrap;     // 12
    MMP_USHORT      ftableWriteWrap;    // 14
    MMP_USHORT      videoReadL2Byte;    // 16
    MMP_USHORT      videoReadH2Byte;    // 18
    MMP_USHORT      audioReadL2Byte;    // 20
    MMP_USHORT      audioReadH2Byte;    // 22
    MMP_USHORT	    _reserved[4];
} AV_BUFFER_INFO;

/**
 @brief AVI OpenDML standard index entry
*/
typedef __packed struct {
    MMP_ULONG       ulOffset;           ///< offset is relative to ullBaseOffset of @ref AVI_STD_INDEX
    MMP_ULONG       ulsize;             ///< the size of the data
} AVI_STD_INDEX_ENTRY;

/**
 @brief AVI OpenDML super index entry
*/
typedef __packed struct {
    MMP_ULONG64     ullOffset;          ///< from the start of the file to the sub-index
    MMP_ULONG       ulsize;             ///< size of the sub-index, in bytes
    MMP_ULONG       ulDuration;         ///< duration covered by the sub-index, in stream ticks.
} AVI_SUPER_INDEX_ENTRY;

typedef struct _MMPF_AVIMUX_HANDLER {
    MMP_ULONG       ulHdrlSize;
    MMP_ULONG       ulVidStrlSize;
    MMP_ULONG       ulVidStrhOffset;
    MMP_ULONG       ulVidStrfOffset;
    MMP_ULONG       ulVidStrfSize;
    MMP_ULONG       ulAudStrlSize;
    MMP_ULONG       ulAudStrhOffset;
    MMP_ULONG       ulAudStrfOffset;
    MMP_ULONG       ulAudStrfSize;
    MMP_ULONG       ulAudStrlJunkSize;
    #if (AVI_IDIT_CHUNK_EN)
    MMP_ULONG       ulIditOffset;
    #endif
    MMP_ULONG       ulMoviOffset;
    #if (AVI_VERSION == AVI_2_0_MUXER)
    MMP_ULONG       ulVidIndxOffset;
    MMP_ULONG       ulVidIndxSize;
    MMP_ULONG       ulAudIndxOffset;
    MMP_ULONG       ulAudIndxSize;
    #if (AVI_SEG_INDEXES == 0)
    MMP_ULONG64     ulVidCurIxOffset;               ///< file offset to current video sub-index
    MMP_ULONG64     ulAudCurIxOffset;               ///< file offset to current audio sub-index
    #else
    MMP_ULONG       ulVidIxCnt;                     ///< total number of video sub-indexes
    MMP_ULONG       ulAudIxCnt;                     ///< total number of audio sub-indexes
    MMP_ULONG       ulVidIxEntryCnt;                ///< total number of entries in current video sub-index
    MMP_ULONG       ulAudIxEntryCnt;                ///< total number of entries in current audio sub-index
    MMP_ULONG       ulIxRelativeOffset;             ///< relative offset to data chunk in sub-index entries
    MMP_ULONG64     ullIxBaseOffset;                ///< the base offset to the data chunk in sub-index
    #endif
    #endif
} MMPF_AVIMUX_HANDLER;

typedef struct _MMPF_CONTAINER_INFO {
    MMP_BOOL        bVideoTrackAvailable;           ///< is there any video track
    MMP_BOOL        bAudioTrackAvailable;           ///< is there any audio track
    MMP_BOOL        bWaitKeyFrame;
    MMP_ULONG       glVideoDataFetch;	            ///< data size that sent to repack buffer
    MMP_ULONG       glAudioDataFetch;	            ///< data size that sent to repack buffer
    MMP_ULONG       glAVSizeProduced;	            ///< AV data size that sent to repack buffer
    MMP_ULONG       gl3gpMgrFinalFileSize;          ///< final encoded file size
    MMP_ULONG       gl3gpMgrCurFileSize;

    MMP_USHORT      VideoEncodeFormat;              ///< video encode format
    MMP_ULONG       ulFrameWidth;                   ///< video frame width
    MMP_ULONG       ulFrameHeight;                  ///< video frame height
    VIDENC_FPS_CTL  FrameRate;                      ///< video frame rate
    
    // GOP Information
    MMP_USHORT	    gsGOPSize;                      ///< GOP size
    MMP_USHORT	    gsPFrameCount;                  ///< P frames count in one GOP
    MMP_USHORT      gsBFrameCount;                  ///< # consecutive B frames

    MMP_ULONG       glVidRecdFrameCount;            ///< accumulated encoded frame count
    MMP_ULONG       glVideoChunkCount;              ///< video chunk count
    MMP_ULONG       glVideoTotalTime;               ///< video total time, update every frame

    MMP_ULONG       gl3GPCreateTime;
    MMP_ULONG       gl3GPModifyTime;
    
    // Audio Encode Parameters
    MMP_USHORT      AudioEncodeFormat;              ///< audio encode format
    MMP_ULONG       glAudioSampleFreq;              ///< audio sample freq. i.e. 22050.
    MMP_ULONG       glAudioBitRate;                 ///< audio bitrate
    MMP_UBYTE       gbAudioChannelNo;               ///< audio record channel count. i.e. 1/2
    MMP_ULONG       glAudioSampleCount;             ///< audio sample count (for m_bOneAudioFramePerChunk is TRUE)
    MMP_ULONG       glAudioChunkCount;              ///< audio chunk count (for m_bOneAudioFramePerChunk is FALSE)
    MMP_ULONG       glAudioTotalTime;               ///< audio total time duration
    MMP_ULONG       ulAudioFrameCount;
    MMP_ULONG       glAMRSampleSize;                ///< audio amr frame size base on sampling rate
    
    // Video Speed Control Parameters
    MMP_UBYTE       bVidSpeedMode;
    MMP_UBYTE       bVidSpeedRatio;
    
    // Metadata
    MMP_ULONG       MetaDataLength[2];
    MMP_UBYTE       MetaData[2][64];
    MMP_USHORT      usProfile;
    MMP_USHORT      usLevel;
    MMP_ULONG       glVidRecdBaseTime;              ///< time of start capture
    
    // Frame Queue index Parameters
    MMP_ULONG       gbFrameQueueReadIndex;
    MMP_ULONG       gbFrameQueueWriteIndex;         ///< read-out and write-in index of queue
    MMP_ULONG       glFrameQueueReadWrap;
    MMP_ULONG       glFrameQueueWriteWrap;          ///< wrap count for frame queue
    
    // Video Compress Buffer Parameters
    MMP_ULONG       VidCompBufFreeSizeStatus;       ///< compress buffer free size status
    MMP_ULONG       VidRecdCompBufAddr;             ///< video recording buffer address
    MMP_ULONG       VidRecdCompBufSize;				///< video recording buffer size
    MMP_ULONG       glVidRecdCompBufReadPtr;        ///< read-out address of video encoded compressed buffer
    MMP_ULONG       glVidRecdCompBufWritePtr;       ///< write-in address of video encoded compressed buffer
    MMP_ULONG       glVidRecdCompBufReadWrap;       ///< wrap times of read ptr
    MMP_ULONG       glVidRecdCompBufWriteWrap;      ///< wrap times of write ptr
    MMP_ULONG       glVidRecdCompBufFreeSize;	    ///< maximum free size for next frame
    
    // Audio Compress Buffer Parameters
    MMP_ULONG       AudRecdCompBufAddr;             ///< Audio recording buffer address
    MMP_ULONG       AudRecdCompBufSize;			    ///< Audio recording buffer size
    MMP_ULONG       glAudCompReadPtr;		        ///< have read-out ptr of audio compressed buffer
    MMP_ULONG       glAudCompWritePtr;		        ///< have write-in ptr of audio compressed buffer
    MMP_ULONG       glAudCompReadWrap;	            ///< wrap times of read-out ptr
    MMP_ULONG       glAudCompWriteWrap;	            ///< wrap times of write-in ptr
    MMP_ULONG       glAudCompBufTotalRead;          ///< total read-out size of audio encoded compressed buffer
    
    // Repack Buffer Parameters
    MMP_ULONG       glAVRepackBufStart;             ///< start address of AV repack buffer
    MMP_ULONG       glAVRepackBufSize;	            ///< size of AV repack buffer
    MMP_ULONG       glAVRepackReadPtr;		        ///< have read-out ptr of AV repack buffer
    MMP_ULONG       glAVRepackWritePtr;		        ///< have write-in ptr of AV repack buffer
    MMP_ULONG       glAVRepackReadWrap;	            ///< wrap times of read-out ptr
    MMP_ULONG       glAVRepackWriteWrap;	        ///< wrap times of write-in ptr
	
    // Aux Table index-paras for high speed mode
    MMP_ULONG       glSpeedAuxTableBufStart[2];		///< start address of frame table/time table buffer
    MMP_ULONG       glSpeedAuxTableBufOffset;       ///< start address of frame table/time table buffer
    MMP_ULONG       glVidTailAddr;
    MMP_ULONG       ulVidCurTailAddr;
    MMP_ULONG       glVidTailSize;
    MMP_BOOL        bVidTailHighSpeedMode;
	
    // Aux Table index Parameters
    MMP_ULONG       glAuxTableBufStart[2];		    ///< start address of frame table/time table buffer
    MMP_USHORT      gsAuxTableBufSize[2];		    ///< size of frame table/time table buffer
    MMP_USHORT      gsAuxTableReadPtr[2];		    ///< have read-out ptr of frame table/time table buffer
    MMP_USHORT      gsAuxTableWritePtr[2];		    ///< have write-in ptr of frame table/time table buffer
    MMP_USHORT      gsAuxTableReadWrap[2];		    ///< wrap times of read-out ptr
    MMP_USHORT      gsAuxTableWriteWrap[2];		    ///< wrap times of write-in ptr
    
    // File Parameters
    MMP_BYTE        bRecdFileName[256*3];
    MMP_ULONG       ulRecdFileID;
    MMP_USHORT      usStreamType;
    MMP_BOOL        bKeep2NextFileEnabled;
    MMP_ULONG       bKeep2NextFileTime;
    MMP_ULONG       ulTotalSkipCount;
    MMP_ULONG       ulContinueSkipCount;

    VIDRECD_USEMODE	VidEncUseMode;
    void            *EncInfo;
    
    // AVI-info Parameters
    #if (MGR_SUPPORT_AVI == 1)
	MMPF_AVIMUX_HANDLER      m_aviHandler;
    #endif 
    #if (AVI_SEG_INDEXES == 1)
    AVI_STD_INDEX_ENTRY      m_vidStdIndexEntries[AVI_STD_INDEX_ENTRY_CNT];
    AVI_STD_INDEX_ENTRY      m_audStdIndexEntries[AVI_STD_INDEX_ENTRY_CNT];
    AVI_SUPER_INDEX_ENTRY    m_vidSuperIndexEntries[AVI_SUPER_INDEX_ENTRY_CNT];
    AVI_SUPER_INDEX_ENTRY    m_audSuperIndexEntries[AVI_SUPER_INDEX_ENTRY_CNT];
    #endif
    
    // User Data
    MMP_BYTE        bUserDataBuf[128];
    MMP_USHORT      usUserDataLength;

    MMP_BYTE        ubHasKeep1stIdx;
    MMP_BYTE        ubInsPanaAtom;
    MMP_ULONG       ulSkipAtomSize;
    MMP_ULONG       ulThumbnailAddr;
    MMP_ULONG       ulThumbnailSize;
    MMP_ULONG       ulProgCnt;          // NO USE
    MMP_ULONG       ulThumbIdx;         // NO USE
    MMP_ULONG       ulBufNum;           // NO USE
    
    // Multiple Track
    #if (VIDRECD_MULTI_TRACK == 1)
    MMP_BOOL        ulMulTrackEn;
    MMP_ULONG       ulTrackNum;
    void            *pNextContainer;
    #endif
} MMPF_CONTAINER_INFO;

#if (SUPPORT_VR_REFIX_TAILINFO)
typedef struct _MMPF_REFIXVIDREC_CTL {
	MMP_BYTE        bRecdFileName[256*3];
	MMP_ULONG       ulRecdFileID;
	MMP_BYTE        bTailFileName[256*3];
	MMP_ULONG       ulTailFileID;
	MMP_ULONG       ulTempBufAddr;
	MMP_ULONG       ulTempBufSize;
	MMP_ULONG       ulAVRepackBufStart;
	MMP_ULONG       ulAVRepackBufSize;
	MMP_ULONG       ulVidTailAddr;
	MMP_ULONG       ulVidTailSize;
	MMP_ULONG       ulSavedPtr;
	MMP_ULONG       ulTableReadPtr[2];
	MMP_ULONG       ulTableReadWrap[2];
} MMPF_REFIXVIDREC_CTL;
#endif

//==============================================================================
//
//                              DATA TYPES
//
//==============================================================================

// Video record callback functions
typedef void        (MMPF_VIDMGR_Callback) (void);
typedef void        (MMPF_VIDMGR_EventCallback) (MMP_ULONG ulPara1, MMP_ULONG ulPara2);
typedef void        (MMPF_VIDMGR_Event2Callback) (MMP_ULONG ulPara1, MMP_ULONG ulPara2, MMP_ULONG ulPara3, MMP_ULONG ulPara4);
typedef MMP_ULONG   (MMPF_VIDMGR_MuxCallback) (MMP_ERR (*VIDMUX_Write)(void *buf, MMP_ULONG size, MMP_ULONG *wr_size, MMP_ULONG ulFileID), MMP_ULONG ulFileID);
typedef MMP_BOOL    (MMPF_VIDMGR_StreamCallback) (MMP_M_STREAMCB_ACTION cbmode, MMP_ULONG framesize, MMP_M_STREAM_INFO* moveinfo);

// Muxer modulation functions
typedef MMP_ERR     (VIDMUX_PreProcessFile_Func)(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_PostProcessFile_Func)(MMP_UBYTE storagemode, MMP_UBYTE * tmp_buf, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_FillTail_Func)(MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_GetSavedSize_Func)(MMP_ULONG AVSize, MMP_ULONG VideoCount, MMP_ULONG AudioSampleCount, MMP_ULONG AudioChunkCount, MMP_ULONG64 *total_size, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_GetRecordingSize_Func)(MMP_ULONG *ulSize, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_AddChunkPrefix_Func)(MMP_UBYTE chunktype, MMP_ULONG framesize, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_AddChunkSuffix_Func)(MMP_UBYTE chunktype, MMP_ULONG framesize, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ULONG   (VIDMUX_CalcChunkPrefixSize_Func)(MMP_UBYTE chunktype, MMP_ULONG framesize, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ULONG   (VIDMUX_CalcChunkSuffixSize_Func)(MMP_UBYTE chunktype, MMP_ULONG framesize, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_InsertDupFrame_Func)(MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_KeepIndexEntry_Func)(MMP_UBYTE chunktype, MMP_ULONG framesize, VIDENC_FRAME_TYPE frametype, MMPF_CONTAINER_INFO* pContainer);
typedef MMP_ERR     (VIDMUX_AddIndexChunk_Func)(MMPF_CONTAINER_INFO* pContainer);
typedef MMP_BOOL    (VIDMUX_IsTimeInfoNeeded_Func)(void);
typedef MMP_BOOL    (VIDMUX_IsSizeInfoNeeded_Func)(void);
typedef MMP_BOOL    (VIDMUX_IsIndexInfoNeeded_Func)(void);

typedef void        (VIDMUX_SetAudEncOp_Func)(MMP_USHORT op);
typedef MMP_ULONG   (VIDMUX_GetAudEncFrameCnt_Func)(void);
typedef void        (VIDMUX_GetAudEncParam_Func)(MMP_ULONG *bitrate, MMP_ULONG *sampleRate);
#if (VR_AUD_TIME_ALWAYS_LONGER)||(VR_MIX_FPS_WITH_AUD)
typedef void        (VIDMUX_GetAudSilenceFrm_Func)(MMP_UBYTE **bs, MMP_ULONG *size);
#endif

typedef MMP_ERR (*pfMMPF_VIDMGR_TransferData2RepackBuf)(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO *pContainer);

typedef struct _MMPF_VIDMGR_VID2REPACKBUF_ACTION {
    MMPF_CONTAINER_INFO *pCurContainer;    
    MMPF_CONTAINER_INFO *pBaseContainer;    
    MMP_ERR sVidStatus;
    MMP_ERR sAudStatus;
    pfMMPF_VIDMGR_TransferData2RepackBuf pfMMPF_VIDMGR_TransferVid2RepackBuf;                     
    pfMMPF_VIDMGR_TransferData2RepackBuf pfMMPF_VIDMGR_TransferAud2RepackBuf;                     
} MMPF_VIDMGR_VID2REPACKBUF_ACTION;

//==============================================================================
//
//                              MODULATION
//
//==============================================================================

// Muxer modulation
typedef struct _MMPF_VIDMUX_FUNC {
    /* Muxing functions */
    VIDMUX_PreProcessFile_Func      *VIDMUX_PreProcessFile;
    VIDMUX_PostProcessFile_Func     *VIDMUX_PostProcessFile;
    VIDMUX_FillTail_Func            *VIDMUX_FillTail;
    VIDMUX_GetSavedSize_Func        *VIDMUX_GetSavedSize;
    VIDMUX_GetRecordingSize_Func    *VIDMUX_GetRecordingSize;
    VIDMUX_AddChunkPrefix_Func      *VIDMUX_AddChunkPrefix;
    VIDMUX_AddChunkSuffix_Func      *VIDMUX_AddChunkSuffix;
    VIDMUX_CalcChunkPrefixSize_Func *VIDMUX_CalcChunkPrefixSize;
    VIDMUX_CalcChunkSuffixSize_Func *VIDMUX_CalcChunkSuffixSize;
    VIDMUX_InsertDupFrame_Func      *VIDMUX_InsertDupFrame;
    VIDMUX_KeepIndexEntry_Func      *VIDMUX_KeepIndexEntry;
    VIDMUX_AddIndexChunk_Func       *VIDMUX_AddIndexChunk;
    VIDMUX_IsTimeInfoNeeded_Func    *VIDMUX_IsTimeInfoNeeded;
    VIDMUX_IsSizeInfoNeeded_Func    *VIDMUX_IsSizeInfoNeeded;
    /* Audio encoder functions */
    VIDMUX_SetAudEncOp_Func         *VIDMUX_SetAudEncOp;
    VIDMUX_GetAudEncFrameCnt_Func   *VIDMUX_GetAudEncFrameCnt;
    VIDMUX_GetAudEncParam_Func      *VIDMUX_GetAudEncParam;
    #if (VR_AUD_TIME_ALWAYS_LONGER)||(VR_MIX_FPS_WITH_AUD)
    VIDMUX_GetAudSilenceFrm_Func    *VIDMUX_GetAudSilenceFrm;
    #endif
} MMPF_VIDMUX_FUNC;

//==============================================================================
//
//                              EXTERN VARIABLES
//
//==============================================================================

#if (SUPPORT_VR_THUMBNAIL)
extern VIDENC_THUMB_TARGET_CTL glThumbKeepQIdx[VIDENC_THUMB_MAX_TYPE];
extern VIDENC_THUMB_ATTR       gsVRThumbAttr[VIDENC_THUMB_MAX_TYPE][VR_THUMB_MAX_BUF_NUM];
extern MMP_ULONG               gulVRThumbWrIdx[VIDENC_THUMB_MAX_TYPE];
extern MMP_ULONG               gulVRThumbRdIdx[VIDENC_THUMB_MAX_TYPE];
extern MMP_USHORT              gusVRThumbBufNum[VIDENC_THUMB_MAX_TYPE];
#endif

//==============================================================================
//
//                              FUNCTION PROTOTYPES
//
//==============================================================================

VIDMGR_AVSYNC_METHOD MMPF_VIDMGR_GetAVSyncMethod(void);
MMP_ERR MMPF_VIDMGR_SlowCardPreGetTimeCBParameter(void);
MMP_ERR MMPF_VIDMGR_SlowCardPreGetTimeExecCB(void);
MMP_ERR MMPF_VIDMGR_StorageErrStopProcess(void);

void MMPF_VIDMGR_MediaFullEvent(void);
void MMPF_VIDMGR_FillFullEvent(void);
void MMPF_VIDMGR_EmergFillFullEvent(void);
void MMPF_VIDMGR_UVCFillFullEvent(void);
#if (DUALENC_SUPPORT)
void MMPF_VIDMGR_DualEncFillFullEvent(void);
#endif
void MMPF_VIDMGR_VidRecdCardSlowStopEvent(void);
#if (UVC_VIDRECD_SUPPORT)
void MMPF_VIDMGR_UVCVidRecdCardSlowStopEvent(void);
#endif
void MMPF_VIDMGR_PreGetTime2CardSlowStopEvent(void);
void MMPF_VIDMGR_VidRecdAPStopEvent(void);
void MMPF_VIDMGR_MediaSlowEvent(void);
void MMPF_VIDMGR_MediaErrorEvent(void);
#if (VIDENC_SEAMLESS == 1)
void MMPF_VIDMGR_SeamlessEvent(void);
#endif
void MMPF_VIDMGR_BitstreamDiscardEvent(void);

void        MMPF_VIDMGR_SetUVCH264StreamingEnable(MMP_BOOL bEnable);
MMP_BOOL    MMPF_VIDMGR_GetUVCH264StreamingEnable(void);

void        MMPF_VIDMGR_SetAVSyncBufInfo(MMP_ULONG bufaddr);
void        MMPF_VIDMGR_SetAVRepackBuffer(MMP_ULONG bufaddr, MMP_ULONG bufsize, MMPF_CONTAINER_INFO *pContainer);
void        MMPF_VIDMGR_SetAudioSyncEncode(MMP_UBYTE syncAudio);
void        MMPF_VIDMGR_SetAudioCompressedBuffer(MMP_ULONG startaddr, MMP_ULONG bufsize);
void        MMPF_VIDMGR_SetAuxTableBuffer(MMP_UBYTE ubTableType, MMP_ULONG startaddr, MMP_USHORT bufsize, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR     MMPF_VIDMGR_InsertDupFrame(MMPF_CONTAINER_INFO* pContainer);
#if (WORK_AROUND_EP3)
void        MMPF_VIDMGR_KeepSizeToFrameQueue(MMP_ULONG CurSize, MMP_ULONG CurTime, VIDENC_FRAME_TYPE CurType, MMP_BOOL bEP3, MMPF_CONTAINER_INFO* pContainer);
#else
void        MMPF_VIDMGR_KeepSizeToFrameQueue(MMP_ULONG FrameSize, MMP_ULONG FrameTime, VIDENC_FRAME_TYPE FrameType, MMPF_CONTAINER_INFO* pContainer);
#endif
#if (VIDENC_SEAMLESS)
void        MMPF_VIDMGR_PutEofMarkToFrameQueue(MMPF_CONTAINER_INFO* pContainer);
#endif
MMP_ERR MMPF_VIDMGR_GetReadableNumFromFrameQueue(MMP_ULONG *ulNum, MMPF_CONTAINER_INFO* pContainer);
void        MMPF_VIDMGR_ResetMgrState(MMP_USHORT usStreamType);
MMP_ERR	 	MMPF_VIDMGR_OpenUVCFile(MMP_USHORT usStreamType);

void        MMPF_VIDMGR_SetPreEncodeDuration(MMP_ULONG ulDurationMs, MMP_USHORT usStreamType);
void        MMPF_VIDMGR_StartAudio(void);
MMP_ERR     MMPF_VIDMGR_PreCapture(MMP_ULONG ulPreCaptureMs);
MMP_ERR     MMPF_VIDMGR_StartCapture(MMP_UBYTE encid);
void 		MMPF_VIDMGR_SetVidRecdCBType(MMPF_3GPMGR_CB_TYPE type);
void 		MMPF_VIDMGR_SetEndFrameofVidRecd(MMP_UBYTE enable, VIDENC_STREAMTYPE type);
void        MMPF_VIDMGR_SetBaseTimeCounter(MMP_ULONG timecnt, MMPF_CONTAINER_INFO *pContainer);
void		MMPF_VIDMGR_SetTimeCounter(MMP_ULONG timecnt);
MMP_ULONG   MMPF_VIDMGR_GetTimeCounter(void);
void        MMPF_VIDMGR_SetGOP(MMP_USHORT usPFrame, MMP_USHORT usBFrame, MMP_USHORT usStreamType, MMPF_CONTAINER_INFO *pContainer);
MMP_BOOL	MMPF_VIDMGR_GetSkipFrameOption(MMP_ULONG ulThreshold, MMPF_CONTAINER_INFO* pContainer);
MMP_BOOL 	MMPF_VIDMGR_GetCompBufFreeSize(MMP_ULONG *ulFreeSize, MMPF_CONTAINER_INFO* pContainer);
MMP_UBYTE   MMPF_VIDMGR_GetAudioSyncEncode(void);
MMP_ERR 	MMPF_VIDMGR_GetAudioParam(MMP_UBYTE ubEncIdx, MMP_ULONG *audsamplefre);
VIDENC_FRAME_TYPE MMPF_VIDMGR_GetVidRecdFrameType(MMP_ULONG ulEncFrameNum, MMPF_CONTAINER_INFO* pContainer);
MMP_ULONG 	MMPF_VIDMGR_AutoCountAudioTime(MMPF_CONTAINER_INFO *pContainer);
MMP_UBYTE 	MMPF_VIDMGR_GetStoragePath(void);
void        MMPF_VIDMGR_SetSlowMediaDetected(MMP_BOOL bDetected);
void 		MMPF_VIDMGR_ChkRecdStopEnable(MMP_SHORT usStreamType);
MMP_ERR     MMPF_VIDMGR_GetAudioCompBufReadPtr(MMP_ULONG *ulReadPtr, MMP_ULONG *ulReadWrap);
MMP_ERR 	MMPF_VIDMGR_AuxTableBuf2Storage(MMP_UBYTE ubTableType, MMP_UBYTE saveall, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR 	MMPF_VIDMGR_AVRepackBuf2Storage(MMP_ULONG savemask, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR 	MMPF_VIDMGR_TransferAVData2RepackBuf(MMP_UBYTE storagemode);
#if (VIDRECD_MULTI_TRACK == 1)
MMP_ERR     MMPF_VIDMGR_ContainerMultiTrackSyncInfo(MMPF_CONTAINER_INFO *pSrcContainer, MMPF_CONTAINER_INFO *pDstContainer);
#endif
#if (UVC_VIDRECD_SUPPORT)
MMP_ERR 	MMPF_VIDMGR_UVCRecdStart(MMP_ULONG pltype);
MMP_ERR 	MMPF_VIDMGR_UVCRecdInputFrame(MMP_ULONG plBufAddr, MMP_ULONG plSize, MMP_ULONG plTime, MMP_ULONG plFrType);
void        MMPF_VIDMGR_UVCRegisterForceICallback(void *pCallback);
void        MMPF_VIDMGR_UVCRegisterSlowCardStopRecord(void *pCallback);
void		MMPF_VIDMGR_UVCTrigger2CloseCurFile(void);
void        MMPF_VIDMGR_UVCSlowCard2StopVidRecd(void);
#endif
MMP_ERR 	MMPF_VIDMGR_SetAVFileSizeLimit(MMP_ULONG fileMax, MMP_ULONG reservedSize, MMP_ULONG *retSize);
MMP_ULONG	MMPF_VIDMGR_GetNextVideoFrameWriteAddr(MMPF_CONTAINER_INFO* pContainer);

MMP_ERR     MMPF_VIDMGR_GetContainerType(VIDMGR_CONTAINER_TYPE *type);
MMP_BOOL    MMPF_VIDMGR_GetSeamlessEnable(void);
void        MMPF_VIDMGR_SetSeamlessEnable(MMP_BOOL bEnable);

MMP_ERR     MMPF_VIDMGR_RegisterCallback(MMP_ULONG ulEvent, void *pCallback);
MMP_ERR     MMPF_VIDMGR_GetRegisteredCallback(MMP_ULONG ulEvent, void **pCallback);
void        MMPF_VIDMGR_DmaMoveData(MMP_ULONG ulSrcaddr, MMP_ULONG ulDstaddr, MMP_ULONG ulCount);
MMP_BOOL    MMP_VIDMGR_PumpStream(void);

#if (MGR_SUPPORT_AVI == 1)
MMP_ERR     MMPF_AVIMUX_AddChunkPrefix(MMP_UBYTE chunktype, MMP_ULONG framesize, MMPF_CONTAINER_INFO* pContainer);
#endif

MMP_ERR     MMPF_VIDMGR_OpenFile(MMP_UBYTE storagemode,const MMP_BYTE *pMode, MMP_USHORT usStreamType);
MMP_ERR     MMPF_VIDMGR_CloseFile(MMP_ULONG *ulFileID);
MMP_ERR     MMPF_VIDMGR_GetFilePos(MMP_ULONG64 *pos, MMP_ULONG ulFileID);
MMP_ERR     MMPF_VIDMGR_SeekFile(MMP_LONG64 llOffset, MMP_LONG lOrigin, MMP_ULONG ulFileID);
MMP_ERR     MMPF_VIDMGR_WriteFile(void *pData, MMP_ULONG NumBytes, MMP_ULONG *write_count, MMP_ULONG ulFileID);
MMP_ERR     MMPF_VIDMGR_ReadFile(void *pData, MMP_ULONG NumBytes, MMP_ULONG *read_count, MMP_ULONG ulFileID);
MMP_ERR     MMPF_VIDMGR_GetFreeRepackSize(MMP_ULONG *ulFreeSpace, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR     MMPF_VIDMGR_DirectWriteRepackBuf(MMP_ULONG srcaddr, MMP_ULONG size, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR     MMPF_VIDMGR_GetFrameTableEntry(MMP_ULONG ulOffset, MMP_ULONG *retSize, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR     MMPF_VIDMGR_EmitLEBytes2RepackBuf(MMP_UBYTE byteCount, MMP_ULONG val, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR     MMPF_VIDMGR_EmitBEBytes2RepackBuf(MMP_UBYTE byteCount, MMP_ULONG val, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR     MMPF_VIDMGR_GetTimeTableEntry(MMP_ULONG ulOffset, MMP_ULONG *retTime, MMPF_CONTAINER_INFO *pContainer);
void        MMPF_VIDMGR_SetBELong(MMP_UBYTE *ptr, MMP_ULONG val);

MMP_ERR     MMPF_VIDMGR_UpdateContainersBufferInfo(MMPF_CONTAINER_INFO *pSrc, MMPF_CONTAINER_INFO *pDes);
MMP_ERR     MMPF_VIDMGR_CheckCompBufFreeSpace(void);
MMP_ERR     MMPF_VIDMGR_GetVideoMetaData(MMPF_CONTAINER_INFO* pContainer);
MMP_ERR     MMPF_3GPMGR_GetRecordingTime(MMP_ULONG ulStreamType, MMP_ULONG *ulTime); 
MMP_ERR     MMPF_3GPMGR_GetRecordingTimeOffset(MMP_ULONG *ulTime, MMP_ULONG ulStreamType); 
#if (UVC_VIDRECD_SUPPORT)
void        MMPF_VIDMGR_UVCVidRecdCardSlowStopEvent(void);
#endif

MMP_BOOL    MMPF_VIDMGR_MoveVidFrm2StreamCallBack(MMP_ULONG fsize, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR     MMPF_VIDMGR_StopCapture(MMP_UBYTE encid, MMP_USHORT usFiletype);
#if (SUPPORT_H264_WIFI_STREAM)
MMP_ERR     MMPF_VIDMGR_StartWifiCapture(MMP_UBYTE encid, MMP_USHORT usStreamType);
MMP_ERR 	MMPF_VIDMGR_StopWifiCaptureDirectly(MMP_UBYTE encid, MMP_USHORT usFiletype);
#endif

MMP_ERR     MMPF_VIDMGR_DirectWriteRepackBuf_Type2(MMP_UBYTE ubPat, MMP_ULONG size, MMPF_CONTAINER_INFO *pContainer);

MMP_ERR MMPF_VIDMGR_TransferVideo2RepackBuf(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR MMPF_VIDMGR_TransferAudio2RepackBuf(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR MMPF_VIDMGR_TransferEmergVid2RepackBuf(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR MMPF_VIDMGR_TransferEmergAud2RepackBuf(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR MMPF_VIDMGR_TransferUvcDualVid2RepackBuf(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO *pContainer);
MMP_ERR MMPF_VIDMGR_TransferUvcDualAud2RepackBuf(MMP_UBYTE storagemode, MMPF_CONTAINER_INFO *pContainer);

MMP_ERR MMPF_3GPMGR_SetPostProcessingTimeLimit( MMP_ULONG ulTimeLimit );
MMP_BOOL MMPF_VIDMGR_IsDualEmergentRecdEnable(void);
#endif // _MMPF_3GPMGR_H_
