#include <base/env.h>
#include <base/printf.h>
#include <calc_session/client.h>
#include <calc_session/connection.h>
#include <timer_session/client.h>
#include <timer_session/connection.h>
using namespace Genode;
int main(void)
{
	Timer::Connection time;
	Calc::Connection h;	
	while(true){
		PERR("start invoke server");
		long start=time.elapsed_ms();
		for(int i=0;i<10000;i++){
			h.sub(2,1);
			h.add(3,2);
			h.mul(1,2);
			h.div(2,1);			
		}
		long end=time.elapsed_ms();	
		PERR("total time: %ld ms",(end-start)/1000);
		time.msleep(10000);	
	}
	return 0;
}
