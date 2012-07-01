
#include <string>

#include <Windows.h>

#include "CounterViewDef.h"
#include "layout.h"

using namespace std;
CConfReader::CConfReader()
{
	m_rootelement = NULL;
}

CConfReader::~CConfReader()
{
	unloadXmlElement();
}

AppRet CConfReader::loadRootElement(const char* filename)
{
	if (!m_xmldoc.LoadFile(filename))
	{
		return Ret_ERR;
	}
	m_rootelement = m_xmldoc.FirstChildElement("layout");
	
	return m_rootelement == NULL ? Ret_ERR : Ret_OK;

}

AppRet CConfReader::loadRect(TiXmlElement* element, CRect &rect)
{

	if ( NULL == element )
	{
		return Ret_Invalid_Param;
	}
	const char* str = element->Attribute("left");
	if ( !str )
	{
		return Ret_ERR;
	}
	rect.left = atoi(str);
	str = element->Attribute("top");
	if ( !str )
	{
		return Ret_ERR;
	}
	rect.top = atoi(str);
	str = element->Attribute("right");
	if ( !str )
	{
		return Ret_ERR;
	}
	rect.right = atoi(str);
	str = element->Attribute("bottom");
	if ( !str )
	{
		return Ret_ERR;
	}
	rect.bottom = atoi(str);
	
	return Ret_OK;
}

AppRet CConfReader::loadFont(TiXmlElement *element, int &r, int &g, int &b, int &size)
{
	if ( NULL == element )
	{
		return Ret_Invalid_Param;
	}
	const char* str = element->Attribute("r");
	if ( !str )
	{
		return Ret_ERR;
	}
	r = atoi(str);
	str = element->Attribute("g");
	if ( !str )
	{
		return Ret_ERR;
	}
	g = atoi(str);
	str = element->Attribute("b");
	if ( !str )
	{
		return Ret_ERR;
	}
	b = atoi(str);
	str = element->Attribute("size");
	if ( !str )
	{
		return Ret_ERR;
	}
	size = atoi(str);

	return Ret_OK;
}

AppRet CConfReader::loadFrame(CRect &rt)
{
	if ( m_rootelement == NULL )
	{
		return Ret_ERR;
	}

	TiXmlElement* element = m_rootelement->FirstChildElement("frame");
	AppRet ret = loadRect(element, rt);
	
	if (element)
	{
		element->Clear();
	}
	return ret;
}

AppRet CConfReader::loadLogo(CRect &rt, int &fontSize, COLORREF &color)
{
	if ( m_rootelement == NULL )
	{
		return Ret_ERR;
	}

	TiXmlElement* element = m_rootelement->FirstChildElement("logo");
	AppRet ret = loadRect(element, rt);
	if ( Ret_OK != ret )
	{
		return ret;
	}

	TiXmlElement* fontElement = element->FirstChildElement("font");
	int r, g, b;
	ret = loadFont(fontElement, r, g, b, fontSize);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	color = RGB(r,g,b);

	if ( fontElement )
	{
		fontElement->Clear();
	}
	if (element)
	{
		element->Clear();
	}
	
	return Ret_OK;
}

AppRet CConfReader::loadDefaultFont(int &fontSize, COLORREF &color)
{
	if ( m_rootelement == NULL )
	{
		return Ret_ERR;
	}

	TiXmlElement* element = m_rootelement->FirstChildElement("font");
	int r, g, b;
	AppRet ret = loadFont(element, r, g, b, fontSize);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	color = RGB(r,g, b);

	return Ret_OK;
}


AppRet CConfReader::loadCheckCardStation(TiXmlElement* element, CheckCardStationLayout &layout)
{
	if ( element == NULL )
	{
		return Ret_ERR;
	}

	TiXmlElement* tmpE = element->FirstChildElement("total_cnt");
	AppRet ret = loadRect(tmpE, layout.totalCnt);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("time");
	ret = loadRect(tmpE, layout.time);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("number");
	ret = loadRect(tmpE, layout.number);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("position");
	ret = loadRect(tmpE, layout.position);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("region");
	ret = loadRect(tmpE, layout.region);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("cnt");
	ret = loadRect(tmpE, layout.cnt);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	
	tmpE = element->FirstChildElement("line_cnt");
	if ( tmpE == NULL )
	{
		return Ret_ERR;
	}
	const char * lineCnt = tmpE->GetText();
	if ( NULL == lineCnt )
	{
		return Ret_ERR;
	}
	layout.lineCntOnePage = atoi(lineCnt);
	return Ret_OK;

}

AppRet CConfReader::loadWorkerInfo(TiXmlElement* element, WorkerInfoLayout &layout)
{
	if ( element == NULL )
	{
		return Ret_ERR;
	}

	TiXmlElement* tmpE = element->FirstChildElement("card_id");
	AppRet ret = loadRect(tmpE, layout.cardId);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("name");
	ret = loadRect(tmpE, layout.name);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("power");
	ret = loadRect(tmpE, layout.power);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("job");
	ret = loadRect(tmpE, layout.job);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("id");
	ret = loadRect(tmpE, layout.id);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("check_time");
	ret = loadRect(tmpE, layout.checkTime);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	tmpE = element->FirstChildElement("picture");
	ret = loadRect(tmpE, layout.picture);
	
	return ret;
}

AppRet CConfReader::loadPage(CheckCardStationLayout &layoutCCL, WorkerInfoLayout &layoutWIL)
{
	TiXmlElement* element = m_rootelement->FirstChildElement("page");
	AppRet ret = loadWorkerInfo(element, layoutWIL);
	if ( ret != Ret_OK )
	{
		return ret;
	}
	element = element->NextSiblingElement("page");
	ret = loadCheckCardStation(element, layoutCCL);
	return ret;
}

AppRet CConfReader::loadGreeting(GreetingLayout &greeting)
{
	TiXmlElement* element = m_rootelement->FirstChildElement("greetings");
	if ( element == NULL )
	{
		return Ret_ERR;
	}

	AppRet ret = loadRect(element, greeting.greeting);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	const char* str = element->Attribute("word_cnt");
	if ( !str )
	{
		return Ret_ERR;
	}
	greeting.wordCnt = atoi(str);

	TiXmlElement* fontElement = element->FirstChildElement("font");
	int r, g, b;
	ret = loadFont(fontElement, r, g, b, greeting.greetingFontSize);
	if ( Ret_OK != ret )
	{
		return ret;
	}
	greeting.greetingColor = RGB(r,g,b);

	return Ret_OK;
}

void CConfReader::unloadXmlElement()
{
	m_rootelement = NULL;
	m_xmldoc.Clear();
}
