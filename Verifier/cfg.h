#ifndef __VERIFIER_CFG__
#define __VERIFIER_CFG__
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
#define DEFAULT_FPS	   	   30

// Communication protocol
#define CMD_HDR_SIZE        8
#define ASYNC_BUFSIZE       4096
#define MAX_USER_CNT        5

//----------------------------------
#define ThisID Verifier

#endif
