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

#ifndef _INCLUDE__CONTROL_SESSION__CONTROL_SESSION_H_
#define _INCLUDE__CONTROL_SESSION__CONTROL_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace Control {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "Control"; }

		virtual void start() = 0;


		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_start, void, start);

		GENODE_RPC_INTERFACE(Rpc_start);
	};
}

#endif /* _INCLUDE__SAKSERVER_SESSION__SAKSERVER_SESSION_H_ */
