/*
 * \brief  Launcher entry widget implementation
 * \author Christian Prochaska
 * \date   2008-04-06
 */

#include "launch_entry.h"
#include <rtc_session/connection.h>

Launch_entry::Launch_entry(const char *filename, QString progtitle, unsigned long default_quota,
                           unsigned long max_quota, Launchpad *launchpad,
			   Genode::Dataspace_capability config_ds,QWidget *parent)
    : QWidget(parent), _filename(filename), _launchpad(launchpad),_config_ds(config_ds),_default_quota(default_quota)
{
	ui.setupUi(this);
	ui.launchButton->setText(progtitle);
	ui.launchButton->setIcon(QIcon(":/appicons/" + QString(filename) + ".png"));
	ui.launchButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	ui.launchButton->setIconSize(QSize(40,40));
	ui.launchButton->setAutoRaise(true);
}

Launch_entry::~Launch_entry()
{

}
void Launch_entry::restore_button(bool enabled){
	//PERR("retore_button");
	ui.launchButton->setEnabled(enabled);
}
void Launch_entry::on_launchButton_clicked()
{	
//	ui.launchButton->setEnabled(false);
	if(strcmp(_filename,"l4linux")==0) _launchpad->start_child("liquid_fb", 10 * 1024 * 1024, _config_ds);
	Genode::Monitor_Connection charge_avail;
	QString qAudit_info = "run task: ";
	qAudit_info.append(_filename);
	QByteArray temp = qAudit_info.toLatin1();    
	const char * audit_info=temp.data();
	if(charge_avail.ChargeApp(_filename,true)){
		//if(strcmp(_filename,"lowlevel_process")==0||strcmp(_filename,"midlevel_process")==0||strcmp(_filename,"highlevel_process")==0||strcmp(_filename,"lowlevel_app")==0||strcmp(_filename,"midlevel_app")==0||strcmp(_filename,"highlevel_app")==0||strcmp(_filename,"log")==0||strcmp(_filename,"funtest")==0||strcmp(_filename,"monitortest")==0||strcmp(_filename,"l4linux")==0)
		ui.launchButton->setEnabled(false);
		Rtc::Connection h; 			
		char _time[20]="";
		h.get_current_time_string(_time);
		PERR("%s",_time);
		PERR("retore_button");
		charge_avail.AddAuditInfo("",audit_info,1,0);
		PERR("%s",_time);
		PERR("retore_button");
		_launchpad->start_child(_filename, _default_quota * 1024, _config_ds);
		PERR("%s",_time);
	}
	else
	{		
		charge_avail.AddAuditInfo("",audit_info,0,0);
		QMessageBox::information(this, tr("警报"), tr("权限不足！"), QMessageBox::Ok);
	}	
	//QObject::connect(child,SIGNAL(button_restore()),this,SLOT(restore_button()),Qt::DirectConnection);
}
