#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <map>

using namespace std;
#define THIS_IS_SERVER

#include "server.h"

void* p_packet = &sendDataPacket;
void* recv_buf;
char* CID = new char[CID_size];
char* Hash = new char[Hash_size];
char* Signed_Hash = new char[Signed_Hash_size];
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

int still_alive(HEADERPACKET* msg){
	
}

/*------------------public key send & response----------------------------*/
int public_key_send(HEADERPACKET* msg){
	reshape_buffer(msg->dataType, msg->dataSize);
	
	if(recv_binary(&g_pNetwork->port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}
	string sorder = "select key_ID from public_key where key_status = 1;";
	char* order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	string key_ID = get_latest_key_ID(order);
	
	sorder = "update public_key set key_status = 0 where key_ID = '" + key_ID + "';";
	delete [] order;
	order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	update_database(order);
	
	string pk((char*)recv_buf);
	char *key_value = new char[pk.length() + 1];
	strcpy(key_value, pk.c_str());
	insert_pk_database(getCID(), key_value);

	return 1;

}
int public_key_response(HEADERPACKET* msg){
	makePacket(Logger, PUBKEY_RES, 0xa0, 0);
	
	return send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet);
}
/*------------------------------------------------------------------------*/

/*-------------------video data send & response---------------------------*/
int video_data_send(HEADERPACKET* msg){
	reshape_buffer(msg->dataType, msg->dataSize);

	memset(recv_buf, 0, msg->dataSize);
	memset(CID, 0, CID_size);
	memset(Hash, 0, Hash_size);
	memset(Signed_Hash, 0, Signed_Hash_size);

	int frame_size =  msg->dataSize - CID_size - Hash_size - Signed_Hash_size;
	FILE *file;

	recv_binary(&g_pNetwork->port, CID_size, (void*)recv_buf);
	strcpy(CID, (char*)recv_buf);
	
	
	string s_dir = storage_dir;
	
	if(x != CID[9]){
		table_name = get_table_name();
		mkdir_func((s_dir + table_name).c_str());
		create_table();
		x = CID[9];
	}

	string frame_dir((const char*)recv_buf);
	frame_dir = s_dir + table_name + "/" + frame_dir; 
	const char* file_name = frame_dir.c_str();

	cout << "video file name: " << file_name << endl;

	file = fopen(file_name, "wb");

	if (file == NULL)
		cout << "file creation failed " << endl;
	memset(recv_buf, 0, msg->dataSize);

	recv_binary(&g_pNetwork->port, Hash_size, (void*)recv_buf);
	strcpy(Hash, (char*)recv_buf);
	memset(recv_buf, 0, msg->dataSize);

	recv_binary(&g_pNetwork->port, Signed_Hash_size, (void*)recv_buf);
	strcpy(Signed_Hash, (char*)recv_buf);
	memset(recv_buf, 0, msg->dataSize);

	recv_binary(&g_pNetwork->port, frame_size, (void*)recv_buf);
	fwrite(recv_buf, sizeof(char), frame_size, file);

	
	//cout << "CID: " << endl << CID << endl;
	//cout << "CID size:" << strlen(CID) << endl;

	//cout << "Hash: " << endl << Hash << endl;
	//cout << "Hash size: " << strlen(Hash) << endl;

	//cout << "Signed Hash: " << endl << Signed_Hash << endl;
	//cout << "Signed Hash: " << strlen(Signed_Hash) << endl;
	

	makePacket(Logger, VIDEO_DATA_RES, 0, 0);
	insert_database(CID, Hash, Signed_Hash);

	fflush(file);
	fclose(file);

 	send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), p_packet);
	
	return 1;
}
int video_data_response(HEADERPACKET* msg){
	cout << "video data response recv" << endl;
	return 1;
}
/*------------------------------------------------------------------------*/

