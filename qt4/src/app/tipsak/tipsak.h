#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QDialog>
#include <QObject>
#include <QApplication>
#include <QCloseEvent>
class QLabel;

class LoginDialog : public QDialog {
	Q_OBJECT

private slots:
    	void thishide();

public:
	LoginDialog(QWidget *parent = 0);

	virtual void keyPressEvent(QKeyEvent *k);
	virtual void closeEvent(QCloseEvent *e);
	
private:
	QLabel *label_user;
	QLabel *label_pass;	
	
};

#endif
 
