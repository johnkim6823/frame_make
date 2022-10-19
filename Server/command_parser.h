#include <stdio.h>
#include <stdlib.h>

#ifndef __CMDPARSERHEADER__
#define __CMDPARSERHEADER__

#include "server.h"
#include "server_cfg.h"

typedef struct _cmdpdesp
{
	u_int8_t code;
	int(* callback)(HEADERPACKET* msg);
}cmdp_desp;

int cmd_parser(IO_PORT port, HEADERPACKET *pmsg);

/* command fucntions*/
int public_key_send(HEADERPACKET* msg);			//PUBKET_SND
int public_key_response(HEADERPACKET* msg);		//PUBKEY_RES
int video_data_send(HEADERPACKET* msg);			//VIDEO_DATA_SND
int video_data_response(HEADERPACKET* msg);		//VIDEO_DATA_RES
int hash_request(HEADERPACKET* msg);			//HASH_REQ
int hash_send(HEADERPACKET* msg);				//HASH_SND
int CID_REQ(HEADERPACKET* msg);					//CID_REQ
int CID_send(HEADERPACKET* msg);				//CID_SND
int verify_request(HEADERPACKET* msg);			//VER_REQ
int verify_response(HEADERPACKET* msg);			//VER_RES
int verified_result_send(HEADERPACKET* msg);	//VER_RSLT_SND
int verified_result_response(HEADERPACKET* msg);//VER_RSLT_RES
int configuration_send(HEADERPACKET* msg);		//CONFIG_SND
int configuration_response(HEADERPACKET* msg);	//CONFIG_RES
int still_alive(HEADERPACKET* msg);				//STILL_ALIVE
int test(HEADERPACKET* msg);
#endif
