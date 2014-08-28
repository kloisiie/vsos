#include <base/env.h>
#include <base/printf.h>
#include <calc_session/connection.h>
#include <base/native_types.h>
#include <cap_session/connection.h>
#include <calc_session/client.h>
#include <base/connection.h>
#include <base/capability.h>
#include <timer_session/client.h>
#include <timer_session/connection.h>
using namespace Genode;
int main(){
	Timer::Connection time;
	time.msleep(10000);
	try{
		Calc::Connection h;
		PWRN("1+2=%d",h.add(1,2));
		PERR("access Calc service successfully");
	}catch(Exception ex){
	     PERR("access Calc service failed");
	}
}
