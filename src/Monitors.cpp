

#include "Monitors.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::vector<MONITORINFOEX> monitors;

////////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	std::vector<MONITORINFOEX>* mons = reinterpret_cast<std::vector<MONITORINFOEX>*>(pData);

	MONITORINFOEX mi;
	mi.cbSize = sizeof MONITORINFOEX;
	if( GetMonitorInfo( hMon, &mi ) )
	{
		mons->push_back(mi);
	}

	return TRUE;
}

bool GetPrimaryMonitor(MONITORINFOEX& monInfo) 
{
	for (size_t i = 0; i < monitors.size(); i++)
	{
		MONITORINFOEX mi = monitors[i];
		if (MONITORINFOF_PRIMARY == (mi.dwFlags & MONITORINFOF_PRIMARY))
		{
			monInfo = mi;
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

RECT GetPrimaryMonitorScreenRect_DPIScaled()
{
	RECT primaryMonitorScreenRect = {0,0,0,0};
	MONITORINFOEX monInfo;

	if (GetPrimaryMonitor(monInfo))
	{
		primaryMonitorScreenRect = monInfo.rcMonitor;
	}	
	
	return primaryMonitorScreenRect;
}