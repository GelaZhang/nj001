#include <assert.h>
#include "stdlib.h"
#include <wchar.h>
#include <AtlConv.h>
#include <atlstr.h>
#include "AppHelper.h"
#include <sstream>

std::wstring GB2312ToUnicode(const char *str, int length)
{
	if ( NULL == str || length <= 0)
	{
		return L"";
	}

	int len = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,str, length, 0, 0 );
	wchar_t *buf = new wchar_t[len + 1];
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,str, length, buf, len );
	buf[len] = '\0';
	std::wstring ret = buf;
	delete []buf;
	return ret;
}

/**
*	将bcd码转成字符串，高位数为0时不显示。
*/
std::wstring bcd2Str2(char *bcd, int len)
{
	int *tmp = new int[len *2];
	for ( int i = 0,  j = 0; i < len; i ++ )
	{
		tmp[j+1] = bcd[i] & 0x0f;
		tmp[j] = (bcd[i]>>4) & 0x0f;
		j+=2;
	}
	bool flag = true;
	std::wostringstream o;
	for ( int i =0 ; i < len * 2; i ++)
	{

		if ( flag && tmp[i] != 0)
		{
			flag = false;
		}
		if ( !flag )
		{
			o<<tmp[i];
		}
	}
	delete []tmp;
	if ( flag )
	{
		o<<0;
	}
	tmp = NULL;
	return o.str();
}

/**
*	将bcd码转成字符串。
*/
std::wstring bcd2Str(char *bcd, int len)
{
	int *tmp = new int[len *2];
	for ( int i = 0,  j = 0; i < len; i ++ )
	{
		tmp[j+1] = bcd[i] & 0x0f;
		tmp[j] = (bcd[i]>>4) & 0x0f;
		j+=2;
	}
	std::wostringstream o;
	for ( int i =0 ; i < len * 2; i ++)
	{
		
		o<<tmp[i];
	}
	delete []tmp;
	tmp = NULL;
	return o.str();
}
/** 
*   \brief 遍历文件夹下的文件加入向量容器
*   \param [in] pszDir 要遍历的目录
*	\param [in] pszFileType 要加入的文件型如：*.avi
*	\param [out] vFilesPath 文件夹下的文件路径容器
*   \return TRUE  成功遍历
*			FLASE 遍历失败
*/
BOOL GetAllFilePath(const TCHAR* pszDir, const TCHAR *pszFileType, 
                    std::vector<std::wstring> &vFilesPath)
{
    if (NULL == pszFileType || NULL == pszDir)
        return FALSE;

    vFilesPath.clear();

    std::wstring strFindDir = FormatStrW(L"%s\\%s", pszDir, pszFileType );

    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA FindFileData;
    DWORD dwError;

    hFind = FindFirstFile(strFindDir.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        return FALSE;
    } 
    else 
    {
        std::wstring strFilePath;
        do		
        {
            if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                strFilePath = FormatStrW(L"%s\\%s",pszDir, FindFileData.cFileName );
                vFilesPath.push_back(strFilePath);
            }

        }while (FindNextFile(hFind, &FindFileData) != 0) ;

        dwError = GetLastError();
        FindClose(hFind);
        if (dwError != ERROR_NO_MORE_FILES) 
        {
            return FALSE;
        }
    }

    return TRUE;
}


/**
*	\brief 以字符串形式返回当前系统时间。格式：2010年01月02日 12:12:12
*   \return CStringA 返回当前系统时间
*/
std::wstring GetCurrentTimeStr()
{
    

    SYSTEMTIME currentTime;
    ::GetLocalTime( &currentTime );
    return FormatStrW( L"%04d年%02d月%02d日 %02d:%02d:%02d", 
        currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour,
        currentTime.wMinute, currentTime.wSecond );
}

//unicode转换为MBCS
std::string Unicode2MBCS(LPCWSTR wstr)
{
	if (wstr==NULL)
		return "";

	BOOL bUsedDefaultChar = FALSE;
	int nLen = wcslen(wstr)*2+2;
	char * pNewStr = new char[nLen];
	memset(pNewStr,0,nLen);
	WideCharToMultiByte(CP_ACP,0,wstr,-1,pNewStr,
		nLen,NULL,&bUsedDefaultChar);

	std::string strRet = pNewStr;
	delete [] pNewStr;
	return strRet;
}

