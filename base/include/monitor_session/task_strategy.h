#ifndef _TASK_STRATEGY_H_
#define _TASK_STRATEGY_H_


#define  MAX_STRATEGY_RANGE              50
#define  MAX_TASKINTERFACE_NUM            5
#define  MAX_TASKINTERFACE_OP_NUM        10

#define  OPUNKNOWN           4
#define  OPCODEREAD          1
#define  OPCODEWRITE         2
#define  OPCODEREADWRITE     3
#define  OPCODENOCONCERN     0

#define  TASKSTYLENORMAL     0
#define  TASKSTYLEADMIN      1
#define  TASKSTYLEAUDIT      2
#define  TASKSTYLESECURE     3

#include <util/string.h>

class Task_strategy
{
     private:
	char Task_Name[20];
        int Task_id;

	int BLP_LEVEL;
	int BIBA_LEVEL;
	char BLP_RANGE[MAX_STRATEGY_RANGE];
	char BIBA_RANGE[MAX_STRATEGY_RANGE];
	
	int Task_style;
	char Task_owner[20];
	void Init_function()
	{
		Genode::memset(Task_Name,0,20);
		Genode::memset(Task_owner,0,20);
		BLP_LEVEL=0;
		BIBA_LEVEL=0;
                Task_id=0;
		
		Task_style = TASKSTYLENORMAL;

		Genode::memset(BLP_RANGE,0,MAX_STRATEGY_RANGE);
		Genode::memset(BIBA_RANGE,0,MAX_STRATEGY_RANGE);
		/*for(int j=0;j<MAX_TASKINTERFACE_NUM;j++)
		{
			Genode::memset(Task_Interface_Name[j],0,20);
		        for(int m=0;m<MAX_TASKINTERFACE_OP_NUM;m++)
			{
				Task_Interface_OP[j][m]=OPUNKNOWN;
			}
		}*/

	}

        void sort_strategy_range(Task_strategy &a)
        {
		int i;
                for(i=0;i<MAX_STRATEGY_RANGE;)
                {
                         if(a.BLP_RANGE[i]!=0)
                         {
				i++;
			 }
                         else
			 {
				break;
			 }
                }
		char iTemp;
		char iPos;
		for(int m=0;m<i-1;m++)
		{
			iTemp = a.BLP_RANGE[m];
			iPos = m;
			for(int j=m+1;j<i;j++)
			{
				if(a.BLP_RANGE[j]<iTemp)
				{
					iTemp = a.BLP_RANGE[j];
					iPos = j;
				}
			}
			a.BLP_RANGE[iPos] = a.BLP_RANGE[m];
			a.BLP_RANGE[m] = iTemp;
		}


                for(i=0;i<MAX_STRATEGY_RANGE;)
                {
                         if(a.BIBA_RANGE[i]!=0)
                         {
				i++;
			 }
                         else
			 {
				break;
			 }
                }

		for(int m=0;m<i-1;m++)
		{
			iTemp = a.BIBA_RANGE[m];
			iPos = m;
			for(int j=m+1;j<i;j++)
			{
				if(a.BIBA_RANGE[j]<iTemp)
				{
					iTemp = a.BIBA_RANGE[j];
					iPos = j;
				}
			}
			a.BIBA_RANGE[iPos] = a.BIBA_RANGE[m];
			a.BIBA_RANGE[m] = iTemp;
		}
    
	}
     public:

	char* GetTaskName()
	{
		return Task_Name;
	}

	void SetTaskID(int _id)
	{
		Task_id = _id;
	}
	int GetTaskID()
	{
		return Task_id;
	}
	char* GetTaskOwnerName()
	{
		return Task_owner;
	}
	void SetTaskOwnerName(char* owner_name)
	{
		if(owner_name!=NULL)
		{
			Genode::strncpy(Task_owner,owner_name,Genode::strlen(owner_name)+1);
		}	
	}
        void SetTaskBlpLevel(int _level)
	{
		BLP_LEVEL = _level;	
	}

	int GetTaskBlpLevel()
	{
		return BLP_LEVEL;
	}

