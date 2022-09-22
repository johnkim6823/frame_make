#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;

int key_generation();                                       //make privatKey and PublicKey
int send_pubKey_to_server();                                //Send PublicKey to Server
int init();                                                 //Init Camera Setting and OPEN CAP
void init_all_settings();                                   //Init all settings at the end
void init_queue();                                          //Init all datas in queues
void *UpdateFrame(void *arg);                               //Update Frames
void capture();                                             //Capture frames;
void convert_frames(queue<cv::Mat> &BGR_QUEUE);             //Convert saved frames;
void show_frames_bgr(queue<cv::Mat> &BGR_QUEUE);            //show frames by bgr
void show_frames_yuv(queue<cv::Mat> &YUV420_QUEUE);         //show frames by yuv
void show_frames_y(queue<cv::Mat> &Y_QUEUE);                //show frames by y
void show_frames_feature_vector(queue<cv::Mat> &FV_QUEUE);  //show frames by feature vector
void edge_detection(queue<cv::Mat> &Y_QUEUE);               //Edge detact by y frames
void make_hash(queue<cv::Mat> &FV_QUEUE);                   //make hash using feature vector
void sign_hash(queue<string> &HASH_QUEUE);                  //Sign hash using Private Key
string getCID();                                            //Make CID for each frames 
unsigned char* reshape_yuv(cv::Mat mat);                    //Reshape yuv420 row size to 1 and return char
void send_data_to_server(queue<cv::Mat> &YUV420_QUEUE, queue<string> &HASH_QUEUE, queue<string> &CID_QUEUE);   //send datas to Server
