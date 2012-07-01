
#include <assert.h>
#include "ConvertUTF.h"
#include "Unicode.h"

using namespace std;
namespace Utility
{
 
    //
    // Helper class, base never defined
    // Usage: WstringHelper<sizeof(wchar_t)>::toUTF8 and fromUTF8.
    //
    template<size_t wcharSize> 
    struct WstringHelper
    {
        static ConversionResult toUTF8(
            const wchar_t*& sourceStart, const wchar_t* sourceEnd, 
            Byte*& targetStart, Byte* targetEnd, ConversionFlags flags);

        static ConversionResult fromUTF8(
            const Byte*& sourceStart, const Byte* sourceEnd, 
            wchar_t*& targetStart, wchar_t* targetEnd, ConversionFlags flags);
    };

    template<>
    struct WstringHelper<2>
    {
        static ConversionResult toUTF8(
            const wchar_t*& sourceStart, const wchar_t* sourceEnd, 
            Byte*& targetStart, Byte* targetEnd, ConversionFlags flags)
        {
            return ConvertUTF16toUTF8(
                reinterpret_cast<const UTF16**>(&sourceStart),
                reinterpret_cast<const UTF16*>(sourceEnd),
                &targetStart, targetEnd, flags);
        }

        static ConversionResult fromUTF8(
            const Byte*& sourceStart, const Byte* sourceEnd, 
            wchar_t*& targetStart, wchar_t* targetEnd, ConversionFlags flags)
        {
            return ConvertUTF8toUTF16(
                &sourceStart, sourceEnd,
                reinterpret_cast<UTF16**>(&targetStart),
                reinterpret_cast<UTF16*>(targetEnd), flags);
        }
    };

    template<>
    struct WstringHelper<4>
    {
        static ConversionResult toUTF8(
            const wchar_t*& sourceStart, const wchar_t* sourceEnd, 
            Byte*& targetStart, Byte* targetEnd, ConversionFlags flags)
        {
            return ConvertUTF32toUTF8(
                reinterpret_cast<const UTF32**>(&sourceStart),
                reinterpret_cast<const UTF32*>(sourceEnd),
                &targetStart, targetEnd, flags);
        }

        static ConversionResult fromUTF8(
            const Byte*& sourceStart, const Byte* sourceEnd, 
            wchar_t*& targetStart, wchar_t* targetEnd, ConversionFlags flags)
        {
            return ConvertUTF8toUTF32(
                &sourceStart, sourceEnd,
                reinterpret_cast<UTF32**>(&targetStart),
                reinterpret_cast<UTF32*>(targetEnd), flags);
        }
    };

    ConversionResult 
    convertUTFWstringToUTF8(
        const wchar_t*& sourceStart, const wchar_t* sourceEnd, 
        Byte*& targetStart, Byte* targetEnd, ConversionFlags flags)
    {
        return WstringHelper<sizeof(wchar_t)>::toUTF8(
            sourceStart, sourceEnd, targetStart, targetEnd, flags);
    }

    ConversionResult
    convertUTF8ToUTFWstring(
        const Byte*& sourceStart, const Byte* sourceEnd, 
        wchar_t*& targetStart, wchar_t* targetEnd, ConversionFlags flags)
    {
        return WstringHelper<sizeof(wchar_t)>::fromUTF8(
            sourceStart, sourceEnd, targetStart, targetEnd, flags);
    }

    ConversionResult 
    convertUTF8ToUTFWstring(const Byte*& sourceStart, const Byte* sourceEnd, 
        std::wstring& target, ConversionFlags flags)
    {
        //
        // Could be reimplemented without this temporary wchar_t buffer
        //
        size_t size = static_cast<size_t>(sourceEnd - sourceStart);
        wchar_t* outBuf = new wchar_t[size];
        wchar_t* targetStart = outBuf; 
        wchar_t* targetEnd = targetStart + size;

        ConversionResult result =  
            convertUTF8ToUTFWstring(sourceStart, sourceEnd, targetStart,
            targetEnd, flags);

        if(result == conversionOK)
        {
            std::wstring s(outBuf, static_cast<size_t>(targetStart - outBuf));
            s.swap(target);
        }
        delete[] outBuf;
        return result;
    }

    std::string wstringToString(const std::wstring& source, ConversionFlags flag/*= lenientConversion*/)
    {
        string target;

        size_t size = source.size() * 3 * (sizeof(wchar_t) / 2);

        Byte* outBuf = new Byte[size];
        Byte* targetStart = outBuf; 
        Byte* targetEnd = outBuf + size;

        const wchar_t* sourceStart = source.data();

        ConversionResult cr = 
            convertUTFWstringToUTF8(
            sourceStart, sourceStart + source.size(), 
            targetStart, targetEnd, flag);

        if(cr != conversionOK)
        {
            delete[] outBuf;
            assert(cr == sourceExhausted || cr == sourceIllegal);
            return "";
        }

        string s(reinterpret_cast<char*>(outBuf),
            static_cast<size_t>(targetStart - outBuf));
        s.swap(target);
        delete[] outBuf;
        return target;
    }

    wstring
    stringToWstring(const string& str, ConversionFlags flags)
    {
        wstring result;
        const Byte* sourceStart = reinterpret_cast<const Byte*>(str.data());

        ConversionResult cr 
            = convertUTF8ToUTFWstring(sourceStart, sourceStart + str.size(),
            result, flags);

        if(cr != conversionOK)
        {
            assert(cr == sourceExhausted || cr == sourceIllegal);

            return L"";
        }
        return result;
    }

}