//MBCS转换为UNICODE
std::wstring MBCS2Unicode(LPCSTR str)
{
	if (str==NULL)
		return L"";

	int nLen = strlen(str)*2+2;
	WCHAR * pNewStr = new WCHAR[nLen];
	memset(pNewStr,0,nLen);
	MultiByteToWideChar(CP_ACP, 0, str, -1, pNewStr, nLen);
	std::wstring strRet = pNewStr;
	delete [] pNewStr;
	return strRet;
}

//获取程序路径
std::string GetAppPathA()
{
#ifndef _WIN32_WCE
	char sDrive[_MAX_DRIVE];
	char sDir[_MAX_DIR];
	char sFilename[_MAX_FNAME],Filename[_MAX_FNAME];
	char sExt[_MAX_EXT];
	
	GetModuleFileNameA(NULL, Filename, _MAX_PATH);
	_splitpath(Filename, sDrive, sDir, sFilename, sExt);

	std::string strRet = sDrive;
	sDir[strlen(sDir)-1] = '\0';
	strRet+=sDir;
    return strRet;
#else
    
    return "";
#endif
	
}

std::wstring GetAppPathW()
{
#ifndef _WIN32_WCE
	WCHAR sDrive[_MAX_DRIVE];
	WCHAR sDir[_MAX_DIR];
	WCHAR sFilename[_MAX_FNAME],Filename[_MAX_FNAME];
	WCHAR sExt[_MAX_EXT];
	
	GetModuleFileNameW(NULL, Filename, _MAX_PATH);
	_wsplitpath(Filename, sDrive, sDir, sFilename, sExt);
	
	std::wstring strRet = sDrive;
	sDir[wcslen(sDir)-1] = '\0';
	strRet+=sDir;
	
	return strRet;
#else
    std::wstring ret;
    CString str;
    GetModuleFileName(NULL,str.GetBuffer(MAX_PATH),MAX_PATH);
    str.ReleaseBuffer();
    ret = str.Left(str.ReverseFind('\\')+1);
    return ret;
#endif
}





std::string FormatStrA(LPCSTR lpszFormat, ...)
{
	std::string str;

	va_list args;
	va_start(args, lpszFormat);
	
	int nBuf;
	char szBuffer[1024]="";
	
	nBuf = _vsnprintf(szBuffer, _countof(szBuffer), lpszFormat, args);
	
	str = szBuffer;
	
	va_end(args);

	return str;
}

std::wstring FormatStrW(LPCWSTR lpszFormat, ...)
{
	std::wstring str;
	
	va_list args;
	va_start(args, lpszFormat);
	
	int nBuf;
	WCHAR szBuffer[1024]=L"";
	
	nBuf = _vsnwprintf(szBuffer, _countof(szBuffer), lpszFormat, args);
	
	str = szBuffer;
	
	va_end(args);
	
	return str;
}
std::string FormatHexStringA(void * data,UINT nLen)
{
	char tmp[10];
	std::string str;
	for (UINT i=0;i<nLen;i++)
	{
		sprintf(tmp,"%02X",((unsigned char *)data)[i]);
		str+=tmp;
	}

	return str;
}

std::wstring FormatHexStringW(void * data,UINT nLen)
{
	WCHAR tmp[10];
	std::wstring str;
	for (UINT i=0;i<nLen;i++)
	{
		swprintf(tmp,L"%02X",((unsigned char *)data)[i]);
		str+=tmp;
	}

	return str;
}

void HexStringToArrayA(const char * hexString,void * data)
{
	int nLen = strlen(hexString);
	char tmp[4];
	BYTE * pData = (BYTE *)data;
	int j = 0;
	for (int i=0;i<nLen;i+=2)
	{
		tmp[0] = hexString[i];
		tmp[1] = hexString[i+1];
		tmp[2] = '\0';
		UINT t;
		sscanf(tmp,"%02X",&t);
		pData[j] = (BYTE)t;
		j++;
	}
}

void HexStringToArrayW(const WCHAR * hexString,void * data)
{
	int nLen = wcslen(hexString);
	WCHAR tmp[4];
	BYTE * pData = (BYTE *)data;
	int j = 0;
	for (int i=0;i<nLen;i+=2)
	{
		tmp[0] = hexString[i];
		tmp[1] = hexString[i+1];
		tmp[2] = L'\0';
		UINT t;
		wscanf(tmp,"L%02X",&t);
		pData[j] = (BYTE)t;
		j++;
	}
}