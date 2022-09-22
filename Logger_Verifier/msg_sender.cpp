////////////////////////// main_sender.c
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

int msq_sender(void)
{
	int      msqid;
	msg_data   data;
	int fps = 30;
	unsigned char size, format;
	size = 0x01;
	format = 0x21;
	
	if ( -1 == ( msqid = msgget( (key_t)1234, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	// by Header
	data.data_type = TYPE_CAMERA_CFG;
	memcpy(data.data_buff, &size, sizeof(unsigned char));
	memcpy(data.data_buff+sizeof(unsigned char), &format, sizeof(unsigned char));
	memcpy(data.data_buff+sizeof(unsigned char)*2, &fps, sizeof(int));
	
	if ( -1 == msgsnd( msqid, &data, sizeof( msg_data) - sizeof( long), 0))
	{
		perror( "msgsnd() failed");
		exit( 1);
	}
	
	else {
		cout << "Data size: " << sizeof(data.data_buff) << endl;
		cout << "MSG_SENT" << endl;
	}

}
