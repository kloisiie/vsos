/*
 * \brief  Helper functions for the Lock implementation
 * \author Martin Stein
 * \date   2011-01-02
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _SRC__BASE__LOCK__LOCK_HELPER_H_
#define _SRC__BASE__LOCK__LOCK_HELPER_H_

/* Genode includes */
#include <base/native_types.h>
#include <base/thread.h>


extern Genode::Native_thread_id main_thread_tid;


/**
 * Yield CPU to any other thread
 */
static inline void thread_yield()
{ Kernel::yield_thread(); }


/**
 * Yield CPU to a specified thread 't'
 */
static inline void
thread_switch_to(Genode::Thread_base *thread_base)
{
	Genode::Native_thread_id t = thread_base ?
	                             thread_base->tid().tid :
	                             main_thread_tid;
	Kernel::yield_thread(t);
}


/**
 * Resume another thread 't' and return if it were paused or not
 */
static inline bool
thread_check_stopped_and_restart(Genode::Thread_base *thread_base)
{
	Genode::Native_thread_id t = thread_base ?
	                             thread_base->tid().tid :
	                             main_thread_tid;
	return Kernel::resume_thread(t) == 0;
}


/**
 * Exclude ourselves from CPU scheduling for now
 */
static inline void thread_stop_myself() { Kernel::pause_thread(); }


#endif /* _SRC__BASE__LOCK__LOCK_HELPER_H_ */

