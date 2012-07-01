
#ifndef APPHELPER_H__
#define APPHELPER_H__

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>

#include <windows.h>

std::wstring GB2312ToUnicode(const char *str, int length);
/**
*	将bcd码转成字符串，高位数为0时不显示。
*/
std::wstring bcd2Str2(char *bcd, int len);

std::wstring bcd2Str(char *bcd, int len);

/** 
*   \brief 遍历文件夹下的文件加入向量容器
*   \param [in] pszDir 要遍历的目录
*	\param [in] pszFileType 要加入的文件型如：*.avi
*	\param [out] vFilesPath 文件夹下的文件路径容器
*   \return TRUE  成功遍历
*			FLASE 遍历失败
*/
BOOL GetAllFilePath(const TCHAR* pszDir, const TCHAR *pszFileType, 
                           std::vector<std::wstring> &vFilesPath);


/**
*	\brief 以字符串形式返回当前系统时间。格式：2010/01/02/12:12:12
*   \return CStringA 返回当前系统时间
*/
std::wstring GetCurrentTimeStr();

/**
	\brief Unicode转换为MBCS
	\param wstr 要转换的UNICODE字符串，不对编码进行校验
	\return 如果为NULL,则返回空字符串，否则返回转换后的MBCS字串.
  */
std::string Unicode2MBCS(LPCWSTR wstr);

/**
	\brief MBCS转换为UNICODE
	\param str 要转换的MBCS字符串，不对编码进行校验
	\return 如果为NULL,则返回空字符串，否则返回转换后的UNICODE字串.
  */
std::wstring MBCS2Unicode(LPCSTR str);


/**
	\brief 获取程序所在的路径
	\return 返回程序所在的目录，例如c:\mydir，注意不是这类c:\mydir\，最后是不带斜杠的。
  */
std::string GetAppPathA();

/**
	\brief 获取程序所在的路径
	\return 返回程序所在的目录，例如c:\mydir，注意不是这类c:\mydir\，最后是不带斜杠的。
  */
std::wstring GetAppPathW();


/**
	\brief 格式化字符串
	\param 请参考printf
	\return 返回格式化后的字符串
  */
std::string FormatStrA(LPCSTR lpszFormat, ...);

/**
	\brief 格式化字符串
	\param 请参考printf
	\return 返回格式化后的字符串
  */
std::wstring FormatStrW(LPCWSTR lpszFormat, ...);

/**
	\brief 将数据格式化成HEX字符串
*/
std::string FormatHexStringA(void * data,UINT nLen);

/**
	\brief 将数据格式化成HEX字符串
*/
std::wstring FormatHexStringW(void * data,UINT nLen);


/**
	\brief 将HEX字符串转换成数组
*/
void HexStringToArrayA(const char * hexString,void * data);


/**
	\brief 将HEX字符串转换成数组
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