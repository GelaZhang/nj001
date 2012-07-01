
//#include <locale>

#include "StringUtil.h"

using namespace std;

namespace Utility
{
	bool splitStringA(const std::string& strSource, const std::string& strDelims, std::vector<std::string>& arrResult)
	{
		arrResult.clear();
		string::size_type begIndex = strSource.find_first_not_of(strDelims);
		string::size_type endIndex;
		while ( begIndex != string::npos )
		{
			endIndex = strSource.find_first_of(strDelims, begIndex);
			if ( string::npos == endIndex )
			{
				endIndex = strSource.length();
			}

			arrResult.push_back(strSource.substr( begIndex, endIndex - begIndex ));

			begIndex = strSource.find_first_not_of(strDelims, endIndex);
		}
		
		return true;
	}

	bool splitStringW(const std::wstring& strSource, const std::wstring& strDelims, std::vector<std::wstring>& arrResult)
	{
		arrResult.clear();
		wstring::size_type begIndex = strSource.find_first_not_of(strDelims);
		wstring::size_type endIndex;
		while ( begIndex != wstring::npos )
		{
			endIndex = strSource.find_first_of(strDelims, begIndex);
			if ( wstring::npos == endIndex )
			{
				endIndex = strSource.length();
			}

			arrResult.push_back(strSource.substr( begIndex, endIndex - begIndex ));

			begIndex = strSource.find_first_not_of(strDelims, endIndex);
		}

		return true;
	}

	std::string joinStringA(const std::vector<std::string> &arrStr, const std::string &strDelimiter)
	{
		ostringstream out;
		for(vector<string>::size_type i = 0; i < arrStr.size(); i++)
		{
			if(i != 0)
			{
				out << strDelimiter;
			}
			out << arrStr[i];
		}
		return out.str();
	}

	std::wstring joinStringW(const std::vector<std::wstring> &arrStr, const std::wstring &strDelimiter)
	{
		wostringstream out;
		for(vector<wstring>::size_type i = 0; i < arrStr.size(); i++)
		{
			if(i != 0)
			{
				out << strDelimiter;
			}
			out << arrStr[i];
		}
		return out.str();
	}

	std::wstring trimW(const std::wstring &str)
	{
		static const wstring strDest = L" \t\r\n";
		std::wstring::size_type begIndex = str.find_first_not_of(strDest);
		if ( std::wstring::npos == begIndex )
		{
			return L"";
		}
		else
		{
			return str.substr(begIndex, str.find_last_not_of(strDest) - begIndex + 1 );
		}
	}

	std::string trimA(const std::string &str)
	{
		static const string strDest = " \t\r\n";
		std::string::size_type begIndex = str.find_first_not_of(strDest);
		if ( std::string::npos == begIndex )
		{
			return "";
		}
		else
		{
			return str.substr(begIndex, str.find_last_not_of(strDest) - begIndex + 1 );
		}
	}

	std::wstring trimLeftW(const std::wstring &str)
	{
		static const wstring strDest = L" \t\r\n";
		std::wstring::size_type begIndex = str.find_first_not_of(strDest);
		if ( std::wstring::npos == begIndex )
		{
			return L"";
		}
		else
		{
			return str.substr( begIndex );
		}
	}

	std::string trimLeftA(const std::string &str)
	{
		static const string strDest = " \t\r\n";
		std::string::size_type begIndex = str.find_first_not_of(strDest);
		if ( std::string::npos == begIndex )
		{
			return "";
		}
		else
		{
			return str.substr( begIndex );
		}
	}

	std::wstring trimRightW(const wstring &str)
	{
		static const wstring strDest = L" \t\r\n";
		std::wstring::size_type endIndex = str.find_last_not_of(strDest);
		if ( std::wstring::npos == endIndex )
		{
			return L"";
		}
		else
		{
			return str.substr( 0, endIndex + 1);
		}
	}

	std::string trimRightA(const string &str)
	{
		static const string strDest = " \t\r\n";
		std::string::size_type endIndex = str.find_last_not_of(strDest);
		if ( std::string::npos == endIndex )
		{
			return "";
		}
		else
		{
			return str.substr( 0, endIndex + 1);
		}
	}

	std::string toLowerA(const std::string &str)
	{
		string result;
		result.reserve(str.size());
		for(string::size_type i = 0; i < str.length(); ++i)
		{
			if(isascii(str[i]))
			{
				result += tolower(static_cast<unsigned char>(str[i]));
			}
			else
			{
				result += str[i];
			}
		}
		return result;
	}
// 	std::string toUpperA(const std::string &str)
// 	{
// 		string result;
// 		result.reserve(str.size());
// 		for(string::size_type i = 0; i < str.length(); ++i)
// 		{
// 			if(isascii(str[i]))
// 			{
// 				result += toupper(static_cast<unsigned char>(str[i]));
// 			}
// 			else
// 			{
// 				result += str[i];
// 			}
// 		}
// 		return result;
// 	}

// 	std::wstring toUpperW(const std::wstring &str)
// 	{
// 		std::locale loc;
// 		std::wstring result;
// 		result.reserve(str.size());
// 		for(wstring::size_type i = 0; i < str.length(); ++i)
// 		{
// 			if(iswascii(str[i]))
// 			{
// 				result += std::toupper<wchar_t>(str[i], loc);
// 			}
// 			else
// 			{
// 				result += str[i];
// 			}
// 		}
// 		return result;
// 	}

	bool isAlphaW(wchar_t c)
	{
		 return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	bool isAlphaA(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	bool isDigitW(wchar_t c)
	{
		return c >= '0' && c <= '9';
	}

	bool isDigitW(const wstring &str)
	{
		for ( wstring::size_type i = 0; i < str.size(); i ++ )
		{
			if ( 0 == i && str[0] == '-')
				continue;
			if ( !isDigitW(str[i] ) )
				return false;
		}
		return true;
	}

	bool isDigitA(const string &str)
	{
		for ( string::size_type i = 0; i < str.size(); i ++ )
		{
			if ( 0 == i && str[0] == '-')
				continue;
			if ( !isDigitA(str[i] ) )
				return false;
		}
		return true;
	}

	bool isDigitA(char c)
	{
		return c >= '0' && c <= '9';
	}

	std::wstring removeWhitespaceW(const std::wstring &str)
	{
		wstring result;
		for(wstring::size_type i = 0; i < str.length(); ++ i)
		{
			if(!iswspace(static_cast<wint_t>(str[i])))
			{
				result += str[i];
			}
		}
		return result;
	}
	std::string removeWhitespaceA(const std::string &str)
	{
		string result;
		for(string::size_type i = 0; i < str.length(); ++ i)
		{
			if(!isspace(static_cast<unsigned char>(str[i])))
			{
				result += str[i];
			}
		}
		return result;
	}
}
