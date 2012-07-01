#include "CardReader.h"

#include "NetworkProcess.h"

#define COM_PORT 1
#define BAUDRATE 9600
#define ONE_WAY_CARD_FRAME 0xfe
#define TWO_WAY_CARD_FRAME 0x24

#define ONE_WAY_CARD_FRAME_LEN 5
#define TWO_WAY_CARD_FRAME_LEN 4
CardReader::CardReader()
{
	m_bufCursor = 0;
	m_bufCnt = 0;
	memset(m_buf, 0, sizeof(m_buf) );
	m_communication = NULL;
    m_workThread = NULL;
    m_shutdownEvent = NULL;
}
AppRet CardReader::start(int comPort, int baudRate, CNetworkProcess *communication)
{
	m_communication = communication;
	DWORD cnt = 0;
	if ( !m_serial.Open(comPort, baudRate) )
	{
		return Ret_ERR;
	}
    m_shutdownEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_workThread = CreateThread(NULL, 0, CardReader::work, this, 0, NULL);
	return Ret_OK;
}

void CardReader::stop()
{
    ::SetEvent(m_shutdownEvent);
    ::WaitForSingleObject(m_workThread, 2000);
}
DWORD CardReader::work(void *arg)
{
	CardReader *instance = static_cast<CardReader*>(arg);
	if ( !instance )
	{
		return 0;
	}
	while (1)
	{
		DWORD cnt = 0;
		BOOL ret = instance->m_serial.ReceiveData(instance->m_buf + instance->m_bufCursor, 256 - instance->m_bufCursor, cnt);
		if ( ret )
		{
			instance->protocolAnalysis( cnt);
		}
        DWORD retWait = ::WaitForSingleObject(instance->m_shutdownEvent, 5);
        if ( WAIT_OBJECT_0 == retWait )
        {
            break;
        }
	}
	return 1;
}

void CardReader::protocolAnalysis(DWORD cnt)
{
	bool flag = true;
	m_bufCnt += cnt;
	for ( int i = 0; i < m_bufCnt; i ++ )
	{
		if ( m_buf[i] == ONE_WAY_CARD_FRAME )
		{
			if ( m_bufCnt - i >= ONE_WAY_CARD_FRAME_LEN )
			{
				if ( m_communication  && ( m_buf[1] + m_buf[2] == m_buf[3]) )
				{
					BYTE buf[3] = { m_buf[i+1], m_buf[i+2], m_buf[i+4]};
					m_communication->getOnewayCardInfo(buf);
				}
				i += ONE_WAY_CARD_FRAME_LEN - 1;
			}
			else
			{
				memcpy(m_buf, m_buf + i, m_bufCnt - i);
				m_bufCursor = m_bufCnt - i;
				m_bufCnt = m_bufCursor;
				flag = false;
				break;
			}
		}
		else if ( m_buf[i] = TWO_WAY_CARD_FRAME )
		{
			if ( m_bufCnt - i >= TWO_WAY_CARD_FRAME_LEN )
			{
				if ( m_communication )
				{
					BYTE buf[2] = { m_buf[i+1], m_buf[i+2]};
					m_communication->getTwowayCardInfo(buf);
				}
				i += TWO_WAY_CARD_FRAME_LEN -1;
			}
			else
			{
				memcpy(m_buf, m_buf + i, m_bufCnt - i);
				m_bufCursor = m_bufCnt - i;
				m_bufCnt = m_bufCursor;
				flag = false;
				break;
			}
		}
		
	}
	if ( flag )
	{
		m_bufCnt = 0;
		m_bufCursor = 0;
	}
	
}