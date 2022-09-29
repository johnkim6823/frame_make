#ifndef __LOGGER_CFG__
#define __LOGGER_CFG__
// Default values for Camera Settings
#define DEFAULT_WIDTH 		640
#define DEFAULT_HEIGHT		480
#define DEFAULT_FPS		10
#define DEFAULT_FRAME_COUNT 	50
//----------------------------------
// Define CAMERA_CONFIG SETTINGS
#define CAMERA_CFG_MQ	   	1129
#define CONFIRM_MQ	   	1130
//----------------------------------
#define TYPE_CAMERA_CFG    	1
#define TYPE_CONFIRM_CFG   	2
//----------------------------------
#define CONFIG_BUFF_SIZE   	6
#define CONFIRM_BUFF_SIZE  	1
//----------------------------------
#define VGA		   	0x01
#define HD		   	0x02
#define CIF		   	0x03
//----------------------------------
#define YUV420		   	0x21
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
