/*
 * \brief  Interface definition of the Hello service
 * \author Björn Döbel
 * \date   2008-03-20
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__HIGHLEVEl_SESSION__HIGHLEVEl_SESSION_H_
#define _INCLUDE__HIGHLEVEl_SESSION__HIGHLEVEl_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace Highlevel {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "Highlevel"; }

		virtual int read_state() = 0;
		virtual void write_state(int id) = 0;
		virtual int change_state(int a) = 0;


		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_read_state, int, read_state);
		GENODE_RPC(Rpc_write_state, void, write_state,int);
		GENODE_RPC(Rpc_change_state, int, change_state,int);

		GENODE_RPC_INTERFACE(Rpc_read_state,Rpc_write_state,Rpc_change_state);
	};
}

#endif /* _INCLUDE__SAKSERVER_SESSION__SAKSERVER_SESSION_H_ */
