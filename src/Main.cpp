

#include <aclapi.h>
#include <wtsapi32.h>

#include <wchar.h>
#include <thread>

#include "Common.h"
#include "ProgramArgs.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

DWORD GetProcessID( const std::wstring& processName )
{
	DWORD processID = 0;

	WTS_PROCESS_INFO* processes;
	DWORD processCount;
	if( WTSEnumerateProcesses(
			WTS_CURRENT_SERVER_HANDLE,
			0, // reserved
			1, // Specifies the version of the enumeration request. Must be 1.
			&processes,
			&processCount
		)
	)
	{
		for( unsigned int c = 0; c < processCount; c++ )
		{
			if( processName == std::wstring(processes[c].pProcessName) )
			{
				processID = processes[c].ProcessId;
				break;
			}
		}

		// OK we need to clean up
		WTSFreeMemory( processes );
	}

	return processID;
}

BOOL IsProcessRunning(HANDLE process)
{
	// Thanks: https://stackoverflow.com/questions/1591342/c-how-to-determine-if-a-windows-process-is-running
	const DWORD ret = WaitForSingleObject(process, 0);    
	return ret == WAIT_TIMEOUT;
}

static BOOL CALLBACK MonitorEnum(HMONITOR hMon,HDC hdc,LPRECT lprcMonitor,LPARAM pData)
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
	RECT primaryMonitorScreenRect;
	EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)&primaryMonitorScreenRect);
	return primaryMonitorScreenRect;
}

void SetWindowsStation()
{
	HWINSTA thisWindowStation = OpenWindowStation(
		L"winsta0", 
		FALSE,
		WINSTA_READATTRIBUTES | WINSTA_WRITEATTRIBUTES
	);

	BOOL bRes = SetProcessWindowStation( thisWindowStation );
	bRes = CloseWindowStation( thisWindowStation );
}

void LockCursor( const RECT& lpRect )
{
	ClipCursor(&lpRect);
}

void UnlockCursor( const RECT& oldCursorClipRect )
{
	ClipCursor(&oldCursorClipRect); 
}

////////////////////////////////////////////////////////////////////////////////

int wmain( int argc , wchar_t** argv )
{
	PrintToConsole( "\n" );
	PrintToConsole( "CursorLocker V: " << PROGRAM_VERSION << "\n" );
	PrintToConsole( "\n" );

	ProgramCmdLineOptions programCmdLineOptions;
	if( !ProcessProgramArgs(argc, argv, programCmdLineOptions) )
	{
		return 1;
	}
	
	SetWindowsStation();

	RECT oldCursorClipRect;
	GetClipCursor(&oldCursorClipRect); 

	HANDLE exePocessHandle = nullptr;
	if( DWORD exePID = GetProcessID( programCmdLineOptions.exeName ) )
	{
		exePocessHandle = OpenProcess(SYNCHRONIZE, FALSE, exePID);
		PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" is already running! Cursor is locked to the primary monitor.\n" );
	}
	else
		PrintToConsole( "Waiting for process: \"" << programCmdLineOptions.exeName.c_str() << "\" to start...\n" );

	bool bRunning = true;
	while( bRunning )
	{
		// if the exe of interest's PID is 0 then this program has just started and we need to see if
		// the exe of interest is running or not.
		if( nullptr == exePocessHandle )
		{
			if( DWORD exePID = GetProcessID( programCmdLineOptions.exeName ) )
			{
				exePocessHandle = OpenProcess(SYNCHRONIZE, FALSE, exePID);
				PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has started! Cursor is locked to the primary monitor.\n" );
			}
		}
		else
		{
			const RECT primaryMonitorSize = GetPrimaryMonitorScreenRect();
			LockCursor(primaryMonitorSize);
		}
			
		// if the exe of interest's PID is valid lets check to see if it's still running
		if( exePocessHandle && FALSE == IsProcessRunning(exePocessHandle) )
		{
			UnlockCursor(oldCursorClipRect);

			bRunning = false;
		}
		else
		{
			std::this_thread::sleep_for( 
				std::chrono::milliseconds( programCmdLineOptions.msToSleep ) 
			);
		}
	}

	if( exePocessHandle )
		CloseHandle(exePocessHandle);

	PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has stopped! Cursor is unlocked.\n" );
	PrintToConsole( "\n" );

	return 0;
}