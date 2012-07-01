/*
 * NetworkProcess.cpp
 *
 *  Created on: 2011-8-18
 */

#include "NetworkProcess.h"

#include <stdlib.h>
#include <malloc.h>

#include "AppHelper.h"
#include "Log.h"

#include "CounterViewFrame.h"
static const char F1 = 8;
static const char ID0 = 0;
static const char ID1 = 0;

using namespace Utility;
CNetworkProcess::CNetworkProcess()
{
	m_dataBuf = NULL;
	m_netConnect = NULL;
	m_recvThread = NULL;
    m_shutdownEvent = NULL;
	m_showWorkerInfo = FALSE;

}

CNetworkProcess::~CNetworkProcess()
{
	if(m_dataBuf != NULL)
		free(m_dataBuf);
	delete m_netConnect;
}

void CNetworkProcess::init(const char *serverIp, int port)
{
	ASSERT(serverIp);
	m_dataBuf = (char*)malloc(INITIAL_DATA_BUF_LEN*sizeof(char));
	m_netConnect = new CNetConnect(serverIp, port);
	m_netConnect->getObject(this);
    m_netConnect->initWinSocket();
	m_greeting = CounterViewApp::getApp()->getConfigure().getGreeting();
	ADDRESS = static_cast<char>(CounterViewApp::getApp()->getConfigure().getAddress());
	m_frequency = CounterViewApp::getApp()->getConfigure().getShiftFrequency() * 1000;

	m_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_shutdownEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

void CNetworkProcess::uninit()
{
    m_netConnect->unInitWinSocket();
}

void CNetworkProcess::run()
{
    m_recvThread = ::CreateThread(NULL, 0, recvThread, this, 0, NULL);
	m_showThread = ::CreateThread(NULL, 0, showThread, this, 0, NULL);
}

void CNetworkProcess::stop()
{
    ::SetEvent(m_shutdownEvent);
	m_netConnect->exit_();
	if ( m_recvThread )
	{
		::WaitForSingleObject(m_recvThread, 3000);
		::CloseHandle(m_recvThread);
	}
    if ( m_showThread )
    {
        ::WaitForSingleObject(m_showThread, 3000);
        ::CloseHandle(m_showThread);
    }

}

void CNetworkProcess::onLine()
{
	CLog(LOG_ERR)<<1<<L"Running onLine function";
	
}

void CNetworkProcess::offLine()
{
	CLog(LOG_ERR)<<1<<L"Running offLine function";
}

void CNetworkProcess::getOnewayCardInfo(BYTE data[3])
{
	for ( int i = 0; i < 3; i ++ )
	{
		m_oneway[i] = data[i];
	}
	m_isOneway = TRUE;

	char buf[13];
	memset(buf, 0, sizeof(buf) );

	buf[0] = F1;
	buf[1] = ID0;
	buf[2] = ID1;
	buf[3] = 0x00 | ((ADDRESS & 0xE0) >> 5);
	buf[4] = ( (ADDRESS & 0x1F) << 3 ) | 0x04;
	for ( int i = 0; i < 3; i ++ )
	{
		buf[i + 5] = data[i];
	}

	if ( Ret_OK != m_netConnect->sendData(buf, 13) )
	{
		CLog(LOG_INFO)<<1<<L"cant not send oneway card info\n";
	}
}

void CNetworkProcess::getTwowayCardInfo(BYTE data[2])
{
	for ( int i = 0; i < 2; i ++ )
	{
		m_oneway[i] = data[i];
	}
	m_isOneway = FALSE;

	char buf[13];
	memset(buf, 0, sizeof(buf) );

	buf[0] = F1;
	buf[1] = ID0;
	buf[2] = ID1;
	buf[3] = 0x00 | ((ADDRESS & 0xE0) >> 5);
	buf[4] = ( (ADDRESS & 0x1F) << 3 ) | 0x04;
	for ( int i = 0; i < 2; i ++ )
	{
		buf[i + 5] = data[i];
	}

	if ( Ret_OK != m_netConnect->sendData(buf, 13) )
	{
		CLog(LOG_INFO)<<1<<L"cant not send oneway card info\n";
	}
}

DWORD CNetworkProcess::recvThread(void *arg)
{
	CNetworkProcess* pThis = (CNetworkProcess*)arg;
	pThis->receive();
	return 1;
}

DWORD CNetworkProcess::showThread(void * arg)
{
	CNetworkProcess* pThis = (CNetworkProcess*)arg;
	pThis->show();
	return 1;
}

void CNetworkProcess::receive()
{
	static const int kFrameLen = 13;
	int cursor = 0;
	m_frame.reserve(50);
	while(1)
	{
		int len = 0;
	    AppRet ret = m_netConnect->recvData(m_dataBuf + cursor, RECV_BUF_LEN - cursor, len);
		if ( Ret_OK == ret )
        {
			for( int i = 0; i < len/kFrameLen; i ++ )
			{
				if ( m_dataBuf[i*kFrameLen + 3] & 0x04 )
				{
					m_netConnect->sendData(m_dataBuf + i*kFrameLen, kFrameLen);
					CLog(LOG_INFO)<<1<<L"test frame\n";
					continue;
				}
				if ( m_dataBuf[i * kFrameLen] & 0xF0 )
				{

					CLog(LOG_INFO)<<1<<L"recv data that is not data frame or normal frame\n";
					continue;
				}
				
				
				if ( m_dataBuf[i * kFrameLen + 4] & 0x04 )
				{
					CLog(LOG_INFO)<<1<<L"recv data that is no ack frame\n";
					continue; //不是上位机发来的，
				}
				CanFrame frame;
				frame.address = m_dataBuf[i * kFrameLen + 3] & 0x7 << 5;
				frame.address |= ( (m_dataBuf[i * kFrameLen + 4] & 0xF8) >> 3 );
				memcpy(frame.data, (m_dataBuf + (i * kFrameLen ) + 5) , 8);
				if ( frame.address != ADDRESS  && frame.address != 0)
				{
					CLog(LOG_INFO)<<1<<L"scrap frame address is :"<<(int)frame.address;
					continue;
				}
				if ( (m_dataBuf[i *kFrameLen + 4] & 0x03) == 0)
				{

					if ( !m_frame.empty() )
					{
						CLog(LOG_ERR)<<1<<L"frame err\n";
					}
					m_frame.clear();
					m_frame.push_back(frame);
					decoder();
					m_frame.clear();

				}
				else 
				{
					if ( (m_dataBuf[i *kFrameLen + 4] & 0x03) == 2)
					{
						m_frame.clear();
					}
					m_frame.push_back(frame);
				}

				if ( (m_dataBuf[i *kFrameLen + 4] & 0x03 )== 1)
				{
					decoder();
					m_frame.clear();
				}

			}
			cursor = len % kFrameLen;
        }	
		else if ( Ret_Quit == ret )
		{
			CLog(LOG_INFO)<<1<<L"recv thread quit\n";
			break;
		}
	}
}

void CNetworkProcess::show()
{
	CAppView &viewer = CounterViewApp::getApp()->getView();
	const int  total = viewer.CheckCardStationLineCnt();
	std::wstring greetings;
    HANDLE hs[2] = {m_event, m_shutdownEvent};
	while(1)
	{
		{
			CSimpleAutoLock autoLock1(&m_greetingLock);
			greetings = m_greeting; 
		}
		{
			CSimpleAutoLock autoLock(&m_viewLock);
			if ( m_showWorkerInfo )
			{
				viewer.showCheckCardInfo(m_workerInfo, greetings);
				m_showWorkerInfo = FALSE;
			}
			else
			{
				CSimpleAutoLock autoLock(&m_stationLock);
				CheckCardStationArr ccs;
				ccs.reserve(total);
				std::wstring tmp = m_curStation;
				CheckCardStationMap::iterator itor = m_cardStations.find(m_curStation);
				if ( ( itor == m_cardStations.end() ) && ( !m_cardStations.empty() ) )
				{
					itor = m_cardStations.begin();
					tmp = itor->first;

				}
				if ( itor != m_cardStations.end() )
				{
					for ( int i = 0; i < total * 2; i ++)
					{
						if (itor != m_cardStations.end() )
						{
							ccs.push_back(itor->second);
							m_curStation = itor->first;
							++itor;
						}
						else
						{
							itor = m_cardStations.begin();
						}
						if ( itor != m_cardStations.end() )
						{
							if ( itor->first == tmp )
							{
								break;
							}
						}
						if ( ccs.size() >= total )
						{
							break;
						}
						
					}
					if ( m_cardStations.size() <= total && (!m_cardStations.empty()) )
					{
						m_curStation = m_cardStations.begin()->first;
					}
				}

				if ( !ccs.empty() )
				{
					viewer.showAllTerminal(m_totalCnt, m_time, ccs, greetings);
				}
				
				
			}
			
		}

		DWORD ret = ::WaitForMultipleObjects(2, hs, FALSE, m_frequency);
        if ( ret == WAIT_OBJECT_0 + 1 )
        {
            break;
        }
	}
}

void CNetworkProcess::decoder()
{
	if ( (m_frame.size() == 8) && m_frame[0].data[0] )
	{
		cardInfo();
	}
	else if ( m_frame.size() >= 2 && m_frame[0].data[0] == 0 && m_frame[0].data[1] == 0)
	{
		cardStation();
	}
	else if ( m_frame.size() == 1 && m_frame[0].data[0] >= 0 && m_frame[0].data[0] <= 0x60 )
	{
		synSystemTime();
	}
	else if ( ((m_frame[0].address == ADDRESS) ||( m_frame[0].address == 0) ) &&
		       ( static_cast<BYTE>(m_frame[0].data[0]) == 0xFF ) &&( static_cast<BYTE>(m_frame[0].data[1]) == 0xFF) )
	{
		greeting();
	}
}

void CNetworkProcess::cardInfo()
{
	if ( ADDRESS != m_frame[0].address )
	{
		CLog(LOG_INFO)<<1<<L"recv card info that no blong to me\n";
		return;
	}
	
	if ( m_isOneway )
	{
		m_workerInfo.cardId = bcd2Str(m_frame[1].data, 2);
		m_workerInfo.power = FormatStrW(L"%0.1fV", 2.1 + (m_oneway[2] & 0x0F ) * 0.1 );

	}
	else
	{
		wchar_t buf[32];
		int id = m_frame[1].data[0] & 0x3F;
		id <<= 8;
		id += (unsigned char )m_frame[1].data[1];
		m_workerInfo.cardId = _itow(id, buf, 10);
		for ( int i = 0; i < 5 - m_workerInfo.cardId.size(); i ++ )
		{
			m_workerInfo.cardId = L"0" + m_workerInfo.cardId;
		}

		char tmp = (m_oneway[0] & 0xC0) >> 6;
		if ( 0 == tmp )
		{
			m_workerInfo.power = L"电量高";
		}
		else if ( 1 == tmp )
		{
			m_workerInfo.power = L"无电量";
		}
		else if ( 2 == tmp )
		{
			m_workerInfo.power = L"电量低";
		}
		else if ( 3 == tmp )
		{
			m_workerInfo.power = L"电量中";
		}
	}
	m_workerInfo.checkTime = FormatStrW(L"%s 月 %s 日 %s 时 %s 分 %s 秒", bcd2Str2(m_frame[7].data + 4,1).c_str(),
		  bcd2Str2(m_frame[7].data + 3,1).c_str(), bcd2Str2(m_frame[7].data + 2,1).c_str(), 
		  bcd2Str2(m_frame[7].data + 1,1).c_str(),  bcd2Str2(m_frame[7].data,1).c_str());
	m_workerInfo.id = bcd2Str(m_frame[2].data, 8) + bcd2Str(m_frame[3].data, 1);
	int tmp[2] = {0};
	for ( int j = 4 ; j <= 5; j++ )
	{
		for ( int i = 7; i >= 0; i -- )
		{
			if ( m_frame[j].data[i] == 0)
			{
				tmp[j-4] ++;
			}
			else
			{
				break;
			}
		}
	}
	

	m_workerInfo.job = GB2312ToUnicode(m_frame[4].data, 8 - tmp[0]) + GB2312ToUnicode(m_frame[5].data, 8 - tmp[1]);

	int len = 0;
	for ( int i = 7; i >= 0; i -- )
	{
		if ( m_frame[0].data[i] == 0)
		{
			len ++;
		}
		else
		{
			break;
		}
	}
	m_workerInfo.name = GB2312ToUnicode(m_frame[0].data, 8 - len );
	CSimpleAutoLock autolock(&m_viewLock);
	m_showWorkerInfo = TRUE;
	::SetEvent(m_event);

}

void CNetworkProcess::greeting()
{
	
	std::wstring greetings;
	int len = 0;
	for ( int i = 7; i >= 2; i -- )
	{
		if ( m_frame[0].data[i] == 0)
		{
			len ++;
		}
		else
		{
			break;
		}
	}
	greetings = GB2312ToUnicode(m_frame[0].data + 2, 6 - len );
	for ( int j = 1; j < m_frame.size(); j ++ )
	{
		int len = 0;
		for ( int i = 7; i >= 0; i -- )
		{
			if ( m_frame[j].data[i] == 0)
			{
				len ++;
			}
			else
			{
				break;
			}
		}
		greetings += GB2312ToUnicode(m_frame[j].data, 8 - len );
	}
	CSimpleAutoLock autoLock(&m_greetingLock);
	m_greeting = greetings;
}

void CNetworkProcess::synSystemTime()
{
	SYSTEMTIME time;
	time.wSecond = (m_frame[0].data[0] & 0x0f)  + ((m_frame[0].data[0]>>4) & 0x0f) * 10;
	time.wMinute = (m_frame[0].data[1] & 0x0f)  + ((m_frame[0].data[1]>>4) & 0x0f) * 10;;
	time.wHour = (m_frame[0].data[2] & 0x0f)  + ((m_frame[0].data[2]>>4) & 0x0f) * 10;;
	time.wDay = (m_frame[0].data[3] & 0x0f)  + ((m_frame[0].data[3]>>4) & 0x0f) * 10;;
	time.wMonth = (m_frame[0].data[4] & 0x0f)  + ((m_frame[0].data[4]>>4) & 0x0f) * 10;;
	time.wYear = 2000 +(m_frame[0].data[5] & 0x0f)  + ((m_frame[0].data[5]>>4) & 0x0f) * 10;;
	::SetLocalTime(&time);
	CLog(LOG_INFO)<<1<<L"set local time:"<<time.wYear<<L"year "<<time.wMonth<<L"month "<<
		time.wDay<<L"day "<<time.wHour<<L"hour "<<time.wMinute<<L"minute "<<time.wSecond;
}

void CNetworkProcess::cardStation()
{
	{

		CSimpleAutoLock autoLock(&m_stationLock);
		m_totalCnt = bcd2Str2(m_frame[0].data + 2, 3);
		m_time = FormatStrW(L"%s月%s日%s时%s分%s秒", bcd2Str2(m_frame[1].data + 4,1).c_str(),
			bcd2Str2(m_frame[1].data + 3,1).c_str(), bcd2Str2(m_frame[1].data + 2,1).c_str(), 
			bcd2Str2(m_frame[1].data + 1,1).c_str(),  bcd2Str2(m_frame[1].data,1).c_str());
	}

	for ( int i = 2; i < m_frame.size(); i += 8)
	{
		CheckCardStation ccs;
		ccs.number = bcd2Str2(m_frame[i].data, 2);
		if ( m_frame.size() - i >= 8 )
		{
			int len = 0;
			for (int k = 0; k < 3; k ++)
			{
				for ( int j = 7; j >= 0; j -- )
				{
					if ( m_frame[i+1 + k].data[j] == 0)
					{
						len ++;
					}
					else
					{
						break;
					}
				}
				ccs.position += GB2312ToUnicode(m_frame[i+1 +k].data, 8 - len );
			}
			
			for ( int k = 0; k < 3; k ++)
			{
				for ( int j = 7; j >= 0; j -- )
				{
					if ( m_frame[i+4 + k].data[j] == 0)
					{
						len ++;
					}
					else
					{
						break;
					}
				}
				ccs.region += GB2312ToUnicode(m_frame[i+4+k].data, 8 - len );
			}
			
			ccs.cnt = bcd2Str2(m_frame[i +7].data, 3);
			{
				CSimpleAutoLock autoLock(&m_stationLock);
				
				m_cardStations[ccs.number] = ccs;

			}
		}
		else
		{
			CLog(LOG_ERR)<<1<<L"card station frame is not right\n";
		}
	}
}