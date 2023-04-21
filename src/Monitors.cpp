

#include "Monitors.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct Monitor
{
	HMONITOR monitorHandle;
	MONITORINFOEX monitorInfo;
	HDC hdc;

	Monitor()
	{
		monitorHandle = nullptr; 
		memset(&monitorInfo, 0, sizeof(MONITORINFOEX));
		hdc = nullptr;
	}

	~Monitor()
	{
		//
	}

	RECT GetMonitorScreenRect()
	{
		return monitorInfo.rcMonitor;
	}
};

std::vector<Monitor> monitors;

////////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	std::vector<Monitor>* mons = reinterpret_cast<std::vector<Monitor>*>(pData);

	MONITORINFOEX mi;
	mi.cbSize = sizeof MONITORINFOEX;
	if( GetMonitorInfo( hMon, &mi ) )
	{
		HDC dc = CreateDC(NULL, mi.szDevice, NULL, NULL);

		Monitor wm;
		wm.monitorHandle = hMon;
		wm.monitorInfo = mi;
		wm.hdc = dc;

		mons->push_back(wm);
	}

	return TRUE;
}

bool GetPrimaryMonitor(Monitor& mon) 
{
	for (size_t i = 0; i < monitors.size(); i++)
	{
		Monitor mi = monitors[i];
		if (MONITORINFOF_PRIMARY == (mi.monitorInfo.dwFlags & MONITORINFOF_PRIMARY))
		{
			mon = mi;
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void InitMonitorAPI()
{
	monitors.reserve(5);

	EnumDisplayMonitors(nullptr, nullptr, MonitorEnum, (LPARAM)&monitors);

}

void CleanupMonitorAPI()
{
	for (size_t i = 0; i < monitors.size(); i++)
	{
		Monitor& mi = monitors[i];
		DeleteDC(mi.hdc);
	}
	monitors.clear();
}

RECT GetPrimaryMonitorScreenRect_DPIScaled()
{
	RECT primaryMonitorScreenRect = {0,0,0,0};
	Monitor mon;
	if (GetPrimaryMonitor(mon))
	{
		primaryMonitorScreenRect = mon.GetMonitorScreenRect();
	}	
	
	return primaryMonitorScreenRect;
}

void DrawRectOnPrimaryMonitor()
{
	Monitor mon;
	if (GetPrimaryMonitor(mon))
	{
		RECT colorRect = {200,200,250,250};
		FillRect(mon.hdc, &colorRect, (HBRUSH) (COLOR_HOTLIGHT+1));
	}
}
