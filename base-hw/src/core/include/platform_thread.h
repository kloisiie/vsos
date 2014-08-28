/*
 * \brief   Userland interface for the management of kernel thread-objects
 * \author  Martin Stein
 * \date    2012-02-02
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CORE__INCLUDE__PLATFORM_THREAD_H_
#define _CORE__INCLUDE__PLATFORM_THREAD_H_

/* Genode includes */
#include <ram_session/ram_session.h>
#include <base/native_types.h>
#include <kernel/syscalls.h>
#include <kernel/log.h>
#include <base/thread.h>
#include <kernel/thread.h>

/* core includes */
#include <assert.h>
#include <address_space.h>

namespace Genode {

	class Pager_object;
	class Thread_state;
	class Thread_base;
	class Rm_client;
	class Platform_thread;

	/**
	 * Userland interface for the management of kernel thread-objects
	 */
	class Platform_thread
	{
		enum { NAME_MAX_LEN = 32 };

		Thread_base *            _thread_base;
		size_t                   _stack_size;
		unsigned                 _pd_id;
		Weak_ptr<Address_space>  _address_space;
		unsigned                 _id;
		Rm_client *              _rm_client;
		bool                     _main_thread;
		Native_utcb *            _phys_utcb;
		Native_utcb *            _virt_utcb;
		Tlb *                    _tlb;
		Ram_dataspace_capability _utcb;
		char                     _name[NAME_MAX_LEN];
		char                     _kernel_thread[sizeof(Kernel::Thread)];

		/**
		 * Common construction part
		 */
		void _init();

		/*
		 * Check if this thread will attach its UTCB by itself
		 */
		bool _attaches_utcb_by_itself();

		public:

			/**
			 * Constructor for core threads
			 */
			Platform_thread(const char * name, Thread_base * const thread_base,
			                size_t const stack_size, unsigned const pd_id);

			/**
			 * Constructor for threads outside of core
			 */
			Platform_thread(const char * name, unsigned const priority,
			                addr_t const utcb);

			/**
			 * Destructor
			 */
			~Platform_thread();

			/**
			 * Join PD identified by 'pd_id'
			 *
			 * \param pd_id        ID of targeted PD
			 * \param main_thread  wether we are the main thread in this PD
			 *
			 * \retval  0  on success
			 * \retval <0  otherwise
			 */
			int join_pd(unsigned const pd_id, bool const main_thread,
			            Weak_ptr<Address_space> address_space);

			/**
			 * Run this thread
			 */
			int start(void * ip, void * sp, unsigned int cpu_no = 0);

			/**
			 * Pause this thread
			 */
			void pause() { Kernel::pause_thread(_id); }

			/**
			 * Resume this thread
			 */
			void resume() { Kernel::resume_thread(_id); }

			/**
			 * Cancel currently blocking operation
			 */
			void cancel_blocking() { resume(); }

			/**
			 * Get raw thread state
			 */
			Thread_state state()
			{
				Kernel::read_thread_state(id());
				return *(Thread_state *)Thread_base::myself()->utcb()->base();
			};

			/**
			 * Override raw thread state
			 */
			void state(Thread_state s)
			{
				*(Thread_state *)Thread_base::myself()->utcb()->base() = s;
				Kernel::write_thread_state(id());
			};

			/**
			 * Return unique identification of this thread as faulter
			 */
			unsigned pager_object_badge() { return _id; }

			/**
			 * Set the executing CPU for this thread
			 */
			void affinity(unsigned cpu) {
				kernel_log() << __PRETTY_FUNCTION__ << ": not implemented\n"; };

			/**
			 * Return the address space to which the thread is bound
			 */
			Weak_ptr<Address_space> address_space();


			/***************
			 ** Accessors **
			 ***************/

			inline char const * name() const { return _name; }

			void pager(Pager_object * const pager);

			Pager_object * pager();

			unsigned pd_id() const { return _pd_id; }

			Native_thread_id id() const { return _id; }

			size_t stack_size() const { return _stack_size; }

			Thread_base * thread_base()
			{
				if (!_thread_base) assert(_main_thread);
				return _thread_base;
			}

			Native_utcb * phys_utcb() const { return _phys_utcb; }

			Native_utcb * virt_utcb() const { return _virt_utcb; }

			Ram_dataspace_capability utcb() const { return _utcb; }

			Tlb * tlb() const { return _tlb; }
	};
}

#endif /* _CORE__INCLUDE__PLATFORM_THREAD_H_ */

