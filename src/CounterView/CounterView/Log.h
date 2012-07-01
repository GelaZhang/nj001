#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include "kLog.h"

namespace Utility
{
    /**
    *   \brief 日志模块。为CkLog的适配器，方便使用C++输出流的方式进行LOG记录。
    *   \remark 每个进程只能拥有一个log文件。
    *           使用方法：
    *           CLog()<<2<<L" 工作";  --这是记录一条普通信息的LOG,结果：类型 日期 内容
    *           CLog(LOG_WARN)<<2<<L" 工作";  --这是记录一条告警信息的LOG,结果：类型 日期 内容
    *           CLog(LOG_ERR)<<2<<L" 工作";  --这是记录一条告警信息的LOG,结果：类型 日期 内容
    */
    class CLog : public std::wostringstream
    {
    public:
        explicit CLog(CC_Log type = LOG_INFO)
        {
            m_type = type;
        }
        ~CLog()
        {
            static std::wstring name = L"日志模块";
            s_log.WriteLog(m_type, name.c_str(), str().c_str());
#ifdef WINCE
//             RETAILMSG(TRUE, (L"[%d][%s][%s]%s\n" , m_type, 
//                 GetCurrentTimeStr().c_str() , name.c_str(), str().c_str()) );
#endif
        }

    public:
        /**
        *   \brief 初始该日志模块。只能调用一次。
        *   \param[in] path 日志文件的路径。
        */
        static bool initLog(const wchar_t *path, int fileSize);
        /**
        *   \brief 清理日志模块。只能调用一次。结束日志的使用时调用。
        */
        static bool unInitLog();
    private:
        static CkLog s_log;
        static bool s_init;
    private:
        CC_Log m_type;
    };
}

#endif//!__LOG_H__