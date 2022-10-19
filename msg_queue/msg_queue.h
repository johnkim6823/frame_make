using namespace std;
// Logger <--> Web UI                                       //Logger <----> WEbUI
void camera_cfg_res_send();                                  //Logger(SND)  -> Web UI(RECV)      B
void camera_cfg_recv(int &WIDTH, int &HEIGHT, int &FPS);     //Logger(RECV) <- Web UI(SND)       BBI

void Image_Hash_request(string HASH);                        //Logger(RECV) <- Web UI(REQ)       B
void Image_Hash_send(string HASH);                           //Logger(SND)  -> Web UI(RECV)      ss
int Image_Hash_response();                                  //Logger(RECV) <- Web UI(SND))      B

// Verifier <--> Server                                     //Verifier <-------> Server
int Server2Verifier_CID_send(string CID );                 //Server(SND)   -> Verifier(RECV)   s
string Server2Verifier_CID_recv(string CID);                  //Server(CND)   -> Verifier(RECV)   s
int Verifier2Server_CID_res_send();                         //Verifier(SND) -> Server(RECV)     B
int Verifier2Server_CID_res_recv();                         //Verifier(SND) -> Server(RECV      B       

int Verifier2Server_VERIFIED_RSLT_send();                   //Server(SND)   <- Verifier(RECV)   sssi  
int Verifier2Server_VERIFIED_RSLT_recv();                   //Server(SND)   <- Verifier(RECV)   sssi      
int Server2Verifier_VERIFIED_RSLT_res_send();               //Server(SND)   -> Verifier(RECV)   B    
int Server2Verifier_VERIFIED_RSLT_res_recv();               //Server(SND)   -> Verifier(RECV)   B