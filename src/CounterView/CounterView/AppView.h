#pragma once

#include <Windows.h>

#include "surface/Surface.h"
class CAppView
{
public:
	AppRet init(HWND wnd, HDC dc);
	
	AppRet start();
	
	AppRet stop();
	
	AppRet uninit();

public:
	void showCheckCardInfo(const WorkerInfo &info, const std::wstring &greeting);

	void showAllTerminal(const std::wstring &totalCnt, const std::wstring &time,
		const CheckCardStationArr &stations, const std::wstring &greeting);
	
	void showGreetings(const std::wstring &txt);

	inline int CheckCardStationLineCnt(){return m_ccsl.lineCntOnePage;};

	inline int GreetingWordCnt(){return m_gl.wordCnt;};

protected:
	void hideTaskBar(bool hide = true);
	
	void maximizeWindow();

protected:
	HWND m_wnd;

	CRect m_frameLocation;
	CRect m_logoLocation;
	int m_logoFontSize;
	COLORREF m_logoColor;
	CFont m_logoFont;
	int m_defaultFontSize;
	COLORREF m_defaultColor;
	CFont m_defaultFont;
	CFont m_greetingFont;
	GreetingLayout m_gl;
	CheckCardStationLayout m_ccsl;
	WorkerInfoLayout m_wil;

	CSurface m_surface;
	
	typedef std::map<std::wstring, std::wstring> Str2Str;
	Str2Str m_pictureMap;
};