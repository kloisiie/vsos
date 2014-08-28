/*
 * \brief   verify test
 * \author  OS5
 * \date    2013-09-04
 */

/* Qt includes */
#include <QtGui>
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDialog>
#include <QObject>
#include <QTextCodec>
/* Qoost includes */
#include "verify.h"

/* Genode includes */
#include <base/env.h>
#include <rom_session/connection.h>


#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <sakserver_session/sakserver_session.h>
#include <trustoneserver_session/trustoneserver_session.h>
#include <base/rpc_server.h>
#include <timer_session/connection.h>
bool sakpressed = false;
bool loginfinished = false;
LoginDialog *dialog;

namespace SAKserver {

	struct Session_component : Genode::Rpc_object<Session>
	{
		void say_hello() {
			PERR("hello");
			if(sakpressed == false) sakpressed = true;
			//else sakpressed = false;
                        }

		int add(int a, int b) {
			return a + b; }
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			SAKserver::Session_component *_create_session(const char *args)
			{
				PDBG("creating SAKserver session.");
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

namespace TrustONEserver {

	struct Session_component : Genode::Rpc_object<Session>
	{
		void trust_one() {
			PERR("login");
			if(loginfinished == false) loginfinished = true;
			//else loginfinished = false;
                        }

		int add(int a, int b) {
			return a + b; }
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			TrustONEserver::Session_component *_create_session(const char *args)
			{
				PDBG("creating TrustONEserver session.");
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
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "sakserver_ep");

	static SAKserver::Root_component sakserver_root(&ep, &sliced_heap);
	env()->parent()->announce(ep.manage(&sakserver_root));


	Cap_connection cap2;
	static Sliced_heap sliced_heap2(env()->ram_session(),
	                               env()->rm_session());
	static Rpc_entrypoint ep2(&cap2, STACK_SIZE, "trustoneserver_ep");

	static TrustONEserver::Root_component trustoneserver_root2(&ep2, &sliced_heap2);
	env()->parent()->announce(ep2.manage(&trustoneserver_root2));




	/* We are done with this and only act upon client requests now. */
	Timer::Connection sakwait;
	while(true){ 
		if(sakpressed||loginfinished) break;
		else {
			sakwait.msleep(800);
		}	
	}
	Genode::Monitor_Connection h;
	h.AddAuditInfo("","sak pressed",1,0);
        PERR("windows");

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QApplication app(argc,argv);
	dialog = new LoginDialog();
	dialog->setWindowFlags(Qt::WindowTitleHint);
	dialog->setWindowIcon(QIcon(":/appicon/verify.png"));
	dialog->show();
	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	int result =app.exec();
	dialog->close();
	delete dialog;
	PERR("Not Closed!");
	return result;
}