	void SetTaskBibaLevel(int _level)
	{
		BIBA_LEVEL = _level;		
	}

	int GetTaskBibaLevel()
	{
		return  BIBA_LEVEL;
	}

	void SetTaskBlpRange(const char* blp_range)
	{
		if(blp_range!=NULL)
		{
			Genode::strncpy(BLP_RANGE,blp_range,Genode::strlen(blp_range)+1);
		}
		sort_strategy_range(*this);
	}

	char* GetTaskBlpRange()
	{
		return BLP_RANGE;
	}

	void SetTaskBibaRange(const char* biba_range)
	{
		if(biba_range!=NULL)
		{
			Genode::strncpy(BIBA_RANGE,biba_range,Genode::strlen(biba_range)+1);
		}
		sort_strategy_range(*this);
	}

	char* GetTaskBibaRange()
	{
		return BIBA_RANGE;
	}

	int GetTaskStyle()
	{
		return Task_style;
	}

        //char Task_Interface_Name[MAX_TASKINTERFACE_NUM][20];
        //int Task_Interface_OP[MAX_TASKINTERFACE_NUM][MAX_TASKINTERFACE_OP_NUM];

	Task_strategy()
        {
		Init_function();
        }

        Task_strategy(const char* _name,const char* owner_name,int _style,int blp_level,int biba_level,const char* blp_range,const char* biba_range)
        {
		Init_function();
		Genode::strncpy(Task_Name,_name,Genode::strlen(_name)+1);
		Genode::strncpy(Task_owner,owner_name,Genode::strlen(owner_name)+1);

		BLP_LEVEL = blp_level;
		BIBA_LEVEL = biba_level;
		Task_style = _style;
		if(blp_range!=NULL)
		{
			Genode::strncpy(BLP_RANGE,blp_range,Genode::strlen(blp_range)+1);
		}
		if(biba_range!=NULL)
		{
			Genode::strncpy(BIBA_RANGE,biba_range,Genode::strlen(biba_range)+1);
		}
		/*if(op!=NULL)
		{		
			for(int n=0;n<MAX_TASKINTERFACE_NUM;n++)
			{
				for(int m=0;m<MAX_TASKINTERFACE_OP_NUM;m++)
				{
					Task_Interface_OP[n][m]=op[n][m];
				}
			}
		}
                if(interfacename!=NULL)
		{
			for(int n=0;n<MAX_TASKINTERFACE_NUM;n++)
			{
				Genode::strncpy(Task_Interface_Name[n],interfacename[n],Genode::strlen(interfacename[n])+1);
			}

		}*/
		sort_strategy_range(*this);
        }

	Task_strategy& operator=(Task_strategy& a)
        {
		Genode::strncpy(this->Task_Name,a.Task_Name,Genode::strlen(a.Task_Name)+1);
		Genode::strncpy(this->Task_owner,a.Task_owner,Genode::strlen(a.Task_owner)+1);
		this->BLP_LEVEL= a.BLP_LEVEL;
		this->BIBA_LEVEL = a.BIBA_LEVEL;
		this->Task_style = a.Task_style;

		/*for(int i=0;i<MAX_STRATEGY_RANGE;i++)
		{
			this->BLP_RANGE[i]=a.BLP_RANGE[i];
			this->BIBA_RANGE[i]=a.BIBA_RANGE[i];
		}*/
		Genode::strncpy(this->BLP_RANGE,a.BLP_RANGE,Genode::strlen(a.BLP_RANGE)+1);
		Genode::strncpy(this->BIBA_RANGE,a.BIBA_RANGE,Genode::strlen(a.BIBA_RANGE)+1);
		/*for(int n=0;n<MAX_TASKINTERFACE_NUM;n++)
		{
			Genode::strncpy(this->Task_Interface_Name[n],a.Task_Interface_Name[n],Genode::strlen(a.Task_Interface_Name[n])+1);
			for(int m=0;m<MAX_TASKINTERFACE_OP_NUM;m++)
			{
				this->Task_Interface_OP[n][m]=a.Task_Interface_OP[n][m];
			}
		}*/
		return *this;
        }
};

#endif
