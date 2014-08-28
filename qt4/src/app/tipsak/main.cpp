/*
 * \brief   verify test
 * \author  OS5
 * \date    2013-09-04
 */

/* Qt includes */
#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDialog>
#include <QObject>
#include <QTextCodec>
/* Qoost includes */
#include "tipsak.h"

/* Genode includes */
#include <base/env.h>
#include <rom_session/connection.h>


#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <trustoneserver_session/trustoneserver_session.h>
#include <base/rpc_server.h>
#include <timer_session/connection.h>


using namespace Genode;



int main(int argc,char* argv[]) {
	
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QApplication app(argc,argv);
	LoginDialog *dialog = new LoginDialog();
	dialog->setWindowFlags(Qt::FramelessWindowHint);
	dialog->show();
	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	int result =app.exec();
	dialog->close();
	delete dialog;
	PERR("Not Closed!");
	return result;
}




