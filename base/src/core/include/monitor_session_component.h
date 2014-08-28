/*

 \author wyq

*/

#ifndef __MONITOR_SESSION_COMPONENT_H_
#define __MONITOR_SESSION_COMPONENT_H_


#include <monitor_session/monitor_session.h>
#include <base/rpc_server.h>
#include <monitor_session/monitor.h>

namespace Genode {
	MONITOR_MAIN m_monitor;
	
	class Monitor_Session_component : public Rpc_object<Monitor_Session>
	{
              
                public:
                int monitor_judge(unsigned long task_s,unsigned long task_d, Name_string interfacename,int op,Strategy_string _time)
		{
			return m_monitor.monitor_judge(task_s,task_d,interfacename.base(),op,_time.base());
		}

/*		bool InsertTaskStrategy(Name_string _name,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)
		{
			//PERR("GetTaskblprange = %s",blp_range.base());
			//PERR("GetTaskbibarange = %s",biba_range.base());	
                        Task_strategy m_task(_name.base(),blp_level,biba_level,blp_range.base(),biba_range.base());
			return m_monitor.InsertTaskStrategy(m_task);
		}
*/
		bool SetTaskStrategyByname(Name_string _name,bool type,int style,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)
		{
			Task_strategy m_task(_name.base(),"",style,blp_level,biba_level,blp_range.base(),biba_range.base());
			if(type)
			{
				return m_monitor.InsertTaskStrategy(m_task);
			}
			else
			{
				return m_monitor.SetTaskStrategyByname(m_task);
			}
			
		}

/*		bool Get_UserCap(bool task_or_service, Strategy_string username,int index,Strategy_out_string& cap_name)
		{
			//Strategy_out_string cap_name;
			if(task_or_service)   //true  task
			{
				cap_name = m_monitor.Get_User_Task_Cap(username.base(),index);
				cap_name.setball(m_monitor.Get_User_Task_Cap(username.base(),index),cap_name.size());
			}
			else
			{
				cap_name = m_monitor.Get_User_Service_Cap(username.base(),index);
				cap_name.setball(m_monitor.Get_User_Service_Cap(username.base(),index),cap_name.size());
			}

			return true;

		}
*/

		int Get_user_cap_num(bool task_or_service,Strategy_string username)
		{

			if(task_or_service)   //true  task
			{
				return  m_monitor.Get_User_Task_Cap_Num(username.base());
			}
			else
			{
				return  m_monitor.Get_User_Service_Cap_Num(username.base());
			}

			//return true;
		}

		int SetTaskID(Name_string taskname, int taskid)
		{
			//PERR("component SetTaskname = %s",taskname.base());
			return m_monitor.SetTaskID(taskname.base(),taskid);
		}
		bool AddAuditInfo(Name_string _name,Audit_string audit_info,bool op_result,Strategy_string _time,int cur_level)
		{
			//PERR("component SetTaskname = %s",taskname.base());
			return m_monitor.AddAuditInfo(_name.base(),audit_info.base(),op_result,_time.base(),cur_level);
		}
		Dataspace_capability GetAuditInfo()
		{
			return m_monitor.data;
		}

/*		bool InsertUserStrategy(Name_string _name,int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)
		{
			User_strategy m_user(_name.base(),blp_level,biba_level,blp_range.base(),biba_range.base());
			return m_monitor.InsertUserStrategy(m_user);
		}
*/
		bool SetUserStrategyByname(Name_string _name,bool type,int style, int blp_level,int biba_level,Strategy_string blp_range,Strategy_string biba_range)
		{
			if(blp_level/10 > blp_level%10 || biba_level/10 > biba_level%10)
			{
				return false;
			}
			if(blp_level/10 ==0 || biba_level/10 ==0 )
			{
				return false;
			}
			//PERR("BLP:%d,biba:%d",blp_level,biba_level);
			User_strategy m_user(_name.base(),style,blp_level/10,blp_level%10,biba_level/10,biba_level%10,blp_range.base(),biba_range.base());
			if(type)
			{
				return m_monitor.InsertUserStrategy(m_user);
			}
			else
			{
				return m_monitor.SetUserStrategyByname(m_user);
			}
			
		}
		bool Grant_cap(bool style,Strategy_string user,Strategy_string taskname ,Strategy_string servicename)
		{
			if(style)
			{
				return m_monitor.Grant_Task_Cap(user.base(),taskname.base());
			}
			else
			{
				return m_monitor.Grant_Service_Cap(user.base(),taskname.base(),servicename.base());
			}			
			//return ;	
		}

