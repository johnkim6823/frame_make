#include <stdio.h>
#include <stdlib.h>

#ifndef __CMDPARSERHEADER__
#define __CMDPARSERHEADER__

#define ThisID Server

#include "server.h"

typedef struct _cmdpdesp
{
	u_int8_t code;
	int(* callback)(HEADERPACKET* msg);
}cmdp_desp;

int cmd_parser(IO_PORT port, HEADERPACKET *pmsg);

/* command fucntions*/
int test(HEADERPACKET* msg);
int video_data_send(HEADERPACKET* msg);
int hash_send(HEADERPACKET* msg);
int CID_send(HEADERPACKET* msg);

int test(HEADERPACKET* msg);
#endif
