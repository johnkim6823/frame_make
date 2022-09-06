#include <iostream>
#include <string>

using namespace std;
#define THIS_IS_SERVER

#include "server.h"

int video_data_send(HEADERPACKET* msg){
	unsigned char *recv_buf = new unsigned char[msg->dataSize];
	char* CID = new char[CID_size];
	char* Hash = new char[Hash_size];

	memset(recv_buf, 0, msg->dataSize);
	int frame_size =  msg->dataSize - CID_size - Hash_size;
	FILE *file;

	recv_binary(&g_pNetwork->port, 23, (void*)recv_buf);
	strcpy(CID, (char*)recv_buf);

	string s_dir = storage_dir;

	if(x != recv_buf[9]){
		table_name = get_table_name();
		mkdir_func((s_dir + table_name).c_str());
		create_table();
		x = recv_buf[9];
	}

	string frame_dir((const char*)recv_buf);
	frame_dir = s_dir + table_name + "/" + frame_dir; 
	const char* file_name = frame_dir.c_str();
	file = fopen(file_name, "wb");
	memset(recv_buf, 0, msg->dataSize);

	recv_binary(&g_pNetwork->port, 64, (void*)recv_buf);
	strcpy(Hash, (char*)recv_buf);
	memset(recv_buf, 0, msg->dataSize);

	recv_binary(&g_pNetwork->port, frame_size, (void*)recv_buf);
	fwrite(recv_buf, sizeof(char), frame_size, file);

	makePacket(VIDEO_DATA_RES, 0, 0);
	insert_database(CID, Hash);

	fflush(file);
	fclose(file);
	delete [] recv_buf;
	delete [] CID;
	delete [] Hash;
	void *p_packet = &sendDataPacket;
 	send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), (void**)p_packet);
	return 1;
}

/*
 This function is for test. Receive data and write down .txt file. 

 commmad : 0xff
 dataType : 0xa0 = char
			0xa1 = unsigned char
			0xb0 = int
			0xb1 = unsigned int
*/
int test(HEADERPACKET* msg){
	void* recv_buf;
	FILE *file = fopen("test.txt", "wb");

	switch(msg->dataType){
		case 0xa0 : recv_buf = (char *)recv_buf;
					recv_buf = new char[msg->dataSize];
		case 0xa1 : recv_buf = (unsigned char*)recv_buf;
					recv_buf = new unsigned char[msg->dataSize];
		case 0xb0 : recv_buf = (int *)recv_buf;
					recv_buf = new int[msg->dataSize];
		case 0xb1 : recv_buf = (unsigned int*)recv_buf;
					recv_buf = new unsigned int[msg->dataSize];
	}
	
	if(recv_binary(&g_pNetwork->port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}
	
	fwrite(recv_buf, sizeof(char), msg->dataSize, file);
	
	fflush(file);
	fclose(file);

	delete [] recv_buf;

	return 1;
}