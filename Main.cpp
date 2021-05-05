

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

const static wchar_t* PROGRAM_VERSION = L"1.1.21.5.4"; // Major-Minor-Year-Month-Day

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

BOOL IsProcessRunning(DWORD pid)
{
	// Thanks: https://stackoverflow.com/questions/1591342/c-how-to-determine-if-a-windows-process-is-running
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
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
	PrintToConsole( "CursorLocker V: " << PROGRAM_VERSION << "\n" );
	PrintToConsole( "\n" );

	ProgramCmdLineOptions programCmdLineOptions;
	if( !ProcessProgramArgs(argc, argv, programCmdLineOptions) )
	{
		PrintToConsole( 
			"USAGE:\n"
			"	CursorLocker.exe [OPTIONS]\n\n"
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

	//bool bPrintedProcessStartedMsg = false;
	bool bRunning = true;
	while( bRunning )
	{
		// if the exe of interest's PID is 0 then this program has just started and we need to see if
		// the exe of interest is running or not.
		if( 0 == exePID )
		{
			exePID = GetProcessID( programCmdLineOptions.exeName );

			if( 0 != exePID )
			{
				PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has started! Cursor is locked to the primary monitor.\n" );
				const RECT primaryMonitorSize = GetPrimaryMonitorScreenRect();
				LockCursor(primaryMonitorSize);
			}
		}
			
		// if the exe of interest's PID is valid lets check to see if it's still running
		if( 0 != exePID && FALSE == IsProcessRunning(exePID) )
		{
			UnlockCursor(oldCursorClipRect);

			exePID = 0;

			bRunning = false;
		}
		std::this_thread::sleep_for( 
			std::chrono::milliseconds( 1000 ) 
		);
	}

	PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has stopped! Cursor is unlocked.\n" );
	PrintToConsole( "\n" );

	return 0;
}