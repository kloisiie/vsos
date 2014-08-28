/*
 * \brief  Launchpad child management
 * \author Norman Feske
 * \author Markus Partheymueller
 * \date   2006-09-01
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 * Copyright (C) 2012 Intel Corporation
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/env.h>
#include <base/child.h>
#include <base/sleep.h>
#include <base/service.h>
#include <base/snprintf.h>
#include <base/blocking.h>
#include <rom_session/connection.h>
#include <ram_session/connection.h>
#include <cpu_session/connection.h>
#include <timer_session/connection.h>
#include <launchpad/launchpad.h>

//added by lele 20130918 begin
#include <timer_session/client.h>
#include <timer_session/connection.h>
#include <base/printf.h>
//added by lele 20130918 end

using namespace Genode;


/***************
 ** Launchpad **
 ***************/

Launchpad::Launchpad(unsigned long initial_quota)
:
	_initial_quota(initial_quota),
	_sliced_heap(env()->ram_session(), env()->rm_session())
{
	/* names of services provided by the parent */
	static const char *names[] = {

		/* core services */
		"CAP", "RAM", "RM", "PD", "CPU", "IO_MEM", "IO_PORT",
		"IRQ", "ROM", "LOG", "SIGNAL", "Monitor","backup",

		/* services expected to got started by init */
		"Nitpicker", "Init", "Timer", "PCI", "Block", "Nic", "Rtc","Control","Terminal","File_system","Uart","Framebuffer","Input",

		0 /* null-termination */
	};
	for (unsigned i = 0; names[i]; i++)
		_parent_services.insert(new (env()->heap())
		                            Parent_service(names[i]));
}


/**
 * Check if a program with the specified name already exists
 */
bool Launchpad::_child_name_exists(const char *name)
{
	Launchpad_child *c = _children.first();

	for ( ; c; c = c->List<Launchpad_child>::Element::next())
		if (strcmp(c->name(), name) == 0)
			return true;

	return false;
}


/**
 * Create a unique name based on the filename
 *
 * If a program with the filename as name already exists, we
 * add a counting number as suffix.
 */
void Launchpad::_get_unique_child_name(const char *filename, char *dst, int dst_len)
{

	//changed by malele 20130922 begin--can be ignored
	//PERR("now ignored the lock_guard(_children_lock)\n");
	//PDBG("now go into the lock_guard(_children_lock)\n");
	Lock::Guard lock_guard(_children_lock);
	//PDBG("now exit the lock_guard(_children_lock)\n");
	//malele 20130922 end
	char buf[64];
	char suffix[8];
	suffix[0] = 0;

	for (int cnt = 1; true; cnt++) {

		/* build program name composed of filename and numeric suffix */
		snprintf(buf, sizeof(buf), "%s%s", filename, suffix);

		/* if such a program name does not exist yet, we are happy */
		if (!_child_name_exists(buf)) {
			strncpy(dst, buf, dst_len);
			return;
		}

		/* increase number of suffix */
		snprintf(suffix, sizeof(suffix), ".%d", cnt + 1);
	}
}


