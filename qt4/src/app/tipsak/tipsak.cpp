/*
 * \brief   tipsak test
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
#include <QFont>
/* Qoost includes */
#include "tipsak.h"


#include <base/env.h>
#include <sakserver_session/client.h>
#include <sakserver_session/connection.h>
#include <trustoneserver_session/client.h>
#include <trustoneserver_session/connection.h>
#include <timer_session/connection.h>

int nowkey = -1;

LoginDialog::LoginDialog(QWidget *parent)
:QDialog(parent)
{	
	label_user = new QLabel(tr("要登入，请按顺序按下如下安全留意键（SAK）：\no + s + 5")); 
	label_user->setFont(QFont("OS5",20,QFont::Bold));
	label_user->setAlignment(Qt::AlignCenter);

    	QHBoxLayout *topLayoutUp = new QHBoxLayout;
    	topLayoutUp->addWidget(label_user);
	
    	QHBoxLayout *topLayoutDown = new QHBoxLayout;
    	topLayoutDown->addWidget(label_pass);
	
    	QHBoxLayout *bottomLayout = new QHBoxLayout;
    	bottomLayout->addStretch();
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
    	mainLayout->addLayout(topLayoutUp);
    	mainLayout->addLayout(topLayoutDown);
    	mainLayout->addLayout(bottomLayout);
    	setLayout(mainLayout);
    	setFixedHeight(sizeHint().height());
	/*
	QTimer *timer = new QTimer(this);
	timer->setSingleShot(true);
	connect(timer,SIGNAL(timeout()),this,SLOT(thishide()));
	timer->start(15000);
	*/
}
void LoginDialog::closeEvent( QCloseEvent *k ){
	this->hide();
}
void LoginDialog::keyPressEvent( QKeyEvent *k ) 
{ 
	PERR("key pressed");	
	if(k->key() == Qt::Key_O && nowkey == -1) 
	{
		nowkey = 31;
	}
	if(k->key() == Qt::Key_S && nowkey == 31) 
	{
		nowkey = 55;
	}
	if(k->key() == Qt::Key_5 && nowkey == 55) 
	{
		nowkey = -1;this->hide();
		Timer::Connection loginwait;
		loginwait.msleep(1000);
		TrustONEserver::Connection t;
		t.trust_one();
	}
}


void LoginDialog::thishide()
{
	this->hide();
}
