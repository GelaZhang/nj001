#pragma once

#include "utils.h"
#include "CounterViewDef.h"

class Communication
{
public:
	AppRet start();
	AppRet stop();

	void getOnewayCardInfo(BYTE data[3]);
	void getTwowayCardInfo(BYTE data[2]);

protected:
	CheckCardStationMap m_cardStations;
	int m_curStationId;
	std::wstring m_totalCnt; 
	std::wstring m_time;
	CSimpleLock m_lock;
};