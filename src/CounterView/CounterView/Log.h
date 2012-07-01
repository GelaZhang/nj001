#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include "kLog.h"

namespace Utility
{
    /**
    *   \brief ��־ģ�顣ΪCkLog��������������ʹ��C++������ķ�ʽ����LOG��¼��
    *   \remark ÿ������ֻ��ӵ��һ��log�ļ���
    *           ʹ�÷�����
    *           CLog()<<2<<L" ����";  --���Ǽ�¼һ����ͨ��Ϣ��LOG,��������� ���� ����
    *           CLog(LOG_WARN)<<2<<L" ����";  --���Ǽ�¼һ���澯��Ϣ��LOG,��������� ���� ����
    *           CLog(LOG_ERR)<<2<<L" ����";  --���Ǽ�¼һ���澯��Ϣ��LOG,��������� ���� ����
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
            static std::wstring name = L"��־ģ��";
            s_log.WriteLog(m_type, name.c_str(), str().c_str());
#ifdef WINCE
//             RETAILMSG(TRUE, (L"[%d][%s][%s]%s\n" , m_type, 
//                 GetCurrentTimeStr().c_str() , name.c_str(), str().c_str()) );
#endif
        }

    public:
        /**
        *   \brief ��ʼ����־ģ�顣ֻ�ܵ���һ�Ρ�
        *   \param[in] path ��־�ļ���·����
        */
        static bool initLog(const wchar_t *path, int fileSize);
        /**
        *   \brief ������־ģ�顣ֻ�ܵ���һ�Ρ�������־��ʹ��ʱ���á�
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