		bool Del_cap(bool style,Strategy_string user,Strategy_string taskname ,Strategy_string servicename)
		{
			if(style)
			{
				return m_monitor.Del_Task_Cap(user.base(),taskname.base());
			}
			else
			{
				return m_monitor.Del_Service_Cap(user.base(),taskname.base(),servicename.base());
			}
		}

		int cap_judge(Strategy_string servicename)
		{

			return m_monitor.cap_judge(servicename.base());
		}

		bool GetUserStrategyByname(Name_string _name,int& blp_level,int& biba_level,Strategy_out_string& blp_range,Strategy_out_string& biba_range)
		{
			User_strategy m_user = m_monitor.GetUserStrategyByname(_name.base());
			int m_low,m_high;
			m_user.GetUserBlpLevel(m_low,m_high);
			blp_level = m_low * 10 + m_high;
			m_user.GetUserBibaLevel(m_low,m_high);
			biba_level = m_low * 10 + m_high;
			//Genode::strncpy(blp_range.base(),m_user.GetUserBlpRange(),Genode::strlen(m_user.GetUserBlpRange())+1);
			//Genode::strncpy(biba_range.base(),m_user.GetUserBibaRange(),Genode::strlen(m_user.GetUserBibaRange())+1);
			blp_range = m_user.GetUserBlpRange();
			blp_range.setball(m_user.GetUserBlpRange(),blp_range.size());
			biba_range = m_user.GetUserBibaRange();
			biba_range.setball(m_user.GetUserBibaRange(),biba_range.size());
			return true;
		}
		bool ChargeApp(Name_string _name,bool style)
		{
			if(style)
			{
				return m_monitor.ChargeAppLevel(_name.base());
			}
			else
			{
				return m_monitor.ChargeAppStyle(_name.base());
			}
		}
/*		bool ChargeAppStyle(Name_string _name)
		{
			return m_monitor.ChargeAppStyle(_name.base());
		}
*/
		bool GetTaskStrategyByname(Name_string _name,Strategy_out_string& owner_name,int& blp_level,int& biba_level,Strategy_out_string& blp_range,Strategy_out_string& biba_range)
		{

			if(blp_level ==55)
			{

				owner_name = m_monitor.Get_User_Task_Cap(_name.base(),biba_level);
				owner_name.setball(m_monitor.Get_User_Task_Cap(_name.base(),biba_level),owner_name.size());
				return true;
	
			}
			if(blp_level ==56)
			{
				owner_name = m_monitor.Get_User_Service_Cap(_name.base(),biba_level);
				owner_name.setball(m_monitor.Get_User_Service_Cap(_name.base(),biba_level),owner_name.size());
				return true;
			}
			Task_strategy m_task = m_monitor.GetTaskStrategyByname(_name.base());
			blp_level = m_task.GetTaskBlpLevel();
			biba_level = m_task.GetTaskBibaLevel();
			//PERR("GetTaskblprange = %s",m_task.BLP_RANGE);
			//PERR("GetTaskbibarange = %s",m_task.BIBA_RANGE);
			//Genode::strncpy(blp_range.base(),m_task.GetTaskBlpRange(),Genode::strlen(m_task.GetTaskBlpRange())+1);
			//Genode::strncpy(biba_range.base(),m_task.GetTaskBibaRange(),Genode::strlen(m_task.GetTaskBibaRange())+1);
/*			for(int i=0;blp_range._balloon[i];i++)
			{
				PDBG(" get blprange %d = %d", i,blp_range._balloon[i]);
			}
			for(int j=0;biba_range._balloon[j];j++)
			{
				PDBG(" get bibarange %d = %d ",j, biba_range._balloon[j]);
			}
*/
			//PDBG("SERVER COPY START");
			blp_range = m_task.GetTaskBlpRange();
			blp_range.setball(m_task.GetTaskBlpRange(),blp_range.size());
			biba_range = m_task.GetTaskBibaRange();
			biba_range.setball(m_task.GetTaskBibaRange(),biba_range.size());
			owner_name = m_task.GetTaskOwnerName();
			owner_name.setball(m_task.GetTaskOwnerName(),owner_name.size());
/*			for(int i=0;i<blp_range.size();i++)
			{
				PDBG("SERVER GET blprange %d = %d", i,blp_range.base()[i]);
			}
			for(int j=0;j<biba_range.size();j++)
			{
				PDBG("SERVER GET bibarange %d = %d ",j, biba_range.base()[j]);
			}
*/
			return true;
		}
	};
}

#endif
