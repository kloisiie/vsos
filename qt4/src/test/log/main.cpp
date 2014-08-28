#include <QtGui>
#include <QApplication>
#include "logwindow.h"


int main(int argc, char *argv[])
{
	static QApplication app(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	logWindow* logwindow=new logWindow;
	Client log;
	QObject::connect(&log,SIGNAL(print_log(AuditInfo*)),logwindow,SLOT(appendText(AuditInfo*)),Qt::QueuedConnection);
	//QObject::connect(logwindow,SIGNAL(log_success()),&log,SLOT(continueLog()),Qt::DirectConnection);
	logwindow->move(50,QApplication::desktop()->height()-logwindow->height()-50);
	logwindow->show();
	logwindow->setWindowIcon(QIcon(":/appicon/log.png"));
	log.start();


	//w.show();
	return app.exec();
}
