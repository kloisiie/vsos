#ifndef MONITORTEST_H
#define MONITORTEST_H

#include <QApplication>
#include <QtGui>
#include <QDialog>
#include <QObject>
#include <QStyle>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
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
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QComboBox;

class TitleBar : public QWidget
{
    Q_OBJECT
public:
	TitleBar(QWidget *parent);
    
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
/*
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
    	void testButtonClicked();
    	void onChanged(int index);
    	void onChangedForTest(int index);
private:
	QComboBox *serviceCombo;
	QComboBox *operationCombo;
	QTextEdit *resultOutput;
    QPushButton *testButton;
	QLabel* labelTitle;
	QLabel* label;
	QLabel* labelOutput;
 	TitleBar *m_titleBar;
 	QPushButton *m_content;
 	QPoint m_old_pos;
 	bool m_mouse_down;
 	bool left, right, bottom;
};
*/

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public:
   	TitleBar *titleBar() const { return m_titleBar; }
	QStringList tasknamelist;
	QStringList usernamelist;
	QStringList taskoutput;
	QStringList useroutput;
    	void mousePressEvent(QMouseEvent *e);
    	void mouseMoveEvent(QMouseEvent *e);
    	void mouseReleaseEvent(QMouseEvent *e);
	

public slots:
    	void crossClicked();
	void UserInterfaceClicked();
	void TaskInterfaceClicked();
	void user_showrange();
	void task_showrange();
    
private:
	char taskblp[50],taskbiba[50];
   	Ui::Dialog *ui;
	TitleBar *m_titleBar;
	QPoint m_old_pos;
 	bool m_mouse_down;
 	bool left, right, bottom;
};
#endif
 
