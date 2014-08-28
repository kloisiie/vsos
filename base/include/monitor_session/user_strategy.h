#ifndef _USER_STRATEGY_H_
#define _USER_STRATEGY_H_


#define  MAX_STRATEGY_RANGE              50
#define  MAX_TASKINTERFACE_NUM            5
#define  MAX_TASKINTERFACE_OP_NUM        10


#define  USERNORMAL     0
#define  USERADMIN      1
#define  USERAUDIT      2
#define  USERSECURE     3

#define  MAX_CAP_NUM         30
#define  MAX_CAP_NAME_LENGTH 30

#include <util/string.h>
#include <base/snprintf.h>
class User_strategy
{
     private:
	
	char User_Name[20];
        int  User_id;

	int  User_style;

	int BLP_LEVEL_LOW;
	int BLP_LEVEL_HIGH;
	int BIBA_LEVEL_LOW;
	int BIBA_LEVEL_HIGH;

	char BLP_RANGE[MAX_STRATEGY_RANGE];
	char BIBA_RANGE[MAX_STRATEGY_RANGE];

	int User_Task_Cap_num;
	char User_Task_Cap[MAX_CAP_NUM][MAX_CAP_NAME_LENGTH]; 
	int User_Service_Cap_num;
	char User_Service_Cap[MAX_CAP_NUM][MAX_CAP_NAME_LENGTH];

	void Init_function()
	{
		Genode::memset(User_Name,0,20);
		BLP_LEVEL_LOW = 0;
		BLP_LEVEL_HIGH = 0;
		BIBA_LEVEL_LOW = 0;
		BIBA_LEVEL_HIGH = 0;

                User_id=0;
		User_style = USERNORMAL;

		Genode::memset(BLP_RANGE,0,MAX_STRATEGY_RANGE);
		Genode::memset(BIBA_RANGE,0,MAX_STRATEGY_RANGE);

		User_Task_Cap_num = 0;
		User_Service_Cap_num = 0;
		for(int i=0;i<MAX_CAP_NUM;i++)
		{
			Genode::memset(User_Task_Cap[i],0,MAX_CAP_NAME_LENGTH);
			Genode::memset(User_Service_Cap[i],0,MAX_CAP_NAME_LENGTH);
		}
	}

        void sort_strategy_range(User_strategy &a)
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
        bool SearchTaskCap(const char* taskname)
	{
		for(int i=0;i<User_Task_Cap_num;i++)
		{
			if(!Genode::strcmp(User_Task_Cap[i],taskname))
			{
				return true;
			}
		}
		return false;
	}	
	bool SearchServiceCap(const char* servicename)
	{
		for(int i=0;i<User_Service_Cap_num;i++)
		{
			if(!Genode::strcmp(User_Service_Cap[i],servicename))
			{
				return true;
			}
		}
		return false;
	}
	char* GetUserName()
	{
		return User_Name;
	}
	int GetUserID()
	{
		return User_id;
	}
        void SetUserBlpLevel(int _low,int _high)
	{
		BLP_LEVEL_LOW = _low;
		BLP_LEVEL_HIGH = _high;	
	}

	void GetUserBlpLevel(int& _low,int& _high)
	{
		_low = BLP_LEVEL_LOW;
		_high = BLP_LEVEL_HIGH;
	}

	void SetUserBibaLevel(int _low,int _high)
	{
		BIBA_LEVEL_LOW = _low;
		BIBA_LEVEL_HIGH = _high;		
	}

	void GetUserBibaLevel(int& _low,int& _high)
	{
		_low = BIBA_LEVEL_LOW;
		_high = BIBA_LEVEL_HIGH;
	}

	void SetUserBlpRange(const char* blp_range)
	{
		if(blp_range!=NULL)
		{
			Genode::strncpy(BLP_RANGE,blp_range,Genode::strlen(blp_range)+1);
		}
		sort_strategy_range(*this);
	}

	char* GetUserBlpRange()
	{
		return BLP_RANGE;
	}

	void SetUserBibaRange(const char* biba_range)
	{
		if(biba_range!=NULL)
		{
			Genode::strncpy(BIBA_RANGE,biba_range,Genode::strlen(biba_range)+1);
		}
		sort_strategy_range(*this);
	}

	char* GetUserBibaRange()
	{
		return BIBA_RANGE;
	}

	int GetUserStyle()
	{
		return User_style;
	}

