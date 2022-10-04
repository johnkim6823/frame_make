#include "msg_type.h"

// Logger <--> Web UI
int confirm_cfg_send();                                     //Logger -> Web UI
int camera_cfg_recv(int &WIDTH, int &HEIGHT, int &FPS);     //WEB UI -> Logger
int Image_HASH_send(string HASH);                           //Logger -> Web UI
int Image_Hash_recv();                                      //Web UI -> Web UI

// Verifier <--> Server
int Server2Verifier_CID_send(string &CID );                 //Server   -> Verifier
int Server2Verifier_CID_recv(string &CID);                  //Verifier -> Server
int Verifier2Server_CID_recv_send();                        //Verifier -> Server
int Verifier2Server_CID_recv_recv();                        //Server   -> Verifier