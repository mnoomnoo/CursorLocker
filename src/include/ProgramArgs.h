#ifndef PROGRAMARGS_H
#define PROGRAMARGS_H

#include <string>


struct ProgramCmdLineOptions
{
	std::string exeName;
	unsigned int msToSleep;

	ProgramCmdLineOptions();
};


bool ProcessProgramArgs(int argc , const char** argv, ProgramCmdLineOptions& programCmdLineOptions);

#endif // PROGRAMARGS_H
