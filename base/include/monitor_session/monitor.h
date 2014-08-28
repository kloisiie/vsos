#ifndef _MONITOR_H_
#define _MONITOR_H_


#define  MAX_TASK_NUM           30
#define  MAX_USER_NUM           10
#define  MAX_AUDIT_INFO         100

#include "task_strategy.h"
#include "user_strategy.h"
#include <base/snprintf.h>
#include "auditinfo.h"
#include <dataspace/capability.h>
#include <platform.h>
#include <core_env.h>

class MONITOR_MAIN 
{
	private:
		Task_strategy  Task_str_list[MAX_TASK_NUM];
		User_strategy  User_str_list[MAX_USER_NUM];

                int m_str_list;
		int m_user_list;
                AuditInfo *audit_base;
                int m_auditnum;
                char defaultusername[20];
		int m_cur_level;

                /*char AuditInfo[MAX_AUDIT_INFO][150];
                int AuditListHead;
                int AuditListTail;
	        bool AuditEmpty;
		bool AuditFull;*/
		

		void Initial_task_str_list()
		{

		    Task_strategy Task("core","admin",TASKSTYLENORMAL,0,0,NULL,NULL);
		    Task_str_list[m_str_list++]=Task;

		    Task_strategy Task1("init","admin",TASKSTYLEADMIN,0,0,NULL,NULL);
		    Task_str_list[m_str_list++]=Task1;

		    char taskblp2[MAX_STRATEGY_RANGE]={12,24,14,99,47,100,76};                   
		    char taskbiba2[MAX_STRATEGY_RANGE]={12,16,14};
		    Task_strategy Task2("lowlevel_app","admin",TASKSTYLENORMAL,1,1,taskblp2,taskbiba2);
		    Task_str_list[m_str_list++]=Task2;

		    char taskblp3[MAX_STRATEGY_RANGE]={12,24,14,99,47,100,76};                   
		    char taskbiba3[MAX_STRATEGY_RANGE]={12,16,14};
		    Task_strategy Task3("midlevel_app","admin",TASKSTYLENORMAL,2,1,taskblp3,taskbiba3);
		    Task_str_list[m_str_list++]=Task3;

		    char taskblp4[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba4[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task4("highlevel_app","admin",TASKSTYLENORMAL,3,1,taskblp4,taskbiba4);
		    Task_str_list[m_str_list++]=Task4;
			
		    char taskblp5[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba5[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task5("midlevel_process","securer",TASKSTYLENORMAL,2,1,taskblp5,taskbiba5);
		    Task_str_list[m_str_list++]=Task5;

		    char taskblp6[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba6[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task6("lowlevel_process","admin",TASKSTYLENORMAL,1,1,taskblp6,taskbiba6);
		    Task_str_list[m_str_list++]=Task6;

		    char taskblp7[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba7[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task7("highlevel_process","admin",TASKSTYLENORMAL,3,1,taskblp7,taskbiba7);
		    Task_str_list[m_str_list++]=Task7;

		    char taskblp8[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba8[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task8("log","audit",TASKSTYLEAUDIT,2,1,taskblp8,taskbiba8);
		    Task_str_list[m_str_list++]=Task8;

		    char taskblp9[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba9[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task9("funtest","admin",TASKSTYLEADMIN,3,1,taskblp9,taskbiba9);
		    Task_str_list[m_str_list++]=Task9;

		    char taskblp10[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba10[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task10("monitortest","securer",TASKSTYLESECURE,2,1,taskblp10,taskbiba10);
		    Task_str_list[m_str_list++]=Task10;

		    char taskblp11[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba11[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task11("dos_server","admin",TASKSTYLEADMIN,3,1,taskblp11,taskbiba11);
		    Task_str_list[m_str_list++]=Task11;

		    char taskblp12[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba12[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task12("dos_client","admin",TASKSTYLEADMIN,3,1,taskblp12,taskbiba12);
		    Task_str_list[m_str_list++]=Task12;

		    char taskblp13[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba13[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task13("dos_attacker","admin",TASKSTYLEADMIN,3,1,taskblp13,taskbiba13);
		    Task_str_list[m_str_list++]=Task13;

		    char taskblp14[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba14[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task14("leak_access","admin",TASKSTYLEADMIN,3,1,taskblp14,taskbiba14);
		    Task_str_list[m_str_list++]=Task14;

		    char taskblp15[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba15[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task15("overflow","admin",TASKSTYLEADMIN,3,1,taskblp15,taskbiba15);
		    Task_str_list[m_str_list++]=Task15;

		    char taskblp16[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba16[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task16("tocttou_mkfiles","admin",TASKSTYLEADMIN,3,1,taskblp16,taskbiba16);
		    Task_str_list[m_str_list++]=Task16;

		    char taskblp17[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba17[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task17("test-lwip_httpsrv","admin",TASKSTYLEADMIN,3,1,taskblp17,taskbiba17);
		    Task_str_list[m_str_list++]=Task17;

		    char taskblp18[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba18[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task18("l4linux","admin",TASKSTYLEADMIN,3,1,taskblp18,taskbiba18);
		    Task_str_list[m_str_list++]=Task18;

		    char taskblp19[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba19[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task19("reuse","admin",TASKSTYLEADMIN,3,1,taskblp19,taskbiba19);
		    Task_str_list[m_str_list++]=Task19;

		    char taskblp20[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba20[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task20("channel_server","admin",TASKSTYLEADMIN,3,1,taskblp20,taskbiba20);
		    Task_str_list[m_str_list++]=Task20;

		    char taskblp21[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba21[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task21("channel_client","admin",TASKSTYLEADMIN,3,1,taskblp21,taskbiba21);
		    Task_str_list[m_str_list++]=Task21;

		    char taskblp22[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba22[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task22("channel_interfere","admin",TASKSTYLEADMIN,3,1,taskblp22,taskbiba22);
		    Task_str_list[m_str_list++]=Task22;

		    char taskblp23[MAX_STRATEGY_RANGE]={12,24,14,99,100,47,76};
		    char taskbiba23[MAX_STRATEGY_RANGE]={12,14,16};
		    Task_strategy Task23("dactest","admin",TASKSTYLENORMAL,3,1,taskblp23,taskbiba23);
		    Task_str_list[m_str_list++]=Task23;

		}

		void Initial_user_str_list()
		{
		    char userblp[MAX_STRATEGY_RANGE]={12,24,14,99,47,100,76};                   
		    char userbiba[MAX_STRATEGY_RANGE]={12,16,14};
		    User_strategy user1("admin",USERADMIN,1,3,1,3,userblp,userbiba);
		    user1.AddUserTaskCap("lowlevel_app");
		    user1.AddUserTaskCap("midlevel_app");
		    user1.AddUserTaskCap("highlevel_app");
		    user1.AddUserTaskCap("midlevel_process");
		    user1.AddUserTaskCap("lowlevel_process");
		    user1.AddUserTaskCap("highlevel_process");
		    user1.AddUserTaskCap("funtest");
		    user1.AddUserTaskCap("dos_server");
		    user1.AddUserTaskCap("dos_client");
		    user1.AddUserTaskCap("dos_attacker");
		    user1.AddUserTaskCap("leak_access");
		    user1.AddUserTaskCap("overflow");
		    user1.AddUserTaskCap("tocttou_mkfiles");
		    user1.AddUserTaskCap("test-lwip_httpsrv");
		    user1.AddUserTaskCap("l4linux");
		    user1.AddUserTaskCap("reuse");
		    user1.AddUserTaskCap("channel_server");
		    user1.AddUserTaskCap("channel_client");
		    user1.AddUserTaskCap("channel_interfere");
		    user1.AddUserTaskCap("init");
		    user1.AddUserTaskCap("dactest");			
		    user1.AddUserServiceCap("Confidential");
		    User_str_list[m_user_list++]=user1;


		    char userblp1[MAX_STRATEGY_RANGE]={12,24,14,99,47,100,76};                   
		    char userbiba1[MAX_STRATEGY_RANGE]={12,16,14};
		    User_strategy user2("audit",USERAUDIT,2,2,1,1,userblp1,userbiba1);
		    user2.AddUserTaskCap("log");
		    
		    user2.AddUserTaskCap("dactest");
		    User_str_list[m_user_list++]=user2;

		    char userblp3[MAX_STRATEGY_RANGE]={12,24,14,99,47,100,76};                   
		    char userbiba3[MAX_STRATEGY_RANGE]={12,16,14};
		    User_strategy user3("securer",USERSECURE,2,2,1,1,userblp3,userbiba3);
		    user3.AddUserTaskCap("monitortest");
		    user3.AddUserTaskCap("midlevel_process");
		    user3.AddUserTaskCap("dactest");
		    User_str_list[m_user_list++]=user3;

		    char userblp4[MAX_STRATEGY_RANGE]={12,24,14,99,47,100,76};                   
		    char userbiba4[MAX_STRATEGY_RANGE]={12,16,14};
		    User_strategy user4("user",USERNORMAL,1,1,1,1,userblp4,userbiba4);
		    user4.AddUserTaskCap("dactest");
		    User_str_list[m_user_list++]=user4;

		}

                int SearchTaskList_by_id(const int task_id)
                { 
                                 
                     for(int i=0;i<MAX_TASK_NUM;i++)
                     {
			//PERR("TASKID=%d %d",Task_str_list[i].Task_id,task_id);
                         if(Task_str_list[i].GetTaskID()==task_id)
                         {
                             return i;
                         } 
                     }
                     return -1;   
                }
                int SearchTaskList_by_name(const char *task_name)
                {
			/*for(int j=0;j<3;j++)
			{
				PERR("listtaskname: %s",Task_str_list[j].Task_Name);
			}*/
                     for(int i=0;i<MAX_TASK_NUM;i++)
                     {
                         if(!Genode::strcmp(Task_str_list[i].GetTaskName(),task_name))
                         {
                             return i;
                         } 
                     }
                     return -1;   
                }
                int SearchUserList_by_name(const char *user_name)
                {
			/*for(int j=0;j<3;j++)
			{
				PERR("listtaskname: %s",Task_str_list[j].Task_Name);
			}*/
                     for(int i=0;i<MAX_USER_NUM;i++)
                     {
                         if(!Genode::strcmp(User_str_list[i].GetUserName(),user_name))
                         {
                             return i;
                         } 
                     }
                     return -1;   
                }
      
                int blp_judge(int index_client,int index_server,bool m_tep)
		{
                        bool m_equal = true;
			if(index_client==0||index_server==0||index_client==1||index_server==1)
			{
				return 0;
			}
			int client_blp_level;
			char* client_blp_range;
		  	int server_blp_level;
		 	char* server_blp_range;
			//int m_cur_user = SearchUserList_by_name(defaultusername);
			if(!Genode::strcmp(Task_str_list[index_client].GetTaskOwnerName(),defaultusername))
			{
				int m_cur_user = SearchUserList_by_name(defaultusername);
				client_blp_level = m_cur_level;
				client_blp_range = User_str_list[m_cur_user].GetUserBlpRange();
			}
			else
			{
		                client_blp_level  = Task_str_list[index_client].GetTaskBlpLevel();
			 	client_blp_range = Task_str_list[index_client].GetTaskBlpRange();
			}

			if(!Genode::strcmp(Task_str_list[index_server].GetTaskOwnerName(),defaultusername))
			{
				int m_cur_user = SearchUserList_by_name(defaultusername);
				server_blp_level = m_cur_level;
				server_blp_range = User_str_list[m_cur_user].GetUserBlpRange();
			}
			else
			{
			  	server_blp_level  = Task_str_list[index_server].GetTaskBlpLevel();
			 	server_blp_range = Task_str_list[index_server].GetTaskBlpRange();
			}


			if(client_blp_level >= server_blp_level)
			{
				int i=0,j=0;
				do
				{
					if(client_blp_range[i]== server_blp_range[j])
					{
						i++;
						j++;
		                                if(client_blp_range[i]==0)
		                                {
							break;
		                                }
					}
					else
					{
		                                m_equal = false;
		                                i++;
		                                if(client_blp_range[i]==0)
		                                {
							break;
		                                }						
					}
				}
				while(server_blp_range[j]!=0);
				if(server_blp_range[j]==0)
		                {
		                        if(client_blp_range[i]==0 && m_equal)
		                        {
		                                if(client_blp_level == server_blp_level)
						{
							return 0;
						}
						else
						{
							return 1;
						}
		                        }
		                        else
					{
						return 1;
					}

				}
				else
				{
					if(client_blp_level == server_blp_level)
					{
						if(blp_judge(index_server,index_client,true)==1)
						{
							return -1;
						}
						else
						{
							return 2;
						}

					}
					else
					{
						return 2;
					}
				}
			}
			else if(blp_judge(index_server,index_client,true)==1)
			{
		                return -1;
			}
			else
			{
				return 2;
			}

		}

		int biba_judge(int index_client,int index_server,bool m_tep)
		{
			bool m_equal = true;
			if(index_client==0||index_server==0||index_client==1||index_server==1)
			{
				return 0;
			}
			//int m_cur_user = SearchUserList_by_name(defaultusername);

                        int client_biba_level  = Task_str_list[index_client].GetTaskBibaLevel();
		  	int server_biba_level  = Task_str_list[index_server].GetTaskBibaLevel();

		 	char* client_biba_range = Task_str_list[index_client].GetTaskBibaRange();
		 	char* server_biba_range = Task_str_list[index_server].GetTaskBibaRange();
/*			for(int i=0;client_biba_range[i]!=0;i++)
			{
				PERR("CLIENT BIBA: %d ,%d",i,client_biba_range[i]);
			}
			for(int j=0;server_biba_range[j]!=0;j++)
			{
				PERR("SERVER BIBA: %d ,%d",j,server_biba_range[j]);
			}
*/
			if(client_biba_level >= server_biba_level)
			{
				int i=0,j=0;
				do
				{
					if(client_biba_range[i]== server_biba_range[j])
					{
						i++;
						j++;
		                                if(client_biba_range[i]==0)
		                                {
							break;
		                                }
					}
					else
					{
						m_equal = false;
		                                i++;
		                                if(client_biba_range[i]==0)
		                                {
							break;
		                                }						
					}
				}
				while(server_biba_range[j]!=0);
				if(server_biba_range[j]==0)
		                {
		                        if(client_biba_range[i]==0 && m_equal)
		                        {
		                                if(client_biba_level == server_biba_level )
						{
							return 0;
						}
						else
						{
							return 1;
						}
		                        }
		                        else
					{
						return 1;
					}
				}
				else
				{
					if(client_biba_level == server_biba_level)
					{
						if(biba_judge(index_server,index_client,true)==1)
						{
							return -1;
						}
						else
						{
							return 2;
						}
					}
					else
					{
						return 2;
					}

				}
			}
			else if(biba_judge(index_server,index_client,true)==1)
			{
		                return -1;
			}
			else
			{
				return 2;
			}

	
		}


		bool AddAuditInfo(const int client,const int server,const char *interfacename,const int op,bool result,const char *_time)
		{
			
			AuditInfo temp(defaultusername,Task_str_list[client].GetTaskName(),Task_str_list[server].GetTaskName(),interfacename,_time,op,Task_str_list[client].GetTaskBlpLevel(),Task_str_list[client].GetTaskBibaLevel(),Task_str_list[server].GetTaskBlpLevel(),Task_str_list[server].GetTaskBibaLevel(),result);
			audit_base[m_auditnum] = temp;
			m_auditnum = (m_auditnum+1)%MAX_AUDIT_INFO;
                        return true;	
		}

/*		bool SearchCurUserTaskCap(char* taskname)
		{
			int m_cur_user = SearchUserList_by_name(defaultusername);
			int m_cap_num = User_str_list[m_cur_user].GetUserTaskCapNum();
			for(int i=0;i<m_cap_num;i++)
			{
				if(!Genode::strcmp(User_str_list[m_cur_user].GetUserTaskCap(i),taskname))
				{
					return true;
				}
			}
			return false;			
		}
		bool SearchCurUserServiceCap(char* servicename)
		{
			int m_cur_user = SearchUserList_by_name(defaultusername);
			int m_cap_num = User_str_list[m_cur_user].GetUserServiceCapNum();
			for(int i=0;i<m_cap_num;i++)
			{
				if(!Genode::strcmp(User_str_list[m_cur_user].GetUserServiceCap(i),servicename))
				{
					return true;
				}
			}
			return false;			
		}
*/



	public:
		Genode::Dataspace_capability data;
     
                MONITOR_MAIN()
                {
			m_str_list = 0 ;
			m_user_list = 0 ;
                        Initial_task_str_list();
			Initial_user_str_list();
			/*AuditListHead = 0;
			AuditListTail = 0;
			AuditEmpty = true;
			AuditFull = false;*/
			enum { STACK_SIZE = 4096 };
			data=Genode::env()->ram_session()->alloc(MAX_AUDIT_INFO * sizeof(AuditInfo));
			audit_base=(AuditInfo *)Genode::env()->rm_session()->attach(data);
                        for(int i=0;i<MAX_AUDIT_INFO;i++)
			{
				audit_base[i].m_IfRead = false;
			}
			m_auditnum = 0 ;
			Genode::strncpy(defaultusername,"default",Genode::strlen("default")+1);
			m_cur_level = 0 ;
                }
		bool ChargeAppLevel(const char *taskname)
		{
			int indexTask = SearchTaskList_by_name(taskname);
			if(indexTask==-1)
			{
				PERR("ChargeAppLevel:taskname not exit");
				return true;
			}

			int indexUser = SearchUserList_by_name(defaultusername);
			bool m_result = User_str_list[indexUser].SearchTaskCap(taskname);
			if(m_result)
			{
				return true;
			}
			else
			{
				return false;
			}		
/*			int m_taskblp = Task_str_list[indexTask].GetTaskBlpLevel();
			int m_taskbiba = Task_str_list[indexTask].GetTaskBibaLevel();
			if(m_taskblp == 0)
			{
				return true;
			}
			int m_userblp_low,m_userblp_high;
			User_str_list[indexUser].GetUserBlpLevel(m_userblp_low,m_userblp_high);
			if(m_taskblp >=m_userblp_low && m_taskblp <= m_userblp_high)
			{
				User_str_list[indexUser].GetUserBibaLevel(m_userblp_low,m_userblp_high);			
				if(m_taskbiba >=m_userblp_low && m_taskbiba <= m_userblp_high)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
*/			
		}
		bool ChargeAppStyle(const char *taskname)
		{
			int indexTask = SearchTaskList_by_name(taskname);
			if(indexTask==-1)
			{
				PERR("ChargeAppStyle:taskname not exit");
				return true;
			}
			int indexUser = SearchUserList_by_name(defaultusername);
			if(Task_str_list[indexTask].GetTaskStyle() == TASKSTYLENORMAL)
			{
				return true;
			}
			else if(Task_str_list[indexTask].GetTaskStyle() == User_str_list[indexUser].GetUserStyle())
			{
				return true;
			}
			else
			{
				return false;
			}

		}
                int SetTaskID(const char *taskname, int taskid)
		{
			//PERR("monitor_main settaskid");
			//PERR("monitor_main settaskid taskname= %s",taskname);
			int index = SearchTaskList_by_name(taskname);
			if(index == -1)
			{
				PERR("taskname not right");
				return -1;
			}
			else
			{
				if(Task_str_list[index].GetTaskID() == 0)
				{
					Task_str_list[index].SetTaskID(taskid);
					return taskid;
				}
				else
				{
					return Task_str_list[index].GetTaskID();
				}
				//PERR("task %d :id %d",index,Task_str_list[index].Task_id);
				//return true;
			}
		}
                bool SetTaskStrategyByid(Task_strategy m_task)
		{
			int index = SearchTaskList_by_id(m_task.GetTaskID());
			if(index==-1)
			{
				PERR("taskid not right");
				return false;
			}
			else
			{
				Task_str_list[index]=m_task;
				return true;
			}

		}
		bool AddAuditInfo(const char* username,const char* operation,bool op_result,const char* _time,int cur_level)
		{
			if(!Genode::strcmp(operation,"user login"))
			{
				if(op_result)
				{
					Genode::strncpy(defaultusername,username,Genode::strlen(username)+1);
					m_cur_level = cur_level;
				}
				else
				{
					AuditInfo temp(username,"","",operation,_time,0,0,0,0,0,op_result);
					audit_base[m_auditnum] = temp;
					m_auditnum = (m_auditnum+1)%MAX_AUDIT_INFO; 
					return true;
				}
				//PERR("1username: %s \n defaultusername: %s \n operation: %s \n time: %s",username,defaultusername,operation,_time);
				
				//PERR("2username: %s \n defaultusername: %s \n operation: %s \n time: %s",username,defaultusername,operation,_time);
			}
			AuditInfo temp(defaultusername,"","",operation,_time,0,0,0,0,0,op_result);
			audit_base[m_auditnum] = temp;
			m_auditnum = (m_auditnum+1)%MAX_AUDIT_INFO; 
			//PERR("3username: %s \n defaultusername: %s \n operation: %s \n time: %s",username,defaultusername,operation,_time);
			return true;               
		}
                bool SetTaskStrategyByname(Task_strategy m_task)
		{
			int index = SearchTaskList_by_name(m_task.GetTaskName());
			if(index==-1)
			{
				PERR("taskname not exit");
				return false;
			}
			else
			{
				m_task.SetTaskOwnerName(Task_str_list[index].GetTaskOwnerName());
				Task_str_list[index]=m_task;
				return true;
			}
		}
                
                bool InsertTaskStrategy(Task_strategy m_task)
		{
			if(m_str_list==MAX_TASK_NUM)
			{
				PERR("tasklist is full");
				return false;
			}
			else
			{
				if(SearchTaskList_by_name(m_task.GetTaskName())!=-1)
				{
					PERR("task already exit");
					return false;
				}
				//PERR("GetTaskblprange = %s",m_task.BLP_RANGE);
				//PERR("GetTaskbibarange = %s",m_task.BIBA_RANGE);
				m_task.SetTaskOwnerName(defaultusername);
				Task_str_list[m_str_list++] = m_task;
				return true;
			}
		}
		Task_strategy GetTaskStrategyByname(const char* taskname)
		{
			int index = SearchTaskList_by_name(taskname);
			if(index==-1)
			{
				PERR("taskname not exit");
				Task_strategy Task("","",0,0,0,NULL,NULL);
				return Task;
			}
			else
			{				
				//PERR("GetTaskblprange = %s",Task_str_list[index].BLP_RANGE);
				//PERR("GetTaskbibarange = %s",Task_str_list[index].BIBA_RANGE);
				return Task_str_list[index];
			}

		}
                bool SetUserStrategyByname(User_strategy m_task)
		{
			int index = SearchUserList_by_name(m_task.GetUserName());
			if(index==-1)
			{
				PERR("username not exit");
				return false;
			}
			else
			{
				User_str_list[index]=m_task;
				return true;
			}
		}
                
                bool InsertUserStrategy(User_strategy m_task)
		{
			if(m_user_list==MAX_USER_NUM)
			{
				PERR("userlist is full");
				return false;
			}
			else
			{
				if(SearchUserList_by_name(m_task.GetUserName())!=-1)
				{
					PERR("user already exit");
					return false;
				}
				//int m_low,m_high;
				//m_task.GetUserBlpLevel(m_low,m_high);
				//PERR("blp_low:%d,blp_high:%d",m_low,m_high);
				//m_task.GetUserBibaLevel(m_low,m_high);
				//PERR("biba_low:%d,biba_high:%d",m_low,m_high);

				User_str_list[m_user_list++] = m_task;
				return true;
			}
		}
		User_strategy GetUserStrategyByname(const char* username)
		{
			int index = SearchUserList_by_name(username);
			if(index==-1)
			{
				PERR("username not exit");
				User_strategy user("",0,0,0,0,0,NULL,NULL);
				return user;
			}
			else
			{	
				return User_str_list[index];
			}
		}
		bool Grant_Task_Cap(const char* user,const char* taskname)
		{
			int index = SearchTaskList_by_name(taskname);
			if(!Genode::strcmp(Task_str_list[index].GetTaskOwnerName(),defaultusername))
			{
				index = SearchUserList_by_name(user);
				User_str_list[index].AddUserTaskCap(taskname);
				return true;	
			}
			else
			{
				PERR("curuser have no right");
				return false;
			}
		}
		bool Grant_Service_Cap(const char* user,const char* taskname ,const char* servicename)
		{	
			int index = SearchTaskList_by_name(taskname);
			if(!Genode::strcmp(Task_str_list[index].GetTaskOwnerName(),defaultusername))
			{
				index = SearchUserList_by_name(user);
				User_str_list[index].AddUserServiceCap(servicename);
				return true;	
			}
			else
			{
				PERR("curuser have no right");
				return false;
			}
		}
		bool Del_Task_Cap(const char* user,const char* taskname)
		{
			int index = SearchTaskList_by_name(taskname);
			if(!Genode::strcmp(Task_str_list[index].GetTaskOwnerName(),defaultusername))
			{
				index = SearchUserList_by_name(user);
				User_str_list[index].DelUserTaskCap(taskname);
				return true;	
			}
			else
			{
				PERR("curuser have no right");
				return false;
			}
		}
		bool Del_Service_Cap(const char* user,const char* taskname ,const char* Servicename)
		{	
			int index = SearchTaskList_by_name(taskname);
			if(!Genode::strcmp(Task_str_list[index].GetTaskOwnerName(),defaultusername))
			{
				index = SearchUserList_by_name(user);
				User_str_list[index].DelUserServiceCap(Servicename);
				return true;	
			}
			else
			{
				PERR("curuser have no right");
				return false;
			}
		}
		char* Get_User_Task_Cap(const char* username,int _index)
		{
			int index = SearchUserList_by_name(username);
			return User_str_list[index].GetUserTaskCap(_index);
		}
		char* Get_User_Service_Cap(const char* username,int _index)
		{
			int index = SearchUserList_by_name(username);
			return User_str_list[index].GetUserServiceCap(_index);
		}
		int Get_User_Task_Cap_Num(const char* username)
		{
			int index = SearchUserList_by_name(username);
			return User_str_list[index].GetUserTaskCapNum();
		}
		int Get_User_Service_Cap_Num(const char* username)
		{
			int index = SearchUserList_by_name(username);
			return User_str_list[index].GetUserServiceCapNum();
		}

                int cap_judge(const char* servicename)
		{
			int index = SearchUserList_by_name(defaultusername);
			int m_cap_num = User_str_list[index].GetUserServiceCapNum();
			for(int i=0;i<m_cap_num;i++)
			{
				if(!Genode::strcmp(User_str_list[index].GetUserServiceCap(i),servicename))
				{
					return 1;
				}
			}
			return -1;			
		}
                int monitor_judge(int task_s, int task_d, const char *interfacename,int op,const char *_time)
                {
                    int m_indexs = SearchTaskList_by_id(task_s);
                    int m_indexd = SearchTaskList_by_id(task_d);

                    if((m_indexs==-1)||(m_indexd==-1))
                    {
                       //  PERR("task %d,%d is not being control",task_s,task_d);
                         return 1;
                    }
                    //int result;
		    int op_code =op;
		  // GetTaskInterfaceOPCODE(m_indexd,interfacename,op);
                    //int op_code = Task_str_list[m_d].MAC_TASKOP[op];

			
                    int blp_result = blp_judge(m_indexs,m_indexd,false);
                    int biba_result = biba_judge(m_indexs,m_indexd,false);
                    bool judge_result;
                    
		    PERR("blp_result :%d,biba_result :%d",blp_result,biba_result);

                    if(op_code==OPCODEREAD)
                    {
                   
			if((blp_result==0||blp_result==1)&&(biba_result==0||biba_result==-1))
			{
                                
				judge_result = true;
			}
			else
			{
				//PERR("blp_result :%d,biba_result :%d",blp_result,biba_result);
				//PERR("strategy not match,permission deny");
				judge_result = false;    
			}
                    }
                    else if(op_code==OPCODEWRITE)
		    {
                        if((blp_result==0||blp_result==-1)&&(biba_result==0||biba_result==1))
			{
				judge_result = true;
			}
			else
			{
			//	PERR("blp_result :%d,biba_result :%d",blp_result,biba_result);
			//	PERR("strategy not match,permission deny"); 
				judge_result = false;    
			}
             
		    }
		    else if(op_code==OPCODEREADWRITE)
                    {
                        if((blp_result==0)&&(biba_result==0))
			{
				judge_result = true;
			}
			else
			{
			//	PERR("blp_result :%d,biba_result :%d",blp_result,biba_result);
			//	PERR("strategy not match,permission deny");
				judge_result = false;     
			}
                    }
		    else if(op_code==OPCODENOCONCERN)
		    {
			 judge_result = true;     
		    }
                    else
		    {
			 //PERR("opcode or interface not right");
			 judge_result = false; 
		    }

		    AddAuditInfo(m_indexs,m_indexd,interfacename,op,judge_result,_time);

		    if(judge_result)
		    {
			return 1;
		    }
		    else
    	    	    {
			return 0;
		    }
                
                 }
};


#endif
