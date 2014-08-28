/*
 * \brief   commserv
 * \author  OS5
 * \date    2013-09-22
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
#include <QWidget>
/* Qoost includes */
#include "logwindow.h"
#include <back_session/connection.h>
int state=0;
namespace Confidential{

	MSG::Connection msg;
	struct Session_component : Genode::Rpc_object<Session>
	{
		
		int read_state() {
			msg.set_msg(1,state);			
			return state;		
		}
		void write_state(int id) {
			msg.set_msg(2,id);
		 	state=id;
		}
		int change_state(int a) {
			int tmp=state;
			state=a;
			msg.set_msg(3,a);
			return tmp; 
		}
	};
	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			Confidential::Session_component *_create_session(const char *args)
			{
				PDBG("creating midlevel session.");
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
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "midlevel_ep");

	static Confidential::Root_component lowlevel_root(&ep, &sliced_heap);
	env()->parent()->announce(ep.manage(&lowlevel_root));


	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QApplication app(argc,argv);
	Client log;
	logWindow* dialog = new logWindow();
	dialog->setWindowFlags(Qt::FramelessWindowHint);
	QObject::connect(&log,SIGNAL(print_log(const char *)),dialog,SLOT(appendText(const char *)),Qt::QueuedConnection);
	QObject::connect(dialog,SIGNAL(log_success()),&log,SLOT(continueLog()),Qt::DirectConnection);
	dialog->setWindowIcon(QIcon(":/appicon/midlevel_process.png"));

	dialog->move((QApplication::desktop()->width()-dialog->width())/2 + 20,150);
	dialog->show();
	log.start();
	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	int result =app.exec();
	dialog->close();
	delete dialog;
	PERR("Not Closed!");
	return result;
}




