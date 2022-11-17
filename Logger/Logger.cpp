#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <iostream>
#include <queue>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>
#include <fstream>

#include "logger_cfg.h"
#include "Logger_function_list.h"
#include "sign.cpp"
#include "../Merkle_Tree/merkle_tree.h"

using namespace std;
using namespace cv;

int width = 640;
int height = 480;
double fps = 30;

cv::VideoCapture cap;
cv::Mat frame(cv::Size(width, height), CV_8UC3);

pthread_mutex_t frameLocker;
pthread_t UpdThread;

queue<cv::Mat> y_queue;              // for y_frame Mat queue
queue<cv::Mat> feature_vector_queue; // for edge detection Canny
queue<string> hash_queue;            // for hash made by feature vector
queue<string> hash_signed_queue;
queue<string> cid_queue; // for CID for images

int c = 1;
int key_generation()
{
    cout << "----Key Geneartion----" << endl;
    RSA *privateRSA = genPrivateRSA();
    publicKey = genPubicRSA(privateRSA);

    cout << "PRIKEY and PUBKEY are made" << endl;
}


int init()
{
    cout << "----Initalizing---------" << endl
         << endl;

    // open the default camera using default API
    int deviceID = 0;         // 0 = open default camera
    int apiID = cv::CAP_V4L2; // use V4L2
    // open selected camera using selected API
    cap.open(deviceID, apiID);

    //camera_cfg_recv(width, height, fps);

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width );
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    cout << "    Frame Width: " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;
    cout << "    Frame Height: " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;
    cout << "    FPS : " << cvRound(cap.get(CAP_PROP_FPS)) << endl;

    cv::Mat img(cv::Size(width, height), CV_8UC3, Scalar(0));
    frame = img.clone();
    img.release();

    //--- If Cap is opened
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    else
    {
        cout << "----Initalized----------" << endl;
        return 0;
    }
}

void init_all_settings()
{

    init_queue();
}

void init_queue()
{

    while (!y_queue.empty())
        y_queue.pop();
    while (!feature_vector_queue.empty())
        feature_vector_queue.pop();
    while (!hash_queue.empty())
        hash_queue.pop();
    while (!hash_signed_queue.empty())
        hash_signed_queue.pop();
    while (!cid_queue.empty())
        cid_queue.pop();
}

void *UpdateFrame(void *arg)
{
    while (true)
    {
        cv::Mat tempFrame(cv::Size(width, height), CV_8UC3);
        cap >> tempFrame;

        pthread_mutex_lock(&frameLocker);
        frame = tempFrame.clone();
        pthread_mutex_unlock(&frameLocker);
    }
    pthread_exit((void *)0);
}

void booting(){
    for(int i  = 0; i < 20; i++) {
        Mat temp;
        cap >> temp;
    }
    cout << "booting end." << endl;
}

void capture()
{
    int i = 0;
    cout << endl
         << "----Starting Capturing" << endl
         << endl;

    pthread_mutex_init(&frameLocker, NULL);
    pthread_create(&UpdThread, NULL, UpdateFrame, NULL);

    while (true)
    {
        cv::Mat currentFrame(cv::Size(height, width), CV_8UC3, Scalar(0));

        pthread_mutex_lock(&frameLocker);
        currentFrame = frame;
        pthread_mutex_unlock(&frameLocker);

        int sum1 = (int)sum(currentFrame)[0];
        int sum2 = (int)sum(currentFrame)[1];
        int sum3 = (int)sum(currentFrame)[2];
        int elementmean = (sum1 + sum2 + sum3) / 3;

        if (currentFrame.empty())
        {
            cout << "Frame is empty" << endl;
        }
        else if (elementmean != 0)
        {
            Mat yuv(Size(640,720), CV_8UC1, Scalar(0));
            Mat y(Size(640,480), CV_8UC1, Scalar(0));
            cv::cvtColor(currentFrame, yuv, COLOR_BGR2YUV_I420);
            cv::cvtColor(yuv, y, COLOR_YUV2GRAY_I420);
            y_queue.push(y);
            cout << y_queue.size() << endl;
            cid_queue.push(c);
            yuv.release();
            y.release();
            sleep(0.05);
            c++;
        }

        else
        {
            cout << "lamping time" << endl;
        }

        if (y_queue.size() == DEFAULT_FRAME_COUNT)
        {

            int ret = pthread_cancel(UpdThread);
            int status;

            if (ret == 0)
            {
                // auto END
                ret = pthread_join(UpdThread, (void **)&status);
                if (ret == 0)
                {
                    // AUTO END = STATUS= -1
                    cout << "Capture End Successfully." << endl;
                    pthread_mutex_destroy(&frameLocker);
                    currentFrame.release();
                }
                else
                {
                    cout << "Thread End Error!" << ret << endl;
                    pthread_mutex_destroy(&frameLocker);
                    currentFrame.release();
                }
                break;
            }
        }

        // if ESC is pressed, then force thread to end
        if (cv::waitKey(20) == 27)
        {

            int ret = pthread_cancel(UpdThread);
            int status;

            if (ret == 0)
            {
                // auto END
                ret = pthread_join(UpdThread, (void **)&status);
                if (ret == 0)
                {
                    // AUTO END = STATUS= -1
                    cout << "Capture End Successfully." << endl;
                    pthread_mutex_destroy(&frameLocker);
                    currentFrame.release();
                }
                else
                {
                    cout << "Thread End Error!" << ret << endl;
                    pthread_mutex_destroy(&frameLocker);
                    currentFrame.release();
                }
                break;
            }
        }
    }
}