Launchpad_child *Launchpad::start_child(const char *filename,
                                        unsigned long ram_quota,
                                        Genode::Dataspace_capability config_ds)
{
	printf("starting '%s' with quota '%ld'\n", filename, ram_quota);

	/* find unique name for new child */
	char unique_name[64];
	//strncpy(unique_name,filename,  sizeof(unique_name));
	_get_unique_child_name(filename, unique_name, sizeof(unique_name));
	//printf("using unique child name \"%s\"\n", unique_name);

	if (ram_quota > env()->ram_session()->avail()) {
		PERR("Child's ram quota is higher than our available quota, using available quota");
		ram_quota = env()->ram_session()->avail() - 256*1000;
	}

	size_t metadata_size = 4096*16 + sizeof(Launchpad_child);

	if (metadata_size > ram_quota) {
		PERR("Too low ram_quota to hold child metadata");
		return 0;
	}

	ram_quota -= metadata_size;

	/* lookup executable elf binary */
	Dataspace_capability  file_cap;
	Rom_session_capability rom_cap;
	try {
		/*
		 * When creating a ROM connection for a non-existing file, the
		 * constructor of 'Rom_connection' throws a 'Parent::Service_denied'
		 * exception.
		 */
		Rom_connection rom(filename, unique_name);
		//PERR("Rom_connection rom(filename, unique_name);");
		rom.on_destruction(Rom_connection::KEEP_OPEN);
		//PERR("rom.on_destruction(Rom_connection::KEEP_OPEN);");
		rom_cap  = rom.cap();
		file_cap = rom.dataspace();
	} catch (...) {
		printf("Error: Could not access file \"%s\" from ROM service.\n", filename);
		return 0;
	}

	/* create ram session for child with some of our own quota */
	Ram_connection ram;
	ram.on_destruction(Ram_connection::KEEP_OPEN);
	//PERR("ram.on_destruction(Ram_connection::KEEP_OPEN);");
	ram.ref_account(env()->ram_session_cap());
	//PERR("ram.ref_account(env()->ram_session_cap());");
	env()->ram_session()->transfer_quota(ram.cap(), ram_quota);
	//PERR("env()->ram_session()->transfer_quota(ram.cap(), ram_quota);");

	/* create cpu session for child */
	Cpu_connection cpu(unique_name);
	cpu.on_destruction(Cpu_connection::KEEP_OPEN);
	//PERR("cpu.on_destruction(Cpu_connection::KEEP_OPEN);");

	if (!ram.cap().valid() || !cpu.cap().valid()) {
		if (ram.cap().valid()) {
			PWRN("Failed to create CPU session");
			env()->parent()->close(ram.cap());
		}
		if (cpu.cap().valid()) {
			PWRN("Failed to create RAM session");
			env()->parent()->close(cpu.cap());
		}
		env()->parent()->close(rom_cap);
		PERR("Our quota is %zd", env()->ram_session()->quota());
		return 0;
	}

	Rm_connection rm;
	rm.on_destruction(Rm_connection::KEEP_OPEN);
	//PERR("rm.on_destruction(Rm_connection::KEEP_OPEN);");
	if (!rm.cap().valid()) {
		PWRN("Failed to create RM session");
		env()->parent()->close(ram.cap());
		env()->parent()->close(cpu.cap());
		env()->parent()->close(rom_cap);
		return 0;
	}

	Launchpad_child *c = new (&_sliced_heap)
		Launchpad_child(unique_name, file_cap, ram.cap(),
		                cpu.cap(), rm.cap(), rom_cap,
		                &_cap_session, &_parent_services, &_child_services,
		                config_ds, this);
	//PERR("Launchpad_child *c = new (&_sliced_heap)");

	Lock::Guard lock_guard(_children_lock);
	//PERR("Lock::Guard lock_guard(_children_lock);");
	_children.insert(c);
	//PERR("_children.insert(c);");

	add_child(unique_name, ram_quota, c, c->heap());
	//PERR("add_child(unique_name, ram_quota, c, c->heap());");
	return c;
}


/**
 * Watchdog-guarded child destruction mechanism
 *
 * During the destruction of a child, all sessions of the child are getting
 * closed. A server, however, may refuse to answer a close call. We detect
 * this case using a watchdog mechanism, unblock the 'close' call, and
 * proceed with the closing the other remaining sessions.
 */
class Child_destructor_thread : Thread<4*4096>
{
	private:

		Launchpad_child *_curr_child;     /* currently destructed child       */
		Allocator       *_curr_alloc;     /* child object'sallocator          */
		Lock             _submit_lock;    /* only one submission at a time    */
		Lock             _activate_lock;  /* submission protocol              */
		bool             _ready;          /* set if submission is completed   */
		int              _watchdog_cnt;   /* watchdog counter in milliseconds */

		/**
		 * Thread entry function
		 */
		void entry() {
			while (true) {
				//PERR("lock");
				/* wait for next submission */
				_activate_lock.lock();
				// PERR("unlock");

				/*
				 * Eventually long-taking operation that involves the
				 * closing of all session of the child. This procedure
				 * may need blocking cancellation to proceed in the
				 * case servers are unresponsive.
				 */
				try {
					//if(_curr_child)
				//	PERR("%lx,%lx",_curr_alloc,_curr_child);
					//_curr_child->destruct_child();
					//_curr_alloc->free(_curr_child,sizeof(Launchpad_child))	;				
					destroy(_curr_alloc, _curr_child);
				} catch (Blocking_canceled) {
					PERR("Suspicious cancellation\n");
				}
				catch(...){
					PERR("ERROR");				
				}
                               // PERR("dfjslkf");
				_ready = true;
				
			}
		}

	public:

		/*
		 * Watchdog timer granularity in milliseconds. This value defined
		 * after how many milliseconds the watchdog is activated.
		 */
		enum { WATCHDOG_GRANULARITY_MS = 10 };

		/**
		 * Constructor
		 */
		Child_destructor_thread() :
			_curr_child(0), _curr_alloc(0),
			_activate_lock(Lock::LOCKED),
			_ready(true)
		{
			start();
		}

