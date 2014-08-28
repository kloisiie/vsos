#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QDialog>
#include <QObject>
#include <QApplication>
#include <QCloseEvent>
class QLabel;

class VersionDialog : public QDialog {
	Q_OBJECT

public:
	VersionDialog(QWidget *parent = 0);
private:
	QLabel *label_version;
};

#endif
 
