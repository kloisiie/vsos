/*
 * \brief   Qt Launchpad main program
 * \author  Christian Prochaska
 * \date    2008-04-05
 */

/* local includes */
#include "qt_launchpad.h"

/* Qt includes */
#include <QtGui>
#include <QApplication>
#include <QTextCodec>
#include <QFont>

/* Genode includes */
#include <base/env.h>
#include <rom_session/connection.h>


#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <loginserver_session/loginserver_session.h>
#include <base/rpc_server.h>
#include <timer_session/connection.h>
#include <init/child_config.h>
#include <os/config.h>

Qt_launchpad *launchpad;
bool loginfinished = false;

namespace LOGINserver {

	struct Session_component : Genode::Rpc_object<Session>
	{
		void login_finished() {
			//PERR("login");
			if(loginfinished == false) loginfinished = true;
			//else loginfinished = false;
                        }

		int add(int a, int b) {
			return a + b; }
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			LOGINserver::Session_component *_create_session(const char *args)
			{
				//PDBG("creating LOGINserver session.");
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



int main(int argc, char *argv[])
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
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "loginserver_ep");

	static LOGINserver::Root_component loginserver_root(&ep, &sliced_heap);
	env()->parent()->announce(ep.manage(&loginserver_root));

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	/* We are done with this and only act upon client requests now. */
	Timer::Connection loginwait;	
	while(true){
		
		if(loginfinished) break;
		else { loginwait.msleep(1500);
               }
	}
        PERR("windows");
	/* look for dynamic linker */
	try {
		static Genode::Rom_connection rom("ld.lib.so");
		Genode::Process::dynamic_linker(rom.dataspace());
	} catch (...) { }

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

	int result;

	QApplication *a = new QApplication(argc, argv);

	launchpad = new Qt_launchpad(Genode::env()->ram_session()->quota());

	try {
		launchpad->process_config(launchpad);

	/* if there exists no configuration, use defaults */
	} catch (...) {
		
	}
	launchpad->setWindowFlags(Qt::FramelessWindowHint);
	launchpad->move(905,0);
	launchpad->setFocus();
	launchpad->show();

	a->connect(a, SIGNAL(lastWindowClosed()), a, SLOT(quit()));

	result = a->exec();
	launchpad->close();
	delete launchpad;
	delete a;

	return result;
}
