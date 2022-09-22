////////////////////////// main_receiver.c
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

int msq_receiver(void)
{
	int      msqid;
	msg_data   data;
	int   	 fps;
	unsigned char size, format;

	if ( -1 == ( msqid = msgget( (key_t)1234, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	while( 1 )
	{
		// datatype 0: receive all datatype
		// change datatype to 1 if you want to receive python data
		if ( -1 == msgrcv( msqid, &data, sizeof( msg_data) - sizeof( long), 0, 0)) {
			perror( "msgrcv() failed");
			exit( 1);
		}
		printf("*** New message received ***\nRaw data: ");
		
		int i;
		for(i = 0; i<BUFF_SIZE; i++)
			printf("%02X ", data.data_buff[i]);
		printf("\n");
		
		if (data.data_type == TYPE_CAMERA_CFG) {
			memcpy(&size, data.data_buff, sizeof(unsigned char));
			memcpy(&format, data.data_buff+sizeof(unsigned char), sizeof(unsigned char));
			memcpy(&fps, data.data_buff+sizeof(unsigned char)*2, sizeof(int));
			cout << (int)size << endl;
			cout << (int)format << endl;
			cout << fps << endl;
		}
	}
}
