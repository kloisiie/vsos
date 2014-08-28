/*
 * \brief   monitortest
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
/* Qoost includes */
#include "monitortest.h"

/* Genode includes */
#include <base/env.h>
#include <rom_session/connection.h>
#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <base/rpc_server.h>
#include <timer_session/connection.h>
#include <base/rpc_server.h>

using namespace Genode;

/*int main(int argc,char* argv[]) {
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QApplication app(argc,argv);
	CommServDialog* dialog = new CommServDialog();
	dialog->setWindowFlags(Qt::FramelessWindowHint);
	dialog->setWindowIcon(QIcon(":/appicon/monitortest.png"));
	dialog->move((QApplication::desktop()->width()-dialog->width())/2-200,QApplication::desktop()->height()-350);
	dialog->show();
//unuse
	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));

	int result =app.exec();
	dialog->close();
	delete dialog;
	PERR("Not Closed!");
	return result;
}
*/

int main(int argc,char* argv[])
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
   	QApplication app(argc, argv);
   	Dialog* dialog=new Dialog();
	dialog->setWindowFlags(Qt::FramelessWindowHint);
	dialog->setWindowIcon(QIcon(":/appicon/monitortest.png"));
	dialog->move((QApplication::desktop()->width()-dialog->width())/2-200,QApplication::desktop()->height()-350);
    	dialog->show();
 	//app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	int result =app.exec();
	dialog->close();
	delete dialog;
	PERR("Not Closed!");
	return result;

	
}




