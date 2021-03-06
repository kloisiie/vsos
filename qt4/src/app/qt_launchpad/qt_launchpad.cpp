/*
 * \brief   Qt Launchpad window implementation
 * \author  Christian Prochaska
 * \date    2008-04-05
 */

#include "qt_launchpad.h"
#include <launchpad/launchpad.h>

#include "launch_entry.h"
#include "child_entry.h"

#include <timer_session/connection.h>
#include <trustoneserver_session/client.h>
#include <trustoneserver_session/connection.h>
#include <sakserver_session/client.h>
#include <sakserver_session/connection.h>
#include <timer_session/connection.h>
#include <os/config.h>
#include <monitor_session/client.h>
#include <monitor_session/connection.h>

int nowkey = -1;
extern bool loginfinished;
Qt_launchpad::Qt_launchpad(unsigned long initial_quota, QWidget *parent)
    : QMainWindow(parent), Launchpad(initial_quota)
{
	setupUi(this);

	// disable minimize and maximize buttons
	Qt::WindowFlags flags = windowFlags();
	flags &= ((~Qt::WindowMinMaxButtonsHint)|(Qt::WindowStaysOnTopHint));
	setWindowFlags(flags);

	// To trigger lastWindowClosed()
	setAttribute(Qt::WA_QuitOnClose, true);

	launcherDockWidgetContents = new QToolBox();
	// put a QScrollArea into launcherDockWidget for scrolling of launcher entries
  	/*QScrollArea *launcherScrollArea = new QScrollArea;
	launcherScrollArea->setFrameStyle(QFrame::NoFrame);
	launcherScrollArea->setWidget(launcherDockWidgetContents);*/
	launcherDockWidget->setWidget(launcherDockWidgetContents);
	launcherDockWidget->setFont(QFont("OS5",12,QFont::Bold));

	// put a QScrollArea into childrenDockWidget for scrolling of child entries
  	QScrollArea *childrenScrollArea = new QScrollArea;
	childrenScrollArea->setFrameStyle(QFrame::NoFrame);
	childrenScrollArea->setWidget(childrenDockWidgetContents);
	
	childrenDockWidget->setWidget(childrenScrollArea);
	childrenDockWidget->setFont(QFont("OS5",12,QFont::Bold));
	
	QVBoxLayout *childrenDockWidgetLayout = new QVBoxLayout;
	childrenDockWidgetLayout->setContentsMargins(0, 0, 0, 0);
	childrenDockWidgetLayout->setSpacing(0);
	childrenDockWidgetLayout->setAlignment(Qt::AlignTop);
	childrenDockWidgetContents->setLayout(childrenDockWidgetLayout);
	childrenDockWidget->hide();
	statusDockWidget->hide();

	QObject::connect(childrenDockWidget,SIGNAL(topLevelChanged(bool)),this,SLOT(enlarge_childrenDockWg(bool)));
	QObject::connect(statusDockWidget,SIGNAL(topLevelChanged(bool)),this,SLOT(enlarge_statusDockWg(bool)));

	// update the available quota bar every 200ms
	QTimer *avail_quota_timer = new QTimer(this);
	connect(avail_quota_timer, SIGNAL(timeout()), this, SLOT(avail_quota_update()));
	avail_quota_timer->start(200);
	Middle* middle=new Middle();
	QObject::connect(middle,SIGNAL(quit_child()),this,SLOT(close_child()));
	QObject::connect(this,SIGNAL(unlock_child()),middle,SLOT(unlock()));	
	middle->start();
}

Qt_launchpad::~Qt_launchpad()
{
	
}

void Qt_launchpad::enlarge_childrenDockWg(bool isFloating)
{
	if(isFloating)
	{
		childrenDockWidget->setMaximumSize(90,240);
		childrenDockWidget->setMinimumSize(90,240);
	}
	if(!isFloating)
	{
		childrenDockWidget->setMinimumSize(110,30);
		childrenDockWidget->setMinimumSize(110,30);
	}
}

