#ifndef _INCLUDE__LEAK_SESSION__LEAK_SESSION_H_
#define _INCLUDE__LEAK_SESSION__LEAK_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace Leak {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "Leak"; }
		virtual int getCapID() = 0;

		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_getCapID, int, getCapID);
		GENODE_RPC_INTERFACE(Rpc_getCapID);
	};
}

#endif 
