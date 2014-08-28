#include "logwindow.h"
#include <base/printf.h>
logWindow::logWindow(QWidget *parent){
	setWindowTitle(tr("log result"));
	userText->setReadOnly(true);
	clientText->setReadOnly(true);
	serverText->setReadOnly(true);	
	opText->setReadOnly(true);
	timeText->setReadOnly(true);
	resultText->setReadOnly(true);
	QVBoxLayout *topLayout1 = new QVBoxLayout;
	topLayout1->addWidget(user);
    	topLayout1->addWidget(userText);
	
	QVBoxLayout *topLayout2 = new QVBoxLayout;
	topLayout2->addWidget(client);
    	topLayout2->addWidget(clientText);
	
	QVBoxLayout *topLayout3 = new QVBoxLayout;
	topLayout3->addWidget(server);
    	topLayout3->addWidget(serverText);
	
	QVBoxLayout *topLayout4 = new QVBoxLayout;
	topLayout4->addWidget(op);
    	topLayout4->addWidget(opText);
	
	QVBoxLayout *topLayout5 = new QVBoxLayout;
	topLayout5->addWidget(time);
    	topLayout5->addWidget(timeText);
	
	QVBoxLayout *topLayout6 = new QVBoxLayout;
	topLayout6->addWidget(result);
    	topLayout6->addWidget(resultText);
	
	
	QGridLayout *mainLayout = new QGridLayout;
    	mainLayout->addLayout(topLayout1,0,0);
    	mainLayout->addLayout(topLayout2,0,1);
	mainLayout->addLayout(topLayout3,0,2);
	mainLayout->addLayout(topLayout4,0,3);
	mainLayout->addLayout(topLayout5,0,4);
	mainLayout->addLayout(topLayout6,0,5);
	mainLayout -> setColumnStretch(0, 1);
	mainLayout -> setColumnStretch(1, 3);
	mainLayout -> setColumnStretch(2, 3);
	mainLayout -> setColumnStretch(3, 1);
	mainLayout -> setColumnStretch(4, 1);
	mainLayout -> setColumnStretch(5, 1);
	mainLayout -> setRowStretch(0, 1);  
    	mainLayout -> setRowStretch(1, 3); 
	/*QGridLayout *topLayout = new QGridLayout;
	topLayout->addWidget(user,0,0,1,1);
    	topLayout->addWidget(userText,1,0,3,1);
	topLayout->addWidget(client,0,1,1,3);
    	topLayout->addWidget(clientText,1,1,3,3);
	topLayout->addWidget(server,0,4,1,3);
    	topLayout->addWidget(serverText,1,4,3,3);
	topLayout->addWidget(op,0,7,1,1);
    	topLayout->addWidget(opText,1,7,3,1);
	topLayout->addWidget(time,0,8,1,1);
    	topLayout->addWidget(timeText,1,8,3,1);
	topLayout->addWidget(result,0,9,1,1);
    	topLayout->addWidget(resultText,1,9,3,1);
	setLayout(topLayout);*/
	setLayout(mainLayout);
	setFixedSize(740,150);
}

void logWindow::appendText(AuditInfo *s){
	PERR("append Text");
	//logText->append(str);
	userText->append(s->m_UserName);
	userText->append("  ");
	clientText->append(s->m_ClientName);
	serverText->append(s->m_ServerName);
	opText->append(s->m_OperationName);
	timeText->append(s->m_time);
	timeText->append("  ");
	opText->append(get_opcode(s->m_OpCode));
	clientText->append(get_level(s->m_ClientBlpLevel));
	serverText->append(get_level(s->m_ServerBlpLevel));
	resultText->append(s->m_OpResult?"Allowed":"Denied");
	resultText->append(" ");
	//emit log_success();
	/*printlog(s[i].m_UserName,s[i].m_ClientName,s[i].m_ServerName);
				tolen15(s[i].m_OperationName);
				tolen10(s[i].m_time);
				tolen10(get_opcode(s[i].m_OpCode));
				tolen15(get_level(s[i].m_ClientBlpLevel));
				tolen15(get_level(s[i].m_ServerBlpLevel));
				tolen10(s[i].m_OpResult?"Allowed":"Denied");*/
	
}

