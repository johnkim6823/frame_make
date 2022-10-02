#ifndef __MSG_QUEUE_CFG__
#define __MSG_QUEUE_CFG__
// ----MSG_TYPE----
// Logger <---> Web UI
#define TYPE_CAMERA_CFG	        1
#define TYPE_CAMERA_CFG_RECV	2
#define TYPE_IMAGE_HASH         3
#define TYPE_IMAGE_HASH_RECV    4
// Verifier <--> Server
#define TYPE_CID                5
#define TYPE_CID_RECV           6
//-------------------------

// ----MSGQ KEY----
#define CAMERA_CFG_MQ			1129
#define CAMERA_CFG_RECV_MQ      1130
#define IMAGE_HASH_MQ			1131
#define IMAGE_HASH_RECV_MQ      1132
#define CID_MQ					1133
#define CID_RECV_MQ				1134
//-------------------------

// ----CAMERA_CFG VALUE----
#define VGA                     0x01
#define HD                      0x02
#define CIF                     0x03
//-------------------------
#define YUV420                  0x21
//-------------------------

// ----IMAGE_HASH----
string IMAGE_PATH = "/home/pi/hanium_2022/Logger/";
// ----BUFF SIZE----
#define CONFIG_BUFF_SIZE        	6
#define CONFIRM_BUFF_SIZE       	1
#define IMAGE_HASH_BUFF_SIZE		93
#define IMAGE_HASH_RECV_BUFF_SIZE	1
#define CID_BUFF_SIZE				24
#define CID_RECV_BUFF_SIZE			1
//-------------------------

struct config_msg_data{
	long data_type;
	unsigned char data_buff[CONFIG_BUFF_SIZE];
};

struct confirm_msg_data{
	long data_type;
	unsigned char data_buff[CONFIRM_BUFF_SIZE];
};

struct Image_hash_msg_data{
	long data_type;
	char data_buff[IMAGE_HASH_BUFF_SIZE];
};

struct Image_hash_recv_msg_data{
	long data_type;
	unsigned char data_buff[IMAGE_HASH_RECV_BUFF_SIZE];
};

struct CID_msg_data{
	long data_type;
	unsigned char data_buff[CID_BUFF_SIZE];
};

struct CID_recv_msg_data{
	long data_type;
	unsigned char data_buff[CID_RECV_BUFF_SIZE];
};

#endif