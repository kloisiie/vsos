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

#ifndef _INCLUDE__BACK_SESSION__CONFIDENTIAL_SESSION_H_
#define _INCLUDE__BACK_SESSION__CONFIDENTIAL_SESSION_H_

#include <session/session.h>
#include <base/rpc.h>

namespace MSG {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "backup"; }

		virtual bool get_msg(int &op,int &arg) = 0;
		virtual void set_msg(int op,int arg) = 0;



		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_set_msg,void, set_msg,int ,int);
		GENODE_RPC(Rpc_get_msg,bool, get_msg,int& ,int&);

		GENODE_RPC_INTERFACE(Rpc_set_msg,Rpc_get_msg);
	};
}

#endif /* _INCLUDE__SAKSERVER_SESSION__SAKSERVER_SESSION_H_ */
