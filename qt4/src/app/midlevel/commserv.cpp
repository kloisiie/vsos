/*
 * \brief   commserv
 * \author  OS5
 * \date    2013-09-22
 */

/* Qt includes */
/* Qoost includes */
#include "commserv.h"


#include <base/env.h>
#include <loginserver_session/client.h>
#include <loginserver_session/connection.h>
#include <timer_session/connection.h>

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

	QRegExp regx("[0-9]+$");
	label=new QLabel("输入：");
	labelOutput=new QLabel("输出：");
	lineEdit = new QLineEdit;
	lineEdit->setValidator(new QRegExpValidator(regx,this));
	lineOutput = new QLineEdit;
	lineOutput->setValidator(new QRegExpValidator(regx,this));
	lineOutput->setEnabled(false);
	
    	readButton = new QPushButton(tr("读"));
	//say_helloButton->setFocus();

	writeButton = new QPushButton(tr("写"));
    	writeButton->setEnabled(false);

	read_writeButton = new QPushButton(tr("读/写"));
    	read_writeButton->setEnabled(false);
	
	QObject::connect(lineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(enableWriteButton(const QString &)));
	QObject::connect(lineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(enableRead_writeButton(const QString &)));
    	QObject::connect(readButton,SIGNAL(clicked()),this,SLOT(readClicked()));
    	QObject::connect(writeButton,SIGNAL(clicked()),this,SLOT(writeClicked()));
    	QObject::connect(read_writeButton,SIGNAL(clicked()),this,SLOT(read_writeClicked()));

    	QHBoxLayout *topLayoutUp = new QHBoxLayout;
	topLayoutUp->addWidget(label);
    	topLayoutUp->addWidget(lineEdit);
      	topLayoutUp->setMargin(5);
       	topLayoutUp->setSpacing(3);

    	QHBoxLayout *topLayoutMiddle = new QHBoxLayout;
	topLayoutMiddle->addWidget(labelOutput);
    	topLayoutMiddle->addWidget(lineOutput);
      	topLayoutMiddle->setMargin(5);
       	topLayoutMiddle->setSpacing(3);

    	QHBoxLayout *topLayoutDown = new QHBoxLayout;
    	topLayoutDown->addWidget(readButton);	
	topLayoutDown->addWidget(writeButton);	
	topLayoutDown->addWidget(read_writeButton);
      	topLayoutDown->setMargin(5);
       	topLayoutDown->setSpacing(3);

	QVBoxLayout *mainLayout = new QVBoxLayout;
    	mainLayout->addWidget(m_titleBar);
    	mainLayout->addLayout(topLayoutUp);
    	mainLayout->addLayout(topLayoutMiddle);
    	mainLayout->addLayout(topLayoutDown);
        mainLayout->setMargin(1);
        mainLayout->setSpacing(0);
    	
    	setLayout(mainLayout);
    	setWindowTitle(tr("中密级应用"));
	setFixedSize(300,125);
}

void CommServDialog::writeClicked() {
        try{
		QString id = QString(lineEdit->text().trimmed());
		bool ok;	
		int _id=id.toInt(&ok,10);
		h.write_state(_id);
		char buf[128];
		Genode::snprintf(buf,128,"成功设置服务进程状态为：%d",_id);
		//QMessageBox::information(this, tr("结果"), buf, QMessageBox::Ok);
		
	}catch(Genode::Permission_denied)
	{
		QMessageBox::information(this, tr("警报"), tr("权限不足！"), QMessageBox::Ok);
	}
	lineEdit->clear();
	read_writeButton->setEnabled(false);
	writeButton->setEnabled(false);	
}

void CommServDialog::readClicked() {  
	try{
		int id=h.read_state();
		char buf[50];
		//Genode::snprintf(buf,50,"成功读取服务进程状态：%d",id);
		Genode::snprintf(buf,15,"%d",id);
		lineOutput->setText(buf);
		//QMessageBox::information(this, tr("结果"), buf, QMessageBox::Ok);
	}catch(Genode::Permission_denied)
	{
		QMessageBox::information(this, tr("警报"), tr("权限不足！"), QMessageBox::Ok);
	}	
}

void CommServDialog::read_writeClicked() {
	try{
		QString num1 = lineEdit->text().trimmed();
		bool ok;	
		int _num1=num1.toInt(&ok,10);
		int result=h.change_state(_num1);
		char buf[128];
		char buf2[15];
		Genode::snprintf(buf2,15,"%d",_num1);
		Genode::snprintf(buf,128,"成功将服务进程状态更改为%d，前一状态为%d",_num1,result);
		lineOutput->setText(buf2);
		//QMessageBox::information(this, tr("结果"), buf, QMessageBox::Ok);
	}catch(Genode::Permission_denied)
	{
		QMessageBox::information(this, tr("警报"), tr("权限不足！"), QMessageBox::Ok);	
	}
	lineEdit->clear();
	read_writeButton->setEnabled(false);
	writeButton->setEnabled(false);
}



void CommServDialog::enableWriteButton(const QString &id) {
    writeButton->setEnabled(!id.isEmpty());
}

void CommServDialog::enableRead_writeButton(const QString &nameId) {
    read_writeButton->setEnabled(!nameId.isEmpty());
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
        
        
        QLabel *label = new QLabel(this);
        label->setText("中密级应用");
        parent->setWindowTitle("中密级应用");

        QLabel *iconlabel = new QLabel(this);
	QPixmap apppix(":/appicon/midlevel_app.png");
        iconlabel->setPixmap(apppix);
	iconlabel->setFixedWidth(40);

        QLabel *label2 = new QLabel(this);
        label2->setText("\t");

        QHBoxLayout *hbox = new QHBoxLayout(this);
        
        hbox->addWidget(iconlabel);
        hbox->addWidget(label);
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
