

#include <aclapi.h>
#include <wtsapi32.h>

#include <wchar.h>
#include <thread>

#include "Common.h"
#include "ProgramArgs.h"
#include "Monitors.h"

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
	DWORD exePID = 0;

	if( exePID = GetProcessID( programCmdLineOptions.exeName ) )
	{
		exePocessHandle = OpenProcess(SYNCHRONIZE, FALSE, exePID);
		if(exePocessHandle) 
		{
			PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" is already running! Cursor is locked to the primary monitor.\n" );
		}
	}
	else
	{
		PrintToConsole( "Waiting for process: \"" << programCmdLineOptions.exeName.c_str() << "\" to start...\n" );
	}

	while( true )
	{
		// If the process handle is null then lets see if we can get the PID of the process name.
		// If the process name has a valid PID lets open the process handle so we can query whether it's running or not
		if( nullptr == exePocessHandle )
		{
			if( exePID = GetProcessID( programCmdLineOptions.exeName ) )
			{
				exePocessHandle = OpenProcess(SYNCHRONIZE, FALSE, exePID);
				if(exePocessHandle) 
					PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has started! Cursor is locked to the primary monitor.\n" );
			}
		}
			
		// If the process handle is not null then lets see if the process is running.
		// If the process is running lets lock the cursor to the rectangle of the primary monitor
		// Else lets un-lock the cursor and exit the program
		else if( nullptr != exePocessHandle )
		{
			if(TRUE == IsProcessRunning(exePocessHandle) )
			{
				const RECT primaryMonitorSize = GetPrimaryMonitorScreenRect();
				LockCursor(primaryMonitorSize);
			}
			else
			{
				UnlockCursor(oldCursorClipRect);
				break;
			}
		}

		std::this_thread::sleep_for( 
			std::chrono::milliseconds( programCmdLineOptions.msToSleep ) 
		);

	}

	if( exePocessHandle )
		CloseHandle(exePocessHandle);

	PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has stopped! Cursor is unlocked.\n" );
	PrintToConsole( "\n" );

	return 0;
}