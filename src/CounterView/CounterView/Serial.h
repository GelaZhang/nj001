#ifndef _SERIAL_H_
#define _SERIAL_H_

/**
*	\file Serial.h
*	\brief 串口类，同步方式实现。
*/
class CSerial
{
public:
	enum CONTROL
	{
		DTR = 0,  //软件流控
		RTS,	  //硬件流控	
		NO_C	  //无流控
	};
	/**
	*	\brief 打开串口。
	*   \param [in] iCom 串口号1-99.
	*	\param [in] dwBaudRate 波特率。
	*	\param [in] dwByteSize 数据位
	*	\param [in] dwParity 校验位。
	*	\param [in] dwStopBits 停止位.
	*	\param [in] c 流控方式。
	*   \return FALSE 失败
	*           TRUE 成功
	*/
	BOOL Open( int iCom, DWORD dwBaudRate, DWORD dwByteSize = 8, 
		DWORD dwParity = NOPARITY, 
		DWORD dwStopBits = ONESTOPBIT,
		CONTROL c = CSerial::NO_C );

	/**
	*	\brief 同步方式往串口写数据。
	*   \param [in] szBuffer 数据缓冲区
	*	\param [in] nLen 缓冲区大小。
	*   \return FALSE 失败
	*           TRUE 成功
	*/
	BOOL SendData( const BYTE* szBuffer, int nLen );
	/**
	*	\brief 同步方式从串口读数据。
	*   \param [in] szBuffer 数据缓冲区
	*	\param [in] dwBufLen 缓冲区大小。
	*	\param [out] dwRevLen 读到的数据长度.
	*   \return FALSE 失败
	*           TRUE 成功
	*/
	BOOL ReceiveData( BYTE* szBuffer, DWORD dwBufLen, DWORD &dwRevLen );
	/**
	*	\brief 关闭串口
	*/
	void Close();

protected:
	BOOL SetupPort();
	BOOL SetState( 
		DWORD dwBaudRate, DWORD dwByteSize = 8, 
		DWORD dwParity = NOPARITY, 
		DWORD dwStopBits = ONESTOPBIT,
		CONTROL c = CSerial::NO_C );
protected:
	HANDLE m_hCom;
};
#endif