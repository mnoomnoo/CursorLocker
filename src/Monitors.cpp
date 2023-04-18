

#include "Monitors.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::vector<MONITORINFO> monitors;

////////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	std::vector<MONITORINFO>* mons = reinterpret_cast<std::vector<MONITORINFO>*>(pData);

	MONITORINFO mi;
	mi.cbSize = sizeof MONITORINFO;
	if( GetMonitorInfo( hMon, &mi ) )
	{
		mons->push_back(mi);
	}

	return TRUE;
}

bool GetPrimaryMonitor(MONITORINFO& monInfo) 
{
	for (size_t i = 0; i < monitors.size(); i++)
	{
		MONITORINFO mi = monitors[i];
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
	MONITORINFO monInfo;

	if (GetPrimaryMonitor(monInfo))
	{
		primaryMonitorScreenRect = monInfo.rcMonitor;
	}	
	
	return primaryMonitorScreenRect;
}