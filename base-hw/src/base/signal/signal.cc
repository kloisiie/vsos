/*
 * \brief  Implementations of the signaling framework specific for HW-core
 * \author Martin Stein
 * \date   2012-05-05
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/thread.h>
#include <base/signal.h>
#include <signal_session/connection.h>
#include <kernel/syscalls.h>

using namespace Genode;


/**
 * Provide a static signal connection
 */
static Signal_connection * signal_connection()
{
	static Signal_connection _object;
	return &_object;
}


/************
 ** Signal **
 ************/

void Signal::_dec_ref_and_unlock()
{
	if (_data.context) {
		Lock::Guard lock_guard(_data.context->_lock);
		_data.context->_ref_cnt--;

		/*
		 * We must ack a signal context to receive the next one,
		 * so new signals are received only when ref_cnt = 0.
		 */
		if (_data.context->_ref_cnt == 0)
			Kernel::ack_signal(_data.context->_cap.dst());
	}
}


void Signal::_inc_ref()
{
	if (_data.context) {
		Lock::Guard lock_guard(_data.context->_lock);
		_data.context->_ref_cnt++;
	}
}


Signal::Signal(Signal::Data data) : _data(data)
{
	/*
	 * We assume that a kernel signal-context doesn't deliver
	 * multiple signals simultaneously.
	 */
	if (_data.context) _data.context->_ref_cnt = 1;
}


/************************
 ** Signal transmitter **
 ************************/

void Signal_transmitter::submit(unsigned cnt)
{
	/* submits to invalid signal contexts get ignored */
	Kernel::submit_signal(_context.dst(), cnt);
}


/*********************
 ** Signal_receiver **
 *********************/

Signal_receiver::Signal_receiver()
{
	/* create a kernel object that corresponds to the receiver */
	bool session_upgraded = 0;
	Signal_connection * const s = signal_connection();
	while (1) {
		try {
			_cap = s->alloc_receiver();
			break;
		} catch (Signal_session::Out_of_metadata)
		{
			/* upgrade session quota and try again, but only once */
			if (session_upgraded) {
				PDBG("Failed to alloc signal receiver");
				break;
			}
			PINF("upgrading quota donation for Signal session");
			env()->parent()->upgrade(s->cap(), "ram_quota=4K");
			session_upgraded = 1;
		}
	}
}


void Signal_receiver::_unsynchronized_dissolve(Signal_context * c)
{
	/*
	 * We first destroy the kernel object. This also ensures
	 * that no delivered but unacked signals of this context exist
	 * in userland anymore.
	 */
	Kernel::kill_signal_context(c->_cap.dst());

	/*
	 * Now we can tell core to regain the memory of the
	 * destructed kernel object.
	 */
	signal_connection()->free_context(c->_cap);

	/* reset the context */
	c->_receiver = 0;
	c->_cap = Signal_context_capability();

	/* forget the context */
	_contexts.remove(&c->_receiver_le);
}


Signal_context_capability Signal_receiver::manage(Signal_context * const c)
{
	/* check if the context is already managed */
	Lock::Guard contexts_guard(_contexts_lock);
	Lock::Guard context_guard(c->_lock);
	if (c->_receiver) throw Context_already_in_use();

	/* create a kernel object that corresponds to the context */
	bool session_upgraded = 0;
	Signal_connection * const s = signal_connection();
	while (1) {
		try {
			c->_cap = s->alloc_context(_cap, (unsigned)c);
			break;
		} catch (Signal_session::Out_of_metadata)
		{
			/* upgrade session quota and try again, but only once */
			PINF("upgrading quota donation for Signal session");
			if (session_upgraded) return Signal_context_capability();
			env()->parent()->upgrade(s->cap(), "ram_quota=4K");
			session_upgraded = 1;
		}
	}
	/* assign the context to us */
	c->_receiver = this;
	_contexts.insert(&c->_receiver_le);
	return c->_cap;
}


void Signal_receiver::dissolve(Signal_context *context)
{
	if (context->_receiver != this)
		throw Context_not_associated();

	Lock::Guard list_lock_guard(_contexts_lock);

	_unsynchronized_dissolve(context);

	/*
	 * We assume that dissolve is always called before the context destructor.
	 * On other platforms a 'context->_destroy_lock' is locked and unlocked at
	 * this point to block until all remaining signals of this context get
	 * destructed and prevent the context from beeing destructed to early.
	 * However on this platform we don't have to wait because
	 * 'kill_signal_context' in '_unsynchronized_dissolve' already does it.
	 */
}


bool Signal_receiver::pending() { return Kernel::signal_pending(_cap.dst()); }


Signal Signal_receiver::wait_for_signal()
{
	/* await a signal */
	Kernel::await_signal(_cap.dst());
	Signal s(*(Signal::Data *)Thread_base::myself()->utcb());
	Signal_context * const c = s.context();

	/* check if the context of the signal is managed by us */
	Lock::Guard context_guard(c->_lock);
	if (c->_receiver != this) {
		PERR("%s: Context not managed by this receiver", __PRETTY_FUNCTION__);
		while (1) ;
	}
	/* check attributes of the signal and return it */
	if (s.num() == 0) PWRN("Returning signal with num == 0");
	return s;
}


void Signal_receiver::local_submit(Signal::Data signal) {
	PDBG("Not implemented"); };

