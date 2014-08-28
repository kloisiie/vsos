#ifndef COMMSERVDIALOG_H
#define COMMSERVDIALOG_H

#include <QApplication>
#include <QtGui>
#include <QDialog>
#include <QObject>
#include <QStyle>
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
	TitleBar(QWidget *parent);
   	~TitleBar(){};
    
public slots:
    void showSmall();
    
    void showMaxRestore();
protected:
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);

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
 
