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

#ifndef _INCLUDE__TRUSTONESERVER_SESSION_H__CLIENT_H_
#define _INCLUDE__TRUSTONESERVER_SESSION_H__CLIENT_H_

#include <trustoneserver_session/trustoneserver_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace TrustONEserver {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }

		void trust_one()
		{
			PDBG("TrustONEserver Hello.");
			call<Rpc_trust_one>();
		}

		int add(int a, int b)
		{
			return call<Rpc_add>(a, b);
		}
	};
}

#endif /* _INCLUDE__TRUSTONESERVER_SESSION_H__CLIENT_H_ */
