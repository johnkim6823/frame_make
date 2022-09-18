#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h> 

#include "sign.cpp"
#include "merkle_tree.h"
#include "client.cpp"
#include "command_define_list.h"
#include "cfg.h"

using namespace std;
using namespace cv;

int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;
int FPS = DEFAULT_FPS;

cv::VideoCapture cap;
cv::Mat frame(cv::Size(width, height), CV_8UC3);

queue<cv::Mat> bgr_queue;                       //for original frame(BGR)Mat queue
queue<cv::Mat> yuv420_queue;                    //for original frame(yuv)Mat queue
queue<cv::Mat> y_queue;                         //for y_frame Mat queue
queue<cv::Mat> feature_vector_queue;            //for edge detection Canny
queue<string> hash_queue;                       //for hash made by feature vector
queue<string> cid_queue;                        //for CID for images

int key_generation();                                      //make privatKey and PublicKey
int send_pubKey_to_server();
int init();                                                 //Init Camera Setting and OPEN CAP
void init_all_settings();                                    //Init all settings at the end
void init_queue();                                          //Init all datas in queues
void capture();                                             //Capture frames;
void convert_frames(queue<cv::Mat> &BGR_QUEUE);             //Convert saved frames;
void show_frames_bgr(queue<cv::Mat> &BGR_QUEUE);            //show frames by bgr
void show_frames_yuv(queue<cv::Mat> &YUV420_QUEUE);         //show frames by yuv
void show_frames_y(queue<cv::Mat> &Y_QUEUE);                //show frames by y
void show_frames_feature_vector(queue<cv::Mat> &FV_QUEUE);  //show frames by feature vector
void edge_detection(queue<cv::Mat> &Y_QUEUE);               //Edge detact by y frames
void make_hash(queue<cv::Mat> &FV_QUEUE);                   //make hash using feature vector
string getCID();                                            //Make CID for each frames 
unsigned char* reshape_yuv(cv::Mat mat);                    //Reshape yuv420 row size to 1 and return char
void send_data_to_server(queue<cv::Mat> &YUV420_QUEUE, queue<string> &HASH_QUEUE, queue<string> &CID_QUEUE);   //send datas to Server

/*/NEED to make
* 1. GET CAMERA CONFIG from SERVER
*/

int key_generation(){
    cout << "----Key Geneartion----" << endl;
    
	RSA * privateRSA = genPrivateRSA();
	publicKey = genPubicRAS(privateRSA);
    
    cout << "PRIKEY and PUBKEY are made" << endl;
    cout << "----GENERATION END-------" << endl << endl;
	return 0;
}

int send_pubKey_to_server() {
    
    cout << "----SEND PUBKEY to SERVER----" << endl;
	int pubKey_bufsize = publicKey.capacity();
	std::cout << "pubKey_bufsize: " << pubKey_bufsize << std::endl;
	
	char *pubKey_buffer = new char[pubKey_bufsize];
    strcpy(pubKey_buffer, publicKey.c_str());
    
    makePacket(0xff, 0xa0, strlen(pubKey_buffer));
    void *p_packet = &sendDataPacket;
    
    if(!send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet)){
        	cout << "PubKey send Error!!" << endl;
    }
    
    if(!send_binary(&g_pNetwork->port, strlen(pubKey_buffer), (void*)pubKey_buffer)){
        	cout << "PubKey send Error!!" << endl;
    }
}

int init() {
    cout << "----Initalizing----------" << endl << endl;

    //open the default camera using default API
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_V4L2;      // use V4L2
    
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, FPS);

    cout << "    FPS: " << FPS << endl;
    cout << "    Frame Size: " << "WIDHT = " << width << "  HEIGHT = " << height << endl << endl;

    
    //--- If Cap is opened
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    else { 
    	cout << "----Initalized----------" <<endl;
    	return 0;
    }
}
 
