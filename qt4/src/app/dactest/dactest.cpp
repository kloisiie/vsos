/*
 * \brief   funtest
 * \author  OS5
 * \date    2013-09-22
 */

/* Qt includes */
/* Qoost includes */

#include "dactest.h"
#include "ui_dacdialog.h"

#include <base/printf.h>
#include <monitor_session/client.h>
#include <monitor_session/connection.h>
#include <base/rpc_args.h>
#include <monitor_session/auditinfo.h>

#include <base/env.h>
#include <timer_session/connection.h>
// begin test
#include <rom_session/connection.h>
#include <io_mem_session/connection.h>
#include <io_port_session/connection.h>
#include <ram_session/connection.h>
#include <log_session/connection.h>
#include <signal_session/connection.h>
#include <pd_session/connection.h>
#include <rm_session/connection.h>
#include <rm_session/rm_session.h>
#include <cap_session/connection.h>
#include <cpu_session/connection.h>
#include <irq_session/connection.h>
//end of test



Dialog::~Dialog()
{
    delete ui;
}


TitleBar::TitleBar(QWidget *parent)
{
	// Don't let this widget inherit the parent's backround color
	setAutoFillBackground(true);
	// Use a brush with a Highlight color role to render the background 
	setBackgroundRole(QPalette::Highlight);

	minimize = new QToolButton(this);
	maximize = new QToolButton(this);
	close= new QToolButton(this);

	// Use the style to set the button pixmaps
	QPixmap pix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
	close->setIcon(pix);

	maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
	maximize->setIcon(maxPix);

	pix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
	minimize->setIcon(pix);

	restorePix = style()->standardPixmap(QStyle::SP_TitleBarNormalButton);

	minimize->setMinimumHeight(20);
	close->setMinimumHeight(20);
	maximize->setMinimumHeight(20);

	maximize->setEnabled(false);
	minimize->setEnabled(false);


	QLabel *labelMonitor = new QLabel(this);
	labelMonitor->setText("自主访问管理程序");
	parent->setWindowTitle("自主访问管理程序");

	QLabel *iconlabel = new QLabel(this);
	QPixmap apppix(":/appicon/dactest.png");
	iconlabel->setPixmap(apppix);
	iconlabel->setFixedWidth(40);

	QLabel *label2 = new QLabel(this);
	label2->setText("\t\t\t\t\t\t");

	QHBoxLayout *hbox = new QHBoxLayout(this);

	hbox->addWidget(iconlabel);
	hbox->addWidget(labelMonitor);
	hbox->addWidget(label2);
	hbox->addWidget(minimize);
	hbox->addWidget(maximize);
	hbox->addWidget(close);

	hbox->insertStretch(1, 500);
	hbox->setSpacing(0);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	maxNormal = false;
	QObject::connect(close,SIGNAL(clicked()),parent,SLOT(crossClicked()));

	//connect(close, SIGNAL( clicked() ), parent, SLOT(quit() ) );
	connect(minimize, SIGNAL( clicked() ), this, SLOT(showSmall() ) );
	connect(maximize, SIGNAL( clicked() ), this, SLOT(showMaxRestore() ) );
}


void TitleBar::showSmall()
{
	parentWidget()->showMinimized();
}

void TitleBar::showMaxRestore()
{
	if (maxNormal) {
	    parentWidget()->showNormal();
	    maxNormal = !maxNormal;
	    maximize->setIcon(maxPix);
	} else {
	    parentWidget()->showMaximized();
	    maxNormal = !maxNormal;
	    maximize->setIcon(restorePix);
	}
}

void TitleBar::mousePressEvent(QMouseEvent *me)
{
	startPos = me->globalPos();
	clickPos = mapToParent(me->pos());
}
void TitleBar::mouseMoveEvent(QMouseEvent *me)
{
	if (maxNormal)
	    return;
	parentWidget()->move(me->globalPos() - clickPos);
}




Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
	ui->setupUi(this);
	m_titleBar = new TitleBar(this);

	ui->verticalLayout_2->removeItem(ui->horizontalLayout);
	ui->verticalLayout_2->removeWidget(ui->tableWidget);
	ui->verticalLayout_2->removeWidget(ui->textBrowser);
	ui->verticalLayout_2->addWidget(m_titleBar);
	ui->verticalLayout_2->addItem(ui->horizontalLayout);
	ui->verticalLayout_2->addWidget(ui->textBrowser);
	ui->verticalLayout_2->addWidget(ui->tableWidget);
	setLayout(ui->verticalLayout_2);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	username_list<<"admin"<<"audit"<<"securer"<<"user";	taskname_list<<"lowlevel_app"<<"midlevel_app"<<"highlevel_app"<<"lowlevel_process"<<"midlevel_process"<<"highlevel_process"<<"log"<<"funtest"<<"monitortest"<<"dos_server"<<"dos_client"<<"dos_attacker"<<"leak_access"<<"overflow"<<"tocttou_mkfiles"<<"test-lwip_httpsrv"<<"l4linux"<<"reuse"<<"channel_server"<<"channel_client"<<"channel_interfere"<<"dactest";
	servicename_list<<"Confidential";
	task_and_service_namelist<<taskname_list<<servicename_list;

	ui->comboBox->addItems(username_list);
	ui->comboBox_2->addItems(taskname_list);
	ui->comboBox_3->addItem(QString("null"));
	

	Genode::Monitor_Connection h;
	Genode::Monitor_Session::Strategy_out_string blprange;
	Genode::Monitor_Session::Strategy_out_string bibarange;
	Genode::Monitor_Session::Strategy_out_string ownername;
	int m_blp,m_biba;
	QObject::connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(grant()));
	QObject::connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(take()));
	QObject::connect(ui->comboBox_2,SIGNAL(currentIndexChanged(int)),this,SLOT(taskchanged()));

	
	QStringList admin_own_taskandservice;
	QStringList audit_own_taskandservice;
	QStringList sercurer_own_taskandservice;
	QStringList user_own_taskandservice;
	QList<QStringList> alluser_own_taskandservice;
	alluser_own_taskandservice<<admin_own_taskandservice<<audit_own_taskandservice<<sercurer_own_taskandservice<<user_own_taskandservice;
	QList<QStringList> alluser_trulyown_taskandservice;
	
