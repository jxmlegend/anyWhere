#ifndef _ALL_FW_H_
#define _ALL_FW_H_

#include "lib_retina.h"

#define	TASK_SYS_PRIO               (1)
#define TASK_LAST_PRIO              (OS_LOWEST_PRIO - 2)

#if (SUPPORT_COMPONENT_FLOW_CTL)
#define TASK_COMP_CTL_PRIO          (6)
#endif

#if (VIDEO_R_EN)
#define TASK_MP4VENC_PRIO           (10)
#define TASK_3GPMERGR_PRIO          (13)
#if (SUPPORT_H264_WIFI_STREAM)
#define TASK_H264WIFICB_PRIO        (29)
#endif
#endif

#if (AUDIO_STREAMING_EN == 1)||(AUI_STREAMING_EN == 1)
#define TASK_AUDIO_STREAM_PRIO      (14)
#else
#define TASK_AUDIO_STREAM_PRIO      (23)
#endif
#define TASK_CYCLIC_JOB_PRIO        (23)

#define TASK_VIDDEC_PRIO            (15)
#define TASK_3GPPARSR_PRIO          (16)
#define TASK_AUDIO_PLAY_PRIO        (17)
#define TASK_VIDPLAY_PRIO           (18)
#define TASK_AUDIO_RECORD_PRIO      (21)

#if (DSC_R_EN) || (DSC_P_EN)
#define TASK_DSC_PRIO               (20)
#define TASK_DSC_STREAM_PRIO        (28)
#define TASK_JPEG_CTL_PRIO          (7)
#endif

#if (HANDLE_LDC_EVENT_BY_QUEUE)
#define TASK_LDC_CTL_PRIO           (8)
#endif
#if (HANDLE_H264E_EVENT_BY_QUEUE)
#define TASK_H264E_CTL_PRIO         (6)
#endif

#if (SENSOR_EN)
#define TASK_SENSOR_PRIO            (19)
#endif

#if (MSDC_WR_FLOW_TEST_EN)
#define TASK_USB_WRITE_PRIO         (22)
#define TASK_USB_PRIO               (25)
#else
#define TASK_USB_PRIO               (22)
#endif

#if (SUPPORT_USB_HOST_FUNC)
#define TASK_USBH_BULKIN_PRIO       (8)
#define TASK_USBH_POSTPROCESS_PRIO  (9)
#endif

#define TASK_HIGH_WORK_PRIO         (11) //for high priority but short execution time work.

#define TASK_FS_PRIO                (24)

#if ((DSC_R_EN)||(VIDEO_R_EN))&&((SUPPORT_MDTC)||(SUPPORT_ADAS))
//#define TASK_ALGORITHM_ON_CPUB_PRIO              (59)    // CarDV-TY Miao, swap with TASK_HIGH_PRIO
#define TASK_ALGORITHM_ON_CPUB_PRIO              (30)//(TASK_MOTOR_CTRL_PRIO) // CarDV-Chiao Su, swapped with higher priority task
#define TASK_ALGORITHM_ON_CPUA_PRIO              (31)
#endif

#if (HANDLE_EVENT_BY_TASK == 1)
#define TASK_EVENT_PRIO             (26)
#endif

#if (SUPPORT_EIS)
#define TASK_GYRO_PRIO              (35)
#define TASK_EIS_PRIO               (38)
#endif

#if (VIDEO_BGPARSER_EN == 1)
#define TASK_BG_PARSER_PRIO         (59) //Note: Confilict with TASK_HIGH_PRIO
extern OS_STK  BG_PARSER_Task_Stk[];
#if (VIDEO_HIGHPRIORITY_FS == 1)
#define TASK_HIGHFS_PRIO            (27)
extern OS_STK  HIGH_PRIO_FS_Task_Stk[];
#endif
#endif

#ifdef BUILD_CE
#define TASK_JOB_DISPATCH_PRIO   	(OS_LOWEST_PRIO - 3)//(60)
#define TASK_HIGH_PRIO            	(OS_LOWEST_PRIO - 5)//(58)  // CarDV-TY Miao, swap with TASK_ALGORITHM_ON_CPUB_PRIO

#if defined(EXBOOT_FW)
#define TASK_JOB_DISPATCH_STK_SIZE  (4096)
#elif (SD_FW_UPDATER) && (!defined(ALL_FW))
#define TASK_JOB_DISPATCH_STK_SIZE  (1024)
#else
#define TASK_JOB_DISPATCH_STK_SIZE  (8192)
#endif

#define TASK_HIGH_STK_SIZE          (1024) //CarDV... For timer lock up issue

#define TASK_HIGH_WORK_STK_SIZE     (1024)
#endif

#define TASK_AHC_INIT_PRIO          (32) 
#define TASK_UART_BRUST_PRIO        (TASK_AHC_INIT_PRIO)

#endif //#ifndef _ALL_FW_H_

