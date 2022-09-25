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
void edge_detection(queue<cv::Mat> &Y_QUEUE);               //Edge detact by y frames
void make_hash(queue<cv::Mat> &FV_QUEUE);                   //make hash using feature vector
void sign_hash(queue<string> &HASH_QUEUE);                  //Sign hash using Private Key
string getCID();                                            //Make CID for each frames
void send_data_to_server(queue<cv::Mat> &YUV420_QUEUE, queue<string> &HASH_QUEUE, queue<string> &CID_QUEUE);   //send datas to Server