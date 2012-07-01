#include "CounterViewConfigure.h"



#include "utf8/Unicode.h"
using namespace Utility;

CConfigure::CConfigure()
{
    m_xmlRoot = NULL;
}

CConfigure::~CConfigure()
{
    if (m_xmlRoot)
    {
        m_xmlRoot->Clear();
        m_xmlRoot = NULL;
    }
	m_xmlDoc.Clear();
}

AppRet CConfigure::load(const std::wstring &path)
{
		
    if ( !m_xmlDoc.LoadFile(wstringToString(path).c_str()) )
	{
		return Ret_ERR;
	}

    m_xmlRoot = m_xmlDoc.FirstChildElement("terminal");
	return m_xmlRoot == NULL ? Ret_ERR : Ret_OK;

}

std::wstring CConfigure::getElement(const char* node)
{
    if (m_xmlRoot  == NULL  )
	{
		return L"";
	}

    TiXmlElement* element = m_xmlRoot->FirstChildElement(node);
    if (element == NULL )
	{
		return L"";
	}

    if (element->FirstChild() == NULL )
	{
		return L"";
	}

    return stringToWstring(element->FirstChild()->Value());
}

const TiXmlElement* CConfigure::childElement(const TiXmlElement* element, 
                                                 const char* node)
{
    if ( element == NULL || node == NULL)
	{
		return NULL;
	}

    return element->FirstChildElement(node);
}

std::wstring CConfigure::getAttr(const TiXmlElement* element,
        const char* attrName)
{
    if (element == NULL )
	{
		return NULL;
	}

    return stringToWstring(element->Attribute(attrName));
}



std::wstring CConfigure::getAppVersion()
{
    return getElement("version");
}


std::wstring CConfigure::getCompanyName()
{
	return getElement("company");
	
}

std::wstring CConfigure::getResPath()
{
	return getElement("res_dir");
}

std::wstring CConfigure::getPicturePath()
{
	return getElement("picture");
}

std::wstring CConfigure::getServerIp()
{
	return getElement("server_ip");
}

int CConfigure::getServerPort()
{
	int ret = _ttoi(getElement("port").c_str());
	return ret;
}

int CConfigure::getCom()
{
	int ret = _ttoi(getElement("com").c_str());
	return ret;
}
int CConfigure::getBaudRate()
{
	int ret = _ttoi(getElement("baud_rate").c_str());
	return ret;
}

std::wstring CConfigure::getGreeting()
{
	return getElement("default_greeting");
}

int CConfigure::getAddress()
{
	const static int MAX_ADDRESS = 0x7F;
	int i =  _ttoi(getElement("address").c_str());
	if ( i > MAX_ADDRESS )
	{
		i = 0;
	}
	return i;
}
int CConfigure::getShiftFrequency()
{
	int ret = _wtoi(getElement("shift_frequency").c_str());
	return ret;
}