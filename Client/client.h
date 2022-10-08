
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "client_cfg.h"

#define HANDLE void*

#define TRUE 1
#define FALSE 0
#define INVALID_SOCKET -1

#define SOCKET u_int32_t

/*Communication protocol*/
#define CMD_HDR_SIZE 8	
#define SERVER_PROTOCOL_PORT 8700
#define ASYNC_BUFSIZE 4096
#define MAX_USER_CNT 5

#ifndef __SERVERSOCKET_HEADER__
#define __SERVERSOCKET_HEADER__

typedef struct
{
    char* Year;
    char* Month;
    char* Day;
    char* Hour;
    char* Min;
    char* Sec;
    char* MSec;
	
} CIDINFO;

typedef struct
{
	u_int8_t startID; 
	u_int8_t destID;
	u_int8_t command;
	u_int8_t dataType;
	u_int32_t dataSize;
}HEADERPACKET;

enum {
	PORT_TYPE_TCP = 0,
	PORT_TYPE_UDP,
};

typedef struct {
    u_int32_t type;
    u_int32_t s;
    u_int32_t timeout;
    struct sockaddr_in  addr;
}IO_PORT;

typedef struct
{
	pthread_t listenThread;
	pthread_t clientThread;

	pthread_mutex_t g_mc_mtx;

	u_int32_t networkLoop;
	u_int32_t recvLoop;
	u_int32_t m_socket;

	IO_PORT port;

}NETWORK_CONTEXT;

int initClient();
void termClient();

int send_binary( IO_PORT *p, long nSize, HANDLE pdata );
int recv_binary( IO_PORT *p, long size,  HANDLE pdata );
void makePacket(uint8_t cmd, uint8_t dataType, uint32_t dataSize);
int ClientServiceThread(void *arg);

#endif

#ifdef THIS_IS_SERVER
#ifndef PLZ_R_ONE_TIME
#define PLZ_R_ONE_TIME

NETWORK_CONTEXT *g_pNetwork;
HEADERPACKET sendDataPacket;
char x;
string table_name;

#endif
#endif
