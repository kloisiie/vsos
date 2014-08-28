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

#ifndef _INCLUDE__LOGINserver_SESSION__LOGINserver_SESSION_H_
#define _INCLUDE__LOGINserver_SESSION__LOGINserver_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace LOGINserver {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "LOGINserver"; }

		virtual void login_finished() = 0;
		virtual int add(int a, int b) = 0;


		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_login_finished, void, login_finished);
		GENODE_RPC(Rpc_add, int, add, int, int);

		GENODE_RPC_INTERFACE(Rpc_login_finished, Rpc_add);
	};
}

#endif /* _INCLUDE__LOGINserver_SESSION__LOGINserver_SESSION_H_ */
