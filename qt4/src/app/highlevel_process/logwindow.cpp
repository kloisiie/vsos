#include "logwindow.h"
#include <base/printf.h>
//namespace Confidential{
extern int state;
//}

logWindow::logWindow(QWidget *parent){
        m_mouse_down = false;
        //setFrameShape(Panel);
        
        // Make this a borderless window which can't
        // be resized or moved via the window system
        setWindowFlags(Qt::FramelessWindowHint);
        setMouseTracking(true);
        this->setAttribute(Qt::WA_QuitOnClose,true);
        m_titleBar = new TitleBar(this);

	logText=new QTextEdit(this);
	setWindowTitle(tr("高密级服务进程"));
	logText->setText("");
	logText->setReadOnly(true);
	/*QGridLayout *layout=new QGridLayout;
	layout->addWidget(m_titleBar,0,0);
	layout->addWidget(logText,0,0);*/
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(m_titleBar);
	mainLayout->addWidget(logText);
	mainLayout->setMargin(2);
	setLayout(mainLayout);
	setFixedSize(300,125);
}

void logWindow::appendText(const char* str){
	//PERR("append Text");
	logText->append(str);
	emit log_success();
	
}

void Client::run()
	{
		PERR("client start");			
		Timer::Connection h;
		MSG::Connection backup;
		_continue=true;	
		/*while(true){
			int op=0,arg=0;
			if(_continue&&backup.get_msg(op,arg)){
				char buf[256];
				if(op>=2)
					state=arg;
				Genode::snprintf(buf,256,"%s %d",getopcode(op),arg);
				_continue=false;
				emit print_log(buf);				
			
			}
			else
				h.msleep(100);					
		}*/
	}


void logWindow::crossClicked(){
	PERR("close");
	exit(0);
}

    void logWindow::mousePressEvent(QMouseEvent *e)
    {
        m_old_pos = e->pos();
        m_mouse_down = e->button() == Qt::LeftButton;
    }
    
    void logWindow::mouseMoveEvent(QMouseEvent *e)
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
    
    void logWindow::mouseReleaseEvent(QMouseEvent *e)
    {
        m_mouse_down = false;
    }

