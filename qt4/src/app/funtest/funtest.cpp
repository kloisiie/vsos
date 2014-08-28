/*
 * \brief   funtest
 * \author  OS5
 * \date    2013-09-22
 */

/* Qt includes */
/* Qoost includes */
#include "funtest.h"


#include <base/env.h>
#include <timer_session/connection.h>
// begin test
#include <rom_session/connection.h>
#include <io_mem_session/connection.h>
#include <io_port_session/connection.h>
#include <ram_session/connection.h>
#include <log_session/connection.h>
#include <signal_session/connection.h>
#include <pd_session/connection.h>
#include <rm_session/connection.h>
#include <rm_session/rm_session.h>
#include <cap_session/connection.h>
#include <cpu_session/connection.h>
#include <irq_session/connection.h>
//end of test
QString currInterface="Rom_dataspace_capability dataspace()";

CommServDialog::CommServDialog(QWidget *parent)
:QDialog(parent)
{
	
        m_mouse_down = false;
        //setFrameShape(Panel);
        
        // Make this a borderless window which can't
        // be resized or moved via the window system
        setMouseTracking(true);
        this->setAttribute(Qt::WA_QuitOnClose,true);
        m_titleBar = new TitleBar(this);

	label=new QLabel("系统基本服务：");
	labelOutput=new QLabel("操作：");
	serviceCombo = new QComboBox(this);
	serviceCombo->resize(50, 20);
	serviceCombo->addItems(QStringList()<<"ROM"<<"IO_MEM"/*<<"IO_PORT"*/<<"RAM"<<"LOG"<<"Signal"<<"PD"<<"RM"<<"CAP"<<"CPU"/*<<"IRQ"*/);
	
	operationCombo = new QComboBox(this);
	operationCombo->resize(450, 20);
	operationCombo->addItems(QStringList()<<"Rom_dataspace_capability dataspace()"<<"void sigh(Signal_context_capability sigh)");
	
	resultOutput = new QTextEdit;
	resultOutput->setText("");
	resultOutput->setReadOnly(true);
	
    	testButton = new QPushButton(tr("测试"));
	
    	QObject::connect(testButton,SIGNAL(clicked()),this,SLOT(testButtonClicked()));
    	QObject::connect(serviceCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));
    	QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
    	
    	QHBoxLayout *topLayoutUp = new QHBoxLayout;
    	topLayoutUp->addWidget(label);
    	topLayoutUp->addWidget(serviceCombo);
    	topLayoutUp->addWidget(labelOutput);
    	topLayoutUp->addWidget(operationCombo);
    	topLayoutUp->addWidget(testButton);
      	topLayoutUp->setMargin(5);
       	topLayoutUp->setSpacing(3);

    	QHBoxLayout *topLayoutMiddle = new QHBoxLayout;
    	topLayoutMiddle->addWidget(resultOutput);
      	topLayoutMiddle->setMargin(5);
       	topLayoutMiddle->setSpacing(3);

	QVBoxLayout *mainLayout = new QVBoxLayout;
    	mainLayout->addWidget(m_titleBar);
    	mainLayout->addLayout(topLayoutUp);
    	mainLayout->addLayout(topLayoutMiddle);
        mainLayout->setMargin(1);
        mainLayout->setSpacing(0);
    	
    	setLayout(mainLayout);
    	setWindowTitle(tr("系统基本服务接口测试程序"));
	setFixedSize(580,300);
}

void CommServDialog::onChanged(int index){
	switch(index){
	case 0:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"Rom_dataspace_capability dataspace()"<<"void sigh(Signal_context_capability sigh)");
		break;
	case 1:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItem("Io_mem_dataspace_capability dataspace()");
		break;
/*	case 2:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"unsigned char inb(unsigned short address)"
			<<"unsigned short inw(unsigned short address)"
			<<"unsigned inl(unsigned short address)"
			<<"void outb(unsigned short address, unsigned char value)"
			<<"void outw(unsigned short address, unsigned short value)"
			<<"void outl(unsigned short address, unsigned value)");
		break;*/
	case 2:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"int ref_account(Ram_session_capability ram_session_cap)"
			<<"int transfer_quota(Ram_session_capability ram_session_cap,size_t amount)"
			<<"Ram_dataspace_capability alloc(size_t size)"
			<<"void free(Ram_dataspace_capability ds_cap)"
			<<"size_t quota()"
			<<"size_t used()");
		break;
	case 3:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItem("size_t write(const char *string)");
		break;
	case 4:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"Signal_source_capability signal_source()"
			<<"Signal_context_capability alloc_context(long imprint)"
			<<"void free_context(Signal_context_capability cap)"
			<<"void submit(Signal_context_capability context,unsigned cnt = 1)");
		break;
	case 5:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"int bind_thread(Thread_capability thread)"/*<<"int assign_parent(Parent_capability parent)"*/);
		break;
	case 6:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
    		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"Local_addr attach(Dataspace_capability ds, size_t size = 0, off_t offset = 0, bool use_local_addr = false, Local_addr local_addr = (void *)0, bool executable = false)"
			<<"void detach(Local_addr local_addr)"
			<<"Pager_capability add_client(Thread_capability thread)"
			<<"void remove_client(Pager_capability)"
			<<"void fault_handler(Signal_context_capability handler)"
			<<"State state()"
			<<"Dataspace_capability dataspace()");
		break;
	case 7:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"Capability alloc(Capability ep_cap)"<<"void free(Capability cap)");
		break;
	case 8:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"Thread_capability create_thread(Name const &name,addr_t utcb = 0)"
			<<"Ram_dataspace_capability utcb(Thread_capability thread)"
			<<"void kill_thread(Thread_capability thread_cap)"
			<<"int set_pager(Thread_capability thread_cap,Pager_capability pager_cap)"
			<<"int cancel_blocking(Thread_capability thread_cap)"
			/*<<"int start(Thread_capability thread_cap,addr_t ip, addr_t sp)"*/
			<<"void pause(Thread_capability thread)"
			<<"void resume(Thread_capability thread)"
			<<"Thread_state state(Thread_capability thread_cap)"
			<<"void state(Thread_capability thread,Thread_state const &state)"
			<<"void exception_handler(Thread_capability thread, Signal_context_capability handler)"
			<<"void single_step(Thread_capability, bool)"
			<<"unsigned num_cpus()"
			<<"void affinity(Thread_capability thread, unsigned cpu)");
		break;
