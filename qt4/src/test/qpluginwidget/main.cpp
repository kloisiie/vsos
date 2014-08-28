#include <QtGui>
#include <QApplication>
#include "logwindow.h"


int main(int argc, char *argv[])
{
	static QApplication app(argc, argv);
	
	logWindow* logwindow=new logWindow;
	Client log;
	QObject::connect(&log,SIGNAL(print_log(AuditInfo*)),logwindow,SLOT(appendText(AuditInfo*)),Qt::QueuedConnection);
	//QObject::connect(logwindow,SIGNAL(log_success()),&log,SLOT(continueLog()),Qt::DirectConnection);
	logwindow->move(0,QApplication::desktop()->height()-logwindow->height()-50);
	logwindow->show();
	log.start();


	//w.show();
	return app.exec();
}
