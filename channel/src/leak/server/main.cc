#include <base/env.h>
#include <base/printf.h>
#include <calc_session/client.h>
#include <calc_session/connection.h>
#include <base/native_types.h>
#include <cap_session/connection.h>
#include <timer_session/client.h>
#include <timer_session/connection.h>
#include <leak_session/leak_session.h>
#include <base/rpc_server.h>
#include <root/component.h>
#include <base/sleep.h>
namespace Leak {

	struct Session_component : Genode::Rpc_object<Session>
	{
		
		int getCapID() {
			Calc::Connection h;
			PLOG("capability id:%d,runing 1+2=%d",h.cap().idx()->id(),h.add(1,2));
			return h.cap().idx()->id(); 
		}
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			Session_component *_create_session(const char *args)
			{
				//PDBG("creating dos session.");
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
int main(){
	Cap_connection cap;

	/*
	 * A sliced heap is used for allocating session objects - thereby we
	 * can release objects separately.
	 */
	static Sliced_heap sliced_heap(env()->ram_session(),
	                               env()->rm_session());

	/*
	 * Create objects for use by the framework.
	 *
	 * An 'Rpc_entrypoint' is created to announce our service's root
	 * capability to our parent, manage incoming session creation
	 * requests, and dispatch the session interface. The incoming RPC
	 * requests are dispatched via a dedicated thread. The 'STACK_SIZE'
	 * argument defines the size of the thread's stack. The additional
	 * string argument is the name of the entry point, used for
	 * debugging purposes only.
	 */
	enum { STACK_SIZE = 4096 };
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "leak_ep");

	static Leak::Root_component leak_root(&ep, &sliced_heap);
	env()->parent()->announce(ep.manage(&leak_root));

	/* We are done with this and only act upon client requests now. */
	sleep_forever();

	return 0;
	
}
