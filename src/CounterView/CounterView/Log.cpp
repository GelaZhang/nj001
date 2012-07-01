
#include "Log.h"

using namespace Utility;

CkLog CLog::s_log;
bool CLog::s_init = false;

bool CLog::initLog(const wchar_t *path, int fileSize)
{
    if ( NULL == path || s_init )
    {
        return false;
    }

    if ( s_log.InitLog(path, fileSize) )
    {
        s_init = true;
    }

    return s_init;

}

bool CLog::unInitLog()
{
    if ( !s_init )
    {
        return false;
    }

    return TRUE == s_log.UninitLog()? true : false;
}