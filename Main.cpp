

#include <aclapi.h>
#include <wtsapi32.h>

#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <algorithm>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const static wchar_t* PROGRAM_VERSION = L"1.0.21.5.2"; // Major-Minor-Year-Month-Day

struct ProgramCmdLineOptions
{
	std::wstring exeName;

	ProgramCmdLineOptions()
	{

	}
};

////////////////////////////////////////////////////////////////////////////////

void __PrintToConsole( const std::wstringstream& ss )
{
	std::wcout << ss.str().c_str();
	std::wcout.flush();
}

#define PrintToConsole( s ) __PrintToConsole( std::wstringstream() << s )

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

std::wstring GetProcessName( const DWORD& processID )
{
	std::wstring processName;

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
			if( processID == processes[c].ProcessId )
			{
				processName = processes[c].pProcessName;
				break;
			}
		}

		// OK we need to clean up
		WTSFreeMemory( processes );
	}

	return processName;
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

bool ProcessProgramArgs(int argc , wchar_t** argv, ProgramCmdLineOptions& programCmdLineOptions)
{
	if( 2 >= argc )
		return false;

	std::vector<std::wstring> cmdArgs;
	for( int c = argc - 1; c >= 1; c-- )
		cmdArgs.push_back( argv[c] );

	while( !cmdArgs.empty() )
	{
		std::wstring iterItem = cmdArgs.back();
		cmdArgs.pop_back();

		if( L"-exe" == iterItem && !cmdArgs.empty() )
		{
			std::wstring exeParam = cmdArgs.back();
			cmdArgs.pop_back();

			std::remove_copy(exeParam.begin(), exeParam.end(), std::back_inserter(programCmdLineOptions.exeName), '\"');
		}
	}

	return cmdArgs.empty();
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
	PrintToConsole( "CursorLock V: " << PROGRAM_VERSION << "\n" );
	PrintToConsole( "\n" );

	ProgramCmdLineOptions programCmdLineOptions;
	if( !ProcessProgramArgs(argc, argv, programCmdLineOptions) )
	{
		PrintToConsole( 
			"USAGE:\n"
			"	CursorLock.exe [OPTIONS]\n\n"
			"OPTIONS:\n"
			"	-exe \"<Executable file>\"	Locks the cursor when this exe file becomes a process\n"
			"	-h, -help			Print version and help info and exits\n"
			"\n"
		);

		return 1;
	}

	PrintToConsole( "Waiting for process: \"" << programCmdLineOptions.exeName.c_str() << "\" to start...\n" );

	DWORD exePID = 0;

	SetWindowsStation();

	RECT oldCursorClipRect;
	GetClipCursor(&oldCursorClipRect); 

	exePID = GetProcessID( programCmdLineOptions.exeName );

	bool bPrintedProcessStartedMsg = false;
	bool bRunning = true;
	while( bRunning )
	{
		if( 0 == exePID )
		{
			exePID = GetProcessID( programCmdLineOptions.exeName );
		}
			
		// if the PID is valid lets check to see if it's still running
		if( 0 != exePID )
		{
			std::wstring processName = GetProcessName( exePID );

			// if the process PID returns an empty name then lets
			// assume the process isn't running anymore. So lets quit
			if( processName.empty()  )
			{
				UnlockCursor(oldCursorClipRect);

				exePID = 0;

				bRunning = false;
			}
			else
			{
				if( !bPrintedProcessStartedMsg )
				{
					PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has started! Cursor is locked to the primary monitor.\n" );
					bPrintedProcessStartedMsg = true;
				}

				const RECT primaryMonitorSize = GetPrimaryMonitorScreenRect();
				LockCursor(primaryMonitorSize);
			}
		}
		std::this_thread::sleep_for( 
			std::chrono::milliseconds( 1000 ) 
		);
	}

	PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has stopped! Cursor is unlocked.\n" );
	PrintToConsole( "\n" );

	return 0;
}