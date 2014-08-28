/*
 * \brief   funtest
 * \author  OS5
 * \date    2013-09-22
 */

/* Qt includes */
/* Qoost includes */

#include "monitortest.h"
#include "ui_monitordialog.h"

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
	labelMonitor->setText("安全标记管理程序");
	parent->setWindowTitle("安全标记管理程序");

	QLabel *iconlabel = new QLabel(this);
	QPixmap apppix(":/appicon/monitortest.png");
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
	Genode::Monitor_Connection h;
	usernamelist<<"admin"<<"audit"<<"securer"<<"user";
	tasknamelist<<"lowlevel_app"<<"midlevel_app"<<"highlevel_app"<<"lowlevel_process"<<"midlevel_process"<<"highlevel_process"<<"funtest"<<"monitortest"<<"dos_server"<<"dos_client"<<"dos_attacker"<<"leak_access"<<"overflow"<<"tocttou_mkfiles"<<"test-lwip_httpsrv"<<"l4linux";
	char taskblp[50]={12,24,14,99,47,100,76};                   
	char taskbiba[50]={12,16,14};
	Genode::Monitor_Session::Strategy_out_string ownername;

    	ui->setupUi(this);
	ui->comboBox->addItems(usernamelist);
	ui->comboBox_4->addItems(tasknamelist);
	setMouseTracking(true);
	m_titleBar = new TitleBar(this);
	ui->verticalLayout_4->removeWidget(ui->tabWidget);
	ui->verticalLayout_4->addWidget(m_titleBar);
	ui->verticalLayout_4->addWidget(ui->tabWidget);
	setLayout(ui->verticalLayout_4);
	QObject::connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(UserInterfaceClicked()));
	QObject::connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(TaskInterfaceClicked()));

	QObject::connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(user_showrange()));
	QObject::connect(ui->comboBox_4,SIGNAL(currentIndexChanged(int)),this,SLOT(task_showrange()));
	setFixedSize(643,430);

	for(int i=0;i<usernamelist.size();i++)
	{
		Genode::Monitor_Session::Strategy_out_string blprange;
		Genode::Monitor_Session::Strategy_out_string bibarange;
		bool set_result=false,get_result=false;
		int get_blp,get_biba;
		QString strblprange,strbibarange;
		QByteArray temp = usernamelist[i].toLatin1();    
		const char * username=temp.data();
		get_result=h.GetUserStrategyByname(username,get_blp,get_biba,blprange,bibarange);
		QString aitem;
		for(int i=0;i<blprange.size()-1;i++)
		{
			strblprange.append(QString::number(blprange.getball()[i]));
			strblprange.append(",");
		}
		for(int i=0;i<bibarange.size()-1;i++)
		{
			strbibarange.append(QString::number(bibarange.getball()[i]));
			strbibarange.append(",");
		}

		aitem.append(QString("username:%1").arg(username));
		if(usernamelist[i].size()<3)
			aitem.append("\t\t\t");
		else if(usernamelist[i].size()<16)
        		aitem.append("\t\t");
		else
			aitem.append("\t");

		aitem.append(QString("blp:%1-%2\t\t\tbiba:%3-%4").arg(QString::number((int)(get_blp/10))).arg(QString::number(get_blp%10)).arg(QString::number((int)(get_biba/10))).arg(QString::number(get_biba%10)));
		aitem.append(QString("\n\t\t\tblprange:%1\tbibarange:%2").arg(strblprange).arg(strbibarange));

		if(get_result)
			ui->textBrowser->append(aitem);
		ui->textBrowser->append("----------------------------------------------------------------------------------------------------");
	}	
	for(int i=0;i<tasknamelist.size();i++)
	{
		Genode::Monitor_Session::Strategy_out_string blprange;
		Genode::Monitor_Session::Strategy_out_string bibarange;
		Genode::Monitor_Session::Strategy_out_string ownername;
		bool set_result=false,get_result=false;
		int get_blp,get_biba;
		QString strblprange,strbibarange;
		QByteArray temp = tasknamelist[i].toLatin1();    
		const char * taskname=temp.data();
		get_result=h.GetTaskStrategyByname(taskname,ownername,get_blp,get_biba,blprange,bibarange);
		QString aitem;

		for(int i=0;i<blprange.size()-1;i++)
		{
			strblprange.append(QString::number(blprange.getball()[i]));
			strblprange.append(",");
		}
		for(int i=0;i<bibarange.size()-1;i++)
		{
			strbibarange.append(QString::number(bibarange.getball()[i]));
			strbibarange.append(",");
		}
	
		aitem.append(QString("taskname:%1").arg(taskname));
		if(tasknamelist[i].size()<3)
			aitem.append("\t\t\t");
		else if(tasknamelist[i].size()<16)
        		aitem.append("\t\t");
		else
			aitem.append("\t");

		aitem.append(QString("blp:%1\t\t\tbiba:%2").arg(QString::number(get_blp)).arg(QString::number(get_biba)));
		aitem.append(QString("\n\t\t\tblprange:%1\tbibarange:%2").arg(strblprange).arg(strbibarange));

		if(get_result)
			ui->textBrowser_2->append(aitem);
		ui->textBrowser_2->append("----------------------------------------------------------------------------------------------------");

	}

}

