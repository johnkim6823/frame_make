////////////////////////// main_sender.c
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

int msg_sender(void)
{
	int      			confirm_msqid;
	confirm_msg_data   	data;
	unsigned char 		confirm = 0x01;
	
	if ( -1 == ( confirm_msqid = msgget( (key_t)CONFIRM_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	// by Header
	data.data_type = TYPE_CONFIRM_CFG;
	memcpy(data.data_buff, &confirm, sizeof(unsigned char));

	if ( -1 == msgsnd( confirm_msqid, &data, sizeof(confirm_msg_data) - sizeof( long), 0))
	{
		perror( "msgsnd() failed");
		exit( 1);
	}
	
	else {
		cout << "    CONFIRM Message sent." << endl;
	}
}
