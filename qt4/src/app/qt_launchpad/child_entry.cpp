/*
 * \brief  Child entry widget implementation
 * \author Christian Prochaska
 * \date   2008-04-06
 */

#include "child_entry.h"

Child_entry::Child_entry(const char *name, int quota_kb, int max_quota_kb,
	   	                   Launchpad *launchpad, Launchpad_child *launchpad_child,
								         QWidget *parent)
    : QWidget(parent), _launchpad(launchpad), _launchpad_child(launchpad_child), _quota_kb(quota_kb)
{
	ui.setupUi(this);
		
	ui.exitButton->setText(name);
	ui.exitButton->setIcon(QIcon(":/appicons/" + QString(name) + "_m.png"));
	ui.exitButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	ui.exitButton->setIconSize(QSize(25,25));
}

Child_entry::~Child_entry()
{

}

void Child_entry::on_exitButton_clicked()
{	
	//added by malele 20130924 begin
	//PDBG("Smeller:now Child_entry::on_exitButton_clicked\n");
	Launchpad *la=_launchpad;
	char _name[64];
	const char *name=_launchpad_child->name();
	memcpy(_name,name,strlen(name)+1);
	//added by malele end
	this->say_hello();
	try{
		_launchpad->exit_child(_launchpad_child);
	}catch(...){}
	//added by malele 20130924 begin
	if(la)
	{	
		//PDBG("Smeller:now restart the closed child %s...\n",_name);
		if(strcmp(_name,"midlevel_process")==0)		
		la->start_child(_name,_quota_kb * 1024, Genode::Dataspace_capability());
	}else{
		//PDBG("Smeller:_launchpad is gone. cannot restart...\n");
				
	}/**/
	//added by malele 20130924 end 
}
