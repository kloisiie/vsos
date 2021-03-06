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
#include <timer_session/client.h>
#include <timer_session/connection.h>
#include <monitor_session/client.h>
#include <monitor_session/connection.h>
#include <monitor_session/auditinfo.h>
#define MAX_LOG 100
class QLabel;
class QTableView;
class QStandardItemModel;

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

        QLabel *iconlabel = new QLabel(this);
	QPixmap apppix(":/appicon/log.png");
        iconlabel->setPixmap(apppix);
	iconlabel->setFixedWidth(40);
        
        QLabel *label = new QLabel(this);
        label->setText("日志结果");
        parent->setWindowTitle("日志结果");
        
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

class logWindow: public QWidget{
	Q_OBJECT
	/*signals:
		void log_success();
	*/	
	public:
		logWindow(QWidget *parent=0);
   		TitleBar *titleBar() const { return m_titleBar; }
    		void mousePressEvent(QMouseEvent *e);
    		void mouseMoveEvent(QMouseEvent *e);
    		void mouseReleaseEvent(QMouseEvent *e);
	public slots:		
		void appendText(AuditInfo* s);
    		void crossClicked();
	private:
		void  changeFormat(){
			for(int i=0;i<row;i+=2){
				table->setSpan(i,0,2,1);
				/*table->setSpan(i,1,2,1);
				table->setSpan(i,2,2,1);
				table->setSpan(i,3,2,1);*/
				table->setSpan(i,4,2,1);
				table->setSpan(i,5,2,1);
			}		
		}
		void  restoreFormat(){
			for(int i=0;i<row;i++){
				table->setSpan(i,0,1,1);
				/*table->setSpan(i,1,1,1);
				table->setSpan(i,2,1,1);
				table->setSpan(i,3,1,1);*/
				table->setSpan(i,4,1,1);
				table->setSpan(i,5,1,1);
			}		
		}		
		const char *get_level(int i){
			switch(i){
				case 1:
					return "低密级";
				case 2:
					return "中密级";
				case 3:
					return "高密级";
				default:
					return "--";			
			}		
		}
		const char *get_opcode(int i){
			switch(i){
				case 1:
					return "读操作";
				case 2:
					return "写操作";
				case 3:
					return "读写操作";
				default:
					return NULL;			
			}		
		}
		const char *get_name(const char *name){
			if(strcmp(name,"midlevel_process")==0)
				return "中密级服务程序";
			else if	(strcmp(name,"midlevel_app")==0)
				return "中密级应用";
			else if	(strcmp(name,"lowlevel_app")==0)
				return "低密级应用";
			else if	(strcmp(name,"highlevel_app")==0)
				return "高密级应用";
			else if(strcmp(name,"")!=0)
				return name;	
			else
				return "--";
		}
		QTableView *table=new QTableView;
		QStandardItemModel* model=new QStandardItemModel;
		int row=0;		
		/*QTextEdit *userText=new QTextEdit;
		QTextEdit *clientText=new QTextEdit;
		QTextEdit *serverText=new QTextEdit;
		QTextEdit *opText=new QTextEdit;
		QTextEdit *timeText=new QTextEdit;
		QTextEdit *resultText=new QTextEdit;
		QLabel *user=new QLabel("USER");
		QLabel *client=new QLabel("ClIENT");
		QLabel *server=new QLabel("SERVER");
		QLabel *op=new QLabel("OPERATION");
		QLabel *time=new QLabel("TIME");
		QLabel *result=new QLabel("RESULT");*/
 		TitleBar *m_titleBar;
 		QPushButton *m_content;
 		QPoint m_old_pos;
 		bool m_mouse_down;
 		bool left, right, bottom;
	
	
};

class Client: public QThread{	
	Q_OBJECT
	private:
	//	bool _continue;
		
	signals:
		void print_log(AuditInfo* s);
	/*public slots:		
		void continueLog(){
			_continue=true;		
		}		
	*/	
	public:
	void run()
	{
		PERR("client start");			
		int i=0;
		Timer::Connection h;
		Genode::Monitor_Connection log;	
		Genode::Dataspace_capability data=log.GetAuditInfo();
		AuditInfo *s=(AuditInfo *)Genode::env()->rm_session()->attach(data);
			
		while(true){
			if(s[i].m_IfRead){
				emit print_log(&s[i]);
				//emit print_log(buf);
				//PERR("buf:%d",strlen(buf));				
				//memcpy(buf,"hello",5);
			        i=(i+1)%MAX_LOG;
			}
			else
				h.msleep(100);					
		}
	}
} ;

