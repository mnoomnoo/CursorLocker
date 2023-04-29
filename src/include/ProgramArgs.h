#ifndef PROGRAMARGS_H
#define PROGRAMARGS_H

#include <string>


struct ProgramCmdLineOptions
{
	std::string exeName;
	unsigned int msToSleep;
	bool loadExeConfigs;

	ProgramCmdLineOptions();
};


bool ProcessProgramArgs(int argc , const char** argv, ProgramCmdLineOptions& programCmdLineOptions);

void PrintHelp();

#endif // PROGRAMARGS_H
