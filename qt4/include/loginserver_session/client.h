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

#ifndef _INCLUDE__LOGINSERVER_SESSION_H__CLIENT_H_
#define _INCLUDE__LOGINSERVER_SESSION_H__CLIENT_H_

#include <loginserver_session/loginserver_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace LOGINserver {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }

		void login_finished()
		{
			PDBG("LOGINserver Hello.");
			call<Rpc_login_finished>();
		}

		int add(int a, int b)
		{
			return call<Rpc_add>(a, b);
		}
	};
}

#endif /* _INCLUDE__LOGINSERVER_SESSION_H__CLIENT_H_ */
