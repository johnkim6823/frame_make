#ifndef __VERIFIER_CFG__
#define __VERIFIER_CFG__
// Default DB, storage Settings
# define DB_IP              "127.0.0.1"
# define DB_user            "hanium"                //user = webui -> Can extern access
# define DB_password        "1234"
# define DB_database        "hanium"
# define storage_dir        "/home/pi/images/"
//----------------------------------
// Default values for Camera Settings
#define DEFAULT_WIDTH 			640
#define DEFAULT_HEIGHT			480
#define DEFAULT_FPS				10
#define DEFAULT_FRAME_COUNT 	10
//----------------------------------
// Deafault values for Verifier
#define pubKey_path             "/home/pi/hanium_2022/Server/PUBKEY.txt"
//----------------------------------
#define ThisID Verifier

#endif
