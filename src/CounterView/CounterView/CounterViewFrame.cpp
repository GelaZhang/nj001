#include <string>

#include "AppHelper.h"
#include "Log.h"
#include "CounterViewFrame.h"

using namespace std;
using namespace Utility;
const wstring CONF_FILE_NAME = L"/conf.xml";
const wstring LOG_FILE_NAME = L"/log.log";
CounterViewApp *CounterViewApp::getApp()
{
	static CounterViewApp app;
	return &app;
}
AppRet CounterViewApp::init(HWND wnd, HDC dc)
{
	wstring logfile = GetAppPathW() + LOG_FILE_NAME;
	CLog::initLog(logfile.c_str(), 1024*1024);
	wstring confPath = GetAppPathW() + CONF_FILE_NAME;
 	AppRet ret = m_conf.load(confPath);
 	if ( Ret_OK != ret )
 	{
 		return ret;
 	}
 	ret = m_view.init(wnd, dc);
	if ( Ret_OK != ret )
 	{
 		return ret;
 	}

	m_net.init(Unicode2MBCS(m_conf.getServerIp().c_str() ).c_str(), m_conf.getServerPort() );

	return Ret_OK;
}

AppRet CounterViewApp::start()
{
	AppRet ret = m_view.start();
 	if ( Ret_OK != ret )
 	{
 		return ret;
 	}
	m_net.run();
	m_carReader.start(m_conf.getCom(), m_conf.getBaudRate(), &m_net);
	return Ret_OK;
}


AppRet CounterViewApp::stop()
{
	m_carReader.stop();
	m_net.stop();
	AppRet ret = m_view.stop();
 	if ( Ret_OK != ret )
 	{
 		return ret;
 	}
	CLog::unInitLog();
	return Ret_OK;
}

AppRet CounterViewApp::uninit()
{
	m_net.uninit();
	AppRet ret = m_view.uninit();
 	if ( Ret_OK != ret )
 	{
 		return ret;
 	}
	return Ret_OK;
}