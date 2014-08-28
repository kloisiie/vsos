/*
 * \brief  NOVA-specific implementation of the Thread API for core
 * \author Norman Feske
 * \author Sebastian Sumpf
 * \author Alexander Boettcher
 * \date   2010-01-19
 */

/*
 * Copyright (C) 2010-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/thread.h>
#include <base/cap_sel_alloc.h>
#include <base/printf.h>
#include <base/sleep.h>
#include <base/env.h>

/* NOVA includes */
#include <nova/syscalls.h>

/* core includes */
#include <nova_util.h>
#include <platform_pd.h>

using namespace Genode;


void Thread_base::_init_platform_thread()
{
	/*
	 * This function is called for constructing server activations and pager
	 * objects. It allocates capability selectors for the thread's execution
	 * context and a synchronization-helper semaphore needed for 'Lock'.
	 */
	using namespace Nova;

	_tid.ec_sel     = cap_selector_allocator()->alloc();
	_tid.exc_pt_sel = cap_selector_allocator()->alloc(NUM_INITIAL_PT_LOG2);
	addr_t pd_sel   = Platform_pd::pd_core_sel();

	/* create running semaphore required for locking */
	addr_t rs_sel =_tid.exc_pt_sel + SM_SEL_EC;
	uint8_t res = create_sm(rs_sel, pd_sel, 0);
	if (res != NOVA_OK) {
		PERR("create_sm returned %u", res);
		throw Cpu_session::Thread_creation_failed();
	}
}


void Thread_base::_deinit_platform_thread()
{
	unmap_local(Nova::Obj_crd(_tid.ec_sel, 0));
	unmap_local(Nova::Obj_crd(_tid.exc_pt_sel, Nova::NUM_INITIAL_PT_LOG2));

	cap_selector_allocator()->free(_tid.ec_sel, 0);
	cap_selector_allocator()->free(_tid.exc_pt_sel,
	                               Nova::NUM_INITIAL_PT_LOG2);

	/* revoke utcb */
	Nova::Rights rwx(true, true, true);
	addr_t utcb = reinterpret_cast<addr_t>(&_context->utcb);
	Nova::revoke(Nova::Mem_crd(utcb >> 12, 0, rwx));
}


void Thread_base::start()
{
	/*
	 * On NOVA, core almost never starts regular threads. This simply creates a
	 * local EC
	 */
	using namespace Nova;

	addr_t sp = reinterpret_cast<addr_t>(&_context->stack[-4]);
	addr_t utcb = reinterpret_cast<addr_t>(&_context->utcb);
	Utcb * utcb_obj = reinterpret_cast<Utcb *>(&_context->utcb);
	addr_t pd_sel = Platform_pd::pd_core_sel();

	/* create local EC */
	enum { CPU_NO = 0, GLOBAL = false };
	uint8_t res = create_ec(_tid.ec_sel, pd_sel, CPU_NO,
	                        utcb, sp, _tid.exc_pt_sel, GLOBAL);
	if (res != NOVA_OK) {
		PERR("create_ec returned %d", res);
		throw Cpu_session::Thread_creation_failed();
	}

	/* default: we don't accept any mappings or translations */
	utcb_obj->crd_rcv = Obj_crd();
	utcb_obj->crd_xlt = Obj_crd();

	if (map_local(reinterpret_cast<Nova::Utcb *>(Thread_base::myself()->utcb()),
	              Obj_crd(PT_SEL_PAGE_FAULT, 0),
	              Obj_crd(_tid.exc_pt_sel + PT_SEL_PAGE_FAULT, 0))) {
		PERR("could not create page fault portal");
		throw Cpu_session::Thread_creation_failed();
	}
}


void Thread_base::cancel_blocking()
{
	using namespace Nova;

	if (sm_ctrl(_tid.exc_pt_sel + SM_SEL_EC, SEMAPHORE_UP))
		nova_die();
}
