# Logger_Verifier + CLient + Server

Client: empty folder -> Client RELATED codes are moved to Logger_Verifier folder

Logger_Verifier: Logger, Verifier folder. 

Server: Server Folder

# How to Use
+ prerequisite

RPI -> 32 bit

OpenCV 4.5.1 required -> See OpenCV_Download_manual.

Cryptopp required -> See Cryptopp_Download_manual.

MariaDB required -> See MariaDB_Download_manual.

After git clone plese mv Logger_Verifier to hanium_2022

mv Logger_Verifier hanium_2022

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
make clean && make Logger Verifier
```

# TEST
1. Logger
```
Logger: Capture frames by Webcam, and convert into YUV420.
        USE only Y-frames to find feature vector. USE feature vector to make hash
        Then send YUV420 video data, hash, Contents ID to SERVER
```
```
IF you can't use Camera to Capture then comment line 406 and 417. After commenting two lines, then add test() below cpature().
    
```

2. Verifier
```
Verifier: GET DATAS from SERVER and CHECK for video data's integrity
```
```
./Verifiy
```
## 2022 09 01 changes -by MJ
Camera settings are moved to cfg.h. No changes are needed.

## 2022 09 07 changes -by MJ
Private Key and Public Key generation added

-> sign.cpp: codes for generating Key and sign hash
