#ifndef MONITORS_H
#define MONITORS_H

#include <Windows.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void InitMonitorAPI();

void CleanupMonitorAPI();

RECT GetPrimaryMonitorScreenRect_DPIScaled();


#endif // MONITORS_H
