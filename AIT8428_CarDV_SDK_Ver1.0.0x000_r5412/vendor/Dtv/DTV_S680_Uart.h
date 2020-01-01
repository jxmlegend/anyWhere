
#ifndef _S680_UART_DRIVER_H_
#define _S680_UART_DRIVER_H_

/*===========================================================================
 * Include file
 *===========================================================================*/

#include "Customer_config.h"
#include "mmpf_typedef.h"
#include "Touch_Uart_ctrl.h"
/*===========================================================================
 * Macro define
 *===========================================================================*/
#ifndef DTV_UART_NUM
#define DTV_UART_NUM                    (MMP_UART_ID_2)
#endif
#ifndef DTV_UART_PADSET
#define DTV_UART_PADSET                 (MMP_UART_PADSET_2)
#endif
#define Uart_RX_QUEUE_SIZE               (256)
#define Uart_RX_SIGNAL                   (0xFE)
#ifndef DTV_UART_BAUD_RATE
#define DTV_UART_BAUD_RATE                   (57600)//(115200)
#endif
#define DTV_UART_CLK                    (264000 >> 1)

#define HEAD1_S680                  0xFF
#define HEAD2_S680                  0xAA

#define LEN_OFFSET_S680             0x02
#define DATA_TYPE_OFFSET_S680       0x03
#define DATA_OFFSET_S680            0x04

#define TAIL1_S680                  0XFF
#define TAIL2_S680                  0XFE

//data1:0x03  data2:�ֱ��� data3:ͣ����� data4:¼�� data5:������Ӧ
#define PARAM_RESOLUTION            0x02
#define PARAM_PARKING_MODE          0x03
#define PARAM_AUDIO_RECORD          0x04
#define PARAM_GSENSOR_SENSITIVITY   0x05

/*===========================================================================
 * Enum define
 *===========================================================================*/