void convert_frames(queue<cv::Mat> &BGR_QUEUE)
{

    cout << endl
         << "----Start to convert Frames into YUV420 and Y----" << endl
         << endl;
    int i = 0;
    while (i > 1000)
    {
        if (BGR_QUEUE.size() == 0)
        {
            break;
        }

        //cv::Mat yuv_frame(cv::Size((height * 3 / 2), width), CV_8UC1, Scalar(0));
        cv::Mat y_frame;
 
        // CONVERT BGR To YUV420 and YUV420 to Y
        //cv::cvtColor(original, yuv_frame, cv::COLOR_BGR2YUV_I420);
        cv::cvtColor(BGR_QUEUE.front(), y_frame, COLOR_YUV2GRAY_I420);

        // save frames into queue
        //yuv420_queue.push(yuv_frame);
        y_queue.push(y_frame);

        // release Mat
        //yuv_frame.release();
        y_frame.release();
        BGR_QUEUE.pop();
        i++;
    }

    cout << "    YUV420 amd Y frame are saved" << endl;
    cout << "    Y frame: " << y_queue.size() << endl
         << endl;
    cout << "----FRAMES CONVERTED---------" << endl
         << endl;
}

void edge_detection(queue<cv::Mat> &Y_QUEUE)
{
    cout << "----Building feature vectors." << endl;

    while (true)
    {
        if (Y_QUEUE.size() == 0)
        {
            break;
        }
        cv::Mat temp;

        // Canny(img, threshold1, threshold2)
        // threshold1 = Determining whether an edge is in the adjacency with another edge
        // threshold2 = Determine if it is an edge or not
        cv::Canny(Y_QUEUE.front(), temp, 20, 40);

        feature_vector_queue.push(temp);
        Y_QUEUE.pop();
        temp.release();
    }
    cout << endl
         << "    Edge Detection made: " << feature_vector_queue.size() << endl;
}

void make_hash(queue<cv::Mat> &FV_QUEUE)
{
    cout << endl
         << "----Make HASH from feature vectors." << endl
         << endl;
    int i = 1;
    while (true)
    {
        if (FV_QUEUE.size() == 0)
        {
            break;
        }
        cv::Mat temp = FV_QUEUE.front();
        FV_QUEUE.pop();

        string mat_data = "";
        string sha_result = "";

        // unsigned char *umat_data = new unsigned char[umat_data_bufsize];
        // memcpy(umat_data, temp.data, umat_data_bufsize);

        for (int i = 0; i < temp.rows; i++)
        {
            for (int j = 0; j < temp.cols; j++)
            {
                mat_data += to_string(temp.at<uchar>(i, j));
            }
        }

        sha_result = hash_sha256(mat_data);
        hash_queue.push(sha_result);
        temp.release();
        cout << i << endl;
        i++;
    }
    cout << "    hash made : " << hash_queue.size() << endl;
}

void sign_hash(queue<string> &HASH_QUEUE)
{
    queue<string> sign(HASH_QUEUE);

    cout << "----Signing Hash by private Key" << endl
         << endl;

    while (true)
    {
        if (sign.size() == 0)
        {
            break;
        }
        string signed_hash = signMessage(privateKey, sign.front());

        char *ch = new char[350];
        strcpy(ch, signed_hash.c_str());

        hash_signed_queue.push(signed_hash);
        sign.pop();
    }
    cout << "    Signed Hash made: " << hash_signed_queue.size() << endl;
}

