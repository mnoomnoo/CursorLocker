
#include "Processes.h"

#include <wtsapi32.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


DWORD GetProcessID( const std::string& processName )
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
			if( processName == std::string(processes[c].pProcessName) )
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
	if(!process)
		return FALSE;

	// Thanks: https://stackoverflow.com/questions/1591342/c-how-to-determine-if-a-windows-process-is-running
	const DWORD ret = WaitForSingleObject(process, 0);    
	return ret == WAIT_TIMEOUT;
}
