//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

#include <iostream>
#include <vector>
#include <string>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    //ソケットの設定
    int sock;
    struct sockaddr_in addr;
    long ttl=0;

    //sock = socket(AF_INET, SOCK_DGRAM, 0);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);  //ポート番号
    addr.sin_addr.s_addr = inet_addr("192.168.2.116"); //送信先IPアドレス
    //addr.sin_addr.s_addr = inet_addr("172.20.10.13"); //送信先IPアドレス
    printf("test\n");

    printf("Trying to connect to %s: \n", "192.168.2.116");
    connect(sock, (struct sockaddr *) &addr, sizeof(addr));
    printf("DONE\n");
//    VideoCapture cap(0);//デバイスのオープン

    //カメラの設定
    cv::VideoCapture cap(0);
/*    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    int a=cap.get(cv::CAP_PROP_FRAME_WIDTH);
    printf("cap  %d ^^^^^^^^^^\n",a);
    int z=cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    printf("cap  %d ^^^^^^^^^^\n",z);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 960);
*/      
    if(!cap.isOpened()){
        cout << "Camera not Found!" << endl;
        return -1;
    }

    
    cv::Mat frame;
    cv::Mat jpgimg;
    static const int sendSize = 65500;
    char buff[sendSize];
    vector<unsigned char> ibuff;
    vector<int> param = vector<int>(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 85;

    stringstream SyncMark;
    int framenum=0;
    
    while(cvWaitKey(1) == -1){
        //cap >> frame;
        cap.read(frame);
        resize(frame,frame,cv::Size(),0.5,0.5);
        cv::imshow("win", frame);//画像を表示．
	
        cv::imencode(".jpg", frame, ibuff, param);
        int bsize=ibuff.size();
        //printf("KKKK %s \n",numStr);
        SyncMark.str("");
        SyncMark.clear(stringstream::goodbit);

        SyncMark << "Takiguchi" << std::setw(7) << std::setfill('0') << bsize;
	printf("SYNC %s\n",SyncMark.str().c_str());
	
        send(sock,SyncMark.str().c_str(),16,0);
	
        if(ibuff.size() < sendSize ){
            for(int i=0; i<ibuff.size(); i++) buff[i] = ibuff[i];
            //sendto(sock, buff, sendSize, 0, (struct sockaddr *)&addr, sizeof(addr));
	    //send(sock, buff, sendSize+1, 0);
            send(sock, buff, ibuff.size(), 0);
            //jpgimg = cv::imdecode(cv::Mat(ibuff), CV_LOAD_IMAGE_COLOR);
        }
        ibuff.clear();
        framenum++;
        //if (framenum >4) break;
        //sleep(1);              
    }
    
    close(sock);
}


