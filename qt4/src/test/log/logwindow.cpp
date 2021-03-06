#include "logwindow.h"
#include <base/printf.h>
logWindow::logWindow(QWidget *parent){
        setWindowFlags(Qt::FramelessWindowHint);
        setMouseTracking(true);
        this->setAttribute(Qt::WA_QuitOnClose,true);
        m_titleBar = new TitleBar(this);

	setWindowTitle(tr("日志结果"));
	model->setColumnCount(6);
	model->setHeaderData(0,Qt::Horizontal,tr("用户名"));
	model->setHeaderData(1,Qt::Horizontal,tr("客户端（主体）"));
	model->setHeaderData(2,Qt::Horizontal,tr("服务端（客体）"));
	model->setHeaderData(3,Qt::Horizontal,tr("操作"));
	model->setHeaderData(4,Qt::Horizontal,tr("结果"));
	model->setHeaderData(5,Qt::Horizontal,tr("时间"));
	table->setModel(model);
	table->setColumnWidth(0,120);
	table->setColumnWidth(1,150);
	table->setColumnWidth(2,150);
	table->setColumnWidth(3,120);
	table->setColumnWidth(4,80);
	table->setColumnWidth(5,100);
	table->verticalHeader()->setVisible(false);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(m_titleBar);
	mainLayout->addWidget(table);
	mainLayout->setMargin(2);
	setLayout(mainLayout);
	setFixedSize(750,200);
}

void logWindow::appendText(AuditInfo *s){
	//PERR("row %d",row);
	//restoreFormat();
	model->setItem(row,0,new QStandardItem(s->m_UserName));
	model->setItem(row,1,new QStandardItem(get_name(s->m_ClientName)));
	model->setItem(row,2,new QStandardItem(get_name(s->m_ServerName)));
	model->setItem(row,3,new QStandardItem(s->m_OperationName));
	model->setItem(row,4,new QStandardItem(s->m_OpResult?tr("允许"):tr("拒绝")));
	model->setItem(row,5,new QStandardItem(s->m_time));	
	row++;
	model->setItem(row,1,new QStandardItem(get_level(s->m_ClientBlpLevel)));
	model->setItem(row,2,new QStandardItem(get_level(s->m_ServerBlpLevel)));
	model->setItem(row,3,new QStandardItem(get_opcode(s->m_OpCode)));
	row++;
	changeFormat();
	
	
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


