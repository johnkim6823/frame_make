#include <iostream>
#include <string>

#define THIS_IS_SERVER

#include "client.h"

using namespace std;

void* p_packet = &sendDataPacket;
void* recv_buf;
char* CID = new char[CID_size];
char* Hash = new char[Hash_size];
FILE* file;

void reshape_buffer(int type, int datasize){
	switch(type){
		case 0xa0 : case 0xc0 : 
					recv_buf = (char *)recv_buf;
					recv_buf = new char[datasize];
					break;
		case 0xa1 : recv_buf = (unsigned char*)recv_buf;
					recv_buf = new unsigned char[datasize];
					break;
		case 0xb0 : recv_buf = (int *)recv_buf;
					recv_buf = new int[datasize];
					break;
		case 0xb1 : recv_buf = (unsigned int*)recv_buf;
					recv_buf = new unsigned int[datasize];
					break;
	}
}

/*------------------public key send & response----------------------------*/
int public_key_send(HEADERPACKET* msg){
	void* recv_buf;
	reshape_buffer(msg->dataType, msg->dataSize);

	recv_binary(&g_pNetwork->port, msg->dataSize, (void*)recv_buf);
	file = fopen("public_key.txt", "wb");
	fwrite(recv_buf, sizeof(char), msg->dataSize, file);
	fflush(file);

	delete [] recv_buf;
	fclose(file);
}
int public_key_response(HEADERPACKET* msg){
	makePacket(PUBKEY_RES, 0xa0, 0);
	
	return send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet);
}
/*------------------------------------------------------------------------*/

/*-------------------video data send & response---------------------------*/
// int video_data_send(HEADERPACKET* msg){
// 	reshape_buffer(msg->dataType, msg->dataSize);

// 	memset(recv_buf, 0, msg->dataSize);
// 	memset(CID, 0, CID_size);
// 	memset(Hash, 0, Hash_size);

// 	int frame_size =  msg->dataSize - CID_size - Hash_size;
// 	//FILE *file;

// 	recv_binary(&g_pNetwork->port, 23, (void*)recv_buf);
// 	strcpy(CID, (char*)recv_buf);

// 	string s_dir = storage_dir;

// 	if(x != CID[9]){
// 		table_name = get_table_name();
// 		mkdir_func((s_dir + table_name).c_str());
// 		create_table();
// 		x = CID[9];
// 	}

// 	string frame_dir((const char*)recv_buf);
// 	frame_dir = s_dir + table_name + "/" + frame_dir; 
// 	const char* file_name = frame_dir.c_str();
// 	file = fopen(file_name, "wb");
// 	memset(recv_buf, 0, msg->dataSize);

// 	recv_binary(&g_pNetwork->port, 64, (void*)recv_buf);
// 	strcpy(Hash, (char*)recv_buf);
// 	memset(recv_buf, 0, msg->dataSize);

// 	recv_binary(&g_pNetwork->port, frame_size, (void*)recv_buf);
// 	fwrite(recv_buf, sizeof(char), frame_size, file);

// 	makePacket(VIDEO_DATA_RES, 0, 0);
// 	insert_database(CID, Hash);

// 	fflush(file);
// 	// fclose(file);
// 	delete [] recv_buf;
// 	delete [] CID;
// 	delete [] Hash;

//  send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), p_packet);
// 	return 1;
// }
int video_data_response(HEADERPACKET* msg){
	cout << "video data response recv" << endl;
	return 1;
}
/*------------------------------------------------------------------------*/

// /*-----------------------Verify request & response------------------------*/
// int verify_request(HEADERPACKET* msg){
// 	reshape_buffer(msg->dataType, msg->dataSize);
// 	recv_binary(&g_pNetwork->port, msg->dataSize, (void*)recv_buf);

// 	string sorder(recv_buf);
// 	sorder = "select Hash from " + sorder.substr(9) + " where CID like '%" + sorder.substr(9, -1) + "%';";
// 	char *order = new char[sorder.length() + 1];
// 	strcpy(order, sorder.c_str());
// 	res = mysql_perform_query(conn, order);
// 	while((row = mysql_fetch_row(res)) != NULL)
// 		cout << row[0] << endl;

// 	return 1;
// }
// int verify_response(HEADERPACKET* msg){
	
// }
// /*------------------------------------------------------------------------*/

// /*--------------------------Verify result send----------------------------*/
// int verified_result_send(HEADERPACKET* msg){

// }
// int verified_result_response(HEADERPACKET* msg){

// }
// /*------------------------------------------------------------------------*/

/*-------------------------Hash request & response------------------------*/
// int hash_request(HEADERPACKET* msg){
// 	reshape_buffer(msg->dataType, msg->dataSize);
// 	recv_binary(&g_pNetwork->port, msg->dataSize, (void*)recv_buf);

// 	string sorder;
// 	//sorder = "select Hash from (table) where CID like '%" + recv_buf + "%';";

// 	makePacket(HASH_REQ, Uchar, CID_size);
// 	send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), p_packet);

// 	//recv_binary(&g_pNetwork->port, HASH_size, )
// 	return 1;
// }

// int hash_send(HEADERPACKET* msg){
// 	cout << "hash request receive";
// 	return 1;
// }
/*------------------------------------------------------------------------*/


/*
 This function is for test. Receive data and write down .txt file. 
 commmad : 0xff
 dataType : 0xa0 = char
			0xa1 = unsigned char
			0xb0 = int
			0xb1 = unsigned int
*/
int test(HEADERPACKET* msg){
	FILE *file = fopen("test.txt", "wb");
	reshape_buffer(msg->dataType, msg->dataSize);
	
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