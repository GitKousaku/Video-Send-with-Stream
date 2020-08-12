
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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
#include "ring.hpp"

using namespace std;
using namespace cv;
int main() {
    int srcSocket;  // Ž©•ª
    int dstSocket;  // ‘ŠŽè
    int sock, numrcv;
    struct sockaddr_in addr; 
    struct sockaddr_in dstAddr;   
    int dstAddrSize = sizeof(addr);
    long ttl=0;
    //sock = socket(AF_INET, SOCK_DGRAM, 0);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("IIJIJ\n");

    listen(sock, 1);//
    
    printf("Waiting for connection ...\n");
    dstSocket = accept(sock, (struct sockaddr *) &dstAddr, (socklen_t *)&dstAddrSize);
    printf("Connected from %s\n", inet_ntoa(addr.sin_addr));

//    cvNamedWindow("ReceiveXXXXX", CV_WINDOW_AUTOSIZE);

    cv::Mat image = cv::Mat(480,640,CV_8UC3);
    static const int receiveSize = 65500;
    static char buff[receiveSize];
    vector<uchar> ibuff;

 

    
    int RecvNum=0;
    int TTLNum=0;
    static const int RecvSize=40000;
    static char RecvBuf[RecvSize];

    int Info0Num;
    bool Info0Comp=false;
    int status=0;

    char SyncMark[10];
    strcpy(SyncMark,"Takiguchi");
    char mon[100];
   

    RingBuf ring;
    while(cvWaitKey( 10 ) == -1)
       	{
       	RecvNum = recv(dstSocket,RecvBuf,RecvSize,0);
        
	if (!ring.Input(RecvBuf,RecvNum)) break;
       	TTLNum=TTLNum+RecvNum;
	int pos=0;
	if (status == 0)
		{
		pos=ring.Check(SyncMark);
                printf("MMMM pos %d\n",pos);
                if (pos >= 0)
			{
                        for (int i=0;i<100;i++) mon[i]=0;
                        printf("BBB\n");
			ring.Pick(mon,pos,16);
                        printf("Sync  %s \n",mon);
                        string str=mon;
                        Info0Num=atoi(str.substr(10,7).c_str());
                        printf("Infonum %d \n",Info0Num);
                        status=1;
			}
		}
	if (status == 1)
		{
                 if (ring.BufLength >= Info0Num)
			{
			ring.Pick(buff,Info0Num);
                        Info0Comp=true;
                        status=0;
			}
		}
                        
        if (Info0Comp){
           	ibuff.clear();
            	for(int i=0; i<Info0Num ; i++){
                	ibuff.push_back((uchar)buff[i]);
            	}
            	printf("Info0Comp sizeof buf %ld \n",ibuff.size());
            	image = imdecode(Mat(ibuff), CV_LOAD_IMAGE_COLOR);
            	cv::imshow("Receive", image);
		
            	ibuff.clear();
		status=0;
		Info0Comp=false;
		}
	}
    close(sock);

    return 0;
}
