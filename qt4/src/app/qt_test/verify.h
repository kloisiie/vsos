#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QObject>
#include <QApplication>
class QLabel;
class QLineEdit;
class QPushButton;
//extern bool sakpressed;

class LoginDialog : public QDialog {
	Q_OBJECT
public:
	LoginDialog(QWidget *parent = 0);
	
private slots:
    	void loginClicked();
    	void cancelClicked();
    	void enableLoginButton(const QString &pass);	
private:
	QLabel *label_user;
	QLabel *label_pass;
	QLabel *label_space;
	QLineEdit *lineEdit_user;
	QLineEdit *lineEdit_pass;
    	QPushButton *loginButton;
    	QPushButton *cancelButton;	
};

#endif
 
