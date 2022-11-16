#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    int width = 640;
    int height = 480;
    int fps = 30;

    cv::VideoCapture cap;
    cv::Mat frame(cv::Size(width, height), CV_8UC3);

    int deviceID = 0;         // 0 = open default camera
    int apiID = cv::CAP_V4L2; // use V4L2
    // open selected camera using selected API
    cap.open(deviceID, apiID);

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width );
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    cap >> frame;
    Mat y;
    
    cvtColor(frame, y, COLOR_BGR2GRAY);
    imwrite("test.jpg", frame);
    return 0;
}