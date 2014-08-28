#ifndef _INCLUDE__CALC_SESSION__CALC_SESSION_H_
#define _INCLUDE__CALC_SESSION__CALC_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace Calc {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "Calc"; }
		virtual int sub(int a, int b) = 0;
		virtual int add(int a, int b) = 0;
		virtual int mul(int a, int b) = 0;
		virtual int div(int a, int b) = 0;
		

		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_sub, int, sub, int, int);
		GENODE_RPC(Rpc_add, int, add, int, int);
		GENODE_RPC(Rpc_mul, int, mul, int, int);
		GENODE_RPC(Rpc_div, int, div, int, int);
		GENODE_RPC_INTERFACE(Rpc_sub, Rpc_add,Rpc_mul, Rpc_div);
	};
}

#endif 
