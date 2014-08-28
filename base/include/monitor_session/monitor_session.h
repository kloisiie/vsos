/*
 * \brief  Interface definition of the monitor service
 * \author wyq
 */


#ifndef _INCLUDE__MONITOR_SESSION__MONITOR_SESSION_H_
#define _INCLUDE__MONITOR_SESSION__MONITOR_SESSION_H_

#include <base/capability.h>
#include <base/stdint.h>
#include <base/rpc_args.h>
#include <session/session.h>
#include <base/rpc.h>
#include <dataspace/capability.h>
//#include <base/rpc_args.h>
//#include <task_strategy.h>
#define  MAX_TASKINTERFACE_OP_NUM        10
#define  MAX_TASKINTERFACE_NUM            5

namespace Genode {

	struct Monitor_Session : Session
	{
		typedef Rpc_in_buffer<20> Name_string;
		typedef Rpc_in_buffer<26> Strategy_string;
		typedef Rpc_out_buffer<26> Strategy_out_string;
		typedef Rpc_in_buffer<56> Audit_string;

		static const char *service_name() { return "Monitor"; }

                virtual int monitor_judge(unsigned long task_s, unsigned long task_d, Name_string interfacename,int op,Strategy_string _time)= 0;

		//virtual bool InsertUserStrategy(Name_string _name,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)= 0;

		virtual bool SetUserStrategyByname(Name_string _name,bool _op_type,int _style,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)= 0;

		virtual bool GetUserStrategyByname(Name_string _name,int& blp_level,int& biba_level,Strategy_out_string& blp_range,Strategy_out_string& biba_range)= 0;

		//virtual bool InsertTaskStrategy(Name_string _name,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)= 0;

		virtual bool SetTaskStrategyByname(Name_string _name,bool type, int _style,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)= 0;

		virtual bool GetTaskStrategyByname(Name_string _name,Strategy_out_string& owner_name, int& blp_level,int& biba_level,Strategy_out_string& blp_range,Strategy_out_string& biba_range )= 0;

		//virtual bool SetTaskStrategyByid(char* _name,int blp_level,int biba_level,int* blp_range,int* biba_range,char interfacename[][20],int op[][MAX_TASKINTERFACE_OP_NUM])= 0;

		virtual int SetTaskID(Name_string taskname, int taskid)= 0;

		virtual bool AddAuditInfo(Name_string _name,Audit_string audit_info,bool op_result,Strategy_string _time,int cur_level)=0;

		virtual bool ChargeApp(Name_string _name,bool style) = 0;

		//virtual bool ChargeAppStyle(Name_string _name) = 0;
                
                virtual Dataspace_capability GetAuditInfo() = 0;
		
		virtual bool Grant_cap(bool style,Strategy_string user,Strategy_string taskname ,Strategy_string servicename) = 0;

		virtual bool Del_cap(bool style,Strategy_string user,Strategy_string taskname ,Strategy_string servicename) = 0;

		//virtual bool Get_UserCap(bool style,Strategy_string username, int index, Strategy_out_string& cap_name ) = 0;

		virtual int Get_user_cap_num(bool task_or_service,Strategy_string username) = 0;

		virtual int cap_judge(Strategy_string servicename ) = 0;

		/*******************
		 ** RPC interface **
		 *******************/

		GENODE_RPC(Rpc_monitor_judge, int, monitor_judge, unsigned long, unsigned long, Name_string, int, Strategy_string );

		//GENODE_RPC(Rpc_InsertTaskStrategy, bool, InsertTaskStrategy, Name_string, int, int, Strategy_string, Strategy_string );
		GENODE_RPC(Rpc_SetTaskStrategyByname, bool, SetTaskStrategyByname, Name_string, bool, int, int, int, Strategy_string, Strategy_string );
		GENODE_RPC(Rpc_GetTaskStrategyByname, bool, GetTaskStrategyByname, Name_string, Strategy_out_string&, int&, int&, Strategy_out_string&, Strategy_out_string& );

