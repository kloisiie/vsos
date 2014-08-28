/*
 * \brief  Connection to Hello service
 * \author Norman Feske
 * \date   2008-11-10
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BACK_SESSION__CONNECTION_H_
#define _INCLUDE__BACK_SESSION__CONNECTION_H_

#include <back_session/client.h>
#include <base/connection.h>

namespace MSG {

	struct Connection : Genode::Connection<Session>, Session_client
	{
		Connection()
		:
			/* create session */
			Genode::Connection<MSG::Session>(session("foo, ram_quota=4K")),

			/* initialize RPC interface */
			Session_client(cap()) { }
	};
		
}

#endif /* _INCLUDE__SAKSERVER_SESSION__CONNECTION_H_ */
