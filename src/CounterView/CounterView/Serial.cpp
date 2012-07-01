#include "StdAfx.h"

#include <stdio.h>
#include <windows.h>
#include <wchar.h>

#include "Serial.h"

BOOL CSerial::Open( int iCom, DWORD dwBaudRate, DWORD dwByteSize /*= 8*/, 
				   DWORD dwParity /*= NOPARITY*/, DWORD dwStopBits /*= ONESTOPBIT*/,
				   CONTROL c /*= CSerial::NO_C*/  )
{
	if ( iCom < 0 || iCom > 99 )
		return FALSE;

	TCHAR szComStr[6] = _T("");

	_stprintf( szComStr, _T("COM%d:"),iCom );

	//先打开串口
	m_hCom = ::CreateFile(
		szComStr,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,	
		NULL
		);

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		ASSERT(_T("串口打开失败"));
		return FALSE;
	}

	if (SetupPort()==FALSE)
		return FALSE;

	if ( SetState(dwBaudRate, dwByteSize, dwParity, dwStopBits, c ) == FALSE )
		return FALSE;

	return TRUE;
}

BOOL CSerial::SetupPort()
{
	if ( NULL == m_hCom || INVALID_HANDLE_VALUE == m_hCom )
		return FALSE;

	//copy code
	//设置推荐缓冲区
	if(!::SetupComm(m_hCom, 4096, 4096))
		return FALSE;	

	//设置超时时间
	COMMTIMEOUTS _CO;
	if(!::GetCommTimeouts(m_hCom, &_CO))
		return FALSE;
	_CO.ReadIntervalTimeout = 0xFFFFFFFF;
	_CO.ReadTotalTimeoutMultiplier = 0;
	_CO.ReadTotalTimeoutConstant = 0;
	_CO.WriteTotalTimeoutMultiplier = 0;
	_CO.WriteTotalTimeoutConstant = 2000;
	if(!::SetCommTimeouts(m_hCom, &_CO))
		return FALSE;	

	//清空串口缓冲区
	if(!::PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
		return FALSE;	

	return TRUE;
}

BOOL CSerial::SetState( DWORD dwBaudRate, DWORD dwByteSize /*= 8*/, 
					   DWORD dwParity /*= NOPARITY*/, DWORD dwStopBits /*= ONESTOPBIT*/,
					   CONTROL c /*= CSerial::NO_C*/ )
{
	if( m_hCom != NULL && m_hCom != INVALID_HANDLE_VALUE )
	{
		DCB _DCB;
		if(!::GetCommState(m_hCom, &_DCB))
			return FALSE;
		_DCB.BaudRate = dwBaudRate;
		_DCB.ByteSize = static_cast<BYTE>(dwByteSize);
		_DCB.Parity   = static_cast<BYTE>(dwParity);
		_DCB.StopBits = static_cast<BYTE>(dwStopBits);
		if ( CSerial::RTS == c )  //硬件流控
		{
			_DCB.fOutX = FALSE;
			_DCB.fInX = FALSE;
			_DCB.fOutxCtsFlow = TRUE;
			_DCB.fRtsControl = RTS_CONTROL_ENABLE;
			_DCB.fRtsControl = RTS_CONTROL_HANDSHAKE;
		}
		else if ( CSerial::DTR == c )  //软件流控
		{
			_DCB.fOutxCtsFlow = FALSE;
			_DCB.fDtrControl = DTR_CONTROL_DISABLE;
			_DCB.fRtsControl = RTS_CONTROL_DISABLE;
			_DCB.fOutxDsrFlow = FALSE;

			_DCB.fOutX = TRUE;
			_DCB.fInX = TRUE;
			_DCB.XonLim = 0;
			_DCB.XoffLim = 0;
			_DCB.XonChar = 0x11;
			_DCB.XoffChar = 0x13;

		}
		else							//我流控
		{
			_DCB.fOutX = FALSE;
			_DCB.fInX = FALSE;
			_DCB.fOutxCtsFlow = FALSE;
			_DCB.fDtrControl = DTR_CONTROL_DISABLE;
			_DCB.fRtsControl = RTS_CONTROL_DISABLE;
			_DCB.fOutxDsrFlow = FALSE;
		}
		return ::SetCommState(m_hCom, &_DCB) == TRUE;
	}
	return FALSE;
}

BOOL CSerial::SendData( const BYTE* szBuffer, int nLen )
{
	if ( NULL == szBuffer || nLen < 0 )
		return FALSE;

	DWORD dwError;
	if(::ClearCommError(m_hCom, &dwError, NULL) && dwError > 0)	//清除错误
		::PurgeComm(m_hCom, PURGE_TXABORT | PURGE_TXCLEAR);	

	DWORD uWritedLength = 0;

	if(::WriteFile(m_hCom, szBuffer, nLen, &uWritedLength, NULL))
	{
		return TRUE;
	}

	return FALSE;
}
BOOL CSerial::ReceiveData( BYTE* szBuffer, DWORD dwBufLen, DWORD &dwRevLen )
{
	if ( NULL == szBuffer || dwBufLen < 0 )
		return FALSE;

	COMSTAT  Stat;
	DWORD dwError;
	if(::ClearCommError(m_hCom, &dwError, &Stat) && dwError > 0)	//清除错误
	{
		::PurgeComm(m_hCom, PURGE_RXABORT | PURGE_RXCLEAR); //清除输入缓冲区
		return 0;
	}
	if(!Stat.cbInQue)// 缓冲区无数据
	{
		//TRACE(_T("reading buffer is empty\n"));
		return FALSE;
	}

	dwBufLen = dwBufLen - 1 > Stat.cbInQue ? Stat.cbInQue : dwBufLen - 1;

	if( ::ReadFile(m_hCom, szBuffer, dwBufLen, &dwRevLen,NULL) )
	{          
		return TRUE;
	}
	return FALSE;
	
}

void CSerial::Close()
{
	if (m_hCom)
		CloseHandle(m_hCom);
}