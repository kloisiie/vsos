#ifndef COMMSERVDIALOG_H
#define COMMSERVDIALOG_H

#include <QtGui>
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDialog>
#include <QObject>
#include <QFont>
#include <QColor>
#include <QPalette>
#include <QString>
#include <QMatrix>
#include <string>
#include <iostream>
#include <confidential_session/connection.h>
#include <confidential_session/client.h>
//#include <hello_session/connection.h>
//#include <hello_session/client.h>
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;


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
        label->setText("低密级应用");
        parent->setWindowTitle("低密级应用");

        QLabel *iconlabel = new QLabel(this);
	QPixmap apppix(":/appicon/lowlevel_app.png");
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

class CommServDialog : public QDialog {
	Q_OBJECT
public:
	CommServDialog(QWidget *parent = 0);
   	TitleBar *titleBar() const { return m_titleBar; }
    	void mousePressEvent(QMouseEvent *e);
    	void mouseMoveEvent(QMouseEvent *e);
    	void mouseReleaseEvent(QMouseEvent *e);

public slots:
    	void crossClicked();
	
private slots:
    	void enableWriteButton(const QString &id);	
    	void enableRead_writeButton(const QString &nameId);
    	void readClicked();
    	void writeClicked();
    	void read_writeClicked();
private:
	QLineEdit *lineEdit;
	QLineEdit *lineOutput;
    	QPushButton *readButton;
    	QPushButton *writeButton;
    	QPushButton *read_writeButton;
	QLabel* label;
	QLabel* labelOutput;
	Confidential::Connection h;
	//Hello::Connection h;
 	TitleBar *m_titleBar;
 	QPushButton *m_content;
 	QPoint m_old_pos;
 	bool m_mouse_down;
 	bool left, right, bottom;
};

#endif
 
