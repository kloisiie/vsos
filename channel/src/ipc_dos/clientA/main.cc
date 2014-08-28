#include <base/env.h>
#include <base/printf.h>
#include <base/thread.h>
#include <dos_session/client.h>
#include <dos_session/connection.h>
#include <timer_session/client.h>
#include <timer_session/connection.h>
using namespace Genode;
class Client : public Thread<4096>
{
		
	public:

		void entry()
		{
			
			Dos::Connection h;
			//PDBG("running thread");			
			while(true) {
				h.addi(1,3);
				h.subr(34,22);	
			}
		}
};

int main(){
	Timer::Connection time;	
	
	PDBG("start attacker");	
	Client client[60];
	int j=0,i=0;
	int k=4000;
	while(true){	
		for(;j>0&&i<60;i++,j--){
			//PDBG("creating thread");
			client[i].start();	
		}
		time.msleep(1000);
		PERR("open %d thread",i);
		time.msleep(k);
		k+=2000;
		j=5;
	}

}
