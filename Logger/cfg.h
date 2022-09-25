#ifndef __LOGGER_CFG__
#define __LOGGER_CFG__
// Define DEFAULT CAMERA SETTINGS
#define DEFAULT_WIDTH		640
#define DEFAULT_HEIGHT		480
#define DEFAULT_FPS		30
//----------------------------------
// Define CAMERA_CONFIG SETTINGS
#define MSG_Q_CONFIG_KEY   	1234
#define MSG_Q_CONFIRM_KEY  	1235
//----------------------------------
#define TYPE_CAMERA_CFG    	1
#define TYPE_CONFIRM_CFG   	2
//----------------------------------
#define CONFIG_BUFF_SIZE   	6
#define CONFIRM_BUFF_SIZE  	1
//----------------------------------
#define VGG			0x01
#define HD			0x02
#define CIF			0x03
//----------------------------------
#define YUV420			0x21
//----------------------------------
struct config_msg_data{
	long data_type;
	unsigned char data_buff[CONFIG_BUFF_SIZE];
};

struct confirm_msg_data{
	long data_type;
	unsigned char data_buff[CONFIRM_BUFF_SIZE];
};
//----------------------------------
#define ThisID Logger

#endif
