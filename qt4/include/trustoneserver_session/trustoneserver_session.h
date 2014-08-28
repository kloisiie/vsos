/*
 * \brief  Interface definition of the Hello service
 * \author Björn Döbel
 * \date   2008-03-20
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__TRUSTONESERVER_SESSION__TRUSTONESERVER_SESSION_H_
#define _INCLUDE__TRUSTONESERVER_SESSION__TRUSTONESERVER_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace TrustONEserver {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "TrustONEserver"; }

		virtual void trust_one() = 0;
		virtual int add(int a, int b) = 0;


		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_trust_one, void, trust_one);
		GENODE_RPC(Rpc_add, int, add, int, int);

		GENODE_RPC_INTERFACE(Rpc_trust_one, Rpc_add);
	};
}

#endif /* _INCLUDE__TRUSTONESERVER_SESSION__TRUSTONESERVER_SESSION_H_ */