void init_all_settings() {
    cap.release();
    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGHT;
    FPS = DEFAULT_FPS;
    init_queue();
    
    cout << endl << "----Initializing all settings." << endl <<endl;
    cout << "    cap closed." <<endl;
    cout << "    bgr queue size: " << bgr_queue.size() << endl;
    cout << "    yuv420 queue size: " << yuv420_queue.size() << endl;
    cout << "    y_frame queue size: " << y_queue.size() << endl;
    cout << "    feature vector queue size: " << feature_vector_queue.size() << endl;
    cout << "    hash queue size: " << hash_queue.size() << endl;
    cout << "    CID queue size: " << cid_queue.size() << endl;
    cout << endl << "--------------------------------" << endl << endl;
}

void init_queue() {         
    yuv420_queue = queue<cv::Mat>();                    //for original frame(yuv)Mat queue
    bgr_queue = queue<cv::Mat>();                    //for original frame(BGR)Mat queue
    y_queue = queue<cv::Mat>();                        //for y_frame Mat queue
    feature_vector_queue = queue<cv::Mat>();                  //for edge detection Canny
    hash_queue = queue<string>();          //for hash made by feature vector
    cid_queue = queue<string>();                          //for CID for frames
}

void capture() {
    cout << endl << "----Starting Capturing" << endl << endl;
    unsigned int capture_count = 0;
    
    while(true){
        cv::Mat currentFrame(cv::Size(height, width), CV_8UC3);
        cap.read(currentFrame);
	    
        if(currentFrame.empty()) {
            cout << "Frame is empty" << endl;
        }
	
        else {
        	//cout << CID.back() << endl;
        	bgr_queue.push(currentFrame);
        	capture_count++;
        
        	//Make CID for FRAMES
        	string s_cid = getCID();
        	cout << capture_count << ": " << s_cid << endl;
        	cid_queue.push(s_cid);
        }
	    
        if (bgr_queue.size() == 50) {break;}
    
        
        //if ESC is pressed, then force thread to end
        if (cv::waitKey(20) == 27 ) {break;}
    }
}


void convert_frames(queue<cv::Mat> &BGR_QUEUE) {
    
    cout << endl << "----Start to convert Frames into YUV420 and Y----" << endl << endl;
    
    queue<cv::Mat> BGR_queue(BGR_QUEUE);
    
    while(true){
        
        if(BGR_queue.size() == 0) {break;}
        
        cv::Mat original = BGR_queue.front();
        cv::Mat yuv_frame(cv::Size(height*3/2, width), CV_8UC1);
        cv::Mat y_frame(cv::Size(height, width), CV_8UC1);
        BGR_queue.pop();

        //CONVERT BGR To YUV420 and YUV420 to Y
        cv::cvtColor(original, yuv_frame, cv::COLOR_BGR2YUV_I420);
        cv::cvtColor(yuv_frame, y_frame, cv::COLOR_YUV2GRAY_I420);
    
        //save frames into queue 
        yuv420_queue.push(yuv_frame);
        y_queue.push(y_frame);
        
    }
    
    cout << "    YUV420 amd Y frame are saved" << endl;
    cout << "    YUV420 frame: " << yuv420_queue.size() << "    Y frame: " << y_queue.size() << endl << endl;
    cout << "----FRAMES CONVERTED---------" << endl << endl;
}

void edge_detection(queue<cv::Mat> &Y_QUEUE) {
    queue<cv::Mat> Y_queue(Y_QUEUE);
    
    cout << "----Building feature vectors." << endl;
    
    int cnt = 0;
    
    while(true) {
        if(Y_queue.size() == 0) {break;}
        cv::Mat temp;
        
        //Canny(img, threshold1, threshold2) 
        //threshold1 = Determining whether an edge is in the adjacency with another edge
        //threshold2 = Determine if it is an edge or not
        cv::Canny(Y_queue.front(), temp, 20, 40);
        
        feature_vector_queue.push(temp);
        Y_queue.pop();
        cnt++;
    }
    cout << endl << "    Edge Detection made: " << feature_vector_queue.size() << endl;
}

