#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_queue.h"

using namespace std;

int camera_cfg_recv(int &WIDTH, int &HEIGHT, int &FPS) {
	int      		camera_cfg_msgid;
	config_msg_data data;
	int   	 		fps = FPS;
	unsigned char 	size, format;
	int 			width = WIDTH, height = HEIGHT;
	
	if ( -1 == ( camera_cfg_msgid = msgget( (key_t)CAMERA_CFG_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}
 	
	sleep(0.1);
	// datatype 0: receive all datatype
	// change datatype to 1 if you want to receive python data
	if ( -1 == msgrcv( camera_cfg_msgid, &data, sizeof(config_msg_data) - sizeof( long), 0, IPC_NOWAIT)) {
		cout << "    No Camera Configuaration value received." << endl;
	}
	else {
		if (data.data_type == TYPE_CAMERA_CFG) {
			cout << "    Camera configuration received." << endl;	
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
		confirm_cfg_send();
	}
	cout << "    width: " << WIDTH << "| height: " << HEIGHT << "| fps: " << FPS << endl;
	return 0;
}

int confirm_cfg_send() {
	int      			camera_cfg_recv_msgid;
	confirm_msg_data   	data;
	unsigned char 		confirm = 0x01;
	
	if ( -1 == ( camera_cfg_recv_msgid = msgget( (key_t)CAMERA_CFG_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	// by Header
	data.data_type = TYPE_CAMERA_CFG_RECV;
	memcpy(data.data_buff, &confirm, sizeof(unsigned char));

	if ( -1 == msgsnd( camera_cfg_recv_msgid, &data, sizeof(confirm_msg_data) - sizeof( long), 0))
	{
		perror( "msgsnd() failed");
		exit( 1);
	}
	
	else {
		cout << "    CONFIRM Message sent." << endl;
	}
}

int Image_HASH_send(string HASH){
	string Img_HASH = IMAGE_PATH + HASH + "\0";
	int					image_hash_send_msqid;
	Image_hash_msg_data	data;

	if ( -1 == ( image_hash_send_msqid = msgget( (key_t)IMAGE_HASH_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	data.data_type = TYPE_IMAGE_HASH;
	memcpy(&data.data_buff, Img_HASH.c_str(), Img_HASH.size());

	if ( -1 == msgsnd( image_hash_send_msqid, &data, sizeof(Image_hash_msg_data) - sizeof( long), 0))
	{
		perror( "msgsnd() failed");
		exit( 1);
	}
	else {
		cout << "Data: " << data.data_buff << endl;
		cout << "PATH and HASH sent." << endl;
	}
}

int Image_Hash_recv() {
	int      					image_hash_recv_msgid;
	Image_hash_recv_msg_data	data;
	unsigned char recv;

	if ( -1 == ( image_hash_recv_msgid = msgget( (key_t)IMAGE_HASH_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	if ( -1 == msgrcv( image_hash_recv_msgid, &data, sizeof(Image_hash_recv_msg_data) -sizeof( long), 0, IPC_NOWAIT)) {
		cout << "Web UI didn't received path and hash." << endl;
	}
	else{
		if (data.data_type == TYPE_IMAGE_HASH_RECV) {
			cout << "WebUI received Image PATH and HASH." << endl;	
			memcpy(&recv, data.data_buff, sizeof(unsigned char));
		}
	}
	return (int)recv;
}

int Server2Verifier_CID_send(string &CID ) {
	int      		cid_msgid;
	CID_msg_data	data;

	if ( -1 == ( cid_msgid = msgget( (key_t)CID_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	data.data_type = TYPE_CID;
	memcpy(&data.data_buff, CID.c_str(), CID.size());
	cout << data.data_buff << endl;

	if ( -1 == msgsnd( cid_msgid, &data, sizeof(CID_msg_data) - sizeof( long), 0))
	{
		perror( "msgsnd() failed");
		exit( 1);
	}
	else {
		cout << "Data: " << data.data_buff << endl;
		cout << "CID sent. " << endl;
	}
	return 0;
}

int Server2Verifier_CID_recv(string &CID){
	int      		cid_msgid;
	CID_msg_data	data;
	char 			sCID[CID_BUFF_SIZE];

	if ( -1 == ( cid_msgid = msgget( (key_t)CID_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}

	if ( -1 == msgrcv( cid_msgid, &data, sizeof(CID_msg_data) - sizeof(long), 0, 0)) {
		cout << "No CID from Server." << endl;
	}
	else {
		cout << "Server sent CID to Verifier" << endl;	
		if (data.data_type == TYPE_CID) {
			memcpy(&sCID, data.data_buff, sizeof(char)*CID_BUFF_SIZE);
		}
	}
	CID = sCID;
	cout << CID << endl;
}  

int Verifier2Server_CID_recv_send() {
	int      		cid_recv_msgid;
	CID_recv_msg_data	data;
	unsigned char recv	= 0x01;

	if ( -1 == ( cid_recv_msgid = msgget( (key_t)CID_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}
	
	data.data_type = TYPE_CID_RECV;
	memcpy(&data.data_buff, &recv, sizeof(unsigned char));

	if ( -1 == msgsnd( cid_recv_msgid, &data, sizeof(CID_recv_msg_data) - sizeof( long), 0))
	{
		perror( "msgsnd() failed");
		exit( 1);
	}
	else {
		cout << "received sent." << endl;
	}

}

int Verifier2Server_CID_recv_recv() {
	int      			cid_recv_msgid;
	CID_recv_msg_data	data;
	unsigned char recv;

	if ( -1 == ( cid_recv_msgid = msgget( (key_t)CID_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror( "msgget() failed");
		exit( 1);
	}
	
	data.data_type = TYPE_CID_RECV;
	if ( -1 == msgrcv( cid_recv_msgid, &data, sizeof(CID_recv_msg_data) - sizeof(long), 0, 0)) {
		cout << "Verifier didn't received CID." << endl;
	}
	else {	
		if (data.data_type == TYPE_CID_RECV) {
			cout << "Verifier received CID." << endl;
			memcpy(&recv, data.data_buff, sizeof(unsigned char));
		}
	}
	return (int)recv;
}