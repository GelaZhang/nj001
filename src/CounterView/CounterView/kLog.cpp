// kLog.cpp: implementation of the CkLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4996)
CkLog::CkLog()
{
	m_hLogThread = NULL;
	m_bRun = FALSE;
//	m_CriticalSection = NULL;
	m_hSemaphore = NULL;

	m_hLogThread = NULL;
    m_fileSize = 0;

}

CkLog::~CkLog()
{
	
}

BOOL CkLog::WriteLog(CC_Log logType,SYSTEMTIME &st,LPCTSTR lpFrom,LPCTSTR lpLog)
{
//	MYTRACE(_T("%d\t%d-%d-%d %d:%d:%d\t%s\n"),logType,
//		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,lpLog);

	if (m_bRun)
	{
		LogData * pData = new LogData;
		ZeroMemory(pData,sizeof(LogData));
		pData->logType = logType;
		memcpy(&pData->st,&st,sizeof(SYSTEMTIME));
		_tcsncpy(pData->szLog,lpLog,MAX_LOG-2);
		_tcsncpy(pData->szFrom,lpFrom,MAX_LOGFROM-2);

		::EnterCriticalSection(&m_CriticalSection);
        
        m_logList.push_back(pData);
 
        ::LeaveCriticalSection(&m_CriticalSection);
		
		::ReleaseSemaphore(m_hSemaphore,1,NULL);        

		return TRUE;
	}

	return FALSE;
}

BOOL CkLog::WriteLog(CC_Log logType,LPCTSTR lpFrom,LPCTSTR lpLog)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	return WriteLog(logType,st,lpFrom,lpLog);
}

BOOL CkLog::WriteLog(LPCTSTR lpFrom,LPCTSTR lpLog)
{
	return WriteLog(LOG_INFO,lpFrom,lpLog);
}

DWORD CkLog::LogThread(void * args)
{
	CkLog * pLog = (CkLog *)args;

	if (pLog == NULL)
		return 0;

	//先打开文件
	HANDLE hFile = NULL;

	hFile = CreateFile(pLog->m_path.c_str(),GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile==INVALID_HANDLE_VALUE)
	{
		pLog->m_bRun = FALSE;
		return 1;
	}

	if (GetLastError()!=ERROR_ALREADY_EXISTS)
	{//说明文件已经存在
		SetFilePointer(hFile,0,FILE_BEGIN,NULL);
#ifdef _UNICODE
		//写入UNICODE标签
		BYTE szSig[2] = {0xFF,0XFE};
		DWORD dw1;
		WriteFile(hFile,szSig,2,&dw1,NULL);
#endif
	}

	TCHAR szData[MAX_LOG+100] = _T("");

	LogList _logList;

	while (pLog->m_bRun)
	{
		DWORD dwRet = ::WaitForSingleObject(pLog->m_hSemaphore,INFINITE);
		
		if(dwRet == WAIT_FAILED )
			break;
		
		EnterCriticalSection(&pLog->m_CriticalSection);
		//copy(m_logList.begin(),m_logList.end(),_logList.begin());这句话不能拷贝？。。。
		LogList::iterator itor1;
		for (itor1=pLog->m_logList.begin();itor1!=pLog->m_logList.end();itor1++)
			_logList.push_back(*itor1);	
		pLog->m_logList.clear();
		LeaveCriticalSection(&pLog->m_CriticalSection);	

		LogList::iterator itor = _logList.begin();
		for (;itor!=_logList.end();itor++)
		{
			LogData * pData = *itor;
			szData[0] = _T('0');
			_stprintf(szData,_T("%d\t%d-%02d-%02d %02d:%02d:%02d.%03d\t[%s]:%s\r\n"),pData->logType,
				pData->st.wYear,pData->st.wMonth,pData->st.wDay,
				pData->st.wHour,pData->st.wMinute,pData->st.wSecond,pData->st.wMilliseconds,
				pData->szFrom,pData->szLog);

			int nSize = 0;
#ifdef _UNICODE
			nSize = _tcslen(szData)*2;
#else
			nSize = _tcslen(szData);
#endif
            DWORD fileSize = GetFileSize(hFile, NULL);
            if ( fileSize >= pLog->m_fileSize )
            {
                DWORD cur = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
                if ( cur == fileSize )
                {
                    SetFilePointer(hFile,0,NULL,FILE_BEGIN);
                }
            }
            else
            {
			    SetFilePointer(hFile,0,NULL,FILE_END);
            }

			ULONG nWrited = nSize;
			WriteFile(hFile,szData,nSize,&nWrited,NULL);

			delete pData;
		}

		_logList.clear();
		
	}

	Sleep(0);

 	CloseHandle(hFile);

	pLog->m_bRun = FALSE;

	return 0;
}


