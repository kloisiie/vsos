#ifndef _INCLUDE__LOGWINDOW_H_
#define _INCLUDE__LOGWINDOW_H_

#include <QtGui>
#include <QObject>
#include <QStyle>
#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QObject>
#include <QMatrix>
#include <qtextedit.h>
#include <qthread.h>
#include <base/thread.h>
#include <base/printf.h>
#define MAX_LOG 100
#include <timer_session/client.h>
#include <timer_session/connection.h>
#include <lowlevel_session/lowlevel_session.h>
#include <base/env.h>
#include <rom_session/connection.h>
#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <base/rpc_server.h>
#include <back_session/connection.h>


class TitleBar : public QWidget
{
    Q_OBJECT
public:
    TitleBar(QWidget *parent)
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
        label->setText("低密级服务进程");
        parent->setWindowTitle("低密级服务进程");

        QLabel *iconlabel = new QLabel(this);
	QPixmap apppix(":/appicon/lowlevel_process.png");
        iconlabel->setPixmap(apppix);
	iconlabel->setFixedWidth(40);

        QLabel *label2 = new QLabel(this);
        label2->setText("               ");

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
    ~TitleBar(){};
    
public slots:
    void showSmall()
    {
        parentWidget()->showMinimized();
    }
    
    void showMaxRestore()
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
protected:
    void mousePressEvent(QMouseEvent *me)
    {
        startPos = me->globalPos();
        clickPos = mapToParent(me->pos());
    }
    void mouseMoveEvent(QMouseEvent *me)
    {
        if (maxNormal)
            return;
        parentWidget()->move(me->globalPos() - clickPos);
    }

private:
    QToolButton *minimize;
    QToolButton *maximize;
    QToolButton *close;
    QPixmap restorePix, maxPix;
    bool maxNormal;
    QPoint startPos;
    QPoint clickPos;
};


class Client: public QThread{	
	Q_OBJECT
	private:
		bool _continue;
	signals:
		void print_log(const char * str);
	public slots:		
		void continueLog(){
			_continue=true;		
		}		
	public:
	const char * getopcode(int i){
		if(i==1)
			return "read state ";
		else if(i==2)
			return "write state ";
		else
			return "change state ";	
	}
	void run();
	
} ;





class logWindow: public QWidget{
	Q_OBJECT
	signals:
		void log_success();
	public:
		logWindow(QWidget *parent=0);
   		TitleBar *titleBar() const { return m_titleBar; }
    		void mousePressEvent(QMouseEvent *e);
    		void mouseMoveEvent(QMouseEvent *e);
    		void mouseReleaseEvent(QMouseEvent *e);
	public slots:		
		void appendText(const char *);
    		void crossClicked();
	private:		
		QTextEdit *logText;
 		TitleBar *m_titleBar;
 		QPushButton *m_content;
 		QPoint m_old_pos;
 		bool m_mouse_down;
 		bool left, right, bottom;
	
	
};

#endif 


