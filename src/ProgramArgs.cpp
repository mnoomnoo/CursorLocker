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

bool DoesCommandArgsHaveHelpOption( std::vector<std::string>& cmdArgs )
{
	for( auto iter : cmdArgs )
	{
		if( "-h" == iter || "-help" == iter )
			return true;
	}
	return false;
}

void PrintHelp() 
{
	PrintToConsole( PROGRAM_HELP << "\n");
}

bool ProcessProgramArgs(int argc , const char** argv, ProgramCmdLineOptions& programCmdLineOptions)
{
	std::vector<std::string> cmdArgs;
	for (size_t c = argc-1; 0 < c; c--)
		cmdArgs.push_back( argv[c] );

	const bool hasOptions = cmdArgs.size() > 1;

	if( cmdArgs.empty() )
	{
		PrintToConsole( "Not enough parameters.\n" );
		return false;
	}

	if( DoesCommandArgsHaveHelpOption( cmdArgs ) )
	{
		return false;
	}
	
	while( !cmdArgs.empty() )
	{
		std::string iterItem = cmdArgs.back();
		cmdArgs.pop_back();

		if (programCmdLineOptions.exeName.empty())
		{
			std::copy(iterItem.begin(), iterItem.end(), std::back_inserter(programCmdLineOptions.exeName));
			continue;
		}

		if(hasOptions)
		{
			if( "-sleepTime" == iterItem )
			{
				if( !cmdArgs.empty() )
				{
					std::string exeParam = cmdArgs.back();
					cmdArgs.pop_back();

					const unsigned long paramToUInt = std::stoul( exeParam.c_str(), nullptr, 10 );
					if( 0 != paramToUInt )
						programCmdLineOptions.msToSleep = paramToUInt;
					else
					{
						PrintToConsole( "-sleepTime parameter: " << exeParam << ", should be in milliseconds. Example: 800" << "\n" );
						return false;
					}
				}
				else
				{
					PrintToConsole( "-sleepTime is missing a parameter\n" );
					return false;
				}
			}
			else
			{
				PrintToConsole( "Invalid parameter: " << iterItem << "\n" );
				return false;				
			}
		}
	}

	return true;
}