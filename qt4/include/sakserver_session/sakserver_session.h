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

#ifndef _INCLUDE__SAKSERVER_SESSION__SAKSERVER_SESSION_H_
#define _INCLUDE__SAKSERVER_SESSION__SAKSERVER_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace SAKserver {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "SAKserver"; }

		virtual void say_hello() = 0;
		virtual int add(int a, int b) = 0;


		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_say_hello, void, say_hello);
		GENODE_RPC(Rpc_add, int, add, int, int);

		GENODE_RPC_INTERFACE(Rpc_say_hello, Rpc_add);
	};
}

#endif /* _INCLUDE__SAKSERVER_SESSION__SAKSERVER_SESSION_H_ */
