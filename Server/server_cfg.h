#ifndef __SERVER_CFG__
#define __SERVER_CFG__
//CHANGE SERVER_IP_ADDR to RPI's IP that working as SERVER
# define SERVER_PORT        8700
# define DB_IP              "127.0.0.1"
# define DB_user            "hanium"                //user = webui -> Can extern access
# define DB_password        "1234"
# define DB_database        "hanium"
# define storage_dir        "/home/pi/images/"

// Deafault values for datasize
# define Hash_size          350
# define CID_size           23

// Communication protocol
#define CMD_HDR_SIZE        8
#define ASYNC_BUFSIZE       4096
#define MAX_USER_CNT        5

#define ThisID Server

#endif
