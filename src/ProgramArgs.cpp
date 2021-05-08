#include "ProgramArgs.h"

#include <vector>
#include <algorithm>

#include "Common.h"

const static wchar_t* PROGRAM_HELP =
	L"USAGE:\n"
	L"	CursorLocker.exe -exe \"<executable file>\" [OPTIONS]\n"
	L"\n"
	L"OPTIONS:\n"
	L"	-exe \"<executable file>\"	Locks the cursor when this exe file becomes a process or is already a process\n"
	L"	-h, -help			Print version and help info and exits\n"
	L"\n"
	L"ADVANCED OPTIONS:\n"
	L"	-sleepTime <milliseconds>	How long to sleep before a CursorLocker processing iteration. Default is 800\n"
	L"\n"
;

ProgramCmdLineOptions::ProgramCmdLineOptions()
{
	msToSleep = 800;
}

bool DoesCommandArgsHaveHelpOption( std::vector<std::wstring>& cmdArgs )
{
	for( auto iter : cmdArgs )
	{
		if( L"-h" == iter || L"-help" == iter )
			return true;
	}
	return false;
}

bool ProcessProgramArgs(int argc , wchar_t** argv, ProgramCmdLineOptions& programCmdLineOptions)
{
	std::vector<std::wstring> cmdArgs;
	for( int c = argc - 1; c >= 1; c-- )
		cmdArgs.push_back( argv[c] );

	if( DoesCommandArgsHaveHelpOption( cmdArgs ) )
	{
		PrintToConsole( PROGRAM_HELP );
		return false;
	}

	if( 2 >= argc )
	{
		PrintToConsole( L"Not enough parameters.\n\n" );
		PrintToConsole( PROGRAM_HELP );
		return false;
	}

	bool bProcessedProgramArgs = true;

	while( !cmdArgs.empty() )
	{
		std::wstring iterItem = cmdArgs.back();
		cmdArgs.pop_back();

		if( cmdArgs.empty() )
			break;

		if( L"-exe" == iterItem )
		{
			if( !cmdArgs.empty() )
			{
				std::wstring exeParam = cmdArgs.back();
				cmdArgs.pop_back();
				std::remove_copy(exeParam.begin(), exeParam.end(), std::back_inserter(programCmdLineOptions.exeName), '\"');
			}
			else
				bProcessedProgramArgs = false;
		}
		else if( L"-sleepTime" == iterItem )
		{
			if( !cmdArgs.empty() )
			{
				std::wstring exeParam = cmdArgs.back();
				cmdArgs.pop_back();

				wchar_t* charEnd = nullptr;
				const unsigned long paramToUInt = std::wcstoul( exeParam.c_str(), &charEnd, 10 );
				if( 0 != paramToUInt )
					programCmdLineOptions.msToSleep = paramToUInt;
				else
				{
					bProcessedProgramArgs = false;
					PrintToConsole( "-sleepTime parameter: " << exeParam 
						<< ", should be in milliseconds. Example: 800" << "\n\n" 
					);
				}
			}
			else
				bProcessedProgramArgs = false;
		}
	}

	return bProcessedProgramArgs;
}