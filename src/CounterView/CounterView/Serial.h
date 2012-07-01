#ifndef _SERIAL_H_
#define _SERIAL_H_

/**
*	\file Serial.h
*	\brief �����࣬ͬ����ʽʵ�֡�
*/
class CSerial
{
public:
	enum CONTROL
	{
		DTR = 0,  //�������
		RTS,	  //Ӳ������	
		NO_C	  //������
	};
	/**
	*	\brief �򿪴��ڡ�
	*   \param [in] iCom ���ں�1-99.
	*	\param [in] dwBaudRate �����ʡ�
	*	\param [in] dwByteSize ����λ
	*	\param [in] dwParity У��λ��
	*	\param [in] dwStopBits ֹͣλ.
	*	\param [in] c ���ط�ʽ��
	*   \return FALSE ʧ��
	*           TRUE �ɹ�
	*/
	BOOL Open( int iCom, DWORD dwBaudRate, DWORD dwByteSize = 8, 
		DWORD dwParity = NOPARITY, 
		DWORD dwStopBits = ONESTOPBIT,
		CONTROL c = CSerial::NO_C );

	/**
	*	\brief ͬ����ʽ������д���ݡ�
	*   \param [in] szBuffer ���ݻ�����
	*	\param [in] nLen ��������С��
	*   \return FALSE ʧ��
	*           TRUE �ɹ�
	*/
	BOOL SendData( const BYTE* szBuffer, int nLen );
	/**
	*	\brief ͬ����ʽ�Ӵ��ڶ����ݡ�
	*   \param [in] szBuffer ���ݻ�����
	*	\param [in] dwBufLen ��������С��
	*	\param [out] dwRevLen ���������ݳ���.
	*   \return FALSE ʧ��
	*           TRUE �ɹ�
	*/
	BOOL ReceiveData( BYTE* szBuffer, DWORD dwBufLen, DWORD &dwRevLen );
	/**
	*	\brief �رմ���
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