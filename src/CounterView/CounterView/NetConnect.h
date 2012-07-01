/*
 * NetConnect.h
 *
 *  Created on: 2011-8-9
 */
#ifndef NETCONNECT_H_
#define NETCONNECT_H_

//#include "../NetworkProcess.h"


#include <Windows.h>
#include <WinSock2.h>

#include "utils.h"
#include "CounterViewDef.h"

#define TIMEVAL_RECEIVE 60
#define TIMEVAL_SEND 30
#define TIMEVAL_RECONNECT 10

class CNetworkProcess;

/*
 * brief 底层网络数据收发，连接维护类
 */
class CNetConnect
{
public:
	CNetConnect(const char* ip, int port);
	~CNetConnect();
    bool initWinSocket();
    void unInitWinSocket();
	/*
	 * brief 连接服务器
	 */
	bool connectServer();
	/*
	 * brief 发送数据包
	 * param[in] pSendBuf 发送缓存
	 * param[in] len 发送的数据大小
	 */
	AppRet sendData(char* pSendBuf, int len);
	/*
	 * brief 接收数据包
	 * param[out] pRecvBuf 接收缓存
	 * param[in] bufLen 接收缓存大小
	 */
	AppRet recvData(char* pRecvBuf, int bufLen, int &len);
	/*
	 * brief 获取CNetworkProcess类的引用
	 * param[in] np 接收CNetworkProcess类的引用
	 */
	void getObject(CNetworkProcess* const np);
   
    /*
    *   \brief 退出
    */
    void exit_();
private:
	/*
	 * brief 连接中断处理
	 */
	void offLineProcess();

	char m_ip[16];
	int  m_port;
	SOCKET m_sock;
	fd_set  m_fdSet_r;
	fd_set  m_fdSet_w;
	bool m_isConnect;
    HANDLE m_event;
	CNetworkProcess* m_pNetPro;
    CSimpleLock m_lock;
};

#endif /* NETCONNECT_H_ */