/*	
	for(int i=0;i<task_and_service_namelist.size();i++)
	{
		int get_blp,get_biba;
		QByteArray temp = task_and_service_namelist[i].toLatin1();    
		const char * taskname=temp.data();	
		h.GetTaskStrategyByname(taskname,ownername,get_blp,get_biba,blprange,bibarange);
		for(int j=0;j<username_list.size();j++)
		{
			if(QString(ownername.getball())==username_list[j])
				alluser_trulyown_taskandservice[j]<<task_and_service_namelist[i];	
		}	
	}
*/

	for(int i=0;i<username_list.size();i++)
	{
		QByteArray temp = username_list[i].toLatin1();    
		const char * username=temp.data();
		int m_tasknum = h.Get_user_cap_num(true,username);	
		int m_servicenum = h.Get_user_cap_num(false,username);
		//int m_tasknum = h.Get_user_cap_num(true,"admin");	
		//int m_servicenum = h.Get_user_cap_num(false,"admin");
		for(int m_i=0;m_i<m_tasknum;m_i++)
		{
			m_blp = 55;
			m_biba = m_i;
			h.GetTaskStrategyByname(username,ownername,m_blp,m_biba,blprange,bibarange);
			alluser_own_taskandservice[i]<<ownername.getball();
		}
		for(int m_j=0;m_j<m_servicenum;m_j++)
		{
			m_blp = 56;
			m_biba = m_j;
			h.GetTaskStrategyByname(username,ownername,m_blp,m_biba,blprange,bibarange);
			alluser_own_taskandservice[i]<<ownername.getball();
		}
	}
	for(int i=0;i<username_list.size();i++)
	{
		ui->textBrowser->append(QString("username:%1").arg(username_list[i]));
		ui->textBrowser->append("task and service list:");
		ui->textBrowser->append(alluser_own_taskandservice[i].join(","));
	}
/*
	for(int i=0;i<username_list.size();i++)
	{
		ui->textBrowser->append(QString("username:%1").arg(username_list[i]));
		ui->textBrowser->append("truly task and service list:");
		ui->textBrowser->append(alluser_own_taskandservice[i].join(","));
	}
	
*/	
	int RowCount=username_list.size();
	int ColumnCount=task_and_service_namelist.size();
	ui->tableWidget->setRowCount(RowCount);
    	ui->tableWidget->setColumnCount(ColumnCount);
	ui->tableWidget->setVerticalHeaderLabels(username_list);
	ui->tableWidget->setHorizontalHeaderLabels(task_and_service_namelist); 
	for(int i=0;i<RowCount;i++)
	{
		for(int j=0;j<alluser_own_taskandservice[i].size();j++)
		{
			for(int k=0;k<ColumnCount;k++)
			{
//				ui->textBrowser->append(QString("has=%1,task=%2").arg(alluser_own_taskandservice[i][j]).arg(task_and_service_namelist[k]));
				if(alluser_own_taskandservice[i][j]==task_and_service_namelist[k])
					ui->tableWidget->setItem(i,k,new QTableWidgetItem("*"));
			}
		}
	}
}

void Dialog::crossClicked(){
	PERR("close");
	exit(0);
}


void Dialog::taskchanged()
{
	if(ui->comboBox_2->currentText()!=QString("midlevel_process"))
	{
		ui->comboBox_3->clear();
		ui->comboBox_3->addItem("null");
	}
	else
	{
		ui->comboBox_3->clear();
		ui->comboBox_3->addItem("null");
		ui->comboBox_3->addItems(servicename_list);
	}
}