void Dialog::crossClicked(){
	PERR("close");
	exit(0);
}


void Dialog::UserInterfaceClicked()
{
	Genode::Monitor_Connection h;
	int get_blp,get_biba;
	bool set_result,get_result;
	char userblp[50]={0};                   
	char userbiba[50]={0};
	ui->textBrowser->clear();
	QString qrange=ui->lineEdit->text();
	QStringList qlrange=qrange.split(",");
	for(int i=0;i<qlrange.size();i++)
	{
		userblp[i]=qlrange[i].toInt();
	}
	QString qrange2=ui->lineEdit_2->text();
	QStringList qlrange2=qrange2.split(",");
	for(int i=0;i<qlrange2.size();i++)
	{
		userbiba[i]=qlrange2[i].toInt();
	}

	QString qinputusername= ui->comboBox->currentText();
	QByteArray temp = qinputusername.toLatin1();    
	const char * inputusername=temp.data();
	int blplevel_l = (ui->comboBox_7->currentText()).toInt();
	int blplevel_h = (ui->comboBox_3->currentText()).toInt();
	int bibalevel_l = (ui->comboBox_8->currentText()).toInt();
	int bibalevel_h = (ui->comboBox_2->currentText()).toInt();
	int blplevel=blplevel_l+blplevel_h*10;
	int bibalevel=bibalevel_l+bibalevel_h*10;
	int type=ui->comboBox_9->currentIndex();
	int isMember= usernamelist.indexOf(qinputusername);
	QString showblprange,showbibarange;


	if(isMember==-1)
	{
		
		
		set_result = h.SetUserStrategyByname(inputusername,true,type,blplevel,bibalevel,userblp,userbiba);
		if(set_result)
		{
			QString qAudit_info = "SetUser: ";
			qAudit_info.append(qinputusername);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,1,0);
			usernamelist<<qinputusername;
			ui->comboBox->addItem(qinputusername);
			ui->textBrowser->append(QString("SetUser:%1 succeeded").arg(inputusername));
		}
		else
		{
			QString qAudit_info = "SetUser: ";
			qAudit_info.append(qinputusername);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,0,0);
			ui->textBrowser->append(QString("SetUser:%1 failed").arg(inputusername));
		}
	}
	else
	{
		
		set_result = h.SetUserStrategyByname(inputusername,false,type,blplevel,bibalevel,userblp,userbiba);
		if(set_result)
		{
			QString qAudit_info = "SetUser: ";
			qAudit_info.append(qinputusername);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,1,0);
			ui->textBrowser->append(QString("SetUser:%1 succeeded").arg(inputusername));
		}
		else
		{
			QString qAudit_info = "SetUser: ";
			qAudit_info.append(qinputusername);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,0,0);
			ui->textBrowser->append(QString("SetUser:%1 failed").arg(inputusername));
		}
		
	}
	
	ui->textBrowser->append("****************************************************************************************************");
