/*
 * \brief   Qt Launchpad window interface
 * \author  Christian Prochaska
 * \date    2008-04-05
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef QT_LAUNCHPAD_H
#define QT_LAUNCHPAD_H

#include <launchpad/launchpad.h>

#include <QtGui>
#include <QString>
#include <QSpacerItem>
#include "ui_qt_launchpad.h"
class Middle:public QThread{
	 Q_OBJECT
	private:	
	Genode::Lock                  children_halted_lock;
	
	public slots:
		void unlock();
	public:
	Middle();
	void run();
	signals:
		void quit_child();
};

class Qt_launchpad : public QMainWindow, public Launchpad, private Ui::Qt_launchpadClass
{
    Q_OBJECT

public:
	Qt_launchpad(unsigned long initial_quota, QWidget *parent = 0);
	~Qt_launchpad();
	QVBoxLayout *layoutMain;
	QVBoxLayout *layoutTest;
	QVBoxLayout *layout3;
	QVBoxLayout *layoutChild;
	QVBoxLayout *layoutQuota;
	QGroupBox *groupBoxMain;
	QGroupBox *groupBoxTest;
	QGroupBox *groupBox3;
	QGroupBox *groupBoxChild;
	QGroupBox *groupBoxQuota;

	virtual void quota(unsigned long quota);

	virtual	void add_launcher(const char *filename, QString progtitle,
		                          unsigned long default_quota,Genode::Dataspace_capability config_ds=Genode::Dataspace_capability(), int groupNum = 0);

	virtual void add_child(const char *unique_name,
		                       unsigned long quota,
													 Launchpad_child *launchpad_child,
		                       Genode::Allocator *alloc);
														 
	virtual void remove_child(const char *name, Genode::Allocator *alloc);	
		 
	virtual void killR_child(const char *name);

	virtual void keyPressEvent(QKeyEvent *k);

	virtual	void closeEvent(QCloseEvent *ev);

	virtual	void process_config(Qt_launchpad *launchpad);


protected:
	
	char _name[64];
private:
	signals:
		void unlock_child();	
public slots:
	void close_child();								
private slots:
		void avail_quota_update();
		void enlarge_childrenDockWg(bool isFloating);
		void enlarge_statusDockWg(bool isFloating);
};

#endif // QT_LAUNCHPAD_H
