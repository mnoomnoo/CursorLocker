#ifndef PROGRAMARGS_H
#define PROGRAMARGS_H

#include <string>
#include <vector>

struct ProgramCmdLineOptions
{
	std::vector<std::string> exeNames;
	unsigned int msToSleep;

	ProgramCmdLineOptions();
};


bool ProcessProgramArgs(int argc , const char** argv, ProgramCmdLineOptions& programCmdLineOptions);

void PrintHelp();

#endif // PROGRAMARGS_H