//	ui->textBrowser->append(QString("sizeofnamelist=%1").arg(QString::number(usernamelist.size())));
	for(int i=0;i<usernamelist.size();i++)
	{
		Genode::Monitor_Session::Strategy_out_string blprange;
		Genode::Monitor_Session::Strategy_out_string bibarange;
		bool set_result=false,get_result=false;
		int get_blp,get_biba;
		QString strblprange,strbibarange;
		QByteArray temp = usernamelist[i].toLatin1();    
		const char * username=temp.data();
		get_result=h.GetUserStrategyByname(username,get_blp,get_biba,blprange,bibarange);
		QString aitem;
		for(int i=0;i<blprange.size()-1;i++)
		{
			strblprange.append(QString::number(blprange.getball()[i]));
			strblprange.append(",");
		}
		for(int i=0;i<bibarange.size()-1;i++)
		{
			strbibarange.append(QString::number(bibarange.getball()[i]));
			strbibarange.append(",");
		}

		aitem.append(QString("username:%1").arg(username));
		if(usernamelist[i].size()<3)
			aitem.append("\t\t\t");
		else if(usernamelist[i].size()<16)
        		aitem.append("\t\t");
		else
			aitem.append("\t");

		aitem.append(QString("blp:%1-%2\t\t\tbiba:%3-%4").arg(QString::number((int)(get_blp/10))).arg(QString::number(get_blp%10)).arg(QString::number((int)(get_biba/10))).arg(QString::number(get_biba%10)));
		aitem.append(QString("\n\t\t\tblprange:%1\tbibarange:%2").arg(strblprange).arg(strbibarange));

		if(get_result)
			ui->textBrowser->append(aitem);
		ui->textBrowser->append("----------------------------------------------------------------------------------------------------");
	}
}

void Dialog::TaskInterfaceClicked()
{
	Genode::Monitor_Connection h;
	int get_blp,get_biba;
	bool set_result,get_result;

	char taskblp[50]={0};                   
	char taskbiba[50]={0};
	ui->textBrowser_2->clear();
	QString qrange=ui->lineEdit_3->text();
	QStringList qlrange=qrange.split(",");
	for(int i=0;i<qlrange.size();i++)
	{
		taskblp[i]=qlrange[i].toInt();
	}
	QString qrange2=ui->lineEdit_4->text();
	QStringList qlrange2=qrange2.split(",");
	for(int i=0;i<qlrange2.size();i++)
	{
		taskbiba[i]=qlrange2[i].toInt();
	}
	QString qinputtaskname= ui->comboBox_4->currentText();
	QByteArray temp = qinputtaskname.toLatin1();    
	const char * inputtaskname=temp.data();
	int blplevel = (ui->comboBox_5->currentText()).toInt();
	int bibalevel = (ui->comboBox_6->currentText()).toInt();
	int type=ui->comboBox_10->currentIndex();
	int isMember= tasknamelist.indexOf(qinputtaskname);
	if(isMember==-1)
	{
		set_result = h.SetTaskStrategyByname(inputtaskname,true,type,blplevel,bibalevel,taskblp,taskbiba);
		if(set_result)
		{
			QString qAudit_info = "Settask: ";
			qAudit_info.append(qinputtaskname);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,1,0);
			tasknamelist<<qinputtaskname;
			ui->comboBox_4->addItem(qinputtaskname);
			ui->textBrowser_2->append(QString("Settask:%1 succeeded").arg(inputtaskname));
		}
		else
		{
			QString qAudit_info = "Settask: ";
			qAudit_info.append(qinputtaskname);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,0,0);
			ui->textBrowser_2->append(QString("Settask:%1 failed").arg(inputtaskname));
		}
	}
	else
	{
		set_result = h.SetTaskStrategyByname(inputtaskname,false,type,blplevel,bibalevel,taskblp,taskbiba);
		if(set_result)
		{
			QString qAudit_info = "Settask: ";
			qAudit_info.append(qinputtaskname);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,1,0);
			ui->textBrowser_2->append(QString("Settask:%1 succeeded").arg(inputtaskname));
		}
		else
		{
			QString qAudit_info = "Settask: ";
			qAudit_info.append(qinputtaskname);
			QByteArray temp = qAudit_info.toLatin1();    
			const char * audit_info=temp.data();
			h.AddAuditInfo("",audit_info,0,0);
			ui->textBrowser_2->append(QString("Settask:%1 failed").arg(inputtaskname));
		}
	}
	
	ui->textBrowser_2->append("****************************************************************************************************");
