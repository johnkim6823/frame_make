#include <iostream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_queue.h"
#include "msg_type.h"

using namespace std;

// Logger(RECV) <- Web UI(SND)
void camera_cfg_recv(int &WIDTH, int &HEIGHT, int &FPS)
{
	int msgid;
	camera_cfg_msg_data data;
	int fps = FPS;
	unsigned char size, format;
	int width = WIDTH, height = HEIGHT;

	if (-1 == (msgid = msgget((key_t)CAMERA_CFG_MQ, IPC_CREAT | 0666)))
	{
		perror("    CAMERA_CFG_MQ creation failed");
		exit(0);
	}

	// datatype 0: receive all datatype
	// change datatype to 1 if you want to receive python data
	if (-1 == msgrcv(msgid, &data, sizeof(camera_cfg_msg_data) - sizeof(long), 0, IPC_NOWAIT))
	{
		cout << "    No Camera Configuaration value received." << endl;
	}
	else
	{
		if (data.data_type == TYPE_CAMERA_CFG)
		{
			//cout << "    Camera configuration received." << endl;
			memcpy(&size, data.data_buff, sizeof(unsigned char));
			memcpy(&format, data.data_buff + sizeof(unsigned char), sizeof(unsigned char));
			memcpy(&fps, data.data_buff + sizeof(unsigned char) * 2, sizeof(int));
		}

		// SET FPS
		FPS = fps;

		// SET SIZE
		switch (size)
		{
		case VGA:
			WIDTH = VGA_WIDTH;
			HEIGHT = VGA_HEIGHT;
			break;
		case HD:
			WIDTH = HD_WIDTH;
			HEIGHT = HD_HEIGHT;
			break;
		case CIF:
			WIDTH = CIF_WIDTH;
			HEIGHT = CIF_HEIGHT;
			break;
		}
		
		sleep(0.2);
		camera_cfg_res_send();
	}
	cout << "    Camera Setting Changes to: " << endl;
	cout << "    width: " << WIDTH << "| height: " << HEIGHT << "| fps: " << FPS << endl;
}

// Logger(RES) -> WEB UI(RECV)
void camera_cfg_res_send()
{
	int msgid;
	camera_cfg_res_msg_data data;
	unsigned char camera_cfg_res = CAMERA_CFG_RES;

	if (-1 == (msgid = msgget((key_t)CAMERA_CFG_RES_MQ, IPC_CREAT | 0666)))
	{
		perror("    CAMERA_CFG_RES_MQ creation failed");
		exit(0);
	}

	// by Header
	data.data_type = TYPE_CAMERA_CFG_RES;
	memcpy(data.data_buff, &camera_cfg_res, sizeof(unsigned char));

	if (-1 == msgsnd(msgid, &data, sizeof(camera_cfg_res_msg_data) - sizeof(long), 0))
	{
		perror("    response send failed ");
		exit(1);
	}

	else
	{
		cout << "    Camera_cfg_res sent." << endl;
	}
}  

// Logger(RECV) <- Web UI(REQ)
void Image_Hash_request(string HASH){
	
	string hash = HASH;
	int msgid;
	Image_hash_req_msg_data data;
	unsigned char image_hash_req;

	if (-1 == (msgid = msgget((key_t)IMAGE_HASH_REQ_MQ, IPC_CREAT | 0666)))
    {
        perror("    IMAGE_HASH_REQ_MQ creation failed.");
        exit(0);
    }

	if (-1 == msgrcv(msgid, &data, sizeof(Image_hash_req_msg_data) - sizeof(long), 0, IPC_NOWAIT))
    {
		cout << "    No request from Web UI.";
    }
	else {
		 if (data.data_type == TYPE_IMAGE_HASH_REQ) {	
			cout << "    Web Requested Hash." << endl;
			Image_Hash_send(hash);
			//cout << "    IMAGE_HASH sent " << endl; 
			//Image_Hash_response();		
		}
	}
}

