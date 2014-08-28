#include <base/printf.h>
#include <base/env.h>
#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <calc_session/calc_session.h>
#include <base/rpc_server.h>
#include <util/string.h>
#include <unistd.h>
#include <stdlib.h>
using namespace Genode;
namespace Calc {
	class Message{
		public:
			Message(){
				isStart=false;
				cur=0;
				Genode::memset(a,0,100);
				isReady=false;			
			}
			~Message(){}
			void start(){
				//PWRN("start");
				if(isStart){
					a[cur]=3;
					changeMod();					
					PERR("receive the message is: %s",msg);
					//exit(0);
					isStart=false;
					isReady=false;
					cur=0;
					Genode::memset(a,0,100);				
				}
				isStart=true;		
			}
			void changeMod(){
				int i=0,j=0;				
				while(i*8<cur){
					msg[i]=0;
					for(j=7;j>=0;j--){
						msg[i]=msg[i]*2+a[i*8+j];
					}
					i++;				
				}
				msg[i]='\0';
							
			}
			void ready(){
				//PWRN("ready");
				if(isStart)
					isReady=true;			
			}
			void go_0(){
				//PWRN("0");
				//sleep(1);
				if(isStart&&isReady){
					a[cur++]=0;
					isReady=false;
					PINF("Receiving bit: 0");				
				}			
			}
			void go_1(){
				//PWRN("1");
				//sleep(1);
				if(isStart&&isReady){
					a[cur++]=1;
					isReady=false;
					PINF("Receiving bit: 1");	
				}			
			}
			
			
		private:
			int cur;
			bool isReady;
			bool isStart;
			int a[200];	
			char msg[25];
	}msg;

	struct Session_component : Genode::Rpc_object<Session>
	{
		int sub(int a, int b) {		
			msg.start();
			return a - b; 
		}
		int add(int a, int b) {
			msg.ready();
			return a + b; 
		}
		int mul(int a, int b) {
			msg.go_0();
			return a * b; 
		}
		int div(int a, int b) {
			msg.go_1();
			return a / b; 
		}
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			Calc::Session_component *_create_session(const char *args)
			{
				PDBG("creating calc session.");
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

int main(void)
{
	/*
	 * Get a session for the parent's capability service, so that we
	 * are able to create capabilities.
	 */
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
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "calc_ep");

	static Calc::Root_component calc_root(&ep, &sliced_heap);
	env()->parent()->announce(ep.manage(&calc_root));

	/* We are done with this and only act upon client requests now. */
	sleep_forever();

	return 0;
}
