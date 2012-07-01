#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include <stdio.h>

#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"

class CConfReader
{
private:
	TiXmlElement* m_rootelement;
	TiXmlDocument m_xmldoc;
private:
	AppRet loadRect(TiXmlElement* element,  CRect &rect);
	AppRet loadFont(TiXmlElement *element, int &r, int &g, int &b, int &size);
	AppRet loadCheckCardStation(TiXmlElement* element, CheckCardStationLayout &layout);
	AppRet loadWorkerInfo(TiXmlElement* element, WorkerInfoLayout &layout);
public:
	CConfReader();
	~CConfReader();
	AppRet loadRootElement(const char* filename);
	AppRet loadFrame(CRect &rt);
	AppRet loadLogo(CRect &rt, int &fontSize, COLORREF &color);
	AppRet loadDefaultFont(int &fontSize, COLORREF &color);
	AppRet loadGreeting(GreetingLayout &greeting);
	AppRet loadPage(CheckCardStationLayout &layoutCCL, WorkerInfoLayout &layoutWIL);
	void unloadXmlElement();
};


#endif //! __LAYOUT_H__