		/**
		 * Destruct child, coping with unresponsive servers
		 *
		 * \param alloc       Child object's allocator
		 * \param child       Child to destruct
		 * \param timeout_ms  Maximum destruction time until the destructing
		 *                    thread gets waken up to give up the close call to
		 *                    an unreponsive server.
		 */
		void submit_for_destruction(Allocator *alloc, Launchpad_child *child,
		                            Timer::Session *timer, int timeout_ms)
		{
			/* block until destructor thread is ready for new submission */
			Lock::Guard _lock_guard(_submit_lock);
			//PERR("Lock::Guard _lock_guard(_submit_lock);%lx",_submit_lock);

			/* register submission values */
			_curr_child   = child;
			_curr_alloc   = alloc;
			_ready        = false;
			_watchdog_cnt = 0;

			/* wake up the destruction thread */
			_activate_lock.unlock();
			//PERR("_activate_lock.unlock();%lx",&_activate_lock);

			/*
			 * Now, the destruction thread attempts to close all the
			 * child's sessions. Check '_ready' flag periodically.
			 */
			
			while (!_ready) {

				/* give the destruction thread some time to proceed */
				timer->msleep(WATCHDOG_GRANULARITY_MS);
				//PERR("timer->msleep(WATCHDOG_GRANULARITY_MS);");
				_watchdog_cnt += WATCHDOG_GRANULARITY_MS;
				//PERR("_watchdog_cnt += WATCHDOG_GRANULARITY_MS;");

				/* check if we reached the timeout */
				if (_watchdog_cnt > timeout_ms) {

					/*
					 * The destruction seems to got stuck, let's shake it a
					 * bit to proceed and reset the watchdog counter to give
					 * the next blocking operation a chance to execute.
					 */
					//PWRN("cancel_blocking");
					cancel_blocking();
					//PERR("cancel_blocking();");
					_watchdog_cnt = 0;
					//PERR("_watchdog_cnt = 0;");
				}
			}
			//PERR("ready");
		}
};


/**
 * Construct a timer session for the watchdog timer on demand
 */
static Timer::Session *timer_session()
{
	static Timer::Connection timer;
	return &timer;
}


/**
 * Destruct Launchpad_child, cope with infinitely blocking server->close calls
 *
 * The arguments correspond to the 'Child_destructor_thread::submit_for_destruction'
 * function.
 */
static void destruct_child(Allocator *alloc, Launchpad_child *child,
                           Timer::Session *timer, int timeout)
{
	/* lazily construct child-destructor thread */
	static Child_destructor_thread child_destructor;
	//PWRN("alloc %lx,child %lx,timer %lx",alloc,child,timer);
	/* if no timer session was provided by our caller, we have create one */
	if (!timer)
		timer = timer_session();
	//PWRN("destruct child");
	child_destructor.submit_for_destruction(alloc, child, timer, timeout);
	
}


void Launchpad::exit_child(Launchpad_child *child,
                           Timer::Session  *timer,
                           int              session_close_timeout_ms)
{
	//added by malele 20130918 begin
	
	const char * name=child->name();
	char _name[16];
	memcpy(_name,name,strlen(name));
	//added by malele end		
	remove_child(child->name(), child->heap());
	
	//PERR("remove_child(child->name(), child->heap())");

	Lock::Guard lock_guard(_children_lock);
	_children.remove(child);
	//PERR("_children.remove(child)");
	
	Rm_session_capability   rm_session_cap = child->rm_session_cap();
	//PERR("_children.remove(child)");
	Ram_session_capability ram_session_cap = child->ram_session_cap();
	//PERR("_children.remove(child)");
	Cpu_session_capability cpu_session_cap = child->cpu_session_cap();
	//PERR("_children.remove(child)");
	Rom_session_capability rom_session_cap = child->rom_session_cap();
//	PERR("_children.remove(child)");
	
	const Genode::Server *server = child->server();
	//PERR("_children.remove(child)");
	destruct_child(&_sliced_heap, child, timer, session_close_timeout_ms);
	//PERR("destruct_child(&_sliced_heap, child, timer, session_close_timeout_ms)");
	
	env()->parent()->close(rm_session_cap);
	//PERR(">close(rm_session_cap)");
	env()->parent()->close(cpu_session_cap);
	//PERR(">close(cpu_session_cap)");
	env()->parent()->close(rom_session_cap);
	//PERR(">close(rom_session_cap)");
	env()->parent()->close(ram_session_cap);
	//PERR(">close(ram_session_cap)");

	/*
	 * The killed child may have provided services to other children.
	 * Since the server is dead by now, we cannot close its sessions
	 * in the cooperative way. Instead, we need to instruct each
	 * other child to forget about session associated with the dead
	 * server. Note that the 'child' pointer points a a no-more
	 * existing object. It is only used to identify the corresponding
	 * session. It must never by de-referenced!
	 */
	///*
	//commented by malele 20130918
	Launchpad_child *c = _children.first();
	for ( ; c; c = c->Genode::List<Launchpad_child>::Element::next())
		{c->revoke_server(server); 
	//PERR(">revoke_server[i]");
	}
	//PERR("exit child");
	//*/
	//added by malele 20130918 begin
	//restart after exit
	//Timer::Connection time;
	//int w_mms=1000;
	//PDBG("now wait for %d mms and restart %s\n",w_mms,_name);
	//time.msleep(w_mms);
	//PDBG("exit done.. \n");
	//start_child(_name,1048576, Genode::Dataspace_capability());
	//PDBG("%s restarted\n",_name);
	//added by malele end
	/*Timer::Connection h;
	h.msleep(100);*/
}