void Qt_launchpad::enlarge_statusDockWg(bool isFloating)
{
	if(isFloating)
	{
		statusDockWidget->setMaximumSize(130,80);
		statusDockWidget->setMinimumSize(130,80);
	}
	if(!isFloating)
	{
		statusDockWidget->setMinimumSize(110,30);
		statusDockWidget->setMinimumSize(110,30);
	}
}

void Qt_launchpad::avail_quota_update()
{
	static Genode::size_t _avail = 0;
	
	Genode::size_t new_avail = Genode::env()->ram_session()->avail();

	if (new_avail != _avail)
		quota(new_avail);

	_avail = new_avail;
}

void Qt_launchpad::quota(unsigned long quota)
{
	totalQuotaProgressBar->setMaximum(initial_quota() / (1024*1024));
	totalQuotaProgressBar->setValue(quota / (1024*1024));
}

void Qt_launchpad::add_launcher(const char *filename, QString progtitle,
                                unsigned long default_quota,Genode::Dataspace_capability config_ds, int groupNum)
{
	Launch_entry *launch_entry = new Launch_entry(filename, progtitle, default_quota / 1024,
                                              	initial_quota() / 1024, this,config_ds);
	launch_entry->setObjectName(QString(filename) + "_launch_entry");
	//launcherDockWidgetContents->layout()->addWidget(launch_entry);
	//launcherDockWidgetContents->layout()->setAlignment(Qt::AlignTop);
	if(groupNum==0&&strcmp(filename,"liquid_fb")!=0) {
		layoutMain->addWidget(launch_entry);
	}
	else if(strcmp(filename,"liquid_fb")==0) {
		launch_entry->ui.launchButton->setIconSize(QSize(0,0));
		launch_entry->ui.launchButton->setMinimumSize(QSize(0,0));
		launch_entry->ui.launchButton->setMaximumSize(QSize(0,0));
		layoutMain->addWidget(launch_entry);
	}
	else if(strcmp(filename,"funtest")==0) {
		layoutTest->addWidget(launch_entry);
	}
	else if(groupNum==1) {
		launch_entry->ui.launchButton->setIconSize(QSize(0,0));
		launch_entry->ui.launchButton->setMinimumSize(QSize(100,20));
		layoutTest->addWidget(launch_entry);
	}
	else if(groupNum==2) {
		layout3->addWidget(launch_entry);
	}
	launch_entry->show();
	launcherDockWidgetContents->adjustSize();
}

void Qt_launchpad::add_child(const char *unique_name,
                             unsigned long quota,
                             Launchpad_child *launchpad_child,
                             Genode::Allocator *alloc)
{
	Child_entry *child_entry = new Child_entry(unique_name, quota / 1024,
                                             initial_quota() / 1024,
                                             this, launchpad_child);
	Launch_entry *launch_entry=launcherDockWidgetContents->findChild<Launch_entry*>(QString(unique_name)+"_launch_entry");
	//PERR("connect unique name %s",unique_name);
	if(child_entry&&launch_entry){
		//PERR("connect");
		QObject::connect(child_entry,SIGNAL(rebutton(bool)),launch_entry,SLOT(restore_button(bool)),Qt::DirectConnection);
	}
	child_entry->setObjectName(QString(unique_name) + "_child_entry");
	childrenDockWidgetContents->layout()->addWidget(child_entry);
	child_entry->show();
	childrenDockWidgetContents->adjustSize();
	child_entry->say_hello(false);
}
														 
void Qt_launchpad::remove_child(const char *name, Genode::Allocator *alloc)
{
	Child_entry *child_entry =
	  childrenDockWidgetContents->findChild<Child_entry*>(QString(name) + "_child_entry");
	if (!child_entry) {
		PWRN("child entry lookup failed");
		return;
	}
	child_entry->say_hello();
	//PWRN("hello %lx",child_entry);
	// still in "button clicked" event handler
	child_entry->deleteLater();
	childrenDockWidgetContents->adjustSize();
	
	//PWRN("exit");
}


