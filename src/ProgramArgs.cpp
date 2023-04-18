#include "ProgramArgs.h"

#include <vector>
#include <algorithm>

#include "Common.h"

constexpr const char* PROGRAM_HELP = 
#include "help.ht"
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
	for (size_t c = 1; c < argc; c++)
		cmdArgs.push_back( argv[c] );

	if( cmdArgs.empty() )
	{
		PrintToConsole( L"Not enough parameters.\n" );
		PrintToConsole( PROGRAM_HELP << "\n");
		return false;
	}

	if( DoesCommandArgsHaveHelpOption( cmdArgs ) )
	{
		PrintToConsole( PROGRAM_HELP << "\n");
		return false;
	}

	bool bProcessedProgramArgs = true;

	while( !cmdArgs.empty() )
	{
		std::wstring iterItem = cmdArgs.back();
		cmdArgs.pop_back();

		if (programCmdLineOptions.exeName.empty())
		{
			std::copy(iterItem.begin(), iterItem.end(), std::back_inserter(programCmdLineOptions.exeName));
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
		else if( L"-lmon" == iterItem )
		{

		}
	}

	return bProcessedProgramArgs;
}