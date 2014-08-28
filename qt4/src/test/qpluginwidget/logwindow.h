#include <QtGui>
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
class logWindow: public QWidget{
	Q_OBJECT
	/*signals:
		void log_success();
	*/	
	public:
		logWindow(QWidget *parent=0);
	public slots:		
		void appendText(AuditInfo* s);
	private:
		const char *get_level(int i){
			switch(i){
				case 1:
					return " (secret) ";
				case 2:
					return " (confidential)";
				case 3:
					return " (top-secret)";
				default:
					return NULL;			
			}		
		}
		const char *get_opcode(int i){
			switch(i){
				case 1:
					return " (Read) ";
				case 2:
					return " (Write) ";
				case 3:
					return "(Read & Write)";
				default:
					return NULL;			
			}		
		}		
		QTextEdit *userText=new QTextEdit;
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
		QLabel *result=new QLabel("RESULT");
	
	
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
				s[i].m_IfRead=false;
			        i=(i+1)%MAX_LOG;
			}
			else
				h.msleep(100);					
		}
	}
} ;