typedef enum _DTVACK_S680ID // send to NAVI
{
	NAVIACK_HEART_BEAT      = 0x01,		
							//��ʽ��0xFF 0xAA len(2) 0x01 data checksum
							//Data :0x01 ͨѶ����
							//��ע��DVR 3sһ�Σ�����15s û���յ���������ж�DVR�쳣,������λDVR 

	NAVIACK_CARD_STATUS     = 0x02,	
							//��ʽ��0xFF 0xAA len(2) 0x02 data checksum
							//Data :0x01 ��ο��쳣	//��λ 1BYTE
							//Data :0x02 �޿�
							//Data :0x03 ��
							//Data :0x04 ����

	NAVIACK_PWRON_MSG       = 0x03,
							//������Ϣ����Ҫ��DVR�汾��Ϣ��DVR����״̬
							//��ʽ��0xFF 0xAA len(7) 0x03 data checksum
							//Data :¼Ӱ״̬1BYTE 	����¼Ӱ 0�� ¼Ӱ1
							//����״̬1BYTE		δ���� 0�� �Ѽ���1
							//�汾��4BYTE 	������V

	NAVIACK_SW_MODE         = 0x04,
							//��ʽ��0xFF 0xAA len 0x04 data checksum
							//Data :
							//data1:01¼��1BYTE��
							//data1:02���գ�1BYTE��
							//data1:03�ط� data2-3:video	data4-5:envent	data6-7:jpg  data8-9:���ڴ����� data10-11:���ڴ�δ�ã�11BYTE��
							//�������� 15.32G  ����1532 =5FC = 0x05 0xFC

	NAVIACK_REC_MODE_OPREQ	= 0x05,	
							//��ʽ��0xFF 0xAA len(3\3\6) 0x05 data checksum
							//data1:0x01	data2:¼Ӱ״̬0����¼Ӱ, 1¼Ӱ data3:����״̬0������, 1����
							//data1:0x02	data2:�˵�״̬0�˲˵�,	1���˵�
							//data1:0x03	data2:�ֱ��� data3:ͣ����� data4:¼�� data5:������Ӧ
							//�ֱ��ʣ�1080P��720P60��720P30��=>��������0��1��2����
							//ͣ����⣨�ء�����=>��������0��1����
							//¼�����ء�����=>��������0��1����
							//������Ӧ���͡��С��ߡ��رգ�=>��������0��1��2��3����  
							// data1:0x04  �ָ���������
							// data1:0x05  date2: 1��ʽ��SD����ʼ��2��ʽ��������

	NAVIACK_CAP_MODE_OPREQ	= 0x06,	
							//��ʽ��0xFF 0xAA len(3\3) 0x06 data checksum
							//data1:0x01	data2:01����OK
							//data1:0x02	data2:�˵�״̬0�˲˵� 1���˵�
							//data1:0x03	data2:�ֱ���
							//			ͼƬ�ֱ��ʣ�12M��10M��8M��5M��3M��2M��=>��������0��1��2��3��4��5����

	NAVIACK_PB_MODE_OPREQ   = 0x07,
							//��ʽ��0xFF 0xAA len(3) 0x07 data checksum
							//data1:0x01	data2:01video 02envent 03jeg

	NAVIACK_MOV_LIST_MSG    = 0x08,							
							//Video�б���Ϣ������video ʱDVR�������ͣ�
							//��ʽ��0xFF 0xAA len(4\10) 0x08 data checksum
							//data1:0x01 data2-data3:����(<1000)
							//data1:0x02 data2-data3:��N��(<1000)  data4-data9:ymdhms 6��BYTE

	NAVIACK_MOV_LIST_OPREQ  = 0x09,
							//��ʽ��0xFF 0xAA len(7\3) 0x09 data checksum
							//data1:0x01 data2-data3:��ǰѡ��1-N	data4�ֱ��ʣ�1080P��720P60��720P30�� data5:��ʱ��
							//Data6:����״̬ 0δ���� 1����
							//data1:0x02 data2����״̬ ֹͣ���˳����Ž�����б����0�����Ž���1

	NAVIACK_MOV_PB_OPREQ    = 0x0A,	
							//��ʽ��0xFF 0xAA len(3) 0x0A data checksum
							//data1:0x01	data2:0x01���� 0x02��ͣ
							//data1:0x02	data2:ʱ��//������ʱ�� �����иı伴��
							//data1:0x03	data2:	0x01 ȫ��������ʼִ��
							//0x02 ȫ��������ʼִ�� 
							//				0x03 ɾ�����п�ʼִ��
							//				0x00 ִ���������

	NAVIACK_EVENT_LIST_MSG  = 0x0C,
							//event�б���Ϣ������event ʱDVR�������ͣ�
							//��ʽ��0xFF 0xAA len(4\10) 0x0C data checksum
							//data1:0x01 data2-data3:����(<1000)
							//data1:0x02 data2-data3:��N��(<1000)  data4-data9:ymdhms 6��BYTE 

	NAVIACK_EVENT_REQ       = 0x0D,	
							//��ʽ��0xFF 0xAA len(7\3) 0x0D data checksum
							//data1:0x01 data2-data3:��ǰѡ��1-N	data4�ֱ��ʣ�1080P��720P60��720P30�� data5:��ʱ��
							//Data6:����״̬ 0δ���� 1����
							//data1:0x02 data2����״̬ ֹͣ���˳����Ž�����б����0�����Ž���1

	NAVIACK_EVENT_REP_REQ   = 0x0E,	
							//��ʽ��0xFF 0xAA len(3) 0x0E data checksum
							//data1:0x01	data2:0x01���� 0x02��ͣ
							//data1:0x02	data2:ʱ��//������ʱ�� �����иı伴��
							//data1:0x03	data2:	0x01 ȫ��������ʼִ��
							//0x02 ȫ��������ʼִ�� 
							//				0x03 ɾ�����п�ʼִ��
							//				0x00 ִ��������� 

	NAVIACK_PHOTO_MSG      = 0x10,	
							//JPG��ǰ��Ϣ��
							//��ʽ��0xFF 0xAA len(10) 0x10 data checksum
							//				  data1-5:ʱ��ymdhm 5BYTE 
							//				  data6:ͼƬ�ֱ��ʣ�12M��10M��8M��5M��3M��2M��=>��������0��1��2��3��4��5����
							//				  data7-8:ͼƬ��С
							//				  Data9:����״̬ 0δ���� 1����
							//����ͼƬ��С���� 15.32M  ����1532 =5FC = 0x05 0xFC

	NAVIACK_PHOTO_VIEW      = 0x11,	
							//��ʽ��0xFF 0xAA len(3) 0x11 data checksum
							//data1:0x03	data2:	0x01 ȫ��������ʼִ��
							//0x02 ȫ��������ʼִ�� 
							//				0x03 ɾ�����п�ʼִ��
							//				0x00 ִ���������

	NAVIACK_WARNING_MSG     = 0x12,
							//������ʾ��Ϣ��������
							//��ʽ��0xFF 0xAA len(2) 0x12 data checksum
							//Data��0x01	������Ϣ����Ŀǰû���ã�
							//Data��0x02	panel��������
							//Data��0x03	panel�����
							//Data:  0x04  ��ʾ������������ 








//*********************************************************************************//
//                                           send string to navi
//*********************************************************************************//
	NAVIACK_FILE_COUNT     = 0x13,//#define STATUS_FILE_COUNT     "@file-count??\r"
	NAVIACK_FILE_STATUS     = 0x14,//@file-avi,5/88,256,filename:xxx
	
	NAVIACK_FILE_UNLOCK     = 0x15,//#define STATUS_RECORD_UNLOCK     "@record-unlock\r"   //�Զ�¼�񴴽���һ���µ�¼���ļ���Ĭ����û�м�����
	NAVIACK_FILE_UNLOCK_END = 0x16,//#define STATUS_RECORD_UNLOCK_END "@unlockrecord-end\r"
	NAVIACK_FILE_LOCK_END   = 0x17,//#define STATUS_RECORD_LOCK_END   "@lockrecord-end\r"
	
	NAVIACK_FILE_RECORD_RESTART  = 0x18,//#define STATUS_RECORD_RESTART    "@record-restart\r"
	NAVIACK_FILE_RECORD_STOP     = 0x19,//#define STATUS_RECORD_OFF        "@record-stop\r"

	NAVIACK_SD_FORMAT_FAIL  = 0x1a,//#define STATUS_FORAMT_FAIL     "@format-fail\r"
	NAVIACK_SD_FORMAT_SUCC  = 0x1b,//#define STATUS_FORAMT_SUCC     "@format-succ\r"
	NAVIACK_SD_FORMAT_NOCARD  = 0x1C,//#define STATUS_FORAMT_SUCC     "@format-succ\r"

	
//*********************************************************************************//
//*********************************************************************************//

	NAVIACK_NULL
} UARTACK_S680ID;