void make_hash(queue<cv::Mat> &FV_QUEUE) {
    
    queue<cv::Mat> Feature_Vector_queue(FV_QUEUE);
    
    cout << endl << "----Make HASH from feature vectors." << endl << endl;
    
    while(true) {
        if(Feature_Vector_queue.size() == 0) {break;}
        cv::Mat temp = Feature_Vector_queue.front();
        Feature_Vector_queue.pop();
        
        string mat_data = "";
        string sha_result = "";
        
        
        for(int i =0; i<temp.rows; i++) {
            for(int j =0; j < temp.cols; j++) {
                mat_data += to_string(temp.at<uchar>(i,j));
            }
        }
        
        
        sha_result = hash_sha256(mat_data);
        cout << "hash: " << sha_result << endl;
        
        //sign_HASH
        string signed_hash = signMessage(privateKey, sha_result);
        hash_queue.push(signed_hash);
    }
    
    cout << "    hash made : " << hash_queue.size() << endl;
}

string getCID() {
    struct timeb tb;   // <sys/timeb.h>                       
    struct tm tstruct;                      
    std::ostringstream oss;   
    
    string s_CID;                             
    char buf[128];                                            
                                                              
    ftime(&tb);
    // For Thread safe, use localtime_r
    if (nullptr != localtime_r(&tb.time, &tstruct)) {         
        strftime(buf, sizeof(buf), "%Y-%m-%d_%T.", &tstruct);  
        oss << buf; // YEAR-MM-DD HH-mm_SS            
        oss << tb.millitm; // millisecond               
    }              

    s_CID = oss.str();
    
    s_CID = s_CID.substr(0,23);
    if(s_CID.length() == 22) {
        s_CID = s_CID.append("0");
    }
    if(s_CID.length() == 21) {
        s_CID = s_CID.append("00");
    }
    
    return s_CID;
}

void send_data_to_server(queue<cv::Mat> &YUV420_QUEUE, queue<string> &HASH_QUEUE, queue<string> &CID_QUEUE) {
    cout << endl << "----SEND DATAS to SERVER" << endl;

    queue<cv::Mat> yuv_send(YUV420_QUEUE);
    queue<string> hash_send(HASH_QUEUE);
    queue<string> cid_send(CID_QUEUE);

    // cout << "------------------------" << endl << endl;
    // cout << "VIDEO data QUEUE size: " << yuv_send.size() << endl;
    // cout << "HASH data QUEUE size: " << hash_send.size() << endl;
    // cout << "CID data QUEUE size: " << cid_send.size() << endl;

    int total_data_size = 0;
    int hash_bufsize = hash_send.front().capacity();
    int cid_bufsize = cid_send.front().capacity();
    int video_rows = yuv_send.front().rows;
    int video_cols = yuv_send.front().cols;
    int video_channels = yuv_send.front().channels();
    int video_bufsize = video_rows * video_cols * video_channels;

    total_data_size += video_bufsize;
    total_data_size += hash_bufsize;
    total_data_size += cid_bufsize;
    
    cout << "total data size : " << total_data_size << endl;
    cout << "size of hash data: " << hash_bufsize << endl;
    cout << "size of CID data: " << cid_bufsize << endl;
    cout << "size of video data: " << video_bufsize << endl;
    cout << endl << "---------------------- " << endl;
    
    int step = 0;
    while(true) {
	if(yuv_send.size() == 0 && hash_send.size() == 0 && cid_send.size() == 0) {break;}
        cout << "step : " << ++step << endl;
      	
	    unsigned char *video_buffer = new unsigned char[video_bufsize];
        char *hash_buffer = new char[hash_bufsize];
        char *cid_buffer = new char[cid_bufsize];
        
        memcpy(video_buffer, yuv_send.front().data, video_bufsize);
        strcpy(hash_buffer, hash_send.front().c_str());
        strcpy(cid_buffer, cid_send.front().c_str());

        makePacket(VIDEO_DATA_SND, 0xa1, total_data_size);

        // cout << hex << (int)sendDataPacket.startID << endl;
        // cout << (int)sendDataPacket.destID << endl;
        // cout << (int)sendDataPacket.command << endl;
        // cout << (int)sendDataPacket.dataType << endl;
        // cout << dec << (int)sendDataPacket.dataSize << endl;
        // cout << endl << "----------------------------------------------------------" << endl << endl;
        // cout << "video rows: " << video_rows << endl << "video cols: " << video_cols << endl;
        // cout << "size: " << (strlen((char*)video_buffer)) * sizeof(unsigned char) << endl;
        // cout << "hash: " << hash_buffer << endl << "size: " << strlen(hash_buffer) * sizeof(char) << endl;
        // cout << "CID: " << cid_buffer << endl ;//<< "size: " << strlen(cid_buffer) * sizeof(char) << endl;
        // cout << endl << "----------------------------------------------------------" << endl << endl;

        void *p_packet = &sendDataPacket;
        if(!send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), (void**)p_packet)){
            cout << "Packet send Error!!" << endl;
            break;
        }
        
        if(!send_binary(&g_pNetwork->port, cid_bufsize, (void*)cid_buffer)){
            cout << "CID send Error!!" << endl;
        }

        if(!send_binary(&g_pNetwork->port, hash_bufsize, (void*)hash_buffer)){
            cout << "hash send Error!!" << endl;
        }

        if(!send_binary(&g_pNetwork->port, video_bufsize, (void*)video_buffer)){
            cout << "Image send Error!!" << endl;
        }

        if(ClientServiceThread((void*)&g_pNetwork->port) == -1){
            cout << "ClientServerThread return -1!!" << endl;
            exit(0);
        }
        
        yuv_send.pop();
        hash_send.pop();
        cid_send.pop();
    }
    cout << "----SEND END----------------" << endl;
}

