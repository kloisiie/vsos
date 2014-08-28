/*
 * \brief   version
 * \author  OS5
 * \date    2014-01-07
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
#include <QFont>
/* Qoost includes */
#include "version.h"


#include <base/env.h>
#include <sakserver_session/client.h>
#include <sakserver_session/connection.h>
#include <trustoneserver_session/client.h>
#include <trustoneserver_session/connection.h>
#include <timer_session/connection.h>



VersionDialog::VersionDialog(QWidget *parent)
:QDialog(parent)
{	
	label_version = new QLabel(tr("VSOS version: V1.0"));
	label_version->setFont(QFont("OS5",15,QFont::Bold));
	label_version->setAlignment(Qt::AlignCenter);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(label_version);
    	setLayout(mainLayout);
    	setFixedHeight(sizeHint().height());
	setGeometry(0,0,100,100);
	
}

