#ifndef _ELCD_CONFIGURE_H_
#define _ELCD_CONFIGURE_H_

/**
*  \brief 用于读取conf.xml
*/

#include "stdio.h"

#include <string>

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

#include "CounterViewDef.h"
class CConfigure
{
private:
	TiXmlDocument m_xmlDoc;
	TiXmlElement* m_xmlRoot;
private:

	std::wstring getElement(const char* node);

	std::wstring getAttr(const TiXmlElement* element, const char* attrName);

	const TiXmlElement* childElement(const TiXmlElement* element,
		const char* node);
public:
	CConfigure();
	~CConfigure();
	AppRet load(const std::wstring &path);

	std::wstring getAppVersion();
	std::wstring getCompanyName();
	std::wstring getResPath();
	std::wstring getPicturePath();
	std::wstring getServerIp();
	int getServerPort();
	int getCom();
	int getBaudRate();
	std::wstring getGreeting();
	int getAddress();
	int getShiftFrequency();
};

#endif//!_ELCD_CONFIGURE_H_
