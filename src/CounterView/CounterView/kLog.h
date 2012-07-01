// kLog.h: interface for the CkLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KLOG_H__39A16AE4_1D2F_4223_A562_E5B4A2EF8552__INCLUDED_)
#define AFX_KLOG_H__39A16AE4_1D2F_4223_A562_E5B4A2EF8552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_LOG	512
#define MAX_LOGFROM 100

#include <list>


#include <Windows.h>
enum CC_Log
{
    LOG_INFO=1,//普通信息
    LOG_WARN,//警告
    LOG_ERR	//错误
};

struct LogData
{
	CC_Log logType;
	SYSTEMTIME st;
	TCHAR szFrom[MAX_LOGFROM];
	TCHAR szLog[MAX_LOG];
};

typedef std::list<LogData *> LogList;

class CkLog
{
public:
	CkLog();
	~CkLog();

public:
	BOOL WriteLog(CC_Log logType,SYSTEMTIME &st,LPCTSTR lpFrom,LPCTSTR lpLog);

	BOOL WriteLog(CC_Log logType,LPCTSTR lpFrom,LPCTSTR lpLog);

	BOOL WriteLog(LPCTSTR lpFrom,LPCTSTR lpLog);

	//初始化日志
	BOOL InitLog(const wchar_t * path, int fileSize);

	//关闭日志
	BOOL UninitLog();

private:
	static DWORD LogThread(void * args);
	HANDLE m_hLogThread;	
	BOOL m_bRun;
	LogList	m_logList;

	HANDLE                   m_hSemaphore;   
	CRITICAL_SECTION         m_CriticalSection; 

    std::wstring m_path;
    int m_fileSize;
};

#endif // !defined(AFX_KLOG_H__39A16AE4_1D2F_4223_A562_E5B4A2EF8552__INCLUDED_)