void Dialog::grant()
{	
	Genode::Monitor_Connection h;
	Genode::Monitor_Session::Strategy_out_string blprange;
	Genode::Monitor_Session::Strategy_out_string bibarange;
	Genode::Monitor_Session::Strategy_out_string ownername;
	int m_blp,m_biba;

//	QStringList tempuser_owned_taskandservice;	
	
	
	bool grant_result=false;
	int userindex=ui->comboBox->currentIndex();
	int taskindex=ui->comboBox_2->currentIndex();
	int serviceindex=ui->comboBox_3->currentIndex();
	const char * username;
	if(serviceindex==0)
	{
		QByteArray temp = username_list[userindex].toLatin1();    
		username=temp.data();
		temp = taskname_list[taskindex].toLatin1();    
		const char * taskname=temp.data();
		grant_result = h.Grant_cap(true,username,taskname,"");
	}
	else
	{
		QByteArray temp = username_list[userindex].toLatin1();    
		username=temp.data();
		temp = servicename_list[serviceindex-1].toLatin1();    
		const char * servicename=temp.data();
		grant_result = h.Grant_cap(false,username,"midlevel_process",servicename);//need to modify if other task has service
	}
	if(grant_result&&serviceindex==0)
		ui->textBrowser->append(QString("succeed to grant task:%1 to user:%2").arg(ui->comboBox_2->currentText()).arg(ui->comboBox->currentText()));
	else if(grant_result&&serviceindex!=0)
		ui->textBrowser->append(QString("succeed to grant service:%1 to user:%2").arg(ui->comboBox_3->currentText()).arg(ui->comboBox->currentText()));
	
	else
		ui->textBrowser->append("failed to grant");
	if(grant_result)
	{

		if(serviceindex==0)
		{
			ui->tableWidget->setItem(userindex,taskindex,new QTableWidgetItem("*"));
		}
		else
		{
			ui->tableWidget->setItem(userindex,taskname_list.size()+serviceindex-1,new QTableWidgetItem("*"));
		}
	}
	
}

void Dialog::take()
{	
	Genode::Monitor_Connection h;
	Genode::Monitor_Session::Strategy_out_string blprange;
	Genode::Monitor_Session::Strategy_out_string bibarange;
	Genode::Monitor_Session::Strategy_out_string ownername;
	int m_blp,m_biba;

//	QStringList tempuser_owned_taskandservice;	
	
	
	bool take_result=false;
	int userindex=ui->comboBox->currentIndex();
	int taskindex=ui->comboBox_2->currentIndex();
	int serviceindex=ui->comboBox_3->currentIndex();
	const char * username;
	if(serviceindex==0)
	{
		QByteArray temp = username_list[userindex].toLatin1();    
		username=temp.data();
		temp = taskname_list[taskindex].toLatin1();    
		const char * taskname=temp.data();
		take_result = h.Del_cap(true,username,taskname,"");
	}
	else
	{
		QByteArray temp = username_list[userindex].toLatin1();    
		username=temp.data();
		temp = servicename_list[serviceindex-1].toLatin1();    
		const char * servicename=temp.data();
		take_result = h.Del_cap(false,username,"midlevel_process",servicename);//need to modify if other task has service
	}
	if(take_result&&serviceindex==0)
		ui->textBrowser->append(QString("succeed to take task:%1 to user:%2").arg(ui->comboBox_2->currentText()).arg(ui->comboBox->currentText()));
	else if(take_result&&serviceindex!=0)
		ui->textBrowser->append(QString("succeed to take service:%1 to user:%2").arg(ui->comboBox_3->currentText()).arg(ui->comboBox->currentText()));
	
	else
		ui->textBrowser->append("failed to take");
	if(take_result)
	{

		if(serviceindex==0)
		{
			ui->tableWidget->setItem(userindex,taskindex,new QTableWidgetItem(""));
		}
		else
		{
			ui->tableWidget->setItem(userindex,taskname_list.size()+serviceindex-1,new QTableWidgetItem(""));
		}
	}
	
}

    void Dialog::mousePressEvent(QMouseEvent *e)
    {
        m_old_pos = e->pos();
        m_mouse_down = e->button() == Qt::LeftButton;
    }
    
    void Dialog::mouseMoveEvent(QMouseEvent *e)
    {
        int x = e->x();
        int y = e->y();
        
        if (m_mouse_down) {
            int dx = x - m_old_pos.x();
            int dy = y - m_old_pos.y();
            
            QRect g = geometry();
            
            if (left)
                g.setLeft(g.left() + dx);
            if (right)
                g.setRight(g.right() + dx);
            if (bottom)
                g.setBottom(g.bottom() + dy);
            
            setGeometry(g);
            
            m_old_pos = QPoint(!left ? e->x() : m_old_pos.x(), e->y());
        } else {
            QRect r = rect();
            left = qAbs(x - r.left()) <= 5;
            right = qAbs(x - r.right()) <= 5;
            bottom = qAbs(y - r.bottom()) <= 5;
            bool hor = left | right;
            
            if (hor && bottom) {
                if (left)
                    setCursor(Qt::SizeBDiagCursor);
                else 
                    setCursor(Qt::SizeFDiagCursor);
            } else if (hor) {
                setCursor(Qt::SizeHorCursor);
            } else if (bottom) {
                setCursor(Qt::SizeVerCursor);
            } else {
                setCursor(Qt::ArrowCursor);
            }
        }
    }
    
    void Dialog::mouseReleaseEvent(QMouseEvent *e)
    {
        m_mouse_down = false;
    }


