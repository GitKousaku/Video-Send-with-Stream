#include <stdio.h>
#include <string>
#include <string.h>
#include "ring.hpp"

using namespace std;


RingBuf::RingBuf(){StartPoint=0;EndPoint=0;BufLength=0;}
RingBuf::~RingBuf(){}

bool RingBuf::Input(char *p,int size){
	if (size > BufSize-BufLength) 
           {printf("Input Fail size=%d BufLength=%d BufSize=%d \n",size,BufLength,BufSize);return false;}
        int ptr=0;
        printf("Input : Recvnum %d\n",size);
        if (EndPoint >= StartPoint)
	   {
	   if (size > BufSize-EndPoint)
                {
                for (int i=EndPoint;i<BufSize;i++) {Buf[i]=p[ptr];ptr++;}
                for (int i=0;i<(EndPoint+size)-BufSize;i++) {Buf[i]=p[ptr];ptr++;}
                EndPoint=(EndPoint+size)-BufSize;
                }
           else
                {
                for (int i=EndPoint;i<EndPoint+size;i++) {Buf[i]=p[ptr];ptr++;}
                EndPoint=EndPoint+size;
		}
           BufLength=BufLength+size;
	   }
        else
	   {
           for (int i=EndPoint;i<EndPoint+size;i++){Buf[i]=p[ptr];ptr++;}
           EndPoint=EndPoint+size;
           BufLength=BufSize-StartPoint+EndPoint;
           }
        printf("Input Comp:StartPoint=%d EndPoint=%d BufLength=%d\n",StartPoint,EndPoint,BufLength);
        return true;
	}

bool  RingBuf::Output(char *p) // Return StartPoint->EndPoint 
{
        int ptr=0;
        
	if (EndPoint>=StartPoint){
		for (int i=StartPoint;i<EndPoint;i++) {p[ptr]=Buf[i];ptr++;}
        }
        else	{
	    	for (int i=StartPoint;i<BufSize;i++)  {p[ptr]=Buf[i];ptr++;}
                for (int i=0;i<EndPoint;i++)  	      {p[ptr]=Buf[i];ptr++;}
                }
        printf("Output Comp:StartPoint=%d EndPoint=%d BufLength=%d\n",StartPoint,EndPoint,BufLength);
	//char ss[16];
	//for (int i=0;i<16;i++) ss[i]=Buf[i+StartPoint];
        //printf("HIHIH  %s\n",ss);
        return true;
}

bool  RingBuf::Output(char *p,int start,int end) // Return start->stop 
{
        int ptr=0;
        
	if (end>=start){
		for (int i=start;i<end;i++) {p[ptr]=Buf[i];ptr++;}
        }
        else	{
	    	for (int i=start;i<BufSize;i++)  {p[ptr]=Buf[i];ptr++;}
                for (int i=0;i<end;i++)  	 {p[ptr]=Buf[i];ptr++;}
                }
	return true;
}

bool RingBuf::Pick(char *p,int size){
        if (BufLength < size ) {printf("Pick Fail\n");return false;}
        int ptr=0;
	if (EndPoint >=StartPoint)
		{
		for (int i=0;i<size;i++) {p[i]=Buf[StartPoint+i];}
                StartPoint=StartPoint+size;
                BufLength=BufLength-size;
                }
	else  	{
		if (BufSize-StartPoint >= size) 
			{
			for (int i=StartPoint;i<StartPoint+size;i++){p[ptr]=Buf[i];ptr++;}
                        StartPoint=StartPoint+size;
                        BufLength=BufLength-size;
                        }
           	else	
			{
			for (int i=StartPoint;i<BufSize;i++){p[ptr]=Buf[i];ptr++;}
                        for (int i=0;i<size-(BufSize-StartPoint);i++){p[ptr]=Buf[i];ptr++;}
			StartPoint=size-(BufSize-StartPoint);
			BufLength=BufLength-size;
			}
		}
        printf("Input Comp:StartPoint=%d EndPoint=%d BufLength=%d\n",StartPoint,EndPoint,BufLength);
	return true;
}

