/*
 * \brief  Server-side API of the RPC framework
 * \author Norman Feske
 * \date   2006-04-28
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__RPC_SERVER_H_
#define _INCLUDE__BASE__RPC_SERVER_H_

#include <base/rpc.h>
#include <base/thread.h>
#include <base/ipc.h>
#include <base/object_pool.h>
#include <base/lock.h>
#include <base/printf.h>
#include <cap_session/cap_session.h>
//#include <log_session/client.h>
//#include <log_session/connection.h>
#include <monitor_session/client.h>
#include <monitor_session/connection.h>

namespace Fiasco{
#include <l4/sys/utcb.h>
}
namespace Genode {

	/**
	 * RPC dispatcher implementing the specified RPC interface
	 *
	 * \param RPC_INTERFACE  class providing the RPC interface description
	 * \param SERVER         class to invoke for the server-side RPC functions
	 *
	 * This class is the base class of each server-side RPC implementation. It
	 * contains the logic for dispatching incoming RPC requests and calls the
	 * server functions according to the RPC declarations in 'RPC_INTERFACE'.
	 *
	 * If using the default argument for 'SERVER', the 'RPC_INTERFACE' is expected
	 * to contain the abstract interface for all RPC functions. So virtual functions
	 * must be declared in 'RPC_INTERFACE'. In contrast, by explicitly specifying
	 * the 'SERVER' argument, the server-side dispatching performs direct function
	 * calls to the respective member functions of the 'SERVER' class and thereby
	 * omits virtual functions calls.
	 */
	template <typename RPC_INTERFACE, typename SERVER = RPC_INTERFACE>
	class Rpc_dispatcher : public RPC_INTERFACE
	{
		/**
		 * Shortcut for the type list of RPC functions provided by this server
		 * component
		 */
		typedef typename RPC_INTERFACE::Rpc_functions Rpc_functions;

		protected:

			template <typename ARG_LIST>
			int _read_args(Ipc_istream &is, ARG_LIST &args)
			{
				int i=0;				
				if (Trait::Rpc_direction<typename ARG_LIST::Head>::Type::IN){
					is >> args._1;
					i=1;				
				}

				return _read_args(is, args._2)+i;
			}

			int _read_args(Ipc_istream &, Meta::Empty) {return 0; }

			template <typename ARG_LIST>
			void _write_results(Ipc_ostream &os, ARG_LIST &args)
			{
				if (Trait::Rpc_direction<typename ARG_LIST::Head>::Type::OUT)
					os << args._1;

				_write_results(os, args._2);
			}

			void _write_results(Ipc_ostream &, Meta::Empty) { }

			template <typename RPC_FUNCTION, typename EXC_TL>
			Rpc_exception_code _do_serve(typename RPC_FUNCTION::Server_args &args,
			                             typename RPC_FUNCTION::Ret_type    &ret,
			                             Meta::Overload_selector<RPC_FUNCTION, EXC_TL>)
			{
				enum { EXCEPTION_CODE = RPC_EXCEPTION_BASE - Meta::Length<EXC_TL>::Value };
				try {
					typedef typename EXC_TL::Tail Exc_tail;
					return _do_serve(args, ret,
					                 Meta::Overload_selector<RPC_FUNCTION, Exc_tail>());
				} catch (typename EXC_TL::Head) { return EXCEPTION_CODE; }
			}

			template <typename RPC_FUNCTION>
			Rpc_exception_code _do_serve(typename RPC_FUNCTION::Server_args &args,
			                             typename RPC_FUNCTION::Ret_type    &ret,
			                             Meta::Overload_selector<RPC_FUNCTION, Meta::Empty>)
			{
						
				RPC_FUNCTION::serve(*static_cast<SERVER *>(this), args, ret);
				return 0;
			}

			template <typename RPC_FUNCTIONS_TO_CHECK>
			Rpc_exception_code _do_dispatch(Rpc_opcode opcode, Ipc_istream &is, Ipc_ostream &os,
			                                Meta::Overload_selector<RPC_FUNCTIONS_TO_CHECK>)
			{
				using namespace Meta;

				typedef typename RPC_FUNCTIONS_TO_CHECK::Head This_rpc_function;
				//if(strcmp(This_rpc_function::get_name(),"read_state")==0) PERR("begin invoke");
				if (opcode == Index_of<Rpc_functions, This_rpc_function>::Value) {

					/*
					 * Argument receive buffer
					 *
					 * To prevent the compiler from complaining about the
					 * 'Server_args' data structure from being uninitialized,
					 * we instantiate the variable as volatile and strip away
					 * the volatile-ness when using it.
					 */
					//if(strcmp(This_rpc_function::get_name(),"read_state")==0) PERR("opcode %d",opcode);
					struct {
						typedef typename This_rpc_function::Server_args Data;
						volatile Data _data;
						Data &data() { return *(Data *)(&_data); }
					} args;
					
					/* read arguments from istream */
					int num=_read_args(is, args.data());

					/*
					 * Dispatch call to matching RPC base class, using
					 * 'This_rpc_function' and the list of its exceptions to
					 * select the overload.
					 */
					typedef typename This_rpc_function::Exceptions Exceptions;

					typename This_rpc_function::Ret_type ret;
					Rpc_exception_code exc;
					{				
						const char *ret_type=This_rpc_function::get_type();
						const char *fun_name=This_rpc_function::get_name();
						int op=strcmp(ret_type,"void")==0?0:1;
						op+=num>0?2:0;
						unsigned long *id=(unsigned long*)Fiasco::l4_utcb_id() ;						unsigned long senderID=*id;
						unsigned long receiverID=*(id+1);
						static Monitor_Connection h;
						if(strcmp(fun_name,"read_state")==0||strcmp(fun_name,"write_state")==0||strcmp(fun_name,"change_state")==0){						
						//if(senderID>=13 && receiverID>=13){
							PERR("%ld->%ld %s %s %d",senderID,receiverID,ret_type,fun_name,op);
							if(h.monitor_judge(senderID,receiverID,fun_name,op)==0){
								return RPC_PERMISSION_DENIED;
							}
							
							//Log_connection h;
							//Log_session::String s("hello");
							//h.write(s);
						
						}
						
					
					}
								
					
					exc = _do_serve(args.data(), ret, Overload_selector<This_rpc_function, Exceptions>());
					os << ret;

					/* write results to ostream 'os' */
					_write_results(os, args.data());

					return exc;
				}
				//if(strcmp(This_rpc_function::get_name(),"read_state")==0)  PERR("end");						
				typedef typename RPC_FUNCTIONS_TO_CHECK::Tail Tail;
				return _do_dispatch(opcode, is, os, Overload_selector<Tail>());
			}

			int _do_dispatch(int opcode, Ipc_istream &, Ipc_ostream &,
			                 Meta::Overload_selector<Meta::Empty>)
			{
				PERR("invalid opcode %d\n", opcode);
				return RPC_INVALID_OPCODE;
			}

			/**
			 * Handle corner case of having an RPC interface with no RPC functions
			 */
			Rpc_exception_code _do_dispatch(int opcode, Ipc_istream &, Ipc_ostream &,
			                                Meta::Overload_selector<Meta::Type_list<> >)
			{
				return 0;
			}

			/**
			 * Protected constructor
			 *
			 * This class is only usable as base class.
			 */
			Rpc_dispatcher() { }

		public:

			Rpc_exception_code dispatch(int opcode, Ipc_istream &is, Ipc_ostream &os)
			{
				return _do_dispatch(opcode, is, os,
				                    Meta::Overload_selector<Rpc_functions>());
			}
	};


	class Rpc_object_base : public Object_pool<Rpc_object_base>::Entry
	{
		public:

			virtual ~Rpc_object_base() { }

			/**
			 * Interface to be implemented by a derived class
			 *
			 * \param op   opcode of invoked method
			 * \param is   Ipc_input stream with method arguments
			 * \param os   Ipc_output stream for storing method results
			 */
			virtual int dispatch(int op, Ipc_istream &is, Ipc_ostream &os) = 0;
	};


	/**
	 * Object that is accessible from remote protection domains
	 *
	 * A 'Rpc_object' is a locally implemented object that can be referenced
	 * from the outer world using a capability. The capability gets created
	 * when attaching a 'Rpc_object' to a 'Rpc_entrypoint'.
	 */
	template <typename RPC_INTERFACE, typename SERVER = RPC_INTERFACE>
	struct Rpc_object : Rpc_object_base, Rpc_dispatcher<RPC_INTERFACE, SERVER>
	{
		/*****************************
		 ** Server-object interface **
		 *****************************/

		Rpc_exception_code dispatch(int opcode, Ipc_istream &is, Ipc_ostream &os)
		{
			return Rpc_dispatcher<RPC_INTERFACE, SERVER>::dispatch(opcode, is, os);
		}

		Capability<RPC_INTERFACE> const cap() const
		{
			return reinterpret_cap_cast<RPC_INTERFACE>(Rpc_object_base::cap());
		}
	};


	/**
	 * RPC entrypoint serving RPC objects
	 *
	 * The entrypoint's thread will initialize its capability but will not
	 * immediately enable the processing of requests. This way, the
	 * activation-using server can ensure that it gets initialized completely
	 * before the first capability invocations come in. Once the server is
	 * ready, it must enable the entrypoint explicitly by calling the
	 * 'activate()' function. The 'start_on_construction' argument is a
	 * shortcut for the common case where the server's capability is handed
	 * over to other parties _after_ the server is completely initialized.
	 */
	class Rpc_entrypoint : Thread_base, public Object_pool<Rpc_object_base>
	{
		private:

			/**
			 * Prototype capability to derive capabilities for RPC objects
			 * from.
			 */
			Untyped_capability _cap;

			enum { SND_BUF_SIZE = 1024, RCV_BUF_SIZE = 1024 };
			Msgbuf<SND_BUF_SIZE> _snd_buf;
			Msgbuf<RCV_BUF_SIZE> _rcv_buf;

			/**
			 * Hook to let low-level thread init code access private members
			 *
			 * This function is only used on NOVA.
			 */
			static void _activation_entry();

			struct Exit
			{
				GENODE_RPC(Rpc_exit, void, _exit);
				GENODE_RPC_INTERFACE(Rpc_exit);
			};

			struct Exit_handler : Rpc_object<Exit, Exit_handler>
			{
				int exit;

				Exit_handler() : exit(false) { }

				void _exit() { exit = true; }
			};

		protected:

			Ipc_server      *_ipc_server;
			Rpc_object_base *_curr_obj;       /* currently dispatched RPC object       */
			Lock             _curr_obj_lock;  /* for the protection of '_curr_obj'     */
			Lock             _cap_valid;      /* thread startup synchronization        */
			Lock             _delay_start;    /* delay start of request dispatching    */
			Lock             _delay_exit;     /* delay destructor until server settled */
			Cap_session     *_cap_session;    /* for creating capabilities             */
			Exit_handler     _exit_handler;
			Capability<Exit> _exit_cap;

			/**
			 * Back-end function to associate RPC object with the entry point
			 */
			Untyped_capability _manage(Rpc_object_base *obj);

			/**
			 * Back-end function to Dissolve RPC object from entry point
			 */
			void _dissolve(Rpc_object_base *obj);

			/**
			 * Force activation to cancel dispatching the specified server object
			 */
			void _leave_server_object(Rpc_object_base *obj);

			/**
			 * Wait until the entrypoint activation is initialized
			 */
			void _block_until_cap_valid();

			/**
			 * Thread interface
			 */
			void entry();

		public:

			/**
			 * Constructor
			 *
			 * \param cap_session  'Cap_session' for creating capabilities
			 *                     for the RPC objects managed by this entry
			 *                     point
			 * \param stack_size   stack size of entrypoint thread
			 * \param name         name of entrypoint thread
			 */
			Rpc_entrypoint(Cap_session *cap_session, size_t stack_size,
			               char const *name, bool start_on_construction = true);

			~Rpc_entrypoint();

			/**
			 * Associate RPC object with the entry point
			 */
			template <typename RPC_INTERFACE, typename RPC_SERVER>
			Capability<RPC_INTERFACE>
			manage(Rpc_object<RPC_INTERFACE, RPC_SERVER> *obj)
			{
				return reinterpret_cap_cast<RPC_INTERFACE>(_manage(obj));
			}

			/**
			 * Dissolve server object from entry point
			 */
			template <typename RPC_INTERFACE, typename RPC_SERVER>
			void dissolve(Rpc_object<RPC_INTERFACE, RPC_SERVER> *obj)
			{
				_dissolve(obj);
			}

			/**
			 * Activate entrypoint, start processing RPC requests
			 */
			void activate();

			/**
			 * Request reply capability for current call
			 *
			 * Note: This is a temporary API function, which is going to be
			 * removed. Please do not use this function.
			 *
			 * Typically, a capability obtained via this function is used as
			 * argument of 'intermediate_reply'.
			 */
			Untyped_capability reply_dst();

			/**
			 * Prevent reply of current request
			 *
			 * Note: This is a temporary API function, which is going to be
			 * removed. Please do not use this function.
			 *
			 * This function can be used to keep the calling client blocked
			 * after the server has finished the processing of the client's
			 * request. At a later time, the server may chose to unblock the
			 * client via the 'intermedate_reply' function.
			 */
			void omit_reply();

			/**
			 * Send a reply out of the normal call-reply order
			 *
			 * Note: This is a temporary API function, which is going to be
			 * removed. Please do not use this function.
			 *
			 * In combination with the 'reply_dst' accessor functions, this
			 * function can be used to implement services that dispatch client
			 * requests out of order. In such cases, the server activation may
			 * send reply messages to multiple blocking clients before
			 * answering the original call.
			 */
			void explicit_reply(Untyped_capability reply_cap, int return_value);

			/**
			 * Return true if the caller corresponds to the entrypoint called
			 */
			bool is_myself() const;
	};
}

#endif /* _INCLUDE__BASE__RPC_SERVER_H_ */
