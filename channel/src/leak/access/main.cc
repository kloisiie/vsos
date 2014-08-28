#include <base/env.h>
#include <base/printf.h>
#include <base/connection.h>
#include <base/capability.h>
#include <base/native_types.h>
#include <rom_session/connection.h>
#include <rom_session/client.h>
#include <dataspace/client.h>
using namespace Genode;
int main(){
	try{
		PLOG("trying to access core rom");
		Rom_connection rom("core");
		Rom_dataspace_capability data=rom.dataspace();
		Dataspace_client tmp2(data);
		PLOG("core rom:: physic address:%lu,writable? %s",tmp2.phys_addr(),tmp2.writable()?"yes":"no");
	}catch(Exception ex){
		PLOG("access core rom failed");
		PLOG("trying to access leak_access rom file");
		Rom_connection rom("leak_access");
		Rom_dataspace_capability data=rom.dataspace();
		Dataspace_client tmp2(data);
		PERR("access leak_access rom successfully!");
		PERR("physical address:%lu,writable? %s",tmp2.phys_addr(),tmp2.writable()?"yes":"no");
	}
	return 0;
}