//	ui->textBrowser_2->append(QString("sizeoftasklist=%1").arg(QString::number(tasknamelist.size())));
	for(int i=0;i<tasknamelist.size();i++)
	{
		Genode::Monitor_Session::Strategy_out_string blprange;
		Genode::Monitor_Session::Strategy_out_string bibarange;
		Genode::Monitor_Session::Strategy_out_string ownername;
		int get_blp,get_biba;
		QString strblprange,strbibarange;
		QByteArray temp = tasknamelist[i].toLatin1();    
		const char * taskname=temp.data();
		get_result=h.GetTaskStrategyByname(taskname,ownername,get_blp,get_biba,blprange,bibarange);
		QString aitem;

		for(int i=0;i<blprange.size()-1;i++)
		{
			strblprange.append(QString::number(blprange.getball()[i]));
			strblprange.append(",");
		}
		for(int i=0;i<bibarange.size()-1;i++)
		{
			strbibarange.append(QString::number(bibarange.getball()[i]));
			strbibarange.append(",");
		}
	
		aitem.append(QString("taskname:%1").arg(taskname));
		if(tasknamelist[i].size()<3)
			aitem.append("\t\t\t");
		else if(tasknamelist[i].size()<16)
        		aitem.append("\t\t");
		else
			aitem.append("\t");

		aitem.append(QString("blp:%1\t\t\tbiba:%2").arg(QString::number(get_blp)).arg(QString::number(get_biba)));
		aitem.append(QString("\n\t\t\tblprange:%1\tbibarange:%2").arg(strblprange).arg(strbibarange));

		if(get_result)
			ui->textBrowser_2->append(aitem);
		ui->textBrowser_2->append("----------------------------------------------------------------------------------------------------");
	}
	
}

void Dialog::user_showrange()
{
	QString showblprange,showbibarange;
	int get_blp,get_biba;
	Genode::Monitor_Connection h;
	QString qinputusername= ui->comboBox->currentText();
	QByteArray temp = qinputusername.toLatin1();    
	const char * inputusername=temp.data();
	QString quserblprange,quserbibarange;
	Genode::Monitor_Session::Strategy_out_string blprange;
	Genode::Monitor_Session::Strategy_out_string bibarange;
	int isMember= usernamelist.indexOf(qinputusername);
	if(isMember==-1)
	{
		ui->lineEdit->clear();
		ui->lineEdit_2->clear();
	}
	else
	{
		
		h.GetUserStrategyByname(inputusername,get_blp,get_biba,blprange,bibarange);
		for(int i=0;i<blprange.size()-1;i++)
		{
			showblprange.append(QString::number(blprange.getball()[i]));
			showblprange.append(",");
						
		}
		for(int i=0;i<bibarange.size()-1;i++)
		{
			showbibarange.append(QString::number(bibarange.getball()[i]));
			showbibarange.append(",");	
		}
		ui->lineEdit->setText(showblprange);
		ui->lineEdit_2->setText(showbibarange);
	}
}

