#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <queue>
#include <pthread.h>
#include <thread>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h> 

#include "verify.cpp"
#include "Verifier_function_list.h"
#include "merkle_tree.h"
#include "client.cpp"
#include "command_define_list.h"
#include "verifier_cfg.h"

using namespace std;

int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;
int fps = DEFAULT_FPS;

queue<cv::Mat> bgr_queue;                       //for original frame(BGR)Mat queue
queue<cv::Mat> yuv420_queue;                    //for original frame(yuv)Mat queue
queue<cv::Mat> y_queue;                         //for y_frame Mat queue
queue<cv::Mat> feature_vector_queue;            //for edge detection Canny
queue<string> hash_server_queue;                //for hash from server
queue<string> hash_verifier_queue;              //for hash made by feature vector by verifier
queue<string> cid_queue;                        //for CID for images 


int read_pubKey(){
    fread();    
}


void init_all_setting() {
    init_queue();
    
}

void init_queue() {         
    yuv420_queue = queue<cv::Mat>();                 //for original frame(yuv)Mat queue
    bgr_queue = queue<cv::Mat>();                    //for original frame(BGR)Mat queue
    y_queue = queue<cv::Mat>();                      //for y_frame Mat queue
    feature_vector_queue = queue<cv::Mat>();         //for edge detection Canny
    hash_server_queue = queue<string>();
    hash_verifier_queue = queue<string>();          
    cid_queue = queue<string>();                     //for CID for frames
}

/*
void get_data_from_server() {
            cv::Mat from_server(video_rows, video_cols, CV_8UC1, video_buffer);
        //cv:: Mat re_yuv = from_server.reshape(0, 352);
        while(true) {
            cv::imshow("rgb", from_server);
            cv::waitKey(0);
    
}
*/

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
    cout << endl << "Edge Detection made: " << feature_vector_queue.size() << endl;
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
                mat_data += to_string(temp.at<uchar>(i,j)) + " ";
            }
        }
        
        sha_result = hash_sha256(mat_data);
        hash_verifier_queue.push(sha_result);
    }
    
    cout << "    hash made : " << hash_verifier_queue.size() << endl << endl;
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
        strftime(buf, sizeof(buf), "%Y-%m-%d %T.", &tstruct);  
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

int make_merkle_tree() {
    std::vector<Node*> leaves;
    
    int i = 0;
    while(true) {
        if(hash_verifier_queue.size() == 0) break;
        string hash = (hash_verifier_queue.front());
        leaves.push_back(new Node(hash));
        hash_verifier_queue.pop();
        i = i+1;
    }
    
    // initialize leaves
    for (unsigned int i = 0; i < leaves.size(); i++) {
        leaves[i]->left = NULL;
        leaves[i]->right = NULL;
    }

    MerkleTree *hashTree = new MerkleTree(leaves);
    std::cout << hashTree->root->hash << std::endl;
    hashTree->printTree(hashTree->root, 0);

    for (unsigned int k = 0; k < leaves.size(); k++) {
        delete leaves[k];
    }

    delete hashTree;
    
    return 0;
}

int main(int, char**) { 

    //Init Client
    if(!initClient()){
        cout << "init client error!!" << endl;
        return -1;
    }
    
    return 0;
}
 