	User_strategy()
        {
		Init_function();
        }
	void AddUserTaskCap(const char* taskname)
	{
		if(User_Task_Cap_num < MAX_CAP_NUM)
		{
			bool m_reuslt = SearchTaskCap(taskname);
			if(!m_reuslt)
			{
				Genode::strncpy(User_Task_Cap[User_Task_Cap_num],taskname,Genode::strlen(taskname)+1);
				User_Task_Cap_num++;
			}
		}
		else
		{
			PERR("User taskcap full");
		}
	}
	void DelUserTaskCap(const char* taskname)
	{
		for(int i=0;i<User_Task_Cap_num;i++)
		{
			if(!Genode::strcmp(User_Task_Cap[i],taskname))
			{
				for(int j=i;j<User_Task_Cap_num;j++)
				{
					Genode::strncpy(User_Task_Cap[j],User_Task_Cap[j+1],Genode::strlen(User_Task_Cap[j+1])+1);	
				}
				User_Task_Cap_num--;
				return ;
			}	
		}
		PERR("Del taskcap: taskname not right");
	}
	char* GetUserTaskCap(int index)
	{
		if(index>User_Task_Cap_num)
		{
			PERR("out of range");
			return "";
		}
		else
		{
			return User_Task_Cap[index];
		}
	}
	int GetUserTaskCapNum()
	{
		return User_Task_Cap_num;
	}
	int GetUserServiceCapNum()
	{
		return User_Service_Cap_num;
	}
	void AddUserServiceCap(const char* servicename)
	{
		if(User_Service_Cap_num < MAX_CAP_NUM)
		{
			bool m_reuslt = SearchServiceCap(servicename);
			if(!m_reuslt)
			{
				Genode::strncpy(User_Service_Cap[User_Service_Cap_num],servicename,Genode::strlen(servicename)+1);
				User_Service_Cap_num++;
			}
		}
		else
		{
			PERR("User servicecap full");
		}
	}	
	void DelUserServiceCap(const char* servicename)
	{
		for(int i=0;i<User_Service_Cap_num;i++)
		{
			if(!Genode::strcmp(User_Service_Cap[i],servicename))
			{
				for(int j=i;j<User_Service_Cap_num;j++)
				{
					Genode::strncpy(User_Service_Cap[j],User_Service_Cap[j+1],Genode::strlen(User_Service_Cap[j+1])+1);	
				}
				User_Service_Cap_num--;
				return ;
			}	
		}
		PERR("Del servicecap: servicename not right");
	}
	char* GetUserServiceCap(int index)
	{
		if(index>User_Service_Cap_num)
		{
			PERR("out of range");
			return "";
		}
		else
		{
			return User_Service_Cap[index];
		}	
	}

        User_strategy(const char* _name,int _style,int blp_level_low,int blp_level_high,int biba_level_low,int biba_level_high,const char* blp_range,const char* biba_range)
        {
		Init_function();
		Genode::strncpy(User_Name,_name,Genode::strlen(_name)+1);
		
		User_style = _style;

		BLP_LEVEL_LOW = blp_level_low;
		BLP_LEVEL_HIGH = blp_level_high;
		BIBA_LEVEL_LOW = biba_level_low;
		BIBA_LEVEL_HIGH = biba_level_high;	
		//PERR("blp_low,blp_high:%d,%d,%d,%d",blp_level_low,blp_level_high,biba_level_low,biba_level_high);
		if(blp_range!=NULL)
		{
			Genode::strncpy(BLP_RANGE,blp_range,Genode::strlen(blp_range)+1);
		}
		if(biba_range!=NULL)
		{
			Genode::strncpy(BIBA_RANGE,biba_range,Genode::strlen(biba_range)+1);
		}

		sort_strategy_range(*this);
        }

	User_strategy& operator=(User_strategy& a)
        {
		Genode::strncpy(this->User_Name,a.User_Name,Genode::strlen(a.User_Name)+1);

		this->BLP_LEVEL_LOW= a.BLP_LEVEL_LOW;
		this->BLP_LEVEL_HIGH = a.BLP_LEVEL_HIGH;
		this->BIBA_LEVEL_LOW= a.BIBA_LEVEL_LOW;
		this->BIBA_LEVEL_HIGH = a.BIBA_LEVEL_HIGH;

		this->User_style = a.User_style;

		Genode::strncpy(this->BLP_RANGE,a.BLP_RANGE,Genode::strlen(a.BLP_RANGE)+1);
		Genode::strncpy(this->BIBA_RANGE,a.BIBA_RANGE,Genode::strlen(a.BIBA_RANGE)+1);

		this->User_Task_Cap_num = a.User_Task_Cap_num;
		this->User_Service_Cap_num = a.User_Service_Cap_num;
		for(int i=0;i<a.User_Task_Cap_num;i++)
		{
			Genode::strncpy(this->User_Task_Cap[i],a.GetUserTaskCap(i),Genode::strlen(a.GetUserTaskCap(i))+1);
			
		}
		for(int j=0;j<a.User_Service_Cap_num;j++)
		{
			Genode::strncpy(this->User_Service_Cap[j],a.GetUserServiceCap(j),Genode::strlen(a.GetUserServiceCap(j))+1);
		}
		
		return *this;
        }
};

#endif
