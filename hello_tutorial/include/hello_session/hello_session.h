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

#ifndef _INCLUDE__HELLO_SESSION__HELLO_SESSION_H_
#define _INCLUDE__HELLO_SESSION__HELLO_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace Hello {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "Hello"; }

		virtual void say_hello() = 0;
		virtual int get_id() = 0;
		virtual void set_id(int id) = 0;
		virtual int add(int a, int b) = 0;


		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_say_hello, void, say_hello);
		GENODE_RPC(Rpc_get_id, int, get_id);
		GENODE_RPC(Rpc_set_id, void, set_id,int);
		GENODE_RPC(Rpc_add, int, add, int, int);

		GENODE_RPC_INTERFACE(Rpc_say_hello,Rpc_get_id,Rpc_set_id,Rpc_add);
	};
}

#endif /* _INCLUDE__HELLO_SESSION__HELLO_SESSION_H_ */
