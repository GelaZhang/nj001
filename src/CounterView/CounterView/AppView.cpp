
#include <string>
#include <Windows.h>

#include "utf8/Unicode.h"
#include "AppHelper.h"
#include "StringUtil.h"

#include "CounterViewDef.h"
#include "CounterViewFrame.h"
#include "layout.h"
#include "AppView.h"

using namespace std;
using namespace Utility;

const wstring CARD_STATION_BG = L"/card_station.png";
const wstring CARD_INFO_BG = L"/card_info.png";
const wstring LAYOUT = L"/layout.xml";
AppRet CAppView::init(HWND wnd, HDC dc)
{
	m_wnd = wnd;
	CConfReader conf;
	CounterViewApp *app = CounterViewApp::getApp();
	string layoutPath = wstringToString(app->getConfigure().getResPath() + LAYOUT);
	AppRet ret = conf.loadRootElement(layoutPath.c_str());
	if ( Ret_OK != ret )
	{
		return ret;
	}
	conf.loadFrame(m_frameLocation);
	conf.loadLogo(m_logoLocation, m_logoFontSize, m_logoColor);
	conf.loadPage(m_ccsl, m_wil );
	conf.loadDefaultFont(m_defaultFontSize, m_defaultColor);
	conf.loadGreeting(m_gl);
	conf.unloadXmlElement();
	m_logoFont.CreateFont(m_logoFontSize, 0, 0, 0,  FW_BOLD , FALSE, FALSE, 0, DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Î¢ÈíÑÅºÚ"));
	m_defaultFont.CreateFont(m_defaultFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Î¢ÈíÑÅºÚ"));
	m_greetingFont.CreateFont(m_gl.greetingFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Î¢ÈíÑÅºÚ"));
	m_surface.Init(dc, m_frameLocation);

	std::vector<std::wstring> pictures;
	pictures.reserve(128);
	GetAllFilePath(app->getConfigure().getPicturePath().c_str(), L"*.*", pictures);
	for ( size_t i = 0; i < pictures.size(); i ++ )
	{
		Utility::ArrWString strs;
		splitStringW(pictures[i], L".\\", strs);
		if ( strs.size() > 1)
		{
			size_t num = strs[strs.size() - 2].find_first_not_of(L"0123456789");
			wstring id = strs[strs.size() - 2].substr(0, num);
            
           // m_pictureMap[strs[strs.size() - 2].substr(0, 
           //     strs[strs.size() - 2].find_first_not_of(L"0123456789") )] = pictures[i];
			m_pictureMap[id] = pictures[i];
		}
	}
	hideTaskBar();
	maximizeWindow();
	return Ret_OK;
}

AppRet CAppView::start()
{
	
	CheckCardStationArr ccs;
	
	showAllTerminal(L"", L"", ccs, L"");

	return Ret_OK;
}


AppRet CAppView::stop()
{
	hideTaskBar(false);
	
	return Ret_OK;
}

AppRet CAppView::uninit()
{
	return Ret_OK;
}

void CAppView::hideTaskBar(bool hide)
{
	HWND hWndTaskBar = ::FindWindow(TEXT("HHTaskBar"), NULL);
	if (hWndTaskBar != NULL)
	{
		int cmd = SW_HIDE;
		if ( !hide )
		{
			cmd = SW_SHOW;
		}
		::ShowWindow(hWndTaskBar, cmd);
	}
	::SetCursorPos(0,0);
//	::SetCursor(NULL);
//	ShowCursor(FALSE);
}

void CAppView::maximizeWindow()
{
	::SetWindowPos(m_wnd, HWND_TOP/*HWND_TOPMOST*/, m_frameLocation.left,
		m_frameLocation.top, m_frameLocation.Width(), 
		m_frameLocation.Height(), SWP_NOMOVE);
}

void CAppView::showAllTerminal(const std::wstring &totalCnt, const std::wstring &time,
					 const CheckCardStationArr &stations, const std::wstring &greeting)
{
	static wstring bgPath = CounterViewApp::getApp()->getConfigure().getResPath() 
		+ CARD_STATION_BG;
	
	m_surface.AddImage(bgPath, m_frameLocation, true);
	m_surface.AddCharacter(CounterViewApp::getApp()->getConfigure().getCompanyName().c_str(),
		m_logoLocation, m_logoColor, m_logoFont, DT_CENTER);
	m_surface.AddCharacter(totalCnt.c_str(), m_ccsl.totalCnt, m_defaultColor, m_defaultFont,DT_LEFT );
	m_surface.AddCharacter(time.c_str(), m_ccsl.time, m_defaultColor, m_defaultFont,DT_LEFT );
	CRect rc;
	for ( size_t i = 0; i < stations.size() && i < m_ccsl.lineCntOnePage; i ++ )
	{
		rc = m_ccsl.number;
		rc.top += i * m_ccsl.number.Height();
		rc.bottom += i * m_ccsl.number.Height();
		m_surface.AddCharacter(stations[i].number.c_str(), rc, m_defaultColor, m_defaultFont, DT_CENTER);
		rc = m_ccsl.position;
		rc.top += i * m_ccsl.position.Height();
		rc.bottom += i * m_ccsl.position.Height();
		m_surface.AddCharacter(stations[i].position.c_str(), rc, m_defaultColor, m_defaultFont, DT_CENTER);

		rc = m_ccsl.region;
		rc.top += i * m_ccsl.region.Height();
		rc.bottom += i * m_ccsl.region.Height();
		m_surface.AddCharacter(stations[i].region.c_str(), rc, m_defaultColor, m_defaultFont, DT_CENTER);

		rc = m_ccsl.cnt;
		rc.top += i * m_ccsl.cnt.Height();
		rc.bottom += i * m_ccsl.cnt.Height();
		m_surface.AddCharacter(stations[i].cnt.c_str(), rc, m_defaultColor, m_defaultFont, DT_CENTER);

	}
	m_surface.AddCharacter(greeting.c_str(), m_gl.greeting, m_gl.greetingColor, m_greetingFont, DT_CENTER );
	m_surface.ShowImage();
}

void CAppView::showCheckCardInfo(const WorkerInfo &info, const std::wstring &greeting)
{
	static wstring bgPath = CounterViewApp::getApp()->getConfigure().getResPath() 
		+ CARD_INFO_BG;
	

	m_surface.AddImage(bgPath, m_frameLocation, true);
	m_surface.AddCharacter(CounterViewApp::getApp()->getConfigure().getCompanyName().c_str(),
		m_logoLocation, m_logoColor, m_logoFont, DT_CENTER);
	m_surface.AddCharacter(info.cardId.c_str(), m_wil.cardId, m_defaultColor, m_defaultFont,DT_LEFT );
	m_surface.AddCharacter(info.name.c_str(), m_wil.name, m_defaultColor, m_defaultFont,DT_LEFT );
	m_surface.AddCharacter(info.checkTime.c_str(), m_wil.checkTime, m_defaultColor, m_defaultFont,DT_LEFT );
	m_surface.AddCharacter(info.id.c_str(), m_wil.id, m_defaultColor, m_defaultFont,DT_LEFT );
	m_surface.AddCharacter(info.job.c_str(), m_wil.job, m_defaultColor, m_defaultFont,DT_LEFT );
	m_surface.AddCharacter(info.power.c_str(), m_wil.power, m_defaultColor, m_defaultFont,DT_LEFT );
	m_surface.AddCharacter(greeting.c_str(), m_gl.greeting, m_gl.greetingColor, m_greetingFont, DT_CENTER );
	Str2Str::iterator itor = m_pictureMap.find(info.cardId);
	if ( itor != m_pictureMap.end() )
	{
		m_surface.AddImage(itor->second, m_wil.picture);
	}
	m_surface.ShowImage();
}

void CAppView::showGreetings(const std::wstring &txt)
{
	m_surface.AddCharacter(txt.c_str(), m_gl.greeting, m_gl.greetingColor, m_greetingFont, DT_CENTER );

	m_surface.ShowImage();
}