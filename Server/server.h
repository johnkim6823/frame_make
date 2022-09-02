
#include <pthread.h>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>

using namespace std;

#include "cfg.h"

#define HANDLE void*

#define TRUE 1
#define FALSE 0
#define INVALID_SOCKET -1

#define SOCKET u_int32_t

#ifndef __SERVERSOCKET_HEADER__
#define __SERVERSOCKET_HEADER__

struct db_user {
	char *server;
	char *user;
	char *password;
	char *database;
	std::string table;
};

typedef struct
{
    u_int8_t Year;
    u_int8_t Month;
    u_int8_t Day;
    u_int8_t Hour;
    u_int8_t Min;
    u_int8_t Sec;
    u_int8_t MSec;
} CIDINFO;

typedef struct
{
	u_int8_t startID; //로거, 검증기, 서버 ...
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

	struct db_user mysqlID;

	IO_PORT port;

}NETWORK_CONTEXT;



int initServer();
void termServer();

int send_binary( IO_PORT *p, long nSize, HANDLE *pdata );
int recv_binary( IO_PORT *p, long size, void *pdata );

string get_table_name();
void mkdir_func(string str);
void create_table();
void makePacket(uint8_t cmd, uint8_t dataType, uint32_t dataSize);
void insert_database(char* CID, char* Hash);

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