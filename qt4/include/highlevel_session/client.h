/*
 * \brief  Client-side interface of the SAKserver service
 * \author Wang Shuo
 * \date   2012-09-12
 */

/*
 * Copyright (C) 2008-2013 NFS, ISCAS
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__HIGHLEVEL_SESSION_H__CLIENT_H_
#define _INCLUDE__HIGHLEVEL_SESSION_H__CLIENT_H_

#include <highlevel_session/highlevel_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace Highlevel {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }

		int read_state()
		{
			return call<Rpc_read_state>();
		}
		void write_state(int id)
		{
			call<Rpc_write_state>(id);
		}
		int change_state(int id)
		{
			return call<Rpc_change_state>(id);
		}
	};
}

#endif /* _INCLUDE__HIGHLEVEL_SESSION_H__CLIENT_H_ */
