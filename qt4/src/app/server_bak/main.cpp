/*
 * \brief   commserv
 * \author  OS5
 * \date    2013-09-22
 */

/* Qt includes */
#include <base/printf.h>
#include <base/env.h>
#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <back_session/back_session.h>
#include <base/rpc_server.h>

namespace MSG{
	int opcodes[100];
	int args[100];
	int cur=0;
	bool locked=true;
	void lockcur(){
		while(true){
			if(locked)
				break;
		}
		locked=false;
	}
	void unlockcur(){
		locked=true;
	}
	
	struct Session_component : Genode::Rpc_object<Session>
	{
		
		int read=0;		
		void set_msg(int op,int arg) {
			lockcur();
			opcodes[cur]=op;
			args[cur]=arg;

			//PERR("setmsg %d",cur);
			cur=(cur++)% 100;
			unlockcur();	
				
				
		}
		bool get_msg(int &op,int&arg){
			if(read<cur){
				//PERR("getmsg %d",read);
				op=opcodes[read];
				arg=args[read];
				read=(read++)% 100;
				return true;		
			}
			return false;
		}
		
	};
	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			MSG::Session_component *_create_session(const char *args)
			{
				PDBG("creating MSG session.");
				return new (md_alloc()) Session_component();
			}

		public:

			Root_component(Genode::Rpc_entrypoint *ep,
			               Genode::Allocator *allocator)
			: Genode::Root_component<Session_component>(ep, allocator)
			{			
				PDBG("Creating root component.");
			}
	};
}


using namespace Genode;
int main(int argc,char* argv[]) {
	
	Cap_connection cap;
	static Sliced_heap sliced_heap(env()->ram_session(),
	                               env()->rm_session());
	enum { STACK_SIZE = 4096 };
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "msg_ep");

	static MSG::Root_component lowlevel_root(&ep, &sliced_heap);
	env()->parent()->announce(ep.manage(&lowlevel_root));
	sleep_forever();
}




