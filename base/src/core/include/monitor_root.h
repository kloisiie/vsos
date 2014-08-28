/*

 \author wyq

*/

#ifndef __MONITOR_SESSION_ROOT_H_
#define __MONITOR_SESSION_ROOT_H_

#include <root/component.h>
#include "monitor_session_component.h"

namespace Genode {

	class Monitor_Root : public Root_component<Monitor_Session_component>
	{
		protected:

			Monitor_Session_component *_create_session(const char *args)
			{
				//PDBG("creating hello session.");
				return new (md_alloc()) Monitor_Session_component();
			}

		public:

			Monitor_Root(Genode::Rpc_entrypoint *ep,
			               Genode::Allocator *allocator)
			: Root_component<Monitor_Session_component>(ep, allocator)
			{
				//PDBG("Creating root component.");
			}
	};
}

#endif
