#include <base/env.h>
#include <base/printf.h>
#include <dos_session/client.h>
#include <dos_session/connection.h>
#include <confidential_session/connection.h>
#include <timer_session/client.h>
#include <timer_session/connection.h>
using namespace Genode;
int main(void)
{
	Timer::Connection time;
	Dos::Connection h;
	while(true){
		PERR("start invoke server");
		long start=time.elapsed_ms();
		PERR("invoking...");
		for(int i=0;i<1000;i++){
			h.addi(1,2);
			h.subr(2,1);
						
		}		
		long end=time.elapsed_ms();	
		PERR("total time: %d ms",(end-start));
		time.msleep(500);	
		
	}
	return 0;
}
