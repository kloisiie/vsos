/*
 * \brief   commserv
 * \author  OS5
 * \date    2013-09-22
 */

/* Qt includes */
/* Qoost includes */
#include "control.h"



#include <base/printf.h>
#include <timer_session/connection.h>
#include <ram_session/connection.h>

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
	textOutput = new QTextEdit;
	textOutput->setEnabled(true);
	
    	readButton = new QPushButton(tr("检查内存是否清空"));
	//say_helloButton->setFocus();

	writeButton = new QPushButton(tr("填充内存"));
    	writeButton->setEnabled(false);

	
	QObject::connect(lineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(enableWriteButton(const QString &)));
    	QObject::connect(readButton,SIGNAL(clicked()),this,SLOT(readClicked()));
    	QObject::connect(writeButton,SIGNAL(clicked()),this,SLOT(writeClicked()));

    	QHBoxLayout *topLayoutUp = new QHBoxLayout;
	topLayoutUp->addWidget(label);
    	topLayoutUp->addWidget(lineEdit);
	topLayoutUp->addWidget(writeButton);	
      	topLayoutUp->setMargin(5);
       	topLayoutUp->setSpacing(3);

    	QHBoxLayout *topLayoutMiddle = new QHBoxLayout;
	topLayoutMiddle->addWidget(readButton);	
      	topLayoutMiddle->setMargin(5);
       	topLayoutMiddle->setSpacing(3);

    	QHBoxLayout *topLayoutDown = new QHBoxLayout;
	topLayoutDown->addWidget(labelOutput);
    	topLayoutDown->addWidget(textOutput);	
      	topLayoutDown->setMargin(5);
       	topLayoutDown->setSpacing(10);

	QVBoxLayout *mainLayout = new QVBoxLayout;
    	mainLayout->addWidget(m_titleBar);
    	mainLayout->addLayout(topLayoutUp);
    	mainLayout->addLayout(topLayoutMiddle);
    	mainLayout->addLayout(topLayoutDown);
        mainLayout->setMargin(1);
        mainLayout->setSpacing(0);
    	
    	setLayout(mainLayout);
    	setWindowTitle(tr("客体重用测试"));
	setFixedSize(300,400);
	ram=Genode::env()->ram_session();
	rm=Genode::env()->rm_session();
}

void CommServDialog::writeClicked() {
	QString id = QString(lineEdit->text().trimmed());
	bool ok;
	Timer::Connection h;	
	int num=id.toInt(&ok,10);
	textOutput->setText("");
	PERR("get input");	
	long quota=ram->avail()/1024/1024;	
	long length=quota*5/6;	
	PERR("%ld,%ld",quota,length);
	textOutput->append(QString("there is %1M available memory").arg(quota));
	Genode::Ram_dataspace_capability data=ram->alloc(length*1024*1024);
	textOutput->append(QString("alloc %1M memory to use").arg(length));
	textOutput->append(QString("leave %1M memory for meta data").arg(quota-length));
	char *addr=(char *)rm->attach(data);
	PERR("alloc memory and attach");
	Genode::memset(addr,num,length);	
	h.msleep(100);
	textOutput->append(QString("filled the mem with %1").arg(num));
	textOutput->append(QString("the first and last num are %1,%2").arg((int)addr[0]).arg((int)addr[length-1]));
	rm->detach(addr);
	PERR("detach");
	ram->free(data);
	h.msleep(500);
	textOutput->append(QString("free the memory"));
	textOutput->append(QString("now,the avail is %1M").arg(ram->avail()/1024/1024));
	
}
bool CommServDialog::checkBlock(const char*addr,int size){
	for(int i=0;i<size;i++)			
		if(addr[i]) return false;
	return true;								
}

void CommServDialog::readClicked(){
	PERR("read clicked");
	long quota=ram->avail()/1024/1024;	
	long length=quota*5/6;	
	textOutput->append(QString("there is %1M available memory").arg(quota));
	PERR("alloc %dM memory",length);
	Genode::Ram_dataspace_capability data=ram->alloc(length*1024*1024);
	char *addr=(char *)rm->attach(data);
	textOutput->append(QString("alloc %1M memory to read").arg(length));
	textOutput->append("check whether the content is cleared");
	bool isCleared=true;
	length*=1024*1024;
	int blocks=(length+4095)/4096;
	for(int i=0;i<blocks;i++){
		bool clear=checkBlock(addr+i*4096,length-i*4096>4096?4096:length-i*4096);
		isCleared|=clear;			
		//textOutput->append(QString("Block %1 Result: %2").arg(i).arg(clear?"cleared":"used"));
	}	
	if(isCleared)
		textOutput->append("RESULT:all the memory is cleared");
	else
		textOutput->append("RESULT:some memory is not cleared");
	rm->detach(addr);
	PERR("detach");
	ram->free(data);
		
}	


void CommServDialog::enableWriteButton(const QString &id) {
    writeButton->setEnabled(!id.isEmpty());
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
