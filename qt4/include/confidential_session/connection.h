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

#ifndef _INCLUDE__CONFIDENTIAL_SESSION__CONNECTION_H_
#define _INCLUDE__CONFIDENTIAL_SESSION__CONNECTION_H_

#include <confidential_session/client.h>
#include <base/connection.h>

namespace Confidential {

	struct Connection : Genode::Connection<Session>, Session_client
	{
		Connection()
		:
			/* create session */
			Genode::Connection<Confidential::Session>(session("foo, ram_quota=4K")),

			/* initialize RPC interface */
			Session_client(cap()) { }
		int read_state()
		{
			try
			{
				return Session_client::read_state();
			}
			catch(int)
			{
				PERR("connection error,so we rebuild it");
				SetNativeCapabilty(session("foo, ram_quota=4K"));
				return Session_client::read_state();
			}			
		}
		void write_state(int id)
		{
			try
			{
				Session_client::write_state(id);
			}
			catch(int)
			{
				PERR("connection error,so we rebuild it");
				SetNativeCapabilty(session("foo, ram_quota=4K"));
				Session_client::write_state(id);
			}
			//call<Rpc_write_state>(id);
		}
		int change_state(int id)
		{
			try
			{
				return Session_client::change_state(id);
			}
			catch(int)
			{
				PERR("connection error,so we rebuild it");
				SetNativeCapabilty(session("foo, ram_quota=4K"));
				return Session_client::change_state(id);
			}
			//return call<Rpc_change_state>(id);
		}
	};
}

#endif /* _INCLUDE__SAKSERVER_SESSION__CONNECTION_H_ */
