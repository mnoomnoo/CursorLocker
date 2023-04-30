#include "ProgramArgs.h"

#include <vector>
#include <algorithm>
#include <fstream>

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

bool DoesCommandArgsHaveExeConfigsOption( std::vector<std::string>& cmdArgs )
{
	for( auto iter : cmdArgs )
	{
		if( "-ec" == iter || "-exesConfig" == iter )
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

	const bool hasExeConfigs = DoesCommandArgsHaveExeConfigsOption(cmdArgs);	
	
	while( !cmdArgs.empty() )
	{
		std::string iterItem = cmdArgs.back();
		cmdArgs.pop_back();

		if(hasExeConfigs)
		{
			std::ifstream file("exesConfig.ecfg");
			if (file.is_open())
			{
				std::string line;
				while(std::getline(file, line))
				{
					if(!line.empty())
					{
						programCmdLineOptions.exeNames.push_back(line);
					}
				}
				file.close();
			}
			else
			{
				PrintToConsole( "exesConfigs.ecfg not found\n" );
				return false;
			}
		}
		else if (programCmdLineOptions.exeNames.empty())
		{
			programCmdLineOptions.exeNames.push_back(iterItem);
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