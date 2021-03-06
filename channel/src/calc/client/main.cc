#include <base/env.h>
#include <base/printf.h>
#include <calc_session/client.h>
#include <calc_session/connection.h>

using namespace Genode;
Calc::Connection h;
char * msg="this is a secret file";
void changeMod(char *msg, int* binary){
	int i=0,j=0,length=strlen(msg);
	PLOG("%d" ,length);	
	for(i=0;i<length;i++){
		int num=msg[i];		
		 while(j<i*8+8)
  		{
  			binary[j++]=num%2;
  			num=num/2;
  		}
	}
	binary[j]=3;
	PLOG("%d",j);	
}
void sendMsg(int *sendmsg){
	PERR("transfering msg:%s",msg);
	int i=100, j=10;
	PLOG("starting: sub");
	int k=h.sub(i,j); //start transport
	while(*sendmsg<=1){
		PLOG("transporting one bit: add");
		h.add(i,j); //transport one bit
		switch(*sendmsg){
			case 0: 
				PLOG("transporting 0: mul");
				h.mul(i,j);  //transport 0
				break;
			case 1:
				PLOG("transporting 1: div"); 
				h.div(i,j);  //transport 1
				break;
			default:	;	
		}
		sendmsg++;
	}
	PLOG("stoping: sub");
	h.sub(i,j);//stop transport
}
int main(void)
{
	int binary[200];
	changeMod(msg,binary);
	sendMsg(binary);	
	return 0;
}
