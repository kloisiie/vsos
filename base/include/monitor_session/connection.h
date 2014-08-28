/*
 * \brief  Connection to Monitor service
 * \author wyq
 */


#ifndef _INCLUDE__MONITOR_SESSION__CONNECTION_H_
#define _INCLUDE__MONITOR_SESSION__CONNECTION_H_

#include <monitor_session/client.h>
#include <base/connection.h>

namespace Genode {

	struct Monitor_Connection : Connection<Monitor_Session>, Monitor_Session_client
	{
		Monitor_Connection()
		:
			/* create session */
			Connection<Monitor_Session>(session("foo, ram_quota=60K")),

			/* initialize RPC interface */
			Monitor_Session_client(cap()) { }
	};
}

#endif /* _INCLUDE__MONITOR_SESSION__CONNECTION_H_ */