/*	case 10:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItem("void wait_for_irq()");
		break;*/
	default:
		QObject::disconnect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->clear();
		QObject::connect(operationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedForTest(int)));
		operationCombo->addItems(QStringList()<<"Rom_dataspace_capability dataspace()"<<"void sigh(Signal_context_capability sigh)");
	}
}

void CommServDialog::onChangedForTest(int index){
	switch(index){
	case 0:
		if(operationCombo->itemText(0)=="Rom_dataspace_capability dataspace()"){
			currInterface.clear();
			currInterface.append("Rom_dataspace_capability dataspace()");
		}
		else if(operationCombo->itemText(0)=="Io_mem_dataspace_capability dataspace()"){
			currInterface.clear();
			currInterface.append("Io_mem_dataspace_capability dataspace()");
		}
		else if(operationCombo->itemText(0)=="unsigned char inb(unsigned short address)"){
			currInterface.clear();
			currInterface.append("unsigned char inb(unsigned short address)");
		}
		else if(operationCombo->itemText(0)=="int ref_account(Ram_session_capability ram_session_cap)"){
			currInterface.clear();
			currInterface.append("int ref_account(Ram_session_capability ram_session_cap)");
		}
		else if(operationCombo->itemText(0)=="size_t write(const char *string)"){
			currInterface.clear();
			currInterface.append("size_t write(const char *string)");
		}
		else if(operationCombo->itemText(0)=="Signal_source_capability signal_source()"){
			currInterface.clear();
			currInterface.append("Signal_source_capability signal_source()");
		}
		else if(operationCombo->itemText(0)=="int bind_thread(Thread_capability thread)"){
			currInterface.clear();
			currInterface.append("int bind_thread(Thread_capability thread)");
		}
		else if(operationCombo->itemText(0)=="Local_addr attach(Dataspace_capability ds, size_t size = 0, off_t offset = 0, bool use_local_addr = false, Local_addr local_addr = (void *)0, bool executable = false)"){
			currInterface.clear();
			currInterface.append("Local_addr attach(Dataspace_capability ds, size_t size = 0, off_t offset = 0, bool use_local_addr = false, Local_addr local_addr = (void *)0, bool executable = false)");
		}
		else if(operationCombo->itemText(0)=="Capability alloc(Capability ep_cap)"){
			currInterface.clear();
			currInterface.append("Capability alloc(Capability ep_cap)");
		}
		else if(operationCombo->itemText(0)=="Thread_capability create_thread(Name const &name,addr_t utcb = 0)"){
			currInterface.clear();
			currInterface.append("Thread_capability create_thread(Name const &name,addr_t utcb = 0)");
		}
		else if(operationCombo->itemText(0)=="void wait_for_irq()"){
			currInterface.clear();
			currInterface.append("void wait_for_irq()");
		}
		break;
	case 1:
		if(operationCombo->itemText(1)=="void sigh(Signal_context_capability sigh)"){
			currInterface.clear();
			currInterface.append("void sigh(Signal_context_capability sigh)");
		}
		else if(operationCombo->itemText(1)=="unsigned short inw(unsigned short address)"){
			currInterface.clear();
			currInterface.append("unsigned short inw(unsigned short address)");
		}
		else if(operationCombo->itemText(1)=="int transfer_quota(Ram_session_capability ram_session_cap,size_t amount)"){
			currInterface.clear();
			currInterface.append("int transfer_quota(Ram_session_capability ram_session_cap,size_t amount)");
		}
		else if(operationCombo->itemText(1)=="Signal_context_capability alloc_context(long imprint)"){
			currInterface.clear();
			currInterface.append("Signal_context_capability alloc_context(long imprint)");
		}
		else if(operationCombo->itemText(1)=="int assign_parent(Parent_capability parent)"){
			currInterface.clear();
			currInterface.append("int assign_parent(Parent_capability parent)");
		}
		else if(operationCombo->itemText(1)=="void detach(Local_addr local_addr)"){
			currInterface.clear();
			currInterface.append("void detach(Local_addr local_addr)");
		}
		else if(operationCombo->itemText(1)=="void free(Capability cap)"){
			currInterface.clear();
			currInterface.append("void free(Capability cap)");
		}
		else if(operationCombo->itemText(1)=="Ram_dataspace_capability utcb(Thread_capability thread)"){
			currInterface.clear();
			currInterface.append("Ram_dataspace_capability utcb(Thread_capability thread)");
		}
		break;
	case 2:
		if(operationCombo->itemText(2)=="unsigned inl(unsigned short address)"){
			currInterface.clear();
			currInterface.append("unsigned inl(unsigned short address)");
		}
		else if(operationCombo->itemText(2)=="Ram_dataspace_capability alloc(size_t size)"){
			currInterface.clear();
			currInterface.append("Ram_dataspace_capability alloc(size_t size)");
		}
		else if(operationCombo->itemText(2)=="void free_context(Signal_context_capability cap)"){
			currInterface.clear();
			currInterface.append("void free_context(Signal_context_capability cap)");
		}
		else if(operationCombo->itemText(2)=="Pager_capability add_client(Thread_capability thread)"){
			currInterface.clear();
			currInterface.append("Pager_capability add_client(Thread_capability thread)");
		}
		else if(operationCombo->itemText(2)=="void kill_thread(Thread_capability thread_cap)"){
			currInterface.clear();
			currInterface.append("void kill_thread(Thread_capability thread_cap)");
		}
		break;
	case 3:
		if(operationCombo->itemText(3)=="void outb(unsigned short address, unsigned char value)"){
			currInterface.clear();
			currInterface.append("void outb(unsigned short address, unsigned char value)");
		}
		else if(operationCombo->itemText(3)=="void free(Ram_dataspace_capability ds_cap)"){
			currInterface.clear();
			currInterface.append("void free(Ram_dataspace_capability ds_cap)");
		}
		else if(operationCombo->itemText(3)=="void submit(Signal_context_capability context,unsigned cnt = 1)"){
			currInterface.clear();
			currInterface.append("void submit(Signal_context_capability context,unsigned cnt = 1)");
		}
		else if(operationCombo->itemText(3)=="void remove_client(Pager_capability)"){
			currInterface.clear();
			currInterface.append("void remove_client(Pager_capability)");
		}
		else if(operationCombo->itemText(3)=="int set_pager(Thread_capability thread_cap,Pager_capability pager_cap)"){
			currInterface.clear();
			currInterface.append("int set_pager(Thread_capability thread_cap,Pager_capability pager_cap)");
		}
		break;
	case 4:
		if(operationCombo->itemText(4)=="void outw(unsigned short address, unsigned short value)"){
			currInterface.clear();
			currInterface.append("void outw(unsigned short address, unsigned short value)");
		}
		else if(operationCombo->itemText(4)=="size_t quota()"){
			currInterface.clear();
			currInterface.append("size_t quota()");
		}
		else if(operationCombo->itemText(4)=="void fault_handler(Signal_context_capability handler)"){
			currInterface.clear();
			currInterface.append("void fault_handler(Signal_context_capability handler)");
		}
		else if(operationCombo->itemText(4)=="int cancel_blocking(Thread_capability thread_cap)"){
			currInterface.clear();
			currInterface.append("int cancel_blocking(Thread_capability thread_cap)");
		}
		break;
	case 5:
		if(operationCombo->itemText(5)=="void outl(unsigned short address, unsigned value)"){
			currInterface.clear();
			currInterface.append("void outl(unsigned short address, unsigned value)");
		}
		else if(operationCombo->itemText(5)=="size_t used()"){
			currInterface.clear();
			currInterface.append("size_t used()");
		}
		else if(operationCombo->itemText(5)=="State state()"){
			currInterface.clear();
			currInterface.append("State state()");
		}
		/*else if(operationCombo->itemText(5)=="int start(Thread_capability thread_cap,addr_t ip, addr_t sp)"){
			currInterface.clear();
			currInterface.append("int start(Thread_capability thread_cap,addr_t ip, addr_t sp)");
		}*/
		else if(operationCombo->itemText(5)=="void pause(Thread_capability thread)"){
			currInterface.clear();
			currInterface.append("void pause(Thread_capability thread)");
		}
		break;
	case 6:
		if(operationCombo->itemText(6)=="Dataspace_capability dataspace()"){
			currInterface.clear();
			currInterface.append("Dataspace_capability dataspace()");
		}
		/*else if(operationCombo->itemText(6)=="void pause(Thread_capability thread)"){
			currInterface.clear();
			currInterface.append("void pause(Thread_capability thread)");
		}*/
		else if(operationCombo->itemText(6)=="void resume(Thread_capability thread)"){
			currInterface.clear();
			currInterface.append("void resume(Thread_capability thread)");
		}
		break;
	case 7:
		/*if(operationCombo->itemText(7)=="void resume(Thread_capability thread)"){
			currInterface.clear();
			currInterface.append("void resume(Thread_capability thread)");
		}*/
		if(operationCombo->itemText(7)=="Thread_state state(Thread_capability thread_cap)"){
			currInterface.clear();
			currInterface.append("Thread_state state(Thread_capability thread_cap)");
		}
		break;
	case 8:
		/*if(operationCombo->itemText(8)=="Thread_state state(Thread_capability thread_cap)"){
			currInterface.clear();
			currInterface.append("Thread_state state(Thread_capability thread_cap)");
		}*/
		if(operationCombo->itemText(8)=="void state(Thread_capability thread,Thread_state const &state)"){
			currInterface.clear();
			currInterface.append("void state(Thread_capability thread,Thread_state const &state)");
		}
		break;
	case 9:
		/*if(operationCombo->itemText(9)=="void state(Thread_capability thread,Thread_state const &state)"){
			currInterface.clear();
			currInterface.append("void state(Thread_capability thread,Thread_state const &state)");
		}*/
		if(operationCombo->itemText(9)=="void exception_handler(Thread_capability thread, Signal_context_capability handler)"){
			currInterface.clear();
			currInterface.append("void exception_handler(Thread_capability thread, Signal_context_capability handler)");
		}
		break;
	case 10:
		/*if(operationCombo->itemText(10)=="void exception_handler(Thread_capability thread, Signal_context_capability handler)"){
			currInterface.clear();
			currInterface.append("void exception_handler(Thread_capability thread, Signal_context_capability handler)");
		}*/
		if(operationCombo->itemText(10)=="void single_step(Thread_capability, bool)"){
			currInterface.clear();
			currInterface.append("void single_step(Thread_capability, bool)");
		}
		break;
	case 11:
		/*if(operationCombo->itemText(11)=="void single_step(Thread_capability, bool)"){
			currInterface.clear();
			currInterface.append("void single_step(Thread_capability, bool)");
		}*/
		if(operationCombo->itemText(11)=="unsigned num_cpus()"){
			currInterface.clear();
			currInterface.append("unsigned num_cpus()");
		}
		break;
	case 12:
		/*if(operationCombo->itemText(12)=="unsigned num_cpus()"){
			currInterface.clear();
			currInterface.append("unsigned num_cpus()");
		}*/
		if(operationCombo->itemText(12)=="void affinity(Thread_capability thread, unsigned cpu)"){
			currInterface.clear();
			currInterface.append("void affinity(Thread_capability thread, unsigned cpu)");
		}
		break;
	/*case 13:
		if(operationCombo->itemText(13)=="void affinity(Thread_capability thread, unsigned cpu)"){
			currInterface.clear();
			currInterface.append("void affinity(Thread_capability thread, unsigned cpu)");
		}
		break;*/
	default:
		currInterface.clear();
		currInterface.append("Rom_dataspace_capability dataspace()");
	}
}

