#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QObject>
#include <QApplication>
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
//extern bool sakpressed;
struct User{
	char name[20];
	char password[20];
};
class LoginDialog : public QDialog {
	Q_OBJECT
public:
	LoginDialog(QWidget *parent = 0);
	struct User users[10]={{"admin","helloos5"},{"audit","show_deep"},{"securer","nfs.safe"},{"user","iscas.ac.cn"},{"guest","iscas.ac"}};
	int num;
	int isActive=true;
	int isEnter=false;
	bool eventFilter(QObject *watched,QEvent *e);
private slots:
    	void loginClicked();
    	void cancelClicked();
	void changeClicked();
    	void enableLoginButton(const QString &pass);
	void enableChangeButton(const QString &pass);	
	
private:
	QLabel *label_user;
	QLabel *label_pass;
	QLabel *label_pass2;
	QLabel *label_blp;
	QLabel *label_space;
	QLineEdit *lineEdit_user;
	QLineEdit *lineEdit_pass;
	QLineEdit *lineEdit_pass2;
	QComboBox *comboBox_blp;
    	QPushButton *loginButton;
    	QPushButton *cancelButton;
	QPushButton *changeButton;	
};

#endif
 
