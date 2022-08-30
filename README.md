# Logger_Verifier + CLient + Server

Client: empty folder -> Client RELATED codes are moved to Logger_Verifier folder

Logger_Verifier: Logger, Verifier folder. 

Server: Server Folder

# How to Use
0. prerequisite
 RPI -> 32 bit

 OpenCV 4.5.1 required -> See OpenCV_Download_manual.

 Cryptopp required -> See Cryptopp_Download_manual.

 MariaDB required -> See MariaDB_Download_manual.

1. Logger configuration Change needed.
```
cd Logger_Verifier
```
```
nano/vi/vim cfg.h
```
```
change SERVER_IP_ADDR/SERVER_PORT to your RPI's working as Server
```
2. compile 
```
make clean && make Logger Verifier sign_verify
```

# TEST
1. Logger
```
Logger: Capture frames by Webcam, and convert into YUV420.
        USE only Y-frames to find feature vector. USE feature vector to make hash
        Then send YUV420 video data, hash, Contents ID to SERVER
```
```
to Test only CAPTURE, CONVERTING change main() to 
    //if(!initClient()){
    //    cout << "init client error!!" << endl;
    //    return -1;
    //}
    
    // main
    init();
    
    //if you need only one frame for test then use test() not capture();
    cout << "start time : "<<getCID() << endl;
    capture();
    
    show_frames_bgr(bgr_queue);
    //Convert fames to YUV and Y
    convert_frames(bgr_queue);
    
    //USE Canny Edge Detection with Y_frames
    edge_detection(y_queue);
    
    // //Make hash by edge_detected datas;
    make_hash(feature_vector_queue);

    //Send Data to Server
    //send_datas_to_server(yuv420_queue, hash_queue, cid_queue);
    
    init_all_setting();
    return 0;
```
```
to Test only CAPTURE, CONVERTING and SEND DATAS to SERVER 
FIRST go to Server folder and ./servetTest then change main()

    if(!initClient()){
        cout << "init client error!!" << endl;
        return -1;
    }
    
    // main
    init();
    
    //if you need only one frame for test then use test() not capture();
    cout << "start time : "<<getCID() << endl;
    capture();
    
    show_frames_bgr(bgr_queue);
    //Convert fames to YUV and Y
    convert_frames(bgr_queue);
    
    //USE Canny Edge Detection with Y_frames
    edge_detection(y_queue);
    
    // //Make hash by edge_detected datas;
    make_hash(feature_vector_queue);

    //Send Data to Server
    send_datas_to_server(yuv420_queue, hash_queue, cid_queue);
    
    init_all_setting();
    return 0;
```
2. Verifier
```
Verifier: GET DATAS from SERVER and CHECK for video data's integrity
```
```
./Verifiy
```
3. sign_verifiy
```
sign_verify: TESTING FOR MAKING PUB KEY, PRI KEY
	     IT's usage to sign hash and verify hash
```
```
./sign_verify
```


