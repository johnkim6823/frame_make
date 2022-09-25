#include <stdio.h>
#include <stdlib.h>

#include "client.h"

#ifndef __CMDPARSERHEADER__
#define __CMDPARSERHEADER__


typedef struct _cmdpdesp
{
	u_int8_t code;
	int(* callback)(HEADERPACKET* msg);
}cmdp_desp;

int cmd_parser(IO_PORT port, HEADERPACKET *pmsg);

/* command fucntions*/
//int video_data_send(HEADERPACKET* msg);
int video_data_response(HEADERPACKET* msg);
int hash_send(HEADERPACKET* msg);
int CID_send(HEADERPACKET* msg);
int public_key_send(HEADERPACKET* msg);
int public_key_response(HEADERPACKET* msg);

int test(HEADERPACKET* msg);
#endif
