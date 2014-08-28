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

#ifndef _INCLUDE__BACK_SESSION_H__CLIENT_H_
#define _INCLUDE__BACK_SESSION_H__CLIENT_H_

#include <back_session/back_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace MSG {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }

		void set_msg(int op,int arg )
		{
			 call<Rpc_set_msg>(op,arg);
		}
		
		bool get_msg(int& op,int &arg)
		{
			return call<Rpc_get_msg>(op,arg);
		}
	};
}

#endif /* _INCLUDE__SAKSERVER_SESSION_H__CLIENT_H_ */
