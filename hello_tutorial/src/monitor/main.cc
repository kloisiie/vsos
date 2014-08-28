/*
 * \brief  Test client for the Monitor RPC interface
 * \author Björn Döbel
 * \date   2008-03-20
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/env.h>
#include <base/printf.h>
#include <monitor_session/client.h>
#include <monitor_session/connection.h>
#include <base/rpc_args.h>
#include <monitor_session/auditinfo.h>
#include <monitor_session/task_strategy.h>
#include <monitor_session/user_strategy.h>
//#include <base/rpc_args.h>

using namespace Genode;

void func(Genode::Monitor_Session::Strategy_string& bb)
{
	bb = "test_new";
}
void func1(Genode::Monitor_Session::Strategy_string bb)
{
	PDBG("bb = %s ", bb.base());
	PDBG("bbsize = %d",bb.size());
}
//typedef Rpc_in_buffer<26> Audit_string;
int main(void)
{       
	PDBG("monitor client");
	Monitor_Connection h;

	//while (1) {
	
        ///char *corename = "core";
        int m_blp;
	int m_biba;

	Genode::Monitor_Session::Strategy_out_string blprange;
	Genode::Monitor_Session::Strategy_out_string bibarange;
	Genode::Monitor_Session::Strategy_out_string ownername;
	Genode::Monitor_Session::Strategy_out_string capname;
/*      char bbtest[10]={13,52,35,34};
	char bbtest1[10]={'a','b','d','d'};
	func1(bbtest1);
	Genode::Monitor_Session::Strategy_out_string blprange;
        PDBG("blprange = %s ", blprange.base());
	func(blprange);
	PDBG("blprange = %s ", blprange.base());
	Genode::Monitor_Session::Strategy_out_string bibarange;
*/


//___________________
//task interface test
	bool b_result;