void Dialog::task_showrange()
{
	QString showblprange,showbibarange;
	int get_blp,get_biba;
	Genode::Monitor_Connection h;
	QString qinputusername= ui->comboBox_4->currentText();
	QByteArray temp = qinputusername.toLatin1();    
	const char * inputusername=temp.data();
	QString quserblprange,quserbibarange;
	Genode::Monitor_Session::Strategy_out_string blprange;
	Genode::Monitor_Session::Strategy_out_string bibarange;
	Genode::Monitor_Session::Strategy_out_string ownername;
	int isMember= tasknamelist.indexOf(qinputusername);
	if(isMember==-1)
	{
		ui->lineEdit_3->clear();
		ui->lineEdit_4->clear();
	}
	else
	{
		
		h.GetTaskStrategyByname(inputusername,ownername,get_blp,get_biba,blprange,bibarange);
		for(int i=0;i<blprange.size()-1;i++)
		{
			showblprange.append(QString::number(blprange.getball()[i]));
			showblprange.append(",");
						
		}
		for(int i=0;i<bibarange.size()-1;i++)
		{
			showbibarange.append(QString::number(bibarange.getball()[i]));
			showbibarange.append(",");	
		}
		ui->lineEdit_3->setText(showblprange);
		ui->lineEdit_4->setText(showbibarange);
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

/*
QString currInterface="Rom_dataspace_capability dataspace()";

CommServDialog::CommServDialog(QWidget *parent)
:QDialog(parent)
{
	
        m_mouse_down = false;
        //setFrameShape(Panel);
        
        // Make this a borderless window which can't
        // be resized or moved via the window system
        setMouseTracking(true);
        this->setAttribute(Qt::WA_QuitOnClose,true);
        m_titleBar = new TitleBar(this);

	label=new QLabel("监控服务：");
	labelOutput=new QLabel("操作：");
	serviceCombo = new QComboBox(this);
	serviceCombo->resize(50, 20);
	serviceCombo->addItems(QStringList()<<"用户标记"<<"任务标记"<<"其他");
	
	operationCombo = new QComboBox(this);
	operationCombo->resize(450, 20);
	operationCombo->addItems(QStringList()<<"bool SetUserStrategyByname()：新增用户标记"<<"bool SetUserStrategyByname()：修改用户标记"<<"bool GetUserStrategyByname()：获取用户标记信息");
	
	resultOutput = new QTextEdit;
	resultOutput->setText("");
	resultOutput->setReadOnly(true);
	
    	testButton = new QPushButton(tr("测试"));
	
    	QObject::connect(testButton,SIGNAL(clicked()),this,SLOT(testButtonClicked()));
    	QObject::connect(serviceCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));
    	QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
    	
    	QHBoxLayout *topLayoutUp = new QHBoxLayout;
    	topLayoutUp->addWidget(label);
    	topLayoutUp->addWidget(serviceCombo);
    	topLayoutUp->addWidget(labelOutput);
    	topLayoutUp->addWidget(operationCombo);
    	topLayoutUp->addWidget(testButton);
      	topLayoutUp->setMargin(5);
       	topLayoutUp->setSpacing(3);

    	QHBoxLayout *topLayoutMiddle = new QHBoxLayout;
    	topLayoutMiddle->addWidget(resultOutput);
      	topLayoutMiddle->setMargin(5);
       	topLayoutMiddle->setSpacing(3);

	QVBoxLayout *mainLayout = new QVBoxLayout;
    	mainLayout->addWidget(m_titleBar);
    	mainLayout->addLayout(topLayoutUp);
    	mainLayout->addLayout(topLayoutMiddle);
        mainLayout->setMargin(1);
        mainLayout->setSpacing(0);
    	
    	setLayout(mainLayout);
    	setWindowTitle(tr("监控服务接口测试程序"));
	setFixedSize(580,300);
}

void CommServDialog::onChanged(int index){
	switch(index){
	case 0:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"bool SetUserStrategyByname()：新增用户标记"<<"bool SetUserStrategyByname()：修改用户标记"<<"bool GetUserStrategyByname()：获取用户标记信息");
		break;
	case 1:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"bool SetTaskStrategyByname()：新增任务标记"<<"bool SetTaskStrategyByname()：修改任务标记"<<"bool GetTaskStrategyByname()：获取任务标记信息"<<"bool SetTaskID()：设置任务ID");
		break;
	case 2:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"bool AddAuditInfo()：添加审计信息"<<"bool ChargeAppLevel()：比较当前用户和预启动task的blp等级"<<"Dataspace_capability GetAuditInfo()：获取审计列表的dataspace");
		break;
	
	default:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"bool SetUserStrategyByname()：新增用户标记"<<"bool SetUserStrategyByname()：修改用户标记"<<"bool GetUserStrategyByname()：获取用户标记信息");
	}
}

void CommServDialog::onChangedForTest(int index){
	switch(index){
	case 0:
		if(operationCombo->itemText(0)=="bool SetUserStrategyByname()：新增用户标记"){
			currInterface.clear();
			currInterface.append("bool SetUserStrategyByname()：新增用户标记");
		}
		else if(operationCombo->itemText(0)=="bool SetTaskStrategyByname()：新增任务标记"){
			currInterface.clear();
			currInterface.append("bool SetTaskStrategyByname()：新增任务标记");
		}
		else if(operationCombo->itemText(0)=="bool AddAuditInfo()：添加审计信息"){
			currInterface.clear();
			currInterface.append("unsigned char inb(unsigned short address)");
		}
		break;
	case 1:
		if(operationCombo->itemText(1)=="bool SetUserStrategyByname()：修改用户标记"){
			currInterface.clear();
			currInterface.append("bool SetUserStrategyByname()：修改用户标记");
		}
		else if(operationCombo->itemText(1)=="bool SetTaskStrategyByname()：修改任务标记"){
			currInterface.clear();
			currInterface.append("bool SetTaskStrategyByname()：修改任务标记");
		}
		else if(operationCombo->itemText(1)=="bool AddAuditInfo()：添加审计信息"){
			currInterface.clear();
			currInterface.append("bool AddAuditInfo()：添加审计信息");
		}
		break;
	case 2:
		if(operationCombo->itemText(2)=="bool GetUserStrategyByname()：获取用户标记信息"){
			currInterface.clear();
			currInterface.append("bool GetUserStrategyByname()：获取用户标记信息");
		}
		else if(operationCombo->itemText(2)=="bool GetTaskStrategyByname()：获取任务标记信息"){
			currInterface.clear();
			currInterface.append("bool GetTaskStrategyByname()：获取任务标记信息");
		}
		else if(operationCombo->itemText(2)=="Dataspace_capability GetAuditInfo()：获取审计列表的dataspace"){
			currInterface.clear();
			currInterface.append("Dataspace_capability GetAuditInfo()：获取审计列表的dataspace");
		}
		break;
	case 3:
		if(operationCombo->itemText(3)=="bool SetTaskID()：设置任务ID"){
			currInterface.clear();
			currInterface.append("bool SetTaskID()：设置任务ID");
		}
		break;
	
	default:
		currInterface.clear();
		currInterface.append("bool SetUserStrategyByname()：新增用户标记");
	}
}

void CommServDialog::testButtonClicked() {  
	try{
			//*******user interface test*******


			if(currInterface=="bool SetTaskStrategyByname()：新增用户标记"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="bool SetUserStrategyByname()：修改用户标记"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				//执行测试 begin				
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="bool GetUserStrategyByname()：获取用户标记信息"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				//执行测试 begin				
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******user interface test*******



			//*******task interface test*******
			else if(currInterface=="bool SetTaskStrategyByname()：新增任务标记"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="bool SetTaskStrategyByname()：修改任务标记"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="bool GetTaskStrategyByname()：获取任务标记信息"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="bool SetTaskID()：设置任务ID"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******task interface test*******

			//*******others*******
			else if(currInterface=="bool AddAuditInfo()：添加审计信息"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="bool ChargeAppLevel()：比较当前用户和预启动task的blp等级"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="Dataspace_capability GetAuditInfo()：获取审计列表的dataspace"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******others*******

			
	}catch(...)
	{
		resultOutput->append("接口测试失败！");
	}	
}

void CommServDialog::crossClicked(){
	PERR("close");
	exit(0);
}

    void CommServDialog::mousePressEvent(QMouseEvent *e)
    {
        m_old_pos = e->pos();
        m_mouse_down = e->button() == Qt::LeftButton;
    }
    
    void CommServDialog::mouseMoveEvent(QMouseEvent *e)
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
    
    void CommServDialog::mouseReleaseEvent(QMouseEvent *e)
    {
        m_mouse_down = false;
    }
*/

