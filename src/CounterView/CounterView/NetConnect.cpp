/*
 * NetConnect.cpp
 *
 *  Created on: 2011-8-9
 */
#include "NetConnect.h"

#include "Log.h"
#include "CounterViewDef.h"
#include "CounterViewFrame.h"
#include "NetworkProcess.h"

#pragma comment(lib, "Ws2.lib")
using namespace Utility;
CNetConnect::CNetConnect(const char* ip, int port)
{
	strncpy(m_ip, ip, sizeof(m_ip));
	m_sock = -1;
	m_port = port;
	m_isConnect = false;
	FD_ZERO(&m_fdSet_r);
	FD_ZERO(&m_fdSet_w);
    m_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pNetPro = NULL;
}

CNetConnect::~CNetConnect()
{
	FD_ZERO(&m_fdSet_r);
	FD_ZERO(&m_fdSet_w);
	if(m_sock != -1)
	{
        ::closesocket(m_sock);
	}
    ::CloseHandle(m_event);
}

bool CNetConnect::connectServer()
{
	if ( -1 != m_sock )
	{
		closesocket(m_sock);
	}
	//创建套接字
	if((m_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))== -1)
	{
        CLog(LOG_ERR)<<1<<L"Create socket failed\n";
		return false;
	}
	int flag = 1;
	int ret = setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
	if (ret == -1)
	{
		CLog(LOG_ERR)<<1<<L"Couldn't setsockopt(TCP_NODELAY)\n";
		return false;
	}
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(sockaddr_in));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(m_ip);
	serverAddress.sin_port = htons(m_port);
	CLog(LOG_INFO)<<1<<L"connect to "<<m_ip<<L"port:"<<m_port;
	int connectRet = connect(m_sock, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if( connectRet == 0 )
	{
		m_isConnect = true;
		CLog(LOG_ERR)<<1<<L"Connect success\n";
		//告诉上层建立连接（包括重连）
		if(m_pNetPro != NULL)
		{
			m_pNetPro->onLine();
		}
#ifdef DEBUG
		 CounterViewApp::getApp()->getView().showGreetings(L"sccuess");
#endif
		return true;
		
	}
#ifdef DEBUG
	CounterViewApp::getApp()->getView().showGreetings(L"faild");
#endif
	CLog(LOG_ERR)<<1<<L"Build connect failed\n";
	return false;
}

AppRet CNetConnect::sendData(char* pSendBuf, int len)
{
	if(pSendBuf == NULL || len <= 0)
	{
		CLog(LOG_ERR)<<1<<L"Input parameters unreasonable";
		return Ret_Invalid_Param;
	}
	//判断是否连接
	if( m_isConnect )
	{
		FD_SET(m_sock, &m_fdSet_w);
		struct timeval timeout;                          //阻塞等待时间
		timeout.tv_sec = TIMEVAL_SEND;
		timeout.tv_usec = 0;
		int retValue = select(m_sock+1, NULL, &m_fdSet_w, NULL, &timeout);
		//出错
		if(retValue <= 0)
		{
			CLog(LOG_ERR)<<1<<L"Network status is busy, fail to send data";
			offLineProcess();
			return Ret_Disconnect;
		}
		if(FD_ISSET(m_sock, &m_fdSet_w))
		{
			int send_len = send(m_sock, pSendBuf, len, 0);
			if(send_len < len)
			{
				CLog(LOG_ERR)<<1<<L"send data less request";
				offLineProcess();
				return Ret_Disconnect;
			}
			else if(send_len <= 0)
			{
				CLog(LOG_ERR)<<1<<L"send failed";
				offLineProcess();
				return Ret_Disconnect;
			}
            else 
            {
                return Ret_OK;
            }
		}
	}
	return Ret_ERR;
}

AppRet CNetConnect::recvData(char* pRecvBuf, int bufLen, int &len)
{
    static const int RECONNECT_INTEVAL = 2000;
	int retValue = 0;
	len = 0;
	while(1)
	{
		if(!m_isConnect)
		{
			//执行连接，如果失败等待一个时间继续循环，直到连接成功或收到中断信号
			if(!connectServer())
			{
				CLog(LOG_INFO)<<1<<L"Waiting for new connect";
				//重连等待时间
                DWORD ret = ::WaitForSingleObject(m_event, RECONNECT_INTEVAL);
				
                if( WAIT_TIMEOUT == ret)
				{
				    continue;
				}
				else if ( WAIT_OBJECT_0 == ret )
				{
					CLog(LOG_ERR)<<1<<L"reconnect interrupt";
					return Ret_Quit;
				}
                else
                {
                    CLog(LOG_ERR)<<1<<L"wait event err\n";
                    return Ret_ERR;
                }
			}
		}
		FD_SET(m_sock, &m_fdSet_r);
		//select等待时间
		struct timeval timeout;
		timeout.tv_sec = TIMEVAL_RECEIVE;
		timeout.tv_usec = 0;
		retValue = select(m_sock+1, &m_fdSet_r, NULL, NULL, &timeout);
		//出错
		if(retValue < 0)
		{
			CLog(LOG_ERR)<<1<<L"select failed";
			offLineProcess();
			return Ret_Disconnect;
		}
		//等待超时
		if(retValue == 0)
		{
			continue;
		}
		if (FD_ISSET(m_sock, &m_fdSet_r))
		{
			len = recv(m_sock, pRecvBuf, bufLen, 0);
			if(len <= 0)
			{
				CLog(LOG_ERR)<<1<<L"receive failed or socket is close";
				offLineProcess();
				return Ret_Disconnect;
			}
			CLog(LOG_INFO)<<1<<L"receive some data\n";
			return Ret_OK;
		}
	}
    return Ret_ERR;
}

void CNetConnect::offLineProcess()
{
	CLog(LOG_INFO)<<1<<L"network is disconnect";
	CSimpleAutoLock autoLock(&m_lock);
    if ( m_isConnect )
    {
	    
	    //告诉上层已断开连接
	    if(m_pNetPro != NULL)
	    {
		    m_pNetPro->offLine();
	    }
        m_isConnect = false;
    }
    else
    {
        CLog(LOG_INFO)<<1<<L"connection had closed\n";
    }
}

void CNetConnect::getObject(CNetworkProcess* const np)
{
	m_pNetPro = np;
}

void CNetConnect::exit_()
{
    ::SetEvent(m_event);
}

bool CNetConnect::initWinSocket()
{
    WSADATA wsd;
    if ( 0 != WSAStartup(MAKEWORD(2,2), &wsd) )
    {
        return false;
    }
    if ( 2 == LOBYTE(wsd.wVersion) && 2 == HIBYTE(wsd.wVersion) )
    {
        return true;
    }
    return false;
}

void CNetConnect::unInitWinSocket()
{
    if ( 0 != WSACleanup() )
    {
        int errCode = WSAGetLastError();
    }
}