/*	char taskblp[50]={55};                   
	char taskbiba[50]={56};
        b_result = h.SetTaskStrategyByname("new_clientnew",true,TASKSTYLENORMAL,3,1,taskblp,taskbiba);
        if(b_result)
	{
                b_result = h.GetTaskStrategyByname("new_clientnew",ownername,m_blp,m_biba,blprange,bibarange);
	        if(b_result)
		{
			PDBG("task blp = %d ", m_blp);
			PDBG("task biba = %d ", m_biba);
			PDBG("ownername = %s ", ownername.getball());
			for(int i=0;i<blprange.size();i++)
			{
				PDBG("blprange %d = %d", i,blprange.getball()[i]);
			}
			for(int j=0;j<bibarange.size();j++)
			{
				PDBG("bibarange %d = %d ",j, bibarange.getball()[j]);
			}

		}
	}
*/
	char task2blp[50]={12,24,14,99,47,100,76};                   
	char task2biba[50]={12,16,18};

	h.AddAuditInfo("admin","user login",true,2);

	b_result = h.SetTaskStrategyByname("new_client",true,TASKSTYLENORMAL,1,1,task2blp,task2biba);

        if(b_result)
	{
                b_result = h.GetTaskStrategyByname("new_client",ownername,m_blp,m_biba,blprange,bibarange);
	        if(b_result)
		{
			PDBG("task blp = %d ", m_blp);
			PDBG("task biba = %d ", m_biba);
			PDBG("ownername = %s ", ownername.getball());
			for(int i=0;i<blprange.size();i++)
			{
				PDBG("blprange %d = %d", i,blprange.getball()[i]);
			}
			for(int j=0;j<bibarange.size();j++)
			{
				PDBG("bibarange %d = %d ",j, bibarange.getball()[j]);
			}
		}
	}
	
	h.AddAuditInfo("audit","user login",true,2);
	char task3blp[50]={12,24,14,99,47,100,76};                   
	char task3biba[50]={12,16,18};
	b_result = h.SetTaskStrategyByname("new_server",true,TASKSTYLENORMAL,2,1,task3blp,task3biba);

	h.SetTaskID("new_client", 41);
	h.SetTaskID("new_server", 42);

	h.AddAuditInfo("admin","user login",true,1);
        int cap_judge_result;
	b_result = h.ChargeApp("lowlevel_app",true);
	if(b_result)
	{
		PERR("lowlevel_app level ok");
	}
	else
	{
		PERR("lowlevel_app level wrong");
	}

	b_result = h.ChargeApp("dos_server",true);
	if(b_result)
	{
		PERR("dos_server level ok");
	}
	else
	{
		PERR("dos_server level wrong");
	}

	h.AddAuditInfo("audit","user login",true,1);

	b_result = h.ChargeApp("lowlevel_app",true);
	if(b_result)
	{
		PERR("lowlevel_app level ok");
	}
	else
	{
		PERR("lowlevel_app level wrong");
	}

	b_result = h.ChargeApp("dos_server",true);
	if(b_result)
	{
		PERR("dos_server level ok");
	}
	else
	{
		PERR("dos_server level wrong");
	}
        

	int cap_index =0;
	cap_judge_result = h.cap_judge("Confidential");

	PERR("cap judge result :%d",cap_judge_result);


	b_result = h.Grant_cap(true,"audit","lowlevel_app","");
	if(b_result)
	{
		PERR("grant ok");
	}
	else
	{
		PERR("grant wrong");
	}
	b_result = h.Grant_cap(false,"audit","midlevel_process","Confidential");
	if(b_result)
	{
		PERR("grant ok");
	}
	else
	{
		PERR("grant wrong");
	}

	int m_tasknum;
	int m_servicenum;
   	m_tasknum = h.Get_user_cap_num(true,"admin");
	PERR("task cap num:%d ",m_tasknum);

	m_servicenum = h.Get_user_cap_num(false,"admin");
	PERR("service cap num:%d ",m_servicenum);
	{
		for(int m_i=0;m_i<m_tasknum;m_i++)
		{
			m_blp = 55;
			m_biba = m_i;
			h.GetTaskStrategyByname("admin",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin task cap %d name = %s ",m_i, ownername.getball());
		}
		for(int m_j=0;m_j<m_servicenum;m_j++)
		{
			m_blp = 56;
			m_biba = m_j;
			h.GetTaskStrategyByname("admin",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin service cap %d name = %s ",m_j, ownername.getball());
		}
	}
   	m_tasknum = h.Get_user_cap_num(true,"audit");
	PERR("task cap num:%d ",m_tasknum);

	m_servicenum = h.Get_user_cap_num(false,"audit");
	PERR("service cap num:%d ",m_servicenum);
	{
		for(int m_i=0;m_i<m_tasknum;m_i++)
		{
			m_blp = 55;
			m_biba = m_i;
			h.GetTaskStrategyByname("audit",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin task cap %d name = %s ",m_i, ownername.getball());
		}
		for(int m_j=0;m_j<m_servicenum;m_j++)
		{
			m_blp = 56;
			m_biba = m_j;
			h.GetTaskStrategyByname("audit",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin service cap %d name = %s ",m_j, ownername.getball());
		}
	}
	//h.GetTaskStrategyByname("new_client",ownername,m_blp,m_biba,blprange,bibarange);

	h.AddAuditInfo("admin","user login",true,1);

	b_result = h.Grant_cap(true,"audit","lowlevel_app","");
	if(b_result)
	{
		PERR("grant ok");
	}
	else
	{
		PERR("grant wrong");
	}
	b_result = h.Grant_cap(false,"audit","midlevel_process","Confidential");
	if(b_result)
	{
		PERR("grant ok");
	}
	else
	{
		PERR("grant wrong");
	}

   	m_tasknum = h.Get_user_cap_num(true,"admin");
	PERR("task cap num:%d ",m_tasknum);

	m_servicenum = h.Get_user_cap_num(false,"admin");
	PERR("service cap num:%d ",m_servicenum);
	{
		for(int m_i=0;m_i<m_tasknum;m_i++)
		{
			m_blp = 55;
			m_biba = m_i;
			h.GetTaskStrategyByname("admin",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin task cap %d name = %s ",m_i, ownername.getball());
		}
		for(int m_j=0;m_j<m_servicenum;m_j++)
		{
			m_blp = 56;
			m_biba = m_j;
			h.GetTaskStrategyByname("admin",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin service cap %d name = %s ",m_j, ownername.getball());
		}
	}
   	m_tasknum = h.Get_user_cap_num(true,"audit");
	PERR("task cap num:%d ",m_tasknum);

	m_servicenum = h.Get_user_cap_num(false,"audit");
	PERR("service cap num:%d ",m_servicenum);
	{
		for(int m_i=0;m_i<m_tasknum;m_i++)
		{
			m_blp = 55;
			m_biba = m_i;
			h.GetTaskStrategyByname("audit",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin task cap %d name = %s ",m_i, ownername.getball());
		}
		for(int m_j=0;m_j<m_servicenum;m_j++)
		{
			m_blp = 56;
			m_biba = m_j;
			h.GetTaskStrategyByname("audit",ownername,m_blp,m_biba,blprange,bibarange);
			PDBG("admin service cap %d name = %s ",m_j, ownername.getball());
		}
	}
	h.AddAuditInfo("audit","user login",true,1);


	b_result = h.ChargeApp("lowlevel_app",true);
	if(b_result)
	{
		PERR("lowlevel_app level ok");
	}
	else
	{
		PERR("lowlevel_app level wrong");
	}

	b_result = h.ChargeApp("dos_server",true);
	if(b_result)
	{
		PERR("dos_server level ok");
	}
	else
	{
		PERR("dos_server level wrong");
	}

	cap_judge_result = h.cap_judge("Confidential");

	PERR("cap judge result :%d",cap_judge_result);

/*	int m_tasknum;
	int m_servicenum;
   	m_tasknum = h.Get_user_cap_num(true,"admin");
	PERR("task cap num:%d ",m_tasknum);

	m_servicenum = h.Get_user_cap_num(false,"admin");
	PERR("service cap num:%d ",m_servicenum);


  	m_tasknum = h.Get_user_cap_num(true,"audit");
	PERR("task cap num:%d ",m_tasknum);
   	m_servicenum = h.Get_user_cap_num(false,"audit");
	PERR("service cap num:%d ",m_servicenum);
*/

/*	b_result = h.ChargeApp("lowlevel_app",false);
	if(b_result)
	{
		PERR("lowlevel_app style ok");
	}
	else
	{
		PERR("lowlevel_app style wrong");
	}

	b_result = h.ChargeApp("lowlevel_app",true);
	if(b_result)
	{
		PERR("lowlevel_app level ok");
	}
	else
	{
		PERR("lowlevel_app level wrong");
	}

	b_result = h.ChargeApp("dos_server",true);
	if(b_result)
	{
		PERR("dos_server level ok");
	}
	else
	{
		PERR("dos_server level wrong");
	}

	h.AddAuditInfo("audit","user login",true,1);

	b_result = h.ChargeApp("lowlevel_app",true);
	if(b_result)
	{
		PERR("lowlevel_app level ok");
	}
	else
	{
		PERR("lowlevel_app level wrong");
	}

	b_result = h.ChargeApp("dos_server",true);
	if(b_result)
	{
		PERR("dos_server level ok");
	}
	else
	{
		PERR("dos_server level wrong");
	}

*/

/*	b_result = h.monitor_judge(41,42,"read",1);
	if(b_result)
	{
		PERR("read ok");
	}
	else
	{
		PERR("read wrong");
	}
	b_result = h.monitor_judge(41,42,"write",2);
	if(b_result)
	{
		PERR("write ok");
	}
	else
	{
		PERR("write wrong");
	}
	b_result = h.monitor_judge(41,42,"readwrite",3);
	if(b_result)
	{
		PERR("readwrite ok");
	}
	else
	{
		PERR("readwrite wrong");
	}


	h.AddAuditInfo("admin","user login",true,2);

	b_result = h.monitor_judge(41,42,"read",1);
	if(b_result)
	{
		PERR("read ok");
	}
	else
	{
		PERR("read wrong");
	}
	b_result = h.monitor_judge(41,42,"write",2);
	if(b_result)
	{
		PERR("write ok");
	}
	else
	{
		PERR("write wrong");
	}
	b_result = h.monitor_judge(41,42,"readwrite",3);
	if(b_result)
	{
		PERR("readwrite ok");
	}
	else
	{
		PERR("readwrite wrong");
	}


	h.AddAuditInfo("admin","user login",true,3);

	b_result = h.monitor_judge(41,42,"read",1);
	if(b_result)
	{
		PERR("read ok");
	}
	else
	{
		PERR("read wrong");
	}
	b_result = h.monitor_judge(41,42,"write",2);
	if(b_result)
	{
		PERR("write ok");
	}
	else
	{
		PERR("write wrong");
	}
	b_result = h.monitor_judge(41,42,"readwrite",3);
	if(b_result)
	{
		PERR("readwrite ok");
	}
	else
	{
		PERR("readwrite wrong");
	}
*/


/*


	b_result = h.monitor_judge(41,42,"read",1,"2014");
	if(b_result)
	{
		PERR("read ok");
	}
	else
	{
		PERR("read wrong");
	}
	b_result = h.monitor_judge(41,42,"write",2,"2014");
	if(b_result)
	{
		PERR("write ok");
	}
	else
	{
		PERR("write wrong");
	}
	b_result = h.monitor_judge(41,42,"readwrite",3,"2014");
	if(b_result)
	{
		PERR("readwrite ok");
	}
	else
	{
		PERR("readwrite wrong");
	}


	b_result = h.SetTaskStrategyByname("new_server",false,TASKSTYLENORMAL,1,3,task3blp,task3biba);

	b_result = h.monitor_judge(41,42,"read",1,"2014");
	if(b_result)
	{
		PERR("read ok");
	}
	else
	{
		PERR("read wrong");
	}
	b_result = h.monitor_judge(41,42,"write",2,"2014");
	if(b_result)
	{
		PERR("write ok");
	}
	else
	{
		PERR("write wrong");
	}
	b_result = h.monitor_judge(41,42,"readwrite",3,"2014");
	if(b_result)
	{
		PERR("readwrite ok");
	}
	else
	{
		PERR("readwrite wrong");
	}
*/


/*	
	b_result = h.SetTaskStrategyByname("new_client",false,TASKSTYLENORMAL,3,1,task3blp,task3biba);
	b_result = h.SetTaskStrategyByname("new_server",false,TASKSTYLENORMAL,1,3,task3blp,task3biba);

	b_result = h.monitor_judge(41,42,"read",1,"2014");
	if(b_result)
	{
		PERR("read ok");
	}
	else
	{
		PERR("read wrong");
	}
	b_result = h.monitor_judge(41,42,"write",2,"2014");
	if(b_result)
	{
		PERR("write ok");
	}
	else
	{
		PERR("write wrong");
	}
	b_result = h.monitor_judge(41,42,"readwrite",3,"2014");
	if(b_result)
	{
		PERR("readwrite ok");
	}
	else
	{
		PERR("readwrite wrong");
	}
*/

/*	char task4blp[50]={12,24,14,99,100};                   
	char task4biba[50]={12,16,22};
	char task5blp[50]={12,24,14,99,100,101};                   
	char task5biba[50]={12,16};
	b_result = h.SetTaskStrategyByname("new_client",false,TASKSTYLENORMAL,2,2,task4blp,task4biba);
	b_result = h.SetTaskStrategyByname("new_server",false,TASKSTYLENORMAL,2,2,task5blp,task5biba);
	b_result = h.monitor_judge(41,42,"read",1,"2014");
	if(b_result)
	{
		PERR("read ok");
	}
	else
	{
		PERR("read wrong");
	}
	        b_result = h.GetTaskStrategyByname("new_client",m_blp,m_biba,blprange,bibarange);
	        if(b_result)
		{
			PDBG("task blp = %d ", m_blp);
			PDBG("task biba = %d ", m_biba);
			for(int i=0;i<blprange.size();i++)
			{
				PDBG("blprange %d = %d", i,blprange.getball()[i]);
			}
			for(int j=0;j<bibarange.size();j++)
			{
				PDBG("bibarange %d = %d ",j, bibarange.getball()[j]);
			}
		}
	        b_result = h.GetTaskStrategyByname("new_server",m_blp,m_biba,blprange,bibarange);
	        if(b_result)
		{
			PDBG("task blp = %d ", m_blp);
			PDBG("task biba = %d ", m_biba);
			for(int i=0;i<blprange.size();i++)
			{
				PDBG("blprange %d = %d", i,blprange.getball()[i]);
			}
			for(int j=0;j<bibarange.size();j++)
			{
				PDBG("bibarange %d = %d ",j, bibarange.getball()[j]);
			}
		}
	b_result = h.monitor_judge(41,42,"write",2,"2014");
	if(b_result)
	{
		PERR("write ok");
	}
	else
	{
		PERR("write wrong");
	}
	b_result = h.monitor_judge(41,42,"readwrite",3,"2014");
	if(b_result)
	{
		PERR("readwrite ok");
	}
	else
	{
		PERR("readwrite wrong");
	}
*/

//___________________________________________
//user interface test
/*
	char userblp[50]={57,54,24,4};                   
	char userbiba[50]={58,7};
        bool b_result = h.SetUserStrategyByname("new_user",true,USERNORMAL,12,11,userblp,userbiba);
        if(b_result)
	{
                b_result = h.GetUserStrategyByname("new_user",m_blp,m_biba,blprange,bibarange);
	        if(b_result)
		{
			PDBG("user blp_low = %d,blp_high = %d ", m_blp/10,m_blp%10);
			PDBG("user biba_low = %d,biba_high =%d ", m_biba/10,m_biba%10);
			for(int i=0;i<blprange.size();i++)
			{
				PDBG("blprange %d = %d", i,blprange.getball()[i]);
			}
			for(int j=0;j<bibarange.size();j++)
			{
				PDBG("bibarange %d = %d ",j, bibarange.getball()[j]);
			}

		}
	}
	char user2blp[50]={12,24,14,99,100};                   
	char user2biba[50]={12,16};

	b_result = h.SetUserStrategyByname("new_user",false,USERNORMAL,1,3,user2blp,user2biba);

        if(b_result)
	{
                b_result = h.GetUserStrategyByname("new_user",m_blp,m_biba,blprange,bibarange);
	        if(b_result)
		{
			PDBG("user blp_low = %d,blp_high = %d ", m_blp/10,m_blp%10);
			PDBG("user biba_low = %d,biba_high =%d ", m_biba/10,m_biba%10);
			for(int i=0;i<blprange.size();i++)
			{
				PDBG("blprange %d = %d", i,blprange.getball()[i]);
			}
			for(int j=0;j<bibarange.size();j++)
			{
				PDBG("bibarange %d = %d ",j, bibarange.getball()[j]);
			}
		}
	}

*/

//_________________________________________________________________
//change user and chargeapp test

/*	
	bool b_result = h.AddAuditInfo("audit","user login",true,"2014-12-12 12:12:12");
	if(b_result)
	{
		b_result = h.ChargeApp("log",false);
		if(b_result)
		{
			PERR("task:%s style right","log");
		}
		else
		{
			PERR("task:%s style wrong","log");
		}
		b_result = h.ChargeApp("highlevel_app",false);
		if(b_result)
		{
			PERR("task:%s style right","highlevel_app");
		}
		else
		{
			PERR("task:%s style wrong","highlevel_app");
		}
		b_result = h.ChargeApp("dos_server",false);
		if(b_result)
		{
			PERR("task:%s style right","dos_server");
		}
		else
		{
			PERR("task:%s style wrong","dos_server");
		}

		b_result = h.ChargeApp("lowlevel_app",true);
		if(b_result)
		{
			PERR("task:%s level right","lowlevel_app");
		}
		else
		{
			PERR("task:%s level wrong","lowlevel_app");
		}

		b_result = h.ChargeApp("midlevel_app",true);
		if(b_result)
		{
			PERR("task:%s level right","midlevel_app");
		}
		else
		{
			PERR("task:%s level wrong","midlevel_app");
		}

		b_result = h.ChargeApp("midlevel_process",true);
		if(b_result)
		{
			PERR("task:%s level right","midlevel_process");
		}
		else
		{
			PERR("task:%s level wrong","midlevel_process");
		}
	}

	b_result = h.AddAuditInfo("admin","user login",true,"2014-12-12 12:12:12");
	if(b_result)
	{
		b_result = h.ChargeApp("log",false);
		if(b_result)
		{
			PERR("task:%s style right","log");
		}
		else
		{
			PERR("task:%s style wrong","log");
		}
		b_result = h.ChargeApp("highlevel_app",false);
		if(b_result)
		{
			PERR("task:%s style right","highlevel_app");
		}
		else
		{
			PERR("task:%s style wrong","highlevel_app");
		}
		b_result = h.ChargeApp("dos_server",false);
		if(b_result)
		{
			PERR("task:%s style right","dos_server");
		}
		else
		{
			PERR("task:%s style wrong","dos_server");
		}

		b_result = h.ChargeApp("lowlevel_app",true);
		if(b_result)
		{
			PERR("task:%s level right","lowlevel_app");
		}
		else
		{
			PERR("task:%s level wrong","lowlevel_app");
		}

		b_result = h.ChargeApp("midlevel_app",true);
		if(b_result)
		{
			PERR("task:%s level right","midlevel_app");
		}
		else
		{
			PERR("task:%s level wrong","midlevel_app");
		}

		b_result = h.ChargeApp("midlevel_process",true);
		if(b_result)
		{
			PERR("task:%s level right","midlevel_process");
		}
		else
		{
			PERR("task:%s level wrong","midlevel_process");
		}
	}	 	


*/


        //b_result = h.InsertTaskStrategy("new_server",1,3,task5blp,task5biba);
        //h.SetTaskID("new_server", 6);

	//int result = h.monitor_judge(5,6,"add",1);
       // PDBG("judge result = %d",result);

       // h.SetTaskStrategyByname("hello_client",1,3,task5blp,task5biba);

	//result = h.monitor_judge(3,4,"add",1);
       // PDBG("judge result = %d",result);
		//int foo = h.add(2, 5);
		//PDBG("Added 2 + 5 = %d", foo);
	//}
//	Dataspace_capability data=h.GetAuditInfo();
//	int temp = 0;
//	AuditInfo *s=(AuditInfo *)env()->rm_session()->attach(data);
/*        while((s)->m_IfRead ==true)
	{
		PDBG("audit username  = %s",s->m_UserName);
		PDBG("audit clientname  = %s",s->m_ClientName);
		PDBG("audit servername  = %s",s->m_ServerName);
		PDBG("audit clientblp  = %d",s->m_ClientBlpLevel);
		PDBG("audit op  = %d",s->m_OpCode);
                s++;
 		//temp = (temp+1)%100;
	}
*/
        //Rpc_in_buffer<56> m_audit;
       // bool bb = h.GetAuditInfo(m_audit);
	//PDBG("audit = %s",m_audit.base());
        //MAC_MONITOR m_monitor;
        //int result = m_monitor.monitor_function("hello_client","hello_server",3);
        //PERR("final resut: %d",result);

	return 0;
}