		//GENODE_RPC(Rpc_InsertUserStrategy, bool, InsertUserStrategy, Name_string, int, int, Strategy_string, Strategy_string );
		GENODE_RPC(Rpc_SetUserStrategyByname, bool, SetUserStrategyByname, Name_string, bool, int, int, int, Strategy_string, Strategy_string );
		GENODE_RPC(Rpc_GetUserStrategyByname, bool, GetUserStrategyByname, Name_string, int&, int&, Strategy_out_string&, Strategy_out_string& );

		//GENODE_RPC(Rpc_SetTaskStrategyByid, bool, SetTaskStrategyByid, Task_strategy );
		GENODE_RPC(Rpc_SetTaskID, int, SetTaskID, Name_string, int );
		GENODE_RPC(Rpc_AddAuditInfo, bool, AddAuditInfo, Name_string, Audit_string, bool, Strategy_string,int );
		
		GENODE_RPC(Rpc_ChargeApp, bool, ChargeApp, Name_string, bool );
		//GENODE_RPC(Rpc_ChargeAppStyle, bool, ChargeAppStyle, Name_string );

		GENODE_RPC(Rpc_GetAuditInfo, Dataspace_capability , GetAuditInfo, );
		GENODE_RPC(Rpc_Grant_cap, bool, Grant_cap, bool,Strategy_string, Strategy_string, Strategy_string );
		GENODE_RPC(Rpc_Del_cap, bool, Del_cap, bool,Strategy_string, Strategy_string, Strategy_string );
		GENODE_RPC(Rpc_cap_judge, int, cap_judge, Strategy_string );
//		GENODE_RPC(Rpc_Get_UserCap, bool, Get_UserCap, bool, Strategy_string, int, Strategy_out_string& );
		GENODE_RPC(Rpc_Get_user_cap_num, int, Get_user_cap_num, bool,Strategy_string );
//		GENODE_RPC_INTERFACE(Rpc_monitor_judge, Rpc_InsertTaskStrategy, Rpc_SetTaskStrategyByname, Rpc_SetTaskID, Rpc_GetAuditInfo, Rpc_AddAuditInfo, Rpc_GetTaskStrategyByname, 					     Rpc_InsertUserStrategy, Rpc_SetUserStrategyByname,  Rpc_GetUserStrategyByname );
		//GENODE_RPC_INTERFACE(Rpc_monitor_judge, Rpc_SetTaskStrategyByname, Rpc_SetTaskID, Rpc_GetAuditInfo, Rpc_AddAuditInfo, Rpc_GetTaskStrategyByname,Rpc_SetUserStrategyByname,  					     Rpc_GetUserStrategyByname, Rpc_ChargeApp );
                //GENODE_RPC_INTERFACE(Rpc_monitor_judge, Rpc_SetTaskID);
			typedef Meta::Type_tuple<Rpc_monitor_judge,
			        Meta::Type_tuple<Rpc_SetTaskStrategyByname,
			        Meta::Type_tuple<Rpc_GetTaskStrategyByname,
			        Meta::Type_tuple<Rpc_SetUserStrategyByname,
			        Meta::Type_tuple<Rpc_GetUserStrategyByname,
			        Meta::Type_tuple<Rpc_SetTaskID,
			        Meta::Type_tuple<Rpc_AddAuditInfo,
			        Meta::Type_tuple<Rpc_ChargeApp,
			        Meta::Type_tuple<Rpc_GetAuditInfo,
			        Meta::Type_tuple<Rpc_Grant_cap,
			        Meta::Type_tuple<Rpc_Del_cap,
			        Meta::Type_tuple<Rpc_cap_judge,
			        Meta::Type_tuple<Rpc_Get_user_cap_num,
			                         Meta::Empty>
			        > > > > > > > > > > > > Rpc_functions;
	};
}

#endif /* _INCLUDE__HELLO_SESSION__HELLO_SESSION_H_ */
