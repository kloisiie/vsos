/*
 * \brief  Client-side interface of the Monitor service
 * \author wyq
 */

#ifndef _INCLUDE__MONITOR_SESSION_H__CLIENT_H_
#define _INCLUDE__MONITOR_SESSION_H__CLIENT_H_

#include <monitor_session/capability.h>
#include <base/rpc_client.h>
#include <base/printf.h>
#include <rtc_session/connection.h>

namespace Genode {

	struct Monitor_Session_client : Rpc_client<Monitor_Session>
	{
		Monitor_Session_client(Monitor_Session_capability cap)
		: Rpc_client<Monitor_Session>(cap) {
		//PERR("monitor session client born"); 
		}
		int monitor_judge(unsigned long task_s, unsigned long task_d, Name_string interfacename,int op)
		{
			static Rtc::Connection h; 			
			char _time[20]="";
			h.get_current_time_string(_time);
			return monitor_judge(task_s,task_d,interfacename,op,_time);
		}

                int monitor_judge(unsigned long task_s, unsigned long task_d, Name_string interfacename,int op,Strategy_string _time)
		{
			return call<Rpc_monitor_judge>(task_s,task_d,interfacename,op,_time);
		}

/*		bool InsertTaskStrategy(Name_string _name,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range )
		{
			char m_blprange[MAX_STRATEGY_RANGE];
                        char m_bibarange[MAX_STRATEGY_RANGE];
			while(*blp_range!=0)monitor_client
                            *(m_blprange++)=*(blp_range++);
			m_blprange='\0';
                        while(*biba_range!=0)
     			    *(m_bibarange++)=*(biba_range++);
			m_bibarange='\0';
			return call<Rpc_InsertTaskStrategy>(_name,blp_level,biba_level,blp_range,biba_range);
		}
*/
		bool SetTaskStrategyByname(Name_string _name,bool type, int style, int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range )
		{

			return call<Rpc_SetTaskStrategyByname>(_name,type,style,blp_level,biba_level,blp_range,biba_range);
		}

		int SetTaskID(Name_string taskname, int taskid)
		{
			//PERR("sessionclient taskname = %s",taskname.base());
			return call<Rpc_SetTaskID>(taskname,taskid);
		}
                
		bool AddAuditInfo(Name_string _name,Audit_string audit_info,bool op_result,Strategy_string _time,int cur_level)
		{
			//PERR("sessionclient taskname = %s",taskname.base());
			return call<Rpc_AddAuditInfo>(_name,audit_info,op_result,_time,cur_level);
		}
		bool AddAuditInfo(Name_string _name,Audit_string audit_info,bool op_result,int cur_level)
		{
			static Rtc::Connection h; 			
			char _time[20];
			h.get_current_time_string(_time);
			return AddAuditInfo(_name,audit_info,op_result,_time,cur_level);
		}
		
		Dataspace_capability GetAuditInfo()
		{
			return call<Rpc_GetAuditInfo>();
		}

/*		bool InsertUserStrategy(Name_string _name,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)
		{
			return call<Rpc_InsertUserStrategy>(_name,blp_level,biba_level,blp_range,biba_range);
		}
*/
		bool SetUserStrategyByname(Name_string _name,bool type,int style, int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)
		{
			return call<Rpc_SetUserStrategyByname>(_name,type,style,blp_level,biba_level,blp_range,biba_range);
		}

		bool GetUserStrategyByname(Name_string _name,int& blp_level,int& biba_level,Strategy_out_string& blp_range,Strategy_out_string& biba_range)
		{
			return call<Rpc_GetUserStrategyByname>(_name,blp_level,biba_level,blp_range,biba_range);
		}

		bool GetTaskStrategyByname(Name_string _name,Strategy_out_string& owner_name, int& blp_level,int& biba_level,Strategy_out_string& blp_range,Strategy_out_string& biba_range)
		{
			//PDBG("CLIENT call gettaskstrategybyname");

			return call<Rpc_GetTaskStrategyByname>(_name,owner_name,blp_level,biba_level,blp_range,biba_range);

		}

		bool ChargeApp(Name_string _name,bool style )
		{
			return call<Rpc_ChargeApp>(_name,style);
		}
		bool Grant_cap(bool style,Strategy_string user,Strategy_string taskname ,Strategy_string servicename)
		{
			return call<Rpc_Grant_cap>(style,user,taskname,servicename);	
		}

/*		bool Get_UserCap(bool style, Strategy_string username,int index,Strategy_out_string& cap_name)
		{
			return  call<Rpc_Get_UserCap>(style,username,index,cap_name);
			//PERR("THIS IS A TEST");
			//return true;
		}
*/

		int Get_user_cap_num(bool task_or_service,Strategy_string username)
		{
			return call<Rpc_Get_user_cap_num>(task_or_service,username);
		}

		bool Del_cap(bool style,Strategy_string user,Strategy_string taskname ,Strategy_string servicename)
		{
			return call<Rpc_Del_cap>(style,user,taskname,servicename);
		}

		int cap_judge(Strategy_string servicename )
		{
			return call<Rpc_cap_judge>(servicename );
		}

/*		bool ChargeAppStyle(Name_string _name)
		{
			return call<Rpc_ChargeAppStyle>(_name);
		}
*/
	};
}

#endif /* _INCLUDE__MONITOR_SESSION_H__CLIENT_H_ */
