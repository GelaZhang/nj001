#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include "atltypes.h"


enum AppRet
{
	Ret_ERR = 0,
	Ret_OK,
	Ret_Invalid_Param,
	Ret_Disconnect,
	Ret_Quit
};

struct CheckCardStation
{
	std::wstring number;
	std::wstring position;
	std::wstring region;
	std::wstring cnt;
};

struct WorkerInfo
{
	std::wstring cardId;
	std::wstring name;
	std::wstring power;
	std::wstring job;
	std::wstring id;
	std::wstring checkTime;
};

struct CheckCardStationLayout
{
	CRect number;
	CRect position;
	CRect region;
	CRect cnt;
	CRect totalCnt;
	CRect time;
	int lineCntOnePage;
};

struct WorkerInfoLayout
{
	CRect cardId;
	CRect name;
	CRect power;
	CRect job;
	CRect id;
	CRect checkTime;
	CRect picture;
	int lineCntOnePage;
};

struct GreetingLayout
{
	CRect greeting;
	int greetingFontSize;
	COLORREF greetingColor;
	int wordCnt;
};

typedef std::vector<CheckCardStation> CheckCardStationArr;
typedef std::map<std::wstring, CheckCardStation> CheckCardStationMap;