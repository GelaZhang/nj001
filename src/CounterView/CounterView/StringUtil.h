#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__
/**
*	\file StringUtil.h
*	\brief  提供字符串的实用操作 
*/

#include <sstream>
#include <string>
#include <vector>

namespace Utility
{
#ifdef _UNICODE
	typedef std::wstring TString;
#else
	typedef std::string TString;
#endif//_UNICODE

	typedef std::vector<std::wstring> ArrWString;
	typedef std::vector<std::string> ArrString;
	typedef std::vector<TString> ArrTString;

	/**
	*	\brief unicode版本
	*			返回删除字符串中所有空格的字符串
	*/
	std::wstring removeWhitespaceW(const std::wstring &str);
	/**
	*	\brief unicode版本
	*			判断字符是否为数字
	*/
	bool isDigitW(wchar_t c);

	/**
	*	\brief unicode版本
	*			判断整个字符串是否均为数字，第一个字母为'-'而不只一个字母也认为是数字
	*/
	bool isDigitW(const std::wstring &str);
	/**
	*	\brief unicode版本
	*			判断字符是否为字母
	*/
	bool isAlphaW(wchar_t c);
	/**
	*	\brief ansi版本
	*			返回大写的字符串,不是ASCII的字符忽略不进行改变
	*/
	std::string toUpperA(const std::string &str);

	/**
	*	\brief ansi版本
	*			返回大写的字符串,不是ASCII的字符忽略不进行改变
	*/
	std::wstring toUpperW(const std::wstring &str);

	/**
	*	\brief ansi版本,暂不提供unicode版本
	*			返回小写的字符串,不是ASCII的字符忽略不进行改变
	*/
	std::string toLowerA(const std::string &str);
	/**
	*	\brief unicode版本
	*			将给定字符串的右边的空格、制表符、换行符去掉
	*			如：给定字符串：" \r\tmystring\n "
	*			则会返回：" \r\tmystring"
	*/
	std::wstring trimRightW(const std::wstring &str);

	/**
	*	\brief unicode版本
	*			将给定字符串的左边的空格、制表符、换行符去掉
	*			如：给定字符串：" \r\tmystring\n "
	*			则会返回："mystring\n "
	*/
	std::wstring trimLeftW(const std::wstring &str);
	/**
	*	\brief unicode版本
	*			将给定字符串的前尾空格、制表符、换行符去掉
	*			如：给定字符串：" \r\tmystring\n"
	*			则会返回："mystring"
	*/
	std::wstring trimW(const std::wstring &str);
	/**
	*	\brief unicode版本
	*			将多个字符串按指定分隔符进行合并，并将结果返回。
	*			如：字符串数组：{"ab","123","#$%"}分隔符为"--" 那么结果为如下字符串
	*				"ab--123--#$%"
	*/
	std::wstring joinStringW(const ArrWString &arrStr, const std::wstring &strDelimiter);
	
	/**
	*	\brief unicode版本
	*			将字符串按指定分隔符进行切分，并将结果返回。
	*			如：字符,"ab,123,#$%" 分隔符为",$" 那么结果为如下四个字符串
	*				"ab"  "123" "#" "%"
	*/
	bool splitStringW(const std::wstring& strSource, const std::wstring& strDelims, ArrWString& arrResult);

	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	bool splitStringA(const std::string& strSource, const std::string& strDelims, ArrString& arrResult);
	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	std::string joinStringA(const ArrString &arrStr, const std::string &strDelimiter);
	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	std::string trimA(const std::string &str);
	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	std::string trimLeftA(const std::string &str);
	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	std::string trimRightA(const std::string &str);
	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	bool isAlphaA(char c);
	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	bool isDigitA(char c);
	/**
	*	\brief Ansi版本
	*			判断整个字符串是否均为数字
	*/
	bool isDigitA(const std::string &str);

	/**
	*	\brief Ansi版本 说明请见unicode版本的函数
	*/
	std::string removeWhitespaceA(const std::string &str);

#ifdef _UNICODE
#define splitString splitStringW
#define joinString joinStringW
#define trim trimW
#define trimLeft trimLeftW
#define trimRight trimRightW
#define isDigit isDigitW
#define isAlpha	isAlphaW
#define removeWhitespace removeWhitespaceW
#else
#define splitString splitStringA
#define joinString joinStringA
#define trim trimA
#define trimLeft trimLeftA
#define trimRight trimRightA
#define isDigit	isDigitA
#define isAlpha isAlphaA
#define removeWhitespace removeWhitespaceA
#endif //_UNICODE
}

#endif//!__STRING_UTIL_H__