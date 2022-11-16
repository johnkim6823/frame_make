using namespace std;

int key_generation();                                                                                                                     // make privatKey and PublicKey
int init();                                                                                                                               // Init Camera Setting and OPEN CAP
void init_all_settings();                                                                                                                 // Init all settings at the end
void init_queue();                                                                                                                        // Init all datas in queues
void *UpdateFrame(void *arg);                                                                                                             // Update Frames
void capture();         
void convert_frames(queue<cv::Mat> &BGR_QUEUE);                                                                                           // Convert saved frames;
void edge_detection(queue<cv::Mat> &Y_QUEUE);                                                                                             // Edge detact by y frames
void make_hash(queue<cv::Mat> &FV_QUEUE);                                                                                                 // make hash using feature vector
void sign_hash(queue<string> &HASH_QUEUE);                                                                                                // Sign hash using Private Key
string getCID();                                                                                                                          // Make CID for each frames
void send_image_hash_to_UI(queue<cv::Mat> &ORI, queue<cv::Mat> &Y);                                                                       // SEND BGR_Image,png, Y_Image.png PATH and Hash code
void save_yuvimage(queue<string> &CID_QUEUE, queue<cv::Mat> &YUV420_QUEUE); // send datas to Server
void save_yimage(queue<string> &CID_QUEUE, queue<cv::Mat> &Y_QUEUE); // send datas to Server
void save_fvimage(queue<string> &CID_QUEUE, queue<cv::Mat> &FV_QUEUE); // send datas to Server
//temp
void write_hash(queue<string> &CID_QUEUE, queue<string> &HASH_QUEUE);
void read_cid();
void read_y(queue<string>  &CID_QUEUE);