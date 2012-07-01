
#ifndef APPHELPER_H__
#define APPHELPER_H__

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>

#include <windows.h>

std::wstring GB2312ToUnicode(const char *str, int length);
/**
*	��bcd��ת���ַ�������λ��Ϊ0ʱ����ʾ��
*/
std::wstring bcd2Str2(char *bcd, int len);

std::wstring bcd2Str(char *bcd, int len);

/** 
*   \brief �����ļ����µ��ļ�������������
*   \param [in] pszDir Ҫ������Ŀ¼
*	\param [in] pszFileType Ҫ������ļ����磺*.avi
*	\param [out] vFilesPath �ļ����µ��ļ�·������
*   \return TRUE  �ɹ�����
*			FLASE ����ʧ��
*/
BOOL GetAllFilePath(const TCHAR* pszDir, const TCHAR *pszFileType, 
                           std::vector<std::wstring> &vFilesPath);


/**
*	\brief ���ַ�����ʽ���ص�ǰϵͳʱ�䡣��ʽ��2010/01/02/12:12:12
*   \return CStringA ���ص�ǰϵͳʱ��
*/
std::wstring GetCurrentTimeStr();

/**
	\brief Unicodeת��ΪMBCS
	\param wstr Ҫת����UNICODE�ַ��������Ա������У��
	\return ���ΪNULL,�򷵻ؿ��ַ��������򷵻�ת�����MBCS�ִ�.
  */
std::string Unicode2MBCS(LPCWSTR wstr);

/**
	\brief MBCSת��ΪUNICODE
	\param str Ҫת����MBCS�ַ��������Ա������У��
	\return ���ΪNULL,�򷵻ؿ��ַ��������򷵻�ת�����UNICODE�ִ�.
  */
std::wstring MBCS2Unicode(LPCSTR str);


/**
	\brief ��ȡ�������ڵ�·��
	\return ���س������ڵ�Ŀ¼������c:\mydir��ע�ⲻ������c:\mydir\������ǲ���б�ܵġ�
  */
std::string GetAppPathA();

/**
	\brief ��ȡ�������ڵ�·��
	\return ���س������ڵ�Ŀ¼������c:\mydir��ע�ⲻ������c:\mydir\������ǲ���б�ܵġ�
  */
std::wstring GetAppPathW();


/**
	\brief ��ʽ���ַ���
	\param ��ο�printf
	\return ���ظ�ʽ������ַ���
  */
std::string FormatStrA(LPCSTR lpszFormat, ...);

/**
	\brief ��ʽ���ַ���
	\param ��ο�printf
	\return ���ظ�ʽ������ַ���
  */
std::wstring FormatStrW(LPCWSTR lpszFormat, ...);

/**
	\brief �����ݸ�ʽ����HEX�ַ���
*/
std::string FormatHexStringA(void * data,UINT nLen);

/**
	\brief �����ݸ�ʽ����HEX�ַ���
*/
std::wstring FormatHexStringW(void * data,UINT nLen);


/**
	\brief ��HEX�ַ���ת��������
*/
void HexStringToArrayA(const char * hexString,void * data);


/**
	\brief ��HEX�ַ���ת��������
*/
void HexStringToArrayW(const WCHAR * hexString,void * data);


#ifdef _UNICODE
#define GetAppPath			GetAppPathW
#define FormatStr			FormatStrW 
#define FormatHexString		FormatHexStringW
#define HexStringToArray	HexStringToArrayW
#else
#define GetAppPath			GetAppPathA
#define FormatStr			FormatStrA
#define FormatHexString		FormatHexStringA
#define HexStringToArray	HexStringToArrayA
#endif



#endif