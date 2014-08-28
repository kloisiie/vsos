/*
 * \brief  Launcher entry widget interface
 * \author Christian Prochaska
 * \date   2008-04-06
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef LAUNCH_ENTRY_H
#define LAUNCH_ENTRY_H

#include <launchpad/launchpad.h>

#include <QtGui/QWidget>
#include <QString>
#include <QIcon>
#include <QSize>
#include <QMessageBox>

#include "ui_launch_entry.h"
#include <monitor_session/client.h>
#include <monitor_session/connection.h>

class Launch_entry : public QWidget
{
    Q_OBJECT

public:
    Launch_entry(const char *filename, QString progtitle, unsigned long default_quota,
                 unsigned long max_quota, Launchpad *launchpad,Genode::Dataspace_capability config_ds,
                 QWidget *parent = 0);
    Ui::Launch_entryClass ui;
    ~Launch_entry();
public   slots:
	void restore_button(bool enabled);
private:
        const char *_filename;
	const char *_progtitle;
	Launchpad *_launchpad;
	Genode::Dataspace_capability _config_ds;
	unsigned long _default_quota;

public slots:
		void on_launchButton_clicked();
		
};

#endif // LAUNCH_ENTRY_H
