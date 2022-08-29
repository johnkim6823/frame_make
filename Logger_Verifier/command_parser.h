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
#endif
