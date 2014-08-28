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
#include <rom_session/connection.h>
#include <rom_session/client.h>
#include <dataspace/client.h>
#include <leak_session/connection.h>
namespace Calc {

	struct Connection2 : Genode::Connection<Session>, Session_client
	{
		Connection2(Genode::Native_capability native)
		:
			/* create session */
			Genode::Connection<Session>(Genode::reinterpret_cap_cast<Session>(native)),

			/* initialize RPC interface */
			Session_client(cap()) { }
	};
}
using namespace Genode;
int main(){
	Timer::Connection time;	
	time.msleep(100);
	PERR("beginning test");
	//use the service by name
	PERR("trying to connect to calc server by name");
	try{
		Calc::Connection h;
		PLOG("1+2=%d",h.add(1,2));
	}catch(Exception ex){
		PERR("connect failed");
	}
	time.msleep(2000);

	Cap_connection cap_session;
	Native_capability tmp=cap_session.alloc(env()->ram_session_cap());
	PERR("trying to fake a capability by id");
	Leak::Connection leak;
	int id=leak.getCapID();
	Native_capability native(tmp.idx()->find_by_id(id));	
	try{	
		Calc::Connection2 test(native);
		PLOG("capability id:%d",id);
		PLOG("calling 1+2=%d",id,test.add(1,2));
	}catch(Exception ex){
		PERR("fake failed");				
	}
	return 0;
}