BOOL CkLog::InitLog(const wchar_t * path, int fileSize)
{
    m_fileSize = fileSize;
	m_hSemaphore = ::CreateSemaphore(NULL,  0 ,  0xFFFF , NULL);
	
	//  If the function fails, the return value is NULL. 
	if (m_hSemaphore == NULL || NULL == path )
		return  FALSE ;
	
	//  the function returns a handle to the existing object and GetLastError returns ERROR_ALREADY_EXISTS. 
	if  (::GetLastError() == ERROR_ALREADY_EXISTS)
		return FALSE ;
	
    m_path = path;
	::InitializeCriticalSection( & m_CriticalSection);
	
	if  ( m_hLogThread  !=  NULL )
		return FALSE ;
	
	DWORD nThreadId  = 0 ;
	m_bRun = TRUE;
	m_hLogThread = (HANDLE)CreateThread(NULL,0,LogThread,this,0 ,&nThreadId);
	
	Sleep(0);

	if (m_hLogThread  ==  NULL)
		return FALSE ;
	
	return TRUE;
}

BOOL CkLog::UninitLog()
{
	m_bRun = FALSE;
	::ReleaseSemaphore(m_hSemaphore,1,NULL);
	
	DWORD dwRet = ::WaitForSingleObject(m_hLogThread, INFINITE);
	
	if  (dwRet == WAIT_FAILED)
		return FALSE;
	
	::CloseHandle(m_hSemaphore);
	m_hSemaphore  =  NULL;

	::CloseHandle(m_hLogThread);
	m_hLogThread = NULL;
	
	::DeleteCriticalSection( & m_CriticalSection);

	//清理剩余的LogData
/*	HANDLE hFile = NULL;
	TCHAR szPath[MAX_PATH*2] = _T("");
	GetAppPath(szPath);
	_tcscat(szPath,_T("\\_CanCapture.log"));
	hFile = CreateFile(szPath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if (hFile==INVALID_HANDLE_VALUE)
	{
		m_bRun = FALSE;
		return 1;
	}
	
	if (GetLastError()!=ERROR_ALREADY_EXISTS)
	{//说明文件已经存在
		SetFilePointer(hFile,0,FILE_BEGIN,NULL);
#ifdef _UNICODE
		//写入UNICODE标签
		BYTE szSig[2] = {0xFF,0XFE};
		DWORD dw1;
		WriteFile(hFile,szSig,2,&dw1,NULL);
#endif
	}
	
	TCHAR szData[MAX_LOG+100] = _T("");
	
	LogList::iterator itor = m_logList.begin();
	for (;itor!=m_logList.end();itor++)
	{
		LogData * pData = *itor;
		szData[0] = _T('0');
		_stprintf(szData,_T("%d\t%d-%02d-%02d %02d:%02d:%02d.%03d\t[%s]:%s\r\n"),pData->logType,
			pData->st.wYear,pData->st.wMonth,pData->st.wDay,
			pData->st.wHour,pData->st.wMinute,pData->st.wSecond,pData->st.wMilliseconds,
			pData->szFrom,pData->szLog);
		
		int nSize = 0;
#ifdef _UNICODE
		nSize = _tcslen(szData)*2;
#else
		nSize = _tcslen(szData);
#endif
		
		SetFilePointer(hFile,0,NULL,FILE_END);
		
		ULONG nWrited = nSize;
			WriteFile(hFile,szData,nSize,&nWrited,NULL);
			delete pData;
	}
	m_logList.clear();
*/	
	return TRUE;
}