typedef enum _DTVCOMMAND_S680ID // get from NAVI
{
	NAVICMD_GPS_MSG         = 0xA1,		
							//Date �������£�
							//<1> ��λ״̬��A=��Ч��λ��V=��Ч��λ1BYTE��
							//<2> γ������ 1BYTE
							//<3> γ��С�� 2BYTE
							//<4> γ�Ȱ���N(������)��S(�ϰ���) 1BYTE
							//<5> ��������1BYTE
							//<6> ����С��2BYTE
							//<7> ���Ȱ���E(����)��W(����) 1BYTE
							//<8> �ٶ�����2BYTE
							//<9> ������ʱ�� ymdhms 6BYTE year-2000���� 

	NAVICMD_PWROFF_MSG      = 0xA2,
							//��ʽ��0xFF 0xAA len(2) 0xA2 0x00 checksum

	NAVICMD_PWRON_MSG_STOP  = 0xA3,
							//��ʽ��0xFF 0xAA len(2) 0xA3 0x00 checksum

	NAVICMD_SW_MODE         = 0xA4,
                            //��ʽ��0xFF 0xAA len(2\2\12) 0xA4 data checksum
                            //Data :data1:01¼�� 02���� 03�ط�
                            
	NAVICMD_REC_MODE_OPREQ	= 0xA5,
	                        //¼Ӱģʽ��������
                            //��ʽ��0xFF 0xAA len(3\3\6\2\2) 0xA5 data checksum
                            //data1:0x01  data2:00 ֹͣ¼Ӱ
                            //                              01¼Ӱ 
                            //                              02ȡͼ 
                            //                              03����
                            //data1:0x02  data2:0�˲˵�1���˵� 

							//data1:0x03  data2:�ֱ��� data3:ͣ����� 
							//data4:¼�� data5:������Ӧ
							//data6:�����Զ�¼��
				
                            //data1:0x04  �ָ���������
                            //data1:0x05  ��ʽ��SD��
                            //data1:0x06  ���ذ汾��
                            
	NAVICMD_CAP_MODE_OPREQ	= 0xA6,
	                        //����ģʽ��������
                            //��ʽ��0xFF 0xAA len(3\3\3) 0xA6 data checksum
                            //data1:0x01  data2:01����
                            //data1:0x02  data2:0�˲˵�1���˵� 
                            //data1:0x03  data2:�ֱ���
                            
	NAVICMD_REP_MODE_OPREQ	= 0xA7,
	                        //�ط�ģʽ��������
                            //��ʽ��0xFF 0xAA  len(3) 0xA7 data checksum
                            //data1:0x01  data2:01video 02envent 03jeg


    NAVICMD_MOV_LIST_OPREQ  = 0xA9,
                            //��������video�б���棩��
                            //��ʽ��0xFF 0xAA len(4\3) 0xA9 data checksum  
                            //data1:0x01  data2-data3:��N��(<1000)  ѡ�� ��ֱ�Ӹ��ڼ��� �൱��DVR idx��
                            //data1:0x02  data2�˳�����0���벥��1 
                            //data1:0x03  data2 0=prev 1=next
                            
	NAVICMD_MOV_REP_OPREQ	= 0xAA,
	                        //��������video���Ž��棩��
                            //��ʽ��0xFF 0xAA len(3) 0xAA data checksum
                            //data1:0x01  data2:0x01 ������ͣ 
                            //data1:0x02  data2:ʱ��    //���ý�����              
                            //data1:0x03  data2:  0x01������������� 
                            //0x02ȫ������ 
                            //0x03ȫ������
                            //0x04����ɾ�� 
                            //0x05ɾ������ 
                            
	NAVICMD_EVENT_REQ	    = 0xAD,
	                        //��������event�б���棩��
                            //��ʽ��0xFF 0xAA len(4\3) 0xAD data checksum  
                            //data1:0x01  data2-data3:��N��(<1000)  ѡ�� ��ֱ�Ӹ��ڼ��� �൱��DVR idx��
                            //data1:0x02  data2�˳�����0���벥��1 
                            
    NAVICMD_EVENT_REP_REQ   = 0xAE,
                            //��������event���Ž��棩��
                            //��ʽ��0xFF 0xAA len(3) 0xAE data checksum
                            //data1:0x01  data2:0x01 ������ͣ 
                            //data1:0x02  data2:ʱ��    //���ý�����              
                            //data1:0x03  data2:  0x01������������� 
                            //0x02ȫ������ 
                            //0x03ȫ������
                            //0x04����ɾ�� 
                            //0x05ɾ������ 
                            
    NAVICMD_PHOTO_VIEW	    = 0xB0,
                            //��ʽ��0xFF 0xAA len(3) 0xB0 data checksum
                            //data1:0x01  data2:0x01��һ�� 0x02��һ��  ��DVR����ΪJPG��ǰ��Ϣ��
                            //data1:0x02  data2:0x01�˳� ���ص��ļ��н���
                            //data1:0x03  data2:  0x01������������� 
                            //0x02ȫ������ 
                            //0x03ȫ������
                            //0x04����ɾ�� 
                            //0x05ɾ������ 

    NAVICMD_SET_DATE	    = 0xC0,
    NAVICMD_SET_TIME	    = 0xC1,
    
    NAVICMD_END 
} UARTCOMMAND_S680ID;

