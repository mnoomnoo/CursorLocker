
#include <thread>
#include <atomic>
#include <fstream>

#include "Common.h"
#include "ProgramArgs.h"
#include "Monitors.h"
#include "Processes.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

RECT oldCursorClipRect = {0,0,0,0};
HANDLE exePocessHandle = nullptr;
std::atomic<bool> runLoop;

////////////////////////////////////////////////////////////////////////////////

void LockCursor( const RECT& lpRect )
{
	ClipCursor(&lpRect);
}

void UnlockCursor( const RECT& oldCursorClipRect )
{
	ClipCursor(&oldCursorClipRect); 
}

void CleanupCursorLocker()
{
	UnlockCursor(oldCursorClipRect);
	if (exePocessHandle)
	{
		CloseHandle(exePocessHandle);
		exePocessHandle = nullptr;
	}
	CleanupMonitorAPI();
}

BOOL WINAPI ConsoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		runLoop.store(false);
		return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

int main( int argc , const char** argv )
{
	PrintToConsole( "\n" );
	PrintToConsole( "CursorLocker V: " << PROGRAM_VERSION << "\n" );
	PrintToConsole( "\n" );

	ProgramCmdLineOptions programCmdLineOptions;
	if( !ProcessProgramArgs(argc, argv, programCmdLineOptions) )
	{
		PrintHelp();
		return 1;
	}

	std::ifstream istrm("exeConfigs");
	if (!istrm.is_open())
	{
		
	}

	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	InitMonitorAPI();
	
	GetClipCursor(&oldCursorClipRect); 
	
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

	runLoop.store(true);

	TimeStamp startTime = GetTimeStamp();

	while( runLoop.load() )
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
				const RECT primaryMonitorSize = GetPrimaryMonitorScreenRect_DPIScaled();
				LockCursor(primaryMonitorSize);
			}
			else
			{
				break;
			}
		}

		std::this_thread::sleep_for( 
			std::chrono::milliseconds( programCmdLineOptions.msToSleep ) 
		);

	}

	if (exePocessHandle) 
	{
		PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\" has stopped! Cursor is unlocked.\n" );
		PrintToConsole( "\n" );

		uint64_t total_ms = GetElapsedMilliseconds(startTime, GetTimeStamp());

		const uint64_t hours = ((total_ms / 1000) / 60) / 60;
		total_ms -= hours * 1000 * 60 * 60;
		const uint64_t mins = (total_ms / 1000) / 60;
		total_ms -= mins * 1000 * 60;		
		const uint64_t secs = total_ms / 1000;
		total_ms -= secs * 1000;		
		const uint64_t ms = total_ms ;
		total_ms -= ms;

		PrintToConsole( "Process: \"" << programCmdLineOptions.exeName.c_str() << "\"" <<
			" was locked for: " << hours << " hours " << mins << " minutes "  <<
			secs << " seconds and " << ms << " ms \n" 
		);
	}

	CleanupCursorLocker();

	PrintToConsole( "\n" );

	return 0;
}