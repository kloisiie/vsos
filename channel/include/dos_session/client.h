#ifndef _INCLUDE__DOS_SESSION_H__CLIENT_H_
#define _INCLUDE__DOS_SESSION_H__CLIENT_H_

#include <dos_session/dos_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace Dos {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }

		int subr(int a,int b)
		{
			return call<Rpc_subr>(a,b);
		}

		int addi(int a, int b)
		{
			return call<Rpc_addi>(a, b);
		}
	};
}

#endif /* _INCLUDE__HELLO_SESSION_H__CLIENT_H_ */