typedef enum _KEY_VAL
{
	KEY_CH_UP =0x80,
	KEY_CH_DOWN = 0x81,
	KEY_VOL_UP = 0x82,
	KEY_VOL_DOWN = 0x83,
	KEY_PAGE_UP = 0x84,
	KEY_MENU = 0x85,
	KEY_PAUSE = 0x86,
	KEY_EXIT = 0x87,
	KEY_RECALL = 0x88,
	KEY_INFO_VAL = 0x89,
	KEY_POWER = 0x8A,
	KEY_PAGE_DOWN = 0x8B,
	KEY_MUTE = 0x8C,
	KEY_REServed = 0x8D,
	KEY_VAL_0 = 0x90,
	KEY_VAL_1 = 0x91,
	KEY_VAL_2 = 0x92,
	KEY_VAL_3 = 0x93,
	KEY_VAL_4 = 0x94,
	KEY_VAL_5 = 0x95,
	KEY_VAL_6 = 0x96,
	KEY_VAL_7 = 0x97,
	KEY_VAL_8 = 0x98,
	KEY_VAL_9 = 0x99,
	KEY_CAPTURE = 0xA0,
	KEY_LOCK = 0xA1,
	KEY_FRCAM_SWITCH = 0xA2
	
} KEY_VAL;

typedef struct tagKeyButton
{
    unsigned char  iCmdId;
    unsigned char  ulkeyEvent; 
	unsigned char  ubkeyname[16];
} CMDKEY;

/*===========================================================================
 * Structure define
 *===========================================================================*/
extern MMPF_OS_FLAGID   	UartCtrlFlag;
extern MMP_UBYTE 			g_Ack2NaviArg;
#endif // _S680_UART_DRIVER_H_