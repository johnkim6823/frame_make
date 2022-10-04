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
#include <string>

#include "verifier_cfg.h"
#include "verify.cpp"
#include "../msg_queue/msg_queue.cpp"
#include "../DB/bout_database.cpp"

using namespace std;

queue<cv::Mat> yuv420_queue;                    //for original frame(yuv)Mat queue
queue<cv::Mat> y_queue;                         //for y_frame Mat queue
queue<cv::Mat> feature_vector_queue;            //for edge detection Canny
queue<string> hash_DB_queue;                    //for hash from server
queue<string> hash_verifier_queue;              //for hash made by feature vector by verifier
queue<string> cid_queue;                        //for CID for images 


int read_pubKey(){
    ifstream file(pubKey_path);

    if(file.is_open()){
        while(getline(file, publicKey)){
            cout << publicKey << endl;
        }
    } else{
        cout << "Unable to get Pubkey" << endl;
    }
    file.close();
    return 0;
}

void get_data_from_DB(string &CID) {
    string table_name = CID.substr(0,4) + "_" + CID.substr(5,2) + CID.substr(8,2);
    string sorder = "select CID, Hash from " + table_name + " where Verified = 0 order by CID DESC limit 10";
    
    char *order = new char[sorder.length() + 1];
    strcpy(order, sorder.c_str());

    cout << order << endl;
	res = mysql_perform_query(conn, order);
    /*
	while((row = mysql_fetch_row(res)) != NULL){
	    cid_queue.push(row[0]);
		hash_DB_queue.push(row[1]);
	}

    cout << cid_queue.size() << endl;
    cout << hash_DB_queue.size() << endl;
    */
}

/*
void init_all_setting() {
    init_queue();
    
}

void init_queue() {         
    yuv420_queue = queue<cv::Mat>();                 //for original frame(yuv)Mat queue
    y_queue = queue<cv::Mat>();                      //for y_frame Mat queue
    feature_vector_queue = queue<cv::Mat>();         //for edge detection Canny
    hash_DB_queue = queue<string>();
    hash_verifier_queue = queue<string>();          
    cid_queue = queue<string>();                     //for CID for frames
}
*/
/*
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
*/

int main() { 
    read_pubKey();
    
    string S_CID = "2022-09-29_10:30:10.111\0";
    string V_CID = "";
    Server2Verifier_CID_send(S_CID);
    Server2Verifier_CID_recv(V_CID);

    Verifier2Server_CID_recv_send();
    Verifier2Server_CID_recv_recv();

    get_data_from_DB(S_CID);

    return 0;
}
 