void Qt_launchpad::killR_child(const char *name)
{	
	memcpy(_name,name,strlen(name)+1);
	//PERR("close %s",_name);
	unlock_child();
 	//children_halted_lock.unlock();
	
}


void Qt_launchpad::keyPressEvent( QKeyEvent *k ) 
{ 

	//PERR("key pressed");	
	if(k->key() == Qt::Key_O && nowkey == -1) 
	{
		nowkey = 31;
	}
	if(k->key() == Qt::Key_S && nowkey == 31) 
	{
		nowkey = 55;
	}
	if(k->key() == Qt::Key_5 && nowkey == 55) 
	{
		nowkey = -1;
		QList<Child_entry*> child_entries = childrenDockWidgetContents->findChildren<Child_entry*>(QString());
		foreach(Child_entry* child_entry, child_entries){
			if (child_entry) this->exit_child(child_entry->_launchpad_child);
    		}
		this->hide();
		QList<QToolButton*> toolbuttons = this->findChildren<QToolButton*>(QString());
		foreach(QToolButton* toolbutton, toolbuttons){
			if (toolbutton) layoutMain->removeWidget(toolbutton);
			if (toolbutton) layoutTest->removeWidget(toolbutton);
			if (toolbutton) layout3->removeWidget(toolbutton);
			toolbutton->deleteLater();
			launcherDockWidgetContents->adjustSize();
    		}
		layoutMain->deleteLater();
		layoutTest->deleteLater();
		layout3->deleteLater();
		layoutChild->deleteLater();
		layoutQuota->deleteLater();
		groupBoxMain->deleteLater();
		groupBoxTest->deleteLater();
		groupBox3->deleteLater();
		groupBoxChild->deleteLater();
		groupBoxQuota->deleteLater();

		TrustONEserver::Connection t;
		t.trust_one();
		loginfinished=false;
		while(!loginfinished){
			Timer::Connection loginwait;
			loginwait.msleep(1000);
			if(loginfinished){
				process_config(this);
				launcherDockWidget->setWidget(launcherDockWidgetContents);
				loginwait.msleep(500);
				this->show();
			}
		}
	}
}

void Qt_launchpad::closeEvent( QCloseEvent *ev ) 
{

}
void Qt_launchpad::close_child(){
	Child_entry *child_entry =
	  	childrenDockWidgetContents->findChild<Child_entry*>(QString(_name)+"_child_entry");
	//PWRN("%s:%lx,%lx",_name,child_entry,child_entry->_launchpad_child);

	if(child_entry) this->exit_child(child_entry->_launchpad_child);
	//added by malele 20130924 begin
	if(this)
	{	
		//PDBG("Smeller:now restart the closed child %s...\n",_name);
		if(strcmp(_name,"midlevel_process")==0)	
			this->start_child(_name,16*1024*1024, Genode::Dataspace_capability());
	}else{
		//PDBG("Smeller:_launchpad is gone. cannot restart...\n");
			
	}/**/
	//added by malele 20130924 end 
}

