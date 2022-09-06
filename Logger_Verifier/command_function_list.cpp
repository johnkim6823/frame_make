#include <iostream>
#include <string>

#include "client.h"

using namespace std;

int video_data_response(HEADERPACKET* msg){
	cout << "video data response recv" << endl;
	return 1;
}