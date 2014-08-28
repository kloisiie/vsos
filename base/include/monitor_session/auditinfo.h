#ifndef _AUDITINFO_H_
#define _AUDITINFO_H_

#define MAX_NAME_LENG 20
#define MAX_OP_LENG   30

#include <util/string.h>
class AuditInfo
{
	public:
	char m_UserName[MAX_NAME_LENG];
	char m_ClientName[MAX_NAME_LENG];
	char m_ServerName[MAX_NAME_LENG];
	char m_OperationName[MAX_OP_LENG];
	char m_time[20];

	int  m_OpCode;
	int  m_ClientBlpLevel;
	int  m_ClientBibaLevel;
 	int  m_ServerBlpLevel;
	int  m_ServerBibaLevel;

	bool m_OpResult;
        bool m_IfRead;
        AuditInfo()
	{
		Genode::memset(m_UserName,0,MAX_NAME_LENG);
		Genode::memset(m_ClientName,0,MAX_NAME_LENG);
		Genode::memset(m_ServerName,0,MAX_NAME_LENG);
		Genode::memset(m_OperationName,0,MAX_NAME_LENG);
		Genode::memset(m_time,0,MAX_NAME_LENG);
		m_OpCode = 0;
		m_ClientBlpLevel = 0;
		m_ClientBibaLevel = 0;
		m_ServerBlpLevel = 0;
		m_ServerBibaLevel = 0;
		m_OpResult = false;
		m_IfRead = false;
	}
	AuditInfo(const char* username,const char* clientname,const char* servername,const char* opname,const char* time,int opcode,int Clientblplevel,int clientbibalevel,int serverblplevel,int serverbibalevel,bool opresult)
	{
		Genode::strncpy(m_UserName,username,Genode::strlen(username)+1);
		Genode::strncpy(m_ClientName,clientname,Genode::strlen(clientname)+1);
		Genode::strncpy(m_ServerName,servername,Genode::strlen(servername)+1);
		Genode::strncpy(m_OperationName,opname,Genode::strlen(opname)+1);
		Genode::strncpy(m_time,time,Genode::strlen(time)+1);
		m_OpCode = opcode;
		m_ClientBlpLevel = Clientblplevel;
		m_ClientBibaLevel = clientbibalevel;
		m_ServerBlpLevel = serverblplevel;
		m_ServerBibaLevel = serverbibalevel;
		m_OpResult = opresult;
		m_IfRead = true;
	}
	AuditInfo& operator=(AuditInfo& a)
	{
		Genode::strncpy(m_UserName,a.m_UserName,Genode::strlen(a.m_UserName)+1);
		Genode::strncpy(m_ClientName,a.m_ClientName,Genode::strlen(a.m_ClientName)+1);
		Genode::strncpy(m_ServerName,a.m_ServerName,Genode::strlen(a.m_ServerName)+1);
		Genode::strncpy(m_OperationName,a.m_OperationName,Genode::strlen(a.m_OperationName)+1);
		Genode::strncpy(m_time,a.m_time,Genode::strlen(a.m_time)+1);
		m_OpCode = a.m_OpCode;
		m_ClientBlpLevel = a.m_ClientBlpLevel;
		m_ClientBibaLevel = a.m_ClientBibaLevel;
		m_ServerBlpLevel = a.m_ServerBlpLevel;
		m_ServerBibaLevel = a.m_ServerBibaLevel;
		m_OpResult = a.m_OpResult;
		m_IfRead = a.m_IfRead;
	}
};
#endif