// Logger(SND) -> Web UI(RECV)
void Image_Hash_send(string HASH)
{
	string Img_HASH = HASH;
	int msgid;
	Image_hash_msg_data data;

	if (-1 == (msgid = msgget((key_t)IMAGE_HASH_MQ, IPC_CREAT | 0666)))
	{
		cout << "    IMAGE_HASH_MQ creation failed." << endl;
		exit(0);
	}

	data.data_type = TYPE_IMAGE_HASH;
	strcpy(data.data_buff, Img_HASH.c_str());

	if ( -1 == msgsnd(msgid, &data, sizeof( Image_hash_msg_data) - sizeof( long), 0))
	{
		perror( "    Hash send Failed.");
		exit( 1);
	}
	else
	{
		cout << "    Data: " << data.data_buff << endl;
	}
}

// Logger(RECV) <- Web UI(RES)
int Image_Hash_response()
{
	int msgid;
	Image_hash_recv_msg_data data;
	unsigned char recv;

	if (-1 == (msgid = msgget((key_t)IMAGE_HASH_RES_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}

	if (-1 == msgrcv(msgid, &data, sizeof(Image_hash_recv_msg_data) - sizeof(long), 0, IPC_NOWAIT))
	{
		cout << "Web UI didn't received path and hash." << endl;
	}
	else
	{
		if (data.data_type == TYPE_IMAGE_HASH_RES)
		{
			cout << "WebUI received Image PATH and HASH." << endl;
			memcpy(&recv, data.data_buff, sizeof(unsigned char));
		}
	}
	return (int)recv;
}

// Verifier(RECV) <- Server(SND)
int Server2Verifier_CID_send(string CID)
{
	int cid_msgid;
	CID_msg_data data;
	if (-1 == (cid_msgid = msgget((key_t)CID_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	data.data_type = TYPE_CID;
	memcpy(&data.data_buff, CID.c_str(), CID.size());
	cout << data.data_buff << endl;

	if (-1 == msgsnd(cid_msgid, &data, sizeof(CID_msg_data) - sizeof(long), 0))
	{
		perror("msgsnd() failed");
		exit(1);
	}
	else
	{
		cout << "Data: " << data.data_buff << endl;
		cout << "CID sent. " << endl;
	}
}

// Verifier(RECV) <- Server(SND)
string Server2Verifier_CID_recv()
{
	int cid_msgid;
	CID_msg_data data;
	char sCID[CID_BUFF_SIZE];
	if (-1 == (cid_msgid = msgget((key_t)CID_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	if (-1 == msgrcv(cid_msgid, &data, sizeof(CID_msg_data) - sizeof(long), 0, 0))
	{
		cout << "No CID from Server." << endl;
	}
	else
	{
		cout << "Server sent CID to Verifier" << endl;
		if (data.data_type == TYPE_CID)
		{
			memcpy(&sCID, data.data_buff, sizeof(char) * CID_BUFF_SIZE);
		}
	}
	cout << sCID << endl;
	return sCID;
}

// Verfier(RES) -> Server(RECV)
int Verifier2Server_CID_res_send()
{
	int cid_recv_msgid;
	CID_recv_msg_data data;
	unsigned char recv = 0x01;
	if (-1 == (cid_recv_msgid = msgget((key_t)CID_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	data.data_type = TYPE_CID_RES;
	memcpy(&data.data_buff, &recv, sizeof(unsigned char));
	if (-1 == msgsnd(cid_recv_msgid, &data, sizeof(CID_recv_msg_data) - sizeof(long), 0))
	{
		perror("msgsnd() failed");
		exit(1);
	}
	else
	{
		cout << "Response sent." << endl;
	}
}

// Verifier(RES) -> Server(RECV)
int Verifier2Server_CID_res_recv()
{
	int cid_recv_msgid;
	CID_recv_msg_data data;
	unsigned char recv;
	if (-1 == (cid_recv_msgid = msgget((key_t)CID_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	if (-1 == msgrcv(cid_recv_msgid, &data, sizeof(CID_recv_msg_data) - sizeof(long), 0, 0))
	{
		cout << "Verifier didn't received CID." << endl;
	}
	else
	{
		if (data.data_type == TYPE_CID_RES)
		{
			cout << "Verifier received CID." << endl;
			memcpy(&recv, data.data_buff, sizeof(unsigned char));
		}
	}
	return (int)recv;
}