void pk_send_test(char *str, uint8_t datatype, int str_size){
    makePacket(TEST_CMD, datatype, str_size);
    void* p_packet = &sendDataPacket;
    send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), (void**)p_packet);
    send_binary(&g_pNetwork->port, 100, (void**)str);
}


void test() {
    for(int i = 0; i<30;i++){
        Mat xxx;
        // xxx = Mat(352, 288, CV_8UC1, buf);
        // xxx.data = buf;
        xxx = imread("/home/pi/Desktop/lenna.bmp");
        bgr_queue.push(xxx);
        string s;
        s = getCID();
        cid_queue.push(s);
    }
}

void packet_testing_func(uint8_t command, uint8_t datatype, uint32_t datasize){
    makePacket(command, datatype, datasize);
    void* p_packet = &sendDataPacket;
    send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), p_packet);
}

int main(int, char**) { 

    //key GEN
    key_generation();

    //Init Client
    if(!initClient()){
        cout << "init client error!!" << endl;
        return -1;
    }
   
    //CID SEND TEST
    string r_cid1("20220918150714");
    string r_cid2("20220918150715");
    unsigned char* cid1 = new unsigned char[r_cid1.length() + 1];
    unsigned char* cid2 = new unsigned char[r_cid2.length() + 1];
    strcpy((char*)cid1, r_cid1.c_str());
    strcpy((char*)cid2, r_cid2.c_str());

    packet_testing_func(VER_REQ, Uchar, r_cid1.length() + 1);
    send_binary(&g_pNetwork->port, r_cid1.length() + 1, (void*)cid1);
    send_binary(&g_pNetwork->port, r_cid1.length() + 1, (void*)cid2);
   
    
    send_pubKey_to_server();
    /*
    while(true) {
	     if(init() == -1) {break;}
        
	     else{
	 	    cout << "Logger Start working: " << getCID() << endl;
		    
	 	    //capture frames
	 	    capture();
	 	    //test();

		    //convert frames to YUV420 and Y
	 	    convert_frames(bgr_queue);

	 	    //USE Canny Edge Detection with Y_Frames
	 	    edge_detection(y_queue);

	 	    //make Hash by edge_detected datas
	 	    make_hash(feature_vector_queue);

	 	    //send Datas to Server
	 	    send_data_to_server(yuv420_queue, hash_queue, cid_queue);

            //initialize all settings
	 	    init_all_settings();
		    
		    return 0;
        }
    }
    * */
}
