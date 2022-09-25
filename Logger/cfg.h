#ifndef __LOGGER_CFG__
#define __LOGGER_CFG__
// CHANGE SERVER_IP_ADDR to RPI's IP that working as SERVER
# define SERVER_IP_ADDR     "127.0.0.1"
# define SERVER_PORT        8700
# define storage_dir        "/home/pi/images/"

// Deafault values for datasize
# define Hash_size          350
# define CID_size           23

// Default values for Camera Settings
#define DEFAULT_WIDTH 	   640
#define DEFAULT_HEIGHT	   480
#define DEFAULT_FPS	   30

// Communication protocol
#define CMD_HDR_SIZE        8
#define ASYNC_BUFSIZE       4096
#define MAX_USER_CNT        5

// Define CAMERA_CONFIG SETTINGS
#define MSG_Q_CONFIG_KEY   1234
#define MSG_Q_CONFIRM_KEY  1235
//----------------------------------
#define TYPE_CAMERA_CFG    1
#define TYPE_CONFIRM_CFG   2
//----------------------------------
#define CONFIG_BUFF_SIZE   6
#define CONFIRM_BUFF_SIZE  1
//----------------------------------
#define VGA		   		   0x01
#define HD		   		   0x02
#define CIF		   		   0x03
//----------------------------------
#define YUV420		       0x21
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