/*-----------------------Verify request & response------------------------*/
int verify_request(HEADERPACKET* msg){
	if(msg->destID == Server){
		reshape_buffer(msg->dataType, msg->dataSize);

		//Receive Start CID from WebUI
		recv_binary(&g_pNetwork->port, msg->dataSize, (void*)recv_buf);
		string first_cid((char*)recv_buf);

		//Receive End CID from WebUI
		recv_binary(&g_pNetwork->port, msg->dataSize, (void*)recv_buf);
		string last_cid((char*)recv_buf);

		if(first_cid > last_cid){
			first_cid.swap(last_cid);
		}

		vector<string> pk_list;
		vector<string> CID_list;
		map<string, vector<string>> key_CID_map;

		CIDINFO start_cid = 
		{
			first_cid.substr(0, 4),
			first_cid.substr(5, 2),
			first_cid.substr(8, 2),
			first_cid.substr(11, 2),
			first_cid.substr(14, 2),
			first_cid.substr(17, 2),
			first_cid.substr(20,3),
		};

		CIDINFO end_cid = 
		{
			last_cid.substr(0, 4),
			last_cid.substr(5, 2),
			last_cid.substr(8, 2),
			last_cid.substr(11, 2),
			last_cid.substr(14, 2),
			last_cid.substr(17, 2),
			last_cid.substr(20,3),
		};

		if(start_cid.Day != end_cid.Day){
			vector<string> table_list;
			int j = stoi(end_cid.Day) - stoi(start_cid.Day);
			for(int i = 0; i <= j; i++){
				int d = i + stoi(start_cid.Day);
				string x_table = start_cid.Year + "_" + start_cid.Month + to_string(d);
				table_list.push_back(x_table);
			}
		}
		else{
			string vtable_name = start_cid.Year + '_' + start_cid.Month + start_cid.Day;
			
			get_list(pk_list, "public_key", "-1", first_cid, -1);
			get_list(pk_list, "public_key", first_cid, last_cid, 1);

			get_list(CID_list, vtable_name, first_cid, pk_list[1], 0);
			key_CID_map[pk_list[0]] = CID_list;
			CID_list.clear();
			for(int i = 1; i < pk_list.size() - 1; i++){
				get_list(CID_list, vtable_name, pk_list[i], pk_list[i + 1], 0);
				key_CID_map[pk_list[i]] = CID_list;
				CID_list.clear();
			}

			get_list(CID_list, vtable_name, pk_list[pk_list.size()-1], last_cid, 0);
			key_CID_map[pk_list[pk_list.size()-1]] = CID_list;
			CID_list.clear();
		}

		map<string, vector<string>>::iterator iter;
		for (iter = key_CID_map.begin(); iter != key_CID_map.end(); ++iter) { //ê²??ƒ‰
			vector<string> inVect = (*iter).second;
			int str_size = CID_size * (inVect.size() + 1);

			makePacket(Verifier, VER_REQ, Uchar, str_size);
			//send_binary(Verifier_port, sizeof(HEADERPACKET), p_packet);

			unsigned char *PK = new unsigned char[CID_size];
			strcpy((char*)PK, (*iter).first.c_str());
			
			//send_binary(Verifier_port, CID_size, (void*)PK);

			unsigned char *image_CID = new unsigned char[CID_size];
			for (int j = 0; j < inVect.size(); j++) {
				strcpy((char*)image_CID, inVect[j].c_str());
				//send_binary(Verifier_port, CID_size, image_CID);
			}
		}
	}
	else if(msg->destID == Verifier){
		cout <<"hello:";
	}
	else{
		cout << "Something was wrong..." << endl;
		exit(1);
	}
}

int verify_response(HEADERPACKET* msg){
	if(msg->destID == Server){
		reshape_buffer(msg->dataType, msg->dataSize);
		recv_binary(&g_pNetwork->port, msg->dataSize, (void*)recv_buf);
	}
	else if(msg->destID == WebUI){
		
	}
}
/*------------------------------------------------------------------------*/

/*--------------------------Verify result send----------------------------*/
int verified_result_send(HEADERPACKET* msg){

}
int verified_result_response(HEADERPACKET* msg){

}
/*------------------------------------------------------------------------*/


/*-------------------------Hash request & response------------------------*/
int hash_request(HEADERPACKET* msg){
	makePacket(Logger, HASH_REQ, 0, 0);
	send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), p_packet);

	return 1;
}

int hash_send(HEADERPACKET* msg){
	cout << "hash request receive";
	return 1;
}
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

	return 1;
}