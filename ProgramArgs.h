#pragma once

#include <string>


struct ProgramCmdLineOptions
{
	std::wstring exeName;
	unsigned int msToSleep;

	ProgramCmdLineOptions();
};


bool ProcessProgramArgs(int argc , wchar_t** argv, ProgramCmdLineOptions& programCmdLineOptions);