bool RingBuf::Pick(char *p,int start,int size){
        if (BufLength < size ) {printf("Pick Fail1\n");return false;}
        if (EndPoint >= StartPoint) 
		{
                if (start < StartPoint || start >= EndPoint) {printf("Pick Fail2\n");return false;}
                }
        else                        
                {
                if (start < StartPoint && start >= EndPoint) {printf("Pick Fail3\n");return false;}
                }
        if (BufLength < (start-StartPoint)+size) {printf("Pick Fail4\n");return false;}
        
        printf("Pick 1\n");
        int ptr=0;
	if (EndPoint >= StartPoint)
		{
                printf("Pick 2\n");
		for (int i=start;i<start+size;i++) {p[ptr]=Buf[i];ptr++;}
                StartPoint=start+size;
                BufLength=EndPoint-(start+size);
		}
	else  	{
                printf("Pick 3\n");
		if (start >= StartPoint)
			{
                        printf("Pick 4  %d %d %d %d\n",start,BufSize,size,size-(BufSize-start));
			if (start+size>=BufSize){
				for (int i=start;i<BufSize;i++) {p[ptr]=Buf[i];ptr++;}
                                for (int i=0;i<size-(BufSize-start);i++) {p[ptr]=Buf[i];ptr++;}
                                StartPoint=size-(BufSize-start);
                                BufLength=EndPoint-StartPoint;
                                }
                        else	{
				for (int i=start;i<start+size;i++) {p[ptr]=Buf[i];ptr++;}
                        	StartPoint=start+size;
                                BufLength=EndPoint+(BufSize-StartPoint);
				}
                        
			}
		else
			{
                        printf("Pick 5\n");
			for  (int i=start;i<start+size;i++) {p[ptr]=Buf[i];ptr++;}
                        StartPoint=start+size;
                        BufLength=EndPoint-StartPoint;
			}
		}
        printf("Pick:StartPoint=%d EndPoint=%d BufLength=%d\n",StartPoint,EndPoint,BufLength);
	return true;
}


int RingBuf::Check(char *p){
	char ch[BufSize];
        //printf("Check Str %s\n",p);
	Output(ch);
	string str=ch;
        //printf("Str %s\n",str.c_str());
        int pos=str.find(p);
        if (pos >= 0)
		{
		if (EndPoint>=StartPoint){
			pos=StartPoint+pos;
                        }
                else{
		 	if (StartPoint+pos < BufSize) pos=StartPoint+pos;
	                else pos=pos-(BufSize-StartPoint);
                     }
                printf("Check Found at %d  Start:%d End:%d Length:%d\n",pos,StartPoint,EndPoint,BufLength);
                return pos;
		}
	else 
		{
		printf("Check No Found\n");
		return -1;
		}
}

/*
int main()
{
	//RingBuf ring();
        char SyncMark[10];
        strcpy(SyncMark,"Takiguchi");
        SyncMark[9]=0;
	static char s[100];
	string str;
	str="0001Takiguchi";
        str=str+"0032394";
        str=str+"0044";
        for (int i=0;i<2;i++) str=str+"0123456789";
        printf("JJJJ:%s,%ld\n",str.c_str(),strlen(str.c_str()));
	

	char b[50];
        for (int i=0;i<50;i++) b[i]=0;
        char c[200];
        strcpy(b,str.c_str());
	printf("input:%s\n",b); 


	RingBuf ring;
	ring.Input(b,strlen(b));
        
        for (int i=0;i<100;i++) c[i]=0;
	ring.Output(c,20,40);
        printf("Output20-40:%s\n",c);

	for (int i=0;i<100;i++) c[i]=0;
        ring.Pick(c,30);
        printf("Pick30:%s\n",c);

	for (int i=0;i<100;i++) c[i]=0;
        ring.Pick(c,10);
        printf("Pick10:%s\n",c);

       	ring.Input(b,40);
 
	for (int i=0;i<100;i++) c[i]=0;
        ring.Output(c);
        printf("OutPut:%s\n",c);

        int p=ring.Check(SyncMark);
        printf("Check %d\n",p);

        for (int i=0;i<100;i++) c[i]=0;
        ring.Pick(c,p,20);
        printf("PickCheck: %s\n",c);
        
	for (int i=0;i<100;i++) c[i]=0;
        ring.Output(c);
        printf("OutPut:%s\n",c);

	return 0;

}
*/


	

