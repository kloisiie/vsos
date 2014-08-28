#include <base/env.h>
#include <base/printf.h>
#include <cap_session/connection.h>
using namespace Genode;
char * msg="01010011";
void sendMsg(char *sendmsg){
	PWRN("transfering msg:%s",msg);
	
	PWRN("starting");
	
	while(*sendmsg!='\0'){
		PWRN("transporting one bit");
		
		switch(*sendmsg){
			case '0': 
				PWRN("transporting 0");
				
				break;
			case '1':
				PWRN("transporting 1"); 
				
				break;
			default:	;	
		}
		sendmsg++;
	}
	PWRN("stoping");
	
}
int main(void)
{
	Cap_connection cap_session;
	int i=10;
	while(i-->0){
		Native_capability native=cap_session.alloc(env()->ram_session_cap());
		PWRN("cap id:%d",native.idx()->id());
	}
	return 0;
}