void CommServDialog::testButtonClicked() {  
	try{
			//*******ROM*******
			if(currInterface=="Rom_dataspace_capability dataspace()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试1: qt_launchpad");
				Genode::Rom_connection rom_session("qt_launchpad");
				Genode::Native_capability native=rom_session.dataspace();
				resultOutput->append("Capability ID: "+QString::number(native.idx()->id()));
				resultOutput->append("\n测试2: hahaha");
				Genode::Rom_connection rom_session2("hahaha");
				Genode::Native_capability native2=rom_session2.dataspace();
				resultOutput->append("Capability ID: "+QString::number(native2.idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void sigh(Signal_context_capability sigh)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				//执行测试 begin				
				Genode::Rom_connection rom_session("qt_launchpad");
				Genode::Signal_connection signal_session;
				Genode::Signal_context_capability sigConCapability = signal_session.alloc_context(56123);
				resultOutput->append("Signal context capability ID: "+QString::number(sigConCapability.idx()->id()));
				rom_session.sigh(sigConCapability);
				resultOutput->append("Register signal handler successfully.");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******ROM*******

			//*******IO_MEM*******
			else if(currInterface=="Io_mem_dataspace_capability dataspace()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试1: qt_launchpad");
				Genode::Rom_connection Io_mem_session("qt_launchpad");
				Genode::Native_capability native=Io_mem_session.dataspace();
				resultOutput->append("Capability ID: "+QString::number(native.idx()->id()));
				resultOutput->append("\n测试2: hahaha");
				Genode::Rom_connection Io_mem_session2("hahaha");
				Genode::Native_capability native2=Io_mem_session2.dataspace();
				resultOutput->append("Capability ID: "+QString::number(native2.idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******IO_MEM*******

			//*******IO_PORT*******
			else if(currInterface=="unsigned char inb(unsigned short address)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试:");
				Genode::Io_port_connection Io_port_session(3535,65535);
				unsigned char native=Io_port_session.inb(65530);
				resultOutput->append("Data: "+QString(native));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="unsigned short inw(unsigned short address)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试:");
				Genode::Io_port_connection Io_port_session(3535,65535);
				unsigned short native=Io_port_session.inw(65530);
				resultOutput->append("Data: "+QString::number(native));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="unsigned inl(unsigned short address)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试:");
				Genode::Io_port_connection Io_port_session(3535,65535);
				unsigned native=Io_port_session.inl(65530);
				resultOutput->append("Data: "+QString::number(native));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void outb(unsigned short address, unsigned char value)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void outw(unsigned short address, unsigned short value)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试:");
				Genode::Io_port_connection Io_port_session(3535,65535);
				Io_port_session.outw(65530,65535);
				resultOutput->append("65530,65535");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void outl(unsigned short address, unsigned value)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试:");
				Genode::Io_port_connection Io_port_session(3535,65535);
				Io_port_session.outl(65530,65535);
				resultOutput->append("65530,65535");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******IO_PORT*******

			//*******RAM*******
			else if(currInterface=="int ref_account(Ram_session_capability ram_session_cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Ram_connection ram;
				Genode::size_t         ram_quota;
				Genode::size_t session_donations = Genode::Rm_connection::RAM_QUOTA+Genode::Cpu_connection::RAM_QUOTA +Genode::Ram_connection::RAM_QUOTA;

				if (ram_quota > session_donations)
					ram_quota -= session_donations;
				else ram_quota = 0;
				int output1;
				output1=ram.ref_account(Genode::env()->ram_session_cap());
				
				resultOutput->append("return of ref_account: "+QString::number(output1));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="int transfer_quota(Ram_session_capability ram_session_cap,size_t amount)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Ram_connection ram;
				Genode::size_t         ram_quota;
				Genode::size_t session_donations = Genode::Rm_connection::RAM_QUOTA+Genode::Cpu_connection::RAM_QUOTA +Genode::Ram_connection::RAM_QUOTA;

				if (ram_quota > session_donations)
					ram_quota -= session_donations;
				else ram_quota = 0;
				int output2;
				output2=Genode::env()->ram_session()->transfer_quota(ram.cap(), ram_quota);
				
				resultOutput->append("return of transfer_quota: "+QString::number(output2));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="Ram_dataspace_capability alloc(size_t size)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				Genode::Native_capability native;
				Genode::size_t sizeofalloc=1024*1024;
				native=Genode::env()->ram_session()->alloc(sizeofalloc);
				resultOutput->append("Capability ID after: "+QString::number(native.idx()->id()));
				//执行测试 end				
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void free(Ram_dataspace_capability ds_cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Ram_dataspace_capability native;
				Genode::size_t sizeofalloc=1024*1024;
				native=Genode::env()->ram_session()->alloc(sizeofalloc);
				Genode::env()->ram_session()->free(native);	
				resultOutput->append("Capability ID after: "+QString::number(native.idx()->id()));			
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="size_t quota()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Ram_connection ram;
				Genode::size_t         ram_quota;
				Genode::size_t session_donations = Genode::Rm_connection::RAM_QUOTA+Genode::Cpu_connection::RAM_QUOTA +Genode::Ram_connection::RAM_QUOTA;

				if (ram_quota > session_donations)
					ram_quota -= session_donations;
				else ram_quota = 0;

				resultOutput->append("return of quota: "+QString::number(Genode::env()->ram_session()->quota()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="size_t used()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Ram_connection ram;
				Genode::size_t         ram_quota;
				Genode::size_t session_donations = Genode::Rm_connection::RAM_QUOTA+Genode::Cpu_connection::RAM_QUOTA +Genode::Ram_connection::RAM_QUOTA;

				if (ram_quota > session_donations)
					ram_quota -= session_donations;
				else ram_quota = 0;

				resultOutput->append("return of used: "+QString::number(Genode::env()->ram_session()->used()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******RAM*******

			//*******LOG*******
			else if(currInterface=="size_t write(const char *string)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin				
				Genode::Log_connection log_session;
				resultOutput->append("Input: OS5 hahaha.");
				unsigned long native=log_session.write("OS5 hahaha.");
				resultOutput->append("Size of Log: "+QString::number(native));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******LOG*******

			//*******Signal*******
			else if(currInterface=="Signal_source_capability signal_source()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Signal_connection signal_session;
				Genode::Signal_source_capability sigSrcCapability = signal_session.signal_source();
				resultOutput->append("Signal source capability ID: "+QString::number(sigSrcCapability.idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="Signal_context_capability alloc_context(long imprint)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Signal_connection signal_session;
				Genode::Signal_context_capability sigConCapability = signal_session.alloc_context(5726492);
				resultOutput->append("Signal context capability ID: "+QString::number(sigConCapability.idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void free_context(Signal_context_capability cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Signal_connection signal_session;
				Genode::Signal_context_capability sigConCapability = signal_session.alloc_context(56123);
				resultOutput->append("Signal context capability ID: "+QString::number(sigConCapability.idx()->id()));
				signal_session.free_context(sigConCapability);
				resultOutput->append("The Signal context capability has been freed.");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void submit(Signal_context_capability context,unsigned cnt = 1)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Signal_connection signal_session;
				Genode::Signal_context_capability sigConCapability = signal_session.alloc_context(56123);
				resultOutput->append("Signal context capability ID: "+QString::number(sigConCapability.idx()->id()));
				signal_session.submit(sigConCapability);
				resultOutput->append("Submit one signal at once to this signal context.");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******Signal*******

			//*******PD*******
			else if(currInterface=="int bind_thread(Thread_capability thread)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin				
				Genode::Cpu_connection cpu_session;
				Genode::Pd_connection pd_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				int native = pd_session.bind_thread(threadCapability);
				resultOutput->append("Return value of bind_thread: "+QString::number(native));
				resultOutput->append("Bind thread successfully.");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="int assign_parent(Parent_capability parent)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******PD*******

			//*******RM*******
			else if(currInterface=="Local_addr attach(Dataspace_capability ds, size_t size = 0, off_t offset = 0, bool use_local_addr = false, Local_addr local_addr = (void *)0, bool executable = false)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Rm_connection Rm_session;
				Genode::Dataspace_capability ds_cap=Rm_session.dataspace();
				Rm_session.attach(ds_cap,0,0,false,(void *)0,false);
				resultOutput->append("Capability ID: "+QString::number(ds_cap.idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void detach(Local_addr local_addr)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试1:");
				Genode::Rm_connection Rm_session;
				Genode::Dataspace_capability ds_cap=Rm_session.dataspace();
				Rm_session.detach(Rm_session.attach(ds_cap,0,0,false,(void *)0,false));
				resultOutput->append("Capability ID: "+QString::number(ds_cap.idx()->id()));
				resultOutput->append("测试2:\nAddress: 27101e56");
				Genode::Rm_connection Rm_session2;
				Genode::Dataspace_capability ds_cap2=Rm_session2.dataspace();
				Rm_session.detach((void *)655367766);
				resultOutput->append("No attachment at 27101e56.");
				//resultOutput->append("Address of the Capability: "+QString::number(addrResult));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="Pager_capability add_client(Thread_capability thread)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Rm_connection rm_session;
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				Genode::Pager_capability pagerCapability = rm_session.add_client(threadCapability);
				resultOutput->append("Add thread to pager successful. Thread capability ID:"+QString::number(threadCapability.idx()->id()));
				resultOutput->append("Pager Capability ID: "+QString::number(pagerCapability.idx()->id()));
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void remove_client(Pager_capability)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Rm_connection rm_session;
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread 2");
				Genode::Pager_capability pagerCapability = rm_session.add_client(threadCapability);
				resultOutput->append("Client's Thread capability ID:"+QString::number(threadCapability.idx()->id()));
				resultOutput->append("Pager Capability ID: "+QString::number(pagerCapability.idx()->id()));
				cpu_session.kill_thread(threadCapability);
				resultOutput->append("Client Thread has been removed.");
				rm_session.remove_client(pagerCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void fault_handler(Signal_context_capability handler)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Rm_connection rm_session;
				Genode::Signal_connection signal_session;
				Genode::Signal_context_capability handler = signal_session.alloc_context(56123);
				resultOutput->append("Signal context capability ID: "+QString::number(handler.idx()->id()));
				rm_session.fault_handler(handler);
				resultOutput->append("Register the signal handler successfully.");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="State state()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Rm_connection rm_session;
				Genode::Rm_session::State state = rm_session.state();
				if(state.type == Genode::Rm_session::READ_FAULT)
					resultOutput->append("state: READ_FAULT\n\n");
				else if(state.type == Genode::Rm_session::WRITE_FAULT)
					resultOutput->append("state: WRITE_FAULT\n\n");
				else if(state.type == Genode::Rm_session::EXEC_FAULT)
					resultOutput->append("state: EXEC_FAULT\n\n");
				else if(state.type == Genode::Rm_session::READY)
					resultOutput->append("state: READY\n\n");
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="Dataspace_capability dataspace()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Rm_connection Rm_session;
				Genode::Dataspace_capability ds_cap=Rm_session.dataspace();
				resultOutput->append("Capability ID: "+QString::number(ds_cap.idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******RM*******

			//*******CAP*******
			else if(currInterface=="Capability alloc(Capability ep_cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试: RAM session capability");
				Genode::Cap_connection cap_session;
				Genode::Native_capability native=cap_session.alloc(Genode::env()->ram_session_cap());
				resultOutput->append("Capability ID: "+QString::number(native.idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void free(Capability cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试: RAM session capability");
				Genode::Cap_connection cap_session;
				cap_session.free(Genode::env()->ram_session_cap());
				resultOutput->append("Capability ID: "+QString::number(Genode::env()->ram_session_cap().idx()->id()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******CAP*******

			//*******CPU*******
			else if(currInterface=="Thread_capability create_thread(Name const &name,addr_t utcb = 0)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				resultOutput->append("Thread Capability ID: "+QString::number(threadCapability.idx()->id()));
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="Ram_dataspace_capability utcb(Thread_capability thread)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread 1");
				Genode::Ram_dataspace_capability ramDataspaceCapability = cpu_session.utcb(threadCapability);

				cpu_session.kill_thread(threadCapability);
				//resultOutput->append("Ram dataspace Capability ID: ");
					

				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void kill_thread(Thread_capability thread_cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				resultOutput->append("Thread Capability ID: "+QString::number(threadCapability.idx()->id()));
				cpu_session.kill_thread(threadCapability);
				resultOutput->append("kill thread successfully.");

				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="int set_pager(Thread_capability thread_cap,Pager_capability pager_cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Rm_connection rm_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				Genode::Pager_capability pagerCapability = rm_session.add_client(threadCapability);
				resultOutput->append("set pager: "+QString::number(cpu_session.set_pager(threadCapability,pagerCapability)));
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="int cancel_blocking(Thread_capability thread_cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				cpu_session.cancel_blocking(threadCapability);
				resultOutput->append("Thread cancel blocking. Thread Capability ID: "+QString::number(threadCapability.idx()->id()));
				//resultOutput->append("return value: "+QString::number(cpu_session.cancel_blocking(threadCapability)));
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="int start(Thread_capability thread_cap,addr_t ip, addr_t sp)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void pause(Thread_capability thread)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				cpu_session.pause(threadCapability);
				resultOutput->append("pause thread successfully. thread capability id:"+QString::number(threadCapability.idx()->id()));
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void resume(Thread_capability thread)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				cpu_session.resume(threadCapability);
				resultOutput->append("resume thread successfully. thread capability id:"+QString::number(threadCapability.idx()->id()));
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="Thread_state state(Thread_capability thread_cap)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				Genode::Thread_state threadState = cpu_session.state(threadCapability);
				resultOutput->append("gate capability id of the thread:"+QString::number(threadState.id));
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void state(Thread_capability thread,Thread_state const &state)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				Genode::Thread_state threadState = cpu_session.state(threadCapability);
				
				resultOutput->append("gate capability id of the thread:"+QString::number(threadState.id));
				cpu_session.state(threadCapability,threadState);
				resultOutput->append("Override the current state of  this thread.");
				cpu_session.kill_thread(threadCapability);
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void exception_handler(Thread_capability thread, Signal_context_capability handler)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin				
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				Genode::Signal_connection signal_session;
				Genode::Signal_context_capability sigConCapability = signal_session.alloc_context(5726492);
				resultOutput->append("Thread Capability ID: "+QString::number(threadCapability.idx()->id()));
				resultOutput->append("Signal Context Capability ID: "+QString::number(sigConCapability.idx()->id()));
				cpu_session.exception_handler(threadCapability,sigConCapability);
				cpu_session.kill_thread(threadCapability);

				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void single_step(Thread_capability, bool)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				resultOutput->append("测试1: ");			
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("qt_launchpad");
				resultOutput->append("Thread Capability ID: "+QString::number(threadCapability.idx()->id()));
				cpu_session.single_step(threadCapability,true);
				cpu_session.kill_thread(threadCapability);
				resultOutput->append("测试2: ");
				Genode::Cpu_connection cpu_session2;
				Genode::Thread_capability threadCapability2 = cpu_session2.create_thread("qt_launchpad");
				resultOutput->append("Thread Capability ID: "+QString::number(threadCapability2.idx()->id()));
				cpu_session2.single_step(threadCapability2,false);
				cpu_session2.kill_thread(threadCapability2);


				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="unsigned num_cpus()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin
				Genode::Cpu_connection cpu_session;
				resultOutput->append("Tnumber of  CPUs: "+QString::number(cpu_session.num_cpus()));
				//执行测试 end
				resultOutput->append("\n\n");
			}
			else if(currInterface=="void affinity(Thread_capability thread, unsigned cpu)"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin				
				Genode::Cpu_connection cpu_session;
				Genode::Thread_capability threadCapability = cpu_session.create_thread("new thread");
				resultOutput->append("Thread Capability ID: "+QString::number(threadCapability.idx()->id()));
				cpu_session.affinity(threadCapability,1);
				cpu_session.kill_thread(threadCapability);

				//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******CPU*******

			//*******IRQ*******			
			else if(currInterface=="void wait_for_irq()"){
				resultOutput->setText("");
				resultOutput->append(currInterface);
				resultOutput->append("\n");
				//执行测试 begin//执行测试 end
				resultOutput->append("\n\n");
			}
			//*******IRQ*******
	}catch(...)
	{
		resultOutput->append("接口测试失败！");
	}	
}

void CommServDialog::crossClicked(){
	PERR("close");
	exit(0);
}

    void CommServDialog::mousePressEvent(QMouseEvent *e)
    {
        m_old_pos = e->pos();
        m_mouse_down = e->button() == Qt::LeftButton;
    }
    
    void CommServDialog::mouseMoveEvent(QMouseEvent *e)
    {
        int x = e->x();
        int y = e->y();
        
        if (m_mouse_down) {
            int dx = x - m_old_pos.x();
            int dy = y - m_old_pos.y();
            
            QRect g = geometry();
            
            if (left)
                g.setLeft(g.left() + dx);
            if (right)
                g.setRight(g.right() + dx);
            if (bottom)
                g.setBottom(g.bottom() + dy);
            
            setGeometry(g);
            
            m_old_pos = QPoint(!left ? e->x() : m_old_pos.x(), e->y());
        } else {
            QRect r = rect();
            left = qAbs(x - r.left()) <= 5;
            right = qAbs(x - r.right()) <= 5;
            bottom = qAbs(y - r.bottom()) <= 5;
            bool hor = left | right;
            
            if (hor && bottom) {
                if (left)
                    setCursor(Qt::SizeBDiagCursor);
                else 
                    setCursor(Qt::SizeFDiagCursor);
            } else if (hor) {
                setCursor(Qt::SizeHorCursor);
            } else if (bottom) {
                setCursor(Qt::SizeVerCursor);
            } else {
                setCursor(Qt::ArrowCursor);
            }
        }
    }
    
    void CommServDialog::mouseReleaseEvent(QMouseEvent *e)
    {
        m_mouse_down = false;
    }

    TitleBar::TitleBar(QWidget *parent)
    {
        // Don't let this widget inherit the parent's backround color
        setAutoFillBackground(true);
        // Use a brush with a Highlight color role to render the background 
        setBackgroundRole(QPalette::Highlight);
        
        minimize = new QToolButton(this);
        maximize = new QToolButton(this);
        close= new QToolButton(this);
        
        // Use the style to set the button pixmaps
        QPixmap pix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
        close->setIcon(pix);
        
        maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
        maximize->setIcon(maxPix);

        pix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
        minimize->setIcon(pix);

        restorePix = style()->standardPixmap(QStyle::SP_TitleBarNormalButton);
        
        minimize->setMinimumHeight(20);
        close->setMinimumHeight(20);
        maximize->setMinimumHeight(20);

        maximize->setEnabled(false);
        minimize->setEnabled(false);
        
        
        QLabel *label = new QLabel(this);
        label->setText("系统基本服务接口测试程序");
        parent->setWindowTitle("系统基本服务接口测试程序");

        QLabel *iconlabel = new QLabel(this);
	QPixmap apppix(":/appicon/funtest.png");
        iconlabel->setPixmap(apppix);
	iconlabel->setFixedWidth(40);

        QLabel *label2 = new QLabel(this);
        label2->setText("\t\t\t\t\t\t");

        QHBoxLayout *hbox = new QHBoxLayout(this);
        
        hbox->addWidget(iconlabel);
        hbox->addWidget(label);
        hbox->addWidget(label2);
        hbox->addWidget(minimize);
        hbox->addWidget(maximize);
        hbox->addWidget(close);
        
        hbox->insertStretch(1, 500);
        hbox->setSpacing(0);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        
        maxNormal = false;
	QObject::connect(close,SIGNAL(clicked()),parent,SLOT(crossClicked()));
        //connect(close, SIGNAL( clicked() ), parent, SLOT(quit() ) );
        connect(minimize, SIGNAL( clicked() ), this, SLOT(showSmall() ) );
        connect(maximize, SIGNAL( clicked() ), this, SLOT(showMaxRestore() ) );
    }


    void TitleBar::showSmall()
    {
        parentWidget()->showMinimized();
    }
    
    void TitleBar::showMaxRestore()
    {
        if (maxNormal) {
            parentWidget()->showNormal();
            maxNormal = !maxNormal;
            maximize->setIcon(maxPix);
        } else {
            parentWidget()->showMaximized();
            maxNormal = !maxNormal;
            maximize->setIcon(restorePix);
        }
    }

    void TitleBar::mousePressEvent(QMouseEvent *me)
    {
        startPos = me->globalPos();
        clickPos = mapToParent(me->pos());
    }
    void TitleBar::mouseMoveEvent(QMouseEvent *me)
    {
        if (maxNormal)
            return;
        parentWidget()->move(me->globalPos() - clickPos);
    }
