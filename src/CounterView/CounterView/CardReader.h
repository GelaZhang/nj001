#pragma once

#include "CounterViewDef.h"
#include "Serial.h"

class CNetworkProcess;

class CardReader
{
public:
	CardReader();
	AppRet start(int comPort, int baudRate, CNetworkProcess *communication);
	void stop();

protected:
	static DWORD work(void *);
	void protocolAnalysis(DWORD cnt);
protected:
	CSerial m_serial;
	BYTE m_buf[256];
	int m_bufCursor;
	int m_bufCnt;
	CNetworkProcess *m_communication;
    HANDLE m_workThread;
    HANDLE m_shutdownEvent;
};