#ifndef _INCLUDE__CALC_SESSION_H__CLIENT_H_
#define _INCLUDE__CALC_SESSION_H__CLIENT_H_

#include <calc_session/calc_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace Calc {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) { }

		int sub(int a,int b)
		{
			return call<Rpc_sub>(a,b);
		}

		int add(int a, int b)
		{
			return call<Rpc_add>(a, b);
		}
		int mul(int a,int b)
		{
			return call<Rpc_mul>(a, b);
		}
		int div(int a,int b){
			return call<Rpc_div>(a, b);		
		}
	};
}

#endif /* _INCLUDE__HELLO_SESSION_H__CLIENT_H_ */
