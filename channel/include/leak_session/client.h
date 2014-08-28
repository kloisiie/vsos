#ifndef _INCLUDE__LEAK_SESSION_H__CLIENT_H_
#define _INCLUDE__LEAK_SESSION_H__CLIENT_H_

#include <leak_session/leak_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace Leak {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }


		int getCapID()
		{
			return call<Rpc_getCapID>();
		}
	};
}

#endif /* _INCLUDE__HELLO_SESSION_H__CLIENT_H_ */
