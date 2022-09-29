#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

int read_pubKey();
void init();
void init_queue();
void get_data_from_server();								          //Get Datas from SERVER
void convert_frames(queue<cv::Mat> BGR_QUEUE);				//Convert Datas
void edge_detection(queue<cv::Mat> &Y_QUEUE);         //Edge detact by y frames
void make_hash(queue<cv::Mat> &FV_QUEUE);             //make hash using feature vector
string getCID();                                      //Make CID for each frames
int coompare_hashs();								                  //compare two hashs
string getCID();                                      //Make CID for frames
int make_merkle_tree();
