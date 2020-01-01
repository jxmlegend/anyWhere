
#ifndef _MENU_STATE_PLAYBACK_MENU_H_
#define _MENU_STATE_PLAYBACK_MENU_H_

/*===========================================================================
 * Include files
 *===========================================================================*/ 

#include "AHC_Common.h"

/*===========================================================================
 * Enum define 
 *===========================================================================*/ 

typedef enum {
    ITEMID_QUICKM_DELETE= 1,
    ITEMID_QUICKM_DEL_YES,
    ITEMID_QUICKM_DEL_NO,
    
    ITEMID_QUICKM_PROTECT,
    ITEMID_QUICKM_PROTECT_YES,
    ITEMID_QUICKM_PROTECT_NO,
    
    ITEMID_CUT,
    ITEMID_CUT_YES,
    ITEMID_CUT_NO,
    
    ITEMID_ROTATE,
    ITEMID_ROTATE_LEFT,
    ITEMID_ROTATE_RIGHT,
    ITEMID_ROTATE_OK,
    ITEMID_ROTATE_CANCEL,
    
    ITEMID_RESIZE,
    ITEMID_RESIZE_SIZE_14M,
    ITEMID_RESIZE_SIZE_12M,
    ITEMID_RESIZE_SIZE_8M,
    ITEMID_RESIZE_SIZE_5M,
    ITEMID_RESIZE_SIZE_3M,
    ITEMID_RESIZE_SIZE_2M,
    ITEMID_RESIZE_SIZE_1_2M,
    ITEMID_RESIZE_SIZE_VGA,
    
    ITEMID_TRIMMING,
    ITEMID_TRIMMING_OK,
    ITEMID_TRIMMING_CANCEL,
    
    ITEMID_SLIDE_SHOW_FILES,
#if (MENU_PLAYBACK_SLIDESHOW_FILE_ALL_EN)    
    ITEMID_FILES_ALL,
#endif
#if (MENU_PLAYBACK_SLIDESHOW_FILE_STILL_EN)    
    ITEMID_FILES_STILL,
#endif
#if (MENU_PLAYBACK_SLIDESHOW_FILE_MOVIE_EN)    
    ITEMID_FILES_MOVIE,
#endif    

    ITEMID_SLIDE_SHOW_EFFECTS,
#if (MENU_PLAYBACK_SLIDESHOW_EFFECT_SIMPLE_EN)
    ITEMID_SLIDE_EFFECTS_SIMPLE,
#endif
#if (MENU_PLAYBACK_SLIDESHOW_EFFECT_NOSTALGIC_EN)    
    ITEMID_SLIDE_EFFECTS_NOSTALGIC,
#endif
#if (MENU_PLAYBACK_SLIDESHOW_EFFECT_STYLISH_EN)    
    ITEMID_SLIDE_EFFECTS_STYLISH,
#endif
#if (MENU_PLAYBACK_SLIDESHOW_EFFECT_ACTIVE_EN)    
    ITEMID_SLIDE_EFFECTS_ACTIVE,
#endif
    
    ITEMID_SLIDE_SHOW_MUSIC,
#if (MENU_PLAYBACK_SLIDESHOW_MUSIC_ON_EN)
    ITEMID_MUSIC_ON,
#endif
#if (MENU_PLAYBACK_SLIDESHOW_MUSIC_OFF_EN)    
    ITEMID_MUSIC_OFF,
#endif
    
    ITEMID_VOLUME,
#ifdef CFG_FIXED_INTERNAL_VOLUME_ID //may be defined in config_xxx.h

#if (MENU_PLAYBACK_VOLUME_LV0_EN)
    ITEMID_VOLUME_00 = ITEMID_VOLUME + 1,
#endif
#if (MENU_PLAYBACK_VOLUME_LV1_EN)    
    ITEMID_VOLUME_01 = ITEMID_VOLUME + 2,
#endif
#if (MENU_PLAYBACK_VOLUME_LV2_EN)    
    ITEMID_VOLUME_02 = ITEMID_VOLUME + 3,
#endif
#if (MENU_PLAYBACK_VOLUME_LV3_EN)    
    ITEMID_VOLUME_03 = ITEMID_VOLUME + 4,
#endif
#if (MENU_PLAYBACK_VOLUME_LV4_EN)    
    ITEMID_VOLUME_04 = ITEMID_VOLUME + 5,
#endif
#if (MENU_PLAYBACK_VOLUME_LV5_EN)    
    ITEMID_VOLUME_05 = ITEMID_VOLUME + 6,
#endif
#if (MENU_PLAYBACK_VOLUME_LV6_EN)    
    ITEMID_VOLUME_06 = ITEMID_VOLUME + 7,
#endif
#if (MENU_PLAYBACK_VOLUME_LV7_EN)    
    ITEMID_VOLUME_07 = ITEMID_VOLUME + 8,
#endif
#if (MENU_PLAYBACK_VOLUME_LV8_EN)    
    ITEMID_VOLUME_08 = ITEMID_VOLUME + 9,
#endif
#if (MENU_PLAYBACK_VOLUME_LV9_EN)    
    ITEMID_VOLUME_09 = ITEMID_VOLUME + 10,
#endif    
#if (MENU_PLAYBACK_VOLUME_LV10_EN) && (AUDIO_PLAYBACK_MUTE)
    ITEMID_VOLUME_10 = ITEMID_VOLUME + 11,
#endif

#else

#if (MENU_PLAYBACK_VOLUME_LV0_EN)
    ITEMID_VOLUME_00,
#endif
#if (MENU_PLAYBACK_VOLUME_LV1_EN)    
    ITEMID_VOLUME_01,
#endif
#if (MENU_PLAYBACK_VOLUME_LV2_EN)    
    ITEMID_VOLUME_02,
#endif
#if (MENU_PLAYBACK_VOLUME_LV3_EN)    
    ITEMID_VOLUME_03,
#endif
#if (MENU_PLAYBACK_VOLUME_LV4_EN)    
    ITEMID_VOLUME_04,
#endif
#if (MENU_PLAYBACK_VOLUME_LV5_EN)    
    ITEMID_VOLUME_05,
#endif
#if (MENU_PLAYBACK_VOLUME_LV6_EN)    
    ITEMID_VOLUME_06,
#endif
#if (MENU_PLAYBACK_VOLUME_LV7_EN)    
    ITEMID_VOLUME_07,
#endif
#if (MENU_PLAYBACK_VOLUME_LV8_EN)    
    ITEMID_VOLUME_08,
#endif
#if (MENU_PLAYBACK_VOLUME_LV9_EN)    
    ITEMID_VOLUME_09,
#endif    
#if (MENU_PLAYBACK_VOLUME_LV10_EN) && (AUDIO_PLAYBACK_MUTE)
    ITEMID_VOLUME_10,
#endif

#endif
    ITEMID_VOLUME_MAX,

    ITEMID_SHOW_VIDEO_TYPE,
#if (MENU_PLAYBACK_VIDEO_TYPE_NORMAL_EN)
    ITEMID_VIDEO_TYPE_NORMAL,
#endif
#if (MENU_PLAYBACK_VIDEO_TYPE_PARKING_EN)    
    ITEMID_VIDEO_TYPE_PARKING,
#endif
#if (MENU_PLAYBACK_VIDEO_TYPE_EMERGENCY_EN)    
    ITEMID_VIDEO_TYPE_EMERGENCY
#endif

}PLAYBACKITEMID;

/*===========================================================================
 * Function prototype
 *===========================================================================*/ 

void MenuStateCameraPlaybackMode( UINT32 ulEvent, UINT32 ulPosition );
void MenuStateVideoPlaybackMode( UINT32 ulEvent, UINT32 ulPosition );
//void MenuStatePlaybackMode(UINT32 ulEvent, UINT32 ulPosition);
void MenuStatePlaybackSelectDB( UINT8 byOpMode );

AHC_BOOL  MenuStatePlaybackModeInit(void* pData);
AHC_BOOL  MenuStateEditToolModeInit(void* pData);
AHC_BOOL  MenuStatePlaybackModeShutDown(void* pData);

#endif //_MENU_STATE_PLAYBACK_MENU_H_


