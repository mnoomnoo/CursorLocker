


#include "Monitors.h"

static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	MONITORINFO mi;
	mi.cbSize = sizeof MONITORINFO;
	if( GetMonitorInfo( hMon, &mi ) )
	{
		if( MONITORINFOF_PRIMARY == ( mi.dwFlags & MONITORINFOF_PRIMARY ) )
		{
			RECT* primaryMonitorScreenRect = reinterpret_cast<RECT*>(pData);
			*primaryMonitorScreenRect = *lprcMonitor;
		}
	}

	return TRUE;
}

RECT GetPrimaryMonitorScreenRect()
{
	RECT primaryMonitorScreenRect = {0,0,0,0};
	EnumDisplayMonitors(nullptr, nullptr, MonitorEnum, (LPARAM)&primaryMonitorScreenRect);
	return primaryMonitorScreenRect;
}