/*
 * NetworkProcess.h
 *
 *  Created on: 2011-8-18
 */

#ifndef NETWORKPROCESS_H_
#define NETWORKPROCESS_H_
#include <list>
#include "NetConnect.h"

#define RECV_BUF_LEN 13*50
#define INITIAL_DATA_BUF_LEN 2048


class CNetConnect;

struct CanFrame
{
	char address;
	char data[8];
};

typedef std::vector<CanFrame> CanFrameArr;
/*
 * brief 终端的网络通讯，收发数据的处理的主体类
 */
class CNetworkProcess
{
public:
	CNetworkProcess();
	~CNetworkProcess();
	/*
	 * brief 初始化函数
	 */
	void init(const char *serverIp, int port);
    void uninit();
	/*
	 * brief 执行函数，连接服务器，启动数据处理线程
	 */
	void run();

	void stop();
	/*
	 * brief 提供给底层的一个接口，当网络连通时调用
	 */
	void onLine();
	/*
	 * brief 提供给底层的一个接口，当网络断开时调用
	 */
	void offLine();

	inline CNetConnect *getNetConnect()
	{
		return m_netConnect;
	}

	void getOnewayCardInfo(BYTE data[3]);
	void getTwowayCardInfo(BYTE data[2]);
private:
	/*
	 * brief 接收线程函数，静态类成员提供给pthread创建线程
	 */
	static DWORD recvThread(void *);
	static DWORD showThread(void *);
	/*
	 * brief 数据接收，接收线程的主要数据处理过程
	 */
	void receive();

	void show();

	void decoder();

	void cardInfo();
	void greeting();
	void synSystemTime();

	void cardStation();

	CNetConnect *m_netConnect;
	char *m_dataBuf;
	HANDLE m_recvThread;
	HANDLE m_showThread;

	char m_oneway[3];
	BOOL m_isOneway;
	CanFrameArr m_frame;
	std::wstring m_greeting;
	CSimpleLock m_greetingLock;
	char ADDRESS ;

	//显示不检卡信息
	CheckCardStationMap m_cardStations;

	std::wstring m_totalCnt; 
	std::wstring m_time;
	std::wstring m_curStation;
	CSimpleLock m_stationLock;

	CSimpleLock m_viewLock;//显示用的锁
	int m_frequency;

	WorkerInfo m_workerInfo;
	BOOL m_showWorkerInfo;
	HANDLE m_event;
    HANDLE m_shutdownEvent;
};

#endif /* NETWORKPROCESS_H_ */
