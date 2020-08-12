#include <stdio.h>
#include <string>
#include <string.h>



class RingBuf
{
	public:
	RingBuf();
 	~RingBuf();
        
	
	static const int BufSize=320000;
	char	Buf[BufSize];
	int	StartPoint;
      	int	EndPoint;
	int	BufLength;
	
	bool	Input(char *p,int size);
        bool    Output(char *p);
        bool 	Output(char *p,int start,int end);
	bool	Pick(char *p,int size);
	bool	Pick(char *p,int start,int size);
	int	Check(char *p);
	//
    	
};
