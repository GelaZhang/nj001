#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__
/**
*	\file StringUtil.h
*	\brief  �ṩ�ַ�����ʵ�ò��� 
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
	*	\brief unicode�汾
	*			����ɾ���ַ��������пո���ַ���
	*/
	std::wstring removeWhitespaceW(const std::wstring &str);
	/**
	*	\brief unicode�汾
	*			�ж��ַ��Ƿ�Ϊ����
	*/
	bool isDigitW(wchar_t c);

	/**
	*	\brief unicode�汾
	*			�ж������ַ����Ƿ��Ϊ���֣���һ����ĸΪ'-'����ֻһ����ĸҲ��Ϊ������
	*/
	bool isDigitW(const std::wstring &str);
	/**
	*	\brief unicode�汾
	*			�ж��ַ��Ƿ�Ϊ��ĸ
	*/
	bool isAlphaW(wchar_t c);
	/**
	*	\brief ansi�汾
	*			���ش�д���ַ���,����ASCII���ַ����Բ����иı�
	*/
	std::string toUpperA(const std::string &str);

	/**
	*	\brief ansi�汾
	*			���ش�д���ַ���,����ASCII���ַ����Բ����иı�
	*/
	std::wstring toUpperW(const std::wstring &str);

	/**
	*	\brief ansi�汾,�ݲ��ṩunicode�汾
	*			����Сд���ַ���,����ASCII���ַ����Բ����иı�
	*/
	std::string toLowerA(const std::string &str);
	/**
	*	\brief unicode�汾
	*			�������ַ������ұߵĿո��Ʊ�������з�ȥ��
	*			�磺�����ַ�����" \r\tmystring\n "
	*			��᷵�أ�" \r\tmystring"
	*/
	std::wstring trimRightW(const std::wstring &str);

	/**
	*	\brief unicode�汾
	*			�������ַ�������ߵĿո��Ʊ�������з�ȥ��
	*			�磺�����ַ�����" \r\tmystring\n "
	*			��᷵�أ�"mystring\n "
	*/
	std::wstring trimLeftW(const std::wstring &str);
	/**
	*	\brief unicode�汾
	*			�������ַ�����ǰβ�ո��Ʊ�������з�ȥ��
	*			�磺�����ַ�����" \r\tmystring\n"
	*			��᷵�أ�"mystring"
	*/
	std::wstring trimW(const std::wstring &str);
	/**
	*	\brief unicode�汾
	*			������ַ�����ָ���ָ������кϲ�������������ء�
	*			�磺�ַ������飺{"ab","123","#$%"}�ָ���Ϊ"--" ��ô���Ϊ�����ַ���
	*				"ab--123--#$%"
	*/
	std::wstring joinStringW(const ArrWString &arrStr, const std::wstring &strDelimiter);
	
	/**
	*	\brief unicode�汾
	*			���ַ�����ָ���ָ��������з֣�����������ء�
	*			�磺�ַ�,"ab,123,#$%" �ָ���Ϊ",$" ��ô���Ϊ�����ĸ��ַ���
	*				"ab"  "123" "#" "%"
	*/
	bool splitStringW(const std::wstring& strSource, const std::wstring& strDelims, ArrWString& arrResult);

	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
	*/
	bool splitStringA(const std::string& strSource, const std::string& strDelims, ArrString& arrResult);
	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
	*/
	std::string joinStringA(const ArrString &arrStr, const std::string &strDelimiter);
	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
	*/
	std::string trimA(const std::string &str);
	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
	*/
	std::string trimLeftA(const std::string &str);
	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
	*/
	std::string trimRightA(const std::string &str);
	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
	*/
	bool isAlphaA(char c);
	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
	*/
	bool isDigitA(char c);
	/**
	*	\brief Ansi�汾
	*			�ж������ַ����Ƿ��Ϊ����
	*/
	bool isDigitA(const std::string &str);

	/**
	*	\brief Ansi�汾 ˵�����unicode�汾�ĺ���
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