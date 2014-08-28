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
#include <QFont>
/* Qoost includes */
#include "verify.h"


/*#include <base/env.h>
#include <loginserver_session/client.h>
#include <loginserver_session/connection.h>
#include <timer_session/connection.h>
*/LoginDialog::LoginDialog(QWidget *parent)
:QDialog(parent)
{	
	label_user = new QLabel(tr("用户名(&U)：")); 
	label_pass = new QLabel(tr("  密码(&P)："));
	label_space = new QLabel(tr("    "));
	label_user->setFont(QFont("Times",10,QFont::Bold));
	label_pass->setFont(QFont("Times",10,QFont::Bold));
	lineEdit_user = new QLineEdit;
	lineEdit_pass = new QLineEdit;
	lineEdit_pass->setEchoMode(QLineEdit::Password);
	label_user->setBuddy(lineEdit_user);
	label_pass->setBuddy(lineEdit_pass);
	
    	loginButton = new QPushButton(tr("登入"));
	loginButton->setFocus();
    	loginButton->setDefault(true);
    	loginButton->setEnabled(false);
	
	cancelButton = new QPushButton(tr("取消"));
	
	QObject::connect(lineEdit_pass,SIGNAL(textChanged(const QString &)),this,SLOT(enableLoginButton(const QString &)));
    	QObject::connect(loginButton,SIGNAL(clicked()),this,SLOT(loginClicked()));
    	QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));
    	QHBoxLayout *topLayoutUp = new QHBoxLayout;
    	topLayoutUp->addWidget(label_user);
    	topLayoutUp->addWidget(lineEdit_user);
	
    	QHBoxLayout *topLayoutDown = new QHBoxLayout;
    	topLayoutDown->addWidget(label_pass);
    	topLayoutDown->addWidget(lineEdit_pass);
	
    	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(label_space);
	bottomLayout->addWidget(loginButton);
	bottomLayout->addWidget(cancelButton);
    	bottomLayout->addStretch();
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
    	mainLayout->addLayout(topLayoutUp);
    	mainLayout->addLayout(topLayoutDown);
    	mainLayout->addLayout(bottomLayout);
    	setLayout(mainLayout);
    	setWindowTitle(tr("五级系统登入"));
    	setFixedHeight(sizeHint().height());	
}

void LoginDialog::loginClicked() {
        if(lineEdit_user->text().trimmed() == tr("admin")
                    && lineEdit_pass->text().trimmed() == tr("helloos5"))
            {
                QMessageBox::information(this, tr("登入"), tr("欢迎来到五级操作系统！"), QMessageBox::Ok);
                this->close();
		
		
            }           
            else
            {
                QMessageBox::warning(this, tr("警告！"), tr("用户名或密码错误！"), QMessageBox::Yes);
                lineEdit_pass->setFocus();
            }
}
void LoginDialog::cancelClicked() {
    lineEdit_user->clear();
    lineEdit_pass->clear();
}
void LoginDialog::enableLoginButton(const QString &pass) {
    loginButton->setEnabled(!pass.isEmpty());
  }
