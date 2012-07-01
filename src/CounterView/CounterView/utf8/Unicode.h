#ifndef __UNICODE_H__
#define __UNICODE_H__

#include <string>

namespace Utility
{

    enum ConversionFlags
    {
        strictConversion = 0,
        lenientConversion
    };
    typedef unsigned char Byte;

    std::string wstringToString(const std::wstring& source, ConversionFlags = lenientConversion);
    std::wstring stringToWstring(const std::string& source, ConversionFlags = lenientConversion);
    bool isLegalUTF8Sequence(const Byte* source, const Byte* end);
}
#endif//__UNICODE_H__