string getCID()
{
    struct timeb tb; // <sys/timeb.h>
    struct tm tstruct;
    std::ostringstream oss;

    string s_CID;
    char buf[128];

    ftime(&tb);
    // For Thread safe, use localtime_r
    if (nullptr != localtime_r(&tb.time, &tstruct))
    {
        strftime(buf, sizeof(buf), "%Y-%m-%d_%T.", &tstruct);
        oss << buf;        // YEAR-MM-DD HH-mm_SS
        oss << tb.millitm; // millisecond
    }

    s_CID = oss.str();

    s_CID = s_CID.substr(0, 23);
    if (s_CID.length() == 22)
    {
        s_CID = s_CID.append("0");
    }
    if (s_CID.length() == 21)
    {
        s_CID = s_CID.append("00");
    }

    return s_CID;
}

void save_yimage(queue<string> &CID_QUEUE, queue<cv::Mat> &Y_QUEUE) {
    cout << endl
         << "----SAVE Y" << endl;

    queue<string> cid_send(CID_QUEUE);
    queue<Mat> y(Y_QUEUE);
    int video_rows = y.front().rows;
    int video_cols = y.front().cols;
    int video_channels = y.front().channels();
    int video_bufsize = video_rows * video_cols * video_channels;

    cout << "width: " << video_cols << endl;
    cout << "height: " << video_rows << endl;
    cout << "channels: " << video_channels << endl;
    cout << "video size: " << video_bufsize << endl; 

    unsigned char *video_buffer = new unsigned char[video_bufsize];

    while(true) {
        if(cid_send.size() == 0 && y.size() == 0) {break;}
        else{

            FILE *file;

            string file_dir = "./y/" + cid_send.front();
            const char* file_name = file_dir.c_str();
            file = fopen(file_name, "wb");
            memset(video_buffer, 0, video_bufsize);

            memcpy(video_buffer, y.front().data, video_bufsize);
            fwrite(video_buffer, sizeof(unsigned char), video_bufsize, file);
            fflush(file);
            fclose(file);

            cid_send.pop();
            y.pop();
        }
  
    }
    cout << "save y end. " << endl;
}

void save_fvimage(queue<string> &CID_QUEUE, queue<cv::Mat> &FV_QUEUE) {
    cout << endl
         << "----SAVE Feature_vector" << endl;

    queue<string> cid_send(CID_QUEUE);
    queue<Mat> fv(FV_QUEUE);
    
    int video_rows = fv.front().rows;
    int video_cols = fv.front().cols;
    int video_channels = fv.front().channels();
    int video_bufsize = video_rows * video_cols * video_channels;

    unsigned char *video_buffer = new unsigned char[video_bufsize];

    while(true) {
        if(cid_send.size() == 0 && fv.size() == 0) {break;}
        else{

            FILE *file;

            string file_dir = "./feature_vector/" + cid_send.front();
            const char* file_name = file_dir.c_str();
            file = fopen(file_name, "wb");

            memset(video_buffer, 0, video_bufsize);

            memcpy(video_buffer, fv.front().data, video_bufsize);
            fwrite(video_buffer, sizeof(unsigned char), video_bufsize, file);
            fflush(file);
            fclose(file);

            cid_send.pop();
            fv.pop();
        }
  
    }
    cout << "save feature_vector end. " << endl;
}

void write_hash(queue<string> &CID_QUEUE, queue<string> &HASH_QUEUE){
    cout << "write to file" << endl;
    ofstream file1("cid+hash.txt", ios::app);
    ofstream file2("count.txt", ios::app);
    ofstream file3("cid.txt", ios::app);
    ofstream file4("hash.txt", ios::app);
    int CNT = 1;
    if(file1.is_open()){
        while(true){
            if(CID_QUEUE.size() == 0 && HASH_QUEUE.size() == 0) {break;}
            else{
                file1 << CNT << "," << CID_QUEUE.front() << "," << HASH_QUEUE.front() << endl;
                file2 << CNT << endl;
                file3 << CID_QUEUE.front() << endl;
                file4 << HASH_QUEUE.front() << endl;
                CID_QUEUE.pop();
                HASH_QUEUE.pop(); 
                CNT++;
            }
        }
        file1.close();
        file2.close();
        file3.close();
        file4.close();
    }
    else {
    }
    
}

void read_cid() {
    ifstream fin;
    fin.open("cid.txt");

    string line;
    while (!fin.eof()) {
        getline(fin, line);
        cid_queue.push(line);
    }
    cout << cid_queue.size() << endl;
    fin.close();
}

int main(int, char **)
{
    // key GEN
    //key_generation();

    if (init() == -1) {
        exit(1);
    }

    else {
        booting();
        // capture
        capture();

        // saving y_frame into y folder
        save_yimage(cid_queue, y_queue);

        // edge_detection
        edge_detection(y_queue);

        // saving feature_vector frame into feature_vector folder
        save_fvimage(cid_queue, feature_vector_queue);

        make_hash(feature_vector_queue);
        write_hash(cid_queue, hash_queue);

    }
}
