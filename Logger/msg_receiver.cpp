////////////////////////// main_receiver.c
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_sender.cpp"

using namespace std;

int msg_receiver(int &WIDTH, int &HEIGHT, int &FPS)
{
	int      		recv_msgid;
	config_msg_data data;
	int   	 		fps = FPS;
	unsigned char 	size, format;
	int 			width = WIDTH, height = HEIGHT;
	
	if ( -1 == ( recv_msgid = msgget( (key_t)MSG_Q_CONFIG_KEY, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}
 	
	// datatype 0: receive all datatype
	// change datatype to 1 if you want to receive python data
	if ( -1 == msgrcv( recv_msgid, &data, sizeof(config_msg_data) - sizeof( long), 0, IPC_NOWAIT)) {
		cout << "    No Camera Configuaration value received." << endl;
	}
	else {
		cout << "    Camera configuration received." << endl;	
		if (data.data_type == TYPE_CAMERA_CFG) {
			memcpy(&size, data.data_buff, sizeof(unsigned char));
			memcpy(&format, data.data_buff+sizeof(unsigned char), sizeof(unsigned char));
			memcpy(&fps, data.data_buff+sizeof(unsigned char)*2, sizeof(int));
		}
		
		//SET FPS
		FPS = fps;
		
		//SET SIZE
		switch(size){
			case 0x01: 
				WIDTH = 640;
				HEIGHT = 480;
				break;
			case 0x02:
				WIDTH = 1280;
				HEIGHT = 720;
				break;
			case 0x03:
				WIDTH = 352;
				HEIGHT = 288;
				break;
		}
		msg_sender();
	}
	cout << "    width: " << WIDTH << "| height: " << HEIGHT << "| fps: " << FPS << endl;
	return 0;
}
