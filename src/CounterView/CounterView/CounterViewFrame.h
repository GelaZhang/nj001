#pragma once

#include "CounterViewDef.h"
#include "AppView.h"
#include "NetworkProcess.h"
#include "CardReader.h"
#include "CounterViewConfigure.h"
class CounterViewApp
{
public:
	static CounterViewApp *getApp();
public:
	
	AppRet init(HWND wnd, HDC dc);
	AppRet start();
	AppRet stop();
	AppRet uninit();

	inline CAppView &getView(){return m_view;}

	inline CConfigure &getConfigure()
	{
		return m_conf;
	}

protected:
	CConfigure m_conf;
	CAppView m_view;
	CNetworkProcess m_net;
	CardReader m_carReader;
};