void Qt_launchpad::process_config(Qt_launchpad *launchpad)
{
	groupBoxMain = new QGroupBox();
	groupBoxTest = new QGroupBox();
	groupBox3 = new QGroupBox();
	groupBoxChild = new QGroupBox();
	groupBoxQuota = new QGroupBox();

	layoutMain = new QVBoxLayout(groupBoxMain);
	layoutMain->setMargin(0);
	layoutMain->setContentsMargins(1, 1, 1, 1);
	layoutMain->setSpacing(2);
	layoutMain->setAlignment(Qt::AlignTop);
	layoutTest = new QVBoxLayout(groupBoxTest);
	layoutTest->setMargin(0);
	layoutTest->setContentsMargins(1, 1, 1, 1);
	layoutTest->setSpacing(2);
	layoutTest->setAlignment(Qt::AlignTop);
	layout3 = new QVBoxLayout(groupBox3);
	layout3->setMargin(0);
	layout3->setContentsMargins(1, 1, 1, 1);
	layout3->setSpacing(2);
	layout3->setAlignment(Qt::AlignTop);
	layoutChild = new QVBoxLayout(groupBoxChild);
	layoutChild->setMargin(0);
	layoutChild->setContentsMargins(1, 1, 1, 1);
	layoutChild->setSpacing(2);
	layoutChild->setAlignment(Qt::AlignTop);
	layoutQuota = new QVBoxLayout(groupBoxQuota);
	layoutQuota->setMargin(0);
	layoutQuota->setContentsMargins(1, 1, 1, 1);
	layoutQuota->setSpacing(2);
	layoutQuota->setAlignment(Qt::AlignTop);

	launcherDockWidgetContents->addItem( ( QWidget* )groupBoxMain , "普通应用");
	launcherDockWidgetContents->addItem( ( QWidget* )groupBoxTest , "测试工具集");
	launcherDockWidgetContents->addItem( ( QWidget* )groupBox3 , "管理工具");
	launcherDockWidgetContents->addItem( ( QWidget* )groupBoxChild , "进程管理");
	launcherDockWidgetContents->addItem( ( QWidget* )groupBoxQuota , "空闲内存");

	layoutChild->addWidget(childrenDockWidgetContents);
	layoutChild->setAlignment(Qt::AlignTop);
	layoutQuota->addWidget(totalQuotaProgressBar);
	layoutQuota->setAlignment(Qt::AlignTop);

	using namespace Genode;

	Monitor_Connection charge_app;
	Xml_node config_node = config()->xml_node();

	/*
	 * Iterate through all entries of the config file and create
	 * launchpad entries as specified.
	 */
	int launcher_cnt = 0;
	for (unsigned i = 0; i < config_node.num_sub_nodes(); i++) {
		Xml_node node = config_node.sub_node(i);
		if (node.has_type("launcher"))

			/* catch XML syntax errors within launcher node */
			try {
				/* read file name and default quote from launcher node */
				Xml_node filename_node = node.sub_node("filename");

				size_t filename_len = filename_node.content_size();
				char *filename = (char *)env()->heap()->alloc(filename_len + 1);
				if (!filename) {
					::printf("Error: Out of memory while processing configuration\n");
					return;
				}
				filename_node.value(filename, filename_len + 1);
				
				Xml_node appname_node = node.sub_node("appname");
				size_t appname_len = appname_node.content_size();
				char *appname = (char *)env()->heap()->alloc(appname_len + 1);
				if (!appname) {
					::printf("Error: Out of memory while processing configuration\n");
					return;
				}
				appname_node.value(appname, appname_len + 1);

				Xml_node ram_quota_node = node.sub_node("ram_quota");
				Number_of_bytes default_ram_quota = 0;
				ram_quota_node.value(&default_ram_quota);

				Xml_node groupnum_node = node.sub_node("groupnum");
				Number_of_bytes groupnum = 0;
				groupnum_node.value(&groupnum);				

				/* obtain configuration for the child */
				Init::Child_config *config = new (env()->heap())
					Init::Child_config(Genode::env()->ram_session_cap(), node);

				/* add launchpad entry */
				if(charge_app.ChargeApp(filename,false))
					add_launcher(filename,appname, default_ram_quota,
				                        config->dataspace(),(int)groupnum);
				launcher_cnt++;

			} catch (...) {
				::printf("Warning: Launcher entry %d is malformed.\n",
				         launcher_cnt + 1);
			}
		else {
			char buf[32];
			node.type_name(buf, sizeof(buf));
			::printf("Warning: Ignoring unsupported tag <%s>.\n", buf);
		}
	}
	if(charge_app.ChargeApp("liquid_fb",false)) add_launcher("liquid_fb", "",  10*1024*1024);
}

void Middle::run() {
	while (true) {
		//PERR("qthread open");
		children_halted_lock.lock();
 		quit_child();
		//PERR("close child");
		
	}
}

void Middle::unlock(){
	//PERR("5435");
	children_halted_lock.unlock();
}

Middle::Middle():
	children_halted_lock(Genode::Lock::LOCKED){}

